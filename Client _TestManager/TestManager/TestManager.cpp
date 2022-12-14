// TestManager.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32") // 소켓을 사용하기 위해서 라이브러리 참조해야 한다.
#pragma warning(disable:4996) // inet_ntoa가 deprecated가 되었는데.. 사용하려면 아래 설정을 해야 한다.
#include "framework.h"
#include "TestManager.h"
#include "ProcessManager.h"
#include "ControlManager.h"
#include <TlHelp32.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <thread>
#include <WinSock2.h> // 소켓을 사용하기 위한 라이브러리
#include <string>
#include <cctype>
#include"SHA256/KISA_SHA256.h" //파일 무결성 검사
#include"zlib/Zipper.h" //파일 압축
#include"LiteUnzip/LiteUnzip.h" //비밀번호 걸린 압축파일 해제
#include <direct.h>
#include <atlstr.h>

#define BUFFERSIZE 128 // 수신 버퍼 사이즈
using namespace std;

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

/// 주요 인스턴스 생성
ControlManager* g_controlManager;
ProcessManager* g_processManager;

vector<wstring> g_processList = {
	L"chrome.exe",
	L"firefox.exe",
	L"kakaotalk.exe",
	L"microsoftedge.exe",
	L"msedge.exe",
	L"iexplore.exe",
	L"mspaint.exe",
	L"notepad.exe",
	L"nateonmain.exe",
	L"cmd.exe",
	L"powershell.exe",
	L"taskmgr.exe"
};

//블랙리스트 추가 함수
// 함수에 인수 전달시 wstring말고도 char, wchar_t로 전달해도 됨!!! 
/* 예시
*
* char str[100];
* wchar_t str2[100];
* addBlackList(str, hWnd);	// 가능
* addBlackList(str2, hWnd);	// 가능
*
*/
void addBlackList(wstring name)
{
	// 블랙 리스트에 있는지 비교
	for (int i = 0; i < g_processList.size(); i++)
	{
		// lstrcmpW는 대소문자 구분 함 "chrome.exe"는 프로그램 죽음 하지만 "Chrome.exe"는 프로그램 안죽음
		// 프로그램 이름을 정확히 적을 필요가 있음
		if (0 == lstrcmpW(g_processList[i].c_str(), name.c_str()))
		{
			// 이미 블랙리스트에 있을 시 메시지 박스 띄움
			// 메시지 박스 띄워주는 것은 클라이언트 말고 서버쪽에 보여줘야 함(수정 필요)
			MessageBox(NULL, L"이미 블랙 리스트에 들어가 있습니다.", L"안내", MB_OK);
			return;
		}
	}
	//블랙리스트에 넣기
	g_processList.push_back(name);
}
int DeleteAllFiles(LPCWSTR szDir, int recur)
{
	HANDLE hSrch;
	WIN32_FIND_DATA wfd;
	int res = 1;

	TCHAR DelPath[MAX_PATH];
	TCHAR FullPath[MAX_PATH];
	TCHAR TempPath[MAX_PATH];

	lstrcpy(DelPath, szDir);
	lstrcpy(TempPath, szDir);
	if (lstrcmp(DelPath + lstrlen(DelPath) - 4, _T("\\*.*")) != 0) {
		lstrcat(DelPath, _T("\\*.*"));
	}

	hSrch = FindFirstFile(DelPath, &wfd);
	if (hSrch == INVALID_HANDLE_VALUE) {
		if (recur > 0) RemoveDirectory(TempPath);
		return -1;
	}

	while (res) {
		wsprintf(FullPath, _T("%s\\%s"), TempPath, wfd.cFileName);

		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_READONLY) {
			SetFileAttributes(FullPath, FILE_ATTRIBUTE_NORMAL);
		}

		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if (lstrcmp(wfd.cFileName, _T("."))
				&& lstrcmp(wfd.cFileName, _T(".."))) {
				recur++;
				DeleteAllFiles(FullPath, recur);
				recur--;
			}
		}
		else {
			DeleteFile(FullPath);
		}

		res = FindNextFile(hSrch, &wfd);
	}

	FindClose(hSrch);

	if (recur > 0) RemoveDirectory(TempPath);

	return 0;
}

//블랙리스트 제거 함수
void removeBlackList(wstring name, HWND hWnd)
{
	// 대소문자 구분 함, 블랙 리스트 비교
	for (int i = 0; i < g_processList.size(); i++)
	{
		if (0 == lstrcmpW(g_processList[i].c_str(), name.c_str()))
		{
			// 블랙리스트에 있을 시 제거
			g_processList.erase(g_processList.begin() + i);
			// 제거 완료 메시지
			// 메시지 박스 띄워주는 것은 클라이언트 말고 서버쪽에 보여줘야 함(수정 필요)
			MessageBox(hWnd, L"블랙리스트에서 제거 됐습니다.", L"안내", MB_OK);
			return;
		}
	}
	// 대소문자 구분 안 함, 블랙리스트 비교
	for (int i = 0; i < g_processList.size(); i++)
	{
		// lstrcmpi는 대소문자 구분 안 함
		if (0 == lstrcmpi(g_processList[i].c_str(), name.c_str()))
		{
			// 프로그램 이름을 정확히 입력하지 않았을 때
			// 메시지 박스 띄워주는 것은 클라이언트 말고 서버쪽에 보여줘야 함(수정 필요)
			MessageBox(hWnd, L"프로그램 이름을 정확히 입력하세요.(대소문자 구분 필수)", L"안내", MB_OK);
			return;
		}
	}

	{
		// 메시지 박스 띄워주는 것은 클라이언트 말고 서버쪽에 보여줘야 함(수정 필요)
		MessageBox(hWnd, L"블랙리스트에 존재하지 않습니다.", L"안내", MB_OK);
	}

}


// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	LPWSTR myProcess;
	int killCnt;

	/// 시작 인수의 값을 문자열에서 숫자로 변환하여 획득
	killCnt = _wtoi(lpCmdLine);
	/// 프로세스 전체 경로 획득
	myProcess = GetCommandLine();

	/// 제어 객체 생성
	g_controlManager = new ControlManager(myProcess, killCnt);
	/// 프로세스 객체 생성
	g_processManager = new ProcessManager(g_controlManager, &g_processList);

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_TESTMANAGER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TESTMANAGER));

	MSG msg;

	// 기본 메시지 루프입니다:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.6
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTMANAGER));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TESTMANAGER);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX,
		500, 50, 660, 200, nullptr, nullptr, hInstance, nullptr);
	//CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

// 시험 시작 버튼 ID
#define EXAM_BUTTON_ID					1
//로그인 버튼 ID
#define LOGIN_BUTTON_ID					2
//학번 입력창 ID
#define HAK_EDIT_ID						3
//이름 입력창 ID
#define NAME_EDIT_ID					4
// 시험 종료 버튼 ID
#define EXAM_END_BUTTON_ID				5
// 로그인 정보 수정 버튼
#define EXAM_LOGIN_BUTTON_ID			6

#define IP_EDIT_ID						7

#define PORT_EDIT_ID					8
#define NOW_HAK_EDIT_ID					9
#define NOW_NAME_EDIT_ID				10

#define TIMER_ID_PROCESS_MANAGEMENT		1
#define TIMER_ID_TIME_DISPLAY			2
#define TIMER_ID_CHECK_STATUS			3

#define INTERVAL_PROCESS_MANAGEMENT		1000
#define INTERVAL_TIME_DISPLAY			1000
#define INTERVAL_CHECK_STATUS			500


int g_lbcnt = 0;
int g_rbcnt = 0;
int g_timeCnt = 0;
bool login = false;							//로그인 하기 전(false)
bool examStatus = false;					//시험 시작 전(false)
wchar_t Userinfo[50] = L"8#"; // 현재 사용자의 학번과 이름이 들어가는 공간
wchar_t hak_buffer[20];						//처음에 학번 입력할때 나오는 변수
wchar_t name_buffer[20];					//처음에 이름 입력할때 나오는 변수
wchar_t ip_buffer[20];						//처음에 아이피 입력할때 나오는 변수
wchar_t port_buffer[20];					//처음에 포트 입력할때 나오는 변수
LPCWSTR status;
char* ExamInfo[3];
wchar_t now_hak_text[20];	// 가장 최신의 학번 저장
wchar_t now_name_text[20];	// 가장 최신의 이름 저장
int Examstart = 0;
wchar_t hak_text[20];	// 수정하려고하는 학번 저장
wchar_t name_text[20];	// 수정하려고 하는 이름 저장

unsigned char m_cmd;
int cmd_BlackListAdd = 1;
int cmd_FileIn = 2;
int cmd_SetExam = 3;
int cmd_ExamStart = 4;
int cmd_ExamEnd = 5;

char* ip_str;
int port_str;
static char lock = 0;

char cMsg[1024] = ""; // 메시지 버퍼
char cBuffer[BUFFERSIZE] = {};
int i = 0;

SOCKET sock;
HWND hEdit_hak, hEdit_name, hButton_login, hEdit_ip, hEdit_port, hEdit_now_hak, hEdit_now_name;

//암호걸린 압축파일 해제
void unzip() {


	//암호 압축폴더 해제
	HUNZIP   huz;
	ZIPENTRY ze;
	int numitems;
	UnzipOpenFileW(&huz, L"C:\\Users\\YUHAN\\Desktop\\ExamFile.zip", "1234"); //2번 : 압축파일 경로,3번 : 비밀번호
	UnzipSetBaseDirW(huz, L"C:\\Users\\YUHAN\\Desktop\\ExamFile"); //압축 해제할 장소
	ze.Index = -1;
	UnzipGetItem(huz, &ze);
	numitems = ze.Index;
	for (ze.Index = 0; ze.Index < numitems; ze.Index++)
	{
		UnzipGetItemW(huz, &ze);
		UnzipItemToFileW(huz, ze.Name, &ze);
	}
	UnzipClose(huz);
}
void zip() {
	bool CreateZip;
	CreateZip = CZipper::ZipFolder("C:\\Users\\YUHAN\\Desktop\\ExamFile", FALSE); //해당 폴더를 압축시킴 - ziptest폴더도 같이 압축됨, 압축 후 상위(work)폴더에 압축파일 생성
}
int checksha(SOCKET sock) {
	FILE* fp = NULL;
	unsigned char buffer2[2048] = { 0, };
	unsigned char result[32] = { 0, };
	wchar_t code[68] = { 0, };
	int read = 0;
	int loop_number = 0;
	//SHA256 변수 초기화.
	SHA256_INFO sha256_info;
	SHA256_Init(&sha256_info);
	//파일 읽기
	fopen_s(&fp, "C:\\Users\\YUHAN\\Desktop\\ExamFile.zip", "rb");
	if (fp == NULL)
	{
		printf("Error : File not find.\n");
	}
	while ((read = fread(buffer2, 1, 2048, fp)) != 0)
	{
		SHA256_Process(&sha256_info, buffer2, read);
	}
	SHA256_Close(&sha256_info, result);
	for (loop_number = 0; loop_number < 32; loop_number++)
	{
		wsprintf(code + lstrlen(code), L"%02x", result[loop_number]);
		//printf("%02x", result[loop_number]);

	}
	
	//MessageBox(NULL, code, code, MB_OK); //sha256값을 메시지 박스로 출력

	send(sock, (char*)code, (wcslen(code) * 2), 0); // hash값 보내기
	unsigned char ret[1];
	recv(sock, (char*)ret, 1, 0);
	if (ret[0] != 3) {
		return 1;
	}
	fclose(fp);
	return 0;
}

