using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Sockets;
using System.Threading;
using System.Net;
using System.Collections.Concurrent;
using System.IO;
using TestWeb;
using System.Security.Cryptography;
using CapstoneDProfessor;

namespace Server
{
    class UserInfo
    {
        public string hak { get; set; }
        public string name { get; set; }
        public bool FileOK { get; set; }
    }

     class MyServer
    {
        private int m_cmd;  // 종류
        public static readonly int cmd_InfoChange = 5;
        public static readonly int cmd_FileIn = 6;
        public static readonly int cmd_ClientMessage = 7;
        public static readonly int cmd_Login = 8;
        public static readonly int cmd_exit = 9;

        MainForm mainForm = new MainForm();

        public static string tempuserhak;
        public static string tempusername;


        public TcpListener server { get; set; }

        // server를 실행시킬 스레드    
        private Thread server_tr, msg_tr, cli_tr;

        // 서버에 연결되어 있는 클라이언트 수 만 됨(정보도 받아와야함)
        public ConcurrentDictionary<TcpClient, UserInfo> connectedClients = new ConcurrentDictionary<TcpClient, UserInfo>();

        public ConcurrentDictionary<TcpClient, TcpClient> ClientServerData = new ConcurrentDictionary<TcpClient, TcpClient>();

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
            if(msg_tr == null)
            {

            }
            else
            {
                msg_tr.Interrupt();
            }
            if (cli_tr == null)
            {

            }
            else
            {
                cli_tr.Interrupt();

            }
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


                    msg_tr = new Thread(new ParameterizedThreadStart(Msg));
                    msg_tr.Start(AcceptClient);

                    cli_tr = new Thread(new ParameterizedThreadStart(joinClient));
                    cli_tr.Start(AcceptClient);
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
            byte[] bytes = new byte[1024]; // 버퍼
            string[] UserInfo;
            int i = 0, k = 0;
            string data = null;

            try
            {
                while ((length = stream.Read(bytes, 0, bytes.Length)) != 0)
                {
                    if (k == 1)
                    {
                        data = Encoding.Unicode.GetString(bytes, 0, length);
                        Console.WriteLine(String.Format("수신: {0}", data));
                    }
                    else
                    {
                        for (; i < 1; i++)
                        {
                            data = Encoding.Unicode.GetString(bytes, 0, length);
                            Console.WriteLine(String.Format("수신: {0}", data));
                            byte[] msg1 = Encoding.Unicode.GetBytes(data);
                            stream.Write(msg1, 0, msg1.Length);
                            Console.WriteLine(String.Format("송신: {0}", data));
                            k = 1;
                        }
                    }
                    UserInfo = data.Split(new String[] { "#" }, StringSplitOptions.None);
                    m_cmd = int.Parse(UserInfo[0]);

                    // Protocol
                    if (m_cmd == cmd_exit)
                    { // 9번프로토콜이 들어왔을때
                        ClientExit(tcpClient);
                        break;
                    }
                    else if (m_cmd == cmd_Login)
                    { // 8번프로토콜이 들어왔을때
                        ClientLogin(tcpClient, UserInfo);
                    }
                    else if (m_cmd == cmd_ClientMessage)
                    { // 7번프로토콜이 들어왔을때 지금은 안씀
                        byte[] msg = Encoding.Default.GetBytes(data);
                        stream.Write(msg, 0, msg.Length);
                        Console.WriteLine(String.Format("송신: {0}", data));
                        ShowconnectUser();
                    }
                    else if (m_cmd == cmd_FileIn)
                    { // 6번프로토콜이 들어왔을때
                        Filerecvfromserver(stream, data);
                    }
                    else if (m_cmd == cmd_InfoChange)
                    { // 5번프로토콜이 들어왔을때
                        
                        ClientInfoChange(tcpClient, UserInfo);
                    }
                }
            }
            catch (Exception e)
            {
                tcpClient.Close();
            }
        }

