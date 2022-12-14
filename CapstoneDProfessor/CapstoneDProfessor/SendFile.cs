using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Microsoft.WindowsAPICodePack.Dialogs;

namespace CapstoneDProfessor
{
    public partial class SendFile : Form
    {
        public delegate void FormSendFileEvent(string a,string b,string c);
        public event FormSendFileEvent FormSendF;

        string folderpath;
        string foldername;
        string recvname;

        

        public SendFile()
        {
            InitializeComponent();
            txt_FileName.Text = "ExamFile";
        } 

        private void btn_dir_Click(object sender, EventArgs e)
        {
            CommonOpenFileDialog commonOpenFileDialog = new CommonOpenFileDialog();
            commonOpenFileDialog.IsFolderPicker = true;
            if (commonOpenFileDialog.ShowDialog() == CommonFileDialogResult.Ok)
            {
                //lbl_FolderPath.Text = "폴더 주소 : " + commonOpenFileDialog.FileName;
                folderpath = commonOpenFileDialog.FileName;
                txtFolderPath.Text = folderpath;
                //lbl_FolderName.Text = "폴더 이름 : " + System.IO.Path.GetFileName(folderpath);
                foldername = System.IO.Path.GetFileName(folderpath);
                txtFolderName.Text = foldername;
            }
            recvname = txt_FileName.Text;

        }

        public string GetPath
        {
            get { return this.folderpath; }
            set
            {
                this.folderpath = value;
            }

        }

        private void btn_Ok_Click(object sender, EventArgs e)
        {
            if (folderpath == txtFolderPath.Text)
            {
                FormSendF(folderpath, foldername, recvname);
                this.Close();
            }
        }
        private void txtFolderName_TextChanged(object sender, EventArgs e)
        {
            // lbl_FolderName.Text = "폴더 이름 : " + System.IO.Path.GetFileName(folderpath);
            foldername = txtFolderName.Text;
        }

        private void SendFile_Load(object sender, EventArgs e)
        {
            txtFolderPath.Text = folderpath;
            txtFolderName.Text = System.IO.Path.GetFileName(folderpath);
        }

    }
}
