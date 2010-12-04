//-------------------------------------------------------------
// File: feeder.cc
//     (part of OMNeT++ simulation of CPMnet)
// Author: Weihai Yu
//-------------------------------------------------------------

#ifndef __FEEDER_CC
#define __FEEDER_CC

#include "feeder.h"
// #include "cpm.h"

// register module class with OMNeT++
Define_Module(Feeder);

Feeder::Feeder ()
{
}

Feeder::~Feeder ()
{

}

void Feeder::fillAutoAttr (xmlNodePtr node, int auto_size, int *auto_vals)
{
	/*if (xmlStrEqual(node->name, BAD_CAST _CPMRUN))
			{
				xmlDocPtr cpmdoc = makeCpmDoc (node);
				xmlChar *cpmxstr = xmlDocToXstr (cpmdoc);
				ev << "nodes form cpm_run: " << endl;
				ev << (char *) cpmxstr << endl;
			}
	*/
	xmlAttrPtr auto_attr = xmlHasProp (node, BAD_CAST _AUTONAME);
	if (auto_attr)
	{
		int autoindex = xmlGetIntProp (node, _AUTOINDEX);
		xmlRemoveProp ( xmlHasProp (node, BAD_CAST _AUTOINDEX) );
		if (autoindex >= auto_size)
		{
			ev << "XMLUtil::fillAutoAttr: autoindex " << autoindex;
			ev << " is greater than " << auto_size << endl;
			return;
		}
		xmlChar *ixstr = intToXstr (auto_vals[autoindex]);
		xmlChar *namexstr = xmlGetProp(node, BAD_CAST _AUTONAME);
		xmlSetProp (node, namexstr,  ixstr);
		xmlFree(ixstr);
		xmlFree(namexstr);
		xmlRemoveProp (auto_attr);
	}

	for (xmlNodePtr child = xmlFirstElementChild(node);
		child != NULL;
		child = xmlNextElementSibling(child))
		fillAutoAttr (child, auto_size, auto_vals);
}

void Feeder::generateRandomIndex (int size, int *agents)
{
	 ev << "Feeder::generateRandomIndex: ";
	for (int i = 0; i<size; ++i)
	{
		int newagent;
		do
		{
			newagent = (int) uniform (1, num_agents);
		}
		while (alreadyGenerated (newagent, agents, i-1));
		agents[i] = newagent;
		ev << newagent << ", ";
	}
	ev << endl;

}
bool Feeder::alreadyGenerated (int agent, int *agents, int sofar)
{
	for (int i=0; i <= sofar; ++i)
		if (agents[i] == agent) return true;
	return false;
}

// <sequence> <goto 1> <flow> <goto 1 ..n> </flow></sequence>
xmlNodePtr Feeder::makeAllocNode (int autosize, int *autoindex)
{
	xmlNodePtr scope_elm = xmlNewNode (NULL, BAD_CAST _SCP);
	xmlSetIntProp (scope_elm, _ADDR, autoindex[0]);
	xmlNodePtr flow_elm = xmlNewNode (NULL, BAD_CAST _FLW);
	xmlAddChild(scope_elm, flow_elm);
	for (int i = 0; i < autosize; i++)
	{
		xmlNodePtr site_elm = xmlNewNode (NULL, BAD_CAST _GTO);
		xmlSetIntProp (site_elm, _ADDR, autoindex[i]);
		xmlAddChild(flow_elm, site_elm);
	}
	return scope_elm;
}

xmlDocPtr Feeder::makeCpmDoc (xmlNodePtr node)
{
	xmlDocPtr d = strToDoc (_NULLCPM);
	xmlNodePtr root = xmlDocGetRootElement (d);
	xmlSetIntProp (root, _FID, Cpm::nextid++);
	xmlNodePtr c_elm = xmlElementChildByName (root, _C);
	xmlAddChild(c_elm, xmlDocCopyNode (node, d, 1));
	return d;
}

