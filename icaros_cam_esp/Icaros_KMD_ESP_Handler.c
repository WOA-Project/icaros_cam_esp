/*++

Module Name:

	handler.c

Abstract:

	This file contains the driver entry points and callbacks.

Environment:

	Kernel-mode Driver Framework

--*/

#include "handler.h"
#include "Debug.h"
#include "Public.h"
#include "Device.h"
#include "Internal.h"

#ifdef ALLOC_PRAGMA
#pragma alloc_text (PAGE, IcarosESPHandler_GetCurrentOrientationData)
#pragma alloc_text (PAGE, IcarosESPHandler_PowerOffEvent)
#pragma alloc_text (PAGE, IcarosESPHandler_PowerOnEvent)
#endif



//----- (00402698) --------------------------------------------------------
int IcarosESPHandler_UnregisterOrientationDataMediator(void* pIn, size_t nInLen, void* pOut, size_t nOutLen, int* pnOutLenReq)
{
	UNREFERENCED_PARAMETER((pIn, nInLen, pOut, nOutLen, pnOutLenReq));
	return -1073741637;
}


//----- (00409000) --------------------------------------------------------
int IcarosESPHandler_GetCurrentOrientationData(void* pIn, size_t nInLen, void* pOut, size_t nOutLen, int* pnOutLenReq)
{
	int v8; // [sp+8h] [bp-30h]
	int v9; // [sp+10h] [bp-28h]

	EspDbgPrintlEx(
		9,
		"ESP KMD",
		"File: %s, Line: %d, Function: %s",
		"Icaros_KMD_ESP_Handler.c",
		396,
		"IcarosESPHandler_GetCurrentOrientationData");
	CameraSensorOrientation_CameraHeartbeat();
	if (pIn)
	{
		if (nInLen == 4)
		{
			if (pOut)
			{
				if (nOutLen == 32)
				{
					if (pnOutLenReq)
					{
						v9 = IcarosESPHandler_SetTemperatureData(*(ULONG*)pIn);
						if (v9 < 0)
							EspDbgPrintlEx(0, "ESP KMD", "IcarosESPHandler_SetTemperatureData() Failed. 0x%x", v9);
						CameraSensorOrientation_GetOrientationData((OrientationInputData*)pOut);
						*pnOutLenReq = 32;
						EspDbgPrintlEx(
							9,
							"ESP KMD",
							"File: %s, Line: %d, Function: %s",
							"Icaros_KMD_ESP_Handler.c",
							438,
							"IcarosESPHandler_GetCurrentOrientationData");
						v8 = 0;
					}
					else
					{
						EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: pnOutLenReq != NULL");
						v8 = -1073741811;
					}
				}
				else
				{
					EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: nOutLen == sizeof(*data)");
					v8 = -1073741811;
				}
			}
			else
			{
				EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: pOut != NULL");
				v8 = -1073741811;
			}
		}
		else
		{
			EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: nInLen == sizeof(*dataIn)");
			v8 = -1073741811;
		}
	}
	else
	{
		EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: pIn != NULL");
		v8 = -1073741811;
	}
	return v8;
}
// 4090A6: variable 'v5' is possibly undefined
// 4090C8: variable 'v6' is possibly undefined

//----- (00409124) --------------------------------------------------------
int IcarosESPHandler_PowerOffEvent()
{
	int v1; // [sp+8h] [bp-20h]
	PFDO_DATA v2; // [sp+14h] [bp-14h]

	EspDbgPrintlEx(
		9,
		"ESP KMD",
		"File: %s, Line: %d, Function: %s",
		"Icaros_KMD_ESP_Handler.c",
		340,
		"IcarosESPHandler_PowerOffEvent");
	g_PoweredCamera = 0;
	if (g_PoweredCameraEvent)
	{
		EspDbgPrintlEx(0, "ESP KMD", "ESP:KeSetEvent 0x%x", g_PoweredCameraEvent);
		KeSetEvent(g_PoweredCameraEvent, 0, 0);
	}
	CameraSensorOrientation_CameraHeartbeat();
	CameraSensorOrientation_SetCameraPower(0);
	CameraSensorOrientation_SetAccelerometerDataRequested(0);
	CameraSensorOrientation_SetGyroscopeDataRequested(0);
	CameraSensorOrientation_SendUpdateNotification();
	if (CameraESPEnsureThermalIoTargetIsReady() >= 0)
	{
		v2 = GetDeviceExtension(m_device);
		v1 = IcarosESPHanlder_SendIOCTL(v2->Interfaces[0].IoTarget, 0x222408u, 0, 0, 0, 0);
		if (v1 < 0)
			EspDbgPrintlEx(0, "ESP KMD", "IcarosESPHanlder_SendIOCTL() IOCTL_CAM_ESP_TZ_CAMERA_POWER_ON: Failed. 0x%x", v1);
	}
	EspDbgPrintlEx(
		9,
		"ESP KMD",
		"File: %s, Line: %d, Function: %s",
		"Icaros_KMD_ESP_Handler.c",
		374,
		"IcarosESPHandler_PowerOffEvent");
	return 0;
}
// 405038: using guessed type int (*KeSetEvent)(ULONG, ULONG, ULONG);

