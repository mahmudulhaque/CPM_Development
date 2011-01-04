//-------------------------------------------------------------
// File: agent.cc
//     (part of OMNeT++ simulation of CPMnet)
// Author: Weihai Yu
//-------------------------------------------------------------


#ifndef __AGENT_CC
#define __AGENT_CC

#include "agent.h"

#define MSGKIND_CONNECT  10
#define MSGKIND_SEND     11
#define MSGKIND_SENDER	 12



Define_Module(Agent);

Agent::Agent()
{
	site = NULL;
	cpm = NULL;
	cmsgh = new CPmessageHandler (this);
	num_cpus_busy = 0;
	total_interpret_time = 0.0;
	total_proc_time = 0.0;
	total_queuing_time = 0.0;
	total_scp_q_time = 0.0,
	total_scpm_time = 0.0;
	site_failrate = 0.0;
	fail_detection_time = 0.0;
	num_interpreted_cmsg = 0;
	num_processed_cmsg = 0;
	num_queued_cmsg = 0;
	num_queued_smsg = 0;
	num_processed_smsg = 0;
	smsg_byte_length = 0;
	centralized = false;
	timeoutMsg = NULL; //for tcp
}

Agent::~Agent()
{
	delete cmsgh;
	cancelAndDelete(timeoutMsg); // for tcp
}

/*
int Agent::sendTo ( int dest, cPacket *msg)
{
	(if (channels[dest]->isBusy())
		return sendDelayed (msg,
				 channels[dest]->getTransmissionFinishTime() - simTime(),
				 agents[dest]);
	else
		return send (msg, agents[dest]);

	// ***for tcp***
	char connectAddress [20];
	socket.renewSocket();
	sprintf(connectAddress, "agent[%d]", dest);
	int connectPort = par("connectPort");
	socket.setOutputGate(gate("tcpOut"));
	socket.connect(IPAddressResolver().resolve(connectAddress), connectPort);
	socket.send(msg);
	socket.close();
	return 1;
}
*/

int Agent::sendTo ( int dest, cMessage *msg)
{
	/*(if (channels[dest]->isBusy())
		return sendDelayed (msg,
				 channels[dest]->getTransmissionFinishTime() - simTime(),
				 agents[dest]);
	else
		return send (msg, agents[dest]);
		*/
	// ***for tcp***

	ev << "in the send to function: " << endl;
	char connectAddress [20];
	uint32 numBytes;
	socket.renewSocket();
	sprintf(connectAddress, "agent[%d]", dest);
	int connectPort = par("connectPort");
	socket.setOutputGate(gate("tcpOut"));
	socket.connect(IPAddressResolver().resolve(connectAddress), connectPort);
	ev << "Agent:: sending to: " << IPAddressResolver().resolve(connectAddress) << " connecting port: "<< connectPort << "socket status: "<< socket.getState()<< " msg kind: " << msg->getKind() ;

	/*
	switch (msg->getKind())
					 	{
					 		case CPM:
					 		{
					 			CPmessage* cmsg = check_and_cast<CPmessage *>(msg);
					 			//cmsg->setKind(1);
					 			//ev<< " Original message sending: ";
					 			numBytes = (uint32)getParentModule()->getParentModule()->par("cmsg_byte_length");
					 			cmsg->setByteLength(numBytes);
					 			ev << " Msg kind: from cpm " << cmsg->getKind()  << endl;

					 			socket.send(cmsg);
					 			break;
					 		}
					 		case SCPM:
					 		{
					 			SCPmessage* smsg = check_and_cast<SCPmessage *>(msg);
					 			ev << " Msg kind: from scpm" << msg->getKind()  << endl;
					 			smsg->setByteLength(smsg_byte_length);
					 			socket.send(smsg);
					 			break;
					 		}

					 	}
	*/
	if (strcmp(msg->getName(), _CPM)==0)
	{
		CPmessage* cmsg = check_and_cast<CPmessage *>(msg);
		numBytes = (uint32)getParentModule()->getParentModule()->par("cmsg_byte_length");
		cmsg->setByteLength(numBytes);
		ev << " Msg kind: from cpm " << cmsg->getKind()  << endl;
		socket.send(cmsg);

	}
	else if (strcmp(msg->getName(), _SCPM)==0)
	{
		SCPmessage* smsg = check_and_cast<SCPmessage *>(msg);
		ev << " Msg kind: from scpm" << msg->getKind()  << endl;
		smsg->setByteLength(smsg_byte_length);
		socket.send(smsg);

	}


	//socket.close();
	return 1;
}


