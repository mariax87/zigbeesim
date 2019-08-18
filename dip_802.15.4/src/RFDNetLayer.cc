/***************************************************************************
 * file:        SimpleNetwLayer.cc
 *
 *

 ***************************************************************************
 * part of:     framework implementation developed by tkn
 * description: network layer: general class for the network layer
 *              subclass to create your own network layer
 ***************************************************************************/


#include "RFDNetLayer.h"
#include "NetwControlInfo.h"
#include "MacControlInfo.h"
#include <cassert>

Define_Module(RFDNetLayer);

void RFDNetLayer::initialize(int stage)
{
    BasicLayer::initialize(stage);

    if(stage==1){
        headerLength= par("headerLength");
        arp = ZigBeeArpAccess().get();
        myNetwAddr = this->getId();
        EV << " myNetwAddr " << myNetwAddr << endl;
    }
}

/**
 * Decapsulates the packet from the received Network packet
 **/
cMessage* RFDNetLayer::decapsMsg(NetwPkt *msg)
{
    cMessage *m = msg->decapsulate();
    m->setControlInfo(new NetwControlInfo(msg->getSrcAddr()));
    // delete the netw packet
    delete msg;
    return m;
}


/**
 * Encapsulates the received ApplPkt into a NetwPkt and set all needed
 * header fields.
 **/
NetwPkt* RFDNetLayer::encapsMsg(cMessage *msg) {
    int macAddr;
    int netwAddr;

    EV <<"in encaps...\n";

    NetwPkt *pkt = new NetwPkt(msg->getName(), msg->getKind());
    pkt->setBitLength(headerLength);

    NetwControlInfo* cInfo = dynamic_cast<NetwControlInfo*>(msg->removeControlInfo());

    if(cInfo == 0){
	EV << "warning: Application layer did not specifiy a destination L3 address\n"
	   << "\tusing broadcast address instead\n";
	netwAddr = L3BROADCAST;
    } else {
	EV <<"CInfo removed, netw addr="<< cInfo->getNetwAddr()<<endl;
        netwAddr = cInfo->getNetwAddr();
	delete cInfo;
    }

    pkt->setSrcAddr(myNetwAddr);
    pkt->setDestAddr(netwAddr);
    EV << " netw "<< myNetwAddr << " sending packet" <<endl;
    if(netwAddr == L3BROADCAST) {
        EV << "sendDown: nHop=L3BROADCAST -> message has to be broadcasted"
           << " -> set destMac=L2BROADCAST\n";
        macAddr = L2BROADCAST;
    }
    else{
        EV <<"sendDown: get the MAC address\n";
        macAddr = arp->getMacAddr(netwAddr);
    }

    pkt->setControlInfo(new MacControlInfo(macAddr));

    //encapsulate the application packet
    assert(static_cast<cPacket*>(msg));
    pkt->encapsulate(static_cast<cPacket*>(msg));
    EV <<" pkt encapsulated\n";
    return pkt;
}

/**
 * Redefine this function if you want to process messages from lower
 * layers before they are forwarded to upper layers
 *
 *
 * If you want to forward the message to upper layers please use
 * @ref sendUp which will take care of decapsulation and thelike
 **/
void RFDNetLayer::handleLowerMsg(cMessage* msg)
{

    NetwPkt *m = static_cast<NetwPkt *>(msg);
    EV << " handling packet from " << m->getSrcAddr() << endl;
    sendUp(decapsMsg(m));
}

/**
 * Redefine this function if you want to process messages from upper
 * layers before they are send to lower layers.
 *
 * For the SimpleNetwLayer we just use the destAddr of the network
 * message as a nextHop
 *
 * To forward the message to lower layers after processing it please
 * use @ref sendDown. It will take care of anything needed
 **/
void RFDNetLayer::handleUpperMsg(cMessage* msg)
{
    sendDown(encapsMsg(msg));
}

