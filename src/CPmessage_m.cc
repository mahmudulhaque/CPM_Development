//
// Generated file, do not edit! Created by opp_msgc 4.1 from CPmessage.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "CPmessage_m.h"

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




Register_Class(CPmessage);

CPmessage::CPmessage(const char *name, int kind) : cPacket(name,kind)
{
    this->src_var = 0;
    this->dest_var = 0;
    this->cpm_var = 0;
    this->rollingback_var = false;
    this->mainflow_var = true;
    this->interpreted_var = false;
    this->processed_var = false;
    this->failed_var = false;
    this->startTime_var = 0.0;
    this->procTime_var = 0.0;
    this->interpretTime_var = 0.0;
    this->queueTime_var = 0.0;
    this->queueDelay_var = 0.0;
    this->skew_var = 0.0;
    this->commTime_var = 0.0;
    this->failLastDetectTime_var = 0.0;
    this->rollbackStartTime_var = 0.0;
    this->numRepeats_var = 0;
    this->numSites_var = 0;
    for (unsigned int i=0; i<100; i++)
        this->sites_var[i] = 0;
    this->numSitesVisited_var = 0;
    for (unsigned int i=0; i<100; i++)
        this->sitesVisited_var[i] = 0;
}

CPmessage::CPmessage(const CPmessage& other) : cPacket()
{
    setName(other.getName());
    operator=(other);
}

CPmessage::~CPmessage()
{
}

CPmessage& CPmessage::operator=(const CPmessage& other)
{
    if (this==&other) return *this;
    cPacket::operator=(other);
    this->src_var = other.src_var;
    this->dest_var = other.dest_var;
    this->cpm_var = other.cpm_var;
    this->rollingback_var = other.rollingback_var;
    this->mainflow_var = other.mainflow_var;
    this->interpreted_var = other.interpreted_var;
    this->processed_var = other.processed_var;
    this->failed_var = other.failed_var;
    this->startTime_var = other.startTime_var;
    this->procTime_var = other.procTime_var;
    this->interpretTime_var = other.interpretTime_var;
    this->queueTime_var = other.queueTime_var;
    this->queueDelay_var = other.queueDelay_var;
    this->skew_var = other.skew_var;
    this->commTime_var = other.commTime_var;
    this->failLastDetectTime_var = other.failLastDetectTime_var;
    this->rollbackStartTime_var = other.rollbackStartTime_var;
    this->numRepeats_var = other.numRepeats_var;
    this->numSites_var = other.numSites_var;
    for (unsigned int i=0; i<100; i++)
        this->sites_var[i] = other.sites_var[i];
    this->numSitesVisited_var = other.numSitesVisited_var;
    for (unsigned int i=0; i<100; i++)
        this->sitesVisited_var[i] = other.sitesVisited_var[i];
    return *this;
}

void CPmessage::parsimPack(cCommBuffer *b)
{
    cPacket::parsimPack(b);
    doPacking(b,this->src_var);
    doPacking(b,this->dest_var);
    doPacking(b,this->cpm_var);
    doPacking(b,this->rollingback_var);
    doPacking(b,this->mainflow_var);
    doPacking(b,this->interpreted_var);
    doPacking(b,this->processed_var);
    doPacking(b,this->failed_var);
    doPacking(b,this->startTime_var);
    doPacking(b,this->procTime_var);
    doPacking(b,this->interpretTime_var);
    doPacking(b,this->queueTime_var);
    doPacking(b,this->queueDelay_var);
    doPacking(b,this->skew_var);
    doPacking(b,this->commTime_var);
    doPacking(b,this->failLastDetectTime_var);
    doPacking(b,this->rollbackStartTime_var);
    doPacking(b,this->numRepeats_var);
    doPacking(b,this->numSites_var);
    doPacking(b,this->sites_var,100);
    doPacking(b,this->numSitesVisited_var);
    doPacking(b,this->sitesVisited_var,100);
}