void Agent::feed (CPmessage *cmsg)
{
	Enter_Method ("feed");
	take (cmsg);
	handleCPmessage (cmsg);
	scheduleCPU ();
}


bool Agent::serviceOk (bool is_rollingback, double svc_failrate)
{
	// rollback always succeeds
	// normal processing succeeds when both site and svc are ok
	return (is_rollingback ||
			uniform (0.0, 1.0) <= (1.0 - site_failrate)*(1.0 - svc_failrate));
}

void Agent::processSvc (CPmessageHandler *msgh, double svc_failrate)
{
	msgh->setProcessed (true);
	if (serviceOk(msgh->getRollingback(), svc_failrate))
	{
		scheduleAt (simTime() + proc_time*normal(1, 0.2), msgh->Msg());
	}
	else
	{	
		// the service just failed
		msgh->setFailed(true);
		scheduleAt (simTime() + fail_detection_time*normal(1, 0.2),
			msgh->Msg());
	}
	// because enact will return, handleCPmessage will release the cpu
	// increase num_cpus_busy will then hold the cpu
	num_cpus_busy++;
	cpu_vec.record(num_cpus_busy);
	ev << "Agent[" << getParentModule()->getIndex() << "]process: use a CPU. Now ";
	ev << num_cpus_busy << " busy" << endl;
}

void Agent::initialize()
{
// CPMnet parameters
	//int agent_dups = (int) getParentModule() -> par("agent_dups");
	int agent_dups = (int) getParentModule()->getParentModule() -> par("agent_dups");
// Agent parameters
	num_cpus = agent_dups;
	interpret_time = par("interpret_time").doubleValue()/1000;
	proc_time = par("proc_time").doubleValue()/1000;
	scpm_proc_time = par("scpm_proc_time").doubleValue()/1000;
	site_failrate = par("site_failrate").doubleValue();
	fail_detection_time = par("fail_detection_time").doubleValue()/1000;
	//smsg_byte_length = (long) getParentModule() -> par("smsg_byte_length");
	smsg_byte_length = (long) getParentModule()->getParentModule() -> par("smsg_byte_length");

	id = getParentModule()->getIndex();
	ev << "Agent:: ID is, " << id << endl;
	//int num_engines = (int) getParentModule() -> par("num_engines");
	int num_engines = (int) getParentModule()->getParentModule() -> par("num_engines");
	if ( num_engines > 0 )
	{
		centralized = true;
		if (id == 0) // This is the central engine
		{
			num_cpus = num_engines;
			//interpret_time += getParentModule()
				//		 	->par("engine_dispatch_time").doubleValue()/1000;
			interpret_time += getParentModule()->getParentModule()
			 	->par("engine_dispatch_time").doubleValue()/1000;
		}
	}
	site = new CPMsite (id, this);
	cpm = new Cpm (site);
	
	//int num_agends = (int) getParentModule() -> par("num_agents");
	int num_agends = (int) getParentModule()->getParentModule() -> par("num_agents");
	agents = new cGate *[num_agends];
	channels = new cChannel *[num_agends];
	//does not need this loop since tcp is implemented
	/*for (int i = 0; i < num_agends; i++)
	{
		agents[i] = gate("to_agent", i);
		channels[i] = agents[i]->getChannel();
	}
	*/

	q_vec.setName("Queuing-Time");
	cpu_vec.setName("Num-CPU-busy");
	// q_stats.setName("Queue-Stats");


	//for tcp connections

	numSessions = numBroken = packetsSent = packetsRcvd = bytesSent = bytesRcvd = bytesRcvdSink = numRequestsToSend = 0;;
	earlySend = false;  // TBD make it parameter
	WATCH(numSessions);
	WATCH(numBroken);
	WATCH(packetsSent);
	WATCH(packetsRcvd);
	WATCH(bytesSent);
	WATCH(bytesRcvd);
	WATCH(numSendBack);
	WATCH(bytesRcvdSink);
    WATCH(numRequestsToSend);
    WATCH(earlySend);
	const char *address = par("address");
	int port = par("port");
    int receivePort = par("receivePort");
    socketReceive.setOutputGate(gate("tcpOut"));
    socketReceive.bind(address[0] ? IPvXAddress(address) : IPvXAddress(), receivePort);
    socketReceive.setCallbackObject(this, NULL);
    socketReceive.listen();
}
void Agent::socketDataArrived(int, void *, cPacket *msg, bool) {
    ev << "Received TCP data, " << msg->getByteLength() << " bytes\\n";
    delete msg;
}
void Agent::socketFailure(int, void *, int code) {
    if (code==TCP_I_CONNECTION_RESET)
        ev << "Connection reset!\\n";
    else if (code==TCP_I_CONNECTION_REFUSED)
        ev << "Connection refused!\\n";
    else if (code==TCP_I_TIMED_OUT)
        ev << "Connection timed out!\\n";
}