//----- (00409220) --------------------------------------------------------
int IcarosESPHandler_PowerOnEvent()
{
	PFDO_DATA v1; // [sp+8h] [bp-20h]
	int v2; // [sp+Ch] [bp-1Ch]

	EspDbgPrintlEx(
		9,
		"ESP KMD",
		"File: %s, Line: %d, Function: %s",
		"Icaros_KMD_ESP_Handler.c",
		301,
		"IcarosESPHandler_PowerOnEvent");
	CameraSensorOrientation_CameraHeartbeat();
	CameraSensorOrientation_SetCameraPower(1u);
	CameraSensorOrientation_SetAccelerometerDataRequested(1u);
	CameraSensorOrientation_SetGyroscopeDataRequested(1u);
	CameraSensorOrientation_SendUpdateNotification();
	v1 = GetDeviceExtension(m_device);
	v1->ioctlRequestRetryCounter = 3;
	if (CameraESPEnsureThermalIoTargetIsReady() >= 0)
	{
		v2 = IcarosESPHanlder_SendIOCTL(v1->Interfaces[0].IoTarget, 0x222404u, 0, 0, 0, 0);
		if (v2 < 0)
			EspDbgPrintlEx(0, "ESP KMD", "IcarosESPHanlder_SendIOCTL() IOCTL_CAM_ESP_TZ_CAMERA_POWER_ON: Failed. 0x%x", v2);
	}
	EspDbgPrintlEx(
		9,
		"ESP KMD",
		"File: %s, Line: %d, Function: %s",
		"Icaros_KMD_ESP_Handler.c",
		330,
		"IcarosESPHandler_PowerOnEvent");
	return 0;
}


//----- (004026B8) --------------------------------------------------------
int IcarosESPHanlder_SendIOCTL(WDFIOTARGET IoTarget, unsigned int IOCTLCode, WDFMEMORY InputBuffer, WDFMEMORY_OFFSET* InputBufferOffset, WDFMEMORY OutputBuffer, WDFMEMORY_OFFSET* OutputBufferOffset)
{
	int v7; // [sp+14h] [bp-44h]
	int v8; // [sp+14h] [bp-44h]
	PFDO_DATA v9; // [sp+18h] [bp-40h]
	int v10; // [sp+1Ch] [bp-3Ch]
	WDF_REQUEST_REUSE_PARAMS Params; // [sp+30h] [bp-28h] BYREF
	WDFIOTARGET varg_r0; // [sp+48h] [bp-10h]
	unsigned int varg_r1; // [sp+4Ch] [bp-Ch]
	WDFMEMORY varg_r2; // [sp+50h] [bp-8h]
	WDFMEMORY_OFFSET* varg_r3; // [sp+54h] [bp-4h]

	varg_r0 = IoTarget;
	varg_r1 = IOCTLCode;
	varg_r2 = InputBuffer;
	varg_r3 = InputBufferOffset;
	EspDbgPrintlEx(
		9,
		"ESP KMD",
		"File: %s, Line: %d, Function: %s",
		"Icaros_KMD_ESP_Handler.c",
		581,
		"IcarosESPHanlder_SendIOCTL");
	v9 = GetDeviceExtension(m_device);
	WDF_REQUEST_REUSE_PARAMS_INIT(&Params, 0, 0);
	v7 = WdfRequestReuse(v9->ioctlRequest, &Params);
	if (v7 >= 0)
	{
		v8 = WdfIoTargetFormatRequestForIoctl(
			varg_r0,
			v9->ioctlRequest,
			varg_r1,
			varg_r2,
			varg_r3,
			OutputBuffer,
			OutputBufferOffset);
		if (v8 >= 0)
		{
			if ((unsigned __int8)WdfRequestSend(v9->ioctlRequest, varg_r0, 0))
			{
				EspDbgPrintlEx(
					9,
					"ESP KMD",
					"File: %s, Line: %d, Function: %s",
					"Icaros_KMD_ESP_Handler.c",
					628,
					"IcarosESPHanlder_SendIOCTL");
			}
			else
			{
				v8 = WdfRequestGetStatus(v9->ioctlRequest);
				EspDbgPrintlEx(0, "ESP KMD", "WdfRequestSend() IOCTL code %lu: Failed. 0x%x", varg_r1, v8);
			}
			v10 = v8;
		}
		else
		{
			EspDbgPrintlEx(
				0,
				"ESP KMD",
				"WdfIoTargetFormatRequestForIoctl() IOCTL_CAM_ESP_TZ_SET_TEMPERATURE: Failed. 0x%x",
				v8);
			v10 = v8;
		}
	}
	else
	{
		EspDbgPrintlEx(0, "ESP KMD", "WdfRequestReuse() Failed. 0x%x", v7);
		v10 = v7;
	}
	return v10;
}


