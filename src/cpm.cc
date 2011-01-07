//-------------------------------------------------------------
// File: cpm.cc
//     (part of OMNeT++ simulation of CPMnet)
// Author: Weihai Yu
//-------------------------------------------------------------


#include "cpm.h"
#include "agent.h"
#include "CPmessageHandler.h"
#include "scpact.h"
#include <list>


long Cpm::nextid = 0;

Cpm::Cpm ( CPMsite* s, xmlDocPtr cpmdoc )
			:site(s), doc(cpmdoc), cpmelm(NULL), actelm(NULL),
			acttype(NON),
			msgh(NULL), rollingback (false)
{
	centralized = site->agent->centralized;
	setReturn();
	c = new Cont ();
	bwc = new Cont ();
	e = new Environ (this);
	if (cpmdoc)
		set (cpmdoc);
}

Cpm::~Cpm ()
{
	delete e;
	delete c;
	delete bwc;
	if (actelm) xmlFreeNode(actelm);
	if (doc)
		xmlFreeDoc (doc);
}

// construct a Cpm instance from the doc generated from the XML message
Cpm* Cpm::set (xmlDocPtr newdoc )
{
	release();
	doc = newdoc;
	if (!doc)
	{
		ev << "Cpm::Cpm: tries to initialize with NULL doc!\n";
		return this;
	}
		// initialize with an xml doc
	cpmelm = xmlDocGetRootElement(doc);
	if (!cpmelm)
		invalidCpm ();
	if ( !xmlStrEqual (cpmelm->name, BAD_CAST _CPM) )
		invalidCpm ();

	fid = xmlGetIntProp (cpmelm, _FID);
	sid = xmlGetIntProp (cpmelm, _SID);
	saddr = xmlGetIntProp (cpmelm, _SADDR);
	bid = xmlGetIntProp (cpmelm, _BID);
	
	xmlChar *headingxstr = xmlGetProp (cpmelm, BAD_CAST _HEADING);
	if (xmlStrEqual (headingxstr, BAD_CAST _BACKWARD) )
		rollingback = true;
	else
		rollingback = false;
	xmlFree (headingxstr);
	
	xmlNodePtr child = xmlElementChildByName (cpmelm, _C);
	if (!child)
		invalidCpm ();
	c->set (doc, child);
	bwc->set (doc, c->currentBwc ());
	
	child = xmlElementChildByName (cpmelm, _E);
	if (!child)
		invalidCpm ();
	e->set(doc, child);

	return this;
}

Cpm* Cpm::set ( const char *str)
{
	// ev << "Cmp::set(char*):\n" << str << endl;
	return set (strToDoc (str));
}

Cpm* Cpm::setFid (long newfid)
{
	fid = newfid;
	xmlSetIntProp (cpmelm, _FID, fid);
	return this;
}

Cpm* Cpm::setNewSid ()
{
	return setSid(nextid++);
}

Cpm* Cpm::setSid (long newsid)
{
	sid = newsid;
	xmlSetIntProp (cpmelm, _SID, sid);
	return this;
}

Cpm* Cpm::setBid (long newbid)
{
	bid = newbid;
	xmlSetIntProp (cpmelm, _BID, bid);
	return this;
}

Cpm* Cpm::setSaddrWithScp ()
{
	if (acttype != SCP && acttype != FLW)
		return this;

	if (xmlHasProp(actelm, BAD_CAST _ADDR))
		return setSaddr (xmlGetIntProp(actelm, _ADDR));
	else
	 	return setSaddr (site->addr ());
}

Cpm* Cpm::setSaddr (int newsaddr)
{
	saddr = newsaddr;
	xmlSetIntProp (cpmelm, _SADDR, newsaddr);
	return this;
}

Cpm* Cpm::setRollingback (bool to_rollback)
{
	rollingback = to_rollback;
	xmlSetProp (cpmelm, BAD_CAST _HEADING,
		to_rollback? BAD_CAST _BACKWARD : BAD_CAST _FORWARD);
	return this;
}

Cpm* Cpm::setReturn (bool back_to_caller)
{
	return_to_caller = back_to_caller;
	return this;
}

Cpm* Cpm::setToNull ()
{
	return set (_NULLCPM);
}

Cpm* Cpm::release ()
{
	if (doc)
	{
		e->reset();
		c->reset();
		bwc->reset();
		actelm = xmlPurgeNode(actelm);
		xmlFreeDoc (doc);
		doc = NULL;
		cpmelm = NULL;
	}
	fid = -1;
	sid = -1;
	saddr = -1;
	bid = -1;
	svc_failrate = 0.0;
	return this;
}