void Agent::handleMessage(cMessage *msg)
{
	/*
	if (socketReceive.belongsToSocket(msg))
		{
			socketReceive.processMessage(msg);

		}
	scheduleCPU ();
	*/


	ev <<"Agent:: Receive socket state: "<< socketReceive.getState() << " testing msg kind: "<< msg->getKind() << "Message name: " << msg->getName()<< " Sender Module:" << getParentModule()->getModuleByRelativePath( msg->getSenderModule()->getFullName())->getFullPath() << " Receiving Module: "<< getParentModule()->getModuleByRelativePath(msg->getArrivalModule()->getFullName())->getFullPath()<<endl;
	//for tcp connections
	if (msg->isSelfMessage())
	{
		ev << "Agent::in handle message" << endl;
		/*switch (msg->getKind())
				 	{
				 		case CPM:
				 		{
				 			ev <<"cpm is called from handle"<<endl;
				 			handleCPmessage (msg);
				 			break;
				 		}


				 		case SCPM:
				 		{
				 			ev <<"scpm is called from handle"<<endl;
				 			handleSCPmessage (msg);
				 			break;
				 		}
				 		default:
				 		{
				 			ev << "Agent::handleMessage: unknown message type!\n";
				 		}
				 	}
				 	*/
		if (strcmp(msg->getName(), _CPM)==0)
			{
				ev <<"Agent::cpm is called from handle"<<endl;
				ev << ((CPmessage *)msg)->getCpm()<< endl;
				handleCPmessage (msg);
			}
		else if (strcmp(msg->getName(), _SCPM)==0)
				{
					ev <<"Agent::scpm is called from handle"<<endl;
					handleSCPmessage (msg);
				}

	}


	else if (strcmp(msg->getName(), _CPM)==0||strcmp(msg->getName(), _SCPM)==0)
	{
		ev << "Agent::Message name in else if: " << msg->getName() <<endl;

		if (msg->getKind()==TCP_I_DATA || msg->getKind()==TCP_I_URGENT_DATA)
			{
				msg->removeControlInfo();

				if(strcmp(msg->getName(), _CPM)==0)
					{
						CPmessage *cpmsg = check_and_cast<CPmessage *>(msg);
						if (cpmsg)
							{
								ev<< "Agent::CPM Message with tcp Connection" << endl;
								//ev << ((CPmessage *)msg)->getCpm()<< endl;
								ev<< cpmsg->getCpm()<< endl;
								handleCPmessage(cpmsg);
								ev<< "Agent::successfull in cpm " << endl;

							}
					}
				else if (strcmp(msg->getName(), _SCPM)==0)
					{

						SCPmessage *scpmsg = check_and_cast<SCPmessage *>(msg);
						if (scpmsg)
							{
	        	        	handleSCPmessage(scpmsg);
	        	        	ev<< "Agent::successfull in scpm " << endl;

							}
					}


			}


	}
	 else
	    {

	    }



	scheduleCPU ();
}