//----- (00401A30) --------------------------------------------------------
int IcarosESPHandler_DeRegPoweredCameraIdEvent(void* pIn, size_t nInLen, void* pOut, size_t nOutLen, int* pnOutLenReq)
{
	int v6; // [sp+8h] [bp-28h]
	int v7; // [sp+14h] [bp-1Ch]

	v7 = 0;
	EspDbgPrintlEx(
		9,
		"ESP KMD",
		"File: %s, Line: %d, Function: %s",
		"Icaros_KMD_ESP_Handler.c",
		744,
		"IcarosESPHandler_DeRegPoweredCameraIdEvent");
	if (pIn)
	{
		if (nInLen == 4)
		{
			if (pOut)
			{
				EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: pOut == NULL");
				v6 = -1073741811;
			}
			else if (nOutLen)
			{
				EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: nOutLen == 0");
				v6 = -1073741811;
			}
			else if (pnOutLenReq)
			{
				EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: pnOutLenReq == NULL");
				v6 = -1073741811;
			}
			else
			{
				if (g_PoweredCameraEvent == *(KEVENT**)pIn)
				{
					EspDbgPrintlEx(0, "ESP KMD", "ESP:g_PoweredCameraEvent 0x%x", g_PoweredCameraEvent);
					g_PoweredCameraEvent = 0;
				}
				else
				{
					v7 = -1073741823;
				}
				EspDbgPrintlEx(
					9,
					"ESP KMD",
					"File: %s, Line: %d, Function: %s",
					"Icaros_KMD_ESP_Handler.c",
					768,
					"IcarosESPHandler_DeRegPoweredCameraIdEvent");
				v6 = v7;
			}
		}
		else
		{
			EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: nInLen == sizeof(dataIn)");
			v6 = -1073741811;
		}
	}
	else
	{
		EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: pIn != NULL");
		v6 = -1073741811;
	}
	return v6;
}

//----- (00401B54) --------------------------------------------------------
int IcarosESPHandler_GetCameraPowerState(void* pIn, size_t nInLen, void* pOut, size_t nOutLen, int* pnOutLenReq)
{
	int v6; // [sp+Ch] [bp-24h]

	EspDbgPrintlEx(
		9,
		"ESP KMD",
		"File: %s, Line: %d, Function: %s",
		"Icaros_KMD_ESP_Handler.c",
		642,
		"IcarosESPHandler_GetCameraPowerState");
	if (pIn)
	{
		EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: pIn == NULL");
		v6 = -1073741811;
	}
	else if (nInLen)
	{
		EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: nInLen == 0");
		v6 = -1073741811;
	}
	else if (pOut)
	{
		if (nOutLen == 4)
		{
			if (pnOutLenReq)
			{
				*(ULONG*)pOut = (unsigned __int8)CameraSensorOrientation_GetCameraPower();
				*pnOutLenReq = 4;
				EspDbgPrintlEx(
					9,
					"ESP KMD",
					"File: %s, Line: %d, Function: %s",
					"Icaros_KMD_ESP_Handler.c",
					658,
					"IcarosESPHandler_GetCameraPowerState");
				v6 = 0;
			}
			else
			{
				EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: pnOutLenReq != NULL");
				v6 = -1073741811;
			}
		}
		else
		{
			EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: nOutLen == sizeof(*data)");
			v6 = -1073741811;
		}
	}
	else
	{
		EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: pOut != NULL");
		v6 = -1073741811;
	}
	return v6;
}

//----- (00401C50) --------------------------------------------------------
int IcarosESPHandler_GetCurrentRequestFlags(void* pIn, size_t nInLen, void* pOut, size_t nOutLen, int* pnOutLenReq)
{
	int v6; // [sp+8h] [bp-28h]

	EspDbgPrintlEx(
		9,
		"ESP KMD",
		"File: %s, Line: %d, Function: %s",
		"Icaros_KMD_ESP_Handler.c",
		91,
		"IcarosESPHandler_GetCurrentRequestFlags");
	if (pIn)
	{
		EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: pIn == NULL");
		v6 = -1073741811;
	}
	else if (nInLen)
	{
		EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: nInLen == 0");
		v6 = -1073741811;
	}
	else if (pOut)
	{
		if (nOutLen >= 4)
		{
			if (pnOutLenReq)
			{
				EspDbgPrintlEx(0, "ESP KMD", "IcarosESPHandler_GetCurrentRequestFlags: pOut = %x", pOut);
				EspDbgPrintlEx(0, "ESP KMD", "IcarosESPHandler_GetCurrentRequestFlags: *outputValue = %d", *(ULONG*)pOut);
				*(ULONG*)pOut = CameraSensorOrientation_GetRequestFlags();
				*pnOutLenReq = 4;
				EspDbgPrintlEx(
					9,
					"ESP KMD",
					"File: %s, Line: %d, Function: %s",
					"Icaros_KMD_ESP_Handler.c",
					111,
					"IcarosESPHandler_GetCurrentRequestFlags");
				v6 = 0;
			}
			else
			{
				EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: pnOutLenReq != NULL");
				v6 = -1073741811;
			}
		}
		else
		{
			EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: nOutLen >= sizeof (*outputValue)");
			v6 = -1073741811;
		}
	}
	else
	{
		EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: pOut != NULL");
		v6 = -1073741811;
	}
	return v6;
}