void CPmessage::parsimUnpack(cCommBuffer *b)
{
    cPacket::parsimUnpack(b);
    doUnpacking(b,this->src_var);
    doUnpacking(b,this->dest_var);
    doUnpacking(b,this->cpm_var);
    doUnpacking(b,this->rollingback_var);
    doUnpacking(b,this->mainflow_var);
    doUnpacking(b,this->interpreted_var);
    doUnpacking(b,this->processed_var);
    doUnpacking(b,this->failed_var);
    doUnpacking(b,this->startTime_var);
    doUnpacking(b,this->procTime_var);
    doUnpacking(b,this->interpretTime_var);
    doUnpacking(b,this->queueTime_var);
    doUnpacking(b,this->queueDelay_var);
    doUnpacking(b,this->skew_var);
    doUnpacking(b,this->commTime_var);
    doUnpacking(b,this->failLastDetectTime_var);
    doUnpacking(b,this->rollbackStartTime_var);
    doUnpacking(b,this->numRepeats_var);
    doUnpacking(b,this->numSites_var);
    doUnpacking(b,this->sites_var,100);
    doUnpacking(b,this->numSitesVisited_var);
    doUnpacking(b,this->sitesVisited_var,100);
}

int CPmessage::getSrc() const
{
    return src_var;
}

void CPmessage::setSrc(int src_var)
{
    this->src_var = src_var;
}

int CPmessage::getDest() const
{
    return dest_var;
}

void CPmessage::setDest(int dest_var)
{
    this->dest_var = dest_var;
}

const char * CPmessage::getCpm() const
{
    return cpm_var.c_str();
}

void CPmessage::setCpm(const char * cpm_var)
{
    this->cpm_var = cpm_var;
}

bool CPmessage::getRollingback() const
{
    return rollingback_var;
}

void CPmessage::setRollingback(bool rollingback_var)
{
    this->rollingback_var = rollingback_var;
}

bool CPmessage::getMainflow() const
{
    return mainflow_var;
}

void CPmessage::setMainflow(bool mainflow_var)
{
    this->mainflow_var = mainflow_var;
}

bool CPmessage::getInterpreted() const
{
    return interpreted_var;
}

void CPmessage::setInterpreted(bool interpreted_var)
{
    this->interpreted_var = interpreted_var;
}

bool CPmessage::getProcessed() const
{
    return processed_var;
}

void CPmessage::setProcessed(bool processed_var)
{
    this->processed_var = processed_var;
}

bool CPmessage::getFailed() const
{
    return failed_var;
}

void CPmessage::setFailed(bool failed_var)
{
    this->failed_var = failed_var;
}

simtime_t CPmessage::getStartTime() const
{
    return startTime_var;
}

void CPmessage::setStartTime(simtime_t startTime_var)
{
    this->startTime_var = startTime_var;
}

simtime_t CPmessage::getProcTime() const
{
    return procTime_var;
}

void CPmessage::setProcTime(simtime_t procTime_var)
{
    this->procTime_var = procTime_var;
}

simtime_t CPmessage::getInterpretTime() const
{
    return interpretTime_var;
}

void CPmessage::setInterpretTime(simtime_t interpretTime_var)
{
    this->interpretTime_var = interpretTime_var;
}

simtime_t CPmessage::getQueueTime() const
{
    return queueTime_var;
}

void CPmessage::setQueueTime(simtime_t queueTime_var)
{
    this->queueTime_var = queueTime_var;
}

simtime_t CPmessage::getQueueDelay() const
{
    return queueDelay_var;
}

void CPmessage::setQueueDelay(simtime_t queueDelay_var)
{
    this->queueDelay_var = queueDelay_var;
}

simtime_t CPmessage::getSkew() const
{
    return skew_var;
}

void CPmessage::setSkew(simtime_t skew_var)
{
    this->skew_var = skew_var;
}

simtime_t CPmessage::getCommTime() const
{
    return commTime_var;
}

void CPmessage::setCommTime(simtime_t commTime_var)
{
    this->commTime_var = commTime_var;
}

simtime_t CPmessage::getFailLastDetectTime() const
{
    return failLastDetectTime_var;
}

void CPmessage::setFailLastDetectTime(simtime_t failLastDetectTime_var)
{
    this->failLastDetectTime_var = failLastDetectTime_var;
}

simtime_t CPmessage::getRollbackStartTime() const
{
    return rollbackStartTime_var;
}

void CPmessage::setRollbackStartTime(simtime_t rollbackStartTime_var)
{
    this->rollbackStartTime_var = rollbackStartTime_var;
}

int CPmessage::getNumRepeats() const
{
    return numRepeats_var;
}

