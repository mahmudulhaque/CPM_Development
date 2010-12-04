//-------------------------------------------------------------
// File: cont.h
//     (part of OMNeT++ simulation of CPMnet)
// Author: Weihai Yu
//-------------------------------------------------------------

#ifndef __CONT_H
#define __CONT_H

#include "cpmcommon.h"

class Cont {
	public:
		Cont ( xmlDocPtr cpmdoc = NULL, xmlNodePtr elm = NULL );
		Cont* set ( xmlDocPtr cpmdoc = NULL, xmlNodePtr elm = NULL );
		Cont* set ( xmlNodePtr elm = NULL );
		Cont* reset ();
		Cont* push ( xmlNodePtr elm );
		xmlNodePtr pop ();
		Cont* trimTill ( xmlNodePtr elm );
		Cont* trimTill ( const char *name, const char *prop, long val );
		xmlNodePtr currentBwc ();
		Cont* import ( xmlNodePtr foreign_elm = NULL );
		Cont* importAndToggle ( xmlNodePtr foreign_elm = NULL );
		Cont* replace ( xmlNodePtr source_elm = NULL, bool toggle = false );
		Cont* concatenate ( xmlNodePtr source_elm = NULL );
		Cont* invC ( const xmlChar *op, Cont *rcvC, xmlNodePtr eoi_elm);
		Cont* invBwc ( Cont *rcvBwc, xmlNodePtr eif_elm);
		Cont* eosBwc (long sid, xmlNodePtr compensate_elm);
		Cont* eoiInvBwc (xmlNodePtr foreign_chl_elm, xmlNodePtr foreign_eoif_elm);
		long getBid ();
		char* toStr () { return (char *) xmlNodeToXstr (domelm); };
		
		long msgSizeC ();
		long msgSizeD ();
		
		xmlNodePtr domelm; // the <c> or <bwc> element
	private:
		xmlDocPtr doc;
		long getBid (xmlNodePtr startelm);
		Cont* importWithToggleFlag ( xmlNodePtr foreign_elm = NULL, bool toggle = false );
		
		long msgSizeC (xmlNodePtr elm);
		long sumChildMsgSizeC (xmlNodePtr parent);
		long msgSizeD (xmlNodePtr elm);
		long sumChildMsgSizeD (xmlNodePtr parent);
		
		// default size of messages for service invocation is 500 bytes
		static const long SVC_MSG_SIZE = 500;
		// default size of messages for an activity in continuation is 100 bytes
		static const long ACT_MSG_SIZE = 100;
};

#endif