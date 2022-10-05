#pragma once

#include "framework.h"

#define PASSWD_REQUEST_CNT		3
#define PASSWD_LENGTH			16
#define PASWD_STRING			L"soda123"


class PasswordController
{
public:
	// 생성자
	PasswordController();
	// 패스워드 한글자 입력
	void enterPassword(wchar_t char__);
	// 패스워드 초기화
	void resetPasswordInput();
	// 패스워드 입력 모드 요청
	bool requestRelease();
	// 입력 패스워드 한글자 지우기
	void requestBackSpace();
	// 패스워드 상태 확인
	bool getStatus();
	// 패스워드 입력모드 확인
	bool getInputMode();
	// 임시로 패스워드 확인을 위한 정보 획득
	void getCurrentPassword(wchar_t *buf__);

private:
	// 패스워드 입력 개수
	int pwdCnt;
	// 패스워드 길이
	int pwdLength;
	// 패스워드 입력 상태
	bool pwdFlag;
	// 입력 상태
	bool status;

	// 입력된 비밀번호
	wchar_t passWord[PASSWD_LENGTH];

	// 정보 초기화
	void resetPasswordInfo();
};

