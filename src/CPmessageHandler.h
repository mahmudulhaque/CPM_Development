//-------------------------------------------------------------
// File: CPmessageHandler.h
//     (part of OMNeT++ simulation of CPMnet)
// Author: Weihai Yu
//-------------------------------------------------------------



#ifndef __CPMMESSAGEHANDLER_H
#define __CPMMESSAGEHANDLER_H


#include <omnetpp.h>

#include "CPmessage_m.h"
// #include "scpact.h"

class Agent;

class CPmessageHandler
{
public:
	CPmessageHandler (Agent *agent_ptr, cMessage *msg = NULL);
	~CPmessageHandler () {};
	void releaseMessage () { delete cmsg; };
	bool isNull () const { return (cmsg == NULL); };
	CPmessageHandler* dup ();
	CPmessage* Msg() { return cmsg; };
	CPmessage* dupMsg() { return check_and_cast<CPmessage *>(cmsg->dup()); };
	
	int64 getByteLength ();
	CPmessageHandler* setByteLength (int64 l);
	
	// field getter/setter methods
	CPmessageHandler* set ( cMessage *msg );
	int getSrc() const;
	CPmessageHandler* setSrc (int src_var);
	int getDest() const;
	CPmessageHandler* setDest (int dest_var);
	const char * getCpm () const;
	CPmessageHandler* setCpm (const char * cpm_var);
	bool getRollingback () const;
	CPmessageHandler* setRollingback (bool rollingback_var);
	bool getMainflow () const;
	CPmessageHandler* setMainflow (bool mainflow_var);
	bool getInterpreted () const;
	CPmessageHandler* setInterpreted (bool interpreted_var);
	bool getProcessed () const;
	CPmessageHandler* setProcessed (bool processed_var);
	bool getFailed () const;
	CPmessageHandler* setFailed (bool failed_var);
	simtime_t getTimestamp() const;
	CPmessageHandler* setTimestamp ();
	simtime_t getProcTime () const;
	CPmessageHandler* setProcTime (simtime_t procTime_var);
	CPmessageHandler* increaseProcTime ();
	simtime_t getInterpretTime () const;
	CPmessageHandler* setInterpretTime (simtime_t interpretTime_var);
	CPmessageHandler* increaseInterpretTime ();
	simtime_t getQueueTime () const;
	CPmessageHandler* setQueueTime (simtime_t queueTime_var);
	simtime_t getQueueDelay () const;
	CPmessageHandler* setQueueDelay (simtime_t queueTime_var);
	CPmessageHandler* increaseQueueTime ();
	simtime_t getSkew () const;
	CPmessageHandler* setSkew (simtime_t skew_var);
	simtime_t getCommTime () const;
	CPmessageHandler* setCommTime (simtime_t commTime_var);
	CPmessageHandler* increaseCommTime ();
	simtime_t getFailLastDetectTime () const;
	CPmessageHandler* setFailLastDetectTime ();
	CPmessageHandler* setFailLastDetectTime (simtime_t failLastDetectTime_var);
	simtime_t getRollbackStartTime () const;
	CPmessageHandler* setRollbackStartTime ();
	CPmessageHandler* setRollbackStartTime (simtime_t rollbackStartTime_var);
	int getNumSitesVisited () const;
	CPmessageHandler* setNumSitesVisited (int numSitesVisited_var);
	unsigned int getSitesVisitedArraySize () const;
	int getSitesVisited (unsigned int k) const;
	CPmessageHandler* setSitesVisited (unsigned int k, int sitesVisited_var);
	CPmessageHandler* insertAgentVisited (int agent_addr);
	

	bool fromLocal () const;
	CPmessageHandler* forward (int remote_id);
	CPmessageHandler* terminate ();
	CPmessageHandler* mergeMsg (int matchkey);

private:
	CPmessage *cmsg;
	Agent *agent;
};


#endif // __CPMMESSAGEHANDLER_H