//----- (00401D64) --------------------------------------------------------
int IcarosESPHandler_GetFocusStatsData(void* pIn, size_t nInLen, void* pOut, size_t nOutLen, int* pnOutLenReq)
{
	int v6; // [sp+0h] [bp-20h]
	unsigned int pSizeData; // [sp+4h] [bp-1Ch] BYREF
	void* varg_r0; // [sp+10h] [bp-10h]
	size_t varg_r1; // [sp+14h] [bp-Ch]
	unsigned __int8* varg_r2; // [sp+18h] [bp-8h]
	size_t varg_r3; // [sp+1Ch] [bp-4h]

	varg_r0 = pIn;
	varg_r1 = nInLen;
	varg_r2 = (unsigned __int8*)pOut;
	varg_r3 = nOutLen;
	pSizeData = 0;
	FocusStats_GetStatsDataSize(&pSizeData);
	EspDbgPrintlEx(0, "ESP KMD", "IcarosESPHandler_GetFocusStatsData >>>");
	if (varg_r0)
	{
		EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: pIn == NULL");
		v6 = -1073741811;
	}
	else if (varg_r1)
	{
		EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: nInLen == 0");
		v6 = -1073741811;
	}
	else if (varg_r2)
	{
		if (varg_r3 >= pSizeData)
		{
			if (pnOutLenReq)
			{
				FocusStats_RetrieveFocusData(varg_r2, pnOutLenReq);
				EspDbgPrintlEx(0, "ESP KMD", "IcarosESPHandler_GetFocusStatsData <<<");
				v6 = 0;
			}
			else
			{
				EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: pnOutLenReq != NULL");
				v6 = -1073741811;
			}
		}
		else
		{
			EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: nOutLen >= requiredStatsSize");
			v6 = -1073741811;
		}
	}
	else
	{
		EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: pOut != NULL");
		v6 = -1073741811;
	}
	return v6;
}

//----- (00401E38) --------------------------------------------------------
int IcarosESPHandler_GetFocusStatsSize(void* pIn, size_t nInLen, void* pOut, size_t nOutLen, int* pnOutLenReq)
{
	int v6; // [sp+0h] [bp-20h]

	EspDbgPrintlEx(0, "ESP KMD", "IcarosESPHandler_GetFocusStatsSize >>>");
	if (pIn)
	{
		EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: pIn == NULL");
		v6 = -1073741811;
	}
	else if (nInLen)
	{
		EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: nInLen == 0");
		v6 = -1073741811;
	}
	else if (pOut)
	{
		if (nOutLen == 4)
		{
			if (pnOutLenReq)
			{
				FocusStats_GetStatsDataSize((unsigned int*)pOut);
				*pnOutLenReq = 4;
				EspDbgPrintlEx(0, "ESP KMD", "IcarosESPHandler_GetFocusStatsSize <<<");
				v6 = 0;
			}
			else
			{
				EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: pnOutLenReq != NULL");
				v6 = -1073741811;
			}
		}
		else
		{
			EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: nOutLen == sizeof(UINT32)");
			v6 = -1073741811;
		}
	}
	else
	{
		EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: pOut != NULL");
		v6 = -1073741811;
	}
	return v6;
}

//----- (00401F04) --------------------------------------------------------
int IcarosESPHandler_GetPoweredCameraId(void* pIn, size_t nInLen, void* pOut, size_t nOutLen, int* pnOutLenReq)
{
	int v6; // [sp+8h] [bp-20h]

	EspDbgPrintlEx(
		9,
		"ESP KMD",
		"File: %s, Line: %d, Function: %s",
		"Icaros_KMD_ESP_Handler.c",
		672,
		"IcarosESPHandler_GetPoweredCameraId");
	if (pIn)
	{
		EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: pIn == NULL");
		v6 = -1073741811;
	}
	else if (nInLen)
	{
		EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: nInLen == 0");
		v6 = -1073741811;
	}
	else if (pOut)
	{
		if (nOutLen == 4)
		{
			if (pnOutLenReq)
			{
				*(ULONG*)pOut = g_PoweredCamera;
				*pnOutLenReq = 4;
				EspDbgPrintlEx(
					9,
					"ESP KMD",
					"File: %s, Line: %d, Function: %s",
					"Icaros_KMD_ESP_Handler.c",
					688,
					"IcarosESPHandler_GetPoweredCameraId");
				v6 = 0;
			}
			else
			{
				EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: pnOutLenReq != NULL");
				v6 = -1073741811;
			}
		}
		else
		{
			EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: nOutLen == sizeof(*data)");
			v6 = -1073741811;
		}
	}
	else
	{
		EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: pOut != NULL");
		v6 = -1073741811;
	}
	return v6;
}

//----- (00401FFC) --------------------------------------------------------
int IcarosESPHandler_PowerOnEventExt(void* pIn, size_t nInLen)
{
	int v3; // [sp+8h] [bp-20h]
	int v4; // [sp+Ch] [bp-1Ch]
	int v5; // [sp+10h] [bp-18h]

	if (pIn)
	{
		if (nInLen)
		{
			v3 = *(ULONG*)pIn;
			EspDbgPrintlEx(
				9,
				"ESP KMD",
				"File: %s, Line: %d, Function: %s",
				"Icaros_KMD_ESP_Handler.c",
				273,
				"IcarosESPHandler_PowerOnEventExt");
			v5 = IcarosESPHandler_PowerOnEvent();
			if (nInLen == 4 && v5 >= 0 && g_PoweredCamera != v3)
			{
				g_PoweredCamera = v3;
				if (g_PoweredCameraEvent)
				{
					EspDbgPrintlEx(0, "ESP KMD", "ESP:KeSetEvent 0x%x", g_PoweredCameraEvent);
					KeSetEvent(g_PoweredCameraEvent, 0, 0);
				}
				EspDbgPrintlEx(0, "ESP KMD", "ESP:s_PoweredCamera %d", g_PoweredCamera);
			}
			EspDbgPrintlEx(
				9,
				"ESP KMD",
				"File: %s, Line: %d, Function: %s",
				"Icaros_KMD_ESP_Handler.c",
				292,
				"IcarosESPHandler_PowerOnEventExt");
			v4 = v5;
		}
		else
		{
			EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: nInLen != 0");
			v4 = -1073741811;
		}
	}
	else
	{
		EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: pIn != NULL");
		v4 = -1073741811;
	}
	return v4;
}
// 405038: using guessed type int (*KeSetEvent)(ULONG, ULONG, ULONG);

