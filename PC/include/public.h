/*++

Copyright (c) Shaul Eizikovich.  All rights reserved.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

Module Name:

    public.h
    
Abstract:

    Public header file for the vJoy project
	Developpers that need to interface with vJoy need to include this file

Author:


Environment:

    kernel mode and User mode

Notes:


Revision History:


--*/
#ifndef _PUBLIC_H
#define _PUBLIC_H

// Compilation directives
#define PPJOY_MODE
#undef PPJOY_MODE	// Comment-out for compatibility mode

#ifdef PPJOY_MODE
#include "PPJIoctl.h"
#endif

#include <INITGUID.H>	// Definitions for controlling GUID initialization

//
// Usage example:
//		CreateFile(TEXT("\\\\.\\vJoy"), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
#ifdef PPJOY_MODE
#define DEVICENAME_STRING			"PPJoyIOCTL1"
#else
#define DEVICENAME_STRING			"vJoy"
#endif
#define NTDEVICE_NAME_STRING		"\\Device\\"DEVICENAME_STRING
#define SYMBOLIC_NAME_STRING		"\\DosDevices\\"DEVICENAME_STRING
#define	DOS_FILE_NAME				"\\\\.\\"DEVICENAME_STRING
#define VJOY_INTERFACE				L"Device_"

// Version parts
#define VER_X_	0
#define VER_H_	2
#define VER_M_	0
#define VER_L_	5

#define STRINGIFY_1(x)   #x
#define STRINGIFY(x)     STRINGIFY_1(x)
#define PASTE(x, y) x##y
#define MAKEWIDE(x) PASTE(L,x)

// Device Attributes
//
#define VENDOR_N_ID		0x1234
#define	PRODUCT_N_ID	0xBEAD
#define	VERSION_N	(VER_L_ + 0x10*VER_M_ + 0x100*VER_H_ + 0x1000*VER_X_)

// Device Strings
//
#define VENDOR_STR_ID		L"Shaul Eizikovich"
#define PRODUCT_STR_ID		L"vJoy - Virtual Joystick"
#define	SERIALNUMBER_STR	MAKEWIDE(STRINGIFY(VER_H_)) L"." MAKEWIDE(STRINGIFY(VER_M_)) L"."  MAKEWIDE(STRINGIFY(VER_L_))

// Function codes;
//#define LOAD_POSITIONS	0x910
//#define GETATTRIB		0x911
// #define GET_FFB_DATA	0x00222912	// METHOD_OUT_DIRECT + FILE_DEVICE_UNKNOWN	+ FILE_ANY_ACCESS
//#define SET_FFB_STAT	0x913	 // METHOD_NEITHER
//#define GET_FFB_STAT	0x916

#define F_LOAD_POSITIONS	0x910
#define F_GETATTRIB			0x911
#define F_GET_FFB_DATA		0x912
#define F_SET_FFB_STAT		0x913
#define F_GET_FFB_STAT		0x916
#define F_GET_DEV_INFO      0x917
// IO Device Control codes;
#define IOCTL_VJOY_GET_ATTRIB		CTL_CODE (FILE_DEVICE_UNKNOWN, GETATTRIB, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define LOAD_POSITIONS	            CTL_CODE (FILE_DEVICE_UNKNOWN, F_LOAD_POSITIONS, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define GET_FFB_DATA	            CTL_CODE (FILE_DEVICE_UNKNOWN, F_GET_FFB_DATA, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define SET_FFB_STAT	            CTL_CODE (FILE_DEVICE_UNKNOWN, F_SET_FFB_STAT, METHOD_NEITHER, FILE_ANY_ACCESS)
#define GET_FFB_STAT	            CTL_CODE (FILE_DEVICE_UNKNOWN, F_GET_FFB_STAT, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define GET_DEV_INFO			    CTL_CODE (FILE_DEVICE_UNKNOWN, F_GET_DEV_INFO, METHOD_BUFFERED, FILE_ANY_ACCESS)

#ifndef __HIDPORT_H__
// Copied from hidport.h
#define IOCTL_HID_SET_FEATURE	0xB0191
#define IOCTL_HID_WRITE_REPORT	0xB000F

#define MAX_N_DEVICES	16 // Maximum number of vJoy devices


typedef struct _HID_DEVICE_ATTRIBUTES {

    ULONG           Size;
    //
    // sizeof (struct _HID_DEVICE_ATTRIBUTES)
    //

    //
    // Vendor ids of this hid device
    //
    USHORT          VendorID;
    USHORT          ProductID;
    USHORT          VersionNumber;
    USHORT          Reserved[11];

} HID_DEVICE_ATTRIBUTES, * PHID_DEVICE_ATTRIBUTES;
#endif

