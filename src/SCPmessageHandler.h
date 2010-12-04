//-------------------------------------------------------------
// File: SCPmessageHandler.h
//     (part of OMNeT++ simulation of CPMnet)
// Author: Weihai Yu
//-------------------------------------------------------------



#ifndef __SCPMESSAGEHANDLER_H
#define __SCPMESSAGEHANDLER_H


#include <omnetpp.h>

#include "SCPmessage_m.h"

class Agent;

class SCPmessageHandler
{
public:
	SCPmessageHandler (Agent *agent_ptr, cMessage *msg = NULL);
	~SCPmessageHandler () {};
	void releaseMessage () { delete smsg; };
	bool isNull () const { return (smsg == NULL); };
	SCPmessageHandler* dup ();
	SCPmessage* Msg() { return smsg; };
	SCPmessage* dupMsg() { return check_and_cast<SCPmessage *>(smsg->dup()); };
	
	// field getter/setter methods
	SCPmessageHandler* set ( cMessage *msg );
	int getSrc() const;
	SCPmessageHandler* setSrc (int src_var);
	int getDest() const;
	SCPmessageHandler* setDest (int dest_var);
	const char * getCmd () const;
	SCPmessageHandler* setCmd (const char * cmd_var);
	bool getProcessed () const;
	SCPmessageHandler* setProcessed (bool processed_var);
	
	bool fromLocal () const;
	SCPmessageHandler* forward (int remote_id);

private:
	SCPmessage *smsg;
	Agent *agent;
};


#endif // __SCPMESSAGEHANDLER_H
