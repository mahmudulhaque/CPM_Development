//--------------------------------------------------------
// File: cpmsite.cc
//     (part of OMNeT++ simulation of CPMnet)
// Author: Weihai Yu
//--------------------------------------------------------


#include "cpmsite.h"
#include "scpact.h"
#include "agent.h"
// #include "cpm.h"


MsgQueue* MsgQueue::enqueue ( cMessage* msg )
{
	msgq.push_back ( msg );
	return this;
}

cMessage* MsgQueue::dequeue ( )
{
	cMessage *msg = NULL;
	if ( !isEmpty () )
	{
		msg = msgq.front ();
		msgq.pop_front ();
	}
	return msg;
}

void MsgQueue::cleanup ()
{
	while ( !isEmpty () )
		delete dequeue();
}

// void MsgQueue::rollBack (long sid, long bid)
// {
// 	Cpm cpm(site);
// 	CPmessageHandler *msgh = new CPmessageHandler (site->agent);
// 	MsgQIt it = msgq.begin();
// 	while (it != msgq.end())
// 	{
// 		msgh->set(*it);
// 		cpm.set (msgh->getCpm());
// 		if ((cpm.getSid() == sid) && (cpm.getBid() == bid))
// 		{
// 			msgq.erase (it);
// 		}
// 		else
// 			it++;
// 	}
// 	delete msgh;
// }

string MsgQueue::dump()
{
	ostringstream stm;
	stm << "==== Queue " << site->addr() << " ====\n";
	MsgQIt it;
	for (it = msgq.begin(); it != msgq.end(); it++)
	{
		stm << "==>>";
		switch ((*it)->getKind())
		{
			case CPM:
			{
				stm << (check_and_cast<CPmessage *>(*it))->getCpm() << "\n\n";
				break;
			}
			case SCPM:
			{
				stm << (check_and_cast<SCPmessage *>(*it))->getCmd() << "\n\n";
				break;
			}

		}
	}
	stm << "==== ==== ==== ====\n";
	return stm.str();
}

MsgPool* MsgPool::insert ( unsigned int key, CPmessage* msg )
{
	msgpool.insert ( PoolEntry(key, msg) );
	return this;
}

// returns the number of msgs removed
int MsgPool::remove ( unsigned int key )
{
	int count = msgpool.count (key);
	msgpool.erase(key);
	return count;
}

// returns the msg with the key
CPmessage* MsgPool::getMsg ( unsigned int key )
{
	MsgMapIt mit = msgpool.find (key);
	if ( mit == msgpool.end() )
		return NULL;
	else
		return mit->second;
}

// returns the MsgMapIt with the key
MsgMapIt MsgPool::getItor ( unsigned int key )
{
	return msgpool.find (key);
}

std::pair<MsgMapIt, MsgMapIt> MsgPool::get_range ( unsigned int key )
{
	return msgpool.equal_range (key);
}


// returns the number of msgs matching the key
int MsgPool::match ( unsigned int key )
{
	return msgpool.count (key);
}

void MsgPool::rollBack (long sid, long bid)
{
	// ev << "MsgPool::rollBack:(" << sid << ',' << bid << ")\n"
	// 	<< dump() << endl;
	Cpm cpm(site);
	for (MsgMapIt it = msgpool.begin(); it != msgpool.end();
		/* See Meyers' "Effective STL" pp 45 */)
	{
		CPmessage *cmsg = it->second;
		cpm.set(cmsg->getCpm());
		if (cpm.getSid() == sid && cpm.getBid() == bid
			&& !cpm.isRollingback())
		{
			// ev << "MsgPool::rollBack: to rollback (" << sid << ',' << bid << ")\n"
			// 	<< cmsg->getCpm() << endl;
			// since the branch site is guarded for rollback, enqueuing it
			// will roll it back (but it has to wait in the line to be fair:-)
			cmsg->setInterpreted (true);
			site->queue->enqueue(cmsg);
			msgpool.erase (it++);
		}
		else
			++it;
	}
}
void MsgPool::cleanup ()
{
	for (MsgMapIt it = msgpool.begin(); it != msgpool.end(); it++)
	{
		delete it->second;
	}
}

