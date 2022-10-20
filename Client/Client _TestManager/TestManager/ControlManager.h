#pragma once

#include "framework.h"

#include "PasswordController.h"


class ControlManager
{
public:
	// ���� ���� ��ư
	HWND examControlButton;
	HWND examControlButtonEnd;
	HWND examControlButtonLogin;

	// �н����� ó�� ��ü
	PasswordController pwdController;

	// ������
	ControlManager(wchar_t* fullPath__, int exeCnt__);

	/// ���α׷� ���� ��� ����
	void setProcessTerminateMode();

	/// ���α׷� ���� ��� ȹ��
	bool getProcessTerminateMode();

	/// �ڱ� ���μ��� �̸� ���� Ȯ��
	bool checkMyProcess(wchar_t *procName__);

	/// ���μ��� �� ���� ���ڿ� ���� �� ��ȯ
	bool genExecuteProcessString(wchar_t *buf__, int len__);

	bool getProcessManagementMode();

	void setProcessManagementMode();

	int getExecuteStatus();

private:
	// ���� �÷��� ����
	bool endFlag;
	// ���� ���� ����
	bool processManagement;

	// ���μ����� ��ü ���
	wchar_t processFullPath[MAX_PATH];
	// �� ���μ��� �̸�
	wchar_t processName[MAX_PATH];

	// ���� ���� ����
	int executeCnt;

	// ��ü �ʱ�ȭ �⺻ �Լ�
	void initControlManager();

	// ���ڿ� ���� ���� �ʱ�ȭ
	void initBuffers();
};

