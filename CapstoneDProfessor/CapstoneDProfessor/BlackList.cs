using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace CapstoneDProfessor
{
    public partial class BlackList : Form
    {
        


        public BlackList()
        {
            InitializeComponent();
            
        }

        private void BlackList_Load(object sender, EventArgs e)
        {
            StreamReader streamReader = new StreamReader("BlackList.txt");

            string a = "/";  // "/" 로 구분
            
                string line = null;
                lvBlacklist.Items.Clear();  // 데이터 초기화
                while ((line = streamReader.ReadLine()) != null)
                {
                    string[] columns = line.Split(new string[] { a }, StringSplitOptions.None);

                    ListViewItem lvt = new ListViewItem(columns[0]);
                    for (int col = 1; col < columns.Length; col++)
                    {
                        lvt.SubItems.Add(columns[col]);
                    }
                    lvBlacklist.Items.Add(lvt);
                }
            streamReader.Close();
            
        }

        private void listView1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }
    }
}