xmlChar* Cpm::toXstr ()
{
	xmlChar *xmlbuff;
    int buffersize;
    xmlDocDumpMemory(doc, &xmlbuff, &buffersize);
	return xmlbuff;
}
void Cpm::invalidCpm ()
{
	ev << "Cpm::set: invalid Cpm!\n";
	evDumpDoc (doc);
	ev << "\n----\n";
	site->agent->callFinish ();
}

void Cpm::dispatch ( CPmessageHandler *cmsgh )
{
	if (cmsgh != NULL)
		msgh = cmsgh;
	ev << "Cpm::dispatch, pre " << site->guard->dump();
	switch (site->guard->guard(sid, bid))
	{
		case NONE:
		{
			applyC()->enact();
			break;
		}
		case ROLLBACK:
		{
			if (rollingback)
			{
				// ev << "continue with rollback\n";
				// evDumpDoc(doc);
				// ev << "\n--------\n";
				applyC()->enact();
			}
			else
			{
				// ev << "dispatch--rollback ...\n";
				// evDumpDoc(doc);
				// ev << "\n--------\n";
				rollBack();
			}
			break;
		}
		case STOPSCP:
		{
			break;
		}
		default:
		{
	
		}
	}
	ev << "Cpm::dispatch, post " << site->guard->dump();
}

void Cpm::leave()
{
	site->guard->leaveGuard(sid, bid);
}