void CPmessage::setNumRepeats(int numRepeats_var)
{
    this->numRepeats_var = numRepeats_var;
}

int CPmessage::getNumSites() const
{
    return numSites_var;
}

void CPmessage::setNumSites(int numSites_var)
{
    this->numSites_var = numSites_var;
}

unsigned int CPmessage::getSitesArraySize() const
{
    return 100;
}

int CPmessage::getSites(unsigned int k) const
{
    if (k>=100) throw cRuntimeError("Array of size 100 indexed by %lu", (unsigned long)k);
    return sites_var[k];
}

void CPmessage::setSites(unsigned int k, int sites_var)
{
    if (k>=100) throw cRuntimeError("Array of size 100 indexed by %lu", (unsigned long)k);
    this->sites_var[k] = sites_var;
}

int CPmessage::getNumSitesVisited() const
{
    return numSitesVisited_var;
}

void CPmessage::setNumSitesVisited(int numSitesVisited_var)
{
    this->numSitesVisited_var = numSitesVisited_var;
}

unsigned int CPmessage::getSitesVisitedArraySize() const
{
    return 100;
}

int CPmessage::getSitesVisited(unsigned int k) const
{
    if (k>=100) throw cRuntimeError("Array of size 100 indexed by %lu", (unsigned long)k);
    return sitesVisited_var[k];
}

void CPmessage::setSitesVisited(unsigned int k, int sitesVisited_var)
{
    if (k>=100) throw cRuntimeError("Array of size 100 indexed by %lu", (unsigned long)k);
    this->sitesVisited_var[k] = sitesVisited_var;
}

class CPmessageDescriptor : public cClassDescriptor
{
  public:
    CPmessageDescriptor();
    virtual ~CPmessageDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(CPmessageDescriptor);

CPmessageDescriptor::CPmessageDescriptor() : cClassDescriptor("CPmessage", "cPacket")
{
}

CPmessageDescriptor::~CPmessageDescriptor()
{
}

bool CPmessageDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<CPmessage *>(obj)!=NULL;
}

const char *CPmessageDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int CPmessageDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 22+basedesc->getFieldCount(object) : 22;
}

unsigned int CPmessageDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
    };
    return (field>=0 && field<22) ? fieldTypeFlags[field] : 0;
}

const char *CPmessageDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "src",
        "dest",
        "cpm",
        "rollingback",
        "mainflow",
        "interpreted",
        "processed",
        "failed",
        "startTime",
        "procTime",
        "interpretTime",
        "queueTime",
        "queueDelay",
        "skew",
        "commTime",
        "failLastDetectTime",
        "rollbackStartTime",
        "numRepeats",
        "numSites",
        "sites",
        "numSitesVisited",
        "sitesVisited",
    };
    return (field>=0 && field<22) ? fieldNames[field] : NULL;
}

int CPmessageDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "src")==0) return base+0;
    if (fieldName[0]=='d' && strcmp(fieldName, "dest")==0) return base+1;
    if (fieldName[0]=='c' && strcmp(fieldName, "cpm")==0) return base+2;
    if (fieldName[0]=='r' && strcmp(fieldName, "rollingback")==0) return base+3;
    if (fieldName[0]=='m' && strcmp(fieldName, "mainflow")==0) return base+4;
    if (fieldName[0]=='i' && strcmp(fieldName, "interpreted")==0) return base+5;
    if (fieldName[0]=='p' && strcmp(fieldName, "processed")==0) return base+6;
    if (fieldName[0]=='f' && strcmp(fieldName, "failed")==0) return base+7;
    if (fieldName[0]=='s' && strcmp(fieldName, "startTime")==0) return base+8;
    if (fieldName[0]=='p' && strcmp(fieldName, "procTime")==0) return base+9;
    if (fieldName[0]=='i' && strcmp(fieldName, "interpretTime")==0) return base+10;
    if (fieldName[0]=='q' && strcmp(fieldName, "queueTime")==0) return base+11;
    if (fieldName[0]=='q' && strcmp(fieldName, "queueDelay")==0) return base+12;
    if (fieldName[0]=='s' && strcmp(fieldName, "skew")==0) return base+13;
    if (fieldName[0]=='c' && strcmp(fieldName, "commTime")==0) return base+14;
    if (fieldName[0]=='f' && strcmp(fieldName, "failLastDetectTime")==0) return base+15;
    if (fieldName[0]=='r' && strcmp(fieldName, "rollbackStartTime")==0) return base+16;
    if (fieldName[0]=='n' && strcmp(fieldName, "numRepeats")==0) return base+17;
    if (fieldName[0]=='n' && strcmp(fieldName, "numSites")==0) return base+18;
    if (fieldName[0]=='s' && strcmp(fieldName, "sites")==0) return base+19;
    if (fieldName[0]=='n' && strcmp(fieldName, "numSitesVisited")==0) return base+20;
    if (fieldName[0]=='s' && strcmp(fieldName, "sitesVisited")==0) return base+21;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *CPmessageDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "string",
        "bool",
        "bool",
        "bool",
        "bool",
        "bool",
        "simtime_t",
        "simtime_t",
        "simtime_t",
        "simtime_t",
        "simtime_t",
        "simtime_t",
        "simtime_t",
        "simtime_t",
        "simtime_t",
        "int",
        "int",
        "int",
        "int",
        "int",
    };
    return (field>=0 && field<22) ? fieldTypeStrings[field] : NULL;
}

