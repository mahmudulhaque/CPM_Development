//
// Generated file, do not edit! Created by opp_msgc 4.1 from SCPmessage.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "SCPmessage_m.h"

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




Register_Class(SCPmessage);

SCPmessage::SCPmessage(const char *name, int kind) : cPacket(name,kind)
{
    this->src_var = 0;
    this->dest_var = 0;
    this->cmd_var = 0;
    this->processed_var = false;
}

SCPmessage::SCPmessage(const SCPmessage& other) : cPacket()
{
    setName(other.getName());
    operator=(other);
}

SCPmessage::~SCPmessage()
{
}

SCPmessage& SCPmessage::operator=(const SCPmessage& other)
{
    if (this==&other) return *this;
    cPacket::operator=(other);
    this->src_var = other.src_var;
    this->dest_var = other.dest_var;
    this->cmd_var = other.cmd_var;
    this->processed_var = other.processed_var;
    return *this;
}

void SCPmessage::parsimPack(cCommBuffer *b)
{
    cPacket::parsimPack(b);
    doPacking(b,this->src_var);
    doPacking(b,this->dest_var);
    doPacking(b,this->cmd_var);
    doPacking(b,this->processed_var);
}

void SCPmessage::parsimUnpack(cCommBuffer *b)
{
    cPacket::parsimUnpack(b);
    doUnpacking(b,this->src_var);
    doUnpacking(b,this->dest_var);
    doUnpacking(b,this->cmd_var);
    doUnpacking(b,this->processed_var);
}

int SCPmessage::getSrc() const
{
    return src_var;
}

void SCPmessage::setSrc(int src_var)
{
    this->src_var = src_var;
}

int SCPmessage::getDest() const
{
    return dest_var;
}

void SCPmessage::setDest(int dest_var)
{
    this->dest_var = dest_var;
}

const char * SCPmessage::getCmd() const
{
    return cmd_var.c_str();
}

void SCPmessage::setCmd(const char * cmd_var)
{
    this->cmd_var = cmd_var;
}

bool SCPmessage::getProcessed() const
{
    return processed_var;
}

void SCPmessage::setProcessed(bool processed_var)
{
    this->processed_var = processed_var;
}

class SCPmessageDescriptor : public cClassDescriptor
{
  public:
    SCPmessageDescriptor();
    virtual ~SCPmessageDescriptor();

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

Register_ClassDescriptor(SCPmessageDescriptor);

SCPmessageDescriptor::SCPmessageDescriptor() : cClassDescriptor("SCPmessage", "cPacket")
{
}

SCPmessageDescriptor::~SCPmessageDescriptor()
{
}

bool SCPmessageDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<SCPmessage *>(obj)!=NULL;
}

const char *SCPmessageDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int SCPmessageDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount(object) : 4;
}

unsigned int SCPmessageDescriptor::getFieldTypeFlags(void *object, int field) const
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
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *SCPmessageDescriptor::getFieldName(void *object, int field) const
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
        "cmd",
        "processed",
    };
    return (field>=0 && field<4) ? fieldNames[field] : NULL;
}

int SCPmessageDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "src")==0) return base+0;
    if (fieldName[0]=='d' && strcmp(fieldName, "dest")==0) return base+1;
    if (fieldName[0]=='c' && strcmp(fieldName, "cmd")==0) return base+2;
    if (fieldName[0]=='p' && strcmp(fieldName, "processed")==0) return base+3;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *SCPmessageDescriptor::getFieldTypeString(void *object, int field) const
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
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : NULL;
}

const char *SCPmessageDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int SCPmessageDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    SCPmessage *pp = (SCPmessage *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string SCPmessageDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    SCPmessage *pp = (SCPmessage *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getSrc());
        case 1: return long2string(pp->getDest());
        case 2: return oppstring2string(pp->getCmd());
        case 3: return bool2string(pp->getProcessed());
        default: return "";
    }
}

bool SCPmessageDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    SCPmessage *pp = (SCPmessage *)object; (void)pp;
    switch (field) {
        case 0: pp->setSrc(string2long(value)); return true;
        case 1: pp->setDest(string2long(value)); return true;
        case 2: pp->setCmd((value)); return true;
        case 3: pp->setProcessed(string2bool(value)); return true;
        default: return false;
    }
}

const char *SCPmessageDescriptor::getFieldStructName(void *object, int field) const
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
    };
    return (field>=0 && field<4) ? fieldStructNames[field] : NULL;
}

void *SCPmessageDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    SCPmessage *pp = (SCPmessage *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


