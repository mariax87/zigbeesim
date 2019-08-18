
//
// Two instances (tic and toc) of Txc connected.
//
import org.mf2.core.channelControl.ChannelControl;

network ZigBee
{
    parameters:
        // parametes for the framework
        double playgroundSizeX;
        double playgroundSizeY;
        double numRouters;
        double numEndDevices;

        @display("bgb=$playgroundSizeX,$playgroundSizeY,white,,;bgp=0,0");
    submodules:
        channelcontrol: ChannelControl {
            parameters:
                playgroundSizeX = playgroundSizeX;
                playgroundSizeY = playgroundSizeY;
                //@display("p=50,25;i=eye14");
        }
        
        coordinatorHost: FFD {
            parameters:            
        }
        routerHost[numRouters]: FFD {
            parameters:            
        }
        endDeviceHost[numEndDevices]: RFD {
            parameters:            
        }

    connections allowunconnected:
    // all connections and gates are to be generated dynamically

}