string MsgPool::dump()
{
	ostringstream stm;
	stm << "==== Pool " << site->addr() << " ====\n";
	for (MsgMapIt it = msgpool.begin(); it != msgpool.end(); it++)
	{
		stm << it->first << ":\n" << it->second->getCpm() << "\n\n";
	}
	stm << "==== ==== ==== ====\n";
	return stm.str();
}


ScpDesc::ScpDesc( long newfid, long newsid, int newsaddr,
				long newpsid, int newpsaddr )
: fid(newfid), sid(newsid), psid(newpsid),
  saddr(newsaddr), psaddr(newpsaddr), status (ACTIVE)
{
}

ScpDesc::~ScpDesc ()
{
	for ( ScpBranchSitesIt sites_it = sites.begin();
			sites_it != sites.end(); ++sites_it)
	{
		delete *sites_it;
	}
}

Status ScpDesc::getBranchStatus ( long bid )
{
	for ( ScpBranchSitesIt sites_it = sites.begin();
			sites_it != sites.end(); ++sites_it)
	{
		if ( (*sites_it)->bid == bid )
			return (*sites_it)->status;
	}
	return UNKNOWN;
}

Status ScpDesc::getBranchSiteStatus ( int addr, long bid )
{
	ScpBranchSitesIt sites_it = findBranchSite (addr, bid);
	if (sites_it == sites.end())
		return UNKNOWN;
	else
		return (*sites_it)->status;
}

ScpBranchSitesIt ScpDesc::setBranchSiteStatus (
	int addr, long bid, Status stat )
{
	ScpBranchSitesIt sites_it = findBranchSite (addr, bid);
	if (sites_it != sites.end())
	{
		(*sites_it)->status = stat;
	}
	return sites_it;
}

ScpBranchSitesIt ScpDesc::setBranchSiteStatus (
	ScpBranchSitesIt sites_it, Status stat )
{
	if (sites_it != sites.end())
	{
		(*sites_it)->status = stat;
	}
	return sites_it;
}

ScpDesc* ScpDesc::newBranchSite ( int newaddr, long bid, Status stat )
{
	for ( ScpBranchSitesIt sites_it = sites.begin();
			sites_it != sites.end(); ++sites_it)
	{
		if ((*sites_it)->bid == bid)
		{
			// update the addr and status of this branch
			(*sites_it)->addr = newaddr;
			(*sites_it)->status = stat;
			return this;
		}
	}
	sites.push_back ( new ScpBranchSite (newaddr, bid, stat) );
	return this;
}

ScpDesc* ScpDesc::insertScp (int addr, long bid, long csid)
{
	ScpBranchSitesIt sites_it = findBranchSite (addr, bid);
	if (sites_it == sites.end())
	{
		ev << "Site " << addr << " with bid " << bid;
		ev << " is not registered with scope " << sid;
		ev << " at " << saddr << endl;
		return this;
	}
	(*sites_it)->csids.insert (csid);
	return this;
}

ScpBranchSitesIt ScpDesc::findBranchSite ( int addr, long bid )
{
	for ( ScpBranchSitesIt sites_it = sites.begin();
			sites_it != sites.end(); ++sites_it)
	{
		if (((*sites_it)->addr == addr)
			&& ((*sites_it)->bid == bid))
			return sites_it;
	}
	return sites.end();
}

string ScpDesc::toStr ()
{
	ostringstream stm;
	stm << "Scope " << sid << " of " << fid << ", " << statusStr (status);
	if (!isTopScp())
		stm << ", inside scope " << psid << " (at " << psaddr << ")";
	stm << ", includes sites:\n";
	for ( ScpBranchSitesIt it = sites.begin(); it != sites.end(); ++it)
	{
		if ((*it)->bid != -1)
			stm << "\tbranch " << (*it)->bid << ", ";
		stm << "site " << (*it)->addr << ", ";
		for ( SidSetIt child_scp_it = (*it)->csids.begin();
			child_scp_it !=(*it)->csids.end(); ++child_scp_it)
			stm << "scope " << *child_scp_it << ", ";
		stm << statusStr ((*it)->status) << "\n";
	}
	return stm.str();
}

