//--------------------------------------------------------
// File: cpmsite.h
//     (part of OMNeT++ simulation of CPMnet)
// Author: Weihai Yu
//--------------------------------------------------------

#ifndef __CPMSITE_H
#define __CPMSITE_H

#include "cpmcommon.h"
#include <list>
#include <vector>
#include <map>
#include <string>
#include <sstream>
using std::ostringstream;
using std::string;

class Agent;
class CPMsite;
class MsgQueue {
public:
	MsgQueue ( CPMsite *newsite ):site(newsite) {};
	MsgQueue* enqueue ( cMessage *msg );
	cMessage* dequeue ( );
	bool isEmpty () { return msgq.empty (); };
	void cleanup ();
	// void rollBack (long sid, long bid);
	string dump ();
private:
	typedef std::list<cMessage *> MsgQ;
	typedef MsgQ::iterator MsgQIt;
	MsgQ msgq;
	CPMsite *site;
};

// muli-map for the pending message pool
typedef std::multimap< unsigned int, CPmessage* > MsgMap;
typedef MsgMap::iterator MsgMapIt;
class MsgPool {
public:
	MsgPool ( CPMsite *newsite ):site(newsite) {};
	MsgPool* insert ( unsigned int key, CPmessage* msg );
	int remove ( unsigned int key );
	CPmessage* getMsg ( unsigned int key );
	MsgMapIt getItor ( unsigned int key );
	MsgMapIt endItor () { return msgpool.end(); };
	std::pair<MsgMapIt, MsgMapIt> get_range ( unsigned int key );
	int match ( unsigned int key );
	void rollBack (long sid, long bid);
	void cleanup ();
	string dump ();
private:
	typedef MsgMap::value_type PoolEntry;
	MsgMap msgpool;
	CPMsite *site;
};


typedef std::set<long> SidSet;
typedef SidSet::iterator SidSetIt;

class ScpBranchSite {
public:
	ScpBranchSite ( int newaddr, long newbid = -1,
		Status stat = ACTIVE )
		: addr (newaddr), bid (newbid), status (stat) {};
	int addr;
	long bid; // -1 if not a branch
	SidSet csids; // child sids
	Status status;
};

typedef std::vector<ScpBranchSite*> ScpBranchSites;
typedef ScpBranchSites::iterator ScpBranchSitesIt;


// scope descriptor
// includes the parent and child scopes and
// the addresses of the agents
// where the queue, pool or scope registry keeps some info about
// the scope (active branches, event handlers, nested scopes etc.)

class ScpDesc
{
public:

	ScpDesc ( long newfid, long newsid, int newsaddr,
				long newpsid, int newpsaddr );
	~ScpDesc ();

	bool isTopScp () { return psid == -1; };
	Status getScpStatus () { return status; };
	ScpDesc* setScpStatus ( Status stat) { status = stat; return this; };
	Status getBranchStatus ( long bid );
	Status getBranchSiteStatus ( int addr, long bid );
	ScpBranchSitesIt setBranchSiteStatus ( int addr, long bid, Status stat );
	ScpBranchSitesIt setBranchSiteStatus ( ScpBranchSitesIt sites_it, Status stat );
	ScpDesc* newBranchSite ( int newaddr, long bid = -1,
								Status stst = ACTIVE );
	ScpDesc* insertScp ( int addr, long bid, long csid );
	string toStr ();
	ScpBranchSites sites;
	
	long fid, sid, psid;
	int saddr, psaddr;
	Status status;
	ScpBranchSitesIt findBranchSite ( int addr, long bid );
};


// scope registry
// map of scope descriptors, indexed by the scope ids
class ScpReg {
public:
	ScpReg ( CPMsite* newsite );
	void handleMessage ( const char *str );
	ScpReg* insertScp ( ScpDesc* scp );
	bool isLocal (ScpBranchSite *scpsite);
	bool isLocal (ScpDesc *scpdesc);
	void cleanup ();
	string dump ();
private:
	void downcastScpMgrs (ScpDesc* scp_desc, ScpActType act_type);
	void downcastScpBranchSites (ScpDesc* scp_desc, const char *str,
		Status new_stat );
	void downTo ( int addr, SCPmessage *smsg );
	int removeScp ( long sid );
	ScpDesc* getScp ( long sid );
	void stopChildScp ( long sid, int saddr, long csid, const char *str );
	typedef std::map< long, ScpDesc* > ScpMap;
	typedef ScpMap::iterator ScpMapIt;
	ScpMap scpmap;
	CPMsite *site;
};


class GuardedBranch {
public:
	GuardedBranch (long init_sid = -1, long init_bid = -1,
		ScpActType init_act = NONE);
	GuardedBranch* setAct (ScpActType act);
	long sid, bid;
	ScpActType guard_act;
	int num_dispatched;
	// two branches may have the same sis and bid
	// such as after eoi, both may be dispatched
};

struct branch_less
{
	bool operator()(const GuardedBranch* lhb, const GuardedBranch* rhb) const
	{
		return (lhb->sid < rhb->sid) ||
				((lhb->sid == rhb->sid) && (lhb->bid < rhb->bid));
	}
};

class DispatchGuard {
public:
	DispatchGuard( CPMsite *newsite );
	
	ScpActType guard (long sid, long bid);
	DispatchGuard* setGuard (GuardedBranch *branch);
	DispatchGuard* leaveGuard (long sid, long bid);
	DispatchGuard* removeScpGuards (long sid);
	string dump ();
private:
	// multiset - because there might be multiple branches of the same 
	// (sid,bid) for example generated by EOI in the no-return case
	typedef std::set< GuardedBranch*, branch_less > Branches;
	typedef Branches::iterator BranchesIt;
	Branches branches;
	CPMsite *site;
	int max_dispatchable, total_dispatched;
};

class CPMsite {
	public:
		CPMsite ( int newaddr, Agent *agent );
		~CPMsite ();
		int addr () { return address; };
		void cleanup ();
		MsgQueue *queue, *scp_queue;
		MsgPool *pool;
		ScpReg *reg;
		DispatchGuard *guard;
		Agent *agent;
	private:
		int address;
};

#endif