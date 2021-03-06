#
# OMNeT++/OMNEST Makefile for CPM_DEVELOPMENT
#
# This file was generated with the command:
#  opp_makemake -f --deep -I/usr/include/libxml2 -I../../inet/src/networklayer/ipv4 -I../../inet/src/networklayer/common -I../../inet/src/networklayer/rsvp_te -I../../inet/src/networklayer/icmpv6 -I../../inet/src/transport/tcp -I../../inet/src/networklayer/mpls -I../../inet/src/base -I../../inet/src/networklayer/ted -I../../inet/src/util/headerserializers -I../../inet/src/networklayer/contract -I../../inet/src/util -I../../inet/src/transport/contract -I../../inet/src/linklayer/mfcore -I../../inet/src/transport/sctp -I../../inet/src/networklayer/ipv6 -I../../inet/src/applications/tcpapp -I../../inet/src/world -I../../inet/src/applications/pingapp -I../../inet/src/linklayer/contract -I../../inet/src/networklayer/arp -I../../inet/src/networklayer/ldp -I../../inet/src/transport/udp -L../../inet/out/$(CONFIGNAME)/src -lxml2 -linet -KINET_PROJ=../../inet
#

# Name of target to be created (-o option)
TARGET = CPM_DEVELOPMENT$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I/usr/include/libxml2 \
    -I../../inet/src/networklayer/ipv4 \
    -I../../inet/src/networklayer/common \
    -I../../inet/src/networklayer/rsvp_te \
    -I../../inet/src/networklayer/icmpv6 \
    -I../../inet/src/transport/tcp \
    -I../../inet/src/networklayer/mpls \
    -I../../inet/src/base \
    -I../../inet/src/networklayer/ted \
    -I../../inet/src/util/headerserializers \
    -I../../inet/src/networklayer/contract \
    -I../../inet/src/util \
    -I../../inet/src/transport/contract \
    -I../../inet/src/linklayer/mfcore \
    -I../../inet/src/transport/sctp \
    -I../../inet/src/networklayer/ipv6 \
    -I../../inet/src/applications/tcpapp \
    -I../../inet/src/world \
    -I../../inet/src/applications/pingapp \
    -I../../inet/src/linklayer/contract \
    -I../../inet/src/networklayer/arp \
    -I../../inet/src/networklayer/ldp \
    -I../../inet/src/transport/udp \
    -I.

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = -L../../inet/out/$(CONFIGNAME)/src  -lxml2 -linet
LIBS += -Wl,-rpath,`abspath ../../inet/out/$(CONFIGNAME)/src`

# Output directory
PROJECT_OUTPUT_DIR = ../out
PROJECTRELATIVE_PATH = src
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = \
    $O/agent.o \
    $O/cont.o \
    $O/cpm.o \
    $O/cpmcommon.o \
    $O/CPmessageHandler.o \
    $O/cpmsite.o \
    $O/environ.o \
    $O/feeder.o \
    $O/scpact.o \
    $O/SCPmessageHandler.o \
    $O/sink.o \
    $O/CPmessage_m.o \
    $O/SCPmessage_m.o

# Message files
MSGFILES = \
    CPmessage.msg \
    SCPmessage.msg

# Other makefile variables (-K)
INET_PROJ=../../inet

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $(TARGET)

$(TARGET) : $O/$(TARGET)
	$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY:

.SUFFIXES: .cc

$O/%.o: %.cc
	@$(MKPATH) $(dir $@)
	$(CXX) -c $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(MSGC) -s _m.cc $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

