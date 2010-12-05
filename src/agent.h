//-------------------------------------------------------------
// File: agent.h
//     (part of OMNeT++ simulation of CPMnet)
// Author: Weihai Yu
//-------------------------------------------------------------


#ifndef __AGENT_H
#define __AGENT_H

#include "cpmcommon.h"
#include "cpmsite.h"
#include "cpm.h"
#include "CPmessageHandler.h"
#include <omnetpp.h>
#include "TCPGenericCliAppBase.h"
#include "INETDefs.h"
#include "TCPSocketMap.h"
#include "TCPSocket.h"
#include "TCPCommand_m.h"
#include "IPv4InterfaceData.h"
#include "InterfaceTableAccess.h"
#include "RoutingTableAccess.h"
#include "IPAddressResolver.h"
#include "IPvXAddress.h"

// #include "SCPmessageHandler.h"

class Agent : public cSimpleModule,  public TCPSocket::CallbackInterface
{
  public:
    Agent();
    virtual ~Agent();

	int Id() const { return id; };
	CPMsite* Site () const { return site; };
	//int sendTo ( int dest, cPacket *msg);
	int sendTo ( int dest, cMessage *msg);

	void feed (CPmessage *cmsg); // only for agent 0
	void processSvc (CPmessageHandler *msgh, double svc_failrate);

    int num_cpus, num_cpus_busy;
	simtime_t total_interpret_time, total_proc_time, total_queuing_time;
	simtime_t total_scp_q_time, total_scpm_time;
	long num_interpreted_cmsg, num_processed_cmsg, num_queued_cmsg;
	long num_queued_smsg, num_processed_smsg;
	int64 smsg_byte_length;
	bool centralized;
	cOutVector q_vec;
	cOutVector cpu_vec;
	// cStdDev q_stats;

  protected:

	//*******Adding for tcp support**********
	simtime_t delay;
    simtime_t maxMsgDelay;
    cMessage *timeoutMsg;
    bool earlySend;  // if true, don't wait with sendRequest() until established()
    int numRequestsToSend; // requests to send in this session
    long bytesRcvdSink; //for sink app
    int packetsRcvdSink;
    /** Utility: sends a request to the server */
    //virtual void sendRequest();
    TCPSocketMap socketMap;
    TCPSocket socketReceive;
    TCPSocket socket;
    virtual void socketDataArrived(int connId, void *yourPtr, cPacket *msg, bool urgent);
    virtual void socketFailure(int, void *, int code);
    // statistics
    int numSessions;
    int numBroken;
    int packetsSent;
    int packetsRcvd;
    int bytesSent;
    int bytesRcvd;
    int numSendBack;
    //**********************************
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
	virtual void finish();


  private:
    int id;
	simtime_t interpret_time, proc_time, scpm_proc_time;
	double site_failrate;
	simtime_t fail_detection_time;
	CPMsite *site;
	cGate **agents;
	cChannel **channels;
	Cpm *cpm;
	CPmessageHandler *cmsgh;
	bool serviceOk (bool is_rollingback, double svc_failrate);
    void handleCPmessage(cMessage *msg);
    void handleSCPmessage(cMessage *msg);
	void scheduleCPU ();
	simtime_t avg (simtime_t val, long num);

	//********* adding for tcp support***********
	/** Redefined to schedule a connect(). */




	    /** preparing the remote connection*/

	   // virtual void sendBack(cMessage *msg);
	    //virtual void sendOrSchedule(cMessage *msg, simtime_t delay);


	//*********


};

#endif // __AGENT_H