// xmlChar* Feeder::makeCPMxstr (xmlNodePtr node)
// {
//
// 	xmlChar *xmlbuff;
//     int buffersize;
// 	xmlDocPtr newdoc = makeCpmDoc(node);
//     xmlDocDumpMemory(newdoc, &xmlbuff, &buffersize);
// 	xmlFreeDoc (newdoc);
// 	return xmlbuff;
// }
//
// CPmessage* Feeder::makeRunCPmsg (int autosize, int *autoindex)
// {
// 	xmlNodePtr this_child = xmlCopyNode (runcpm_template_node, 1);
// 	fillAutoAttr (this_child, autosize, autoindex);
// 	// ev << "To run:\n";
// 	// evDumpNode (this_child);
// 	// ev << "\n-----\n";
//
// 	CPmessage *runcmsg = new CPmessage (_CPM, CPM);
// 	xmlDocPtr cpmdoc = makeCpmDoc(this_child);
// 	xmlChar *cpmxstr = xmlDocToXstr (cpmdoc);
// 	runcmsg->setCpm((char *)cpmxstr);
// 	xmlFreeDoc (cpmdoc);
// 	xmlFree (cpmxstr);
// 	xmlFreeNode (this_child);
//
// 	runcmsg->setByteLength (cmsg_byte_length);
// 	runcmsg->setNumSites (autosize);
// 	for (int i = 0; i < autosize; i++)
// 	{
// 		runcmsg->setSites(i, autoindex[i]);
// 	}
// 	return runcmsg;
// }
//
// void Feeder::runRandomCpm (simtime_t when)
// {
// 	int autoindex[flow_size+1];
// 	generateRandomIndex (flow_size + 1, autoindex);
//
// 	CPmessage *runcmsg = makeRunCPmsg(flow_size + 1, autoindex);
// 	runcmsg->setNumRepeats (num_repeats);
// 	scheduleAt (when, runcmsg);
// }
//
// void Feeder::runRepeatCpm (simtime_t when, CPmessage* cmsg)
// {
// 	int num_sites = cmsg->getNumSites();
// 	int autoindex[num_sites];
// 	ev << "runRepeatCpm: [";
// 	for (int i = 0; i < num_sites; i++)
// 	{
// 		autoindex[i] = cmsg->getSites(i);
// 		ev << autoindex[i] << ", ";
// 	}
// 	ev << "]\n";
//
// 	CPmessage *runcmsg = makeRunCPmsg(num_sites, autoindex);
// 	if (cmsg->getNumRepeats () < 1000)
// 		runcmsg->setNumRepeats (cmsg->getNumRepeats () - 1);
// 	else
// 		// repeat for ever
// 		runcmsg->setNumRepeats (num_repeats);
// 	scheduleAt (when, runcmsg);
// }

void Feeder::runCpm (simtime_t when, CPmessage* cmsg)
{
	int autoindex[flow_size+1];

	int autosize;
	bool is_first_in_repeat, is_last_in_repeat;

	if (!cmsg)
	{
		is_first_in_repeat = true;
		is_last_in_repeat = (num_repeats <= 0);
	}
	else
	{
		is_first_in_repeat = (cmsg->getNumRepeats() <= 0);
		is_last_in_repeat = (cmsg->getNumRepeats() == 1);
	}

	if (is_first_in_repeat)
	{
		generateRandomIndex (flow_size + 1, autoindex);
		autosize = flow_size + 1;
	}
	else
	{
		autosize = cmsg->getNumSites();
		for (int i = 0; i < autosize; i++)
		{
			autoindex[i] = cmsg->getSites(i);
		}
	}

	xmlDocPtr cpmdoc = makeCpmDoc(runcpm_template_node);
	xmlNodePtr cpmroot = xmlDocGetRootElement (cpmdoc);
	fillAutoAttr (cpmroot, autosize, autoindex);
	if (is_first_in_repeat && instantiate)
	{  // last in <c>: first in stack
		xmlAddChild(xmlElementChildByName (cpmroot, _C),
					makeAllocNode (autosize, autoindex));

	}
	if (is_last_in_repeat && instantiate)
	{  // first in <c>: last in stack (before <none />)
		xmlAddNextSibling (xmlFirstElementChild (xmlElementChildByName (cpmroot, _C)),
						  makeAllocNode (autosize, autoindex));
	}

	CPmessage *runcmsg = new CPmessage (_CPM, CPM);
	xmlChar *cpmxstr = xmlDocToXstr (cpmdoc);
	runcmsg->setCpm((char *)cpmxstr);
	xmlFreeDoc (cpmdoc);
	xmlFree (cpmxstr);

	runcmsg->setByteLength (cmsg_byte_length);
	runcmsg->setNumSites (autosize);
	for (int i = 0; i < autosize; i++)
	{
		runcmsg->setSites(i, autoindex[i]);
	}

	if (is_first_in_repeat)
	{
		runcmsg->setNumRepeats (num_repeats);
	}
	else
	{
		if (cmsg->getNumRepeats () < 1000)
			runcmsg->setNumRepeats (cmsg->getNumRepeats () - 1);
		else
			// repeat for ever
			runcmsg->setNumRepeats (num_repeats);
	}

	scheduleAt (when, runcmsg);
}

