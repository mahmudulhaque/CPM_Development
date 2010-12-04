//-------------------------------------------------------------
// File: scpact.cc
//     (part of OMNeT++ simulation of CPMnet)
// Author: Weihai Yu
//-------------------------------------------------------------


#include "scpact.h"
#include "agent.h"

// used to make a scpm message
ScpAct::ScpAct( CPMsite *site,
				long newfid, long newsid, int newsaddr,
				long newbid, int newsrc, bool rollingbk )
: local_site(site),
  fid(newfid), sid(newsid),
  bid(newbid), csid(-1), psid(-1),
  saddr(newsaddr), src(newsrc),
  newsite(-1), psaddr(-1), rollingback(rollingbk)
{
	act = NONE;
}

// init from a doc
ScpAct::ScpAct( xmlDocPtr doc )
{
	set (doc);
}

// init from a string
ScpAct::ScpAct( const char *str)
{
	set (str);
}

ScpAct::~ScpAct ()
{
}

ScpAct* ScpAct::set( xmlDocPtr doc )
{
	xmlNodePtr elm = xmlDocGetRootElement (doc);
	if ( !xmlStrEqual(elm->name, BAD_CAST _SCPM))
	{
		ev << "ScpAct::ScpAct(): wrong doc type!\n";
		return this;
	}

	fid = xmlGetIntProp (elm, _FID);
	sid = xmlGetIntProp (elm, _SID);
	bid = xmlGetIntProp (elm, _BID);
	src = xmlGetIntProp (elm, _SRC);
	xmlChar *headingxtr = xmlGetProp (elm, BAD_CAST _HEADING);
	if (xmlStrEqual(headingxtr, BAD_CAST _BACKWARD))
		rollingback = true;
	else
		rollingback = false;
	xmlFree (headingxtr);

	xmlChar *actxstr = xmlGetProp (elm, BAD_CAST _ACTION);
	setAct ( (char *) actxstr );
	xmlFree (actxstr);
	switch (act)
	{
		case NEWSITE:
		{
			newsite = xmlGetIntProp (elm, _ADDR);
			break;
		}
		case NEWSCP:
		{
			csid = xmlGetIntProp (elm, _CSID);
			break;
		}
		case STOPSCP: case ROLLBACKSCP:
		{
			psid = xmlGetIntProp (elm, _PSID);
			psaddr = xmlGetIntProp (elm, _PSADDR);
			break;
		}
		default:
		{
		}

	}
	return this;
}

ScpAct* ScpAct::set (const char * str)
{
	// ev << "ScpAct::set " << str << endl;
	xmlDocPtr doc = strToDoc (str);
	set (doc);
	xmlFreeDoc (doc);
	return this;
}

ScpAct* ScpAct::setAct (const char* str)
{
	if (strcmp (str, _NON) == 0)
		act = NONE;
	else if (strcmp(str, _NEWSITE) == 0)
		act = NEWSITE;
	else if (strcmp(str, _NEWSCP) == 0)
		act = NEWSCP;
	else if (strcmp(str, _DROPSITE) == 0)
		act = DROPSITE;
	else if (strcmp(str, _ROLLBACK) == 0)
		act = ROLLBACK;
	else if (strcmp(str, _STOPSCP) == 0)
		act = STOPSCP;
	else if (strcmp(str, _ROLLBACKSCP) == 0)
		act = ROLLBACKSCP;
	else
		ev << "ScpAct::setAct(): unknown act type!\n";
	return this;
}

const char* const ScpAct::actStr ()
{
	switch (act)
	{
		case NONE: return _NON;
		case NEWSITE: return _NEWSITE;
		case NEWSCP: return _NEWSCP;
		case DROPSITE: return _DROPSITE;
		case ROLLBACK: return _ROLLBACK;
		case STOPSCP: return _STOPSCP;
		case ROLLBACKSCP: return _ROLLBACKSCP;
	}
	return NULL;
}

ScpAct* ScpAct::setNewSite ( int addr )
{
	newsite = addr;
	return this;
}

ScpAct* ScpAct::setNewScp ( long newsid )
{
	csid = newsid;
	return this;
}

ScpAct* ScpAct::setParent ( long newpsid, int newpsaddr )
{
	psid = newpsid;
	psaddr = newpsaddr;
	return this;
}

char* ScpAct::toStr ()
{
	ostringstream stm;
	stm << "<scpm fid=\"" << fid;
	stm << "\" sid=\"" << sid;
	stm << "\" src=\"" << src;
	stm << "\" bid=\"" << bid;
	if (rollingback)
		stm << "\" heading=\"backward";
	else
		stm << "\" heading=\"forward";
	stm << "\" action=\"" << actStr() << "\" ";

	switch (act)
	{
		case NEWSITE:
		{
			stm << "addr =\"" << newsite << "\" ";
			break;
		}
		case NEWSCP:
		{
			stm << " csid =\"" << csid << "\" ";
		}
		case STOPSCP: case ROLLBACKSCP:
		{
			stm << " psid =\"" << psid << "\" ";
			stm << " psaddr =\"" << psaddr << "\" ";
			break;
		}
		case DROPSITE: case ROLLBACK: case NONE:
		{
			// no additional info to generate
			break;
		}
	}

	stm << "/>";

	return strdup (stm.str().c_str());
}

SCPmessage* ScpAct::toMsg ()
{
	SCPmessage *msg = new SCPmessage (_SCPM, SCPM);
	char *actstr = toStr ();
	msg->setCmd(actstr);
	delete actstr;
	return msg;
}

void ScpAct::sendToScpMgr ()
{
		// if the scope manager is local, there is no scpm process time
		if ( saddr == local_site->addr())
		{
			char *actstr = toStr ();
			local_site->reg->handleMessage (actstr);
			delete actstr;
		}
		else
		{
			SCPmessage *smsg = toMsg();
			smsg->setSrc (local_site->addr());
			smsg->setDest (saddr);
			smsg->setByteLength (local_site->agent->smsg_byte_length);
			local_site->agent->sendTo (saddr, smsg);
		}
}
