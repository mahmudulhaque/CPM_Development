//-------------------------------------------------------------
// File: environ.cc
//     (part of OMNeT++ simulation of CPMnet)
// Author: Weihai Yu
//-------------------------------------------------------------


#include "environ.h"
#include "cpm.h"

const long Environ::ACT_ENV_MSG_SIZE;

Environ* Environ::set (xmlDocPtr newdoc, xmlNodePtr elm)
{
	reset ();
	doc = newdoc;
	set (elm);
	xmlSetIntProp (domelm, _FID, cpm->getFid());
	return this;
}

Environ* Environ::set (xmlNodePtr elm)
{
	if (!doc) return NULL;
	if (!elm)
	{
		elm = xmlNewNode (NULL, BAD_CAST _E);
	}
	if (domelm)
	{
		xmlReplaceNode (domelm, elm);
		xmlFreeNode (domelm);
		domelm = elm;
	}
	else
	{
		domelm = elm;
	}
	currelm = findCurrElm (domelm);

	return this;
}

Environ* Environ::importSet (xmlNodePtr elm)
{
	if (!elm)
	{
		ev << "Environ::importSet - empty element!\n";
		return this;
	}
	return set (xmlDocCopyNode (elm, doc, 1));
}

Environ* Environ::reset ()
{
	doc = NULL;
	domelm = NULL;
	currelm = NULL;
	return this;
}

Environ* Environ::append (xmlNodePtr elm, const char  *stat)
{
	xmlSetProp (elm, BAD_CAST _STATUS, BAD_CAST stat);
	xmlAddChild (currelm, elm);
	currelm = findCurrElm (currelm);
	return this;
}

Environ* Environ::importAppend (xmlNodePtr elm, const char  *stat, bool deep)
{
	int deepflag = deep?1:2;
	xmlNodePtr homeelm = xmlDocCopyNode (elm, doc, deepflag);
	return append (homeelm, stat);
}

Status Environ::getStatus (xmlNodePtr elm)
{
	if (!elm)
		elm = currelm;
	xmlChar *statxstr = xmlGetProp (elm, BAD_CAST _STATUS);
	Status res = status (statxstr);
	xmlFree (statxstr);
	return res;
}

Environ* Environ::setStatus (xmlNodePtr elm, Status stat)
{
	xmlSetProp (elm, BAD_CAST _STATUS, statusXStr(stat));
	currelm = findCurrElm (domelm);
	// ev << "Environ::setStatus: Current is  <"
	// 	  << (char*) currelm->name << ">\n";
	return this;
}

// actname is provided only for the purpose of extra checking
// to avoid any surprise
Environ* Environ::setStatus (const char  *actname, Status stat)
{
	if (!xmlStrEqual (currelm->name, BAD_CAST actname))
	{
		ev << "Environ::setStatus: Current <"
			<< (char *) currelm->name
			<< "> is not <" << actname << ">\n";
		return this;
	}
	return setStatus (currelm, stat);
}

Environ* Environ::branchDone ()
{
	if (!xmlStrEqual (currelm->name, BAD_CAST _FLW))
	{
		ev << "Environ::branchDone - current element is <"
			<< (char *) currelm->name << ">, should be <flow>!\n";
		return this;
	}
	
	xmlNodePtr branch_elm = xmlNewNode (NULL, BAD_CAST _BRANCH);
	xmlSetIntProp (branch_elm, _BID, cpm->getBid());
	xmlSetProp (branch_elm, BAD_CAST _STATUS,
				cpm->isRollingback()?BAD_CAST _UNDONE : BAD_CAST _DONE);
	
	xmlNodePtr next_ch_elm = xmlFirstElementChild (currelm);
	while (next_ch_elm)
	{
		xmlNodePtr ch_elm = next_ch_elm;
		next_ch_elm = xmlNextElementSibling (ch_elm);
		xmlUnlinkNode (ch_elm);
		xmlAddChild (branch_elm, ch_elm);
	}
	xmlAddChild (currelm, branch_elm);
	return this;
}

Environ* Environ::merge ( int matchkey )
{
	if (!xmlStrEqual (currelm->name, BAD_CAST _FLW))
	{
		ev << "Environ::merge - current element is <"
			<< (char *) currelm->name << ">, should be <flow>!\n";
		return this;
	}
	long bid = cpm->getBid();
	std::pair<MsgMapIt,MsgMapIt> range = cpm->site->pool->get_range(matchkey);
	for (MsgMapIt mit = range.first; mit != range.second; mit++)
	{
		Cpm branch_cpm (cpm->site);
		xmlDocPtr branch_doc = strToDoc (mit->second->getCpm());
		branch_cpm.set (branch_doc);
		xmlNodePtr branch_elm = xmlFirstElementChild (branch_cpm.e->currelm);
		if (!xmlStrEqual (branch_elm->name, BAD_CAST _BRANCH))
		{
			ev << "Environ::merge - <" << (char *) branch_elm->name
				<< ">, should be <branch>!\n";
			continue;
		}
		if (xmlGetIntProp (branch_elm, _BID) != bid)
			xmlAddChild (currelm, xmlDocCopyNode (branch_elm, doc, 1));
	}
	return setStatus (currelm, cpm->isRollingback()?UNDONE:DONE);
}

// find the lastest element that is not done/undone yet
xmlNodePtr Environ::findCurrElm (xmlNodePtr elm)
{
	xmlNodePtr child = xmlLastElementChild (elm);
	if (!child)
		return elm;
	xmlChar *statxstr = xmlGetProp (child, BAD_CAST _STATUS);
	if (xmlStrEqual (statxstr, BAD_CAST _ACTIVE)
	|| xmlStrEqual (statxstr, BAD_CAST _ROLLINGBACK))
	{
		xmlFree (statxstr);
		return findCurrElm (child);
	}
	xmlFree (statxstr);
		return elm;
}


long Environ::msgSize ()
{
		return sumChildMsgSize (domelm);
}

long Environ::msgSize (xmlNodePtr elm)
{
	if (elm == NULL) return 0;

	if (xmlStrEqual (elm->name, BAD_CAST _INS)
		|| xmlStrEqual (elm->name, BAD_CAST _OUTS))
		return sumChildMsgSize (elm);
	
	if (xmlStrEqual (elm->name, BAD_CAST _VARS))
		return 0;
		
	if (xmlStrEqual (elm->name, BAD_CAST _VAR))
		return xmlGetIntProp (elm, _SIZE);
	
	return sumChildMsgSize (elm) + ACT_ENV_MSG_SIZE;
}

long Environ::sumChildMsgSize (xmlNodePtr parent)
{
	long sz = 0;
	for (xmlNodePtr child = xmlFirstElementChild (parent);
		child != NULL; child=xmlNextElementSibling(child))
		sz += msgSize (child);	
	return sz;	
}