ScpReg::ScpReg( CPMsite *newsite)
: site(newsite)
{
}

void ScpReg::handleMessage ( const char *str )
{
	// TODO: state of the scope ....
	ev << "ScpReg::handleMessage - Site " << site->addr() << "\n";
	ev << str << "\n";
	ScpAct sact (str);
	ScpMapIt mit = scpmap.find (sact.sid);

	switch (sact.act)
	{
		case NEWSITE:
		{
			if ( mit == scpmap.end() )
			{
				// the receiver of the msg is supposed to be
				// the manager of the scope
				ev << "ScpReg::handleMessage: Scope " << sact.sid;
				ev << " of " << sact.fid;
				ev << " is not managed by site " << site->addr() << endl;
				return;
			}
			ScpDesc *scpdesc = mit->second;
			// ev << scpdesc->toStr() << endl;

			Status branch_site_status = scpdesc
					->getBranchSiteStatus(sact.newsite, sact.bid);
			switch (scpdesc->getBranchStatus(sact.bid))
			{
				case STOPPING:
				{
					if (branch_site_status == STOPPING)
						break;
					if (branch_site_status == UNKNOWN)
						scpdesc->newBranchSite(sact.newsite,
												  sact.bid,
												  STOPPING);
					else
						scpdesc->setBranchSiteStatus(sact.newsite,
													 sact.bid,
													 STOPPING);
					ScpAct stop_act (site, sact.fid, sact.sid,
							sact.saddr, sact.bid, site->addr(),
							sact.rollingback);
					stop_act.setAct (STOPSCP);
					downTo ( sact.src, stop_act.toMsg() );
					break;
				}
				case ROLLINGBACK:
				{
					if (branch_site_status == ROLLINGBACK)
						break;
					if (branch_site_status == UNKNOWN)
						scpdesc->newBranchSite(sact.newsite,
												  sact.bid,
												  ROLLINGBACK);
					else
						scpdesc->setBranchSiteStatus(sact.newsite,
													 sact.bid,
													 ROLLINGBACK);
					if (!sact.rollingback)
					{
						// if this is from the site that is still
						// running forward, tell it to rollback
						ScpAct rollbk_act (site, sact.fid, sact.sid,
								sact.saddr, sact.bid, site->addr(), true);
						rollbk_act.setAct (ROLLBACKSCP);
						downTo ( sact.newsite, rollbk_act.toMsg() );					}

					break;
				}
				default:
				{
					switch (branch_site_status)
					{
						case UNKNOWN:
						{
							scpdesc->newBranchSite(sact.newsite, sact.bid);
							break;
						}
						case ACTIVE:
						{
							// already registered
							break;
						}
						default:
						{
							ev << "ScpReg::handleMessage:NEWSITE: error!\n";
						}
					}
				}
			}

			ev << "ScpReg::handleMessage: after NEWSITE\n" << dump();
			break;
		}
		case NEWSCP:
		{
			if ( mit == scpmap.end() )
			{
				// the receiver of the msg is supposed to be
				// the manager of the scope
				ev << "ScpReg::handleMessage: Scope " << sact.sid;
				ev << " of " << sact.fid;
				ev << " is not managed by site " << site->addr() << endl;
				return;
			}
			ScpDesc *scpdesc = mit->second;
			switch (scpdesc->getBranchSiteStatus(sact.newsite, sact.bid))
			{
				case UNKNOWN:
				{
					// ev << "Site " << sact.src << " with bid " << sact.bid;
					// ev << " is not registered with scope " << sact.sid;
					// ev << " at " << site->addr() << ", register now.\n";
					scpdesc->newBranchSite(sact.src, sact.bid);
					scpdesc->insertScp(sact.src, sact.bid, sact.csid);
					break;
				}
				case ACTIVE:
				{
					scpdesc->insertScp(sact.src, sact.bid, sact.csid);
					break;
				}
				case STOPPING:
				{
					// stop the child scope
					ScpAct stop_c_act (site, sact.fid, sact.csid,
							sact.src, sact.bid, site->addr(),
							sact.rollingback);
					stop_c_act.setAct (STOPSCP);
					downTo ( sact.src, stop_c_act.toMsg() );
					// stop the branch site
					ScpAct stop_act (site, sact.fid, sact.sid,
							sact.saddr, sact.bid, site->addr(),
							sact.rollingback);
					stop_act.setAct (STOPSCP);
					downTo ( sact.src, stop_act.toMsg() );
					break;
				}
				case ROLLINGBACK:
				{
					// rollback the child scope
					ScpAct rollbk_c_act (site, sact.fid, sact.csid,
							sact.src, sact.bid, site->addr(),
							sact.rollingback);
					rollbk_c_act.setAct (ROLLBACKSCP);
					downTo ( sact.src, rollbk_c_act.toMsg() );
					// rolback the branch site
					ScpAct rollbk_act (site, sact.fid, sact.sid,
							sact.saddr, sact.bid, site->addr(),
							sact.rollingback);
					rollbk_act.setAct (ROLLBACKSCP);
					downTo ( sact.src, rollbk_act.toMsg() );
					break;
				}
				default:
				{
					ev << "ScpReg::handleMessage:NEWSITE: error!\n";
				}
			}

			break;
		}
		case STOPSCP:
		{
			if ( mit != scpmap.end() )
			{
				ScpDesc *scpdesc = mit->second;
				// ev << "ScpReg::handleMessage: before STOPSCP\n" << dump();
				delete scpdesc;
				scpmap.erase (mit);
			}
			site->guard->removeScpGuards (sact.sid);
			stopChildScp (sact.psid, sact.psaddr, sact.sid, str);
			// ev << "ScpReg::handleMessage: after STOPSCP\n" << dump();

			break;
		}
		case ROLLBACKSCP:
		{
			// ev << "before handling rollbackscp\n" << dump ();
			// downcast to all sites of the scope
			if ( mit != scpmap.end() )
			{
				ScpDesc *scpdesc = mit->second;
				// set the new status of the originating site
				// so that the message is not downcasted back to it.
				scpdesc->setBranchSiteStatus (sact.src, sact.bid, ROLLINGBACK);
				// first downcast to child scopes
				downcastScpMgrs ( scpdesc, ROLLBACKSCP);
				// then downcast to the branch sites
				downcastScpBranchSites (scpdesc, str, ROLLINGBACK);
				if (scpdesc->getScpStatus () == ACTIVE)
					scpdesc->setScpStatus (ROLLINGBACK);
				site->guard->setGuard (
					new GuardedBranch(sact.sid, sact.bid, ROLLBACK));
				site->pool->rollBack (sact.sid, sact.bid);
			}
			else
			{
				// set the guard even when the branch site is not known
				// yet. It is possible that the rollback info comes
				// before the corresponding cpm message in handled.
				// (due, for example, to higher priority of SCPmessage
				// queue, which is a good thing, because the rollback
				// can start as early as possible)
				site->guard->setGuard (
					new GuardedBranch(sact.sid, sact.bid, ROLLBACK));
			}
			// ev << "after handling rollbackscp\n" << dump ();

			break;
		}
		default:
		{
			ev << "ScpReg::handleMessage: no action type ";
			ev << sact.actStr() << endl;
		}
	}
}

