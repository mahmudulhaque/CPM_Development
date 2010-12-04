//-------------------------------------------------------------
// File: CPmessageHandler.cc
//     (part of OMNeT++ simulation of CPMnet)
// Author: Weihai Yu
//-------------------------------------------------------------



#ifndef __CPMMESSAGEHANDLER_CC
#define __CPMMESSAGEHANDLER_CC

#include "CPmessageHandler.h"
#include "agent.h"

CPmessageHandler::CPmessageHandler(Agent *agent_ptr, cMessage *msg)
	:agent (agent_ptr)
{
	set (msg);
}

// field getter/setter methods
CPmessageHandler* CPmessageHandler::set (cMessage *msg)
{
	if (msg)
	{
		cmsg = check_and_cast<CPmessage *>(msg);
	}
	else
		cmsg = NULL;
	return this;
}

CPmessageHandler* CPmessageHandler::dup ()
{
	return new CPmessageHandler(agent, (cMessage *)cmsg->dup());
}


int64 CPmessageHandler::getByteLength ()
{
	return cmsg -> getByteLength ();
}

CPmessageHandler* CPmessageHandler::setByteLength (int64 l)
{
	cmsg -> setByteLength (l);
	return this;
}

int CPmessageHandler::getSrc () const
{
	return cmsg->getSrc();
}

CPmessageHandler* CPmessageHandler::setSrc (int src_var)
{
	cmsg->setSrc(src_var);
	return this;
}

int CPmessageHandler::getDest () const
{
	return cmsg->getDest();
}

CPmessageHandler* CPmessageHandler::setDest (int dest_var)
{
	cmsg->setDest(dest_var);
	return this;
}

const char * CPmessageHandler::getCpm () const
{
	return cmsg->getCpm();
}

CPmessageHandler* CPmessageHandler::setCpm(const char * cpm_var)
{
	cmsg->setCpm(cpm_var);
	return this;
}

bool CPmessageHandler::getRollingback () const
{
	return cmsg->getRollingback();
}

CPmessageHandler* CPmessageHandler::setRollingback (bool rollingback_var)
{
	cmsg->setRollingback(rollingback_var);
	return this;
}

bool CPmessageHandler::getMainflow () const
{
	return cmsg->getMainflow();
}

CPmessageHandler* CPmessageHandler::setMainflow (bool mainflow_var)
{
	cmsg->setMainflow(mainflow_var);
	return this;
}

bool CPmessageHandler::getInterpreted () const
{
	return cmsg->getInterpreted();
}

CPmessageHandler* CPmessageHandler::setInterpreted (bool interpreted_var)
{
	cmsg->setInterpreted(interpreted_var);
	return this;
}

bool CPmessageHandler::getFailed () const
{
	return cmsg->getFailed();
}

CPmessageHandler* CPmessageHandler::setFailed (bool failed_var)
{
	cmsg->setFailed(failed_var);
	return this;
}

bool CPmessageHandler::getProcessed () const
{
	return cmsg->getProcessed();
}

CPmessageHandler* CPmessageHandler::setProcessed (bool processed_var)
{
	cmsg->setProcessed(processed_var);
	return this;
}

simtime_t CPmessageHandler::getTimestamp() const
{
	return cmsg->getTimestamp();
}

CPmessageHandler* CPmessageHandler::setTimestamp ()
{
	cmsg->setTimestamp();
	return this;
}

simtime_t CPmessageHandler::getProcTime () const
{
	return cmsg->getProcTime();
}

CPmessageHandler* CPmessageHandler::setProcTime (simtime_t procTime_var)
{
	cmsg->setProcTime(procTime_var);
	return this;
}

CPmessageHandler* CPmessageHandler::increaseProcTime ()
{
	simtime_t delta = simTime() - getTimestamp();
	setTimestamp();
	setProcTime (getProcTime() + delta);
	agent->num_processed_cmsg++;
	agent->total_proc_time += delta;
	return this;
}

