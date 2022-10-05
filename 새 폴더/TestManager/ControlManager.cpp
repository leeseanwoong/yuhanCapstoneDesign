#include "ControlManager.h"



void ControlManager::setProcessTerminateMode()
{
	endFlag = true;
}


bool ControlManager::getProcessTerminateMode()
{
	return endFlag;
}


int ControlManager::getExecuteStatus()
{
	return executeCnt;
}


bool ControlManager::genExecuteProcessString(wchar_t *buf__, int len__)
{
	if (nullptr == buf__)
		return false;

	if (MAX_PATH < len__)
		return false;

	if (0 == lstrlenW(processFullPath))
		return false;

	// 실행 횟수 증가
	executeCnt++;
/*
	wchar_t buf[256] = { 0, };
	wsprintfW(buf, L"%d executed", executeCnt);
	MessageBox(NULL, buf, buf, MB_OK);
*/
	wsprintfW(buf__, L"%s %d", processFullPath, executeCnt);
	
	return true;
}


void ControlManager::initControlManager()
{
	endFlag = false;
}


void ControlManager::initBuffers()
{
	wmemset(processName, 0x00, MAX_PATH);
	wmemset(processFullPath, 0x00, MAX_PATH);
}


ControlManager::ControlManager(wchar_t* fullPath__, int exeCnt__)
{
	processManagement = false;
	initBuffers();
	initControlManager();

	/// 실행 상태 횟수 정보 누적
	executeCnt = exeCnt__;

	/// 전체 경로를 복사
	wmemcpy_s(processFullPath, MAX_PATH, fullPath__, MAX_PATH);
	/// 프로세스 이름 획득
	_wsplitpath_s(fullPath__, NULL, NULL, NULL, NULL, processName, MAX_PATH, NULL, NULL);
}


bool ControlManager::checkMyProcess(wchar_t *procName__)
{
	if (0 == lstrcmpW(processName, procName__))
		return true;

	return false;
}

bool ControlManager::getProcessManagementMode()
{
	return processManagement;
}


void ControlManager::setProcessManagementMode()
{
	processManagement = true;
}
