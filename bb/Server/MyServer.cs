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
    class MyServer
    {
        public TcpListener server { get; set; }

        // server를 실행시킬 스레드    
        private Thread server_tr, msg_tr, all_tr;

        // 서버에 연결되어 있는 클라이언트 수 만 됨(정보도 받아와야함)
        public ConcurrentDictionary<TcpClient, string> connectedClients = new ConcurrentDictionary<TcpClient, string>();

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
            int byteRcvd;
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
                    all_tr = new Thread(new ParameterizedThreadStart(allSendMsg));
                    all_tr.Start(AcceptClient);
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
            int i = 0;

            try
            {
                while ((length = stream.Read(bytes, 0, bytes.Length)) != 0)
                {
                    data = Encoding.Default.GetString(bytes, 0, length);
                    Console.WriteLine(String.Format("수신: {0}", data));
                    byte[] msg = Encoding.Default.GetBytes(data);
                    stream.Write(msg, 0, msg.Length);
                    Console.WriteLine(String.Format("송신: {0}", data));

                    for (; i < 1; i++)
                    {
                        connectedClients.TryAdd(tcpClient, data);
                    } 

                    /* 클라이언트 값 저장
                    foreach (var display in connectedClients)
                    {
                        Console.WriteLine(display.Key);
                        Console.WriteLine(display.Value);
                    }*/

                    Console.WriteLine("현재 접속해 있는 클라이언트 수 : "+connectedClients.Count());

                    if (data == "exit")
                    {
                        Console.WriteLine("클라이언트 종료: {0} ", ((IPEndPoint)tcpClient.Client.RemoteEndPoint).ToString());
                        for (int j = 0; j < connectedClients.Count(); j++)
                        {
                            connectedClients.TryRemove(tcpClient, out _);
                        }
                        tcpClient.Close();
                        Console.WriteLine("현재 접속해 있는 클라이언트 수 : " + connectedClients.Count());
                    }
                    else if (data == "asd") // 클라이언트에서 무언가를 보냈을때에 처리할수 있는 부분
                    {
                        Console.WriteLine("asd"); // 처리문 지금은 그냥 출력만 하는것
                    }
                }
            }
            catch (Exception e)
            {
                tcpClient.Close();
            }

        }

        private void allSendMsg(object obj)
        {
            foreach (var display in connectedClients)
            {
                Console.WriteLine(display.Key);
                NetworkStream stream = display.Key.GetStream(); // 스트림
                Console.WriteLine(data);
                byte[] msg = Encoding.Default.GetBytes(data);
                stream.Write(msg, 0, msg.Length);
                Console.WriteLine(String.Format("전체송신: {0}", data));
            }
        }
    }
}
