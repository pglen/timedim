
/* =====[ usbdrive.cpp ]========================================== 
                                                                             
   Description:     The timedim project, implementation of the usbdrive.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  8/29/2007  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#include "stdafx.h"
#include "timedim.h"

#include <io.h>
#include <direct.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

#include <stdio.h>
#include <dbt.h>						// For DeviceChange.

#include "usbdrive.h"



//////////////////////////////////////////////////////////////////////////
//
// Function name:    GetDisksProperty
// Description:      <empty description>
// Return type:      BOOL 
// Argument:         HANDLE hDevice
// Argument:         PSTORAGE_DEVICE_DESCRIPTOR pDevDesc
//
//////////////////////////////////////////////////////////////////////////

BOOL GetDisksProperty(HANDLE hDevice, PSTORAGE_DEVICE_DESCRIPTOR pDevDesc)

{
	STORAGE_PROPERTY_QUERY	Query;	// input param for query
	DWORD dwOutBytes;				// IOCTL output length
	BOOL bResult;					// IOCTL return val

	// specify the query type
	Query.PropertyId = StorageDeviceProperty;
	Query.QueryType = PropertyStandardQuery;

	// Query using IOCTL_STORAGE_QUERY_PROPERTY 
	bResult = ::DeviceIoControl(hDevice,			// device handle
			IOCTL_STORAGE_QUERY_PROPERTY,			// info of device property
			&Query, sizeof(STORAGE_PROPERTY_QUERY),	// input data buffer
			pDevDesc, pDevDesc->Size,				// output data buffer
			&dwOutBytes,							// out's length
			(LPOVERLAPPED)NULL);					

	return bResult;
}

//////////////////////////////////////////////////////////////////////////
//
// Function name:    getbustype
// Description:      Return the bus type of DRIVE drv
// Return type:      STORAGE_BUS_TYPE 
// Argument:         const char* drv
//
// Enums as follows:
//
//  BusTypeUnknown = 0x00,
//    BusTypeScsi,
//    BusTypeAtapi,
//    BusTypeAta,
//    BusType1394,
//    BusTypeSsa,
//    BusTypeFibre,
//    BusTypeUsb,
//    BusTypeRAID,
//    BusTypeMaxReserved = 0x7F
//
//////////////////////////////////////////////////////////////////////////

STORAGE_BUS_TYPE GetBustype(const char* drv)

{
	STORAGE_BUS_TYPE bus = BusTypeUnknown;
	HANDLE			hDevice;
	
	PSTORAGE_DEVICE_DESCRIPTOR pDevDesc;

	CString devname;
	devname.Format("\\\\?\\%c:", *drv);

	hDevice = CreateFile(devname, GENERIC_READ,
				FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);

	if (hDevice != INVALID_HANDLE_VALUE)
		{
		// Allocate mem for bus type query
		pDevDesc = (PSTORAGE_DEVICE_DESCRIPTOR)new BYTE[sizeof(STORAGE_DEVICE_DESCRIPTOR) + 512 - 1];
		pDevDesc->Size = sizeof(STORAGE_DEVICE_DESCRIPTOR) + 512 - 1;

		if(GetDisksProperty(hDevice, pDevDesc))
			{
			bus = pDevDesc->BusType;
			}
		delete pDevDesc;
		CloseHandle(hDevice);
		}

	//P2N("Got bus type of %s -> bus=%d\r\n", devname, bus);

	return bus;
}		

