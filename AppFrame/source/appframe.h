/*****************************************************************//**
 * \file   appframe.h
 * \brief  フレームワークのライブラリ指定
 *
 * \author 山本莉久
 * \date   March 2023
 *********************************************************************/
#pragma once
#include<vector>
#include<map>
#include<string>
#include<cmath>
#include <thread>
#include <future>

#if _DEBUG
#pragma comment(lib,"../x64/Debug/AppFrame.lib")
#elif !_DEBUG
#pragma comment(lib,"../x64/Release/AppFrame.lib")
#endif

//#pragma comment(lib,"AppFrame.lib")

#include "Application/ApplicationBase.h"
#include "CFile/CFile.h"
#include "Mode/ModeServer.h"
#include "modelManager/modelManager.h"