const char *CPmessageDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int CPmessageDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    CPmessage *pp = (CPmessage *)object; (void)pp;
    switch (field) {
        case 19: return 100;
        case 21: return 100;
        default: return 0;
    }
}

std::string CPmessageDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    CPmessage *pp = (CPmessage *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getSrc());
        case 1: return long2string(pp->getDest());
        case 2: return oppstring2string(pp->getCpm());
        case 3: return bool2string(pp->getRollingback());
        case 4: return bool2string(pp->getMainflow());
        case 5: return bool2string(pp->getInterpreted());
        case 6: return bool2string(pp->getProcessed());
        case 7: return bool2string(pp->getFailed());
        case 8: return double2string(pp->getStartTime());
        case 9: return double2string(pp->getProcTime());
        case 10: return double2string(pp->getInterpretTime());
        case 11: return double2string(pp->getQueueTime());
        case 12: return double2string(pp->getQueueDelay());
        case 13: return double2string(pp->getSkew());
        case 14: return double2string(pp->getCommTime());
        case 15: return double2string(pp->getFailLastDetectTime());
        case 16: return double2string(pp->getRollbackStartTime());
        case 17: return long2string(pp->getNumRepeats());
        case 18: return long2string(pp->getNumSites());
        case 19: return long2string(pp->getSites(i));
        case 20: return long2string(pp->getNumSitesVisited());
        case 21: return long2string(pp->getSitesVisited(i));
        default: return "";
    }
}

bool CPmessageDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    CPmessage *pp = (CPmessage *)object; (void)pp;
    switch (field) {
        case 0: pp->setSrc(string2long(value)); return true;
        case 1: pp->setDest(string2long(value)); return true;
        case 2: pp->setCpm((value)); return true;
        case 3: pp->setRollingback(string2bool(value)); return true;
        case 4: pp->setMainflow(string2bool(value)); return true;
        case 5: pp->setInterpreted(string2bool(value)); return true;
        case 6: pp->setProcessed(string2bool(value)); return true;
        case 7: pp->setFailed(string2bool(value)); return true;
        case 8: pp->setStartTime(string2double(value)); return true;
        case 9: pp->setProcTime(string2double(value)); return true;
        case 10: pp->setInterpretTime(string2double(value)); return true;
        case 11: pp->setQueueTime(string2double(value)); return true;
        case 12: pp->setQueueDelay(string2double(value)); return true;
        case 13: pp->setSkew(string2double(value)); return true;
        case 14: pp->setCommTime(string2double(value)); return true;
        case 15: pp->setFailLastDetectTime(string2double(value)); return true;
        case 16: pp->setRollbackStartTime(string2double(value)); return true;
        case 17: pp->setNumRepeats(string2long(value)); return true;
        case 18: pp->setNumSites(string2long(value)); return true;
        case 19: pp->setSites(i,string2long(value)); return true;
        case 20: pp->setNumSitesVisited(string2long(value)); return true;
        case 21: pp->setSitesVisited(i,string2long(value)); return true;
        default: return false;
    }
}

const char *CPmessageDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    };
    return (field>=0 && field<22) ? fieldStructNames[field] : NULL;
}

void *CPmessageDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    CPmessage *pp = (CPmessage *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