// Downcasts to child scopes
void ScpReg::downcastScpMgrs (ScpDesc* scp_desc, ScpActType act_type)
{
	for ( ScpBranchSitesIt sites_it = scp_desc->sites.begin();
			sites_it != scp_desc->sites.end(); ++sites_it)
	{
		for ( SidSetIt child_scp_it = (*sites_it)->csids.begin();
			child_scp_it !=(*sites_it)->csids.end(); ++child_scp_it)
		{
			int remote_id = (*sites_it)->addr;
			ScpAct scpact (site, scp_desc->fid,
						*child_scp_it, remote_id, (*sites_it)->bid, site->addr());
			scpact.setAct (act_type);
			scpact.setParent (scp_desc->sid, scp_desc->saddr);

			SCPmessage *smsg = scpact.toMsg();
			downTo (remote_id, smsg);
		}
	}
}

void ScpReg::downcastScpBranchSites (ScpDesc* scp_desc, const char *str,
										Status new_stat )
{
	for ( ScpBranchSitesIt sites_it = scp_desc->sites.begin();
			sites_it != scp_desc->sites.end(); ++sites_it)
	{
		int remote_id = (*sites_it)->addr;
		// send to sites whose status have not been updated
		if ((*sites_it)->status != new_stat)
		{
			(*sites_it)->status = new_stat;
			ScpAct scpact (str);
			scpact.bid = (*sites_it)->bid;
			SCPmessage *smsg = new SCPmessage (_SCPM, SCPM);
			char *scpactstr = scpact.toStr();
			smsg->setCmd(scpactstr);
			delete scpactstr;
			downTo (remote_id, smsg);
		}
	}
}