/*void recvExamData(SOCKET clientSocket) {
	char cBuffer[BUFFERSIZE] = {};
	recv(clientSocket, cBuffer, BUFFERSIZE, 0);
	LPCWSTR e = (LPCWSTR)cBuffer;
	char* resultStr = new char[wcslen(e) + 1];
	wsprintfA(resultStr, "%S", e);

	char* a = strtok(resultStr, "#");
	int i = 0;
	while (a != NULL)              //ptr이 NULL일때까지 (= strtok 함수가 NULL을 반환할때까지)
	{
		if (i == 3)
			break;
		ExamInfo[i] = a;
		a = strtok(NULL, "#"); // 자른 문자 다음부터 구분자 또 찾기
		i++;
	}
}

void recvBlackList(SOCKET clientSocket) {
	wchar_t Buffer[1024] = L"";
	recv(clientSocket, (char*)Buffer, 1024, 0);

	wchar_t* a = wcstok(Buffer, L"#");
	while (a != NULL)              //ptr이 NULL일때까지 (= strtok 함수가 NULL을 반환할때까지)
	{
		addBlackList(a); // 서버에서 넘어온 블랙리스트 추가시키기
		a = wcstok(NULL, L"#"); // 자른 문자 다음부터 구분자 또 찾기
	}
}*/

SYSTEMTIME g_passTime;
void compulsionQuit(HWND hWnd, wchar_t* hak_buffer, wchar_t* name_buffer);

void recvFile(SOCKET sock);
char* ConvertWCtoC(wchar_t* str) // wchar_t에서 char로 변경
{
	//반환할 char* 변수 선언
	char* pStr;
	//입력받은 wchar_t 변수의 길이를 구함
	int strSize = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	//char* 메모리 할당
	pStr = new char[strSize];
	//형 변환 
	WideCharToMultiByte(CP_ACP, 0, str, -1, pStr, strSize, 0, 0);
	return pStr;
}
void passTimeCalc(wchar_t* str__)
{
	int hour = 0;
	int min = 0;
	int sec = 0;

	min = g_timeCnt / 60;
	hour = min / 60;
	sec = g_timeCnt % 60;
	min = min % 60;

	wsprintf(str__, L"%02d:%02d:%02d", hour, min, sec);
}

void firstsend(wchar_t* userinfo, SOCKET sock) // 사용자 정보 보낼때
{
	send(sock, (char*)userinfo, (wcslen(userinfo) * 2), 0);
	char cBuffer[BUFFERSIZE] = {};
	recv(sock, cBuffer, BUFFERSIZE, 0);
}

void Insert(int idx, char ch)
{
	memmove(cMsg + idx + 1, cMsg + idx, strlen(cMsg) - idx + 1);
	cMsg[idx] = ch;
}

void Delete(int idx)
{
	memmove(cBuffer + idx, cBuffer + idx + 1, strlen(cBuffer) - idx);
}

void DestroyFirstWindow() {// 첫 로그인 화면 삭제

	DestroyWindow(hButton_login);
	DestroyWindow(hEdit_hak);
	DestroyWindow(hEdit_name);
	DestroyWindow(hEdit_ip);
	DestroyWindow(hEdit_port);
}

int conServer(char* ip_str, int port_str) { // 서버 연결
	WSADATA wsaData1; // 소켓 정보 데이터 설정

	if (WSAStartup(MAKEWORD(2, 2), &wsaData1) != 0) // 소켓 실행.
	{
		return 1;
	}

	sock = socket(PF_INET, SOCK_STREAM, 0); // Internet의 Stream 방식으로 소켓 생성 

	SOCKADDR_IN addr = { 0, }; // 소켓 주소 설정

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET; // 소켓은 Internet 타입
	addr.sin_addr.s_addr = inet_addr(ip_str); // 입력한 ip로 접속
	addr.sin_port = htons(port_str); // 입력한 포트로 접속

	if (connect(sock, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) // 접속
	{
		cout << "error" << endl; // 에러 콘솔 출력
		return 1;
	}
	return 0;
}

HANDLE hdl_server;

void StartExam(HWND hWnd) {
	int r = 0;
	examStatus = true;
	status = L"시험을 시작합니다.";
	Examstart = 1;
	unzip();
	r = remove("C:\\Users\\YUHAN\\Desktop\\ExamFile.zip"); //받은 압축 파일 삭제
	InvalidateRect(hWnd, NULL, TRUE);
}

DWORD WINAPI CreateServer(LPVOID param)
//int CreateServer() { // 파일 받아올수 있는 서버 만들기
{
	WSADATA wsaData2;
	SOCKET MySocket, ServerSocket;
	SOCKADDR_IN serverAddress = {};

	int serverPort = 9001;
	int len = sizeof(SOCKADDR_IN);

	if (WSAStartup(MAKEWORD(2, 2), &wsaData2) != 0) // Winsock을초기화합니다.
		return 1;

	MySocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); // TCP 소켓을생성합니다.

	if (MySocket == INVALID_SOCKET)
		return 1;

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); // 4바이트정수를네트워크바이트형식으로
	serverAddress.sin_port = htons(serverPort); // 2바이트정수네트워크바이트형식으로

	bind(MySocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress));
	listen(MySocket, 1);


	ServerSocket = accept(MySocket, (sockaddr*)&serverAddress, &len);

	if (ServerSocket == INVALID_SOCKET) {

		MessageBox(NULL, L"연결에러", L"", MB_OK);
		return 1;
	}
	else
		MessageBox(NULL, L"서버와 연결 완료", L"", MB_OK);
	while (1) { // 클라이언트의메시지를받아서그대로다시전달합니다.
		unsigned char Protocol[1];
		if (recv(ServerSocket, (char*)Protocol, 1, 0) == INVALID_SOCKET) {
			break;
		}
		m_cmd = Protocol[0];
		if (m_cmd == cmd_FileIn)
		{
			recvFile(ServerSocket);
		}
		/*else if (m_cmd == cmd_BlackListAdd)
		{
			recvBlackList(ServerSocket);
		}
		else if (m_cmd == cmd_SetExam)
		{
			recvExamData(ServerSocket);
		}*/
		else if (m_cmd == cmd_ExamStart)
		{
			StartExam((HWND)param);
			// 타이머가 가게 하면됨.
		}
		else if (m_cmd == cmd_ExamEnd)
		{
			compulsionQuit((HWND)param, hak_buffer, name_buffer);
			break;
		}
	}
	return 0;
}

