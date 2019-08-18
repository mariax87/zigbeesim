//***************************************************************************
// * file:        RFDNetLayer.ned
// ***************************************************************************
// * part of:     framework implementation developed by tkn
// * description: network layer: general class for the network layer
// *              subclass to create your own network layer
// **************************************************************************/


// Basic class for the network layer
// 
// This is the generic class for all network layer modules. If you
// want to implement your own network layer you have to subclass your
// module from this class.
//
// @author Daniel Willkomm
module FFDNetLayer
{
    gates:
        input nldeSapIn;
        output nldeSapOut;
        
        input nlmeSapIn;
        output nlmeSapOut;
        
        input mcpsSapIn;
        output mcpsSapOut;
        
        input mlmeSapIn;
        output mlmeSapOut;
    submodules:
        nwkPib: NwkPib;
        nlme: Nlme;
        nlde: Nlde;
    connections:
        // external connections
        nldeSapIn --> nlde.nldeSapIn;
        nldeSapOut <-- nlde.nldeSapOut;
        
        nlmeSapIn --> nlme.nlmeSapIn;
        nlmeSapOut <-- nlme.nlmeSapOut;
        
        mcpsSapIn --> nlde.mcpsSapIn;
        mcpsSapOut <-- nlde.mcpsSapOut;
        
        mlmeSapIn --> nlme.mlmeSapIn;
        mlmeSapOut <-- nlme.mlmeSapOut;
        
        //internal connections
        nlde.nlmeOut --> nlme.nldeIn;
        nlde.nlmeIn <-- nlme.nldeOut;
        
        nlme.nwkPibOut --> nwkPib.nlmeIn;
        nlme.nwkPibIn <-- nwkPib.nlmeOut;
}