// enact the current Cpm machine
// If the current state depends on some other message, put it into the msg pool
// When the next state is ready to carry on, enact it immediately.
// If more than one branch should go on, enact one and
// put the rest into the queue waiting to be enacted.
// A remote activity is always forwarded to the remote agent.
// msgh is used to pass the message context betwwen enact() and the calling agent.
// enact() will _never_ directly  use the CPM string in the message.
void Cpm::enact ()
{
	if (!actelm)
	{
		// ev << "----------\nenact() NULL actelm!\n";
		// evDumpDoc(doc);
		// ev << "----------";
		return;
	}
	if (centralized && acttype == SCP)

		xmlSetIntProp(actelm, _ADDR, 0);

	if (!isLocal())
	{
		if (acttype == INV || acttype == OWY)
		{
			xmlSetIntProp (actelm, _SRC, site->addr());
		}
		c->push(actelm);
		evDumpNode(actelm);
		ev<< "in islocal"<<endl;
		ev << "====\n";
		forward (xmlGetIntProp(actelm, _ADDR));
		if (acttype != OWY)
			return;
		// the invoking branch of OWY continues
	}

	if (rollingback)
		ev << "... rolling back ";
	ev << "====  enact: " << (char *)actelm->name << " of " << fid << endl;

	evDumpDoc(doc);
	evDumpNode(actelm);
	ev << "====\n";

	switch (acttype)
	{
		case NON:
		{
			terminate();
			break;
		}
		case SCP:
		{

			if (rollingback)
			{
				ev << "enact: should not encounter SCP during rollback!\n";
				terminate();
				break;
			}
			long psid = sid;
			long psaddr = saddr;
			createScope (nextid++);


			// a scope consists of a number of event handlers,
			//  a compensation handler and a primary activity.
			// Enact event handlers and the primary activity.
			// Encaspulate compensation handler in eos.

			xmlNodePtr primary_elm = NULL;
			xmlNodePtr compensate_elm = NULL;
			xmlNodePtr currchild = xmlFirstElementChild (actelm);
			xmlNodePtr nextchild;
			while (currchild != NULL)
			{
				nextchild = xmlNextElementSibling (currchild);
				xmlUnlinkNode(currchild);

				if (xmlStrEqual(currchild->name, BAD_CAST _EHL))
				{
					Cpm eventcpm (site,
								  xmlNodeToDoc (xmlFirstElementChild(currchild)));
					xmlFreeNode(currchild);
					CPmessage *eventmsg = msgh->dupMsg();
					xmlChar *cpmxstr = eventcpm.toXstr();
					eventmsg->setCpm((char *)cpmxstr);
					xmlFree(cpmxstr);
					eventmsg->setMainflow(false);
					site->queue->enqueue (eventmsg);
				}
				else
				if (xmlStrEqual(currchild->name, BAD_CAST _CHL))
				{
					compensate_elm = currchild;
				}
				else
				if (xmlStrEqual(currchild->name, BAD_CAST _VARS))
				{
					// do nothing so far
					xmlFreeNode(currchild);
				}
				else
				{
					// now the primary activity
					primary_elm = currchild;
				}
				currchild = nextchild;
			}

			c->push ( makeEos (psid, psaddr, compensate_elm) );

			xmlPurgeNode (actelm);
			actelm = primary_elm;
			acttype = actType(actelm->name);
			// ev << "====  SCP - before further enact:\n";
			// evDumpDoc(doc);
			// ev << "\n======\n";
			moveOn ();
			break;
		}
		case INV:
		{

			if ( site->pool->match( poolMatchKey () ) > 0 )
			{
				ev << "RR invoke successful!\n";
				xmlNodePtr inv_elm = xmlCopyNode (actelm, 2); //shallow copy
				xmlNodePtr invars_elm = xmlElementChildByName(actelm, _INS);
				if (inv_elm && invars_elm)
					xmlAddChild (inv_elm, xmlCopyNode(invars_elm, 1)); // deep
				e->append (inv_elm, _ACTIVE);

				Cpm rcvcpm(site);
				// ev << site->pool->dump() << endl;
				// ev << "MatchKey: " << poolMatchKey() << endl;
				rcvcpm.set (site->pool->getMsg(poolMatchKey())->getCpm());
				long iid = nextid++;
				
				int svc_fail_percent = xmlGetIntProp (actelm, _FAILRATE);
				svc_failrate =
						(svc_fail_percent < 0)? 0.0 : svc_fail_percent / 100.0;
				
				xmlChar *opxstr = xmlGetProp(actelm, BAD_CAST _OP);
				c->invC(opxstr, rcvcpm.c, makeEoi(iid));
				xmlFree (opxstr);
				// ev << "====  INV - before further enact:\n";
				// evDumpDoc(doc);
				// ev << "\n======\n";
				applyC ()->enact ();
			} else {

				ev << "Invoked service not available!\n";
				// TODO: rollback
				applyC()->enact();
			}
			break;
		}
		case RCV:
		{
			site->pool->insert ( poolMatchKey (), refreshMsgH()->Msg() );
			leave();
			//ev << "pool:\n";
			//ev << site->pool->dump() << endl;
			break;
		}
		case RPL:
		{
			ev << "reply should have been replaced by eoi!\n";
			applyC ()->enact ();
			break;
		}
		case OWY:
		{
			/*
			// receiver of OWY
			if ( site->pool->match( poolMatchKey () ) > 0 )
			{
				ev << "OWY invoke successful!\n";
				set(site->pool->getMsg( poolMatchKey () )->getCpm());
				applyC();
				msgh->setMainflow(false);

				moveOn ();
			} else {
				ev << "Invoked service not available!\n";
			}
			*/
			break;
		}
		case SEQ:
		{
			xmlNodePtr curr = xmlLastElementChild (actelm);
			while (curr)
			{
				xmlNodePtr prev = xmlPrevElementSibling (curr);
				ev << "child:\n";
				evDumpNode(curr);
				xmlUnlinkNode (curr);
				c->push (curr);
				// if (rollingback)
				// 	bwc->push ((DOMElement *)child->cloneNode(true));
				curr = prev;
			}
			applyC ();
			moveOn ();
			break;
		}
		case FLW:
		{
			int num_branches = xmlChildElementCount (actelm);
			ev << "CPM::FLW num of branches in the flow " <<num_branches << endl;
			if (num_branches == 0)
			{
				applyC ();
				moveOn ();
				break;
			}
			if (num_branches == 1)
			{
				actelm = xmlFirstElementChild (actelm);
				moveOn ();
				break;
			}

			long jid = nextid++;
			long psid = sid;
			int psaddr = saddr;
			// ev << site->queue->dump();
			// A flow implicitly creates a scope
			// This helps guarding the branch sites.
			// But this idea does not go very well with the main
			// purpose of scope: fault handling.
			// An issues to be figured out later.
			createScope (jid);

			// create the branches
			Cpm branchcpm (site);
			xmlNodePtr nextchild = xmlFirstElementChild (actelm);
			while (nextchild != NULL)
			{
				xmlNodePtr currchild = nextchild;
				nextchild = xmlNextElementSibling (nextchild);
				xmlUnlinkNode (currchild);
				long newbid = nextid++;
				xmlDocPtr branchdoc = xmlNodeToDoc(cpmelm);
				// ev << "flow init branchcpm:\nNode\n";
				// evDumpNode (cpmelm);
				// ev << "\nDoc\n";
				// evDumpDoc (branchdoc);
				branchcpm.set (branchdoc);
				xmlNodePtr cjoin_elm = branchcpm.makeJoin (jid, psid, psaddr,
					num_branches, newbid, _FORWARD);
				branchcpm.c->push ( cjoin_elm );
				if (rollingback)
					branchcpm.bwc->push ( xmlCopyNode (cjoin_elm, 1) );
				else
					branchcpm.bwc->
						push ( branchcpm.makeJoin (jid, psid, psaddr,
												num_branches, newbid, _BACKWARD) );
				branchcpm.c->push (xmlDocCopyNode (currchild, branchdoc, 1));
				branchcpm.setBid (newbid);

				// register the branch at the newly created scope
				branchcpm.scpNewSite (site->addr());

				xmlChar *cpmxstr = branchcpm.toXstr();
				CPmessageHandler *branchmsgh = msgh->dup()
									               ->setCpm((char *)cpmxstr)
									               ->setInterpreted (true);
				// ev << "Branch:\n" << (char *)cpmxstr << endl;
				xmlFree(cpmxstr);
				xmlFreeNode(currchild);
				// Instead of dispatching immediately, the branch waits in the queue
				// site->queue->enqueue ( branchmsg->Msg() );
				branchcpm.dispatch(branchmsgh);
				delete branchmsgh;
			}
			msgh->releaseMessage();
			break;
		}
		case PCK:
		{
			// skip over
			applyC ()->enact ();
			break;
		}
		case EOS:
		{
			e->setStatus (_SCP, DONE);
			
			xmlNodePtr new_bwc = c->currentBwc ();
			if (new_bwc) // this is not a top-level scope
			{
				xmlNodePtr chl_elm = xmlElementChildByName (actelm, _CHL);
				if (chl_elm)
				{
					xmlNodePtr cmp_elm = xmlFirstElementChild(chl_elm);
					xmlUnlinkNode (cmp_elm);
					bwc->set (c->currentBwc ())->push (cmp_elm);
				}
				else
				{
					xmlNodePtr scp_bwc_elm = xmlElementChildByName (actelm, _BWC);
					bwc->set (c->currentBwc ());
					bwc->concatenate (scp_bwc_elm);
				}
			}

			stopScope ( xmlGetIntProp (actelm, _SID),
						xmlGetIntProp (actelm, _ADDR),
						xmlGetIntProp (actelm, _PSID),
						xmlGetIntProp (actelm, _PSADDR) );
			applyC ();
			// ev << "====  EOS - before further enact:\n";
			// evDumpDoc(doc);
			// ev << "\n======\n";

			moveOn ();
			break;
		}
		case ESF:
		{
			e->setStatus (_SCP, UNDONE);
			stopScope ( xmlGetIntProp (actelm, _SID),
						xmlGetIntProp (actelm, _ADDR),
						xmlGetIntProp (actelm, _PSID),
						xmlGetIntProp (actelm, _PSADDR) );
			// Rollingback of the current scope has finished
			setRollingback (false); 
			applyC ()->moveOn ();
			break;
		}
		case EOI:
		{
			bool just_failed = msgh->getFailed();
			// resume the invoking branch
			Cpm *invcpm = invCpm(just_failed);
			if (!invcpm)
				break;
			
			xmlChar *cpmxstr = invcpm->toXstr();	
			CPmessageHandler *invmsgh = this->msgh->dup()->setCpm((char *)cpmxstr);
			xmlFree (cpmxstr);
			invmsgh->setFailed(false); // the failure was detected, no new one yet
			invcpm->msgh = invmsgh;
			
			if (just_failed)
			{
				msgh->releaseMessage();
				leave ();
				site->guard
						->setGuard (new GuardedBranch(invcpm->getSid(),
													  invcpm->getBid(),
													  ROLLBACK))
						->guard(invcpm->getSid(), invcpm->getBid());
				ev << "start rolling back ...\n";
				scpRollback ();
				// ev << "====  EOI - invoker - before rollback:\n";
				// evDumpDoc(invcpm->doc);
				// ev << "\n======\n";
				invcpm->rollBack ();
				delete invcpm;
				delete invmsgh;
				break;
			}

			
			// ev << "====  EOI - invoker - before further enact:\n";
			// evDumpDoc(invcpm->doc);
			// ev << "\n======\n";
			invcpm->msgh->setProcessed (false)->setInterpreted(false);
			invcpm->invReturn (xmlGetIntProp(actelm, _ADDR));
			delete invcpm;
			delete invmsgh;

			// continue with the invoked branch
			applyC ();
			// ev << "====  EOI - invokee - before further enact:\n";
			// evDumpDoc(doc);
			// ev << "\n======\n";
			msgh->setMainflow(false);
			enact (); // finish up locally even for the centralized case
			break;
		}
		/*
		case EIF:
		{
			// continue rollingback the invoking branch

			Cpm *invcpm = new Cpm (site);
			invcpm->setToNull();
			invcpm->setFid (c->getIntAttr(X_FID));
			invcpm->setSid (c->getIntAttr(X_SID));
			invcpm->setSaddr (c->getIntAttr(X_SADDR));
			invcpm->setBid (bid);
			invcpm->setRollingback (true);
			invcpm->e->importSet (this->e->domelm);
			invcpm->e->setStatus (X_INV, ABORTED);
			// true for toggle betwen c and bwc
			invcpm->c->importAndToggle (XMLUtil::getFirstElementChild(c->domelm));
			invcpm->bwc->import (XMLUtil::getFirstElementChild(c->domelm));
			invcpm->applyC ();

			invReturn (invcpm);

			// continue rollingback the invoked branch
			applyC ();
			// ev << "====  EIF - invokee - before further enact:\n";
			// evDumpDoc(doc);
			// ev << "\n======\n";
			msgh->setMainflow(false);
			enact (); // finish up locally even for the centralized case
			break;
		}
		*/
		case JON:
		{
			e->branchDone ();

			int matchkey = poolMatchKey ();
			int count = xmlGetIntProp (actelm, _COUNT);

			// Note that msgh only preserves the contxt of the message
			// when the agent calls enact()
			// The current cpm state may not be the same as in the msg
			// when, for example, enact() calls itself
			// Therefore call refreshMsgH()
			refreshMsgH ();
			if ( site->pool->match(matchkey) == count - 1 )
			{
				// this is the last branch, join successful
				ev << "Join successful!\n";

				long jid = xmlGetIntProp (actelm, _JID);

				e->merge (matchkey);

				if (!rollingback)
				{
					xmlNodePtr rollback_flow =
						makeRollbackFlow (matchkey, jid);
						// notice side-effect
					bwc-> push (rollback_flow);
				}
				stopScope (jid, site->addr(),
							xmlGetIntProp (actelm, _PSID),
							xmlGetIntProp (actelm, _PSADDR));

				// merge timing info in msg
				// and remove the branch msg´s from the registry

				msgh->mergeMsg (matchkey);
				// ev << site->pool->dump() << endl;
				applyC ();
				// ev << "====  JON - before further enact:\n";
				// evDumpDoc(doc);
				// ev << "\n======\n";
				moveOn ();
			}
			else
			{
				site->pool->insert ( matchkey, msgh->Msg() );
				leave();
				// ev << site->pool->dump() << endl;
			}

			break;
		}
		case PRC:
		{
			leave();
			site->agent->processSvc(refreshMsgH(), svc_failrate);
			svc_failrate = 0.0;
			break;
		}
		case GTO:
		{
			applyC ()->enact ();
			break;
		}
		default:
		{
			ev << "actType does not exist!" << endl;
			applyC ()->moveOn();
		}
	}
}