void Agent::handleCPmessage(cMessage *msg)
{
	cmsgh->set(msg);
	
	if (cmsgh->getProcessed())
	{
		cmsgh->setProcessed (false);
		cmsgh->increaseProcTime ();
	
		if ((!cmsgh->getRollingback()) && cmsgh->getFailed()
			&& centralized && id != 0)
		{
			ev << "Processing failed, forwarding to engine.\n";
			cmsgh->forward(0);
		}
		else
		{
			ev << "\n::calling from Get_Processed::" << endl;
			cpm->set (cmsgh->getCpm())->dispatch(cmsgh);
		}
		num_cpus_busy--;
		cpu_vec.record(num_cpus_busy);
		ev << "Agent[" << getParentModule()->getIndex() << "]: release a CPU. Now "
			<< num_cpus_busy << " busy\n";
	}
	else
	if (cmsgh->getInterpreted())
	{
		cmsgh->increaseInterpretTime ()->setInterpreted(false);
		ev << "Agent::handleCPmessage:queue before dispatch:\n";
		//ev << site->queue->dump();
		ev << "Agent::handleCPmessage:\n" << cmsgh->getCpm() << endl;
		ev << "\n::calling from Get_Interpreted::" << endl;
		cpm->set (cmsgh->getCpm())->dispatch(cmsgh);
	
		ev << "Agent::handleCPmessage:queue after dispatch:\n";
		//ev << site->queue->dump();
		//ev << "Agent::Msg pool after installation: " << site->pool->dump() << endl;
		num_cpus_busy--;
		cpu_vec.record(num_cpus_busy);
		ev << "Agent[" << getParentModule()->getIndex() << "]: release a CPU. Now "
			<< num_cpus_busy << " busy\n";
	}
	else
	{
		if (!cmsgh->fromLocal())
		{
			//cmsgh->insertAgentVisited(id);
			cmsgh->insertAgentVisited(getParentModule()->getIndex());

			cmsgh->increaseCommTime ();
			//ev << "First time not calling from Agent: " << getIndex() << endl;
			ev << "Agent::First time not calling from Agent: " << getParentModule()->getFullName() << endl;
		}
		// setTimestamp before enqueuing such that queuing time
		// is correctly calculated
		ev << "Agent::handlecp_message calling from Agent: " << getParentModule()->getFullName() <<endl;
		cmsgh->Msg()->setTimestamp();
		site->queue->enqueue(cmsgh->Msg());
	}
}

void Agent::handleSCPmessage(cMessage *msg)
{
	SCPmessage* smsg = check_and_cast<SCPmessage *>(msg);
	if (smsg->getProcessed())
	{
		// ev <<  "Agent::handleSCPmessage - SCPmessage:\n\t"
		// << smsg->getCmd() << endl;
		num_processed_smsg++;
		total_scpm_time += scpm_proc_time;
		num_cpus_busy--;
		cpu_vec.record(num_cpus_busy);
		ev << "Agent[" << id << "]: release a CPU. Now "
			<< num_cpus_busy << " busy\n";
	
		site->reg->handleMessage (smsg->getCmd());
		// ev << site->reg->dump() << endl;
		delete smsg;
	}
	else
	{
		// setTimestamp before enqueuing such that queuing time
		// is correctly calculated
		msg->setTimestamp();
		site->scp_queue->enqueue(msg);
	}
}

