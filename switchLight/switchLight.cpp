
//#include <iostream.h>
#include <stdio.h>
#include <windows.h>
#include <conio.h>

	// ADS headers
#include "C:\TwinCAT\AdsApi\TcAdsDll\Include\TcAdsDef.h"
#include "C:\TwinCAT\AdsApi\TcAdsDll\Include\TcAdsApi.h"
using namespace std;

void main()
{
	long     nErr, nPort;
	AmsAddr  Addr;
	PAmsAddr pAddr = &Addr;
	bool    dwData = false;
	bool    drData = false;
	bool    preDrData = false;

	// Open communication port on the ADS router
	nPort = AdsPortOpen();
	nErr = AdsGetLocalAddress(pAddr);
	if (nErr) {
		printf("Error: AdsGetLocalAddress: %d \n", nErr);
	}

    pAddr->netId.b[0] = 5;
	pAddr->netId.b[1] = 18;
	pAddr->netId.b[2] = 141;
	pAddr->netId.b[3] = 52;
	pAddr->netId.b[4] = 1;
	pAddr->netId.b[5] = 1;

	pAddr->port = 300;

	while (1) {
		nErr = AdsSyncReadReq(pAddr, 0x14002, 0x329, 0x1, &drData);
		if (nErr) {
			printf("Error: AdsSyncReadReq: %d \n", nErr);
			break;
		}
		if (true == drData) {
			nErr = AdsSyncReadReq(pAddr, 0x14002, 0x328, 0x1, &drData);
			if (nErr) {
				printf("Error: AdsSyncReadReq: %d \n", nErr);
				break;
			}
			if (true == preDrData && false == drData) {
				dwData = !dwData;
				nErr = AdsSyncWriteReq(pAddr, 0x13002, 0x1B8, 0x1, &dwData);
				if (nErr) {
					printf("Error: AdsSyncWriteReq: %d \n", nErr);
				    break;
				} 
			}
			preDrData = drData;
		}
		Sleep(100);
	}

	// Close communication port
	nErr = AdsPortClose();
	if (nErr) {
		printf("Error: AdsPortClose: %d \n", nErr);
	}
}