

//
// Immediately sends out any message it receives. It can optionally generate
// a message at the beginning of the simulation, to bootstrap the process.
//

import org.mf2.core.blackboard.Blackboard;
import org.mf2.contrib.mobility.ConstSpeedMobility;

module FFD
{
    parameters:
        @display("i=device/accesspoint");
    gates:
        input radioIn;
    submodules:
        blackboard: Blackboard {
            parameters:
                //@display("p=200,75;b=30,25");
        }
        mobility: ConstSpeedMobility {
            parameters:
                //@display("p=200,150;b=30,25");
        }
        app: FFDAppLayer {
            parameters:
                //@display("p=90,50;b=100,20,rect");
        }
        net: FFDNetLayer {
            parameters:
                //@display("p=90,100;b=100,20,rect");
        }
        arp: ZigBeeArp {
            parameters:
        }
        nic: Nic802154 {
            parameters:
                //@display("p=90,150;b=100,20,rect");
        }

    connections:
        app.nldeSapOut --> net.nldeSapIn;
        app.nldeSapIn <-- net.nldeSapOut;

        app.nlmeSapOut --> net.nlmeSapIn;
        app.nlmeSapIn <-- net.nlmeSapOut;

        net.mcpsSapOut --> nic.mcpsSapIn;
        net.mcpsSapIn <-- nic.mcpsSapOut;

        net.mlmeSapOut --> nic.mlmeSapIn;
        net.mlmeSapIn <-- nic.mlmeSapOut;

        radioIn --> nic.radioIn;
}

