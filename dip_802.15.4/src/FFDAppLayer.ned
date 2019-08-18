//***************************************************************************
//***************************************************************************
//* part of:     framework implementation developed by tkn
//* description: application layer: general class for the application layer
//*              subclass to create your own application layer
//***************************************************************************
//***************************************************************************/


// Basic class for the application layer
// 
// This is the generic class for all application layer modules. If you
// want to implement your own application layer you have to subclass your
// module from this class.
//
// In this implementation all nodes randomly send broadcast packets to
// all connected neighbors. Every node who receives this packet will
// send a reply to the originator node.
//
// @author Daniel Willkomm
simple FFDAppLayer
{
    parameters:
        bool debug; // debug switch
        double headerLength; // length of the application message header (in bits)
    gates:
        output nldeSapOut;
        input nldeSapIn;
        
        output nlmeSapOut;
        input nlmeSapIn;
}
