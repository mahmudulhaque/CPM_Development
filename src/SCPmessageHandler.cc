//-------------------------------------------------------------
// File: SCPmessageHandler.cc
//     (part of OMNeT++ simulation of CPMnet)
// Author: Weihai Yu
//-------------------------------------------------------------



#ifndef __SCPMESSAGEHANDLER_CC
#define __SCPMESSAGEHANDLER_CC

#include "SCPmessageHandler.h"
#include "agent.h"

SCPmessageHandler::SCPmessageHandler(Agent *agent_ptr, cMessage *msg)
	:agent (agent_ptr)
{
	set (msg);
}

// field getter/setter methods
SCPmessageHandler* SCPmessageHandler::set (cMessage *msg)
{
	if (msg)
	{
		smsg = check_and_cast<SCPmessage *>(msg);
	}
	else
		smsg = NULL;
	return this;
}

SCPmessageHandler* SCPmessageHandler::dup ()
{
	return new SCPmessageHandler(agent, (cMessage *)smsg->dup());
}

int SCPmessageHandler::getSrc () const
{
	return smsg->getSrc();
}

SCPmessageHandler* SCPmessageHandler::setSrc (int src_var)
{
	smsg->setSrc(src_var);
	return this;
}

int SCPmessageHandler::getDest () const
{
	return smsg->getDest();
}

SCPmessageHandler* SCPmessageHandler::setDest (int dest_var)
{
	smsg->setDest(dest_var);
	return this;
}

const char * SCPmessageHandler::getCmd () const
{
	return smsg->getCmd();
}

SCPmessageHandler* SCPmessageHandler::setCmd(const char * cmd_var)
{
	smsg->setCmd(cmd_var);
	return this;
}

bool SCPmessageHandler::getProcessed () const
{
	return smsg->getProcessed();
}

SCPmessageHandler* SCPmessageHandler::setProcessed (bool processed_var)
{
	smsg->setProcessed(processed_var);
	return this;
}

bool SCPmessageHandler::fromLocal () const
{
	return (getSrc() == agent->Id());
}

SCPmessageHandler* SCPmessageHandler::forward (int remote_id)
{
	setSrc (agent->Id());
	setDest (remote_id);
	setProcessed (false);
	smsg->setByteLength (agent->smsg_byte_length);
	//agent->send (smsg, "to_agent", remote_id);
	//for tcp connections
	ev << "forward is called from scpm" << endl;
	agent->sendTo(remote_id, smsg);
	return this;
}

#endif // __SCPMESSAGEHANDLER_CC
