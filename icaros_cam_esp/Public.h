/*++

Module Name:

    public.h

Abstract:

    This module contains the common declarations shared by driver
    and user applications.

Environment:

    user and kernel

--*/

#pragma once

#include <ntddk.h>
#include <wdf.h>
#include <initguid.h>

//
// Define an Interface Guid so that apps can find the device and talk to it.
//

#define INTERFACE_NEEDED 0
#define INTERFACE_READY 1

typedef enum _TCamDrvReqFlags {
    ECamDrvReqFlagsNone,
    ECamDrvReqFlagsAcc,
    ECamDrvReqFlagsGyro,
    ECamDrvReqFlagsEnd = 0x7FFFFFFF
} TCamDrvReqFlags;

typedef struct _OrientationInputData {
    ULONG acc_data_available;
    float acc_X;
    float acc_Y;
    float acc_Z;
    ULONG gyro_data_available;
    float gyro_X;
    float gyro_Y;
    float gyro_Z;
} OrientationInputData, *POrientationInputData;

typedef struct _InternalCameraSensorOrientationData {
    TCamDrvReqFlags currentFlags;
    OrientationInputData currentInputData;
    PKEVENT notificationEventObject;
    HANDLE notificationEventHandle;
    BOOLEAN isCameraPowerOn;
    ULONG updatesSinceLastHeartbeat;
    ULONG thermalCooling;
} InternalCameraSensorOrientationData, * PInternalCameraSensorOrientationData;
typedef struct _InternalFocusStatsData {
    PVOID pSharpnessDataBuffer;
    ULONG sharpnessBufferSizeBytes;
} InternalFocusStatsData, * PInternalFocusStatsData;

typedef struct {
    WDFIOTARGET IoTarget;
    ULONG Status;
} FDO_INTERFACE_DATA, *PFDO_INTERFACE_DATA;

typedef struct {
    WDFREQUEST ioctlRequest;
    ULONG ioctlRequestRetryCounter;
    FDO_INTERFACE_DATA Interfaces[1];
} FDO_DATA, * PFDO_DATA;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(FDO_DATA, GetDeviceExtension);