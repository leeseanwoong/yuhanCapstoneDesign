using System;
using System.IO;
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
    public partial class WhiteList : Form
    {
        public WhiteList()
        {
            InitializeComponent();
        }

        private void WhiteList_Load(object sender, EventArgs e)
        {

            StreamReader streamReader = new StreamReader("WhiteList.txt");

            string a = "/";  // "/" 로 구분

            string line = null;
            lvWhitelist.Items.Clear();  // 데이터 초기화
            while ((line = streamReader.ReadLine()) != null)
            {
                string[] columns = line.Split(new string[] { a }, StringSplitOptions.None);

                ListViewItem lvt = new ListViewItem(columns[0]);
                for (int col = 1; col < columns.Length; col++)
                {
                    lvt.SubItems.Add(columns[col]);
                }
                lvWhitelist.Items.Add(lvt);
            }
            streamReader.Close();

        }

        private void lvWhitelist_SelectedIndexChanged(object sender, EventArgs e)
        {

        }
    }
}