void Setblank() { // 초기 로그인 화면 공백으로 변경
	SetWindowText(hEdit_ip, L"");
	SetWindowText(hEdit_port, L"");
	SetWindowText(hEdit_name, L"");
	SetWindowText(hEdit_hak, L"");
}

void SetStr(wchar_t* str, const wchar_t* ip_buffer, const wchar_t* port_buffer, const wchar_t* hak_buffer, const wchar_t* name_buffer) {
	// 로그인 누를때 한번더 확인할때 나오는 문자 정의
	wcscpy(now_hak_text, hak_buffer);
	wcscpy(now_name_text, name_buffer);

	wchar_t ip[] = L"IP : "; // 로그인 후 아이피이 들어가는 곳에  " IP : " 만 
	wchar_t port[] = L"Port : "; // 로그인 후 포트이 들어가는 곳에  " Port : " 만 
	wchar_t name[] = L"이름 : "; // 로그인 후 이름이 들어가는 곳에  " 이름 : " 만 
	wchar_t hak[] = L"학번 : ";// 로그인 후 학번이 들어가는 곳에  " 학번 : " 만 
	wchar_t enter[] = L"\n"; // 줄바꿈

	wcscat(str, ip); wcscat(str, ip_buffer);
	wcscat(str, enter);
	wcscat(str, port); wcscat(str, port_buffer);
	wcscat(str, enter);
	wcscat(str, hak); wcscat(str, hak_buffer);
	wcscat(str, enter);
	wcscat(str, name); wcscat(str, name_buffer);
}
void SetStr2(wchar_t* str, const wchar_t* hak_buffer, const wchar_t* name_buffer) {

	wchar_t name[] = L"이름 : "; // 로그인 후 이름이 들어가는 곳에  " 이름 : " 만 
	wchar_t hak[] = L"학번 : ";// 로그인 후 학번이 들어가는 곳에  " 학번 : " 만 
	wchar_t enter[] = L"\n"; // 줄바꿈

	wcscat(str, hak); wcscat(str, hak_buffer);
	wcscat(str, enter);
	wcscat(str, name); wcscat(str, name_buffer);
}

void SetMyInfo(wchar_t* Userinfo, const wchar_t* hak_buffer, const wchar_t* name_buffer) { // 처음 보내는 학번과 이름 정의
	wcscat(Userinfo, hak_buffer);
	wcscat(Userinfo, L"#");
	wcscat(Userinfo, name_buffer);
}

int SendFile(HWND hWnd) { // 서버로 파일 전송
	zip();
	FILE* fp = fopen("C:\\Users\\YUHAN\\Desktop\\ExamFile.zip", "rb"); // 현재 보낼 파일이 있는 위치 및 파일 이름
	int r = 0;
	if (fp == NULL) // 취득 실패하면 종료한다.
	{
		cout << "File open failed" << endl;
		return 1;
	}
	// 파일 길이 취득
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	// 파일 데이터를 읽어온다.
	unsigned char* data = new unsigned char[size];
	fread(data, 1, size, fp);
	// 파일 리소스 닫기
	fclose(fp);
	// 파일 사이즈 전송
	send(sock, (char*)&size, 4, 0);
	// 파일 데이터 전송
	send(sock, (char*)data, size, 0);
	// 데이터 메모리 해제
	delete data;

	unsigned char ret[1];
	recv(sock, (char*)ret, 1, 0);
	if (ret[0] == 1) // 서버쪽에서 파일전송이 완료되면 byte=1을 보내는데 1이 오면 일단 종료되게 해놓음
	{
		MessageBox(hWnd, L"파일 전송이 완료 되었습니다", L"파일 전송", MB_OK);
	}
	r = remove("C:\\Users\\YUHAN\\Desktop\\ExamFile.zip");//전송완료한 파일 삭제
	DeleteAllFiles(_T("C:\\Users\\YUHAN\\Desktop\\ExamFile"), 1);
	return 0;
}
void OutputFileNameAdd(wchar_t* OutputFileName, wchar_t* hak_buffer, wchar_t* name_buffer) {
	// 서버에게 파일전송할 때에 이름 정의
	wcscat(OutputFileName, hak_buffer);
	wcscat(OutputFileName, L"_");
	wcscat(OutputFileName, name_buffer);
}