simtime_t CPmessageHandler::getInterpretTime () const
{
	return cmsg->getInterpretTime();
}

CPmessageHandler* CPmessageHandler::setInterpretTime (simtime_t interpretTime_var)
{
	cmsg->setInterpretTime(interpretTime_var);
	return this;
}

CPmessageHandler* CPmessageHandler::increaseInterpretTime ()
{
	simtime_t delta = simTime() - getTimestamp();
	setTimestamp();
	setInterpretTime(getInterpretTime() + delta);
	agent->num_interpreted_cmsg++;
	agent->total_interpret_time += delta;
	return this;
}

simtime_t CPmessageHandler::getQueueTime () const
{
	return cmsg->getQueueTime();
}

CPmessageHandler* CPmessageHandler::setQueueTime (simtime_t queueTime_var)
{
	cmsg->setQueueTime(queueTime_var);
	return this;
}

simtime_t CPmessageHandler::getQueueDelay () const
{
	return cmsg->getQueueDelay();
}

CPmessageHandler* CPmessageHandler::setQueueDelay (simtime_t queueDelay_var)
{
	cmsg->setQueueDelay(queueDelay_var);
	return this;
}

CPmessageHandler* CPmessageHandler::increaseQueueTime ()
{
	simtime_t delta = simTime() - getTimestamp();
	setTimestamp();
	setQueueTime(getQueueTime() + delta);
	setQueueDelay(getQueueDelay() + delta);
	agent->num_queued_cmsg++;
	agent->total_queuing_time += delta;
	agent->q_vec.record(delta);
	// agent->q_stats.collect(delta);
	return this;
}

simtime_t CPmessageHandler::getSkew () const
{
	return cmsg->getSkew();
}

CPmessageHandler* CPmessageHandler::setSkew (simtime_t skew_var)
{
	cmsg->setSkew(skew_var);
	return this;
}

simtime_t CPmessageHandler::getCommTime() const
{
	return cmsg->getCommTime();
}

CPmessageHandler* CPmessageHandler::setCommTime (simtime_t commTime_var)
{
	cmsg->setCommTime(commTime_var);
	return this;
}

CPmessageHandler* CPmessageHandler::increaseCommTime ()
{
	setCommTime(getCommTime() + simTime() - getTimestamp());
	setTimestamp();
	return this;
}

simtime_t CPmessageHandler::getFailLastDetectTime() const
{
	return cmsg->getFailLastDetectTime();
}

CPmessageHandler* CPmessageHandler::setFailLastDetectTime ()
{
	cmsg->setFailLastDetectTime(simTime());
	return this;
}

CPmessageHandler* CPmessageHandler::setFailLastDetectTime (simtime_t failLastDetectTime_var)
{
	cmsg->setFailLastDetectTime(failLastDetectTime_var);
	return this;
}

simtime_t CPmessageHandler::getRollbackStartTime() const
{
	return cmsg->getRollbackStartTime();
}

CPmessageHandler* CPmessageHandler::setRollbackStartTime ()
{
	cmsg->setRollbackStartTime(simTime());
	return this;
}

CPmessageHandler* CPmessageHandler::setRollbackStartTime (simtime_t rollbackStartTime_var)
{
	cmsg->setRollbackStartTime(rollbackStartTime_var);
	return this;
}

int CPmessageHandler::getNumSitesVisited () const
{
	return cmsg->getNumSitesVisited();
}

CPmessageHandler* CPmessageHandler::setNumSitesVisited (int numSitesVisited_var)
{
	cmsg->setNumSitesVisited(numSitesVisited_var);
	return this;
}

unsigned int CPmessageHandler::getSitesVisitedArraySize () const
{
	return cmsg->getSitesVisitedArraySize();
}

int CPmessageHandler::getSitesVisited (unsigned int k) const
{
	return cmsg->getSitesVisited(k);
}

CPmessageHandler* CPmessageHandler::setSitesVisited (unsigned int k, int sitesVisited_var)
{
	cmsg->setSitesVisited(k, sitesVisited_var);
	return this;
}

