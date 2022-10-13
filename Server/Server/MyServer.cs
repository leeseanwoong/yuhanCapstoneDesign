using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Sockets;
using System.Threading;
using System.Net;
using System.Collections.Concurrent;

namespace Server
{
    class UserInfo
    {
        public string hak { get; set; }
        public string name { get; set; }
    }

    class MyServer
    {
        private int m_cmd;  // 종류

        public static readonly int cmd_ClientFileSendRequest = 7;
        public static readonly int cmd_Login = 8;
        public static readonly int cmd_exit = 9;


        public TcpListener server { get; set; }

        // server를 실행시킬 스레드    
        private Thread server_tr, msg_tr, all_tr;

        // 서버에 연결되어 있는 클라이언트 수 만 됨(정보도 받아와야함)
        public ConcurrentDictionary<TcpClient, UserInfo> connectedClients = new ConcurrentDictionary<TcpClient, UserInfo>();

        string data = null;

        public MyServer() // 생성자
        {
            // TcpListener 클래스 생성 ( IP, 포트 )
            server = new TcpListener(System.Net.IPAddress.Any, 9000);

            server_tr = new Thread(WaitAndAcceptClient);
            server_tr.IsBackground = true;
        }

        ~MyServer() // 소멸자
        {
            server.Stop();
            server_tr.Interrupt();
        }

        public void Start() // 시작
        {
            // 서버 시작
            server.Start();
            server_tr.Start();
            Console.WriteLine("서버켜짐");
        }

        private void WaitAndAcceptClient() // 클라이언트 응답 요청
        {
            try
            {
                while (true)
                {
                    // 서버에 접속하려는 client 접속요청 허용후 클라이언트 객체에 할당
                    // 접속하려는 client가 없으면 무한 대기
                    // 서버에 접속하면 접속한 IP 출력
                    TcpClient AcceptClient = server.AcceptTcpClient();
                    Console.WriteLine("클라이언트 접속: {0} ", ((IPEndPoint)AcceptClient.Client.RemoteEndPoint).ToString());

                    ClientData clientData = new ClientData(AcceptClient);

                    msg_tr = new Thread(new ParameterizedThreadStart(Msg));
                    msg_tr.Start(AcceptClient);
                    /*all_tr = new Thread(new ParameterizedThreadStart(allSendMsg));
                    all_tr.Start(AcceptClient); */// 전체 보내기 기능 일단 죽여놓음
                }
            }
            catch (ThreadStateException)
            {
                return;
            }
        }

        private void Msg(object obj)
        {
            TcpClient tcpClient = (TcpClient)obj; // 스레드 에서 받아온 tcpclient 정보
            NetworkStream stream = tcpClient.GetStream(); // 스트림
            int length; // 길이
            byte[] bytes = new byte[256]; // 버퍼
            string[] TempData;
            int i = 0 , k = 0;

            try
            {
                while ((length = stream.Read(bytes, 0, bytes.Length)) != 0)
                {
                    if (k == 1) {
                        data = Encoding.Default.GetString(bytes, 0, length);
                        Console.WriteLine(String.Format("수신: {0}", data));
                        byte[] msg = Encoding.Default.GetBytes(data);
                        stream.Write(msg, 0, msg.Length);
                        Console.WriteLine(String.Format("송신: {0}", data));
                        ShowconnectUser();
                    } else {
                        for (; i < 1; i++)
                        {
                            data = Encoding.Unicode.GetString(bytes, 0, length + 2);
                            Console.WriteLine(String.Format("수신: {0}", data));
                            byte[] msg1 = Encoding.Unicode.GetBytes(data);
                            stream.Write(msg1, 0, msg1.Length);
                            Console.WriteLine(String.Format("송신: {0}", data));
                            k = 1;
                        }
                    }

                    TempData = data.Split("#");
                    m_cmd = int.Parse(TempData[0]);

                    // Protocol
                    if (m_cmd == cmd_exit) {
                        ClientExit(tcpClient);
                    }
                    else if (m_cmd == cmd_Login) {
                        ClientLogin(tcpClient, TempData);
                    }
                    else if (m_cmd == 2) {

                    }
                }
            }
            catch (Exception e) {
                tcpClient.Close();
            }
        }
        private void allSendMsg(object obj) 
        {
            foreach (var display in connectedClients)
            {
                NetworkStream stream = display.Key.GetStream(); // 보낼 유저의 스트림 가져오기
                byte[] msg = Encoding.Default.GetBytes(display.Value.name); // 보낼값
                stream.Write(msg, 0, msg.Length);
                Console.WriteLine(String.Format("{0}", data)); // 보낸값 출력
            }
        } // 현재 접속해 있는 모든 유저에게 메시지 전송
        private string Left(string str, int Length) 
        {
            if (str.Length < Length)
                Length = str.Length;

            return str.Substring(0, Length);
        } // left 함수 구현 미사용
        private void ShowconnectUser() 
        {
            Console.WriteLine("현재 접속해 있는 클라이언트 수 : " + connectedClients.Count());
            foreach (var display in connectedClients)
            {
                Console.Write(display.Value.hak + "#");
                Console.WriteLine(display.Value.name);
            }
        } // 현재 접속해 있는 모든 유저의 학번과 이름 출력
        private void ClientExit(TcpClient tcpClient)
        {
            Console.WriteLine("클라이언트 종료: {0} ", ((IPEndPoint)tcpClient.Client.RemoteEndPoint).ToString());
            for (int j = 0; j < connectedClients.Count(); j++)
            {
                connectedClients.TryRemove(tcpClient, out _);
            }
            tcpClient.Close();
            ShowconnectUser();
        }  // 나감 처리
        private void ClientLogin(TcpClient tcpClient, string[] tempdata)
        {
            UserInfo userinfo = new UserInfo();
            userinfo.hak = tempdata[1];
            userinfo.name = tempdata[2];
            connectedClients.TryAdd(tcpClient, userinfo);
            ShowconnectUser();
        }  // 클라이언트 로그인 햇을때에 해당 유저의 정보를 서버 배열에 저장되는 구조
    }
}
