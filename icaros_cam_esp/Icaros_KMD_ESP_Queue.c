/*++

Module Name:

    queue.c

Abstract:

    This file contains the queue entry points and callbacks.

Environment:

    Kernel-mode Driver Framework

--*/

#include "Queue.h"
#include "Handler.h"
#include "Debug.h"

#ifdef ALLOC_PRAGMA
#pragma alloc_text (PAGE, CameraESPQueueInitialize)
#endif


//----- (00402F80) --------------------------------------------------------
void CameraESPEvtIoControl
(
    _In_
    WDFQUEUE Queue,
    _In_
    WDFREQUEST Request,
    _In_
    size_t OutputBufferLength,
    _In_
    size_t InputBufferLength,
    _In_
    ULONG IoControlCode
)
{
    int Status; // [sp+8h] [bp-98h]
    int Statusa; // [sp+8h] [bp-98h]
    unsigned int Information; // [sp+Ch] [bp-94h] BYREF
    void* pIn; // [sp+10h] [bp-90h]
    void* pOut; // [sp+14h] [bp-8Ch]
    int* pnOutLenReq; // [sp+18h] [bp-88h]
    unsigned int v11; // [sp+1Ch] [bp-84h]
    int v12; // [sp+20h] [bp-80h]
    int v13; // [sp+24h] [bp-7Ch]
    int v14; // [sp+28h] [bp-78h]
    WDFMEMORY Memory; // [sp+2Ch] [bp-74h] BYREF
    int v16; // [sp+30h] [bp-70h]
    int v17; // [sp+34h] [bp-6Ch]
    int v18; // [sp+38h] [bp-68h]
    void* v19; // [sp+3Ch] [bp-64h]
    int v20; // [sp+40h] [bp-60h]
    int v21; // [sp+44h] [bp-5Ch]
    int v22; // [sp+48h] [bp-58h]
    int v23; // [sp+4Ch] [bp-54h]
    int v24; // [sp+50h] [bp-50h]
    int v25; // [sp+54h] [bp-4Ch]
    int v26; // [sp+58h] [bp-48h]
    WDFMEMORY v27; // [sp+5Ch] [bp-44h] BYREF
    int v28; // [sp+60h] [bp-40h]
    WDFDEVICE v29; // [sp+64h] [bp-3Ch]
    int v30; // [sp+68h] [bp-38h]
    void* v31; // [sp+6Ch] [bp-34h]
    int v32; // [sp+70h] [bp-30h]
    int v33; // [sp+74h] [bp-2Ch]
    int v34; // [sp+78h] [bp-28h]
    WDFDEVICE v35; // [sp+7Ch] [bp-24h]
    size_t BufferSize; // [sp+80h] [bp-20h] BYREF

    pIn = 0;
    pOut = 0;
    Information = 0;
    pnOutLenReq = 0;
    v29 = WdfIoQueueGetDevice(Queue);
    v35 = v29;
    EspDbgPrintlEx(
        9,
        "ESP KMD",
        "File: %s, Line: %d, Function: %s",
        "Icaros_KMD_ESP_Queue.c",
        171,
        "CameraESPEvtIoControl");
    if (InputBufferLength)
    {
        v25 = WdfRequestRetrieveInputMemory(Request, &Memory);
        Status = v25;
        if (v25)
        {
            EspDbgPrintlEx(0, "ESP KMD", "CameraESPEvtIoControl: ERROR: Retrieving input memory failed: %d", v25);
            WdfRequestComplete(Request, Status);
            return;
        }
        v31 = (void*)WdfMemoryGetBuffer(Memory, &BufferSize);
        pIn = v31;
        if (!v31)
        {
            EspDbgPrintlEx(
                0,
                "ESP KMD",
                "CameraESPEvtIoControl: ERROR: Getting pointer to input buffer failed: %d",
                -1073741823);
            WdfRequestComplete(Request, -1073741823);
            return;
        }
    }
    if (OutputBufferLength)
    {
        v17 = WdfRequestRetrieveOutputMemory(Request, &v27);
        Statusa = v17;
        if (v17)
        {
            EspDbgPrintlEx(0, "ESP KMD", "CameraESPEvtIoControl: ERROR: Retrieving output memory failed: %d", v17);
            WdfRequestComplete(Request, Statusa);
            return;
        }
        v19 = (void*)WdfMemoryGetBuffer(v27, 0);
        pOut = v19;
        if (!v19)
        {
            EspDbgPrintlEx(
                0,
                "ESP KMD",
                "CameraESPEvtIoControl: ERROR: Getting pointer to output buffer failed: %d",
                -1073741823);
            WdfRequestComplete(Request, -1073741823);
            return;
        }
        pnOutLenReq = (int*)&Information;
    }
    v11 = IoControlCode - 2237440;
    switch (IoControlCode)
    {
    case 0x222400u:
        v34 = IcarosESPHandler_RegisterOrientationDataMediator(pIn, InputBufferLength, pOut, OutputBufferLength, pnOutLenReq);
        WdfRequestCompleteWithInformation(Request, v34, Information);
        break;
    case 0x222404u:
        v21 = IcarosESPHandler_UnregisterOrientationDataMediator(pIn, InputBufferLength, pOut, OutputBufferLength, pnOutLenReq);
        WdfRequestCompleteWithInformation(Request, v21, Information);
        break;
    case 0x222408u:
        v12 = IcarosESPHandler_GetCurrentRequestFlags(pIn, InputBufferLength, pOut, OutputBufferLength, pnOutLenReq);
        WdfRequestCompleteWithInformation(Request, v12, Information);
        break;
    case 0x22240Cu:
        v23 = IcarosESPHandler_SetCurrentOrientationData(pIn, InputBufferLength, pOut, OutputBufferLength, pnOutLenReq);
        WdfRequestCompleteWithInformation(Request, v23, Information);
        break;
    case 0x222410u:
        v33 = IcarosESPHandler_PowerOnEvent();
        WdfRequestCompleteWithInformation(Request, v33, Information);
        break;
    case 0x222414u:
        v14 = IcarosESPHandler_PowerOffEvent();
        WdfRequestCompleteWithInformation(Request, v14, Information);
        break;
    case 0x222418u:
        v16 = IcarosESPHandler_GetCurrentOrientationData(pIn, InputBufferLength, pOut, OutputBufferLength, pnOutLenReq);
        WdfRequestCompleteWithInformation(Request, v16, Information);
        break;
    case 0x22241Cu:
        v18 = IcarosESPHandler_SyncPerFrameData(pIn, InputBufferLength, pOut, OutputBufferLength, pnOutLenReq);
        WdfRequestCompleteWithInformation(Request, v18, Information);
        break;
    case 0x222420u:
        v20 = IcarosESPHandler_GetCameraPowerState(pIn, InputBufferLength, pOut, OutputBufferLength, pnOutLenReq);
        WdfRequestCompleteWithInformation(Request, v20, Information);
        break;
    case 0x222428u:
        v22 = IcarosESPHandler_SetAutoFocusStats(pIn, InputBufferLength, pOut, OutputBufferLength, pnOutLenReq);
        WdfRequestCompleteWithInformation(Request, v22, Information);
        break;
    case 0x22242Cu:
        v24 = IcarosESPHandler_GetFocusStatsSize(pIn, InputBufferLength, pOut, OutputBufferLength, pnOutLenReq);
        WdfRequestCompleteWithInformation(Request, v24, Information);
        break;
    case 0x222430u:
        v26 = IcarosESPHandler_GetFocusStatsData(pIn, InputBufferLength, pOut, OutputBufferLength, pnOutLenReq);
        WdfRequestCompleteWithInformation(Request, v26, Information);
        break;
    case 0x222434u:
        v13 = IcarosESPHandler_PowerOnEventExt(pIn, InputBufferLength);
        WdfRequestCompleteWithInformation(Request, v13, Information);
        break;
    case 0x222438u:
        v28 = IcarosESPHandler_GetPoweredCameraId(pIn, InputBufferLength, pOut, OutputBufferLength, pnOutLenReq);
        WdfRequestCompleteWithInformation(Request, v28, Information);
        break;
    case 0x22243Cu:
        v30 = IcarosESPHandler_RegPoweredCameraIdEvent(pIn, InputBufferLength, pOut, OutputBufferLength, pnOutLenReq);
        WdfRequestCompleteWithInformation(Request, v30, Information);
        break;
    case 0x222440u:
        v32 = IcarosESPHandler_DeRegPoweredCameraIdEvent(pIn, InputBufferLength, pOut, OutputBufferLength, pnOutLenReq);
        WdfRequestCompleteWithInformation(Request, v32, Information);
        break;
    default:
        EspDbgPrintlEx(0, "ESP KMD", "CameraESPEvtIoControl: Error: got invalid message");
        WdfRequestCompleteWithInformation(Request, -1073741637, Information);
        break;
    }
    EspDbgPrintlEx(
        9,
        "ESP KMD",
        "File: %s, Line: %d, Function: %s",
        "Icaros_KMD_ESP_Queue.c",
        306,
        "CameraESPEvtIoControl");
}

