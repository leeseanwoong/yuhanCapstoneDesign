#include "PasswordController.h"

#include <cctype>


void PasswordController::resetPasswordInfo()
{
	// 각종 값 초기화
	pwdCnt = 0;
	pwdLength = 0;
	pwdFlag = false;
	status = false;

	// 비밀번호 입력 버퍼 초기화
	wmemset(passWord, 0x00, PASSWD_LENGTH);
}


PasswordController::PasswordController()
{
	resetPasswordInfo();
}


void PasswordController::enterPassword(wchar_t char__)
{
	wchar_t input;

	// 비밀번호 입력 모드 확인
	if (false == pwdFlag)
		return;

	if (16 < pwdLength)
	{
		MessageBox(NULL, L"너무 많은 입력이 발생하였습니다.", L"초기화합니다.", MB_OK);
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

	// 4회 이상 입력된 경우, 입력 정보 초기화
	if (PASSWD_REQUEST_CNT < pwdCnt)
	{
		resetPasswordInfo();
		return false;
	}

	// ESC 키 입력 3회 
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
