#include "PasswordController.h"

#include <cctype>


void PasswordController::resetPasswordInfo()
{
	// ���� �� �ʱ�ȭ
	pwdCnt = 0;
	pwdLength = 0;
	pwdFlag = false;
	status = false;

	// ��й�ȣ �Է� ���� �ʱ�ȭ
	wmemset(passWord, 0x00, PASSWD_LENGTH);
}


PasswordController::PasswordController()
{
	resetPasswordInfo();
}


void PasswordController::enterPassword(wchar_t char__)
{
	wchar_t input;

	// ��й�ȣ �Է� ��� Ȯ��
	if (false == pwdFlag)
		return;

	if (16 < pwdLength)
	{
		MessageBox(NULL, L"�ʹ� ���� �Է��� �߻��Ͽ����ϴ�.", L"�ʱ�ȭ�մϴ�.", MB_OK);
		resetPasswordInfo();
		return;
	}

	input = std::tolower(char__);
	passWord[pwdLength++] = input;

	if (0 == lstrcmpW(PASWD_STRING, passWord))
	{
		status = true;
	}
}


void PasswordController::resetPasswordInput()
{
	resetPasswordInfo();
}


void PasswordController::requestBackSpace()
{
	if (false == pwdFlag)
		return;

	pwdLength--;
	if (0 >= pwdLength)
	{
		pwdLength = 0;
		return;
	}

	passWord[pwdLength] = NULL;
}


void PasswordController::getCurrentPassword(wchar_t *buf__)
{
	wmemcpy(buf__, passWord, pwdLength);
}


bool PasswordController::requestRelease()
{
	pwdCnt++;

	// 4ȸ �̻� �Էµ� ���, �Է� ���� �ʱ�ȭ
	if (PASSWD_REQUEST_CNT < pwdCnt)
	{
		resetPasswordInfo();
		return false;
	}

	// ESC Ű �Է� 3ȸ 
	if (PASSWD_REQUEST_CNT == pwdCnt)
	{
		pwdFlag = true;
		return true;
	}

	return false;
}


bool PasswordController::getStatus()
{
	return status;
}


bool PasswordController::getInputMode()
{
	return pwdFlag;
}