//----- (00403384) --------------------------------------------------------
void CameraESPEvtIoQueueContextDestroy(void* Object)
{
    QueueGetContext(Object);
    EspDbgPrintlEx(
        9,
        "ESP KMD",
        "File: %s, Line: %d, Function: %s",
        "Icaros_KMD_ESP_Queue.c",
        140,
        "CameraESPEvtIoQueueContextDestroy");
    EspDbgPrintlEx(
        9,
        "ESP KMD",
        "File: %s, Line: %d, Function: %s",
        "Icaros_KMD_ESP_Queue.c",
        144,
        "CameraESPEvtIoQueueContextDestroy");
}

//----- (004033DC) --------------------------------------------------------
void CameraESPThermalEvtQueueIoStop(WDFQUEUE Queue, WDFREQUEST Request, unsigned int ActionFlags)
{
    int v3; // [sp+8h] [bp-28h]

    UNREFERENCED_PARAMETER(Queue);

    EspDbgPrintlEx(
        9,
        "ESP KMD",
        "File: %s, Line: %d, Function: %s",
        "Icaros_KMD_ESP_Queue.c",
        346,
        "CameraESPThermalEvtQueueIoStop");
    if ((ActionFlags & 0x10000000) == 0)
    {
    LABEL_5:
        WdfRequestStopAcknowledge(Request, 0);
        goto CameraESPThermalQueueIoStopEnd;
    }
    v3 = WdfRequestUnmarkCancelable(Request);
    if (v3 != -1073741536)
    {
        NT_ASSERT(v3 >= 0);
        goto LABEL_5;
    }
CameraESPThermalQueueIoStopEnd:
    EspDbgPrintlEx(
        9,
        "ESP KMD",
        "File: %s, Line: %d, Function: %s",
        "Icaros_KMD_ESP_Queue.c",
        361,
        "CameraESPThermalEvtQueueIoStop");
}
// 403430: control flows out of bounds to 403432

