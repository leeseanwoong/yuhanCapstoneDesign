#pragma once

#include "framework.h"
#include "ControlManager.h"


class ProcessManager
{
public:
	static bool initProcessInfo(WCHAR *dest__, WCHAR *src__, int pid__);
	static void killTargetProcess(int pid__);
	static bool checkTargetProcess(WCHAR * procName__);
	static void CALLBACK checkProcessMain(HWND, UINT, WPARAM, LPARAM);
	ProcessManager(ControlManager *cm__);
private:
	static ControlManager *controlManager;
	static bool checkPid(int pid__);
	static bool checkChar(WCHAR char__);
	static bool charTranslation(WCHAR *dest__, WCHAR *src__);
	static bool exceptProcess(WCHAR *procName__);
};