//----- (00402100) --------------------------------------------------------
int IcarosESPHandler_RegPoweredCameraIdEvent(void* pIn, size_t nInLen, void* pOut, size_t nOutLen, int* pnOutLenReq)
{
	int v6; // [sp+8h] [bp-28h]
	KEVENT* v7; // [sp+10h] [bp-20h]
	int v8; // [sp+14h] [bp-1Ch]

	v8 = 0;
	EspDbgPrintlEx(
		9,
		"ESP KMD",
		"File: %s, Line: %d, Function: %s",
		"Icaros_KMD_ESP_Handler.c",
		704,
		"IcarosESPHandler_RegPoweredCameraIdEvent");
	if (pIn)
	{
		if (nInLen == 4)
		{
			if (pOut)
			{
				EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: pOut == NULL");
				v6 = -1073741811;
			}
			else if (nOutLen)
			{
				EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: nOutLen == 0");
				v6 = -1073741811;
			}
			else if (pnOutLenReq)
			{
				EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: pnOutLenReq == NULL");
				v6 = -1073741811;
			}
			else
			{
				v7 = *(KEVENT**)pIn;
				if (g_PoweredCameraEvent)
				{
					v8 = -1073741823;
				}
				else
				{
					g_PoweredCameraEvent = *(KEVENT**)pIn;
					EspDbgPrintlEx(0, "ESP KMD", "ESP:g_PoweredCameraEvent 0x%x", v7);
				}
				EspDbgPrintlEx(
					9,
					"ESP KMD",
					"File: %s, Line: %d, Function: %s",
					"Icaros_KMD_ESP_Handler.c",
					728,
					"IcarosESPHandler_RegPoweredCameraIdEvent");
				v6 = v8;
			}
		}
		else
		{
			EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: nInLen == sizeof(dataIn)");
			v6 = -1073741811;
		}
	}
	else
	{
		EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: pIn != NULL");
		v6 = -1073741811;
	}
	return v6;
}

//----- (00402220) --------------------------------------------------------
int IcarosESPHandler_RegisterOrientationDataMediator(void* pIn, size_t nInLen, void* pOut, size_t nOutLen, int* pnOutLenReq)
{
	int v6; // [sp+8h] [bp-20h]

	EspDbgPrintlEx(
		9,
		"ESP KMD",
		"File: %s, Line: %d, Function: %s",
		"Icaros_KMD_ESP_Handler.c",
		48,
		"IcarosESPHandler_RegisterOrientationDataMediator");
	if (pIn)
	{
		EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: pIn == NULL");
		v6 = -1073741811;
	}
	else if (nInLen)
	{
		EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: nInLen == 0");
		v6 = -1073741811;
	}
	else if (pOut)
	{
		EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: pOut == NULL");
		v6 = -1073741811;
	}
	else if (nOutLen)
	{
		EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: nOutLen == 0");
		v6 = -1073741811;
	}
	else if (pnOutLenReq)
	{
		EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: pnOutLenReq == NULL");
		v6 = -1073741811;
	}
	else
	{
		CameraSensorOrientation_ObtainNotificationEvent();
		EspDbgPrintlEx(
			9,
			"ESP KMD",
			"File: %s, Line: %d, Function: %s",
			"Icaros_KMD_ESP_Handler.c",
			59,
			"IcarosESPHandler_RegisterOrientationDataMediator");
		v6 = 0;
	}
	return v6;
}

//----- (004022FC) --------------------------------------------------------
int IcarosESPHandler_SetAutoFocusStats(void* pIn, size_t nInLen, void* pOut, size_t nOutLen, int* pnOutLenReq)
{
	int v6; // [sp+0h] [bp-20h]

	UNREFERENCED_PARAMETER(pnOutLenReq);
	UNREFERENCED_PARAMETER(nOutLen);
	UNREFERENCED_PARAMETER(pOut);

	EspDbgPrintlEx(0, "ESP KMD", "IcarosESPHandler_SetAutoFocusStats >>>");
	if (pIn)
	{
		if (nInLen)
		{
			FocusStats_SetStatsData((unsigned int)nInLen, (unsigned __int8*)pIn);
			EspDbgPrintlEx(0, "ESP KMD", "IcarosESPHandler_SetAutoFocusStats <<<");
			v6 = 0;
		}
		else
		{
			EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: nInLen > 0");
			v6 = -1073741811;
		}
	}
	else
	{
		EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: pIn != NULL");
		v6 = -1073741811;
	}
	return v6;
}

