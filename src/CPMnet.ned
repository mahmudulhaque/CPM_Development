//------------------------------------------------------------
// File: CPMnet.ned
//
//  OMNeT++  simulation of p2p BPEL enactment
// 
//  Weihai Yu created May 2007
//------------------------------------------------------------


// Feeder --
//
// A feeder generates flows
//
//import inet.nodes.inet.StandardHost;

// Sink --
//
// A sink collects results
//
package cpm.src;


// Agent --
//
// CPM agent
//


//endsimple 

// chennels

import inet.nodes.inet.Router;
import inet.nodes.inet.StandardHost;
import ned.DatarateChannel;
import inet.networklayer.autorouting.FlatNetworkConfigurator;
import inet.applications.tcpapp.TCPApp;
import inet.nodes.ethernet.EtherSwitch;
import cpm.src.Feeder;

channel InterProc extends ned.DatarateChannel
{
    parameters:
        delay = normal(50us, 0.0001);
        ber = 0;
        datarate = 1000Mbps;
}

channel IntraLan extends ned.DatarateChannel
{
    parameters:
      //  delay = normal(1ms, 0.0001);
		delay = 1ms;
        ber = 0;
        datarate = 100Mbps;
}

channel InterLan extends ned.DatarateChannel
{
    parameters:
      //  delay = normal(5ms, 0.0001);
		delay = 50ms;
        ber = 0;
        datarate = 10Mbps;
}

// CPMnet --
//
network CPMnet
{
    parameters:
        bool debugging;
        bool instantiate; // whether to instantiate for decentralization
        int num_engines; // 0 for decentralized
		// time for dispatching to engine when centralized with multiple engine,
        volatile double engine_dispatch_time @unit(ms); 
        string cpm_file;
        int num_agents;
        int agent_dups; // number of servers = (num_agents-1)*agent_dups
        int mpl; // multi-programming level of agents
        int num_repeats; // number of repeated runs for each process
		                 // 0, no repeat
		                 // greater than 1000, repeat for ever
        int flow_num_branches;
        int flow_num_steps;
        volatile int cmsg_byte_length;
        volatile int smsg_byte_length;
        // start recording after this time to avoid noises
        double record_start_time @unit(s);
    submodules:
        configurator: FlatNetworkConfigurator {
            @node();
            @labels(node,Router StandardHost);
            networkAddress = "145.236.0.0";
            netmask = "255.255.0.0";
            @display("p=550,44;i=block/cogwheel");

        }
        feeder: Feeder {
            parameters:
                @display("i=block/process,cyan;p=160,20");
        }
		//sink: Sink {
		  //  parameters:
		    //    @display("i=block/process,gold;p=360,80");
		
		
		//    gates:
		 //       from_agent[num_agents];
		//}
		// sink will also be on top of StandardHost
		sink: StandardHost{
		}
		
       // agent[num_agents]: Agent {
         //   parameters:
           //     @display("i=device/server,blue;p=20,100,ring,200,100");

            //gates:
              //  to_agent[num_agents];
               // from_agent[num_agents];
         //}
         // working agents on StandardHost 
        agent[num_agents]: StandardHost {
            //@display("p=52,112,ring,80,80;i=device/pc2");
             //@display("i=device/server,blue;p=20,100,ring,200,100");

        }
        switch1: EtherSwitch {
            @display("p=75,142");
        }
        router: Router {
            @display("p=357,70;i=abstract/router");
        }
        switch2: EtherSwitch {
            @display("p=500,142");
        }
        switch3: EtherSwitch {
            @display("p=357,200");
        }
    connections allowunconnected:

	    // TODO: more realistic connections than all-connected??
       
       // for i=0..num_agents-1, for j=0..num_agents-1 {
         //   agent[i].to_agent[j] --> InterLan --> agent[j].from_agent[i];
        //}
        
        //for i=0..num_agents-1, for j=0..num_agents-1 {
          //  agent[i].to_agent[j] --> InterLan --> agent[j].from_agent[i];
        //}
       
     //   for i=0..num_agents-1 {
           // agent[i].to_sink --> {  delay = 0s; } --> sink.from_agent[i];
       // }
       
       // connecting with switch and routers
       
       for i=0..(num_agents/2-1) {
            agent[i].ethg++ <--> switch1.ethg++;
        }
        for i=((num_agents)/2)..num_agents-1 {
            agent[i].ethg++ <--> switch3.ethg++;
        }
        sink.ethg++ <--> switch2.ethg++;
        router.ethg++ <--> switch1.ethg++;
        router.ethg++ <--> switch2.ethg++;
        router.ethg++ <--> switch3.ethg++;
}


