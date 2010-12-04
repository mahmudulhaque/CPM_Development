//-------------------------------------------------------------
// File: cpmcommon.h
//     (part of OMNeT++ simulation of CPMnet)
// Author: Weihai Yu
//-------------------------------------------------------------



#ifndef __CECOMMON_H
#define __CECOMMON_H


#include <omnetpp.h>

#include "CPmessage_m.h"
#include "SCPmessage_m.h"

#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>


// message kind values (packet types):
// CPM - Continuation-passing
// SCPM - Scope management
// INSM - (Un-)Instantiatin
enum msgType { CPM, SCPM };

const char _CPM[] = "cpm";
const char _CPMINSTALL[] = "cpminstall";
const char _CPMRUN[] = "cpmrun";

const char _C[] = "c";
const char _E[] = "e";
const char _BWC[] = "bwc";

enum ActType {NON, SCP, INV, RCV, RPL, OWY, SEQ, FLW,
				PCK, EOS, ESF, EOI, EIF, JON, PRC, GTO, ASGN,
				ALLOC, DEALLOC};
const int numOfActTypes = 19;
const char _NON[] = "none";
const char _SCP[] = "scope";
const char _INV[] = "invoke";
const char _RCV[] = "receive";
const char _RPL[] = "reply";
const char _OWY[] = "oneway";
const char _SEQ[] = "sequence";
const char _FLW[] = "flow";
const char _PCK[] = "pick";
const char _EOS[] = "eos";
const char _ESF[] = "eosf";
const char _EOI[] = "eoi";
const char _EIF[] = "eoif";
const char _JON[] = "join";
const char _PRC[] = "proc";
const char _GTO[] = "goto";
const char _ASGN[] = "assign";
const char _ALLOC[] = "alloc";
const char _DEALLOC[] = "dealloc";


ActType actType (const xmlChar* xstr);
const char* actTypeStr (ActType t);
const xmlChar* actTypeXStr (ActType t);

const ActType actTypeVals[numOfActTypes] =
	{NON, SCP, INV, RCV, RPL, OWY, SEQ, FLW,
	 PCK, EOS, ESF, EOI, EIF, JON, PRC, GTO};

const char _0[] = "0";
const char _FID[] = "fid";
const char _SID[] = "sid";
const char _SADDR[] = "saddr";
const char _ADDR[] = "addr";
const char _JID[] = "jid";
const char _BID[] = "bid";
const char _COUNT[] = "count";
const char _LINK[] = "link";
const char _OP[] = "op";
const char _IID[] = "iid";
const char _SRC[] = "src";

const char _AUTONAME[] = "autoname";
const char _AUTOINDEX[] = "autoindex";

const char _VARS[] = "variables";
const char _INS[] = "inputVariables";
const char _OUTS[] = "outputVariables";
const char _VAR[] = "variable";
const char _NAME[] = "name";
const char _SIZE[] = "size";
const char _CPY[] = "copy";
const char _FROM[] = "from";
const char _TO[] = "to";
const char _EHL[] = "eventhandler";
const char _CHL[] = "compensationhandler";
const char _FHL[] = "faulthandler";
const char _HEADING[] = "heading";
const char _FORWARD[] = "forward";	
const char _BACKWARD[] = "backward";
const char _BRANCH[] = "branch";
const char _FAILRATE[] = "failrate";
	
const char* const _NULLCPM = 
"<?xml version=\"1.0\"?>\
 <cpm fid=\"-1\" sid=\"-1\" saddr=\"-1\" bid=\"-1\">\
  <c><none/></c><e/>\
</cpm>";

enum Status {ACTIVE, DONE, ROLLINGBACK, ABORTED, UNDONE, STOPPING, UNKNOWN};
const char _STATUS[] = "STATUS";
const char _ACTIVE[] = "ACTIVE";
const char _DONE[] = "DONE";
const char _ROLLINGBACK[] = "ROLLINGBACK";
const char _ABORTED[] = "ABORTED";
const char _UNDONE[] = "UNDONE";
const char _STOPPING[] = "STOPPING";
const char _UNKNOWN[] = "UNKNOWN";

Status status (const xmlChar* xstr);
const char* statusStr (Status stat);
const xmlChar* statusXStr (Status stat);

enum ScpActType {NONE, NEWSITE, NEWSCP, DROPSITE, ROLLBACK, STOPSCP, ROLLBACKSCP};
const char _SCPM[] = "scpm";
const char _ACTION[] = "action";
const char _CSID[] = "csid";
const char _PSID[] = "psid";
const char _PSADDR[] = "psaddr";
const char _NEWSITE[] = "newsite";
const char _NEWSCP[] = "newscp";
const char _DROPSITE[] = "dropsite";
const char _ROLLBACK[] = "rollback";
const char _STOPSCP[] = "stopscp";
const char _ROLLBACKSCP[] = "rollbackscp";

int xstrToInt (xmlChar* xstr);
xmlChar* intToXstr (int i);
xmlDocPtr xmlNodeToDoc (xmlNodePtr node);
xmlChar* xmlDocToXstr ( xmlDocPtr doc);
xmlChar* xmlNodeToXstr (xmlNodePtr node);
xmlDocPtr strToDoc (const char* str);
int	xmlGetIntProp (xmlNodePtr node, const char* name);
xmlAttrPtr xmlSetIntProp (xmlNodePtr node, const char* name, int val);
xmlNodePtr xmlElementChildByName ( xmlNodePtr parent, const char* name);
void evDumpDoc (xmlDocPtr doc);
void evDumpNode (xmlNodePtr node);
// a bug fixing of the libxml2 xmlPreviousElementSibling
xmlNodePtr xmlPrevElementSibling(xmlNodePtr node);
xmlNodePtr xmlPurgeNode (xmlNodePtr node);
void xmlPurgeChildren (xmlNodePtr node);



/*
// notify the scope manager of a frowarding
const char* const _INIT_NEWSITES =
"<scpm fid=\"-1\" sid=\"-1\" saddr=\"-1\" bid=\"-1\" src=\"-1\" action=\"newsite\" addr=\"-1\" />";

// notify the scope manage of the creation of a new scope
const char* const _INIT_NEWSCP =
"<scpm fid=\"-1\" sid=\"-1\" saddr=\"-1\" bid=\"-1\" src=\"-1\" action=\"newscp\" csid=\"-1\" />";

// notify the scope manager of one more step backward
const char* const _INIT_DROPSITE =
"<scpm fid=\"-1\" sid=\"-1\" saddr=\"-1\" bid=\"-1\" src=\"-1\" action=\"dropsite\" />";

// notify the scope manager of the start of a rollback
const char* const _INIT_ROLLBACK =
"<scpm fid=\"-1\" sid=\"-1\" saddr=\"-1\" bid=\"-1\" src=\"-1\" action=\"rollback\" />";

// command the site to stop the scope
const char* const _INIT_STOPSCP =
"<scpm fid=\"-1\" sid=\"-1\" saddr=\"-1\" psid=\"-1\" psaddr=\"-1\" bid=\"-1\" src=\"-1\" action=\"stopscp\" />";

// command the site to rollback the scope
const char* const _INIT_ROLLBACKSCP =
"<scpm fid=\"-1\" sid=\"-1\" saddr=\"-1\" psid=\"-1\" psaddr=\"-1\" bid=\"-1\" src=\"-1\" action=\"rollbackscp\" />";
*/
#endif // __CECOMMON_H