        private void joinClient(object obj) // 클라이언트로 보낼때 사용하는 스트림 만들기
        {
            TcpClient AcceptClient = (TcpClient)obj;
            string clientaddress = ((IPEndPoint)AcceptClient.Client.RemoteEndPoint).ToString();
            string[] clientip = clientaddress.Split(new String[] { ":" }, StringSplitOptions.None);
            //Console.WriteLine(clientip[0]);

            TcpClient tc = new TcpClient(clientip[0], 9001);
            NetworkStream clistream = tc.GetStream();
            ClientServerData.TryAdd(AcceptClient, tc);

            while (ClientServerData.Count() != 0)
            {
                string a = "";
                if ((a = Console.ReadLine()) == "a") // 서버에서 a를 눌렀을때 연결된 전체 클라이언트에게 파일이 보내지게 해놓음.
                {
                    Console.WriteLine("all send");
                    allSendFile();
                }
                else if (a == "b")
                {
                    Console.WriteLine("블랙리스트 전송");
                    string[] BlacklistData = { "5", "168", "188", "28", "29", "155" };
                    //SendBlackList(BlacklistData);
                }
                else if (a == "set")
                {
                    Console.WriteLine("시험정보 전송");
                    string[] ExamData = { "과목명", "시험시간", "담당교수" };
                    //SendExamData(ExamData);
                }
                else if (a == "start")
                {
                    Console.WriteLine("시험시작 전송");
                    SendExamStart();
                }
                else if (a != "") // 서버에서 학번을 입력햇을때 그 학번에게 파일이 보내지게 해놓음.
                {

                    Console.WriteLine("학번 " + a + " 에게 전송");
                    Fliesendfromserver(clistream, a);
                }
                else
                {
                }
            }

        }

        /*private void SendBlackList(string[] BlacklistData) // 블랙리스트 보내기
        {
            string b = null;
            for (int i = 0; i < BlacklistData.Length; i++)
            {
                b = b + BlacklistData[i] + "#";
            }
            b = Left(b, b.Length - 1);

            byte[] msg1 = Encoding.Unicode.GetBytes(b);
            foreach (var item in ClientServerData)
            {
                NetworkStream ClientStream = item.Value.GetStream();
                ClientStream.Write(new byte[1] { 1 }, 0, 1); // 1바이트를 보냄
                ClientStream.Write(msg1, 0, msg1.Length);
            }

        }
        private void SendExamData(string[] ExamData)
        {
            string b = null;
            for (int i = 0; i < ExamData.Length; i++)
            {
                b = b + ExamData[i] + "#";
            }
            b = Left(b, b.Length - 1);

            byte[] msg1 = Encoding.Unicode.GetBytes(b);
            foreach (var item in ClientServerData)
            {
                NetworkStream ClientStream = item.Value.GetStream();
                ClientStream.Write(new byte[1] { 3 }, 0, 1); // byte=1의 데이터를 보낸다.
                ClientStream.Write(msg1, 0, msg1.Length);
            }
        }*/

        public void SendExamStart()
        {
            foreach (var item in ClientServerData)
            {
                NetworkStream ClientStream = item.Value.GetStream();
                ClientStream.Write(new byte[1] { 4 }, 0, 1); // byte=4의 데이터를 보낸다.
            }
        }

        public void ExamEnd()
        {
            foreach (var item in ClientServerData)
            {
                NetworkStream ClientStream = item.Value.GetStream();
                ClientStream.Write(new byte[1] { 5 }, 0, 1); // byte=5의 데이터를 보낸다.
            }
        }

        public static string GetChecksum(string file)
        {
            using (SHA256 SHA256 = SHA256Managed.Create())
            {
                using (FileStream fileStream = System.IO.File.OpenRead(file))
                {
                    string result = "";
                    foreach (var hash in SHA256.ComputeHash(fileStream))
                    {
                        result += hash.ToString("x2");
                    }

                    return result;
                }
            }
        }

