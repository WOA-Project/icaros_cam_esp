/*++

Module Name:

    device.h

Abstract:

    This file contains the device definitions.

Environment:

    Kernel-mode Driver Framework

--*/

#pragma once

#include "public.h"

EXTERN_C_START

static WDFDEVICE m_device;

static GUID GUID_THERMAL_COOLING_INTERFACE =
{
  3971893160u,
  50328u,
  19385u,
  { 189u, 112u, 232u, 103u, 224u, 148u, 13u, 34u }
};
static GUID GUID_DEVINTERFACE_CAMERA_ESP_DRIVER =
{
  3858622935u,
  31246u,
  16456u,
  { 129u, 187u, 167u, 124u, 175u, 123u, 52u, 159u }
};

//
// The device context performs the same job as
// a WDM device extension in the driver frameworks
//
typedef struct _DEVICE_CONTEXT
{
    ULONG PrivateDeviceData;  // just a placeholder

} DEVICE_CONTEXT, *PDEVICE_CONTEXT;

//
// This macro will generate an inline function called DeviceGetContext
// which will be used to get a pointer to the device context memory
// in a type safe manner.
//
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_CONTEXT, DeviceGetContext)
//
// Function to initialize the device and its callbacks
//
NTSTATUS
CameraESPDeviceCreate(
    _Inout_ PWDFDEVICE_INIT DeviceInit
    );

int RegisterThermalCoolingInterface(PDEVICE_CONTEXT pDevCxt);
int CameraESPInitializeThermalIoTarget();

EXTERN_C_END
