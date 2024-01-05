/*++

Module Name:

    driver.c

Abstract:

    This file contains the driver entry points and callbacks.

Environment:

    Kernel-mode Driver Framework

--*/

#include "driver.h"
#include "Device.h"
#include "Debug.h"

#ifdef ALLOC_PRAGMA
#pragma alloc_text (INIT, DriverEntry)
#pragma alloc_text (PAGE, CameraESPEvtDeviceAdd)
#endif


//----- (0040A000) --------------------------------------------------------
NTSTATUS
DriverEntry(
    _In_ PDRIVER_OBJECT  DriverObject,
    _In_ PUNICODE_STRING RegistryPath
)
/*++

Routine Description:
    DriverEntry initializes the driver and is the first routine called by the
    system after the driver is loaded. DriverEntry specifies the other entry
    points in the function driver, such as EvtDevice and DriverUnload.

Parameters Description:

    DriverObject - represents the instance of the function driver that is loaded
    into memory. DriverEntry must initialize members of DriverObject before it
    returns to the caller. DriverObject is allocated by the system before the
    driver is loaded, and it is released by the system after the system unloads
    the function driver from memory.

    RegistryPath - represents the driver specific path in the Registry.
    The function driver can use the path to store driver related data between
    reboots. The path does not store hardware instance specific data.

Return Value:

    STATUS_SUCCESS if successful,
    STATUS_UNSUCCESSFUL otherwise.

--*/
{
    int v3; // [sp+8h] [bp-58h]
    WDF_DRIVER_CONFIG Config; // [sp+18h] [bp-48h] BYREF
    WDF_OBJECT_ATTRIBUTES Attributes; // [sp+30h] [bp-30h] BYREF
    DRIVER_OBJECT* DriverObjecta; // [sp+58h] [bp-8h]
    UNICODE_STRING* RegistryPatha; // [sp+5Ch] [bp-4h]

    DriverObjecta = DriverObject;
    RegistryPatha = RegistryPath;
    EspDbgPrintlEx(9, "ESP KMD", "File: %s, Line: %d, Function: %s", "Icaros_KMD_ESP_Driver.c", 59, "DriverEntry");
    WDF_DRIVER_CONFIG_INIT(&Config, CameraESPEvtDeviceAdd);
    WDF_OBJECT_ATTRIBUTES_INIT(&Attributes);
    Attributes.SynchronizationScope = WdfSynchronizationScopeNone;
    Config.DriverPoolTag = 1347634508;
    v3 = WdfDriverCreate(DriverObjecta, RegistryPatha, &Attributes, &Config, 0);
    if (v3 >= 0)
        EspDbgPrintlEx(9, "ESP KMD", "File: %s, Line: %d, Function: %s", "Icaros_KMD_ESP_Driver.c", 83, "DriverEntry");
    else
        EspDbgPrintlEx(0, "ESP KMD", "Error: WdfDriverCreate failed 0x%x", v3);
    return v3;
}



//----- (004092EC) --------------------------------------------------------
NTSTATUS
CameraESPEvtDeviceAdd(
    _In_    WDFDRIVER       Driver,
    _Inout_ PWDFDEVICE_INIT DeviceInit
)
/*++
Routine Description:

    EvtDeviceAdd is called by the framework in response to AddDevice
    call from the PnP manager. We create and initialize a device object to
    represent a new instance of the device.

Arguments:

    Driver - Handle to a framework driver object created in DriverEntry

    DeviceInit - Pointer to a framework-allocated WDFDEVICE_INIT structure.

Return Value:

    NTSTATUS

--*/
{
    int v4; // [sp+18h] [bp-18h]

    UNREFERENCED_PARAMETER(Driver);

    PAGED_CODE();

    EspDbgPrintlEx(
        9,
        "ESP KMD",
        "File: %s, Line: %d, Function: %s",
        "Icaros_KMD_ESP_Driver.c",
        117,
        "CameraESPEvtDeviceAdd");
    v4 = CameraESPDeviceCreate(DeviceInit);
    EspDbgPrintlEx(
        9,
        "ESP KMD",
        "File: %s, Line: %d, Function: %s",
        "Icaros_KMD_ESP_Driver.c",
        121,
        "CameraESPEvtDeviceAdd");
    return v4;
}
// 409318: control flows out of bounds to 40931A
// 40933A: variable 'v2' is possibly undefined
// 405000: using guessed type int (*KeGetCurrentIrql)(void);