void ExamQuit(HWND hWnd, wchar_t* hak_buffer, wchar_t* name_buffer) {
	wchar_t OutputFileName[500] = L"6#"; // 서버에 보낼 프로토콜과 파일명이 들어가는 공간
	wchar_t CheckUserInfo[500] = L"";
	int CheckInfo;
	OutputFileNameAdd(CheckUserInfo, hak_buffer, name_buffer);
	OutputFileNameAdd(OutputFileName, hak_buffer, name_buffer); // 보낼 문자열에 학번과 이름 추가하기.

	if (CheckInfo = MessageBox(hWnd, (LPCWSTR)CheckUserInfo, L"!!학번이름확인!!", MB_YESNO) == IDYES) {
		if (CheckInfo = MessageBox(hWnd, (LPCWSTR)CheckUserInfo, L"!!학번이름재확인!!", MB_YESNO) == IDYES) {
			send(sock, (char*)OutputFileName, (wcslen(OutputFileName) * 2), 0); // 파일 보내기 
			if (1 == SendFile(hWnd)) { // 파일전송에 실패하면
				MessageBox(hWnd, L"파일전송실패", L"경고", MB_OK);
			}
			else {
				send(sock, (char*)L"9#", 6, 0);
				closesocket(sock); // 서버 소켓 종료
				WSACleanup(); // 소켓 종료
				PostQuitMessage(0); // 폼종료
			}
		}
	}
}

void compulsionQuit(HWND hWnd, wchar_t* hak_buffer, wchar_t* name_buffer) {
	wchar_t OutputFileName[500] = L"6#"; // 서버에 보낼 프로토콜과 파일명이 들어가는 공간
	wchar_t CheckUserInfo[500] = L"";
	OutputFileNameAdd(CheckUserInfo, hak_buffer, name_buffer);
	OutputFileNameAdd(OutputFileName, hak_buffer, name_buffer); // 보낼 문자열에 학번과 이름 추가하기.
	send(sock, (char*)OutputFileName, (wcslen(OutputFileName) * 2), 0); // 파일 보내기 
	if (1 == SendFile(NULL)) { // 파일전송에 실패하면
		MessageBox(NULL, L"파일전송실패", L"경고", MB_OK);
	}
	else {
		send(sock, (char*)L"9#", 6, 0);
		closesocket(sock); // 서버 소켓 종료
		WSACleanup(); // 소켓 종료
		delete g_controlManager;
		delete g_processManager;
		CloseHandle(hdl_server);
		exit(1);
	}
}

