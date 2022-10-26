#pragma once

#include "framework.h"

#include "PasswordController.h"


class ControlManager
{
public:
	// 시험 시작 버튼
	HWND examControlButton;
	HWND examControlButtonEnd;
	HWND examControlButtonLogin;

	// 패스워드 처리 객체
	PasswordController pwdController;

	// 생성자
	ControlManager(wchar_t* fullPath__, int exeCnt__);

	/// 프로그램 종료 모드 설정
	void setProcessTerminateMode();

	/// 프로그램 종료 모드 획득
	bool getProcessTerminateMode();

	/// 자기 프로세스 이름 여부 확인
	bool checkMyProcess(wchar_t *procName__);

	/// 프로세스 재 실행 문자열 생성 및 반환
	bool genExecuteProcessString(wchar_t *buf__, int len__);

	bool getProcessManagementMode();

	void setProcessManagementMode();

	int getExecuteStatus();

private:
	// 종료 플래그 상태
	bool endFlag;
	// 제어 실행 상태
	bool processManagement;

	// 프로세스의 전체 경로
	wchar_t processFullPath[MAX_PATH];
	// 내 프로세스 이름
	wchar_t processName[MAX_PATH];

	// 최초 실행 여부
	int executeCnt;

	// 객체 초기화 기본 함수
	void initControlManager();

	// 문자열 버퍼 최초 초기화
	void initBuffers();
};