//----- (00409578) --------------------------------------------------------
NTSTATUS CameraESPQueueInitialize(_In_ WDFDEVICE Device)
{
    int v3; // [sp+Ch] [bp-64h]
    WDFQUEUE Queue; // [sp+20h] [bp-50h] BYREF
    WDF_IO_QUEUE_CONFIG Config; // [sp+28h] [bp-48h] BYREF
    WDFDEVICE Devicea; // [sp+68h] [bp-8h]

    Devicea = Device;

    PAGED_CODE();

    EspDbgPrintlEx(
        9,
        "ESP KMD",
        "File: %s, Line: %d, Function: %s",
        "Icaros_KMD_ESP_Queue.c",
        70,
        "CameraESPQueueInitialize");
    WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&Config, WdfIoQueueDispatchSequential);
    Config.EvtIoDeviceControl = CameraESPEvtIoControl;
    v3 = WdfIoQueueCreate(Devicea, &Config, 0, &Queue);
    if (v3 >= 0)
        EspDbgPrintlEx(
            9,
            "ESP KMD",
            "File: %s, Line: %d, Function: %s",
            "Icaros_KMD_ESP_Queue.c",
            112,
            "CameraESPQueueInitialize");
    else
        EspDbgPrintlEx(0, "ESP KMD", "WdfIoQueueCreate() failed 0x%x", v3);
    return v3;
}
// 4095A4: control flows out of bounds to 4095A6
// 405000: using guessed type int (*KeGetCurrentIrql)(void);