void recvFile(SOCKET sock) {

	wchar_t buffer[BUFFERSIZE] = L"C:\\Users\\YUHAN\\Desktop\\";
	// 저장할 파일명 변수
	wchar_t filename[BUFFERSIZE];

	int size = 0;
	int r = 0;
	// 처음에는 데이터의 사이즈가 온다. C++과 C#은 기본적으로 빅 엔디안이기 때문에 엔디안 변환은 필요없다.
	// char*를 4바이트로 받아버리면 int형이 된다.
	if (recv(sock, (char*)&size, 4, 0) == SOCKET_ERROR)
	{
		cout << "error" << endl;
		return;
	}
	// 데이터를 unsigned char형식으로 받는다. =byte 
	MessageBox(NULL, L"시험파일 수신중..", L"", MB_OK);
	unsigned char* buffer1 = new unsigned char[size]; // 문제

	if (recv(sock, (char*)buffer1, size, 0) == SOCKET_ERROR)
	{
		cout << "error" << endl;
		return;
	}
	unsigned char* data = buffer1;

	// 첫번째는 파일명이다. 이번에는 C#에서 unicode식이 아닌 utf8형식으로 파일명을 보냈다.
	// c++에서는 unicode를 다루기 때문에 변환이 필요하다. (MB_ERR_INVALID_CHARS)
	MultiByteToWideChar(CP_UTF8, 0, (const char*)data, size, filename, BUFFERSIZE);

	delete data;
	// 디렉토리 + 파일명
	wcscat(buffer, filename);

	// 저장할 파일 객체를 받는다.
	FILE* fp = _wfopen(buffer, L"wb");

	// 데이터를 다시 받는다. 이번엔 업로드하는 파일 데이터이다.

	// 처음에는 데이터의 사이즈가 온다. C++과 C#은 기본적으로 빅 엔디안이기 때문에 엔디안 변환은 필요없다.
	// char*를 4바이트로 받아버리면 int형이 된다.
	if (recv(sock, (char*)&size, 4, 0) == SOCKET_ERROR)
	{
		cout << "error" << endl;
		return;
	}
	// 데이터를 unsigned char형식으로 받는다. =byte 
	buffer1 = new unsigned char[4096]; // 문제
	int readsize = 4096;
	while (size > 0) {
		if (recv(sock, (char*)buffer1, readsize, 0) == -1) {
			MessageBox(NULL, L"rev", L"", MB_OK);
		}

		size -= readsize;
		if (fp != NULL)
		{
			// 파일 저장  
			fwrite(buffer1, 1, readsize, fp);
		}
		else
		{
			// 파일 객체 취득에 실패할 경우 콘솔 에러 표시
			cout << "File open failed" << endl;
		}

		if (size <= 4096) {
			readsize = size;
		}

	}readsize = 4096;
	// 파일 닫기
	fclose(fp);

	// 송신 데이터 선언 byte=1를 보내면 송신 완료.
	int Secc;
	if ((Secc = checksha(sock)) == 1) {
		return;
	}

	char ret[1] = { 1 };
	// 클라이언트로 완료 패킷을 보낸다.
	send(sock, ret, 1, 0); MessageBox(NULL, L"파일 수신 완료", L"", MB_OK);

	return;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	TCHAR str[MAX_PATH] = { 0, };
	int wmId = LOWORD(wParam);
	wchar_t Checkstr[200] = L""; // 로그인을 확인할 때에 사용하는 글자들이 들어가는 공간
	wchar_t Checkstr2[200] = L""; // 로그인을 확인할 때에 사용하는 글자들이 들어가는 공간


	switch (message)
	{
	case WM_TIMER:
		switch (wParam)
		{
		case TIMER_ID_TIME_DISPLAY:
			SYSTEMTIME st;

			if (Examstart == 1) {
				g_timeCnt++;
			}
			GetLocalTime(&st);

			hdc = GetDC(hWnd);

			HFONT font, oldfont;
			font = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, L"맑은 굴림");

			oldfont = (HFONT)SelectObject(hdc, font);

			if (false != g_controlManager->getProcessManagementMode())
			{
				passTimeCalc(str);

				//MessageBox(hWnd, str, str, MB_OK);
				//wsprintf(str, L"TIME");
				TextOut(hdc, 400, 70, str, lstrlenW(str));
				memset(str, 0x00, MAX_PATH);
			}

			wsprintf(str, L"%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);


			TextOut(hdc, 400, 10, str, lstrlenW(str));

			SelectObject(hdc, oldfont);

			ReleaseDC(hWnd, hdc);

			DeleteObject(font);
			break;

		case TIMER_ID_CHECK_STATUS:
			// 프로그램 종료 상태 여부 확인
			if (false != g_controlManager->getProcessTerminateMode())
			{
				SetWindowTextW(hWnd, L"프로그램 종료가 가능합니다... ★★★★★");
				break;
			}
			// 시험 시작 전이라면 별도 처리 없음
			if (false == g_controlManager->getProcessManagementMode())
				break;

			// 비밀번호 입력 모드 확인 및 처리
			if (false == g_controlManager->pwdController.getInputMode())
				SetWindowTextW(hWnd, L"시험을 시작하였습니다...");
			else
				SetWindowTextW(hWnd, L"시험을 시작하였습니다..");

			break;

		default:
			break;
		}
		break;
	case EN_CHANGE:
	case WM_CREATE:

		//로그인 버튼 생성
		hButton_login = CreateWindow(L"button", L"로그인", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			200,
			80,
			150,
			50,
			hWnd,
			(HMENU)LOGIN_BUTTON_ID,
			hInst,
			NULL);
		hEdit_now_hak = CreateWindow(L"edit", now_hak_text, WS_CHILD | WS_BORDER | ES_AUTOHSCROLL | ES_NUMBER,
			60,
			35,
			100,
			20,
			hWnd,
			(HMENU)NOW_HAK_EDIT_ID,
			hInst,
			NULL);

		hEdit_now_name = CreateWindow(L"edit", now_name_text, WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
			60,
			58,
			100,
			20,
			hWnd,
			(HMENU)NOW_NAME_EDIT_ID,
			hInst,
			NULL);
		
		//학번 입력칸 생성
		hEdit_hak = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_NUMBER,
			60,
			40,
			100,
			20,
			hWnd,
			(HMENU)HAK_EDIT_ID,
			hInst,
			NULL);

		//이름 입력칸 생성
		hEdit_name = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			250,
			40,
			100,
			20,
			hWnd,
			(HMENU)NAME_EDIT_ID,
			hInst,
			NULL);
		// 시험 종료 버튼 생성
		g_controlManager->examControlButtonEnd = CreateWindow(
			L"button", L"시험 종료",
			WS_CHILD | BS_PUSHBUTTON,
			10,
			80,
			150,
			50,
			hWnd,
			(HMENU)EXAM_END_BUTTON_ID,
			hInst,
			NULL);

		// 로그인 정보 수정 버튼 생성
		g_controlManager->examControlButtonLogin = CreateWindow(
			L"button", L"로그인 정보 수정",
			WS_CHILD | BS_PUSHBUTTON,
			20,
			80,
			150,
			50,
			hWnd,
			(HMENU)EXAM_LOGIN_BUTTON_ID,
			hInst,
			NULL);

		//IP 입력칸 생성
		hEdit_ip = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			60,
			10,
			100,
			20,
			hWnd,
			(HMENU)IP_EDIT_ID,
			hInst,
			NULL);

		//Port 입력칸 생성
		hEdit_port = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_NUMBER,
			250,
			10,
			100,
			20,
			hWnd,
			(HMENU)PORT_EDIT_ID,
			hInst,
			NULL);
		SetWindowText(hEdit_port, L"9000");
		SetWindowTextW(hWnd, L"시험 시작 전입니다...");

		SetTimer(hWnd, TIMER_ID_PROCESS_MANAGEMENT, INTERVAL_PROCESS_MANAGEMENT, (TIMERPROC) & (g_processManager->checkProcessMain));
		SetTimer(hWnd, TIMER_ID_TIME_DISPLAY, INTERVAL_TIME_DISPLAY, NULL);
		SetTimer(hWnd, TIMER_ID_CHECK_STATUS, INTERVAL_CHECK_STATUS, NULL);
		SendMessage(hWnd, WM_TIMER, TIMER_ID_PROCESS_MANAGEMENT, NULL);

		//SetWindowPos(hWnd, HWND_TOP, 500, 50, 850, 200, SWP_NOSIZE);

		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			if (false != g_controlManager->pwdController.requestRelease())
			{
				break;
			}
			break;

		case VK_BACK:
			g_controlManager->pwdController.requestBackSpace();
			break;

		default:
			g_controlManager->pwdController.enterPassword(wParam);
			break;
		}

		if (false != g_controlManager->pwdController.getStatus())
		{
			MessageBox(hWnd, L"입력 완료", L"수고하셨습니다.", MB_OK);
			g_controlManager->setProcessTerminateMode();
		}

		break;

	case WM_MOUSEMOVE:
		g_lbcnt = 0;
		g_rbcnt = 0;
		break;

	case WM_LBUTTONDOWN:
		g_lbcnt++;

		if (1 == g_rbcnt)
		{
			/// 프로세스를 종료 가능 상태로 변경
			g_controlManager->setProcessTerminateMode();
		}
		break;

	case WM_RBUTTONDOWN:
		if (3 != g_lbcnt)
			break;

		g_rbcnt++;
		break;

	case WM_COMMAND:
	{
		// 메뉴 선택을 구문 분석합니다:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case EXAM_BUTTON_ID:

			break;
		case EXAM_END_BUTTON_ID:
			// 시험 종료버튼 눌렀을 때 실시
			int EXAM_END_MB_ID;
			if (EXAM_END_MB_ID = MessageBox(hWnd, L"시험을 종료 하시겠습니까?", L"!!필수 확인!!", MB_YESNO) == IDYES) {
				//확인버튼 눌렀을때 파일 전송후 연결 해제
				ExamQuit(hWnd, hak_buffer, name_buffer);
				CloseHandle(hdl_server);
				exit(1);
			}
			break;
		case EXAM_LOGIN_BUTTON_ID:
		{// 로그인 정보 수정 버튼 눌렀을 때 실시
			GetWindowText(hEdit_now_hak, hak_text, 20);
			GetWindowText(hEdit_now_name, name_text, 20);
			// 로그인 정보 수정 버튼 눌렀을 때 실시
			SetStr2(Checkstr2, hak_text, name_text);
			int EXAM_EDIT_MB_ID;
			if (EXAM_EDIT_MB_ID = MessageBox(hWnd, Checkstr2, L"아래와 같이 정보를 변경하시겠습니까? ^^", MB_YESNO) == IDYES) {
				SetWindowText(hEdit_now_hak, hak_text);
				SetWindowText(hEdit_now_name, name_text);
				// 문자열 저장
				wcscpy(now_hak_text, hak_text);
				wcscpy(now_name_text, name_text);
				wchar_t ChangeInfo[50] = L"5#";
				wcscat(ChangeInfo, now_hak_text);
				wcscat(ChangeInfo, L"#");
				wcscat(ChangeInfo, now_name_text);
				send(sock, (char*)ChangeInfo, (wcslen(ChangeInfo) * 2), 0); // 프로토콜5번 보내기 
			}
			else
			{
				SetWindowText(hEdit_now_hak, now_hak_text);
				SetWindowText(hEdit_now_name, now_name_text);
			}
			///////////////////

			break;
		}
		case LOGIN_BUTTON_ID:
			//로그인 버튼 눌렸을 때 실시	
			GetDlgItemText(hWnd, IP_EDIT_ID, (LPWSTR)ip_buffer, 20);
			GetDlgItemText(hWnd, PORT_EDIT_ID, (LPWSTR)port_buffer, 10);
			GetDlgItemText(hWnd, NAME_EDIT_ID, (LPWSTR)name_buffer, 20);
			GetDlgItemText(hWnd, HAK_EDIT_ID, (LPWSTR)hak_buffer, 20);


			SetStr(Checkstr, ip_buffer, port_buffer, hak_buffer, name_buffer);
			SetWindowText(hEdit_now_hak, now_hak_text);
			SetWindowText(hEdit_now_name, now_name_text);
			int LOGIN_MB_ID;
			LOGIN_MB_ID = MessageBox(hWnd, (LPCWSTR)Checkstr, L"!!필수 확인!!", MB_YESNO);
			if (LOGIN_MB_ID == IDYES)
			{
				if (wcslen(ip_buffer) == 0 || wcslen(port_buffer) == 0 || wcslen(name_buffer) == 0 || wcslen(port_buffer) == 0) { // 아무것도 입력하지 않았을때에 처리
					MessageBox(hWnd, L"입력하지 않은 항목이 있습니다 다시 입력해주세요", L"경고", MB_OK);
					break;
				}
				login = true;
				int Connect = 0;
				port_str = stoi(port_buffer); // 문자 port를 int로 변환하는것
				ip_str = ConvertWCtoC(ip_buffer);
				if (Connect == conServer(ip_str, port_str)) { // 서버와 연결하는중
					hdl_server = CreateThread(NULL, 0, CreateServer, hWnd, 0, NULL);
					SetMyInfo(Userinfo, hak_buffer, name_buffer);
					firstsend(Userinfo, sock);
					DestroyFirstWindow();
				}
				else {
					MessageBox(hWnd, L"서버 연결에 실패했습니다.", L"", MB_OK); // 서버 연결에 실패 하였을때.
					Setblank();
					break;
				}
			}
			else if (LOGIN_MB_ID == IDNO) {
				// 본인 정보 확인시 아니요 누르면 
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case HAK_EDIT_ID:
			//학번 입력창
			GetDlgItemText(hEdit_hak, HAK_EDIT_ID, (LPWSTR)hak_buffer, 20);			//학번 입력 받기
			break;
		case NAME_EDIT_ID:
			//이름 입력창
			GetDlgItemText(hEdit_name, NAME_EDIT_ID, (LPWSTR)name_buffer, 20);		//이름 입력 받기
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

		if (1 != login)					//로그인 안 됐을 때
		{
			wsprintf(str, L"학번 : ");
			TextOut(hdc, 10, 40, str, lstrlenW(str));
			wmemset(str, 0x00, MAX_PATH);
			wsprintf(str, L"이름 : ");
			TextOut(hdc, 200, 40, str, lstrlenW(str));
			wmemset(str, 0x00, MAX_PATH);
			wsprintf(str, L"IP : ");
			TextOut(hdc, 29, 10, str, lstrlenW(str));
			wmemset(str, 0x00, MAX_PATH);
			wsprintf(str, L"Port : ");
			TextOut(hdc, 206, 10, str, lstrlenW(str));
			wmemset(str, 0x00, MAX_PATH);
		}
		else if (1 != examStatus)
		{
			ShowWindow(g_controlManager->examControlButtonLogin, SW_SHOW);



			ShowWindow(hEdit_now_hak, SW_SHOW);
			ShowWindow(hEdit_now_name, SW_SHOW);

			status = L"시험 : 시험 준비";
			wsprintf(str, status);
			TextOut(hdc, 10, 10, str, lstrlenW(str));


			wsprintf(str, L"학번 : ");
			TextOut(hdc, 10, 38, str, lstrlenW(str));
			wmemset(str, 0x00, MAX_PATH);





			wsprintf(str, L"이름 : ");
			TextOut(hdc, 10, 58, str, lstrlenW(str));
			wmemset(str, 0x00, MAX_PATH);



			if (0 != g_controlManager->getExecuteStatus())
			{
				HPEN myPen, oldPen;

				myPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
				oldPen = (HPEN)SelectObject(hdc, myPen);

				wsprintf(str, L"더 이상 시도하지 마세요!");
				TextOut(hdc, 415, 10, str, lstrlenW(str));

				SelectObject(hdc, oldPen);
				DeleteObject(myPen);
			}

			/*LPCWSTR aa = (LPCWSTR)ExamInfo[0];
			wsprintf(str, aa);
			TextOut(hdc, 100, 10, str, lstrlenW(str));*/

			/*aa = (LPCWSTR)ExamInfo[1];
			wsprintf(str, aa);
			TextOut(hdc, 150, 10, str, lstrlenW(str));

			aa = (LPCWSTR)ExamInfo[2];
			wsprintf(str, aa);
			TextOut(hdc, 200, 10, str, lstrlenW(str));*/

			SetWindowTextW(hWnd, L"시험을 준비중입니다.");
			//wsprintf(str, L"공부하신만큼 좋은 성적 거두길 바라겠습니다.  :D");
			//TextOut(hdc, 10, 10, str, lstrlenW(str));
		}
		else
		{
			DestroyWindow(hEdit_now_hak);
			DestroyWindow(hEdit_now_name);
			DestroyWindow(g_controlManager->examControlButtonLogin);
			status = L"시험 : 시험 시작";
			wsprintf(str, status);
			TextOut(hdc, 10, 10, str, lstrlenW(str));

			g_controlManager->setProcessManagementMode();
			wsprintf(str, L"학번 : ");
			TextOut(hdc, 10, 38, str, lstrlenW(str));
			wmemset(str, 0x00, MAX_PATH);

			wsprintf(str, now_hak_text);
			TextOut(hdc, 55, 38, str, lstrlenW(str));
			wmemset(str, 0x00, MAX_PATH);

			wsprintf(str, L"이름 : ");
			TextOut(hdc, 10, 58, str, lstrlenW(str));
			wmemset(str, 0x00, MAX_PATH);

			wsprintf(str, now_name_text);
			TextOut(hdc, 55, 58, str, lstrlenW(str));
			wmemset(str, 0x00, MAX_PATH);

			g_controlManager->setProcessManagementMode();
			SetWindowTextW(hWnd, L"시험을 시작하였습니다.");
			GetLocalTime(&g_passTime);
			ShowWindow(g_controlManager->examControlButtonEnd, SW_SHOW);

			if (0 != g_controlManager->getExecuteStatus())
			{
				HPEN myPen, oldPen;

				myPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
				oldPen = (HPEN)SelectObject(hdc, myPen);

				wsprintf(str, L"더 이상 시도하지 마세요!");
				TextOut(hdc, 415, 10, str, lstrlenW(str));

				SelectObject(hdc, oldPen);
				DeleteObject(myPen);
			}
		}

		EndPaint(hWnd, &ps);
	}
	break;

	case WM_CLOSE:
		if (false == g_controlManager->getProcessTerminateMode())
		{
			MessageBox(hWnd, L"종료 버튼을 누르지 마세요!!! 막혀있습니다!!!", L"!!경고!!", MB_OK);
		}
		else
		{
			send(sock, (char*)L"9#", 4, 0);
			closesocket(sock); // 서버 소켓 종료
			WSACleanup(); // 소켓 종료
			PostQuitMessage(0); // 폼종료
		}
		break;

	case WM_DESTROY:
		STARTUPINFO SI;
		PROCESS_INFORMATION PI;
		TCHAR procName[MAX_PATH];

		wmemset(procName, 0x00, MAX_PATH);
		memset(&SI, 0x00, sizeof(SI));

		if (false == g_controlManager->getProcessTerminateMode())
		{
			if (false == g_controlManager->genExecuteProcessString(procName, MAX_PATH))
				return -1;

			CreateProcess(NULL, procName, NULL, NULL, NULL, 0, NULL, NULL, &SI, &PI);
			CloseHandle(PI.hThread);
			CloseHandle(PI.hProcess);
		}
		delete g_controlManager;
		delete g_processManager;

		CloseHandle(hdl_server);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
