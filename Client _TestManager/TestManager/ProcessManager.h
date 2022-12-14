#pragma once

#include "framework.h"
#include "ControlManager.h"

// 벡터와 wstring을 쓰기위해 넣어 줌
#include <vector>
#include <string>

using namespace std;

class ProcessManager
{
public:
	static bool initProcessInfo(WCHAR* dest__, WCHAR* src__, int pid__);
	static void killTargetProcess(int pid__);
	static bool checkTargetProcess(WCHAR* procName__);
	static void CALLBACK checkProcessMain(HWND, UINT, WPARAM, LPARAM);
	// ProcessManager : 생성자
	// 벡터의 포인터를 이용해 블랙리스트를 넘겨줌
	ProcessManager(ControlManager* cm__, vector<wstring>* g_processList);
private:
	static ControlManager* controlManager;
	static bool checkPid(int pid__);
	static bool checkChar(WCHAR char__);
	static bool charTranslation(WCHAR* dest__, WCHAR* src__);
	static bool exceptProcess(WCHAR* procName__);
};

