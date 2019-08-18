/* -*- mode:c++ -*- ********************************************************
 * file:        CSMAMacLayer.cc
 *
 ***************************************************************************
 * part of:     framework implementation developed by tkn
 ***************************************************************************/


#include "Mac802154.h"
#include "NicControlType.h"
#include "FWMath.h"
#include <cassert>

Define_Module( Mac802154 )

/**
 * Initialize the of the omnetpp.ini variables in stage 1. In stage
 * two subscribe to the RadioState.
 */
void Mac802154::initialize(int stage)
{
    BasicMacLayer::initialize(stage);

    if (stage == 0) {

        queueLength = hasPar("queueLength") ? par("queueLength") : 10;
        busyRSSI = hasPar("busyRSSI") ? par("busyRSSI") : -90;
        slotDuration = hasPar("slotDuration") ? par("slotDuration") : 0.1;
        difs = hasPar("difs") ? par("difs") : 0.001;
        maxTxAttempts = hasPar("maxTxAttempts") ? par("maxTxAttempts") : 7;
        bitrate = hasPar("bitrate") ? par("bitrate") : 10000;
        initialCW = hasPar("contentionWindow") ? par("contentionWindow") : 31;

        radioState = RadioState::RECV;
        rssi = 0;
        droppedPacket.setReason(DroppedPacket::NONE);
        nicId = getParentModule()->getId();
        RadioState cs;
        RSSI rs;

        catRadioState = bb->subscribe(this, &cs, getParentModule()->getId());
        catRSSI = bb->subscribe(this, &rs, getParentModule()->getId());

        catDroppedPacket = bb->getCategory(&droppedPacket);

        // get handle to radio
        radio = SingleChannelRadioAccess().get();

        // initialize the timer
        backoffTimer = new cMessage("backoff");
        minorMsg = new cMessage("minClear");
        macState = RX;

        txAttempts = 0;
    }
    else if(stage == 1) {
        int channel;
        myMacAddr = getParentModule()->getId();
        channel = hasPar("defaultChannel") ? par("defaultChannel") : 0;
        radio->setActiveChannel(channel);
        radio->setBitrate(bitrate);

        EV << "queueLength = " << queueLength
           << " busyRSSI = " << busyRSSI
           << " slotDuration = " << slotDuration
           << " difs = " << difs
           << " maxTxAttempts = " << maxTxAttempts
           << " bitrate = " << bitrate
           << " contentionWindow = " << initialCW << endl;

        busyRSSI = FWMath::dBm2mW(busyRSSI);
    }
}


void Mac802154::finish() {
    MacQueue::iterator it;
    if (!backoffTimer->isScheduled()) delete backoffTimer;
    if (!minorMsg->isScheduled()) delete minorMsg;
    for(it = macQueue.begin(); it != macQueue.end(); ++it) {
        delete (*it);
    }
    macQueue.clear();
}

/**
 * First it has to be checked whether a frame is currently being
 * transmitted or waiting to be transmitted. If so the newly arrived
 * message is stored in a queue. If there is no queue or it is full
 * print a warning.
 *
 * Before transmitting a frame it is tested whether the channel
 * is busy at the moment or not. If the channel is busy, a short
 * random time will be generated and the MacPkt is buffered for this
 * time, before a next attempt to send the packet is started.
 *
 * If channel is idle the frame will be transmitted immediately.
 */
void Mac802154::handleUpperMsg(cMessage *msg)
{
    assert(static_cast<cPacket*>(msg));
    MacPkt *mac = encapsMsg(static_cast<cPacket*>(msg));

    // message has to be queued if another message is waiting to be send
    // or if we are already trying to send another message

    if (macQueue.size() <= queueLength) {
        macQueue.push_back(mac);
	EV << "packet putt in queue\n  queue size:" << macQueue.size() << " macState:" << macState
	   << " (RX=" << RX << ") is scheduled:" << backoffTimer->isScheduled() << endl;;

        if((macQueue.size() == 1) && (macState == RX) && !backoffTimer->isScheduled()) {
            scheduleBackoff();
        }
    }
    else {
        // queue is full, message has to be deleted
        EV << "New packet arrived, but queue is FULL, so new packet is deleted\n";
        mac->setName("MAC ERROR");
        mac->setKind(NicControlType::PACKET_DROPPED);
        sendControlUp(mac);
        droppedPacket.setReason(DroppedPacket::QUEUE);
        bb->publishBBItem(catDroppedPacket, &droppedPacket, nicId);
    }
}

/**
 * After the timer expires try to retransmit the message by calling
 * handleUpperMsg again.
 */
