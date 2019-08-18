//***************************************************************************
//* file:        CSMAMacLayer.ned
//*

//***************************************************************************
//* part of:     framework implementation developed by tkn
//* description: MAC layer class for the CSMAMac
//**************************************************************************/


//
// 802.15.4 MAC protocol
//
module Mac802154
{
    gates:
        input mcpsSapIn;
        output mcpsSapOut;

        input mlmeSapIn;
        output mlmeSapOut;

        input pdSapIn;
        output pdSapOut;

        input plmeSapIn;
        output plmeSapOut;
    submodules:
        mcps: Mcps;
        macPib: MacPib;
        mlme: Mlme;
    connections:
        // external connections
        mcpsSapIn --> mcps.mcpsSapIn;
        mcpsSapOut <-- mcps.mcpsSapOut;

        mlmeSapIn --> mlme.mlmeSapIn;
        mlmeSapOut <-- mlme.mlmeSapOut;

        pdSapIn --> mcps.pdSapIn;
        pdSapOut <-- mcps.pdSapOut;

        plmeSapIn --> mlme.plmeSapIn;
        plmeSapOut <-- mlme.plmeSapOut;

        // internal connections
        mcps.mlmeOut --> mlme.mcpsIn;
        mcps.mlmeIn <-- mlme.mcpsOut;

		mlme.macPibOut --> macPib.mlmeIn;
		mlme.macPibIn <-- macPib.mlmeOut;
}
