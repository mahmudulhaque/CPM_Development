//-------------------------------------------------------------
// File: scpact.h
//     (part of OMNeT++ simulation of CPMnet)
// Author: Weihai Yu
//-------------------------------------------------------------

#ifndef __SCPACT_H
#define __SCPACT_H

#include "cpmcommon.h"
#include "cpmsite.h"


class ScpAct
{
public:
	ScpAct ( CPMsite *site,
			long newfid,
			long newsid,
			int newsaddr,
			long newbid,
			int newsrc,
			bool rollingbk = false );
	ScpAct ( xmlDocPtr doc );
	ScpAct ( const char *str );
	~ScpAct ();
	ScpAct* set ( xmlDocPtr doc );
	ScpAct* set ( const char *str );
	ScpAct* setAct ( const char* str );
	ScpAct* setAct ( ScpActType a ) { act = a; return this; };
	const char* const actStr ();
	ScpAct* setNewSite ( int addr );
	ScpAct* setNewScp ( long newsid );
	ScpAct* setParent ( long newpsid, int newpsaddr );
	char* toStr ();
	SCPmessage* toMsg ();
	void sendToScpMgr ();

	CPMsite *local_site;
	// csid is only used with NEWSCP
	// newsite is only used with NEWSITE
	// psid and psaddr are only used with STOPSCP and ROLLBACKSCP
	long fid, sid, bid, csid, psid;
	int saddr, src, newsite, psaddr;
	bool rollingback;
	ScpActType act;
	
};

#endif