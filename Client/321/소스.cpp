#pragma comment(lib, "ws2_32") // ������ ����ϱ� ���ؼ� ���̺귯�� �����ؾ� �Ѵ�.
#pragma warning(disable:4996) // inet_ntoa�� deprecated�� �Ǿ��µ�.. ����Ϸ��� �Ʒ� ������ �ؾ� �Ѵ�.
#include <stdio.h>
#include <iostream>
#include <vector>
#include <thread>
#include <WinSock2.h> // ������ ����ϱ� ���� ���̺귯��
#include <string>
#include <cctype>
#define BUFFERSIZE 64 // ���� ���� ������
using namespace std;
// �ֿܼ� �޽����� ����ϴ� �Լ�


/*void print(vector<wchar_t>* str)
{
    
    int p = 0; // ����Ʈ ��ġ
    
    wchar_t out[BUFFERSIZE + 2];// ���� ����. +1�� \0�� �ֱ� ���� ũ��
    
    cout << "From server message : ";// �ܼ� ���

    for (int n = 0; n < (str->size() / BUFFERSIZE) + 1; n++)
    {
        
        int size = str->size();// ���� ������ ����
        
        if (size > BUFFERSIZE) {// ���� �����Ͱ� ���� ����� �Ѿ��� ���.
            if (str->size() < (n + 1) * BUFFERSIZE)
            {
                size = str->size() % BUFFERSIZE;
            }
            else
            {
                size = BUFFERSIZE;
            }
        }
        
        for (int i = 0; i < size; i++, p++)// echo �޽����� �ܼ� �޽����� �ۼ��Ѵ�.
        {
            out[i] = *(str->begin() + p);
        }
        
        wprintf(L"%s", out);// �ܼ� �޽��� �ܼ� ���. //cout << out;
        
    }
}*/
void firstsend(const wchar_t* userinfo, SOCKET sock)
{
    wprintf_s(userinfo);
    send(sock, (char*)userinfo, (wcslen(userinfo) * 2), 0);
    char cBuffer[BUFFERSIZE] = {};
    recv(sock, cBuffer, BUFFERSIZE, 0);
}


// ���� �Լ�
int main() 
{
    int i = 0;

    const wchar_t * UserInfo = L"8#201807006#�̼���"; // Ŭ���̾�Ʈ �й�
    
    //string * Msg = &hak; // ������ �����ٶ��� ���
    WSADATA wsaData; // ���� ���� ������ ����
    
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) // ���� ����.
    {
        return 1;
    }

    SOCKET sock = socket(PF_INET, SOCK_STREAM, 0); // Internet�� Stream ������� ���� ���� 

    SOCKADDR_IN addr; // ���� �ּ� ����

    memset(&addr, 0, sizeof(addr)); // ����ü �ʱ�ȭ
    addr.sin_family = AF_INET; // ������ Internet Ÿ��
    addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 127.0.0.1(localhost)�� �����ϱ�
    addr.sin_port = htons(9000); // ��Ʈ 8898���� ����


    if (connect(sock, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) // ����
    {
        cout << "error" << endl; // ���� �ܼ� ���
        return 1;
    }

    

    firstsend(UserInfo, sock);

    char cMsg[1024] = ""; // �޽��� ����

   

    while (1) {

        cout << "�Է�" << endl;
        cin >> cMsg;

        printf("%d - ���� ������ �޽��� : -%s-\n", i, cMsg);
        send(sock, cMsg, strlen(cMsg), 0);

        char cBuffer[BUFFERSIZE] = {};
        recv(sock, cBuffer, BUFFERSIZE, 0);
        printf("%d - �������� �� �޽��� : -%s-\n",i++, cBuffer);

        if (cMsg[0] == '9' && cMsg[1] == '\0') { 
            break;
        }  // Ŭ���̾�Ʈ ���� ó��
    }
    closesocket(sock); // ���� ���� ����
    WSACleanup(); // ���� ����
    return 0;
}



/*
   vector<wchar_t> buffer; // telent�� �ѱ��ھ� �����Ͱ� ���� ������ ���ڸ� ���� buffer�� �ʿ��ϴ�.(wchar_t Ÿ������ ����)
   wchar_t x; // ���� ������
   while (1)
   {
       // �����͸� �޴´�. ������ �߻��ϸ� �����.
       // char* �������� �ޱ� ������ Ÿ�� ĳ������ �Ѵ�.
       if (recv(sock, (char*)&x, sizeof(x), 0) == SOCKET_ERROR)
       {
           cout << "error" << endl; // ���� �ܼ� ���
           break;
       }
       buffer.push_back(x); // ���ۿ� ���ڸ� �ϳ� �ִ´�.
       if (buffer.size() > 4 && *(buffer.end() - 4) == '\r' && *(buffer.end() - 3) == '\n' && *(buffer.end() - 2) == '>' && *(buffer.end() - 1) == '\0') // \r\n>\0�� ������ �ֿܼ� ����ϰ� �ַܼ� ���� �޽����� ��ٸ���.
       {



           buffer.clear(); // ���� �ʱ�ȭ

           char input[BUFFERSIZE]; // �ַܼ� ���� �Է��� �޴´�.

           cin >> input; // ������ ���� �Է� �ޱ�

           int size = strlen(input); // �Է¹��� ���̸� �޴´�.

           wchar_t buffer[BUFFERSIZE * 2]; // ������ �ִ´�.

           mbstowcs(&buffer[0], input, BUFFERSIZE * 2); // char*���� wchar_t*���� ��ȯ�ϴ� �Լ�
           *(buffer + size) = '\r';
           *(buffer + size + 1) = '\n';
           *(buffer + size + 2) = '\0';
           // ������ ������
           // send�Լ��� char* �������� ���� �� �ֱ� ������ Ÿ�� ĳ������ �Ѵ�.
           send(sock, (char*)buffer, wcslen(buffer) * 2, 0);

           if (*input == 'e' && *(input + 1) == 'x' && *(input + 2) == 'i' && *(input + 3) == 't') // �޽����� exit��� ����
           {
               break;
           }
           else if (*input == 'a' && *(input + 1) == 'a' && *(input + 2) == 'a' && *(input + 3) == 'a') {
               cout << "aa";
           }
           continue;
       }
   }*/