void Feeder::initialize ()
{
	ev << "Feeder::initialize: ..." << endl;
	//ev<< "module type: " << (getParentModule()->getSubmodule("agent", 0))->getSubmodule(tcpApp,0) <<endl;
	//agent_0 = check_and_cast<Agent *>(getParentModule()->getSubmodule("agent", 0);
	agent_0 = check_and_cast<Agent *>(getParentModule()->getSubmodule("agent", 0)->getSubmodule("tcpApp",0));

	// CPMnet parameters
	const char *cpm_file = getParentModule() -> par("cpm_file").stringValue();
	debugging = getParentModule() -> par("debugging");
	instantiate = getParentModule() -> par("instantiate");
	num_agents = (int) getParentModule() -> par("num_agents");
	int agent_dups = (int) getParentModule() -> par("agent_dups");
	num_servers = (num_agents -1) * agent_dups;
	mpl = (int) getParentModule() -> par("mpl");
	num_repeats = (int) getParentModule() -> par("num_repeats");
	flow_num_branches = (int) getParentModule() -> par("flow_num_branches");
	flow_num_steps = (int) getParentModule() -> par("flow_num_steps");
	flow_size = flow_num_branches * flow_num_steps;
	cmsg_byte_length = (long) getParentModule() -> par("cmsg_byte_length");
	ev << "[Feeder] " << num_servers << " servers, ";
	ev << flow_num_branches << " branches, ";
	ev << flow_num_steps << " steps, with mpl " << mpl << " at agents\n";
	ev << "runcpm file: " << cpm_file << endl;
	if (debugging)
		ev << "... in debugging mode ...\n";

	// Feeder parameters
	flow_ia_time = par("flow_ia_time").doubleValue()/1000;

	// For every run, start with the same global state.
	// This is necessary because several runs can be executed in the same process.
	// Cpm::reserId ();


     // this initialize the libxml2 library and check potential ABI mismatches
     // between the version it was compiled for and the actual shared
     // library used.

    LIBXML_TEST_VERSION
	doc = xmlReadFile (cpm_file, NULL, 0);

	for (xmlNodePtr child = xmlFirstElementChild (xmlDocGetRootElement(doc));
	child != NULL;
	child = xmlNextElementSibling (child))
	{
		//ev << "Feeder::initialize:\n" << XMLString::transcode(child->getTagName()) << endl;
		//ev<< "name: " << child->name<<endl;
		if (xmlStrEqual(child->name, BAD_CAST _CPMINSTALL))
		{
			// ev << "Feeder::initialize: cpminstall\n";
			for (xmlNodePtr installchild = xmlFirstElementChild (child);
			installchild != NULL;
			installchild = xmlNextElementSibling (installchild) )
			{
				for (int agent_i = 1; agent_i < num_agents; agent_i++)
				{
					xmlNodePtr this_child = xmlCopyNode (installchild, 1);
					int autoindex[0];
					autoindex[0] = agent_i;
					fillAutoAttr (this_child, 1, autoindex);
					// ev << "To install:\n";
					// evDumpNode (this_child);
					// ev << "\n-----\n";
					CPmessage *cmsg = new CPmessage (_CPM, CPM);
					xmlDocPtr cpmdoc = makeCpmDoc (this_child);
					xmlChar *cpmxstr = xmlDocToXstr (cpmdoc);
					cmsg->setCpm((char *)cpmxstr);
					ev << "CPM message installed in agent: "<<agent_i << endl;
					ev << (char *)cpmxstr << endl;
					xmlFree (cpmxstr);
					xmlFreeNode (this_child);
					xmlFreeDoc (cpmdoc);
					scheduleAt (0.0, cmsg);
				}
			}
		}
		if (xmlStrEqual(child->name, BAD_CAST _CPMRUN))
		{
			ev << "Feeder::initialize: cpmrun ";
			ev << (num_servers*mpl)/flow_num_branches <<" flows\n";
			runcpm_template_node = xmlFirstElementChild (child);
			xmlDocPtr cpmdoc_run = makeCpmDoc (runcpm_template_node);
			xmlChar *cpmxstr_run = xmlDocToXstr (cpmdoc_run);
			ev << "nodes form cpm_run: " << endl;
			ev << (char *) cpmxstr_run << endl;
			if (!runcpm_template_node)
			{
				ev << "Feeder: nothing to run!\n";
				return;
			}
			for (int flow_i = 1; flow_i <= (num_servers*mpl)/flow_num_branches; flow_i++)
			{
				ev << "Calling runcpm with: "<< flow_i*flow_ia_time/mpl << endl;
				runCpm (0.5 + flow_i*flow_ia_time/mpl);
				if (debugging)
					return; // run only one flow for debugging
			}
			xmlFree(cpmxstr_run);
			xmlFreeDoc(cpmdoc_run);
		}
	}
}

void Feeder::handleMessage(cMessage *msg)
{
	CPmessage *cmsg = check_and_cast<CPmessage *>(msg);
	if (msg->isSelfMessage())
	{

		cmsg->setStartTime (simTime());
		cmsg->setTimestamp ();
		cmsg->setSrc(-1);
		cmsg->setDest(0);
		agent_0->feed(cmsg);
	}
	else
	{
		runCpm (simTime(), cmsg);
		delete msg;
	}
}

void Feeder::finish ()
{
	xmlFreeDoc (doc);
	xmlCleanupParser ();
}


#endif // __FEEDER_CC
