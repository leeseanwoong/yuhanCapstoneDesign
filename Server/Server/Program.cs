using System;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace Server
{
    class Program
    {
        public static Program program = null;

        // TCP 서버를 관리하고 클라이언트와 통신하는 객체
        private MyServer server;


        static void Main(string[] args)
        {
            Program program = Program.GetInstance();
            program.Start();
            while (true)
            {

            }
        }

        public static Program GetInstance()
        {
            if (program == null)
            {
                program = new Program();
            }
            return program;
        }

        private Program()
        {
            server = new MyServer();
        }

        ~Program()
        {
        }
        public void Start()
        {
            server.Start();
        }

    }
}
