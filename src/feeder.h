//-------------------------------------------------------------
// File: feeder.h
//     (part of OMNeT++ simulation of CPMnet)
// Author: Weihai Yu
//-------------------------------------------------------------

#ifndef __FEEDER_H
#define __FEEDER_H

// Feeder is the workload generator

#include "cpmcommon.h"
#include "agent.h"

class Feeder : public cSimpleModule
{
public:
	Feeder ();
	virtual ~Feeder();

protected:
	virtual void initialize ();
	virtual void handleMessage (cMessage *msg);
	virtual void finish ();

private:
	void fillAutoAttr (xmlNodePtr node, int auto_size, int *auto_vals);
	void fillAutoindex (xmlNodePtr node, int auto_size);
	void generateRandomIndex (int size, int *agents);
	bool alreadyGenerated (int agent, int *agents, int sofar);
	xmlNodePtr makeAllocNode (int autosize, int *autoindex);
	xmlDocPtr makeCpmDoc (xmlNodePtr node);
	// xmlChar* makeCPMxstr (xmlNodePtr node);
	// CPmessage* makeRunCPmsg (int autosize, int *autoindex);
	// void runRandomCpm ( simtime_t when);
	// void runRepeatCpm (simtime_t when, CPmessage* cmsg);
	void runCpm (simtime_t when, CPmessage* cmsg = NULL);
	void runCpm_static (simtime_t when, CPmessage* cmsg = NULL);
	bool debugging, instantiate;
	int addr_agent[20];
	Agent *agent_0;
	int num_agents, num_servers, mpl, num_repeats, index;
	int flow_num_branches, flow_num_steps, flow_size;
	int64 cmsg_byte_length;
	simtime_t flow_ia_time;
	xmlDocPtr doc;
	xmlNodePtr runcpm_template_node;
};


#endif // __FEEDER_H
