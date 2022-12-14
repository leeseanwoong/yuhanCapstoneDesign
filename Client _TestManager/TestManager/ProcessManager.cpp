#include "ProcessManager.h"

#include <cctype>
#include <TlHelp32.h>


ControlManager* ProcessManager::controlManager = NULL;

// ������ �����͸� �޴� ����
vector<wstring>* ProcessName;

// ������
ProcessManager::ProcessManager(ControlManager* cm__, vector<wstring>* g_processList)
{
	controlManager = cm__;
	// ������ �ּҸ� ������ ����
	ProcessName = g_processList;
}


bool ProcessManager::checkChar(WCHAR char__)
{
	if (('A' <= char__) && ('Z' >= char__))
		return true;
	if (('a' <= char__) && ('z' >= char__))
		return true;

	return false;
}


bool ProcessManager::checkPid(int pid__)
{
	if (0 == pid__)
		return false;
	if (4 == pid__)
		return false;

	return true;
}


bool ProcessManager::charTranslation(WCHAR* dest__, WCHAR* src__)
{
	int len = 0;
	wchar_t tmp[MAX_PATH] = { 0, };

	if (nullptr == src__)
		return false;

	len = lstrlenW(src__);
	if (MAX_PATH < len)
		return false;

	for (int i = 0; i < len; i++)
	{
		if (false == checkChar(src__[i]))
		{
			tmp[i] = src__[i];
			continue;
		}

		tmp[i] = std::tolower(src__[i]);
	}

	memcpy_s(dest__, MAX_PATH, tmp, len * 2);

	return true;
}


bool ProcessManager::initProcessInfo(WCHAR* dest__, WCHAR* src__, int pid__)
{
	do
	{
		// pid Ȯ��
		if (false == checkPid(pid__))
			break;

		// ���μ��� �̸��� �ҹ��ڷ� ����
		if (false == charTranslation(dest__, src__))
			break;

		// ���� ���μ��� ���� Ȯ��
		if (false != exceptProcess(dest__))
			break;

		return true;

	} while (false);

	return false;
}


bool ProcessManager::exceptProcess(WCHAR* procName__)
{
	int i = 0;
	static const wchar_t exceptProcessName[][40] = {
		L"system",
		L"system interrupts",
		L"svchost.exe",
		L"wslhost.exe",
		L"conhost.exe",
		L"runtimebroker.exe",
		L"wsl.exe",
		L"services.exe",
		L"sihost.exe"
		L"smss.exe",
		L"winlogon.exe",
		L"audiodg.exe",
		L".exe",
		L"vmmem",
		L"vmwp.exe",
		L"csrss.exe",
		L"lsass.exe",
		L"registry",
		L"srcure system",
		L"spoolsv.exe",
		L"sqlwriter.exe",
		L"vmms.exe",
		L"wininit.exe",
	};

	for (i = 0; i < sizeof(exceptProcessName) / sizeof(exceptProcessName[0]); i++)
	{
		if (0 == lstrcmpW(exceptProcessName[i], procName__))
		{
			return true;
		}
	}

	return false;
}

// ���� ����ǰ� �ִ� ���α׷��� ������Ʈ�� �� --> ������Ʈ�� �ִٸ� ���α׷� ����
bool ProcessManager::checkTargetProcess(WCHAR* procName__)
{
	for (int i = 0; i < ProcessName->size(); i++)
	{
		if (0 == lstrcmpW(ProcessName->at(i).c_str(), procName__))
		{
			return true;
		}
	}
	return false;
}


void ProcessManager::killTargetProcess(int pid__)
{
	HANDLE hProc;

	if (false == checkPid(pid__))
		return;

	hProc = OpenProcess(PROCESS_TERMINATE, 0, pid__);
	if (NULL == hProc)
		return;

	TerminateProcess(hProc, 0);
}


#define _DEBUG_MODE_	0

void CALLBACK ProcessManager::checkProcessMain(HWND hWnd__, UINT msg__, WPARAM lParam__, LPARAM wParam__)
{
	HANDLE hSnap;
	PROCESSENTRY32 pe;
	TCHAR procName[MAX_PATH] = { 0, };

	/// ���� ��� ���°� �ƴϸ� ó������ ����
	if (false == controlManager->getProcessManagementMode())
	{
		return;
	}

	hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == (HANDLE)-1)
	{
		return;
	}

	pe.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(hSnap, &pe))
	{

		do {
			// ���� �ʱ�ȭ
			memset(procName, 0x00, MAX_PATH);
			// ���μ��� ���� �ʱ�ȭ �� ���μ��� �̸� ȹ��
			if (false == initProcessInfo(procName, pe.szExeFile, pe.th32ProcessID))
				continue;

			// ! ���� �� �ý��� ���μ����� �ƴ� ��쿡�� ���μ��� �̸� ȹ�� �� �� ��ġ�� ����

			// �ڱ� ���μ������ ó������ ����
			if (false != controlManager->checkMyProcess(procName))
				continue;

			// ���� ��� ���μ��� ���� Ȯ��
#if(_DEBUG_MODE_)
			// ������ ��� �ڵ� ����
			//if (0 == lstrcmpW(L"taskmgr.exe", procName))
			if (0 == lstrcmpW(L"microsoftedge.exe", procName))
			{
				MessageBox(NULL, L"Find EDGE", L"YES", MB_OK);
				killTargetProcess(pe.th32ProcessID);
			}
#else
			if (false != checkTargetProcess(procName))
			{
				killTargetProcess(pe.th32ProcessID);
			}
#endif			

		} while (Process32Next(hSnap, &pe));

	}

	CloseHandle(hSnap);
	//MessageBox(hWnd__, L"I'm called", L"Hey", MB_OK);
}
