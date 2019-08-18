/* -*- mode:c++ -*- ********************************************************
 * file:        SimpleArp.h
 *
 ***************************************************************************
 * part of:     framework implementation developed by tkn
 ***************************************************************************/

#ifndef ZIGBEEARP_H
#define ZIGBEEARP_H

#include <omnetpp.h>
#include <BasicModule.h>
#include <ModuleAccess.h>

/**
 * @brief A class to convert integer layer addresses
 *
 * This class takes the network layer id as the network layer
 * address and the mac layer id as the mac layer address
 *
 * ARP implementations are divers, it does not make sense to start
 * from a common class. Their main purpose is to translate layer 2
 * into layer 3 addresses. However, these addresses can be very
 * different. Here, simple integers are sufficient, but for
 * hardware-in-the-loop simulations more complex ones are appropriate.
 *
 * In contrast to this ARP, others may want to cache entries. This
 * adds another set of basic operations that may or may not make sense
 * for ARPs.
  **/
class ZigBeeArp : public BasicModule
{


public:

    /** @brief should not be called,
     *  instead direct calls to the radio methods should be used.
     */
    virtual void handleMessage( cMessage* ){
        error("ARP module cannot receive messages!");
    };

    /** @brief returns a L2 address to a given L3 address*/
    int getMacAddr(const int netwAddr);

    /** @brief returns a L3 address to a given L2 address*/
    int getNetwAddr(const int macAddr);
};

class  ZigBeeArpAccess : public ModuleAccess<ZigBeeArp>
{
  public:
    ZigBeeArpAccess() : ModuleAccess<ZigBeeArp>("arp") {}
};


#endif