// Error levels for status report
enum ERRLEVEL {INFO, WARN, ERR, FATAL, APP};
// Status report function prototype
#ifdef WINAPI
typedef BOOL (WINAPI *StatusMessageFunc)(void * output, TCHAR * buffer, enum ERRLEVEL level);
#endif

///////////////////////////////////////////////////////////////

/////////////////////// Joystick Position ///////////////////////
//
// This structure holds data that is passed to the device from
// an external application such as SmartPropoPlus.
//
// Usage example:
//	JOYSTICK_POSITION iReport;
//	:
//	DeviceIoControl (hDevice, 100, &iReport, sizeof(HID_INPUT_REPORT), NULL, 0, &bytes, NULL)
typedef struct _JOYSTICK_POSITION
{
	BYTE	bDevice;	// Index of device. 1-based.
	LONG	wThrottle;
	LONG	wRudder;
	LONG	wAileron;
	LONG	wAxisX;
	LONG	wAxisY;
	LONG	wAxisZ;
	LONG	wAxisXRot;
	LONG	wAxisYRot;
	LONG	wAxisZRot;
	LONG	wSlider;
	LONG	wDial;
	LONG	wWheel;
	LONG	wAxisVX;
	LONG	wAxisVY;
	LONG	wAxisVZ;
	LONG	wAxisVBRX;
	LONG	wAxisVBRY;
	LONG	wAxisVBRZ;
	LONG	lButtons;	// 32 buttons: 0x00000001 means button1 is pressed, 0x80000000 -> button32 is pressed
	DWORD	bHats;		// Lower 4 bits: HAT switch or 16-bit of continuous HAT switch
	DWORD	bHatsEx1;	// Lower 4 bits: HAT switch or 16-bit of continuous HAT switch
	DWORD	bHatsEx2;	// Lower 4 bits: HAT switch or 16-bit of continuous HAT switch
	DWORD	bHatsEx3;	// Lower 4 bits: HAT switch or 16-bit of continuous HAT switch
} JOYSTICK_POSITION, *PJOYSTICK_POSITION;

// Superset of JOYSTICK_POSITION
// Extension of JOYSTICK_POSITION with Buttons 33-128 appended to the end of the structure.
typedef struct _JOYSTICK_POSITION_V2
{
	/// JOYSTICK_POSITION
	BYTE	bDevice;	// Index of device. 1-based.
	LONG	wThrottle;
	LONG	wRudder;
	LONG	wAileron;
	LONG	wAxisX;
	LONG	wAxisY;
	LONG	wAxisZ;
	LONG	wAxisXRot;
	LONG	wAxisYRot;
	LONG	wAxisZRot;
	LONG	wSlider;
	LONG	wDial;
	LONG	wWheel;
	LONG	wAxisVX;
	LONG	wAxisVY;
	LONG	wAxisVZ;
	LONG	wAxisVBRX;
	LONG	wAxisVBRY;
	LONG	wAxisVBRZ;
	LONG	lButtons;	// 32 buttons: 0x00000001 means button1 is pressed, 0x80000000 -> button32 is pressed
	DWORD	bHats;		// Lower 4 bits: HAT switch or 16-bit of continuous HAT switch
	DWORD	bHatsEx1;	// Lower 4 bits: HAT switch or 16-bit of continuous HAT switch
	DWORD	bHatsEx2;	// Lower 4 bits: HAT switch or 16-bit of continuous HAT switch
	DWORD	bHatsEx3;	// Lower 4 bits: HAT switch or 16-bit of continuous HAT switch LONG lButtonsEx1; // Buttons 33-64
	
	/// JOYSTICK_POSITION_V2 Extenssion
	LONG lButtonsEx1; // Buttons 33-64
	LONG lButtonsEx2; // Buttons 65-96
	LONG lButtonsEx3; // Buttons 97-128
} JOYSTICK_POSITION_V2, *PJOYSTICK_POSITION_V2;


// HID Descriptor definitions
#define HID_USAGE_X		0x30
#define HID_USAGE_Y		0x31
#define HID_USAGE_Z		0x32
#define HID_USAGE_RX	0x33
#define HID_USAGE_RY	0x34
#define HID_USAGE_RZ	0x35
#define HID_USAGE_SL0	0x36
#define HID_USAGE_SL1	0x37
#define HID_USAGE_WHL	0x38
#define HID_USAGE_POV	0x39


#endif


