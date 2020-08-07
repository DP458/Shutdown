#pragma once

#include <WinSDKVer.h>

#define _WIN32_WINNT _WIN32_WINNT_WIN7

#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <windowsx.h>
#include <CommCtrl.h>
#include <sstream>
#include <fstream>
#include <memory>
#include <powrprof.h>
#include <shobjidl.h>
#include <shldisp.h>
#include "MainStaticObject.h"
#include "Shutdown.h"