// In centralized case, let the engin enact
void Cpm::moveOn ()
{
	if (centralized && site->addr() != 0)
		forward (0);

	else
		enact();
}

void Cpm::rollBack ()
{
	msgh->setRollingback (true)
		->setFailLastDetectTime ()
		->setRollbackStartTime ()
		->setProcessed (false)
		->setInterpreted(true);
		
	setRollingback (true);
	
	applyBwc();
	dispatch();
}

void Cpm::terminate ()
{
	ev << "CPM::terminate called" << endl;
	refreshMsgH()->terminate();
	leave();
}

Cpm* Cpm::applyC ()
{
	xmlPurgeNode (actelm);
	actelm = c->pop();
	ev << "type: " << (char *) (actelm->name) << endl;
	acttype = actType(actelm->name);
	return this;
}

Cpm* Cpm::applyBwc ()
{
	// ev << "====  applyBwc - before applying bwc:\n";
	// evDumpDoc(doc);
	
	xmlNodePtr curr_bwc_elm = bwc->domelm;
	xmlNodePtr curr_scp_elm = curr_bwc_elm->parent;
	// ev << "curr_scp_elm name: " << (char *)curr_scp_elm->name << endl;
	int psid, psaddr, iid;
	bool parent_is_scp = xmlStrEqual(curr_scp_elm->name, BAD_CAST _EOS);
	if (parent_is_scp)
	{
		psid = xmlGetIntProp (curr_scp_elm, _PSID);
		psaddr = xmlGetIntProp (curr_scp_elm, _PSADDR);
	}
	else
	{
		// bwc was enclosed in <eoi>
		iid = xmlGetIntProp (curr_scp_elm, _IID);
	}
	// ev << "curr_scp_elm:\n";
	// evDumpNode (curr_scp_elm);
	xmlUnlinkNode (curr_bwc_elm);
	c->trimTill (curr_scp_elm);
	if (parent_is_scp)
		c->push(makeEsf(psid, psaddr));
	else
		;// TODO:push Eif
	c->concatenate (curr_bwc_elm);
	// ev << "====  applyBwc - before further enact:\n";
	// evDumpDoc(doc);
	return this;
}

