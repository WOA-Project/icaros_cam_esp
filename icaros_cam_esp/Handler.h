/*++

Module Name:

    handler.h

Abstract:

    This file contains the driver definitions.

Environment:

    Kernel-mode Driver Framework

--*/

#pragma once

#include "Public.h"

void CameraSensorOrientation_CameraHeartbeat();
void CameraSensorOrientation_DiscardNotificationEvent();
int CameraSensorOrientation_GetCameraPower();
void CameraSensorOrientation_GetOrientationData(OrientationInputData* data);
TCamDrvReqFlags CameraSensorOrientation_GetRequestFlags();
void CameraSensorOrientation_GetThermalCooling(unsigned int* data);
void CameraSensorOrientation_Initialize();
BOOLEAN CameraSensorOrientation_IsAccelerometerDataRequested();
BOOLEAN CameraSensorOrientation_IsCameraAlive();
BOOLEAN CameraSensorOrientation_IsGyroscopeDataRequested();
void CameraSensorOrientation_MediatorServiceEvent();
int CameraSensorOrientation_ObtainNotificationEvent();
int CameraSensorOrientation_SendUpdateNotification();
void CameraSensorOrientation_SetAccelerometerDataRequested(unsigned __int8 isRequested);
void CameraSensorOrientation_SetCameraPower(unsigned __int8 isCameraPowerOn);
void CameraSensorOrientation_SetGyroscopeDataRequested(unsigned __int8 isRequested);
void CameraSensorOrientation_SetOrientationData(const OrientationInputData* data);
void CameraSensorOrientation_SetThermalCooling(unsigned int data);


void EnsureInitialized();
void EnsureFocusStatsInitialized();
void FocusStats_GetStatsDataSize(unsigned int* pSizeData);
void FocusStats_RetrieveFocusData(unsigned __int8* pOutData, int* pNumBytesReturned);
void FocusStats_SetStatsData(unsigned int uStatsDataSizeBytes, unsigned __int8* pStatsData);

int CameraESPEnsureThermalIoTargetIsReady();

int IcarosESPHandler_DeRegPoweredCameraIdEvent(void* pIn, size_t nInLen, void* pOut, size_t nOutLen, int* pnOutLenReq);
int IcarosESPHandler_GetCameraPowerState(void* pIn, size_t nInLen, void* pOut, size_t nOutLen, int* pnOutLenReq);
int IcarosESPHandler_GetCurrentRequestFlags(void* pIn, size_t nInLen, void* pOut, size_t nOutLen, int* pnOutLenReq);
int IcarosESPHandler_GetFocusStatsData(void* pIn, size_t nInLen, void* pOut, size_t nOutLen, int* pnOutLenReq);
int IcarosESPHandler_GetFocusStatsSize(void* pIn, size_t nInLen, void* pOut, size_t nOutLen, int* pnOutLenReq);
int IcarosESPHandler_GetPoweredCameraId(void* pIn, size_t nInLen, void* pOut, size_t nOutLen, int* pnOutLenReq);
int IcarosESPHandler_PowerOnEventExt(void* pIn, size_t nInLen);
int IcarosESPHandler_RegPoweredCameraIdEvent(void* pIn, size_t nInLen, void* pOut, size_t nOutLen, int* pnOutLenReq);
int IcarosESPHandler_RegisterOrientationDataMediator(void* pIn, size_t nInLen, void* pOut, size_t nOutLen, int* pnOutLenReq);
int IcarosESPHandler_SetAutoFocusStats(void* pIn, size_t nInLen, void* pOut, size_t nOutLen, int* pnOutLenReq);
int IcarosESPHandler_SetCurrentOrientationData(void* pIn, size_t nInLen, void* pOut, size_t nOutLen, int* pnOutLenReq);
int IcarosESPHandler_SetTemperatureData(unsigned int temperature);
int IcarosESPHandler_SyncPerFrameData(void* pIn, size_t nInLen, void* pOut, size_t nOutLen, int* pnOutLenReq);
int IcarosESPHandler_UnregisterOrientationDataMediator(void* pIn, size_t nInLen, void* pOut, size_t nOutLen, int* pnOutLenReq);
int IcarosESPHanlder_SendIOCTL(WDFIOTARGET IoTarget, unsigned int IOCTLCode, WDFMEMORY InputBuffer, WDFMEMORY_OFFSET* InputBufferOffset, WDFMEMORY OutputBuffer, WDFMEMORY_OFFSET* OutputBufferOffset);
int IcarosESPHandler_GetCurrentOrientationData(void* pIn, size_t nInLen, void* pOut, size_t nOutLen, int* pnOutLenReq);
int IcarosESPHandler_PowerOffEvent();
int IcarosESPHandler_PowerOnEvent();