        public void Fliesendfromserver(NetworkStream stream, string targetclient) // 서버에서 targetclient에게 파일을 보낼때 사용
        {
            var fp = mainForm.sendfilepath(); //  폴더 경로 저장 변수 .
            var ffp = @"";
            var path = Path.Combine(ffp, fp);

            var ffn = @"D:\";
            var fn = mainForm.sentfilename();
            var pathh = Path.Combine(ffn, fn);
            

            NetworkStream clistream = stream;
            bool clistreamchange = false;

            foreach (var item in connectedClients)
            {
                if (item.Value.hak == targetclient)
                {
                    foreach (var item2 in ClientServerData)
                    {
                        if (item2.Key.GetStream() == item.Key.GetStream())
                        {
                            Console.WriteLine("학번에 해당하는 정보가져오는중");
                            clistream = item2.Value.GetStream();
                            clistreamchange = true; // 학번이 존재할때에
                        }
                    }
                }
            }
            if (clistreamchange == false) // 해당 학번이 존재하지 않을 경우 파일을 보내지 않음
            {
                Console.WriteLine("해당하는 학번이 없습니다");
                return;
            }
            ZipManager.ZipFiles(path, pathh + ".zip", "1234", false);

            FileInfo file = new FileInfo(pathh+".zip");
            // stream을 취득한다.
            string hash = GetChecksum(pathh+".zip");
            Console.WriteLine(hash);
            using (FileStream filestream = new FileStream(file.FullName, FileMode.Open, FileAccess.Read))
            {
                // 파일 binary를 가져온다.
                byte[] data = new byte[file.Length];
                //data[0] = 3;
                filestream.Read(data, 0, data.Length);
                int FileSize = (int)file.Length;
                clistream.Write(new byte[1] { 2 }, 0, 1); // 2
                // 전송 람다 함수 (C++과 약속한 규약대로 데이터 사이즈를 보내고 데이터를 보낸다.)
                Action<byte[]> Send = (b) =>
                {
                    byte[] datanamesize = BitConverter.GetBytes(b.Length);
                    // 먼저 데이터 사이즈를 보내고.
                    clistream.Write(datanamesize, 0, datanamesize.Length);
                    // 데이터를 보낸다.
                    clistream.Write(b, 0, b.Length);
                };


                var rcvfilename =mainForm.recvfname();
                // 먼저 파일 명을 전송한다. (C++에서 \0를 보내지 않으면 메모리 구분이 되지 않으니 \0를 포함해서 보낸다.)
                // 이번에는 unicode가 아닌 utf8형식으로 전송합니다.
                Send(Encoding.UTF8.GetBytes("ExamFile.zip\0"));

                // 파일 바이너리 데이터를 보낸다.
                byte[] datasize = BitConverter.GetBytes(data.Length);
                // 먼저 데이터 사이즈를 보내고.
                clistream.Write(datasize, 0, datasize.Length);
                Console.WriteLine(FileSize);
                int StartPoint = 0;
                int SendSize = 4096;
                while (FileSize > 0)
                {

                    clistream.Write(data, StartPoint, SendSize);
                    int remaining = FileSize;

                    FileSize -= SendSize;
                    StartPoint += SendSize;
                    if (FileSize < 4096)
                    {
                        SendSize = (int)FileSize;
                    }
                }

                byte[] bytes = new byte[1024]; // 버퍼
                string readdata = null;
                int le = 0;
                le = clistream.Read(bytes, 0, bytes.Length);
                readdata = Encoding.Unicode.GetString(bytes, 0, le);

                if (readdata == hash)
                {
                    clistream.Write(new byte[1] { 3 }, 0, 1);
                    foreach (var item in connectedClients)
                    {
                        if (item.Value.hak == targetclient)
                        {
                            foreach (var item2 in ClientServerData)
                            {
                                if (item2.Key.GetStream() == item.Key.GetStream())
                                {
                                    item.Value.FileOK = true;
                                }
                            }
                        }
                    }
                }
                else
                {
                    return;
                }

                // 서버로 부터 byte=1 데이터가 오면 클라이언트 종료한다.
                byte[] ret = new byte[1];
                clistream.Read(ret, 0, ret.Length);
                if (ret[0] == 1)
                {
                    Console.WriteLine("전송 완료");
                }
                ret[0] = 5;

                filestream.Flush();
                filestream.Close();
            }
        }

