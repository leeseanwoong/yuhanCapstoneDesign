using Server;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CapstoneDProfessor
{
    class ServerProgram
    {
        public static ServerProgram program = null;

        // TCP 서버를 관리하고 클라이언트와 통신하는 객체
        private MyServer server;


        /*static void Main(string[] args)
        {
            Progra progra = Progra.GetInstance();
            progra.Start();
            while (true)
            {

            }
        }*/

        public static ServerProgram GetInstance()
        {
            if (program == null)
            {
                program = new ServerProgram();
            }
            return program;
        }

        private ServerProgram()
        {
            server = new MyServer();
        }

        ~ServerProgram()
        {
        }
        public void Start()
        {
            server.Start();
        }

    }
}
