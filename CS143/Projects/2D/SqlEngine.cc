/**
 * Copyright (C) 2008 by The Regents of the University of California
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL).
 *
 * @author Junghoo "John" Cho <cho AT cs.ucla.edu>
 * @date 3/24/2008
 */

#include <cstdio>
#include <iostream>
#include <fstream>
#include "Bruinbase.h"
#include "SqlEngine.h"
#include "BTreeIndex.h"

using namespace std;

// external functions and variables for load file and sql command parsing
extern FILE* sqlin;
int sqlparse(void);


RC SqlEngine::run(FILE* commandline)
{
    fprintf(stdout, "Bruinbase> ");
    
    // set the command line input and start parsing user input
    sqlin = commandline;
    sqlparse();  // sqlparse() is defined in SqlParser.tab.c generated from
    // SqlParser.y by bison (bison is GNU equivalent of yacc)
    return 0;
}

RC SqlEngine::select(int attr, const string& table, const vector<SelCond>& cond)
{
    RecordFile rf;   // RecordFile containing the table
    RecordId   rid;  // record cursor for table scanning
    
    RC     rc;
    int    key;
    string value;
    int    count;
    int    diff;
    BTreeIndex idx;
    bool hasIdx = false;
    bool isLast = true;
  
    // open the table file
    if ((rc = rf.open(table + ".tbl", 'r')) < 0)
    {
        fprintf(stderr, "Error: table %s does not exist\n", table.c_str());
        return rc;
    }
    
   if ((rc = idx.open(table + ".idx", 'r') ) == 0)
   {
	hasIdx = true;
 	int lower = -1, upper = -1, testVal;
	bool found = false, haveLower = false, haveUpper = false;
	count = 0;

	for (int i = 0; i < cond.size(); i++)
	{
	   if (cond[i].attr == 1 && cond[i].comp != SelCond::NE)
	   {
		found = true;
		testVal = atoi(cond[k].value);

		switch (cond[i].comp)
		{
		   case SelCond::GT:
		   case SelCond::GE: haveLower = true; if (testVal > lower) lower = testVal; break;
		   case SelCond::LT:
		   case SelCond::LE: haveUpper = true; if (testVal < upper) upper = testVal; break;
		   case SelCond::EQ: haveLower = true; haveUpper = true; upper = testVal; lower = testVal; break;
		}		
	   }
	}
	
	// check for index
	if (!found)
		goto no_index;
	
	IndexCursor cursor;
	if (!haveLower)
	{
	   idx.locate(-1, cursor);
	   while ((idx.readFoward(cursor, key, rid) == 0) && (key <= upper))
	   {
		// indexed count
		if (attr == 4 && key != upper)
		{
		   count++;
		   continue;
		}

		if ((rc = rf.read(rid, key, value)) < 0)
		{
		   fprintf(stderr, "Error reading tuple from %s\n", table.c_str());
		   goto exit_select;
		}
		
		// find diff between tuple val and cond val
		for (unsigned i = 0; i < cond.size(); i++)
		{
		   switch (cond[i].attr)
		   {
			case 1: diff = key - atoi(cond[i].value); break;
			case 2:	diff = strcmp(value.c_str(), cond[i].value); break;
		   }
		
		   // if any condition not met, skip tuple
		   switch (cond[i].comp)
		   {
			case SelCond::EQ: if (diff != 0)  goto next_lt_tuple; break;
			case SelCond::NE: if (diff == 0) goto next_lt_tuple; break;
			case SelCond::GT: if (diff <= 0) goto next_lt_tuple; break;
			case SelCond::LT: if (diff >= 0) goto next_lt_tuple; break;
			case SelCond::GE: if (diff < 0) goto next_lt_tuple; break;
			case SelCond::LE: if (diff > 0) goto next_lt_tuple; break;
		   }
		}
		// condition must have been met, increment tuple count
		count++;

		// print tuple
		switch (attr)
		{
		   case 1: fprintf(stdout, "%d\n", key); break;			    // SELECT key
		   case 2: fprintf(stdout, "%s\n", value.c_str()); break; 	    // SELECT value
		   case 3: fprintf(stdout, "%d '%s'\n", key, value.c_str()); break; // SELECT *		
		}
		
		next_lt_tuple:
		int workDammit;
	   }

	}
 	else if(!haveUpper)
	{
	   idx.locate(lower, cursor);
	   
	   while (idx.readForward(cursor, key, rid) == 0 || isLast)
	   {
		if (cursor.pid == 0)
			isLast = false;
		else
			isLast = true;

		// indexed count
		if (attr == 4)
		{
		   count++;
		   continue;
		}
		
		if ((rc = rf.read(rid, key, value)) < 0)
		{
		   fprintf(stderr, "Error reading tuple from %s\n", table.c_str());
		   goto exit_select;
		}
		
		//  compute diff between tuple val and cond val
		for (unsigned i = 0; i < cond.size(); i++)
		{
		   switch (cond[i].attr)
		   {
			case 1: diff = key - atoi(cond[i].value); break;
			case 2: diff = strcmp(value.c_str(), cond[i].value); break;
		   }

		   // if any condition not met, skip tuple
		   switch (cond[i].comp)
		   {
			case SelCond::EQ: if (diff != 0) goto next_gt_tuple; break;
			case SelCond::NE: if (diff == 0) goto next_gt_tuple; break;
			case SelCond::GT: if (diff <= 0) goto next_gt_tuple; break;
			case SelCond::LT: if (diff >= 0) goto next_gt_tuple; break;
			case SelCond::GE: if (diff < 0) goto next_gt_tuple; break;
			case SelCond::LE: if (diff > 0) goto next_gt_tuple; break;
		   }
		}
		
		// condition must have been met, increment tuple count
		count++;

		// print tuple
		switch (attr)
		{
		   case 1: fprintf(stdout, "%d\n", key); break; 		    // SELECT key
		   case 2: fprintf(stdout, "%s\n", value.c_str()); break; 	    // SELECT value
		   case 3: fprintf(stdout, "%d '%s'\n", key, value.c_str()); break; // SELECT *
		}
		
		next_gt_tuple:
		int workDammit;
	   }
	}
	else
	{
	   isLast = true;
	   idx.locate(lower, cursor);
	
	   while ((idx.readForward(cursor, key, rid) == 0 || isLast) && (key <= upper))
	   {
		if (cursor.pid == 0)
			isLast = false;
		else
			isLast = true;

		if ((rc = rf.read(rid, key, value)) < 0)
		{
			fprintf(stderr, "Error reading tuple from %s\n", table.c_str());
			goto exit_select;
		}

		// compute diff between tuple val and cond val
		for (unsigned i = 0; i < cond.size(); i++)
		{
		   switch (cond[i].attr)
		   {
			case 1: diff = key - atoi(cond[i].value); break;
			case 2: diff = strcmp(value.c_str(), cond[i].value); break;
		   }

		   // if any condition not met, skip tuple
		   switch (cond[i].comp)
		   {
			case SelCond::EQ: if (diff != 0) goto next_eq_tuple; break;
			case SelCond::NE: if (diff == 0) goto next_eq_tuple; break;
			case SelCond::GT: if (diff <= 0) goto next_eq_tuple; break;
			case SelCond::LT: if (diff >= 0) goto next_eq_tuple; break;
			case SelCond::GE: if (diff < 0) goto next_eq_tuple; break;
			case SelCond::LE: if (diff > 0) goto next_eq_tuple; break;
		   }
		}
		// condition must have been met, increment tuple count
		count++;

		// print tuple
		switch (attr)
		{
			case 1: fprintf(stdout, "%d\n", key); break; 			 // SELECT key
			case 2: fprintf(stdout, "%s\n", value.c_str()); break; 		 // SELECT value
			case 3: fprintf(stdout, "%d '%s'\n", key, value.c_str()); break; // SELECT *
		}

		next_eq_tuple:
		int workDammit;
	   }
	}

	if (attr == 4)
	   fprintf(stdout, "%d\n", count);

	rc = 0;
	idx.close();
   }
   else
   {
	no_index:	

 	// scan the table file from the beginning
	rid.pid = rid.sid = 0;
	count = 0;
	
	if (hasIdx)
	{
	   if (attr == 4)
	   {	
		IndexCursor cursor;
		idx.locate(-1, cursor);

		while (idx.readForward(cursor, key, rid) == 0 || isLast)
		{
		   if (cursor.pid == 0)
			isLast = false;
		   else
			isLast = true;
		   count++;
		}
		
		fprintf(stdout, "%d\n", count);
		goto exit_select;
	   }
	   idx.close();
	}

    while (rid < rf.endRid())
    {
        // read the tuple
        if ((rc = rf.read(rid, key, value)) < 0)
        {
            fprintf(stderr, "Error: while reading a tuple from table %s\n", table.c_str());
            goto exit_select;
        }
        
        // check the conditions on the tuple
        for (unsigned i = 0; i < cond.size(); i++)
        {
            // compute the difference between the tuple value and the condition value
            switch (cond[i].attr)
            {
                case 1: diff = key - atoi(cond[i].value); break;
                case 2: diff = strcmp(value.c_str(), cond[i].value); break;
            }
            
            // skip the tuple if any condition is not met
            switch (cond[i].comp)
            {
                case SelCond::EQ:
                    if (diff != 0) goto next_tuple;
                    break;
                case SelCond::NE:
                    if (diff == 0) goto next_tuple;
                    break;
                case SelCond::GT:
                    if (diff <= 0) goto next_tuple;
                    break;
                case SelCond::LT:
                    if (diff >= 0) goto next_tuple;
                    break;
                case SelCond::GE:
                    if (diff < 0) goto next_tuple;
                    break;
                case SelCond::LE:
                    if (diff > 0) goto next_tuple;
                    break;
            }
        }
        
        // the condition is met for the tuple.
        // increase matching tuple counter
        count++;
        
        // print the tuple
        switch (attr)
        {
            case 1:  // SELECT key
                fprintf(stdout, "%d\n", key);
                break;
            case 2:  // SELECT value
                fprintf(stdout, "%s\n", value.c_str());
                break;
            case 3:  // SELECT *
                fprintf(stdout, "%d '%s'\n", key, value.c_str());
                break;
        }
        
        // move to the next tuple
    	next_tuple:
        ++rid;
    }
    
    // print matching tuple count if "select count(*)"
    if (attr == 4) 
	fprintf(stdout, "%d\n", count);
    rc = 0;
    
    // close the table file and return
    exit_select:
    rf.close();
    return rc;
}


RC SqlEngine::load(const string& table, const string& loadfile, bool index)
{
    ifstream in;
    RecordFile rf;   // RecordFile containing the table
    RecordId   rid;  // record cursor for table scanning
    BTreeIndex idx;
    RC     rc;
    int    key;
    string value;
    string line;
    
    // open table, display error if fail
    if ( (rc = rf.open(table + ".tbl", 'w') ) != 0 )
    {
        fprintf(stderr, "Error: file %s does not exist.\n", table.c_str());
        
        return rc;
    }
    
    // open index file
    if ( index )
    {
        printf("Loading index...\n");
        
        if ( (rc = idx.open(table + ".idx", 'w') ) < 0)
        {
            fprintf(stderr, "Error: load index of table %s failed.\n", table.c_str());
            return rc;
        }
    }
    
    // open input file
    in.open( loadfile.c_str() );
    
    // if open successfully
    if ( in.is_open() )
    {
        // while it contains stuff
        while ( in )
        {
            // pick a line
            getline(in, line, '\n');
            
            // attempt to parse line
            if ( (rc = parseLoadLine(line, key, value)) != 0 )
                break;
            if ( (rc = rf.append(key, value, rid)) != 0 )
                break;
            if ( (rc = idx.insert(key, rid)) != 0)
                break;
        }
    }
    else fprintf(stderr, "Error: input file could not be opened.\n");
    
    // close and return
    in.close();
    idx.close();
    rf.close();
    return rc;
}

RC SqlEngine::parseLoadLine(const string& line, int& key, string& value)
{
    const char *s;
    char        c;
    string::size_type loc;
    
    // ignore beginning white spaces
    c = *(s = line.c_str());
    while (c == ' ' || c == '\t') { c = *++s; }
    
    // get the integer key value
    key = atoi(s);
    
    // look for comma
    s = strchr(s, ',');
    if (s == NULL) { return RC_INVALID_FILE_FORMAT; }
    
    // ignore white spaces
    do { c = *++s; } while (c == ' ' || c == '\t');
    
    // if there is nothing left, set the value to empty string
    if (c == 0) { 
        value.erase();
        return 0;
    }
    
    // is the value field delimited by ' or "?
    if (c == '\'' || c == '"') {
        s++;
    } else {
        c = '\n';
    }
    
    // get the value string
    value.assign(s);
    loc = value.find(c, 0);
    if (loc != string::npos) { value.erase(loc); }
    
    return 0;
}
