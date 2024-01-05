/*++

Module Name:

    device.c - Device handling events for example driver.

Abstract:

   This file contains the device entry points and callbacks.
    
Environment:

    Kernel-mode Driver Framework

--*/

#include "device.h"
#include "Handler.h"
#include "Queue.h"
#include "Debug.h"
#include "Poclass.h"

#ifdef ALLOC_PRAGMA
#pragma alloc_text (PAGE, CameraESPDeviceCreate)
#pragma alloc_text (PAGE, CameraESPInitializeThermalIoTarget)
#endif

static WDFDEVICE m_device;

//----- (004094B8) --------------------------------------------------------
int CameraESPInitializeThermalIoTarget()
{
    int v1; // [sp+0h] [bp-A0h]
    WDFIOTARGET IoTarget; // [sp+4h] [bp-9Ch] BYREF
    PFDO_DATA v3; // [sp+8h] [bp-98h]
    int v4; // [sp+Ch] [bp-94h]
    int v5; // [sp+14h] [bp-8Ch]
    PFDO_DATA v6; // [sp+18h] [bp-88h]
    UNICODE_STRING TargetDeviceName; // [sp+20h] [bp-80h] BYREF
    WDF_OBJECT_ATTRIBUTES Attributes; // [sp+28h] [bp-78h] BYREF
    WDF_IO_TARGET_OPEN_PARAMS Params; // [sp+48h] [bp-58h] BYREF

    v6 = GetDeviceExtension(m_device);
    v3 = v6;
    WDF_OBJECT_ATTRIBUTES_INIT(&Attributes);
    v4 = WdfIoTargetCreate(m_device, &Attributes, &IoTarget);
    v1 = v4;
    if (v4 >= 0)
    {
        RtlInitUnicodeString(&TargetDeviceName, L"\\??\\Icaros_KMD_ESP_Thermal");
        WDF_IO_TARGET_OPEN_PARAMS_INIT_OPEN_BY_NAME(&Params, &TargetDeviceName, 0x1F0000u);
        v5 = WdfIoTargetOpen(IoTarget, &Params);
        v1 = v5;
        if (v5 >= 0)
        {
            v3->Interfaces[0].IoTarget = IoTarget;
            v3->Interfaces[0].Status = INTERFACE_READY;
        }
        else
        {
            EspDbgPrintlEx(0, "ESP KMD", "Couldn't open IoTarget for interface. 0x%x", v5);
            WdfObjectDelete(IoTarget);
        }
    }
    else
    {
        WdfObjectDelete(IoTarget);
        EspDbgPrintlEx(0, "ESP KMD", "Couldn't create IoTarget for interface. 0x%x", v1);
    }
    return v1;
}
// 40503C: using guessed type int (*RtlInitUnicodeString)(ULONG, ULONG);
// 40993C: using guessed type wchar_t aIcarosKmdEspTh[27];



//----- (00402A4C) --------------------------------------------------------
int CameraESPEnsureThermalIoTargetIsReady()
{
    PFDO_DATA v1; // [sp+8h] [bp-20h]
    int v2; // [sp+Ch] [bp-1Ch]

    v2 = 0;
    EspDbgPrintlEx(
        9,
        "ESP KMD",
        "File: %s, Line: %d, Function: %s",
        "Icaros_KMD_ESP_Device.c",
        266,
        "CameraESPEnsureThermalIoTargetIsReady");
    v1 = GetDeviceExtension(m_device);
    if (v1->ioctlRequestRetryCounter <= 0)
        return -1073741823;
    EspDbgPrintlEx(0, "ESP KMD", "devExt->ioctlRequestRetryCounter = %d", v1->ioctlRequestRetryCounter);
    if (v1->Interfaces[0].Status != INTERFACE_READY)
    {
        v2 = CameraESPInitializeThermalIoTarget();
        if (v2 < 0)
        {
            EspDbgPrintlEx(0, "ESP KMD", "CameraESPInitializeThermalIoTarget() Failed. 0x%x", v2);
            --v1->ioctlRequestRetryCounter;
        }
    }
    EspDbgPrintlEx(
        9,
        "ESP KMD",
        "File: %s, Line: %d, Function: %s",
        "Icaros_KMD_ESP_Device.c",
        293,
        "CameraESPEnsureThermalIoTargetIsReady");
    return v2;
}

//----- (00402B18) --------------------------------------------------------
int CameraESPInitializeLocalParams(WDFDEVICE device)
{
    PFDO_DATA Request; // [sp+8h] [bp-28h]
    int v3; // [sp+Ch] [bp-24h]

    EspDbgPrintlEx(
        9,
        "ESP KMD",
        "File: %s, Line: %d, Function: %s",
        "Icaros_KMD_ESP_Device.c",
        193,
        "CameraESPInitializeLocalParams");
    Request = GetDeviceExtension(device);
    v3 = WdfRequestCreate(0, 0, &Request->ioctlRequest);
    if (v3 >= 0)
    {
        Request->Interfaces[0].IoTarget = 0;
        Request->Interfaces[0].Status = INTERFACE_NEEDED;
        Request->ioctlRequestRetryCounter = 0;
        EspDbgPrintlEx(
            9,
            "ESP KMD",
            "File: %s, Line: %d, Function: %s",
            "Icaros_KMD_ESP_Device.c",
            213,
            "CameraESPInitializeLocalParams");
    }
    else
    {
        EspDbgPrintlEx(0, "ESP KMD", "WdfRequestCreate() Failed. 0x%x", v3);
    }
    return v3;
}

