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
package src;


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
import src.Feeder;
import src.Sinkone;

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
        datarate = 10000Mbps;
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
        string cpm_file_static = default("default-static.xml");
        int num_switches;
        int num_routers;
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
		sink: Sinkone {
           @display("i=block/process,cyan;p=450,20");
            //@display("p=539,214");
            gates:
		        from_agent;
        }

        agent[num_agents]: StandardAgent;
        switch[num_switches]: EtherSwitch {
        }
        router[num_routers]: Router {
        }
        
    connections allowunconnected:
        for i=0..(num_agents%num_switches-1) {
            agent[i+num_switches*(ceil((num_agents/num_switches))-1)].ethg++ <--> switch[num_switches-1].ethg++; 
        }

        for i=0..(num_switches-1), for j=0..(ceil((num_agents/num_switches)-1)), if(i*(ceil(num_agents/num_switches)-1)+j+i < (num_agents - num_agents%num_switches)) {
         // for i=0..(num_switches-1), for j=0..(ceil((num_agents - num_agents%num_switches)/(num_switches-1)-1)), if(i*(ceil(num_agents/num_switches)-1)+j+i < (num_agents - num_agents%num_switches)) {  
            agent[i*(ceil(num_agents/num_switches)-1)+j+i].ethg++ <--> switch[i].ethg++;
        }
        
        for i=0..(num_routers-2) {
             //router[i].ethg++ <--> router[i+1].ethg++;
             router[i].pppg++ <-->InterLan <--> router[i+1].pppg++;
            
            
        }
        for i=0..(num_switches/2-1) {
        router[0].ethg++ <--> switch[i].ethg++;
        }
        
        for i=num_switches/2..(num_switches-1) {
        router[num_routers-1].ethg++ <--> switch[i].ethg++;
        }
}


