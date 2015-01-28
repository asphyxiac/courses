/*
 * Copyright (C) 2008 by The Regents of the University of California
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL).
 *
 * @author Junghoo "John" Cho <cho AT cs.ucla.edu>
 * @date 3/24/2008
 */
 
#include "BTreeIndex.h"
#include "BTreeNode.h"

using namespace std;

/*
 * BTreeIndex constructor
 */
BTreeIndex::BTreeIndex()
{
    rootPid = -1;
	treeHeight = 0;
}

/*
 * Open the index file in read or write mode.
 * Under 'w' mode, the index file should be created if it does not exist.
 * @param indexname[IN] the name of the index file
 * @param mode[IN] 'r' for read, 'w' for write
 * @return error code. 0 if no error
 */
RC BTreeIndex::open(const string& indexname, char mode)
{
	RC r = pf.open(indexname, mode);
	return rc;
	
	// I think we should be copying the rootPid info
	// in if it's opened in read only mode, right?
	// Maybe use a switch statement?
/*
	RC rc;
	char buffer[PageFile::PAGE_SIZE];
	int * buffPtr = (int*) buffer;
	
	if ((rc = pf.open(indexname, mode)) < 0)
		return rc;
	
	if (pf.endPid() == 0)
	{
		rootPid = -1;
		treeHeight = 0;
		buffPtr[0] = rootPid;
		buffPtr[1] = treeHeight;
		pf.write(0, buffer);
	}
	else
	{	
		pf.read(0, buffer);
		rootPid = buffPtr[0];
		treeHeight = buffPtr[1];
	}

    return 0;
	*/
}

/*
 * Close the index file.
 * @return error code. 0 if no error
 */
RC BTreeIndex::close()
{
	RC rc = pf.close();
	
	return rc;

//	RC rc;
//	char buffer[PageFile::PAGE_SIZE];
//	int * buffPtr = (int*) buffer;
//	buffPtr[0] = rootPid;
//	buffPtr[1] = treeHeight;
//	pf.write(0, buffer);
//	
//	if ((rc = pf.close()) < 0)
//		return rc;
//	
//   return 0;
}

/*
 * Insert (key, RecordId) pair to the index.
 * @param key[IN] the key for the value inserted into the index
 * @param rid[IN] the RecordId for the record being inserted into the index
 * @return error code. 0 if no error
 */
RC BTreeIndex::insert(int key, const RecordId& rid)
{
	PageId siblingPid;
	int newRootKey;
	
	if (treeHeight == 0)
	{	
		root(key, rid);
		return 0;
	}
	
	newRootKey = inserter(key, rid, 1, rootPid, siblingPid);
	if ( newRootKey != 0)
	{
		BTNonLeafNode * newRoot = new BTNonLeafNode;
		newRoot -> initializeRoot(rootPid, newRootKey, siblingPid);
		rootPid = pf.endPid();
		newRoot -> write(rootPid, pf);
		treeHeight++;
	}
	
	fprintf(stderr, "newRootKey = %d\n", newRootKey);
	fprintf(stderr, "treeHeight = %d\n", treeHeight);

    return 0;
}

/*
 * Find the leaf-node index entry whose key value is larger than or 
 * equal to searchKey, and output the location of the entry in IndexCursor.
 * IndexCursor is a "pointer" to a B+tree leaf-node entry consisting of
 * the PageId of the node and the SlotID of the index entry.
 * Note that, for range queries, we need to scan the B+tree leaf nodes.
 * For example, if the query is "key > 1000", we should scan the leaf
 * nodes starting with the key value 1000. For this reason,
 * it is better to return the location of the leaf node entry 
 * for a given searchKey, instead of returning the RecordId
 * associated with the searchKey directly.
 * Once the location of the index entry is identified and returned 
 * from this function, you should call readForward() to retrieve the
 * actual (key, rid) pair from the index.
 * @param key[IN] the key to find.
 * @param cursor[OUT] the cursor pointing to the first index entry
 *                    with the key value.
 * @return error code. 0 if no error.
 */
