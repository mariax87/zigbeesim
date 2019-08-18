/* -*- mode:c++ -*- ********************************************************
 * file:        FFDAppLayer.h
 ***************************************************************************
 * part of:     ZigBee Simulation model
 * description: - Module describing the behavior of the application layer
 ***************************************************************************/

#ifndef FFD_APP_LAYER_H
#define FFD_APP_LAYER_H

#include <omnetpp.h>

#include "BasicApplLayer.h"
#include "typedef.h"
#include "Messages_m.h"
#include <string>

/**
 * @brief Basic class for the application layer
 *
 * This is the generic class for all application layer modules. If you
 * want to implement your own application layer you have to subclass your
 * module from this class.
 **/
class FFDAppLayer: public BasicApplLayer {
public:

	/** @brief Initialization of the module and some variables*/
	virtual void initialize(int);

	/** @brief Called every time a message arrives*/
	void handleMessage(cMessage*);

protected:
	/** @brief gate id*/
	/*@{*/
	int nldeSapIn;
	int nldeSapOut;
	int nlmeSapIn;
	int nlmeSapOut;
	/*@}*/

	/**
	 * @brief Length of the ApplPkt header
	 *
	 * @todo for now it is read from omnetpp.ini but should be easily
	 * settable by the user later
	 **/
	int headerLength;

	/** @brief Sets the level of comments to the EV output */
	CommentsLevel commentsLevel;
	/** @todo add some comment on this one */
	Role role;

	/**
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
	void handleSelfMsg(cMessage *);

	/**
	 * @brief Handle messages from lower layer
	 *
	 * Redefine this function if you want to process messages from lower
	 * layers.
	 *
	 * The basic application layer just silently deletes all messages it
	 * receives.
	 **/
	virtual void handleNldeMsg(cMessage *);

	/**
	 * @brief Handle control messages from lower layer
	 *
	 * The basic application layer just silently deletes all messages it
	 * receives.
	 **/
	virtual void handleNlmeMsg(cMessage *);

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
	void sendNldeDown(cMessage *);

	void sendNlmeDown(cMessage *);

	/**
	 * @brief Return my application layer address
	 *
	 * We use the network module index as application address
	 **/
	virtual const int myApplAddr() {
		return getParentModule()->getIndex();
	}

	virtual void comment(CommentsLevel level, std::string s) {
		/** @todo add logname substrings for routers and endDevices */
		if ((level & commentsLevel) > COMMENT_NOTHING) {
			std::cout << logName() << "." << getName() << "\t" << s << endl;
		}
	}

	virtual void commentMsgSending(cMessage *msg) {
		std::stringstream commentStream;
		commentStream << "Sending " << msg->getName();
		comment(COMMENT_MESSAGE, commentStream.str());
	}

	virtual void commentMsgReceived(cMessage *msg) {
		std::stringstream commentStream;
		commentStream << "Received " << msg->getName();
		comment(COMMENT_MESSAGE, commentStream.str());
	}
public:
	Role getRole() {
		return this->role;
	}
	void setRole(Role role) {
		this->role = role;
	}
};

#endif
