//--------------------------------------------------------
// File: environ.h
//     (part of OMNeT++ simulation of CPMnet)
// Author: Weihai Yu
//--------------------------------------------------------

#ifndef __ENVIRON_H
#define __ENVIRON_H

#include "cpmcommon.h"

class Cpm;

class Environ {
public:
	Environ ( Cpm *a_cpm ):cpm (a_cpm){};
	Environ* set ( xmlDocPtr newdoc = NULL, xmlNodePtr elm = NULL );
	Environ* set ( xmlNodePtr elm = NULL );
	Environ* importSet ( xmlNodePtr elm = NULL );
	Environ* reset ();

	Environ* append (xmlNodePtr elm, const char *stat);
	Environ* importAppend (xmlNodePtr elm, const char *stat, bool deep);
	Status getStatus (xmlNodePtr elm = NULL);
	Environ* setStatus (xmlNodePtr elm, Status stat);
	Environ* setStatus (const char *actname, Status stat);

	Environ* branchDone ();
	Environ* merge ( int matchkey );
	
	long msgSize ();

	Cpm *cpm;
	xmlDocPtr doc;
	xmlNodePtr domelm; // the <e> element
	xmlNodePtr currelm; // the current active element

private:
	xmlNodePtr findCurrElm (xmlNodePtr elm);
		
	long msgSize (xmlNodePtr elm);
	long sumChildMsgSize (xmlNodePtr parent);
	
	// default size of messages for an activity in env is 50 bytes
	static const long ACT_ENV_MSG_SIZE = 50;
};

#endif
