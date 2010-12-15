//-------------------------------------------------------------
// File: sink.h
//     (part of OMNeT++ simulation of CPMnet)
// Author: Weihai Yu
//-------------------------------------------------------------


#ifndef __SINK_H
#define __SINK_H

#include "cpmcommon.h"

class Sinkone : public cSimpleModule
{
  public:
    Sinkone();
    virtual ~Sinkone();

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
	virtual void finish();
	
  private:
	cGate *feeder;
	bool debugging;
    int num_servers, num_engines, mpl, flow_num_branches, flow_num_steps;
    int num_done, num_rolledback, num_sunk;
	// succeeded flows
	simtime_t total_e2e_time_s, total_comm_time_s;
	simtime_t total_interpret_time_s, total_proc_time_s;
	simtime_t total_queuing_time_s, total_queuing_delay_s, total_skew_s;
	// failed flows
	simtime_t total_e2e_time_f, total_comm_time_f;
	simtime_t total_interpret_time_f, total_proc_time_f;
	simtime_t total_queuing_time_f, total_queuing_delay_f, total_skew_f;
	simtime_t total_recovery_time, total_fail_detect_time;
	
	simtime_t record_start_time;
	bool record_started;
	
	bool recordStarted ();
	
	
	cOutVector s_delay_vec;
	cOutVector f_delay_vec;
	cOutVector rec_time_vec;
	cDoubleHistogram e2e_s_hist;
	cDoubleHistogram e2e_f_hist;
	cDoubleHistogram rec_t_hist;
};

#endif // __SINK_H