//----- (00402378) --------------------------------------------------------
int IcarosESPHandler_SetCurrentOrientationData(void* pIn, size_t nInLen, void* pOut, size_t nOutLen, int* pnOutLenReq)
{
	int v7; // [sp+Ch] [bp-24h]

	EspDbgPrintlEx(
		9,
		"ESP KMD",
		"File: %s, Line: %d, Function: %s",
		"Icaros_KMD_ESP_Handler.c",
		125,
		"IcarosESPHandler_SetCurrentOrientationData");
	CameraSensorOrientation_MediatorServiceEvent();
	if (!CameraSensorOrientation_IsCameraAlive())
		IcarosESPHandler_PowerOffEvent();
	if (pIn)
	{
		if (nInLen == 32)
		{
			if (pOut)
			{
				EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: pOut == NULL");
				v7 = -1073741811;
			}
			else if (nOutLen)
			{
				EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: nOutLen == 0");
				v7 = -1073741811;
			}
			else if (pnOutLenReq)
			{
				EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: pnOutLenReq == NULL");
				v7 = -1073741811;
			}
			else
			{
				CameraSensorOrientation_SetOrientationData((const OrientationInputData*)pIn);
				EspDbgPrintlEx(
					9,
					"ESP KMD",
					"File: %s, Line: %d, Function: %s",
					"Icaros_KMD_ESP_Handler.c",
					179,
					"IcarosESPHandler_SetCurrentOrientationData");
				v7 = 0;
			}
		}
		else
		{
			EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: nInLen == sizeof(*data)");
			v7 = -1073741811;
		}
	}
	else
	{
		EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: pIn != NULL");
		v7 = -1073741811;
	}
	return v7;
}
// 402426: variable 'v5' is possibly undefined

//----- (00402478) --------------------------------------------------------
int IcarosESPHandler_SetTemperatureData(unsigned int temperature)
{
	int v3; // [sp+8h] [bp-30h]
	int v4; // [sp+8h] [bp-30h]
	int v5; // [sp+Ch] [bp-2Ch]
	WDFMEMORY Memory; // [sp+10h] [bp-28h] BYREF
	PFDO_DATA v7; // [sp+14h] [bp-24h]
	int v8; // [sp+18h] [bp-20h]
	int v9; // [sp+1Ch] [bp-1Ch]
	PFDO_DATA v10; // [sp+20h] [bp-18h]
	int v11; // [sp+24h] [bp-14h]
	ULONG Buffer[2]; // [sp+30h] [bp-8h] BYREF

	Buffer[0] = temperature;
	EspDbgPrintlEx(
		9,
		"ESP KMD",
		"File: %s, Line: %d, Function: %s",
		"Icaros_KMD_ESP_Handler.c",
		519,
		"IcarosESPHandler_SetTemperatureData");
	v7 = GetDeviceExtension(m_device);
	v10 = v7;
	v8 = CameraESPEnsureThermalIoTargetIsReady();
	if (v8 < 0)
		return 0;
	v9 = WdfMemoryCreatePreallocated(0, Buffer, 4u, &Memory);
	v3 = v9;
	if (v9 >= 0)
	{
		v11 = IcarosESPHanlder_SendIOCTL(v10->Interfaces[0].IoTarget, 0x222400u, Memory, 0, 0, 0);
		v4 = v11;
		if (v11 < 0)
			EspDbgPrintlEx(0, "ESP KMD", "IcarosESPHanlder_SendIOCTL() IOCTL_CAM_ESP_TZ_SET_TEMPERATURE: Failed. 0x%x", v11);
		WdfObjectDelete(Memory);
		EspDbgPrintlEx(
			9,
			"ESP KMD",
			"File: %s, Line: %d, Function: %s",
			"Icaros_KMD_ESP_Handler.c",
			560,
			"IcarosESPHandler_SetTemperatureData");
		v5 = v4;
	}
	else
	{
		EspDbgPrintlEx(0, "ESP KMD", "WdfMemoryCreatePreallocated() Failed. 0x%x", v9);
		v5 = v3;
	}
	return v5;
}

//----- (00402568) --------------------------------------------------------
int IcarosESPHandler_SyncPerFrameData(void* pIn, size_t nInLen, void* pOut, size_t nOutLen, int* pnOutLenReq)
{
	int v8; // [sp+8h] [bp-30h]
	int v9; // [sp+14h] [bp-24h]

	EspDbgPrintlEx(
		9,
		"ESP KMD",
		"File: %s, Line: %d, Function: %s",
		"Icaros_KMD_ESP_Handler.c",
		460,
		"IcarosESPHandler_SyncPerFrameData");
	CameraSensorOrientation_CameraHeartbeat();
	if (pIn)
	{
		if (nInLen == 4)
		{
			if (pOut)
			{
				if (nOutLen == 36)
				{
					if (pnOutLenReq)
					{
						v9 = IcarosESPHandler_SetTemperatureData(*(ULONG*)pIn);
						if (v9 < 0)
							EspDbgPrintlEx(0, "ESP KMD", "IcarosESPHandler_SetTemperatureData() Failed. 0x%x", v9);
						CameraSensorOrientation_GetOrientationData((OrientationInputData*)pOut);
						CameraSensorOrientation_GetThermalCooling((unsigned int*)pOut + 8);
						*pnOutLenReq = 36;
						EspDbgPrintlEx(
							9,
							"ESP KMD",
							"File: %s, Line: %d, Function: %s",
							"Icaros_KMD_ESP_Handler.c",
							505,
							"IcarosESPHandler_SyncPerFrameData");
						v8 = 0;
					}
					else
					{
						EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: pnOutLenReq != NULL");
						v8 = -1073741811;
					}
				}
				else
				{
					EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: nOutLen == sizeof(*dataOut)");
					v8 = -1073741811;
				}
			}
			else
			{
				EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: pOut != NULL");
				v8 = -1073741811;
			}
		}
		else
		{
			EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: nInLen == sizeof(*dataIn)");
			v8 = -1073741811;
		}
	}
	else
	{
		EspDbgPrintlEx(0, "ESP KMD", "ERROR, ASSERTION FAILED: pIn != NULL");
		v8 = -1073741811;
	}
	return v8;
}
// 40260E: variable 'v5' is possibly undefined
// 402630: variable 'v6' is possibly undefined