        private void Filerecvfromserver(NetworkStream stream, string data)  // 서버에서 파일을 받을때 사용
        {


            Func<byte[]> Receive = () =>
            {
                // big endian타입으로 데이터 크기를 받는다.
                var datae = new byte[4];
                stream.Read(datae, 0, datae.Length);
                // binary 객체 선언
                int ReadFileSize = BitConverter.ToInt32(datae, 0);
                Console.WriteLine(ReadFileSize);
                var data1 = new byte[ReadFileSize];

                int readsize = 4096;
                int offset = 0;
                int remaining = ReadFileSize;
                while (remaining > 0)
                {
                    if (remaining < 4096)
                    {
                        readsize = remaining;
                    }
                    int read = stream.Read(data1, offset, readsize);
                    remaining -= read;
                    offset += read;

                }
                readsize = 4096;
                // 데이터 받기
                //stream.Read(data1, 0, data1.Length);
                return data1;
            };
            string[] filename = data.Split(new String[] { "#" }, StringSplitOptions.None); // 프로토콜과 파일이름 분리
            File.WriteAllBytes("D:\\work\\serverwork\\" + filename[1] + ".zip", Receive()); // 파일 데이터를 받고 저장한다.
            stream.Write(new byte[1] { 1 }, 0, 1); // byte=1의 데이터를 보낸다.
        }

        public void allSendFile() // 현재 접속해 있는 모든 유저에게 파일 전송
        {
            var fp = mainForm.sendfilepath(); //  폴더 경로 저장 변수 .
            var ffp = @"";
            var path = Path.Combine(ffp, fp);
            var ffn = @"D:\";
            var fn = System.IO.Path.GetFileName(fp);

            var pathh = Path.Combine(ffn, fn);

            ZipManager.ZipFiles(path, pathh + ".zip", "1234", false);

            FileInfo file = new FileInfo(pathh + ".zip");
            // stream을 취득한다.

            NetworkStream stream;

            string hash = GetChecksum(pathh + ".zip");
            Console.WriteLine(hash);

            foreach (var display in ClientServerData)
            {
                //Console.WriteLine(display.Key.Client.RemoteEndPoint.ToString());
                stream = display.Value.GetStream(); // 보낼 유저의 스트림 가져오기
                // stream을 취득한다.
                using (FileStream filestream = new FileStream(file.FullName, FileMode.Open, FileAccess.Read))
                {
                    // 파일 binary를 가져온다.
                    byte[] data = new byte[file.Length];
                    //data[0] = 3;
                    filestream.Read(data, 0, data.Length);
                    int FileSize = (int)file.Length;
                    stream.Write(new byte[1] { 2 }, 0, 1); // 2
                                                           // 전송 람다 함수 (C++과 약속한 규약대로 데이터 사이즈를 보내고 데이터를 보낸다.)
                    Action<byte[]> Send = (b) =>
                    {
                        byte[] datanamesize = BitConverter.GetBytes(b.Length);
                        // 먼저 데이터 사이즈를 보내고.
                        stream.Write(datanamesize, 0, datanamesize.Length);
                        // 데이터를 보낸다.
                        stream.Write(b, 0, b.Length);
                    };


                    var rcvfilename = mainForm.recvfname();
                    // 먼저 파일 명을 전송한다. (C++에서 \0를 보내지 않으면 메모리 구분이 되지 않으니 \0를 포함해서 보낸다.)
                    // 이번에는 unicode가 아닌 utf8형식으로 전송합니다.
                    Send(Encoding.UTF8.GetBytes("ExamFile.zip\0"));

                    // 파일 바이너리 데이터를 보낸다.
                    byte[] datasize = BitConverter.GetBytes(data.Length);
                    // 먼저 데이터 사이즈를 보내고.
                    stream.Write(datasize, 0, datasize.Length);
                    Console.WriteLine(FileSize);
                    int StartPoint = 0;
                    int SendSize = 4096;
                    while (FileSize > 0)
                    {

                        stream.Write(data, StartPoint, SendSize);
                        int remaining = FileSize;

                        FileSize -= SendSize;
                        StartPoint += SendSize;
                        if (FileSize < 4096)
                        {
                            SendSize = (int)FileSize;
                        }
                    }

                    byte[] bytes = new byte[1024]; // 버퍼
                    string readdata = null;
                    int le = 0;
                    le = stream.Read(bytes, 0, bytes.Length);
                    readdata = Encoding.Unicode.GetString(bytes, 0, le);

                    if (readdata == hash)
                    {
                        stream.Write(new byte[1] { 3 }, 0, 1);
                        foreach (var item in connectedClients)
                        {
                            if (item.Key.GetStream() == stream)
                            {
                                item.Value.FileOK = true;
                            }
                        }
                    }
                    else
                    {
                        return;
                    }
                    // 서버로 부터 byte=1 데이터가 오면 클라이언트 종료한다.
                    byte[] ret = new byte[1];
                    stream.Read(ret, 0, ret.Length);
                    if (ret[0] == 1)
                    {
                        Console.WriteLine("전송 완료");
                    }
                }
            }
        }

