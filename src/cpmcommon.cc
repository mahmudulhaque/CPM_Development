//-------------------------------------------------------------
// File: cpmcommon.cc
//     (part of OMNeT++ simulation of CPMnet)
// Author: Weihai Yu
//-------------------------------------------------------------


#include "cpmcommon.h"

ActType actType (const xmlChar* xstr)
{
	if (xmlStrEqual (xstr, BAD_CAST _NON))
		return NON;
	if (xmlStrEqual (xstr, BAD_CAST _SCP))
		return SCP;
	if (xmlStrEqual (xstr, BAD_CAST _INV))
		return INV;
	if (xmlStrEqual (xstr, BAD_CAST _RCV))
		return RCV;
	if (xmlStrEqual (xstr, BAD_CAST _RPL))
		return RPL;
	if (xmlStrEqual (xstr, BAD_CAST _OWY))
		return OWY;
	if (xmlStrEqual (xstr, BAD_CAST _SEQ))
		return SEQ;
	if (xmlStrEqual (xstr, BAD_CAST _FLW))
		return FLW;
	if (xmlStrEqual (xstr, BAD_CAST _PCK))
		return PCK;
	if (xmlStrEqual (xstr, BAD_CAST _EOS))
		return EOS;
	if (xmlStrEqual (xstr, BAD_CAST _ESF))
		return ESF;
	if (xmlStrEqual (xstr, BAD_CAST _EOI))
		return EOI;
	if (xmlStrEqual (xstr, BAD_CAST _EIF))
		return EIF;
	if (xmlStrEqual (xstr, BAD_CAST _JON))
		return JON;
	if (xmlStrEqual (xstr, BAD_CAST _PRC))
		return PRC;
	if (xmlStrEqual (xstr, BAD_CAST _GTO))
		return GTO;
	if (xmlStrEqual (xstr, BAD_CAST _ASGN))
		return ASGN;

	ev << "unknown activity type: " << (char *) xstr << endl;
	return NON;
}

const char* actTypeStr (ActType t)
{
	switch (t)
	{
		case NON: return _NON;
		case SCP: return _SCP;
		case INV: return _INV;
		case RCV: return _RCV;
		case RPL: return _RPL;
		case OWY: return _OWY;
		case SEQ: return _SEQ;
		case FLW: return _FLW;
		case PCK: return _PCK;
		case EOS: return _EOS;
		case ESF: return _ESF;
		case EOI: return _EOI;
		case EIF: return _EIF;
		case JON: return _JON;
		case PRC: return _PRC;
		case GTO: return _GTO;
		case ASGN: return _ASGN;
		default: return _NON;
	}
}

const xmlChar* actTypeXStr (ActType t)
{
	return BAD_CAST actTypeStr (t);
}

Status status (const xmlChar *xstr)
{
	if (xmlStrEqual (xstr, BAD_CAST _ACTIVE))
		return ACTIVE;
	if (xmlStrEqual (xstr, BAD_CAST _DONE))
		return DONE;
	if (xmlStrEqual (xstr, BAD_CAST _ROLLINGBACK))
		return ROLLINGBACK;
	if (xmlStrEqual (xstr, BAD_CAST _ABORTED))
		return ABORTED;
	if (xmlStrEqual (xstr, BAD_CAST _UNDONE))
		return UNDONE;
	if (xmlStrEqual (xstr, BAD_CAST _STOPPING))
		return STOPPING;
	return 	UNKNOWN;
}


const char* statusStr (Status stat)
{
	switch (stat)
	{
		case ACTIVE: return _ACTIVE;
		case DONE: return _DONE;
		case ROLLINGBACK: return _ROLLINGBACK;
		case ABORTED: return _ABORTED;
		case UNDONE: return _UNDONE;
		case STOPPING: return _STOPPING;
		default: return _UNKNOWN;
	}
}

const xmlChar* statusXStr (Status stat)
{
	return BAD_CAST statusStr (stat);
}

int xstrToInt (xmlChar* xstr)
{
	return atoi ((char *) xstr);
}