void ScpReg::downTo ( int addr, SCPmessage *smsg )
{
	// if the scope manager is local, handle immediately
	if ( addr == site->addr())
	{
		ev << "locally handle " << smsg->getCmd() << endl;
		handleMessage (smsg->getCmd());
		delete smsg;
		return;
	}
	smsg->setSrc (site->addr());
	smsg->setDest (addr);
	smsg->setByteLength (site->agent->smsg_byte_length);

	ev << "sending " << smsg->getCmd() << "\nto " << addr << endl;

	site->agent->sendTo (addr, smsg);
}


ScpReg* ScpReg::insertScp ( ScpDesc* scp )
{
	scpmap[scp->sid] = scp;
	return this;
}

// returns the number of addrs removed
int ScpReg::removeScp ( long sid )
{
	int count = scpmap.count (sid);
	scpmap.erase(sid);
	return count;
}

// returns the addr with the key
ScpDesc* ScpReg::getScp ( long sid )
{
	return scpmap.count (sid) > 0 ? scpmap[sid] : NULL;
}

// tell the parent scope to stop this child
void ScpReg::stopChildScp ( long sid, int saddr, long csid, const char *str )
{
	if (sid == -1) // this is a top scope
		return;
	if (saddr == site->addr())
	{
		// the child scope is managed locally
		ScpDesc *scp_desc = getScp (sid);
		if (scp_desc == NULL) return;
		for ( ScpBranchSitesIt sites_it = scp_desc->sites.begin();
				sites_it != scp_desc->sites.end();
				/* see Meyers' "Effective STL" pp 45 */)
		{
			if ((*sites_it)->csids.count(csid) > 0)
			{
				// erase the site alltogether,
				// because it is cleaned up with STOPSCP
				delete *sites_it;
				scp_desc->sites.erase (sites_it++);
				return;
			}
			else
				++sites_it;
		}
		return;
	}

	// the child scope is managed at another site
	SCPmessage *smsg = new SCPmessage (_SCPM, SCPM);
	smsg->setCmd(str);
	smsg->setSrc (site->addr());
	smsg->setDest (saddr);
	smsg->setByteLength (site->agent->smsg_byte_length);
	site->agent->sendTo (saddr, smsg);
}

void ScpReg::cleanup ()
{
	for (ScpMapIt it = scpmap.begin(); it != scpmap.end(); it++)
	{
		delete it->second;
	}
}

string ScpReg::dump() {
	ostringstream stm;
	stm << "==== Scope Reg " << site->addr() << " ====\n";
	ScpMapIt it;
	for (it = scpmap.begin(); it != scpmap.end(); it++)
	{
		stm << it->second->toStr() << '\n';
	}
	stm << "==== ==== ==== ====\n";
	return stm.str();
}

GuardedBranch::GuardedBranch (long init_sid, long init_bid,
							  ScpActType init_act)
: sid(init_sid), bid(init_bid), guard_act(init_act)
{
	num_dispatched = 0;
}