        private string Left(string str, int Length) // left 함수 구현 미사용
        {
            if (str.Length < Length)
                Length = str.Length;
            return str.Substring(0, Length);
        }

        private void ShowconnectUser() // 현재 접속해 있는 모든 유저의 학번과 이름 출력
        {
            Console.WriteLine("현재 접속해 있는 클라이언트 수 : " + connectedClients.Count());
            foreach (var display in connectedClients)
            {
                Console.Write(display.Value.hak + "#");
                Console.WriteLine(display.Value.name);
            }
        }

        private void ClientExit(TcpClient tcpClient) // 나감 처리
        {
            Console.WriteLine("클라이언트 종료: {0} ", ((IPEndPoint)tcpClient.Client.RemoteEndPoint).ToString());

            foreach (var item in connectedClients)
            {
                if (item.Key == tcpClient)
                {
                    tempuserhak = item.Value.hak;
                    tempusername = item.Value.name;
                }
            }
            mainForm.Removedata(tempuserhak, tempusername);
            connectedClients.TryRemove(tcpClient, out _); // 배열에서 삭제
            foreach (var item in ClientServerData)
            {
                if (item.Key.GetStream() == tcpClient.GetStream())
                {
                    TcpClient temp = item.Value;
                    temp.Close(); // 클라이언트로 연결되어 있는 스트림 종료
                }
            }
            ClientServerData.TryRemove(tcpClient, out _);// 배열에서 삭제

            tcpClient.Close(); // 서버에 연결되어 있는 스트림 종료
            ShowconnectUser();
        }

        private void ClientLogin(TcpClient tcpClient, string[] tempdata)  // 클라이언트 로그인 햇을때에 해당 유저의 정보를 서버 배열에 저장되는 구조
        {
            UserInfo userinfo = new UserInfo();
            userinfo.hak = tempdata[1];
            userinfo.name = tempdata[2];
            mainForm.Setuser(userinfo.hak, userinfo.name);
            
            connectedClients.TryAdd(tcpClient, userinfo);
            ShowconnectUser();
        }

        private void ClientInfoChange(TcpClient tcpClient, string[] tempdata)
        {
            foreach (var item in connectedClients)
            {
                if (item.Key == tcpClient)
                {
                    tempuserhak = item.Value.hak;
                    tempusername = item.Value.name;
                }
            }
            mainForm.Updatedata(tempuserhak,tempusername, tempdata[1], tempdata[2]);
            //mainForm.Removedata(tempuserhak,tempusername);
            connectedClients.TryRemove(tcpClient, out _);
            UserInfo userinfo = new UserInfo();
            userinfo.hak = tempdata[1];
            userinfo.name = tempdata[2];
            userinfo.FileOK = false;
            connectedClients.TryAdd(tcpClient, userinfo);
            ShowconnectUser();
        }

    }
}