xmlChar* intToXstr ( int i )
{
	// -2147483647 - +2147483647 ?
	char str[10];
	snprintf(str, sizeof(str), "%d", i);
	return xmlCharStrdup (str);
}

xmlDocPtr xmlNodeToDoc ( xmlNodePtr node )
{
	xmlDocPtr doc = xmlNewDoc(BAD_CAST XML_DEFAULT_VERSION);
	xmlDocSetRootElement(doc, xmlDocCopyNode (node, doc, 1));
	return doc;
}

xmlChar* xmlDocToXstr ( xmlDocPtr doc )
{
	xmlChar *xmlbuff;
    int buffersize;
    xmlDocDumpMemory(doc, &xmlbuff, &buffersize);
	return xmlbuff;
}

xmlChar* xmlNodeToXstr ( xmlNodePtr node )
{
	xmlDocPtr doc = xmlNodeToDoc (node);
	xmlChar *xmlbuff = xmlDocToXstr (doc);
	xmlFreeDoc (doc);
	return xmlbuff;
}

xmlDocPtr strToDoc (const char* str)
{
	return xmlReadDoc (BAD_CAST str, NULL, NULL, 0);
}

int	xmlGetIntProp (xmlNodePtr node, const char* name)
{
	xmlChar* valxstr = xmlGetProp (node, BAD_CAST name);
	if (!valxstr)
		return -1;
	int res = atoi ((char *) valxstr);
	xmlFree (valxstr);
	return res;
}

xmlAttrPtr xmlSetIntProp (xmlNodePtr node, const char* name, int val)
{
	xmlChar *valxstr = intToXstr (val);
	xmlAttrPtr attr = xmlSetProp (node, BAD_CAST name, valxstr);
	xmlFree (valxstr);
	return (attr);
}

xmlNodePtr xmlElementChildByName ( xmlNodePtr parent, const char* name)
{
	for (xmlNodePtr child = xmlFirstElementChild (parent);
		child != NULL;
		child = xmlNextElementSibling (child) )
	{
		if (strcmp (name, (char *) child->name) == 0)
			return child;
	}
	return NULL;
}

void evDumpDoc (xmlDocPtr doc)
{
	xmlChar *xmlbuff;
    int buffersize;
    xmlDocDumpMemory(doc, &xmlbuff, &buffersize);
	ev << (char *) xmlbuff;
	xmlFree (xmlbuff);
}


void evDumpNode ( xmlNodePtr node )
{
	xmlChar *xmlbuff = xmlNodeToXstr (node);
    ev << (char *) xmlbuff + 22; // skip the <?xml ..?> header
	xmlFree (xmlbuff);
}

// a bug fixing of the libxml2 xmlPreviousElementSibling
xmlNodePtr xmlPrevElementSibling(xmlNodePtr node) {
    if (node == NULL)
        return(NULL);
    switch (node->type) {
        case XML_ELEMENT_NODE:
        case XML_TEXT_NODE:
        case XML_CDATA_SECTION_NODE:
        case XML_ENTITY_REF_NODE:
        case XML_ENTITY_NODE:
        case XML_PI_NODE:
        case XML_COMMENT_NODE:
        case XML_XINCLUDE_START:
        case XML_XINCLUDE_END:
            node = node->prev;
            break;
        default:
            return(NULL);
    }
    while (node != NULL) {
        if (node->type == XML_ELEMENT_NODE)
            return(node);
        node = node->prev; // The bug was node->next
    }
    return(NULL);
}

xmlNodePtr xmlPurgeNode (xmlNodePtr node)
{
	if (node)
	{
		xmlUnlinkNode(node);
		xmlFreeNode (node);
	}
	return (xmlNodePtr)NULL;
}

void xmlPurgeChildren (xmlNodePtr node)
{
	xmlNodePtr curr = xmlFirstElementChild (node);
	while (curr)
	{
		xmlNodePtr next = xmlNextElementSibling (curr);
		xmlPurgeNode (curr);
		curr = next;
	}
}