//----- (00402BF0) --------------------------------------------------------
void PassiveCoolingCb(
    _Inout_opt_ PVOID aContext,
    _In_ ULONG aPercentage
)
{
    UNREFERENCED_PARAMETER(aContext);
    EspDbgPrintlEx(0, "ESP KMD", "PassiveCoolingCb aPercentage=%d", aPercentage);
    CameraSensorOrientation_SetThermalCooling(aPercentage);
}

//----- (00402C1C) --------------------------------------------------------
int RegisterThermalCoolingInterface(PDEVICE_CONTEXT pDevCxt)
{
    int v3; // [sp+Ch] [bp-5Ch]
    WDF_QUERY_INTERFACE_CONFIG InterfaceConfig; // [sp+20h] [bp-48h] BYREF
    THERMAL_DEVICE_INTERFACE Interface; // [sp+38h] [bp-30h] BYREF

    EspDbgPrintlEx(
        9,
        "ESP KMD",
        "File: %s, Line: %d, Function: %s",
        "Icaros_KMD_ESP_Device.c",
        58,
        "RegisterThermalCoolingInterface");
    PAGED_CODE();
    Interface.Size = sizeof(Interface);
    Interface.Version = 1;
    Interface.Flags = 0;
    Interface.InterfaceReference = WdfDeviceInterfaceReferenceNoOp;
    Interface.InterfaceDereference = WdfDeviceInterfaceDereferenceNoOp;
    Interface.Context = pDevCxt;
    Interface.ActiveCooling = NULL;
    Interface.PassiveCooling = PassiveCoolingCb;

    WDF_QUERY_INTERFACE_CONFIG_INIT(&InterfaceConfig, 
        (PINTERFACE)&Interface,
        &GUID_THERMAL_COOLING_INTERFACE, 
        NULL);

    v3 = WdfDeviceAddQueryInterface(m_device, &InterfaceConfig);
    if (v3 < 0)
        EspDbgPrintlEx(0, "ESP KMD", "Installing thermal cooling interface failed 0x%X\n", v3);
    EspDbgPrintlEx(
        9,
        "ESP KMD",
        "File: %s, Line: %d, Function: %s",
        "Icaros_KMD_ESP_Device.c",
        80,
        "RegisterThermalCoolingInterface");
    return v3;
}
// 402C5C: control flows out of bounds to 402C5E
// 405000: using guessed type int (*KeGetCurrentIrql)(void);


//----- (0040937C) --------------------------------------------------------
NTSTATUS
CameraESPDeviceCreate(
    _Inout_ PWDFDEVICE_INIT DeviceInit
)
/*++

Routine Description:

    Worker routine called to create a device and its software resources.

Arguments:

    DeviceInit - Pointer to an opaque init structure. Memory for this
                    structure will be freed by the framework when the WdfDeviceCreate
                    succeeds. So don't access the structure after that point.

Return Value:

    NTSTATUS

--*/
{
    int v5; // [sp+8h] [bp-68h]
    int v6; // [sp+8h] [bp-68h]
    WDFDEVICE Device; // [sp+10h] [bp-60h] BYREF
    int v9; // [sp+18h] [bp-58h]
    int v10; // [sp+1Ch] [bp-54h]
    int v11; // [sp+20h] [bp-50h]
    PDEVICE_CONTEXT v12; // [sp+24h] [bp-4Ch]
    int v13; // [sp+28h] [bp-48h]
    PDEVICE_CONTEXT pDevCxt; // [sp+2Ch] [bp-44h]
    int v15; // [sp+30h] [bp-40h]
    int v16; // [sp+34h] [bp-3Ch]
    int v17; // [sp+38h] [bp-38h]
    WDF_OBJECT_ATTRIBUTES Attributes; // [sp+40h] [bp-30h] BYREF

    PAGED_CODE();

    v15 = 1;
    EspDbgPrintlEx(
        9,
        "ESP KMD",
        "File: %s, Line: %d, Function: %s",
        "Icaros_KMD_ESP_Device.c",
        112,
        "CameraESPDeviceCreate");

    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&Attributes, FDO_DATA);
    Attributes.SynchronizationScope = WdfSynchronizationScopeQueue;
    Attributes.ExecutionLevel = WdfExecutionLevelPassive;
    v10 = WdfDeviceCreate(&DeviceInit, &Attributes, &Device);
    v5 = v10;
    if (v10 >= 0)
    {
        m_device = Device;
        v13 = WdfDeviceCreateDeviceInterface(Device, &GUID_DEVINTERFACE_CAMERA_ESP_DRIVER, 0);
        v6 = v13;
        if (v13 >= 0)
        {
            v17 = CameraESPQueueInitialize(Device);
            v6 = v17;
            if (v17 >= 0)
            {
                v11 = CameraESPInitializeLocalParams(Device);
                v6 = v11;
                if (v11 >= 0)
                {
                    v12 = DeviceGetContext(Device);
                    pDevCxt = v12;
                    v16 = RegisterThermalCoolingInterface(v12);
                    v6 = v16;
                    if (v16 >= 0)
                        WdfDeviceSetDeviceInterfaceState(Device, &GUID_DEVINTERFACE_CAMERA_ESP_DRIVER, 0, 1);
                }
            }
        }
        EspDbgPrintlEx(
            9,
            "ESP KMD",
            "File: %s, Line: %d, Function: %s",
            "Icaros_KMD_ESP_Device.c",
            183,
            "CameraESPDeviceCreate");
        v9 = v6;
    }
    else
    {
        EspDbgPrintlEx(0, "ESP KMD", "CameraESPDeviceCreate ( WdfDeviceCreate() ) failed. 0x%x\n", v10);
        v9 = v5;
    }
    return v9;
}
// 4093A8: control flows out of bounds to 4093AA
// 409422: variable 'v2' is possibly undefined
// 409452: variable 'v3' is possibly undefined
// 405000: using guessed type int (*KeGetCurrentIrql)(void);