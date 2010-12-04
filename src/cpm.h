//-------------------------------------------------------------
// File: cpm.h
//     (part of OMNeT++ simulation of CPMnet)
// Author: Weihai Yu
//-------------------------------------------------------------

#ifndef __CPM_H
#define __CPM_H

#include "cpmcommon.h"
#include "cont.h"
#include "environ.h"
#include "cpmsite.h"

class CPmessageHandler;
class ScpAct;

class Cpm
{
	public:
		//unique flow id (fid), scope id (sid) or join id (jid)
		static long nextid; 
		
		Cpm ( CPMsite* s, xmlDocPtr cpmdoc = NULL );
		~Cpm ();

		Cpm* set ( xmlDocPtr newdoc );
		Cpm* set ( const char *str );
		
		void dispatch ( CPmessageHandler *cmsgh = NULL );
		void leave ();
		
		void enact ();
		void moveOn ();
		
		void rollBack ();
		bool isRollingback () { return rollingback; };
		
		void terminate ();
		Cpm* applyC ();
		Cpm* applyBwc ();

		long getFid () const { return fid; };
		long getSid () const { return sid; };
		long getBid () const { return bid; };
		int getSaddr () const { return saddr; };
		
		
		static void resetId () { nextid = 0; };
		CPMsite* site;

		xmlDocPtr doc;
		xmlNodePtr cpmelm, actelm;
		ActType acttype;
		Environ* e;
		Cont* c;
		Cont* bwc;
		CPmessageHandler *msgh;
	private:
		long fid;
		long sid;
		long bid;
		int saddr;
		bool return_to_caller; // whether return to the invoker
		bool rollingback;
		bool centralized;
		double svc_failrate;
		
		Cpm* setToNull ();
		Cpm* release ();
		
		Cpm* setFid (long newfid);
		Cpm* setNewSid ();
		Cpm* setSid (long newsid);
		Cpm* setBid (long newbid);
		Cpm* setSaddrWithScp ();
		Cpm* setSaddr (int newsaddr);
		Cpm* setRollingback (bool to_rollback);
		
		Cpm* setReturn (bool back_to_caller = false);
		xmlChar* toXstr ();
		void invalidCpm ();
		bool isLocal () const;
		bool isEnd () const { return acttype == NON; };
		
		CPmessageHandler* refreshMsgH ();
		long msgSize ();
		void forward (int remote_addr, bool leave_guard = true);
		unsigned int hash (const xmlChar* xstr);
		unsigned int poolMatchKey ();
		
		Cpm* invCpm ( bool just_failed = false );
		
		void createScope ( long newsid );
		void stopScope (long esid, int esaddr, long epsid, int epsaddr);
		Cpm* invReturn (int caller);
		
		Cont* createBwc (xmlNodePtr elm);
		xmlNodePtr makeEos (long psid, int psaddr, xmlNodePtr compensate_elm);
		xmlNodePtr makeEsf (long psid, int psaddr);
		xmlNodePtr makeJoin ( long jid, long psid, int psaddr,
								int count, int bid, const char *heading );
		xmlNodePtr makeRollbackFlow ( int matchkey, long jid );
		xmlNodePtr makeRollbackFlowBranch ( Cpm *branch_cpm, long jid );
		xmlNodePtr makeEoi ( long iid );
		xmlNodePtr makeEif ( long iid );
		
		void scpNewSite (int newsite);
		void scpNewScp ( long psid, int psaddr);
		void scpStop ( long esid, int esaddr, long epsid, int epsaddr );
		void scpRollback ();
};

#endif