void Agent::scheduleCPU ()
{
	ev << "Agent::scheduleCPU - num_cpus_busy=" << num_cpus_busy <<endl;
	while (num_cpus_busy < num_cpus)
	{
		// the scp_queue has higher priority, such that rollback etc
		// can start as soon as possible
		cMessage *msg = site->scp_queue->dequeue();
		if (msg == NULL)
			msg = site->queue->dequeue();
		if (msg == NULL)
			return;
	
		num_cpus_busy++; // use a cpu
		cpu_vec.record(num_cpus_busy);
		ev << "Agent[" << getParentModule()->getIndex() << "]: use a CPU. Now ";
		ev << num_cpus_busy << " busy" << endl;
		if (strcmp(msg->getName(), _CPM)==0)
			{
				cmsgh->set (msg);
				ev << "Agent[" << getParentModule()->getIndex() << "]: dequeued\n" ;
				ev<<"schedule cpu:\n"<< cmsgh->getCpm() << endl;
	
				cmsgh->increaseQueueTime()->setInterpreted (true);
				scheduleAt (simTime() + interpret_time*normal(1, 0.1),cmsgh->Msg());
				//while dealing with inet

				//scheduleAt (simTime() + interpret_time*truncnormal(0.5, 0.1),cmsgh->Msg());
				//scheduleAt (simTime() + interpret_time*truncnormal(1, 0.1),cmsgh->Msg());
			}
		else if (strcmp(msg->getName(), _SCPM)==0)
			{
				// code copied from cmsgh->increaseQueueTime(), bad style
				simtime_t delta = simTime() - msg->getTimestamp();
				msg->setTimestamp();
				num_queued_smsg++;
				total_scp_q_time += delta;
				q_vec.record(delta);
				// q_stats.collect(delta);
	
				// ev << "Agent[" << id << "]: dequeued\n";
				// ev << ((SCPmessage *) msg)->getCmd() << endl;
				((SCPmessage *) msg)->setProcessed(true);
				scheduleAt (simTime() + scpm_proc_time*normal(1, 0.1), msg);

			}
		else
			{
				ev << "Agent::scheduleCPU: unknown message type!\n";
			}

	}
}

void Agent::finish ()
{
	site->cleanup ();
	delete cpm;
	delete site;
	delete[] agents;
	delete[] channels;
	//if (id>2) return;
	// only report the first 3 agents
	ev << "Agent[" << getParentModule()->getIndex() << "]:\n";
	ev << num_processed_cmsg << "   processed, total time: " << total_proc_time;
	ev << " average: " << avg (total_proc_time, num_processed_cmsg) << '\n';

	ev << num_interpreted_cmsg << " interpreted, total time: " << total_interpret_time;
	ev << " average: " << avg (total_interpret_time, num_interpreted_cmsg) << '\n';

	ev << num_queued_cmsg << "  cpm queued, total time: " << total_queuing_time;
	ev << " average: " << avg (total_queuing_time, num_queued_cmsg) << '\n';

	ev << num_processed_smsg << "   scp msg, total scp proc time: ";
	ev << total_scpm_time << '\n';

	ev << num_queued_smsg << "  scp queued, total time: " << total_scp_q_time;
	ev << " average: " << avg (total_scp_q_time, num_queued_smsg) << '\n';

	simtime_t cpu_time = total_proc_time + total_interpret_time + total_scpm_time;
	double cpu_usage = cpu_time / (simTime()*num_cpus);
	double proc_part = 0.0;
	double intr_part = 0.0;
	double scpm_part = 0.0;
	if (cpu_time != 0)
	{
		proc_part = total_proc_time / cpu_time;
		intr_part = total_interpret_time /cpu_time;
		scpm_part = total_scpm_time /cpu_time;
	}

	ev << "CPU usage: " << cpu_usage << " where\n";
	ev << "  Processing     " << proc_part << '\n';
	ev << "  Interpretation " << intr_part << '\n';
	ev << "  Scp Management " << scpm_part << '\n';

	recordScalar("CPU-usage", cpu_usage);
	recordScalar("Processing-part", proc_part);
	recordScalar("Interpretation-part", intr_part);
	recordScalar("ScpManage-part", scpm_part);
	// q_stats.record();
}

simtime_t Agent::avg (simtime_t val, long num)
{
	if (num == 0)
		return 0;
	else
		return val/num;
}
#endif // __AGENT_CC

