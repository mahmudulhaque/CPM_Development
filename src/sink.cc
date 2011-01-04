//-------------------------------------------------------------
// File: sink.cc
//     (part of OMNeT++ simulation of CPMnet)
// Author: Weihai Yu
//-------------------------------------------------------------


#ifndef __SINK_CC
#define __SINK_CC

#include "sink.h"


//Define_Module(Sink);
Define_Module(Sinkone);

Sinkone::Sinkone()
{
	num_done = 0;
	num_rolledback = 0;
	num_sunk = 0;
	
	total_e2e_time_s = 0.0;
	total_comm_time_s = 0.0;
	total_interpret_time_s = 0.0;
	total_proc_time_s = 0.0;
	total_queuing_time_s = 0.0;
	total_queuing_delay_s = 0.0;
	total_skew_s = 0.0;
	
	total_e2e_time_f = 0.0;
	total_comm_time_f = 0.0;
	total_interpret_time_f = 0.0;
	total_proc_time_f = 0.0;
	total_queuing_time_f = 0.0;
	total_queuing_delay_f = 0.0;
	total_skew_f = 0.0;
	total_recovery_time = 0.0;
	total_fail_detect_time = 0.0;
	
	record_started = false;
}

Sinkone::~Sinkone()
{
}

bool Sinkone::recordStarted ()
{
	if (!record_started)
	{
		if (simTime() < record_start_time)
			return false;
		else
			record_started = true;
	}
	return true;
}

void Sinkone::initialize()
{
	// CPMnet parameters
	feeder = getParentModule()->getSubmodule("feeder")->gate("from_sink");
	debugging = getParentModule() -> par("debugging");
	int num_agents = (int) getParentModule() -> par("num_agents");
	int agent_dups = (int) getParentModule() -> par("agent_dups");
	num_servers = (num_agents - 1) * agent_dups;
	num_engines = (int) getParentModule() -> par("num_engines");
	
	mpl = (int) getParentModule() -> par("mpl");
	flow_num_branches = (int) getParentModule() -> par("flow_num_branches");
	flow_num_steps = (int) getParentModule() -> par("flow_num_steps");
	
	record_start_time = (double) getParentModule() -> par("record_start_time");
	s_delay_vec.setName("E2E_delay_succ");
	f_delay_vec.setName("E2E_delay_fail");
	rec_time_vec.setName("Recovery_time");
	e2e_s_hist.setName("e2e_succ_histogram");
	e2e_f_hist.setName("e2e_fail_histogram");
	rec_t_hist.setName("rec_time_histogram");
	e2e_s_hist.setNumCells(10);
	e2e_f_hist.setNumCells(10);
	rec_t_hist.setNumCells(10);
	e2e_s_hist.setRangeAutoUpper(0, 50, 1.6);
	e2e_f_hist.setRangeAutoUpper(0, 50, 1.6);
	rec_t_hist.setRangeAutoUpper(0, 50, 1.6);
}