//----- (00401838) --------------------------------------------------------
void EnsureFocusStatsInitialized()
{
	if (!s_isFocusStatInitialized)
	{
		s_focusData.pSharpnessDataBuffer = 0;
		s_focusData.sharpnessBufferSizeBytes = 0;
		s_isFocusStatInitialized = 1;
	}
}

//----- (00401864) --------------------------------------------------------
void FocusStats_GetStatsDataSize(unsigned int* pSizeData)
{
	EnsureFocusStatsInitialized();
	if (pSizeData)
		*pSizeData = s_focusData.sharpnessBufferSizeBytes;
	else
		EspDbgPrintlEx(0, "ESP KMD", "ESP:FocusStats_GetStatsDataSize Error: Pointer param null, abort!");
}

//----- (004018A0) --------------------------------------------------------
void FocusStats_RetrieveFocusData(unsigned __int8* pOutData, int* pNumBytesReturned)
{
	EnsureFocusStatsInitialized();
	if (pNumBytesReturned && pOutData)
	{
		memmove(pOutData, (char*)s_focusData.pSharpnessDataBuffer, s_focusData.sharpnessBufferSizeBytes);
		*pNumBytesReturned = s_focusData.sharpnessBufferSizeBytes;
	}
	else
	{
		EspDbgPrintlEx(0, "ESP KMD", "ESP:FocusStats_RetrieveFocusData: Error: Pointer param null, abort!");
	}
}

//----- (004018F0) --------------------------------------------------------
void FocusStats_SetStatsData(unsigned int uStatsDataSizeBytes, unsigned __int8* pStatsData)
{
	EnsureFocusStatsInitialized();
	if (s_focusData.pSharpnessDataBuffer)
	{
		ExFreePool(s_focusData.pSharpnessDataBuffer);
		s_focusData.pSharpnessDataBuffer = 0;
	}
	s_focusData.pSharpnessDataBuffer = (unsigned __int8*)ExAllocatePoolWithTag(
		512,
		uStatsDataSizeBytes,
		(uStatsDataSizeBytes << 16) | 0x45530000);
	if (s_focusData.pSharpnessDataBuffer)
	{
		s_focusData.sharpnessBufferSizeBytes = uStatsDataSizeBytes;
		memmove(s_focusData.pSharpnessDataBuffer, (char*)pStatsData, uStatsDataSizeBytes);
	}
	else
	{
		s_focusData.sharpnessBufferSizeBytes = 0;
		EspDbgPrintlEx(0, "ESP KMD", "ESP:FocusStats_SetStatsData: MemAlloc failed. Abort!");
	}
}


//----- (00401430) --------------------------------------------------------
void CameraSensorOrientation_CameraHeartbeat()
{
	EnsureInitialized();
	s_data.updatesSinceLastHeartbeat = 0;
}

//----- (00401448) --------------------------------------------------------
void CameraSensorOrientation_DiscardNotificationEvent()
{
	EnsureInitialized();
	if (s_data.notificationEventHandle)
	{
		ZwClose(s_data.notificationEventHandle);
		s_data.notificationEventHandle = 0;
	}
}
// 405030: using guessed type int (*ZwClose)(ULONG);

//----- (00401478) --------------------------------------------------------
int CameraSensorOrientation_GetCameraPower()
{
	EnsureInitialized();
	return s_data.isCameraPowerOn;
}

//----- (0040149C) --------------------------------------------------------
void CameraSensorOrientation_GetOrientationData(OrientationInputData* data)
{
	float v2; // r1
	float v3; // r2
	float v4; // r3
	float v5; // r1
	float v6; // r2
	float v7; // r3

	EnsureInitialized();
	v2 = s_data.currentInputData.acc_X;
	v3 = s_data.currentInputData.acc_Y;
	v4 = s_data.currentInputData.acc_Z;
	data->acc_data_available = s_data.currentInputData.acc_data_available;
	data->acc_X = v2;
	data->acc_Y = v3;
	data->acc_Z = v4;
	v5 = s_data.currentInputData.gyro_X;
	v6 = s_data.currentInputData.gyro_Y;
	v7 = s_data.currentInputData.gyro_Z;
	data->gyro_data_available = s_data.currentInputData.gyro_data_available;
	data->gyro_X = v5;
	data->gyro_Y = v6;
	data->gyro_Z = v7;
}

//----- (004014E8) --------------------------------------------------------
TCamDrvReqFlags CameraSensorOrientation_GetRequestFlags()
{
	EnsureInitialized();
	return s_data.currentFlags;
}

//----- (00401508) --------------------------------------------------------
void CameraSensorOrientation_GetThermalCooling(unsigned int* data)
{
	EnsureInitialized();
	*data = s_data.thermalCooling;
}

