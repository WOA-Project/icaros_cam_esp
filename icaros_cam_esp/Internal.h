#pragma once

#include "Public.h"

unsigned __int8 s_isInitialized = 0u;

InternalCameraSensorOrientationData s_data =
{
  ECamDrvReqFlagsNone,
  { 0,  0.0,  0.0,  0.0, 0,  0.0,  0.0,  0.0 },
  NULL,
  NULL,
  0u,
  0,
  0u
};

unsigned __int8 s_isFocusStatInitialized = 0u;

InternalFocusStatsData s_focusData = { NULL, 0u };

int g_PoweredCamera = 0;

KEVENT* g_PoweredCameraEvent = NULL;