void Sinkone::handleMessage(cMessage *msg)
{
	CPmessage *cmsg = check_and_cast<CPmessage *>(msg);
	simtime_t e2e_delay = simTime() - cmsg->getStartTime();
	simtime_t recovery_time = cmsg->getRollingback()?
						(simTime() - cmsg->getRollbackStartTime()):0;
	simtime_t fail_detect_time = cmsg->getRollingback()?
						(cmsg->getFailLastDetectTime()
						 - cmsg->getRollbackStartTime())
						:0;
	if (debugging)
	{
		ev << cmsg->getCpm()
		   << "\n----------\n"
		   << "Agents visited ("
		   << cmsg->getNumSitesVisited()
		   << " in the slowest path): ";
		for (int i=0; i < cmsg->getNumSitesVisited(); i++)
		{
			ev << "-" << cmsg->getSitesVisited(i);
		}

		ev << "\nEnd-to-end delay:    " << e2e_delay
		   << "\nRecovery time:       " << recovery_time
		   << "\nFail Detect time:    " << fail_detect_time
		   << "\nCommunication time:  " << cmsg->getCommTime()
		   << "\nQueuing time:        " << cmsg->getQueueTime()
		   << "\nQueuing delay:       " << cmsg->getQueueDelay()
		   << "\nSkew:                " << cmsg->getSkew()
		   << "\nInterpretation time: " << cmsg->getInterpretTime()
		   << "\nProcessing time:     " << cmsg->getProcTime()
		   << "\n----------\n";	
	}
	
	if ( cmsg->getMainflow() )
	{
		ev << "This is a main flow\n";
		
		if ( recordStarted() )
		{
			num_sunk++;
			num_done++;
			
			if ( cmsg->getRollingback() )
				{
					num_rolledback++;
					total_recovery_time += recovery_time;
					total_fail_detect_time += fail_detect_time;
					total_e2e_time_f += e2e_delay;
					total_comm_time_f += cmsg->getCommTime();
					total_interpret_time_f += cmsg->getInterpretTime();
					total_proc_time_f += cmsg->getProcTime();
					total_queuing_time_f += cmsg->getQueueTime();
					total_queuing_delay_f += cmsg->getQueueDelay();
					total_skew_f += cmsg->getSkew();	
					f_delay_vec.record (e2e_delay);
					rec_time_vec.record (recovery_time);
					e2e_f_hist.collect(e2e_delay);
					rec_t_hist.collect(recovery_time);
			}
			else
			{
				total_e2e_time_s += e2e_delay;
				total_comm_time_s += cmsg->getCommTime();
				total_interpret_time_s += cmsg->getInterpretTime();
				total_proc_time_s += cmsg->getProcTime();
				total_queuing_time_s += cmsg->getQueueTime();
				total_queuing_delay_s += cmsg->getQueueDelay();
				total_skew_s += cmsg->getSkew();
				s_delay_vec.record (e2e_delay);
				e2e_s_hist.collect(e2e_delay);
			}
		}

		 //if (debugging)
		// {
		 //	delete cmsg;
		//	return;
		 //}
		sendDirect ( cmsg, feeder );
	}
	else
	{
		if ( recordStarted() )
			num_sunk++;
		ev << "This is not a main flow\n";
		delete cmsg;		
	}
}