RC BTreeIndex::locate(int searchKey, IndexCursor& cursor)
{
	//return BTLeafNode::locate(searchKey, &cursor);

	BTNonLeafNode * tmpNonLeafNode; 
	PageId tmpPid = rootPid;

	for (int i = 1; i < treeHeight; i++)
	{
		tmpNonLeafNode = new BTNonLeafNode;
		tmpNonLeafNode -> read(tmpPid, pf);
		tmpNonLeafNode -> locateChildPtr(searchKey, tmpPid);
		delete tmpNonLeafNode;
	}

	BTLeafNode *tmpLeafNode = new BTLeafNode;
	tmpLeafNode -> read(tmpPid, pf);
	bool empty;
	
	do 
	{
		empty = false;
		if((tmpLeafNode -> locate(searchKey, cursor.eid)) < 0)
		{
			tmpPid = tmpLeafNode -> getNextNodePtr();
			
			if(tmpPid == -1)
				return -1;
				
			tmpLeafNode -> read(tmpPid, pf);
			empty = true;
		}
	} while (empty);
	
	cursor.pid = tmpPid;
	delete tmpLeafNode;
	
    return 0;
}

/*
 * Read the (key, rid) pair at the location specified by the index cursor,
 * and move foward the cursor to the next entry.
 * @param cursor[IN/OUT] the cursor pointing to an leaf-node index entry in the b+tree
 * @param key[OUT] the key stored at the index cursor location.
 * @param rid[OUT] the RecordId stored at the index cursor location.
 * @return error code. 0 if no error
 */
RC BTreeIndex::readForward(IndexCursor& cursor, int& key, RecordId& rid)
{
	RC rc;
	BTLeafNode * leafNode = new BTLeafNode;
	leafNode -> read(cursor.pid, pf);
	
	if(rc = (leafNode -> readEntry(cursor.eid, key, rid)) < 0) 
		return rc;
		
	// if eid is last entry in node
	if(cursor.eid == (leafNode -> getKeyCount()) - 1) 
	{
		cursor.pid = leafNode -> getNextNodePtr();
		cursor.eid = 0;
	} // eid not last entry in node
	else 
		cursor.eid++;
	
	delete leafNode;
	
	return 0;
}

int BTreeIndex::inserter(const int key, const RecordId &rid, int n, PageId pid, PageId &siblingPid)
{
	// != treeHeight => in the NonLeafNode
	if(n != treeHeight)
	{
		PageId tmpPid;
		BTNonLeafNode *tmpNode = new BTNonLeafNode;
		tmpNode -> read(pid, pf);
		tmpNode -> locateChildPtr(key, tmpPid);
		int siblingKey;
		if(siblingKey = insertionHelper(key, rid, n+1, tmpPid, siblingPid) != 0)
		{
			if((tmpNode -> insert(siblingKey, siblingPid)) < 0) 
			{
				int midKey;
				BTNonLeafNode *siblingNode = new BTNonLeafNode;
				tmpNode -> insertAndSplit (siblingKey, siblingPid, *siblingNode, midKey);
				siblingPid = pf.endPid();
				siblingNode -> write(siblingPid, pf);
				tmpNode -> write(pid, pf);

				return midKey;
			}
			else 
			{
				tmpNode -> write(pid, pf);
				return 0;
			}
		}

	}
	// Equal treeHeight => in LeafNode; insert key and rid in leafNode; check for overflow
	// If overflow, return siblingKey to insert in parentNode, otherwise return 0.
	else if (n == treeHeight)
	{
		BTLeafNode *tmpNode = new BTLeafNode;
		tmpNode -> read(pid, pf);
		if((tmpNode -> insert(key, rid)) < 0) 
		{
			BTLeafNode *siblingNode = new BTLeafNode;
			int siblingKey;
			tmpNode -> insertAndSplit (key, rid, *siblingNode, siblingKey);
			siblingPid = pf.endPid();
			tmpNode -> setNextNodePtr(siblingPid);
			siblingNode -> write(siblingPid, pf);
			tmpNode -> write (pid, pf);
			// Function returns before releasing memory
			// Shouldn't there be a call to delete?
			return siblingKey;
		}
		else 
		{
			tmpNode -> write(pid, pf);
			return 0;
		}
	}
}

RC BTreeIndex::root(const int key, const RecordId &rid)
{
	BTNonLeafNode * root = new BTNonLeafNode;
	BTLeafNode * firstNode = new BTLeafNode;
	BTLeafNode * secondNode = new BTLeafNode;	
	
	// Not necessary for root to be a nonleaf node.
	// It's easier if first root is just a leaf node, and then
	// can insert/split and create new root when adding
	// parent nodes.
	rootPid = pf.endPid();
	secondNode->insert(key, rid);
	root->initializeRoot(2, key, 3);
	firstNode->setNextNodePtr(3);
	root->write(rootPid, pf);

	firstNode->write(2, pf);
	secondNode->write(3, pf);
	
	treeHeight += 2;
}