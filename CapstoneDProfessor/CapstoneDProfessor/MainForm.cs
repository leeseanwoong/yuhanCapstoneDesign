using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Collections;
using Microsoft.WindowsAPICodePack.Dialogs;
using System.Net;
using Server;
using System.Net.Sockets;
using System.IO;
using TestWeb;
using System.Security.Cryptography;

namespace CapstoneDProfessor
{
    public partial class MainForm : MetroFramework.Forms.MetroForm // 상속 클래스 변경 -> 디자인 변경
    {
        public static BackgroundWorker _bgwork1 = new BackgroundWorker();
        public static BackgroundWorker _bgwork2= new BackgroundWorker();
        public static BackgroundWorker _bgwork3 = new BackgroundWorker();

        MyServer myServer;
        
        static DataTable table = null;
        DataRow row = null;
        
        static string rehak;
        static string rename;
        static string oldhak;
        static string oldname;

        static public string fp; 
        static public string fn; 
        static public string rcf;

        static string subject;
        static string professor;
        static string testtime;

        static string ip;
        static string port = "9000";

        SetExam f2 = new SetExam();

        DateTime now;

        int timerc = 0;
        
        public MainForm()
        {
            InitializeComponent();
            _bgwork1.RunWorkerCompleted += new RunWorkerCompletedEventHandler(_bgwork1_AddData);
            _bgwork2.RunWorkerCompleted += new RunWorkerCompletedEventHandler(_bgwork2_UpdateData);
            _bgwork3.RunWorkerCompleted += new RunWorkerCompletedEventHandler(_bgwork3_RemoveData);

        }
        private void MainForm_Load(object sender, EventArgs e)
        {
            /*ServerProgram serverProgram = ServerProgram.GetInstance();
            serverProgram.Start();*/
            myServer = new MyServer();
            myServer.Start();
            
            button1.Enabled = false;
            btnStart.Enabled = false;
            btnSend.Enabled = false;

            table = new DataTable();

            

            table.Columns.Add("hak");
            table.Columns.Add("name");
            table.Columns.Add("state");
            table.Columns.Add("filesend");

            dgStudent.DataSource = table;

            IPHostEntry ip = Dns.GetHostByName(Dns.GetHostName());
            lblIpaddress.Text = "아 이 피  : " +ip.AddressList[0].ToString();
            lblport.Text = "포트번호 : " + port;

        }
        void _bgwork1_AddData(object sender, RunWorkerCompletedEventArgs e)
        {
            DataGridAdd1();
            DataGridRefresh();

        }
        void _bgwork2_UpdateData(object sender, RunWorkerCompletedEventArgs e)
        {
            DataGridUpdate();
            DataGridAdd1();
            DataGridRefresh();
        }
        void _bgwork3_RemoveData(object sender, RunWorkerCompletedEventArgs e)
        {
            DataGridRemove();
            DataGridAdd();
            DataGridRefresh();
        }
        private void DataGridAdd()
        {
            if (dgStudent.InvokeRequired)
            {
                
                dgStudent.Invoke(new MethodInvoker(delegate { dgStudent.DataSource = table; }));
                
            }
            else
            {
                dgStudent.DataSource = table;
            }
        }
        private void DataGridAdd1()
        {
            if (dgStudent.InvokeRequired)
            {

                dgStudent.Invoke(new MethodInvoker(delegate { dgStudent.DataSource = table; }));
                dgStudent.Rows[dgStudent.RowCount - 1].Cells[3] = new DataGridViewButtonCell();
            }
            else
            {
                dgStudent.DataSource = table;
            }
        }