void Sinkone::finish ()
{
	ev << "Sink: " << num_servers << " servers, ";
	if (num_engines == 0)
		ev << "decentralized\n";
	else
		ev << num_engines << " engines\n";
	if (num_done == 0)
	{
		ev << "No flow finished!\n";
		return;
	}
	
	ev << " mpl " << mpl << ", ";
	ev << flow_num_branches << " branches " << flow_num_steps << " steps\n";
	ev << num_done << " finished (of " << num_sunk << " sunk)\n";
	ev << num_rolledback << " rolled back\n";
	ev << "Now: " << simTime() << " record_start_time: " << record_start_time << endl;
	simtime_t interval = simTime() - record_start_time;
	int num_succ = num_done - num_rolledback;
	
	// display performance of succeeded flows
	if (num_done == 0) return;
	double throughput = num_succ/interval;
	if (num_succ > 0)
	{
		ev << "Throughput total (flows per sec):  " << throughput << '\n';
		ev << "Throughput service (services per sec per server):  ";
		ev << (throughput*flow_num_branches*flow_num_steps) / num_servers << '\n';
		if (num_engines > 0)
			ev << "Throughput engine (flows per sec):  " << throughput/num_engines << '\n';
		ev << "average end-to-end delay:    " << total_e2e_time_s/num_succ << '\n';
		ev << "average communication time:  " << total_comm_time_s/num_succ << '\n';
		ev << "average interpretation time: " << total_interpret_time_s/num_succ << '\n';
		ev << "average processing time:     " << total_proc_time_s/num_succ << '\n';
		ev << "average queuing time:        " << total_queuing_time_s/num_succ << '\n';
		ev << "average queuing delay:       " << total_queuing_delay_s/num_succ << '\n';
		ev << "average Skew:                " << total_skew_s/num_succ << '\n';		
	}

	
	ev << "fail rate:      " << (double) num_rolledback/num_done << '\n';
	// display performance of failed flows
	double throughput_rollback = num_rolledback/interval;
	if (num_rolledback >0)
	{
		ev << "Rolled back flows:\n";
		ev << "Rollbabk-throughput total (flows per sec):  " << throughput_rollback << '\n';
		ev << "Rollbabk-throughput service (services per sec per server):  ";
		ev << (throughput_rollback*flow_num_branches*flow_num_steps) / num_servers << '\n';
		if (num_engines > 0)
			ev << "Rollbabk-throughput engine (flows per sec):  "
				<< throughput_rollback/num_engines << '\n';
		ev << "average recovery time:       " << total_recovery_time/num_rolledback << '\n';	
		ev << "average lastfail detect time:" << total_fail_detect_time/num_rolledback << '\n';
		ev << "average end-to-end delay:    " << total_e2e_time_f/num_rolledback << '\n';
		ev << "average communication time:  " << total_comm_time_f/num_rolledback << '\n';
		ev << "average interpretation time: " << total_interpret_time_f/num_rolledback << '\n';
		ev << "average processing time:     " << total_proc_time_f/num_rolledback << '\n';
		ev << "average queuing time:        " << total_queuing_time_f/num_rolledback << '\n';
		ev << "average queuing delay:       " << total_queuing_delay_f/num_rolledback << '\n';
		ev << "average Skew:                " << total_skew_f/num_rolledback << '\n';		
	}
		
	recordScalar("MPL", mpl);
	// record performance of succeeded flows
	if (num_succ > 0)
	{
		recordScalar("Total-fail-rate", (double) num_rolledback/num_done);
		recordScalar("Throughput-total-succ", throughput);
		recordScalar("Throughput-server",
						(throughput*flow_num_branches*flow_num_steps) / num_servers );
		if (num_engines > 0)
			recordScalar("Throughput-engine", throughput/num_engines);
		recordScalar("End-to-end-delay", total_e2e_time_s/num_succ);
		recordScalar("Interpretation-time", total_interpret_time_s/num_succ);
		recordScalar("Processing-time", total_proc_time_s/num_succ);
		recordScalar("Communication-time", total_comm_time_s/num_succ);
		recordScalar("Queuing-delay", total_queuing_delay_s/num_succ);
		recordScalar("Queuing-time", total_queuing_time_s/num_succ);
		recordScalar("Skew", total_skew_s/num_succ);		
	}

	
	// record performance of failed flows
	if (num_rolledback >0)
	{
		recordScalar("Rollback-throughput-total", throughput_rollback);
		recordScalar("Rollback-throughput-server",
						(throughput_rollback*flow_num_branches*flow_num_steps) / num_servers );
		if (num_engines > 0)
			recordScalar("Rollback-throughput-engine", throughput_rollback/num_engines);
		recordScalar("recovery-time", total_recovery_time/num_rolledback);
		recordScalar("Failed-end-to-end-delay", total_e2e_time_f/num_rolledback);
		recordScalar("Failed-interpretation-time", total_interpret_time_f/num_rolledback);
		recordScalar("Failed-processing-time", total_proc_time_f/num_rolledback);
		recordScalar("Failed-communication-time", total_comm_time_f/num_rolledback);
		recordScalar("Failed-queuing-delay", total_queuing_delay_f/num_rolledback);
		recordScalar("Failed-queuing-time", total_queuing_time_f/num_rolledback);
		recordScalar("Failed-skew", total_skew_f/num_rolledback);		
	}
	e2e_s_hist.record();
	e2e_f_hist.record();
	rec_t_hist.record();

}


#endif // __SINK_CC

