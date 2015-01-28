#include "BTreeNode.h"

using namespace std;

int getMaxKeys();

// Constructor: Allocates memory for leaf node by initializing
// buffer (pads the buffer).
BTLeafNode::BTLeafNode()
{
	memset(buffer, 0, PageFile::PAGE_SIZE);
}

/*
 * Read the content of the node from the page pid in the PageFile pf.
 * @param pid[IN] the PageId to read
 * @param pf[IN] PageFile to read from
 * @return 0 if successful. Return an error code if there is an error.
 * 
 */

RC BTLeafNode::read(PageId pid, const PageFile& pf)
{ 
	memset(buffer, 0, PageFile::PAGE_SIZE);
	return pf.read(pid, buffer);
}
    
/*
 * Write the content of the node to the page pid in the PageFile pf.
 * @param pid[IN] the PageId to write to
 * @param pf[IN] PageFile to write to
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::write(PageId pid, PageFile& pf)
{ 
	return pf.write(pid, buffer); 
}

/*
 * Return the number of keys stored in the node.
 * @return the number of keys in the node
 */
int BTLeafNode::getKeyCount()
{
	// I completely misunderstood what this was asking for
	// I thought it initially asked for total number of keys
	// but it's just the keys in the specific node
	// looks to me as though getRecordCount does just that
	// but they made it local to RecordFile, so we can't use it
	int count = 0 ;

	// the first four bytes of a page contains # records in the page
	// I am not convinced that this will return what we want.
	memcpy(&count, &buffer, sizeof(int));
	return count;
	
	//return 0; 
}

/*
 * Insert a (key, rid) pair to the node.
 * @param key[IN] the key to insert
 * @param rid[IN] the RecordId to insert
 * @return 0 if successful. Return an error code if the node is full.
 */
RC BTLeafNode::insert(int key, const RecordId& rid)
{ 
	RC rc;
	RecordId ri; 
	int rkey;

	if (isFull())
		return -1;
	
	int count = getKeyCount();
	int attempt = count;
	int *buffPtr = (int*) buffer;
	
	
	// Checks to see if key being inserted
	// belongs in current node.
	for (int i = 0; i < count; i++)
	{
		// Attempting to read each entry in the node.
		if (rc = readEntry(i, rkey, ri) < 0)
			return rc; // Return error code.
		if (rkey >= key) // Checking if entry key is greater than key to insert.
		{
			attempt = i;
			break;
		}
	}
	
	if (attempt != count) // If for-loop terminated early (i.e., could not insert)
		return shift(attempt);
	if (rc = buffInsert(key, rid, attempt) < 0)
		return rc;
		
	keyIncrement(true);
	
	return 0; 
}

/*
 * Insert the (key, rid) pair to the node
 * and split the node half and half with sibling.
 * The first key of the sibling node is returned in siblingKey.
 * @param key[IN] the key to insert.
 * @param rid[IN] the RecordId to insert.
 * @param sibling[IN] the sibling node to split with. This node MUST be EMPTY when this function is called.
 * @param siblingKey[OUT] the first key in the sibling node after split.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::insertAndSplit(int key, const RecordId& rid, 
                              BTLeafNode& sibling, int& siblingKey)
{ 
	RC rc;
	RecordId ri;
	int mov = getKeyCount() / 2;
	int notMov = getKeyCount() - mov;
	int keys = getKeyCount();
	int rkey;
	
	if (rc = insert(key, rid) < 0)
		return rc;
	
	for (int i = keys - 1; i >= notMov; i--)
	{
		if (rc = readEntry(i, rkey, ri) < 0)
			return rc;
		if (rc = sibling.insert(rkey, ri) < 0)
			return rc;
		if (rc = del(i) < 0)
			return rc;
		keyIncrement(false);
	}
	
	if (rc = sibling.readEntry(0, siblingKey, ri) < 0)
		return rc;
	
	return 0; 
}

/*
 * Find the entry whose key value is larger than or equal to searchKey
 * and output the eid (entry number) whose key value >= searchKey.
 * Remeber that all keys inside a B+tree node should be kept sorted.
 * @param searchKey[IN] the key to search for
 * @param eid[OUT] the entry number that contains a key larger than or equalty to searchKey
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::locate(int searchKey, int& eid)
{ 
	RC rc;
	RecordId ri;
	int count = getKeyCount();
	int currKey;
	
	if (count <= 0)
		return -1;
	
	for (int i = 0; i < count; i++)
	{
		if (rc = readEntry(i, currKey, ri) < 0)
			return rc;
		if (currKey >= searchKey)
		{	
			eid = i;
			return 0;
		}
	}
	
	return -1;
}

/*
 * Read the (key, rid) pair from the eid entry.
 * @param eid[IN] the entry number to read the (key, rid) pair from
 * @param key[OUT] the key from the entry
 * @param rid[OUT] the RecordId from the entry
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::readEntry(int eid, int& key, RecordId& rid)
{ 
	int count = getKeyCount();
	int offset = eid * 3 + 1;
	int *buffPtr = (int*) buffer;
	
	if (count <= eid || eid < 0)
		return -1;

	rid.pid = *(buffPtr + offset + 1);
	rid.sid = *(buffPtr + offset + 2);
	key = *(buffPtr + offset);	
	
	return 0; 
}

/*
 * Return the pid of the next slibling node.
 * @return the PageId of the next sibling node 
 */
