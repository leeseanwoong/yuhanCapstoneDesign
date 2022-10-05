#pragma comment(lib, "ws2_32") // 소켓을 사용하기 위해서 라이브러리 참조해야 한다.
#pragma warning(disable:4996) // inet_ntoa가 deprecated가 되었는데.. 사용하려면 아래 설정을 해야 한다.
#include <stdio.h>
#include <iostream>
#include <vector>
#include <thread>
#include <WinSock2.h> // 소켓을 사용하기 위한 라이브러리
#define BUFFERSIZE 1024 // 수신 버퍼 사이즈
using namespace std;
// 콘솔에 메시지를 출력하는 함수


/*void print(vector<wchar_t>* str)
{
    
    int p = 0; // 포인트 위치
    
    wchar_t out[BUFFERSIZE + 2];// 버퍼 설정. +1은 \0를 넣기 위한 크기
    
    cout << "From server message : ";// 콘솔 출력

    for (int n = 0; n < (str->size() / BUFFERSIZE) + 1; n++)
    {
        
        int size = str->size();// 버퍼 사이즈 설정
        
        if (size > BUFFERSIZE) {// 수신 데이터가 버퍼 사이즈를 넘었을 경우.
            if (str->size() < (n + 1) * BUFFERSIZE)
            {
                size = str->size() % BUFFERSIZE;
            }
            else
            {
                size = BUFFERSIZE;
            }
        }
        
        for (int i = 0; i < size; i++, p++)// echo 메시지와 콘솔 메시지를 작성한다.
        {
            out[i] = *(str->begin() + p);
        }
        
        wprintf(L"%s", out);// 콘솔 메시지 콘솔 출력. //cout << out;
        
    }
}*/
void firstsend(const char* hak, SOCKET sock)
{
    printf("학번 전송 : %s\n", hak);
    send(sock, hak, strlen(hak), 0);
    char cBuffer[BUFFERSIZE] = {};
    recv(sock, cBuffer, BUFFERSIZE, 0);
}

// 실행 함수
int main()
{
    const char * hak = "201807006#lee"; // 클라이언트 학번
    //string * Msg = &hak; // 서버에 보내줄때에 사용
    WSADATA wsaData; // 소켓 정보 데이터 설정
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) // 소켓 실행.
    {
        return 1;
    }

    SOCKET sock = socket(PF_INET, SOCK_STREAM, 0); // Internet의 Stream 방식으로 소켓 생성 

    SOCKADDR_IN addr; // 소켓 주소 설정

    memset(&addr, 0, sizeof(addr)); // 구조체 초기화
    addr.sin_family = AF_INET; // 소켓은 Internet 타입
    addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 127.0.0.1(localhost)로 접속하기
    addr.sin_port = htons(9000); // 포트 8898으로 접속


    if (connect(sock, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) // 접속
    {
        cout << "error" << endl; // 에러 콘솔 출력
        return 1;
    }

    firstsend(hak, sock);


    while (1) {

        char cMsg[1024] = ""; // 메시지 버퍼

        cout << "입력" << endl;
        cin >> cMsg;

        printf("내가 보내는 메시지 : %s\n", cMsg);
        send(sock, cMsg, strlen(cMsg), 0);

        char cBuffer[BUFFERSIZE] = {};
        recv(sock, cBuffer, BUFFERSIZE, 0);
            
        printf("서버에서 온 메시지 : %s\n", cBuffer);

        if (cMsg[0] == *"exit")
        {
            break;
        }

    }
    closesocket(sock); // 서버 소켓 종료
    WSACleanup(); // 소켓 종료
    return 0;

}


/*
   vector<wchar_t> buffer; // telent은 한글자씩 데이터가 오기 때문에 글자를 모을 buffer가 필요하다.(wchar_t 타입으로 선언)
   wchar_t x; // 수신 데이터
   while (1)
   {
       // 데이터를 받는다. 에러가 발생하면 멈춘다.
       // char* 형식으로 받기 때문에 타입 캐스팅을 한다.
       if (recv(sock, (char*)&x, sizeof(x), 0) == SOCKET_ERROR)
       {
           cout << "error" << endl; // 에러 콘솔 출력
           break;
       }
       buffer.push_back(x); // 버퍼에 글자를 하나 넣는다.
       if (buffer.size() > 4 && *(buffer.end() - 4) == '\r' && *(buffer.end() - 3) == '\n' && *(buffer.end() - 2) == '>' && *(buffer.end() - 1) == '\0') // \r\n>\0가 나오면 콘솔에 출력하고 콘솔로 부터 메시지를 기다린다.
       {



           buffer.clear(); // 버퍼 초기화

           char input[BUFFERSIZE]; // 콘솔로 부터 입력을 받는다.

           cin >> input; // 유저로 부터 입력 받기

           int size = strlen(input); // 입력받은 길이를 받는다.

           wchar_t buffer[BUFFERSIZE * 2]; // 개행을 넣는다.

           mbstowcs(&buffer[0], input, BUFFERSIZE * 2); // char*에서 wchar_t*으로 변환하는 함수
           *(buffer + size) = '\r';
           *(buffer + size + 1) = '\n';
           *(buffer + size + 2) = '\0';
           // 서버로 보내기
           // send함수가 char* 형식으로 보낼 수 있기 때문에 타입 캐스팅을 한다.
           send(sock, (char*)buffer, wcslen(buffer) * 2, 0);

           if (*input == 'e' && *(input + 1) == 'x' && *(input + 2) == 'i' && *(input + 3) == 't') // 메시지가 exit라면 종료
           {
               break;
           }
           else if (*input == 'a' && *(input + 1) == 'a' && *(input + 2) == 'a' && *(input + 3) == 'a') {
               cout << "aa";
           }
           continue;
       }
   }*/