GuardedBranch* GuardedBranch::setAct (ScpActType act)
{
	guard_act = act;
	return this;
}

DispatchGuard::DispatchGuard (CPMsite *newsite)
:site (newsite), total_dispatched (0)
{
	max_dispatchable = site->agent->num_cpus;
}

ScpActType DispatchGuard::guard (long sid, long bid)
{
	ev << "site " << site->addr() << " guard: (";
	ev << sid << ',' << bid << ")\n";
	//ev << dump();
	total_dispatched++;
	GuardedBranch *br = new GuardedBranch(sid, bid, NONE);
	BranchesIt it = branches.find( br );
	if ( it != branches.end() )
	{
		delete br;
		(*it)->num_dispatched++;
		return (*it)->guard_act;
	}
	branches.insert (br);
	br->num_dispatched++;
	return NONE;
}

DispatchGuard* DispatchGuard::setGuard (GuardedBranch *branch)
{
	BranchesIt it = branches.find( branch );
	if (it == branches.end())
	{
		// ev << "site " << site->addr() << " setGuard: ";
		// ev << "The branch to guard (" << branch->sid << ", ";
		// ev << branch->bid << ") is not guarded.";
		total_dispatched++;
		branch->num_dispatched++;
		branches.insert (branch);
	}
	else
	{
		(*it)->guard_act = branch->guard_act;
		delete branch;
	}
	return this;
}

DispatchGuard* DispatchGuard::leaveGuard (long sid, long bid)
{
	ev << "site " << site->addr() << " leaveGuard: (";
	ev << sid << ',' << bid << ")\n";
	GuardedBranch *br = new GuardedBranch(sid, bid);
	BranchesIt it = branches.find( br );
	delete br;
	if ( it == branches.end() )
	{
		ev << "site " << site->addr() << " leaveGuard: branch (";
		ev <<  sid << ", " << bid << ") is not guarded!\n";
		return this;
	}
	// ev << "site " << site->addr() << " leaveGuard: ";
	// ev << "Currently " << total_dispatched << " dsipatched.\n";
	(*it)->num_dispatched--;
	if ((*it)->num_dispatched <= 0 && (*it)->guard_act == NONE)
	{
		delete (*it);
		branches.erase (it);
	}
	total_dispatched--;
	return this;
}

DispatchGuard* DispatchGuard::removeScpGuards (long sid)
{
	BranchesIt it = branches.begin();
	while (it != branches.end())
	{
		if ((*it)->sid == sid)
		{
			BranchesIt found_it = it;
			total_dispatched -= (*found_it)->num_dispatched;
			it++;
			delete (*found_it);
			branches.erase (found_it);
		}
		else
			it++;
	}
	return this;
}

string DispatchGuard::dump()
{
	ostringstream stm;
	stm << "==== Guard " << site->addr() << " ====\n";
	for (BranchesIt it = branches.begin(); it != branches.end(); it++)
	{
		stm << "==>> Sid: " << (*it)->sid << ", ";
		if ((*it)->bid != -1)
			stm << "Bid: " << (*it)->bid << ", ";
		switch ((*it)->guard_act)
		{
			case ROLLBACK:
			{
				stm << "ScpAct: ROLLBACK\n";
				break;
			}
			case STOPSCP:
			{
				stm << "ScpAct: STOPSCP\n";;
				break;
			}
			default:
			{
				stm << '\n';
			}

		}
	}
	stm << "==== ==== ==== ====\n";
	return stm.str();
}

CPMsite::CPMsite( int newaddr, Agent *newagent)
: agent(newagent), address(newaddr)
{
	queue = new MsgQueue(this);
	scp_queue = new MsgQueue(this);
	pool = new MsgPool(this);
	reg = new ScpReg(this);
	guard = new DispatchGuard(this);
}

CPMsite::~CPMsite()
{

	delete queue;
	delete scp_queue;
	delete pool;
	delete reg;
	delete guard;
}

void CPMsite::cleanup ()
{
	queue->cleanup();
	scp_queue->cleanup();
	pool->cleanup();
	reg->cleanup();
}