//----- (00401528) --------------------------------------------------------
void CameraSensorOrientation_Initialize()
{
	s_data.currentFlags = ECamDrvReqFlagsNone;
	s_data.currentInputData.acc_data_available = 0;
	s_data.currentInputData.acc_X = 0.0;
	s_data.currentInputData.acc_Y = 0.0;
	s_data.currentInputData.acc_Z = 0.0;
	s_data.currentInputData.gyro_data_available = 0;
	s_data.currentInputData.gyro_X = 0.0;
	s_data.currentInputData.gyro_Y = 0.0;
	s_data.currentInputData.gyro_Z = 0.0;
	s_data.notificationEventObject = 0;
	s_data.notificationEventHandle = 0;
	s_data.isCameraPowerOn = 0;
	s_data.updatesSinceLastHeartbeat = 0;
	s_data.thermalCooling = 100;
	s_isInitialized = 1;
}

//----- (004015B4) --------------------------------------------------------
BOOLEAN CameraSensorOrientation_IsAccelerometerDataRequested()
{
	EnsureInitialized();
	return (s_data.currentFlags & 1) == 1;
}

//----- (004015EC) --------------------------------------------------------
BOOLEAN CameraSensorOrientation_IsCameraAlive()
{
	return s_data.updatesSinceLastHeartbeat <= 1800;
}

//----- (00401624) --------------------------------------------------------
BOOLEAN CameraSensorOrientation_IsGyroscopeDataRequested()
{
	EnsureInitialized();
	return (s_data.currentFlags & 2) == 2;
}

//----- (0040165C) --------------------------------------------------------
void CameraSensorOrientation_MediatorServiceEvent()
{
	EnsureInitialized();
	++s_data.updatesSinceLastHeartbeat;
}

//----- (00401678) --------------------------------------------------------
int CameraSensorOrientation_ObtainNotificationEvent()
{
	int v1; // [sp+0h] [bp-18h]
	UNICODE_STRING v2; // [sp+8h] [bp-10h] BYREF

	EnsureInitialized();

	RtlInitUnicodeString(&v2, L"\\BaseNamedObjects\\CamDrvOrientationInterfaceUpdate");
	s_data.notificationEventObject = (KEVENT*)IoCreateNotificationEvent(&v2, &s_data.notificationEventHandle);
	if (s_data.notificationEventObject)
		v1 = 0;
	else
		v1 = 240;
	return v1;
}
// 403A60: using guessed type wchar_t aBasenamedobjec[51];
// 405034: using guessed type int (*IoCreateNotificationEvent)(ULONG, ULONG);
// 40503C: using guessed type int (*RtlInitUnicodeString)(ULONG, ULONG);

//----- (004016D0) --------------------------------------------------------
int CameraSensorOrientation_SendUpdateNotification()
{
	EnsureInitialized();
	if (!s_data.notificationEventObject)
		return -1073741604;
	KeSetEvent(s_data.notificationEventObject, 0, 0);
	return 0;
}
// 405038: using guessed type int (*KeSetEvent)(ULONG, ULONG, ULONG);

//----- (00401710) --------------------------------------------------------
void CameraSensorOrientation_SetAccelerometerDataRequested(unsigned __int8 isRequested)
{
	EnsureInitialized();
	if (isRequested)
		s_data.currentFlags |= 1u;
	else
		s_data.currentFlags &= 0xFFFFFFFE;
}

//----- (0040174C) --------------------------------------------------------
void CameraSensorOrientation_SetCameraPower(unsigned __int8 isCameraPowerOn)
{
	EnsureInitialized();
	s_data.isCameraPowerOn = isCameraPowerOn;
}

//----- (00401770) --------------------------------------------------------
void CameraSensorOrientation_SetGyroscopeDataRequested(unsigned __int8 isRequested)
{
	EnsureInitialized();
	if (isRequested)
		s_data.currentFlags |= 2u;
	else
		s_data.currentFlags &= 0xFFFFFFFD;
}

//----- (004017AC) --------------------------------------------------------
void CameraSensorOrientation_SetOrientationData(const OrientationInputData* data)
{
	float v2; // r1
	float v3; // r2
	float v4; // r3
	float v5; // r1
	float v6; // r2
	float v7; // r3

	EnsureInitialized();
	v2 = data->acc_X;
	v3 = data->acc_Y;
	v4 = data->acc_Z;
	s_data.currentInputData.acc_data_available = data->acc_data_available;
	s_data.currentInputData.acc_X = v2;
	s_data.currentInputData.acc_Y = v3;
	s_data.currentInputData.acc_Z = v4;
	v5 = data->gyro_X;
	v6 = data->gyro_Y;
	v7 = data->gyro_Z;
	s_data.currentInputData.gyro_data_available = data->gyro_data_available;
	s_data.currentInputData.gyro_X = v5;
	s_data.currentInputData.gyro_Y = v6;
	s_data.currentInputData.gyro_Z = v7;
}

//----- (004017FC) --------------------------------------------------------
void CameraSensorOrientation_SetThermalCooling(unsigned int data)
{
	EnsureInitialized();
	s_data.thermalCooling = data;
}

//----- (0040181C) --------------------------------------------------------
void EnsureInitialized()
{
	if (!s_isInitialized)
		CameraSensorOrientation_Initialize();
}
