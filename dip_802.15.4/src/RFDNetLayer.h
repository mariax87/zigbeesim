/* -*- mode:c++ -*- ********************************************************
 * file:        SimpleNetwLayer.h
 ***************************************************************************
 * part of:     framework implementation developed by tkn
 * description: network layer: general class for the network layer
 *              subclass to create your own network layer
 **************************************************************************/


#ifndef RFD_NET_LAYER_H
#define RFD_NET_LAYER_H

#include <omnetpp.h>

#include <BasicLayer.h>

#include "ZigBeeArp.h"
#include "NetwPkt_m.h"
#include "SimpleAddress.h"

/**
 * @brief Simple class for the network layer
 *
 * @ingroup netwLayer
 * @author Daniel Willkomm
 **/
class RFDNetLayer : public BasicLayer
{

  protected:
    /**
     * @brief Length of the NetwPkt header
     * Read from omnetpp.ini
     **/
    int headerLength;

    /** @brief Pointer to the arp module*/
    ZigBeeArp* arp;

    /** @brief cached variable of my networ address */
    int myNetwAddr;

public:


    /** @brief Initialization of the module and some variables*/
    virtual void initialize(int);

  protected:
    /**
     * @name Handle Messages
     * @brief Functions to redefine by the programmer
     *
     * These are the functions provided to add own functionality to your
     * modules. These functions are called whenever a self message or a
     * data message from the upper or lower layer arrives respectively.
     *
     **/
    /*@{*/

    /** @brief Handle messages from upper layer */
    virtual void handleUpperMsg(cMessage* msg);

    /** @brief Handle messages from lower layer */
    virtual void handleLowerMsg(cMessage* msg);

    /** @brief Handle self messages */
    virtual void handleSelfMsg(cMessage* msg){ delete msg;};

    /** @brief Handle control messages from lower layer */
    virtual void handleLowerControl(cMessage* msg) {
        EV << "deleting control message from lower layer\n";
        delete msg;
    }

    /*@}*/

    /** @brief decapsulate higher layer message from NetwPkt */
    virtual cMessage* decapsMsg(NetwPkt*);

    /** @brief Encapsulate higher layer packet into an NetwPkt*/
    virtual NetwPkt* encapsMsg(cMessage*);
};

#endif
