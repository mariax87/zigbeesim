/* -*- mode:c++ -*- ********************************************************
 * file:        BasicApplLayer.h
 *
***************************************************************************
 * part of:     framework implementation developed by tkn
 * description: application layer: general class for the application layer
 *              subclass to create your own application layer
 **************************************************************************/


#ifndef RFD_APP_LAYER_H
#define RFD_APP_LAYER_H

#include <omnetpp.h>

#include "BasicModule.h"
#include "ApplPkt_m.h"



lass RFDAppLayer : public BasicModule
{
  protected:
    /** @brief gate id*/
    /*@{*/
    int lowergateIn;
    int lowerControlIn;
    int lowergateOut;
    /*@}*/

    /**
     * @brief Length of the ApplPkt header
     *
     * @todo for now it is read from omnetpp.ini but should be easily
     * settable by the user later
     **/
    int headerLength;

  public:


    /** @brief Initialization of the module and some variables*/
    virtual void initialize(int);

    /** @brief Called every time a message arrives*/
    void handleMessage(cMessage*);

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

    /**
     * @brief Handle self messages such as timer...
     *
     * Define this function if you want to process timer or other kinds
     * of self messages
     **/
    virtual void handleSelfMsg(cMessage* msg){
	EV << "BasicAppl: handleSelfMsg not redefined; delete msg\n";
	delete msg;
    };

    /**
     * @brief Handle messages from lower layer
     *
     * Redefine this function if you want to process messages from lower
     * layers.
     *
     * The basic application layer just silently deletes all messages it
     * receives.
     **/
    virtual void handleLowerMsg(cMessage* msg){
	EV << "BasicAppl: handleLowerMsg not redefined; delete msg\n";
	delete msg;
    };

    /**
     * @brief Handle control messages from lower layer
     *
     * The basic application layer just silently deletes all messages it
     * receives.
     **/
    virtual void handleLowerControl(cMessage* msg){
	EV << "BasicAppl: handleLowerControl not redefined; delete msg\n";
	delete msg;
    };

    /*@}*/

    /**
     * @name Convenience Functions
     * @brief Functions for convenience - NOT to be modified
     *
     * These are functions taking care of message encapsulation and
     * message sending. Normally you should not need to alter these but
     * should use them to handle message encasulation and sending. They
     * will wirte all necessary information into packet headers and add
     * or strip the appropriate headers for each layer.
     *
     **/
    /*@{*/

    /** @brief Sends a message to the lower layer*/
    void sendDown(cMessage *);

    /** @brief Sends a message to the lower layer; waits delay seconds
	before sending*/
    void sendDelayedDown(cMessage *, simtime_t delay);

    /*@}*/


    /**
     * @brief Return my application layer address
     *
     * We use the network module index as application address
     **/
    virtual const int myApplAddr(){
	return getParentModule()->getIndex();
    };

};

#endif