        private void DataGridUpdate()
        {
            foreach (DataRow Rrow in table.Rows)
            {
                if (Rrow[0].ToString() == oldhak && Rrow[1].ToString() == oldname)
                {
                    Rrow[0] = rehak;
                    Rrow[1] = rename;
                }
            }
        }
        private void DataGridRemove()
        {
            table.AcceptChanges();
            foreach (DataRow Rrow in table.Rows)
            {
                if (Rrow["hak"].ToString() == rehak && Rrow["name"].ToString() == rename)
                {
                    Rrow.Delete() ;
                }
            }
            table.AcceptChanges();
        }
        private void DataGridRefresh()
        {
            if(dgStudent.InvokeRequired)
            {
                dgStudent.Invoke(new MethodInvoker(delegate { dgStudent.Refresh(); }));
            }
            else
                dgStudent.Refresh();
        }
        public void Updatedata(string oldh,string oldn,string newhak,string newname)
        {
            rehak = newhak;  rename = newname;
            oldhak = oldh; oldname = oldn;
            _bgwork2.RunWorkerAsync();
        }
        public void Removedata(string rmhak,string rmname)
        {
            rehak = rmhak; rename = rmname;
            _bgwork3.RunWorkerAsync();
        }
        void Adddata()
        {
            row = table.NewRow();

            row["hak"] = rehak;
            row["name"] = rename;
            row["state"] = "진행";
            row["filesend"] = "전송";

            table.Rows.Add(row);
            _bgwork1.RunWorkerAsync();
        }
        public void Setuser(string a, string b)
        {
            rehak = a;
            rename = b;
            Adddata();  
        }
        private void dgStudent_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {
            
            int selectedRow = e.RowIndex;
            DataGridViewRow row = dgStudent.Rows[selectedRow];
            if (dgStudent.CurrentCell.ColumnIndex.Equals(3))
                if (dgStudent.CurrentCell != null && dgStudent.CurrentCell.Value != null)
                {
                    myServer.Fliesendfromserver(null, row.Cells[0].Value.ToString());
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
        
        private void btnsetExam_Click(object sender, EventArgs e) // 시험 설정 ->과목명,시험시간,감독교수 입력
        {
            using (SetExam setexam = new SetExam())
            {
                setexam.FormSend += new SetExam.FormSendEvent(setex);
                setexam.ShowDialog();
            }
            btnSend.Enabled = true;
        }
        private void setex(string sub,string name,int time)
        {
            timer1.Stop();
            subject = sub; professor = name;
            lblsubName.Text = "과목 이름 : " + subject;
            lblName.Text = "담당 교수 : " + professor;

            int hour;
            int min;

            hour = time / 60;
            min = time % 60;
            lblTime.Text = String.Format("{0}:{1}:{2}",hour.ToString("D2"),min.ToString("D2"),"00" );
            testtime = lblTime.Text;
            timerc = time;
        }
        private void btnStart_Click(object sender, EventArgs e) //시험 시작
        {
            //블랙리스트에 해당하는 프로세스 중지, 타이머 시작
            myServer.SendExamStart();
            timer1.Stop();

            now = DateTime.Now.AddMinutes(timerc);

            timer1.Interval = 1000;
            timer1.Start();
        }

        public string sendfilepath()
        {
            return fp;
        }
        public string sentfilename()
        {
            return fn;
        }
        public string recvfname()
        {
            return rcf;
        }
        private void btnSend_Click(object sender, EventArgs e) //문제 전송
        {
            using( SendFile sendFile = new SendFile())
            {
                sendFile.FormSendF += new SendFile.FormSendFileEvent(RcvFile);
                sendFile.GetPath = fp;
                sendFile.ShowDialog();
            }
            // OpenFileDialog openFileDialog = new OpenFileDialog(); 
            // openFileDialog.Filter = "Text Files(*.txt)|*.txt|All Files(*.*)|*.*";
            button1.Enabled = true;            
            //문제 파일 선택, 파일 전송

        }
        private void RcvFile(string dirpath,string filename,string recvfilename)
        {
            fp = dirpath;
            fn = filename;
            rcf = recvfilename;

        }
        private void btnBlack_Click(object sender, EventArgs e) //블랙 리스트
        {
            using (BlackList blackList = new BlackList())
            {
                blackList.ShowDialog();
            }

        }
        private void btnWhite_Click(object sender, EventArgs e) //화이트 리스트
        {
            using (WhiteList whiteList= new WhiteList())
            {
                whiteList.ShowDialog();
            }
        }
        private void Timer_Tick(object sender, EventArgs e) //타이머 시작 메소드 : timer1.Start();
        {
            TimeSpan t = now - DateTime.Now;
            Math.Abs(t.Seconds);
            lblTime.Text = String.Format("{0}", t.ToString("hh':'mm':'ss"));
        }

        private void openFileDialog1_FileOk(object sender, CancelEventArgs e)
        {
            MessageBox.Show("선택");
        }

        private void lblName_Click(object sender, EventArgs e)
        {

        }

        private void btnSelect_Click_1(object sender, EventArgs e)
        {
            timer1.Stop();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            var result = MessageBox.Show("과목 이름 : " + subject + "\r" +
                             "담당 교수 : " + professor + "\r" +
                             "시험 시간 : " + testtime + "\r" +
                             "폴더 경로 : " + fp + "\r" +
                             "폴더 이름 : " + fn + "\r"
             , "확인해 주세요!!!!", MessageBoxButtons.YesNo, MessageBoxIcon.Warning);
            if( result == DialogResult.Yes)
            {
                btnStart.Enabled = true;
                myServer.allSendFile();
                dgStudent.Invalidate();
            }
            
        }

        private void btnEnd_Click(object sender, EventArgs e)
        {
            myServer.ExamEnd();
        }
    }
}

