#pragma once

#include "framework.h"

#define PASSWD_REQUEST_CNT		3
#define PASSWD_LENGTH			16
#define PASWD_STRING			L"soda123"


class PasswordController
{
public:
	// ������
	PasswordController();
	// �н����� �ѱ��� �Է�
	void enterPassword(wchar_t char__);
	// �н����� �ʱ�ȭ
	void resetPasswordInput();
	// �н����� �Է� ��� ��û
	bool requestRelease();
	// �Է� �н����� �ѱ��� �����
	void requestBackSpace();
	// �н����� ���� Ȯ��
	bool getStatus();
	// �н����� �Է¸�� Ȯ��
	bool getInputMode();
	// �ӽ÷� �н����� Ȯ���� ���� ���� ȹ��
	void getCurrentPassword(wchar_t *buf__);

private:
	// �н����� �Է� ����
	int pwdCnt;
	// �н����� ����
	int pwdLength;
	// �н����� �Է� ����
	bool pwdFlag;
	// �Է� ����
	bool status;

	// �Էµ� ��й�ȣ
	wchar_t passWord[PASSWD_LENGTH];

	// ���� �ʱ�ȭ
	void resetPasswordInfo();
};

