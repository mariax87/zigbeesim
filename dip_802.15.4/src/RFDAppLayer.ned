//***************************************************************************
//* file:        BasicApplLayer.ned
//*
//*              This program is free software; you can redistribute it 
//*              and/or modify it under the terms of the GNU General Public 
//*              License as published by the Free Software Foundation; either
//*              version 2 of the License, or (at your option) any later 
//*              version.
//*              For further information see file COPYING 
//*              in the top level directory
//***************************************************************************
//* part of:     framework implementation developed by tkn
//* description: application layer: general class for the application layer
//*              subclass to create your own application layer
//***************************************************************************


// Basic class for the application layer
// 
// This is the generic class for all application layer modules. If you
// want to implement your own application layer you have to subclass your
// module from this class.
//
// In this implementation all nodes randomly send broadcast packets to
// all connected neighbors. Every node who receives this packet will
// send a reply to the originator node.

simple RFDAppLayer
{
    parameters:
        bool debug; // debug switch
        double headerLength; // length of the application message header (in bits)
    gates:
        output netOut;
        input netIn;
}
