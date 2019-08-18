//***************************************************************************
// * file:        NicCsma.ned
// *
// ***************************************************************************
// * part of:     framework implementation developed by tkn
// **************************************************************************/

import org.mf2.contrib.nic.radios.SingleChannelRadio;

//
// The CsmaNic contains a csma-like MAC layer.
// The BasicDecider is used all messages will be delivered to
// the MAC and no packets get lost
//
// This nic is intended to be used in order to test the implementation
// of the csma protocol.
//
// IMPORTANT:<br> In order to work with the ChannelControl module the snrEval
// module has to be called "snrEval" in the ned file.
//
// @see BasicLayer, BasicDecider, BasicSnrEval
// @author Marc Loebbers
//

module Nic802154
{
    gates:
        input mcpsSapIn;
        output mcpsSapOut;
        
        input mlmeSapIn;
        output mlmeSapOut;

        input radioIn; // to receive AirFrames
        
    submodules:
        llc: Llc8022;
        sscs: Sscs;
        mac: Mac802154 {
            parameters:
                @display("i=block/layer");
        }
        phy: Phy802154;
		decider: SnrDecider802154;
        snrEval: SnrEval802154 {
            parameters:
                @display("i=block/wrxtx");
        }
        radio: SingleChannelRadio;
    connections:
        mlmeSapIn --> mac.mlmeSapIn;
        mlmeSapOut <-- mac.mlmeSapOut;
        
        mcpsSapIn --> llc.mcpsSapIn;
        mcpsSapOut <-- llc.mcpsSapOut;
        
        llc.sscsSapOut --> sscs.llcSapIn;
        llc.sscsSapIn <-- sscs.llcSapOut;
        
        sscs.mcpsSapOut --> mac.mcpsSapIn;
        sscs.mcpsSapIn <-- mac.mcpsSapOut;
        
        mac.pdSapOut --> phy.pdSapIn;
        mac.pdSapIn <-- phy.pdSapOut;
        
        mac.plmeSapOut --> phy.plmeSapIn;
        mac.plmeSapIn <-- phy.plmeSapOut;
        
        phy.rfSapOut --> snrEval.uppergateIn;
        phy.rfSapIn <-- decider.uppergateOut;
        snrEval.uppergateOut --> decider.lowergateIn;
        snrEval.upperControlOut --> phy.rfControlIn;
        
        radioIn --> snrEval.radioIn;
}
