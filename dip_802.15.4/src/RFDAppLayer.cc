/* -*- mode:c++ -*- *******************************************************
 * file:        BasicApplLayer.cc

 ***************************************************************************
 * part of:     framework implementation developed by tkn
 * description: application layer: general class for the application layer
 *              subclass to create your own application layer
 ***************************************************************************/


#include "RFDAppLayer.h"


Define_Module(RFDAppLayer);


/**
 * First we have to initialize the module from which we derived ours,
 * in this case BasicModule.
 *
 * Then we have to intialize the gates and - if necessary - some own
 * variables.
 **/
void RFDAppLayer::initialize(int stage)
{
    BasicModule::initialize(stage);

    if(stage==0){
	headerLength= par("headerLength");
	lowergateOut=findGate ("lowergateOut");
	lowergateIn=findGate ("lowergateIn");
        lowerControlIn  = findGate("lowerControlIn");
    }
}

/**
 * The basic handle message function.
 *
 * Depending on the gate a message arrives handleMessage just calls
 * different handle message functions to further process the message.
 *
 * You should not make any changes in this function but implement all
 * your functionality into the handle*Msg functions called from here.
 *
 * @sa handleLowerMsg, handleSelfMsg
 **/
void RFDAppLayer::handleMessage(cMessage *msg)
{
    if(msg->getArrivalGateId()==lowergateIn){
        handleLowerMsg(msg);
    }
    else if(msg->getArrivalGateId()==lowerControlIn){
        EV << "handle lower control" << endl;
        handleLowerControl(msg);
    }
    else {
        handleSelfMsg(msg);
    }
}

/**
 * Send message down to lower layer
 **/
void RFDAppLayer::sendDown(cMessage *msg) {
    sendDelayedDown(msg, 0.0);
}

/**
 * Send message down to lower layer after delay seconds
 **/
void RFDAppLayer::sendDelayedDown(cMessage *msg, simtime_t delay) {
    sendDelayed(msg, delay, lowergateOut);
}
