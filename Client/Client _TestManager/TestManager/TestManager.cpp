// TestManager.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#define _CRT_SECURE_NO_WARNINGS
#include "framework.h"
#include "TestManager.h"
#include "ProcessManager.h"
#include "ControlManager.h"
#include <TlHelp32.h>
#include <string.h>

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

/// 주요 인스턴스 생성
ControlManager *g_controlManager;
ProcessManager *g_processManager;


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
	g_processManager = new ProcessManager(g_controlManager);

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TESTMANAGER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
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

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTMANAGER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TESTMANAGER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
   
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW&~WS_MAXIMIZEBOX&~WS_SIZEBOX,
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
bool examStatus = true;					//시험 시작 전(false)
wchar_t hak_buffer[20];						//학번 버퍼
wchar_t name_buffer[20];					//이름 버퍼
wchar_t enter[] = L"\n";
wchar_t name[] = L"이름 : ";
wchar_t hak[] = L"학번 : ";
wchar_t str[200] = L"";

HWND hEdit_hak, hEdit_name, hButton_login, hEdit_ip, hEdit_port;				//학번, 이름 핸들


SYSTEMTIME g_passTime;


void passTimeCalc(wchar_t *str__)
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


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	TCHAR str[MAX_PATH] = { 0, };

    switch (message)
    {
	case WM_TIMER:
		switch (wParam)
		{
		case TIMER_ID_TIME_DISPLAY:
			SYSTEMTIME st;

			g_timeCnt++;

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
				wmemset(str, 0x00, MAX_PATH);
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
			hButton_login=CreateWindow(L"button", L"로그인", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				200,
				80,
				150,
				50,
				hWnd,
				(HMENU)LOGIN_BUTTON_ID,
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
		
			// 시험 시작 버튼 생성
			/*
			g_controlManager->examControlButton = CreateWindow(
				L"button", L"시험 시작",
				WS_CHILD | BS_PUSHBUTTON,
				20,
				80,
				150,
				50,
				hWnd,
				(HMENU)EXAM_BUTTON_ID,
				hInst,
				NULL);
			*/
			// 시험 종료 버튼 생성
			g_controlManager->examControlButtonEnd = CreateWindow(
				L"button", L"시험 종료",
				WS_CHILD | BS_PUSHBUTTON,
				180,
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
				(HMENU)NAME_EDIT_ID,
				hInst,
				NULL);

			//Port 입력칸 생성
			hEdit_port = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_NUMBER,
				250,
				10,
				100,
				20,
				hWnd,
				(HMENU)NAME_EDIT_ID,
				hInst,
				NULL);

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
            int wmId = LOWORD(wParam);
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
				/// 시험 시작으로 프로세스 제어 시작
				/*
				g_controlManager->setProcessManagementMode();
				ShowWindow(g_controlManager->examControlButton, SW_HIDE);
				SetWindowTextW(hWnd, L"시험을 시작하였습니다...");
				GetLocalTime(&g_passTime);
				ShowWindow(g_controlManager->examControlButtonEnd, SW_SHOW);
				ShowWindow(g_controlManager->examControlButtonLogin, SW_SHOW);	
				*/
				break;
			case EXAM_END_BUTTON_ID:
				// 시험 종료버튼 눌렀을 때 실시
				int EXAM_END_MB_ID;
				EXAM_END_MB_ID = MessageBox(hWnd, L"시험을 종료 하시겠습니까?", L"!!필수 확인!!", MB_YESNO);
				//확인버튼 눌렀을때 파일 전송
				break;

			case EXAM_LOGIN_BUTTON_ID: 
				// 로그인 정보 수정 버튼 눌렀을 때 실시
				break;

			case LOGIN_BUTTON_ID:
				//로그인 버튼 눌렸을 때 실시	
				GetDlgItemText(hWnd, NAME_EDIT_ID, (LPWSTR)name_buffer, 20);
				GetDlgItemText(hWnd, HAK_EDIT_ID, (LPWSTR)hak_buffer, 20);
				wcscat(str, hak);
				wcscat(str, hak_buffer);
				wcscat(str, enter);
				wcscat(str, name);
				wcscat(str, name_buffer);
				
				int LOGIN_MB_ID;

				LOGIN_MB_ID = MessageBox(hWnd, (LPCWSTR)str, L"!!필수 확인!!", MB_YESNO);
				if (LOGIN_MB_ID == IDYES)
				{
					login = true;
					DestroyWindow(hButton_login);
					DestroyWindow(hEdit_hak);
					DestroyWindow(hEdit_name);
					DestroyWindow(hEdit_ip);
					DestroyWindow(hEdit_port);

					ShowWindow(g_controlManager->examControlButton, SW_SHOW);
					
				}
				else if (LOGIN_MB_ID == IDNO)
				{
					SetWindowText(hEdit_ip, L"");
					SetWindowText(hEdit_port, L"");
					SetWindowText(hEdit_name, L"");
					SetWindowText(hEdit_hak, L"");
					
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
			else if(1 != examStatus)
			{		
				wsprintf(str, L"시험: 준비중");
				TextOut(hdc, 10, 10, str, lstrlenW(str));
				//wsprintf(str, L"공부하신만큼 좋은 성적 거두길 바라겠습니다.  :D");
				//TextOut(hdc, 10, 10, str, lstrlenW(str));
				wmemset(str, 0x00, MAX_PATH);
				wsprintf(str, L"본 프로그램을 죽이지 마세요 ^^;");
				TextOut(hdc, 10, 30, str, lstrlenW(str));
				wmemset(str, 0x00, MAX_PATH);
			
				wsprintf(str, L"죽이기를 시도하면, 벌칙이 주어질 수 있습니다.");
				TextOut(hdc, 10, 50, str, lstrlenW(str));
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
			}
			else
			{
				wsprintf(str, L"시험: 시험시작");
				TextOut(hdc, 10, 10, str, lstrlenW(str));

				g_controlManager->setProcessManagementMode();
				//ShowWindow(g_controlManager->examControlButton, SW_HIDE);
				SetWindowTextW(hWnd, L"시험을 시작하였습니다...");
				GetLocalTime(&g_passTime);
				ShowWindow(g_controlManager->examControlButtonEnd, SW_SHOW);
				ShowWindow(g_controlManager->examControlButtonLogin, SW_SHOW);

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
			PostQuitMessage(0);
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
		}
		delete g_controlManager;
		delete g_processManager;
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