bool Cpm::isLocal () const
{
	// TODO: for decentralizaed scope, choose the site of the next op
	// to be the scp manager, if possible
	switch (acttype)
	{
		case RCV: case INV: case OWY: case JON:
		case SCP: case EOS: case ESF: case GTO:
		{
			int act_addr = xmlGetIntProp(actelm, _ADDR);
			return (act_addr == -1 || act_addr == site->addr());
		}
		default:
		{
			return true;
		}
	}
}

CPmessageHandler* Cpm::refreshMsgH ()
{
	xmlChar *cpmxstr = toXstr();
	// ev << "refresh cpm msg with size " << msgSize () << "\n";
	// ev << cpmstr << "\n-------------------------\n";
	msgh->setCpm ((char *)cpmxstr)->setByteLength (msgSize ())
	->setProcessed (false)->setInterpreted(false);
	xmlFree (cpmxstr);
	return msgh;
}

long Cpm::msgSize ()
{
	if (centralized)
		return c->msgSizeC ();
	else
		return c->msgSizeD () + e->msgSize ();
}

void Cpm::forward (int remote_addr, bool leave_guard)
{
	refreshMsgH ();
	if (leave_guard)
		leave();

	actelm = xmlPurgeNode(actelm);
	ev << "Forwarding to agent " << remote_addr <<endl;;

	if (!centralized)
		scpNewSite (remote_addr);
	msgh->forward (remote_addr);
}

