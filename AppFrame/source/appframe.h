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

//#pragma comment(lib,"AppFrame.lib")
#pragma comment(lib,"../x64/Debug/AppFrame.lib")
//#pragma comment(lib,"../x64/Release/AppFrame.lib")

#include "Application/ApplicationBase.h"
#include "CFile/CFile.h"
#include "Mode/ModeServer.h"
#include "modelManager/modelManager.h"