using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CapstoneDProfessor
{
    public delegate void DataGetEventHandler(string data);
    public partial class SetExam : Form
    {

        public delegate void FormSendEvent(string sub, string name, int time);
        public event FormSendEvent FormSend;
        public static int hour = 0;
        public static int min = 0;
        public static int sec = 0;
        int timee = 0;
        string s;
        string n;
        string t;

        public SetExam()
        {
            InitializeComponent();
        }



        private void btnSetOk_Click(object sender, EventArgs e) //정보 form1에 넘겨주기
        {
            min = int.Parse(txtTime.Text);

            timee = min;

            s = txtSubName.Text;
            n = txtProName.Text;
            FormSend(s, n, timee);
            this.Close();
            
        }


        private void btn30_Click(object sender, EventArgs e)
        {
            txtTime.Text = "30";
        }

        private void btn60_Click(object sender, EventArgs e)
        {
            txtTime.Text = "60";
        }

        private void btn90_Click(object sender, EventArgs e)
        {
            txtTime.Text = "90";
        }
    }
}