// borrowed from Xerce-c XMLString::hash()
unsigned int Cpm::hash (const xmlChar *xstr)
{
	unsigned int hashVal = 0;
    if (xstr) {
		int len = xmlStrlen (xstr);
        for(int i = 0; i < len; ++i)
        {
            unsigned int top = hashVal >> 24;
            hashVal += (hashVal * 37) + top + (unsigned int)xstr[i];
        }
    }

    // Divide by modulus
	return hashVal; // % UINT_MAX; // UINT_MAX = 4294967295
}

// key used in msg pool
unsigned int Cpm::poolMatchKey ()
{
	unsigned int keyval = 0;
	switch ( acttype )
	{
		case RCV: case INV: case OWY:
		{
			xmlChar *opxstr = xmlGetProp(actelm, BAD_CAST _OP);
			keyval =  hash(opxstr);
			xmlFree (opxstr);
			break;
		}
		case JON:
		{
			xmlChar *jidxstr = xmlGetProp (actelm, BAD_CAST _JID);
			xmlChar *headingxstr = xmlGetProp (actelm, BAD_CAST _HEADING);
			keyval = hash (xmlStrcat (jidxstr, headingxstr));
			xmlFree (jidxstr);
			xmlFree (headingxstr);
			break;
		}
		default:
		{
		}
	}
	return keyval;
}

Cpm* Cpm::invCpm (bool just_failed)
{
	xmlNodePtr eoi_c_elm = xmlElementChildByName (actelm, _C);
	if (!eoi_c_elm)
	{
		ev << "Missing <c> in Eoi!" << endl;
		return NULL;
	}
	
	Cpm *invcpm = new Cpm (site);
	invcpm->setToNull();
	invcpm->setFid (xmlGetIntProp (actelm, _FID));
	invcpm->setSid (xmlGetIntProp (actelm, _SID));
	invcpm->setSaddr (xmlGetIntProp (actelm, _SADDR));
	invcpm->setBid (bid);
	if (rollingback)
	{
		invcpm->setRollingback(true);
	}
	
	invcpm->c->import (eoi_c_elm);
	invcpm->bwc->set (invcpm->c->currentBwc ());
	
	xmlNodePtr outvars_elm = xmlElementChildByName (cpmelm, _OUTS);
	if (outvars_elm)
		xmlAddChild (this->e->currelm, xmlCopyNode (outvars_elm, 1));
	invcpm->e->importSet (this->e->domelm);
	if (just_failed)
		invcpm->e->setStatus (_INV, ABORTED);
	else
		invcpm->e->setStatus (_INV, DONE);
	
	// do not add compensation during rollback
	if (just_failed || rollingback)
		return invcpm;
		
	xmlNodePtr chl_elm = xmlElementChildByName(actelm, _CHL);
	if (chl_elm)
	{
		// add the element of <compensationhandler> to bwc
		xmlNodePtr cmp_elm = xmlFirstElementChild (chl_elm);
		if (cmp_elm)
			invcpm->bwc
			->push (xmlDocCopyNode (cmp_elm, invcpm->doc, 1));
	}
	else
	{
		// concatenate the <bwc> of <eoi> to the current bwc
		xmlNodePtr eoi_bwc_elm = xmlElementChildByName (actelm, _BWC);
		invcpm->bwc
		->concatenate (xmlDocCopyNode (eoi_bwc_elm, invcpm->doc, 1));
	}
	return invcpm;
}