PageId BTLeafNode::getNextNodePtr()
{ 
	int *buffPtr = (int*) buffer;
	
	//
	return *(buffPtr + 255); 
}

/*
 * Set the pid of the next slibling node.
 * @param pid[IN] the PageId of the next sibling node 
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::setNextNodePtr(PageId pid)
{ 
	int *bufPtr = (int*) buffer;
	*(bufPtr + 255) = pid;
	
	return 0;
}


RC BTLeafNode::buffInsert(const int key, const RecordId rid, const int eid)
{	
	int *buffPtr = (int*) buffer;
	int offset = eid * 3 + 1;
	
	if (eid < 0)
		return -1;
	if (isFull())
		return -1;
		
	*(buffPtr + offset + 1) = rid.pid;
	*(buffPtr + offset + 2) = rid.sid;
	*(buffPtr + offset) = key;
	
	return 0;
}
	
RC BTLeafNode::del(const int eid)
{
	RC rc;
	int offset = eid * 3 + 1;
	int *buffPtr = (int*) buffer;
	
	*(buffPtr + offset + 1) = -1;
	*(buffPtr + offset + 2) = -1;
	*(buffPtr + offset) = -1;
	
	return 0;	
}

// Input: entry number for record.
RC BTLeafNode::shift(const int eid)
{
	RC rc;
	RecordId ri;
	int rkey;
	int *buffPtr = (int*) buffer;
	
	if (eid < 0)
		return -1;
	
	// Starting with the last node,
	// Check that 
	for (int i = getKeyCount() - 1; i >= eid; i--)
	{
		if (rc = readEntry(i, rkey, ri) < 0)
			return rc;
		if (rc = buffInsert(rkey, ri, i + 1) < 0)
			return rc;
	}
	
	return 0;
}
	
bool BTLeafNode::isFull()
{
	if (MAX_RECORDS <= getKeyCount())
		return true;
	return false;
}
	
void BTLeafNode::keyIncrement(bool increment)
{
	int *buffPtr = (int*) buffer;
	if (increment)
		buffPtr[0]++;
	else
		buffPtr[0]--;	
}


BTNonLeafNode::BTNonLeafNode() 
{
	memset(buffer, 0, PageFile::PAGE_SIZE);
}
/*
 * Read the content of the node from the page pid in the PageFile pf.
 * @param pid[IN] the PageId to read
 * @param pf[IN] PageFile to read from
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::read(PageId pid, const PageFile& pf)
{ 
	return pf.read(pid, buffer);
}
    
/*
 * Write the content of the node to the page pid in the PageFile pf.
 * @param pid[IN] the PageId to write to
 * @param pf[IN] PageFile to write to
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::write(PageId pid, PageFile& pf)
{ 
	return pf.write(pid, buffer); 
}

/*
 * Return the number of keys stored in the node.
 * @return the number of keys in the node
 */
int BTNonLeafNode::getKeyCount()
{ 
	int count = 0;
	
	memcpy(&count, &buffer, sizeof(int));
	return count; 
}