clean:
	-rm -rf $O
	-rm -f CPM_DEVELOPMENT CPM_DEVELOPMENT.exe libCPM_DEVELOPMENT.so libCPM_DEVELOPMENT.a libCPM_DEVELOPMENT.dll libCPM_DEVELOPMENT.dylib
	-rm -f ./*_m.cc ./*_m.h

cleanall: clean
	-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/agent.o: agent.cc \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h \
	$(INET_PROJ)/src/networklayer/rsvp_te/IntServ.h \
	$(INET_PROJ)/src/applications/tcpapp/TCPGenericCliAppBase.h \
	SCPmessage_m.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/rsvp_te/Utils.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPRoute.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/transport/contract/TCPSocket.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	CPmessageHandler.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	environ.h \
	$(INET_PROJ)/src/networklayer/rsvp_te/IntServ_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	CPmessage_m.h \
	$(INET_PROJ)/src/networklayer/ipv4/RoutingTableAccess.h \
	$(INET_PROJ)/src/transport/contract/TCPCommand_m.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceTableAccess.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	cont.h \
	cpmsite.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	cpm.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	cpmcommon.h \
	$(INET_PROJ)/src/transport/contract/TCPSocketMap.h \
	agent.h \
	$(INET_PROJ)/src/networklayer/ipv4/IRoutingTable.h
$O/scpact.o: scpact.cc \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h \
	$(INET_PROJ)/src/networklayer/rsvp_te/IntServ.h \
	$(INET_PROJ)/src/applications/tcpapp/TCPGenericCliAppBase.h \
	SCPmessage_m.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPRoute.h \
	$(INET_PROJ)/src/networklayer/rsvp_te/Utils.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/transport/contract/TCPSocket.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	CPmessageHandler.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	environ.h \
	$(INET_PROJ)/src/networklayer/rsvp_te/IntServ_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	CPmessage_m.h \
	$(INET_PROJ)/src/networklayer/ipv4/RoutingTableAccess.h \
	$(INET_PROJ)/src/transport/contract/TCPCommand_m.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceTableAccess.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	cont.h \
	cpmsite.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	cpm.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	scpact.h \
	$(INET_PROJ)/src/transport/contract/TCPSocketMap.h \
	cpmcommon.h \
	agent.h \
	$(INET_PROJ)/src/networklayer/ipv4/IRoutingTable.h
$O/cpm.o: cpm.cc \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h \
	$(INET_PROJ)/src/networklayer/rsvp_te/IntServ.h \
	$(INET_PROJ)/src/applications/tcpapp/TCPGenericCliAppBase.h \
	SCPmessage_m.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPRoute.h \
	$(INET_PROJ)/src/networklayer/rsvp_te/Utils.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/transport/contract/TCPSocket.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	CPmessageHandler.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	environ.h \
	$(INET_PROJ)/src/networklayer/rsvp_te/IntServ_m.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	CPmessage_m.h \
	$(INET_PROJ)/src/networklayer/ipv4/RoutingTableAccess.h \
	$(INET_PROJ)/src/transport/contract/TCPCommand_m.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceTableAccess.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	cont.h \
	cpmsite.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	cpm.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	scpact.h \
	cpmcommon.h \
	$(INET_PROJ)/src/transport/contract/TCPSocketMap.h \
	agent.h \
	$(INET_PROJ)/src/networklayer/ipv4/IRoutingTable.h
$O/cont.o: cont.cc \
	CPmessage_m.h \
	SCPmessage_m.h \
	cpmcommon.h \
	cont.h
$O/cpmcommon.o: cpmcommon.cc \
	CPmessage_m.h \
	SCPmessage_m.h \
	cpmcommon.h
$O/SCPmessageHandler.o: SCPmessageHandler.cc \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h \
	$(INET_PROJ)/src/networklayer/rsvp_te/IntServ.h \
	$(INET_PROJ)/src/applications/tcpapp/TCPGenericCliAppBase.h \
	SCPmessage_m.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPRoute.h \
	$(INET_PROJ)/src/networklayer/rsvp_te/Utils.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/transport/contract/TCPSocket.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	CPmessageHandler.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	environ.h \
	$(INET_PROJ)/src/networklayer/rsvp_te/IntServ_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	CPmessage_m.h \
	$(INET_PROJ)/src/networklayer/ipv4/RoutingTableAccess.h \
	$(INET_PROJ)/src/transport/contract/TCPCommand_m.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceTableAccess.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	cont.h \
	SCPmessageHandler.h \
	cpmsite.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	cpm.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	cpmcommon.h \
	$(INET_PROJ)/src/transport/contract/TCPSocketMap.h \
	agent.h \
	$(INET_PROJ)/src/networklayer/ipv4/IRoutingTable.h
$O/feeder.o: feeder.cc \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h \
	$(INET_PROJ)/src/applications/tcpapp/TCPGenericCliAppBase.h \
	$(INET_PROJ)/src/networklayer/rsvp_te/IntServ.h \
	SCPmessage_m.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPRoute.h \
	$(INET_PROJ)/src/networklayer/rsvp_te/Utils.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/transport/contract/TCPSocket.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	CPmessageHandler.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	environ.h \
	$(INET_PROJ)/src/networklayer/rsvp_te/IntServ_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	CPmessage_m.h \
	$(INET_PROJ)/src/networklayer/ipv4/RoutingTableAccess.h \
	$(INET_PROJ)/src/transport/contract/TCPCommand_m.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceTableAccess.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	cont.h \
	cpmsite.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	cpm.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/transport/contract/TCPSocketMap.h \
	cpmcommon.h \
	feeder.h \
	agent.h \
	$(INET_PROJ)/src/networklayer/ipv4/IRoutingTable.h
$O/SCPmessage_m.o: SCPmessage_m.cc \
	SCPmessage_m.h
$O/sink.o: sink.cc \
	CPmessage_m.h \
	SCPmessage_m.h \
	cpmcommon.h \
	sink.h
$O/cpmsite.o: cpmsite.cc \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h \
	$(INET_PROJ)/src/networklayer/rsvp_te/IntServ.h \
	$(INET_PROJ)/src/applications/tcpapp/TCPGenericCliAppBase.h \
	SCPmessage_m.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPRoute.h \
	$(INET_PROJ)/src/networklayer/rsvp_te/Utils.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/transport/contract/TCPSocket.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	CPmessageHandler.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	environ.h \
	$(INET_PROJ)/src/networklayer/rsvp_te/IntServ_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	CPmessage_m.h \
	$(INET_PROJ)/src/networklayer/ipv4/RoutingTableAccess.h \
	$(INET_PROJ)/src/transport/contract/TCPCommand_m.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceTableAccess.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	cont.h \
	cpmsite.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	cpm.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	scpact.h \
	$(INET_PROJ)/src/transport/contract/TCPSocketMap.h \
	cpmcommon.h \
	agent.h \
	$(INET_PROJ)/src/networklayer/ipv4/IRoutingTable.h
$O/CPmessageHandler.o: CPmessageHandler.cc \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h \
	$(INET_PROJ)/src/networklayer/rsvp_te/IntServ.h \
	$(INET_PROJ)/src/applications/tcpapp/TCPGenericCliAppBase.h \
	SCPmessage_m.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPRoute.h \
	$(INET_PROJ)/src/networklayer/rsvp_te/Utils.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/transport/contract/TCPSocket.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	CPmessageHandler.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	environ.h \
	$(INET_PROJ)/src/networklayer/rsvp_te/IntServ_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	CPmessage_m.h \
	$(INET_PROJ)/src/networklayer/ipv4/RoutingTableAccess.h \
	$(INET_PROJ)/src/transport/contract/TCPCommand_m.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceTableAccess.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	cont.h \
	cpmsite.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	cpm.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	cpmcommon.h \
	$(INET_PROJ)/src/transport/contract/TCPSocketMap.h \
	agent.h \
	$(INET_PROJ)/src/networklayer/ipv4/IRoutingTable.h
$O/environ.o: environ.cc \
	CPmessage_m.h \
	cpmsite.h \
	SCPmessage_m.h \
	cpm.h \
	environ.h \
	cpmcommon.h \
	cont.h
$O/CPmessage_m.o: CPmessage_m.cc \
	CPmessage_m.h