void Cpm::createScope ( long newsid )
{
	leave ();
	long psid = sid;
	int psaddr = saddr;
	setSid (newsid);
	setSaddrWithScp ();
	

	xmlNodePtr scpelm = xmlCopyNode(actelm, 2);
	if (acttype == FLW)
		xmlSetIntProp (scpelm, _JID, sid);
	else
		xmlSetIntProp (scpelm, _SID, sid);
	xmlNodePtr varselm = xmlElementChildByName(actelm, _VARS);
	if (varselm)
		xmlAddChild (scpelm, xmlCopyNode(varselm, 1));
	e->append (scpelm, _ACTIVE);
	
	scpNewScp (psid, psaddr);
	site->reg->insertScp(new ScpDesc (fid, sid, saddr, psid, psaddr));
	// ev << site->reg->dump() << endl;
	site->guard->guard(sid, bid);
	return;
}

void Cpm::stopScope (long esid, int esaddr, long epsid, int epsaddr)
{
	leave ();
	scpStop (esid, esaddr, epsid, epsaddr);
	setSid (epsid);
	setSaddr (epsaddr);
	setBid (c->getBid());
	site->guard->guard(sid, bid);
	return;
}

Cpm* Cpm::invReturn (int caller)
{
	if (return_to_caller &&  !isLocal())
	{
		// ev << "Returning to caller " << xmlGetIntProp(actelm, _ADDR) <<endl;
		// ev << "returning cpm msg with size " << invcpm->msgSize () << "\n";
		msgh->setByteLength (msgSize ());
		forward (xmlGetIntProp(actelm, _ADDR), false);
	}
	else
	if (centralized && site->addr() != 0)
	{
		// Should the size be calculated from eoi rather than <c>?
		// ev << "returning cpm msg with size " << msgSize () << "\n";
		msgh->setByteLength (msgSize ());
		forward (0, false);
	}
	else
	{
		msgh->setInterpreted (true);
		dispatch();
	}
	return this;
}

Cont* Cpm::createBwc (xmlNodePtr elm)
{
	xmlNodePtr bwc_elm = xmlNewNode (NULL, BAD_CAST _BWC);
	xmlAddChild ( elm, bwc_elm );
	xmlAddChild ( bwc_elm, xmlNewNode (NULL, BAD_CAST _NON) );
	bwc->set(bwc_elm);
	return bwc;
}

xmlNodePtr Cpm::makeEos (long psid, int psaddr, xmlNodePtr compensate_elm)
{
	xmlNodePtr elm = xmlNewNode (NULL, BAD_CAST _EOS);

	xmlSetIntProp (elm, _SID, sid);
	xmlSetIntProp (elm, _ADDR, saddr);
	xmlSetIntProp (elm, _PSID, psid);
	xmlSetIntProp (elm, _PSADDR, psaddr);
	createBwc ( elm );
	
	if (compensate_elm)
		xmlAddChild ( elm, compensate_elm );

	return elm;
}

xmlNodePtr Cpm::makeEsf ( long psid, int psaddr )
{
	xmlNodePtr elm = xmlNewNode (NULL, BAD_CAST _ESF);

	xmlSetIntProp (elm, _SID, sid);
	xmlSetIntProp (elm, _ADDR, saddr);
	xmlSetIntProp (elm, _PSID, psid);
	xmlSetIntProp (elm, _PSADDR, psaddr);

	return elm;
}

xmlNodePtr Cpm::makeJoin ( long jid, long psid, int psaddr,
						int count, int bid, const char *heading )
{
	xmlNodePtr elm = xmlNewNode (NULL, BAD_CAST _JON);

	xmlSetIntProp (elm, _JID, jid);
	xmlSetIntProp (elm, _PSID, psid);
	xmlSetIntProp (elm, _PSADDR, psaddr);
	xmlSetIntProp (elm, _ADDR, site->addr());
	xmlSetIntProp (elm, _COUNT, count);
	xmlSetIntProp (elm, _BID, bid);
	xmlSetProp (elm, BAD_CAST _HEADING, BAD_CAST heading);

	return elm;
}

// side-effect: bwc pops till join element!
xmlNodePtr Cpm::makeRollbackFlow ( int matchkey, long jid)
{
	xmlNodePtr flow_elm = xmlNewNode (NULL, BAD_CAST _FLW);
	int count = site->pool->match (matchkey);
	MsgMapIt mit = site->pool->getItor (matchkey);

	xmlAddChild (flow_elm, makeRollbackFlowBranch(this, jid));

	for (int i = 1; i <= count; i++)
	{
		// Why? When the following two line is merged into one,
		// memory failure.
		// new Cpm (site, strToDoc (mit->second->getCpm()))
		Cpm *branch_cpm = new Cpm (site);
		branch_cpm->set (strToDoc (mit->second->getCpm()));
		xmlAddChild (flow_elm, makeRollbackFlowBranch(branch_cpm, jid));
		delete branch_cpm;

		++mit;
	}
	return flow_elm;
}