void Mac802154::handleSelfMsg(cMessage *msg)
{
    if(msg == backoffTimer) {
        EV << "backoffTimer ";
        if(macState == RX) {
            EV << " RX ";
            if(macQueue.size() != 0) {
		macState = CCA;
                if(radioState == RadioState::RECV) {
		    EV << "rssi:" << rssi << " busyRSSI:" << busyRSSI << " ";
                    if(rssi < busyRSSI) {
                        EV << " idle ";
                        scheduleAt(simTime()+difs, minorMsg);
                    }
                    else{
			macState = RX;
                        EV << " busy ";
			scheduleBackoff();
                    }
                }
            }
        }
        else {
	    EV << "" << endl;
	    EV << "state=" << macState << "(TX="<<TX<<", CCA="<<CCA<<")\n";
            error("backoffTimer expired, MAC in wrong state");
        }
    }
    else if(msg == minorMsg) {
        EV << " minorMsg ";
        if((macState == CCA) && (radioState == RadioState::RECV)) {
	    EV << "rssi:" << rssi << " busyRSSI:" << busyRSSI << " ";
            if(rssi < busyRSSI) {
                EV << " idle -> to send ";
                macState = TX;
                radio->switchToSend();
            }
            else {
                EV << " busy -> backoff ";
                macState = RX;
		if(!backoffTimer->isScheduled())
		    scheduleBackoff();
            }
        }
        else {
            error("minClearTimer fired -- channel or mac in wrong state");
        }
    }
    EV << endl;
}


/**
 * Compare the address of this Host with the destination address in
 * frame. If they are equal or the frame is broadcast, we send this
 * frame to the upper layer. If not delete it.
 */
void Mac802154::handleLowerMsg(cMessage *msg)
{
    MacPkt *mac = static_cast<MacPkt *>(msg);
    int dest = mac->getDestAddr();

    if(dest == myMacAddr || dest == L2BROADCAST)
    {
        EV << "sending pkt to upper...\n";
        sendUp(decapsMsg(mac));
    }
    else {
        EV << "packet not for me, deleting...\n";
	delete mac;
    }

    if(!backoffTimer->isScheduled()) scheduleBackoff();
}

void Mac802154::handleLowerControl(cMessage *msg)
{
    if(msg->getKind() == NicControlType::TRANSMISSION_OVER) {
        EV << " transmission over" << endl;
        macState = RX;
        radio->switchToRecv();
        txAttempts = 0;
    }
    else {
        EV << "control message with wrong kind -- deleting\n";
    }
    delete msg;

    if(!backoffTimer->isScheduled()) scheduleBackoff();
}

void Mac802154::scheduleBackoff()
{
    if(backoffTimer->isScheduled()) {
        std::cerr << " is scheduled: MAC state "
                  << macState << " radio state : " << radioState << endl;
    }

    if(txAttempts > maxTxAttempts) {
        EV << " drop packet " << endl;
        cMessage *mac = macQueue.front();
        mac->setName("MAC ERROR");
        mac->setKind(NicControlType::PACKET_DROPPED);
        txAttempts = 0;
        macQueue.pop_front();
        sendControlUp(mac);
        droppedPacket.setReason(DroppedPacket::CHANNEL);
        bb->publishBBItem(catDroppedPacket, &droppedPacket, nicId);
    }
    if(macQueue.size() != 0) {
        EV << " schedule backoff " << endl;
        txAttempts++;
        EV << " " << txAttempts  << " " << endl;
        simtime_t time = intrand(initialCW - txAttempts)*slotDuration
            + 2.0*dblrand()*slotDuration;

	if(minorMsg->isScheduled()){
	    cancelEvent( minorMsg );
	    macState=RX;
	}

        scheduleAt(simTime() + time, backoffTimer);
    }

}

/**
 * Update the internal copies of interesting BB variables
 *
 */
void Mac802154::receiveBBItem(int category, const BBItem *details, int scopeModuleId)
{
    Enter_Method_Silent();
    BasicMacLayer::receiveBBItem(category, details, scopeModuleId);

    if(category == catRadioState) {
        radioState = static_cast<const RadioState *>(details)->getState();
        if((macState == TX) && (radioState == RadioState::SEND)) {
            EV << " radio in SEND state, sendDown packet" << endl;
            sendDown(macQueue.front());
            macQueue.pop_front();
        }
    }
    else if(category == catRSSI) {
        rssi = static_cast<const RSSI *>(details)->getRSSI();
        if(radioState == RadioState::RECV) {
            if(rssi < busyRSSI) {
                if(macState == CCA) {
                    if(!minorMsg->isScheduled()) scheduleAt(simTime()+difs, minorMsg);
                }
            }
            else {
                if(minorMsg->isScheduled()) cancelEvent(minorMsg);
                if(backoffTimer->isScheduled()) cancelEvent(backoffTimer);
                macState = RX;
            }
        }
    }
}


