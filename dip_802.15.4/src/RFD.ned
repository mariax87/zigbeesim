

// Immediately sends out any message it receives. It can optionally generate
// a message at the beginning of the simulation, to bootstrap the process.
//
import org.mf2.core.blackboard.Blackboard;
import org.mf2.contrib.mobility.ConstSpeedMobility;

module RFD
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
        //mobility: ConstSpeedMobility {
        //    parameters:
        //        //@display("p=200,150;b=30,25");
        //}
        app: RFDAppLayer {
            parameters:
                //@display("p=90,50;b=100,20,rect");
        }
        net: RFDNetLayer {
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
        app.netOut --> net.appIn;
        app.netIn <-- net.appOut;
        
        net.nicOut --> nic.netIn;
        net.nicIn <-- nic.netOut;
        
        radioIn --> nic.radioIn;      
}

