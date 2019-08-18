/* -*- mode:c++ -*- ********************************************************
 * file:        BBItems.h
 ***************************************************************************
 * part of:     ZigBee Simulation model
 * description: - Blackboard objects containing radio state values
 ***************************************************************************/

#ifndef BBITEMS_H_
#define BBITEMS_H_

#include <omnetpp.h>
#include <Blackboard.h>
#include <sstream>

/**
 * @brief Hold current Bitrate of a radio, as double in bit per second
 *
 * @ingroup utils
 * @ingroup blackboard
 * @author Andreas K�pke
 * @sa Blackboard
 */
/** @brief Theese are not needed at the moment, check this at the end of the project */
class PhyCurrentChannel: public BBItem {
BBITEM_METAINFO(BBItem);
protected:
	/** @brief current channel id */
	int channel;
public:
	/** @brief Get current channel */
	int getChannel() const {return channel;}
	/** @brief Set channel */
	void setChannel(int ch) {channel = ch;}
	/** @brief Constructor*/
	PhyCurrentChannel(int ch=0) : BBItem(), channel(ch) {};
	/** @brief Enables inspection */
	std::string info() const {
		std::ostringstream ost;
		ost << " Current channel is " << channel;
		return ost.str();
	}
};

class ScanDuration: public BBItem {
BBITEM_METAINFO(BBItem);
protected:
	/** @brief current channel id */
	unsigned char duration;
public:
	/** @brief Get current duration */
	unsigned char getDuration() const {return duration;}
	/** @brief Set duration */
	void setDuration(unsigned char d) {duration = d;}
	/** @brief Constructor*/
	ScanDuration(unsigned char d=0) : BBItem(), duration(d) {};
	/** @brief Enables inspection */
	std::string info() const {
		std::ostringstream ost;
		ost << " Scan duration is " << std::hex << duration << std::dec;
		return ost.str();
	}
};
#endif
