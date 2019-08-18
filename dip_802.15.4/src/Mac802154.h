/* -*- mode:c++ -*- ********************************************************
 * file:        CSMAMacLayer.h
 *
 ***************************************************************************
 * part of:     framework implementation developed by tkn
 ***************************************************************************/


#ifndef MAC802154_H
#define MAC802154_H

#include <list>
#include <ActiveChannel.h>
#include <RadioState.h>
#include <RSSI.h>
#include <Bitrate.h>
#include <DroppedPacket.h>
#include <BasicMacLayer.h>
#include <Blackboard.h>
#include <MacPkt_m.h>
#include <SingleChannelRadio.h>
#include <SimpleAddress.h>


/**
 * @class CSMAMacLayer
 * @brief MAC module which provides non-persistent CSMA
 *
 * This is an implementation of a simple non-persistent CSMA. The
 * idea of nonpersistent CSMA is "listen before talk". Before
 * attempting to transmit, a station will sense the medium for a
 * carrier signal, which, if present, indicates that some other
 * station is sending.
 *
 * If the channel is busy a random waiting time is computed and after
 * this time the channel is sensed again. Once the channel gets idle
 * the message is sent. (State of the channel is obtained from SnrEval
 * via Blackboard.)
 *
 * An option of this module is to use a queue. If a packet from the
 * upper layer arrives although there is still another packet waiting
 * for its transmission or being transmitted the new packet can be
 * stored in this queue. The length of this queue can be specified by
 * the user in the omnetpp.ini file. By default the length is 0. If
 * the queue is full or there is no queue (length = 0) new packet(s)
 * will be deleted.
 *
 * @todo Inform upper layers about the full queue!
 *
 * ATTENTION: Imagine the following scenario:
 *
 * Several stations receive a broadcast request packet, usally exactly
 * at the same time. They will all try to answer at exactly the same
 * time, i.e. they all will sense the channel at exactly the same time
 * and start to transmit because the channel was sensed idle by all of
 * them. Therefore a small random delay should be built into one/some
 * of the upper layers to simulate a random processing time!
 *
 * The TestApplLayer e.g. implements such a processing delay!
 *
 * @ingroup macLayer
 * @author Marc L�bbers, Yosia Hadisusanto, Andreas Koepke
 */
class  Mac802154 : public BasicMacLayer
{
  public:


    /** @brief Initialization of the module and some variables*/
    virtual void initialize(int);

    /** @brief Delete all dynamically allocated objects of the module*/
    virtual void finish();

    /** @brief Handle messages from lower layer */
    virtual void handleLowerMsg(cMessage*);

    /** @brief Handle messages from upper layer */
    virtual void handleUpperMsg(cMessage*);

    /** @brief Handle self messages such as timers */
    virtual void handleSelfMsg(cMessage*);

    /** @brief Handle control messages from lower layer */
    virtual void handleLowerControl(cMessage *msg);

    /** @brief Called by the Blackboard whenever a change occurs we're interested in */
    virtual void receiveBBItem(int category, const BBItem *details, int scopeModuleId);

  protected:
    typedef std::list<MacPkt*> MacQueue;

    /** @brief MAC states
     *
     *  The MAC states help to keep track what the MAC is actually
     *  trying to do -- this is esp. useful when radio switching takes
     *  some time.
     *  RX  -- MAC accepts packets from PHY layer
     *  TX  -- MAC transmits a packet
     *  CCA -- Clear Channel Assessment - MAC checks
     *         whether medium is busy
     */

    enum States {
        RX,
        CCA,
        TX,
    };

    /** @brief kepp track of MAC state */
    States macState;

    /** @brief Current state of active channel (radio), set using radio, updated via BB */
    RadioState::States radioState;
    /** @brief category number given by bb for RadioState */
    int catRadioState;

    /** @brief Last RSSI level */
    double rssi;
    /** @brief category number given by bb for RSSI */
    int catRSSI;

    /** @brief RSSI level where medium is considered busy */
    double busyRSSI;

    /** @brief Duration of a slot
     *
     *  This duration can be a mini-slot (that is an RX-TX-turnaround
     *  time) but in general it should be the time it takes to send a
     *  packet plus some guard times (RX-TX-turnaround + a minimum
     *  inter-packet space).
     *
     */
    double slotDuration;

    /** @brief Maximum time between a packet and its ACK
     *
     * Usually this is slightly more then the tx-rx turnaround time
     * The channel should stay clear within this period of time.
     */
    double difs;

    /** @brief cached pointer to radio module */
    SingleChannelRadio* radio;

    /** @brief A queue to store packets from upper layer in case another
    packet is still waiting for transmission..*/
    MacQueue macQueue;

    /** @brief length of the queue*/
    unsigned queueLength;

    /** @brief Timer for backoff (non-persistent CSMA) */
    cMessage* backoffTimer;

    /** @brief Multi-purpose message.
     *
     * Keeps track of things like minimum time of clear channel
     * (important for atomic acks) and race condition avoidance.
     */
    cMessage* minorMsg;

    /** @brief count the number of tx attempts
     *
     * This holds the number of transmission attempts, since no Acks
     * are used this is the way how we can empty our queue even with
     * overloaded channels.
     */
    unsigned txAttempts;

     /** @brief maximum number of transmission attempts
     *
     * The packet is discared when this number is reached.
     */
    unsigned maxTxAttempts;

    /** @brief the bit rate at which we transmit */
    double bitrate;

    /** @brief initial contention window size */
    unsigned initialCW;

    /** @brief Inspect reasons for dropped packets */
    DroppedPacket droppedPacket;

    /** @brief plus category from BB */
    int catDroppedPacket;

    /** @brief publish dropped packets nic wide */
    int nicId;

protected:
    /** @brief schedule a backoff
     *
     *  A non-persistent CSMA ideally uses a fixed backoff window, but
     *  it could also increase this window in a linear fashion, or
     *  even exponential -- here, a linear increase is used. Overwrite
     *  this function to define another schedule function.  It also
     *  checks the retransmission counters -- if you overwrite it, do
     *  it with care.
     */
    virtual void scheduleBackoff();
};

#endif