xmlNodePtr Cpm::makeRollbackFlowBranch ( Cpm *branch_cpm, long jid )
{
	xmlNodePtr branch_elm = xmlNewNode (NULL, BAD_CAST _SEQ);

	xmlNodePtr branch_bwc_elm = branch_cpm->bwc->pop();
	while ( branch_bwc_elm )
	{
		if ( xmlStrEqual (branch_bwc_elm->name, BAD_CAST _JON) )
		{
			if ( xmlGetIntProp (branch_bwc_elm, _JID) == jid )
			{
				xmlPurgeNode(branch_bwc_elm);
				break;
			}
		}
		xmlAddChild(branch_elm, xmlDocCopyNode (branch_bwc_elm,doc, 1));
		xmlPurgeNode(branch_bwc_elm);
		branch_bwc_elm = branch_cpm->bwc->pop();
	}
	return branch_elm;
}

xmlNodePtr Cpm::makeEoi ( long iid )
{
	xmlNodePtr elm = xmlNewNode (NULL, BAD_CAST _EOI);

	xmlSetIntProp (elm, _FID, fid);
	xmlSetIntProp (elm, _SID, sid);
	xmlSetIntProp (elm, _SADDR, saddr);
	xmlSetIntProp (elm, _IID, iid);
	xmlChar *opxstr = xmlGetProp(elm, BAD_CAST _OP);
	xmlChar *srcxstr = xmlGetProp(elm, BAD_CAST _SRC);
	xmlSetProp (elm, BAD_CAST _OP, opxstr);
	xmlSetProp (elm, BAD_CAST _SRC, srcxstr);
	xmlFree (opxstr);
	xmlFree (srcxstr);

	xmlNodePtr outvars_elm = xmlElementChildByName (actelm, _OUTS);
	if (outvars_elm)
		xmlAddChild (elm, xmlCopyNode (outvars_elm, 1));
	
	createBwc ( elm );
	
	xmlNodePtr compensate_elm = xmlElementChildByName (actelm, _CHL);
	if (compensate_elm)
		xmlAddChild (elm, xmlCopyNode (compensate_elm, 1));
	xmlAddChild (elm, xmlCopyNode (c->domelm, 1));
	return elm;
}

xmlNodePtr Cpm::makeEif ( long iid )
{
	xmlNodePtr elm = xmlNewNode (NULL, BAD_CAST _EIF);

	xmlSetIntProp (elm, _FID, fid);
	xmlSetIntProp (elm, _SID, sid);
	xmlSetIntProp (elm, _SADDR, saddr);
	xmlSetIntProp (elm, _IID, iid);
	xmlChar *opxstr = xmlGetProp(elm, BAD_CAST _OP);
	xmlChar *srcxstr = xmlGetProp(elm, BAD_CAST _SRC);
	xmlSetProp (elm, BAD_CAST _OP, opxstr);
	xmlSetProp (elm, BAD_CAST _SRC, opxstr);
	xmlFree (opxstr);
	xmlFree (srcxstr);
	xmlAddChild (elm, xmlCopyNode(bwc->domelm, 1));
	return elm;
}

void Cpm::scpNewSite (int newsite)
{
	if (saddr == -1) return;
	ScpAct scpact (site, fid, sid, saddr, bid, site->addr(), rollingback);
	scpact.setAct(NEWSITE);
	if (centralized)
		scpact.setNewSite (0); // all management thing happens in the engine
	else
		scpact.setNewSite (newsite);
	scpact.sendToScpMgr ();
}

void Cpm::scpNewScp (long psid, int psaddr)
{
	if (psaddr == -1) return;
	ScpAct scpact (site, fid, psid, psaddr, bid, site->addr(), rollingback);
	scpact.setAct (NEWSCP);
	scpact.setNewScp (sid);
	scpact.sendToScpMgr ();
	return;

}

void Cpm::scpStop ( long esid, int esaddr, long epsid, int epsaddr )
{
	ScpAct scpact (site, fid, esid, esaddr, bid, site->addr(), rollingback);
	scpact.setAct (STOPSCP);
	scpact.setParent (epsid, epsaddr);
	scpact.sendToScpMgr ();
	return;
}

void Cpm::scpRollback ()
{
	ScpAct scpact (site, fid, sid, saddr, bid, site->addr(), rollingback);
	scpact.setAct (ROLLBACKSCP);
	scpact.sendToScpMgr ();
	return;
}