/*
 * Insert a (key, pid) pair to the node.
 * @param key[IN] the key to insert
 * @param pid[IN] the PageId to insert
 * @return 0 if successful. Return an error code if the node is full.
 */
RC BTNonLeafNode::insert(int key, PageId pid)
{
	RC rc;
	int keys = getKeyCount();
	int *buffPtr = (int*) buffer;
	
	if (isFull())
		return -1;
	for (int i = 0; i < keys; ++i)
	{
		if (key < *((buffPtr + 2) + 2 * i))
		{
			shift(i);
			break;
		}
	}
	
	*((buffPtr + 2) + 2 * i) = key;
	*((buffPtr + 3) + 2 * i) = pid;
	keyIncrement(true);
	
	return 0; 
}

/*
 * Insert the (key, pid) pair to the node
 * and split the node half and half with sibling.
 * The middle key after the split is returned in midKey.
 * @param key[IN] the key to insert
 * @param pid[IN] the PageId to insert
 * @param sibling[IN] the sibling node to split with. This node MUST be empty when this function is called.
 * @param midKey[OUT] the key in the middle after the split. This key should be inserted to the parent node.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::insertAndSplit(int key, PageId pid, BTNonLeafNode& sibling, int& midKey)
{ 
	RC rc;
	RecordId ri;
	int mov = getKeyCount() / 2;
	int notMov = getKeyCount() - mov;
	int keys = getKeyCount();
	int rkey;
	
	for (int i = keys - 1; i >= notMov; i--)
	{
		if (rc = sibling.insert(rkey, pid) < 0)
			return rc;
		if (rc = del(i) < 0)
			return rc;
		keyIncrement(false);
	}
	
	midKey = mov;
	del(mov);
	
	if (key < midKey)
		insert(key, pid);
	else
		sibling.insert(key, pid);
		
	return 0;
}

/*
 * Given the searchKey, find the child-node pointer to follow and
 * output it in pid.
 * @param searchKey[IN] the searchKey that is being looked up.
 * @param pid[OUT] the pointer to the child node to follow.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::locateChildPtr(int searchKey, PageId& pid)
{ 
	int keys = getKeyCount();
	int *buffPtr = (int*) buffer;
	
	if (keys <= 0)
		return -1;
	
	for (int i = 0; i < keys; i++)
	{
		if (searchKey < *((buffPtr + 2) + 2 * i))
		{
			pid = *((buffPtr + 1) + 2 * i);
			return 0;
		}
	}
	
	pid = *((buffPtr + 1) + 2 * i);
	return 0; 
}

/*
 * Initialize the root node with (pid1, key, pid2).
 * @param pid1[IN] the first PageId to insert
 * @param key[IN] the key that should be inserted between the two PageIds
 * @param pid2[IN] the PageId to insert behind the key
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::initializeRoot(PageId pid1, int key, PageId pid2)
{ 
	int *buffPtr = (int*) buffer;
	
	*(buffPtr + 1) = pid1;
	*(buffPtr + 2) = key;
	*(buffPtr + 3) = pid2;
	keyIncrement(true);
	
	return 0; 
}

bool BTNonLeafNode::isFull()
{
	if (getKeyCount() >= MAX_RECORDS_NONLEAF)
		return true;
	else
		return false;
}

RC BTNonLeafNode::shift(const int pos)
{
	RC rc;
	int offset;
	int keys = getKeyCount();
	int *buffPtr = (int*) buffer;
	
	for (int i = keys - 1; i >= pos; i--)
	{
		offset = pos * 2 + 2;
		*(buffPtr + offset + 2) = *(buffPtr + offset);
		*(buffPtr + offset + 3) = *(buffPtr + offset + 1);
	}
	
	return 0;
}

RC BTNonLeafNode::del(const int pos)
{
	RC rc;
	int offset;
	int keys = getKeyCount();
	int *buffPtr = (int*) buffer;
	
	
	if (pos >= keys)
		return -1;
		
	offset = pos * 2;
	*(buffPtr + offset) = -1;
	*(buffPtr + offset + 1) = -1;
	return 0;
}

void BTNonLeafNode::keyIncrement(bool inc)
{
	int *buffPtr = (int*) buffer;
	
	if (inc)
		buffPtr[0]++;
	else
		buffPtr[0]--;
}