CPmessageHandler* CPmessageHandler::insertAgentVisited (int agent_addr)
{
	setSitesVisited(getNumSitesVisited(), agent_addr);
	cmsg->setNumSitesVisited(getNumSitesVisited() + 1);
	return this;
}

bool CPmessageHandler::fromLocal () const
{
	return (getSrc() == agent->Id());
}

CPmessageHandler* CPmessageHandler::forward (int remote_id)
{
	// if (sim_centralized && agent->Id() != 0)
	// 	remote_id = 0;
	if (remote_id == -1)
	{
		ev << "Error: CPmessageHandler::forward:\n" << getCpm () 
			<< "--\n";
		return this;
	}
	setSrc (agent->Id());
	setDest (remote_id);
	setInterpreted (false);
	ev << "forward is called " << endl;
	agent->sendTo (remote_id, cmsg);

	return this;
}

CPmessageHandler* CPmessageHandler::terminate ()
{
	if (getMainflow() == false)
	{
		// for better performance
		// if we need to collect info at sink, comment out this if-block
		delete cmsg;
		return this;
	}
	setSrc (agent->Id());
	agent->send (cmsg, "to_sink");
	return this;
}

CPmessageHandler* CPmessageHandler::mergeMsg (int matchkey)
{
	int count = agent->Site()->pool->match (matchkey);
	MsgMapIt mit = agent->Site()->pool->getItor (matchkey);

	// Accumulate the timing of the branches.
	// Warning: accumulation is not correct, because the timing
	// before the fork is falsefully accumulated 'count' times!
	// For queuing time, calculate the max of the branches.
	// For rollbackStartTime, use the smallest other than 0
	simtime_t max_q_delay = getQueueDelay();
	simtime_t min_q_delay = max_q_delay;
	simtime_t max_timestamp = getTimestamp();
	simtime_t min_timestamp = max_timestamp;
	simtime_t rb_start_time = getRollbackStartTime();
	simtime_t lastfail_detect_time = getFailLastDetectTime();
	bool is_rollingback = getRollingback();
	for (int i = 1; i <= count; i++)
	{
		CPmessage *curr_msg = mit->second;
		if (curr_msg->getRollingback() != is_rollingback)
			ev << "Warning: mergeMsg - different fates!\n";

		if (curr_msg->getQueueDelay() > max_q_delay)
			max_q_delay = curr_msg->getQueueDelay();
		if (curr_msg->getQueueDelay() < min_q_delay)
			min_q_delay = curr_msg->getQueueDelay();

		if (curr_msg->getTimestamp() > max_timestamp)
			max_timestamp = curr_msg->getTimestamp();
		if (curr_msg->getTimestamp() < min_timestamp)
			min_timestamp = curr_msg->getTimestamp();

		if (curr_msg->getRollbackStartTime() > 0.0)
			if (rb_start_time == 0.0
			|| (curr_msg->getRollbackStartTime() < rb_start_time))
				rb_start_time = curr_msg->getRollbackStartTime();
		if (lastfail_detect_time < curr_msg->getFailLastDetectTime())
			lastfail_detect_time = curr_msg->getFailLastDetectTime();

		// accumulate timing info
		setInterpretTime (getInterpretTime() + curr_msg->getInterpretTime());
		setProcTime (getProcTime() + curr_msg->getProcTime());
		setQueueTime (getQueueTime() + curr_msg->getQueueTime());
		setCommTime (getCommTime() + curr_msg->getCommTime());
		delete curr_msg;
		// to do: accumulate agents visited
		++mit;
	}
	setQueueDelay (max_q_delay);
	setSkew (getSkew() + max_timestamp - min_timestamp);
	setRollbackStartTime (rb_start_time);
	setFailLastDetectTime (lastfail_detect_time);
	agent->Site()->pool->remove (matchkey);
	return this;
}
#endif // __CPMMESSAGEHANDLER_CC
