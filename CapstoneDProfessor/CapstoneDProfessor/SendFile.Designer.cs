
namespace CapstoneDProfessor
{
    partial class SendFile
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.lbl_FolderPath = new System.Windows.Forms.Label();
            this.btn_Dir = new System.Windows.Forms.Button();
            this.btn_Ok = new System.Windows.Forms.Button();
            this.lbl_FolderName = new System.Windows.Forms.Label();
            this.txtFolderPath = new System.Windows.Forms.TextBox();
            this.txtFolderName = new System.Windows.Forms.TextBox();
            this.txt_FileName = new System.Windows.Forms.TextBox();
            this.lbl_FileName = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // lbl_FolderPath
            // 
            this.lbl_FolderPath.AutoSize = true;
            this.lbl_FolderPath.Font = new System.Drawing.Font("굴림", 14F);
            this.lbl_FolderPath.Location = new System.Drawing.Point(11, 12);
            this.lbl_FolderPath.Name = "lbl_FolderPath";
            this.lbl_FolderPath.Size = new System.Drawing.Size(103, 19);
            this.lbl_FolderPath.TabIndex = 0;
            this.lbl_FolderPath.Text = "폴더 주소 :";
            // 
            // btn_Dir
            // 
            this.btn_Dir.Font = new System.Drawing.Font("굴림", 11F);
            this.btn_Dir.Location = new System.Drawing.Point(466, 86);
            this.btn_Dir.Name = "btn_Dir";
            this.btn_Dir.Size = new System.Drawing.Size(75, 31);
            this.btn_Dir.TabIndex = 1;
            this.btn_Dir.Text = "폴더선택";
            this.btn_Dir.UseVisualStyleBackColor = true;
            this.btn_Dir.Click += new System.EventHandler(this.btn_dir_Click);
            // 
            // btn_Ok
            // 
            this.btn_Ok.Font = new System.Drawing.Font("굴림", 11F);
            this.btn_Ok.Location = new System.Drawing.Point(547, 85);
            this.btn_Ok.Name = "btn_Ok";
            this.btn_Ok.Size = new System.Drawing.Size(75, 31);
            this.btn_Ok.TabIndex = 2;
            this.btn_Ok.Text = "완료";
            this.btn_Ok.UseVisualStyleBackColor = true;
            this.btn_Ok.Click += new System.EventHandler(this.btn_Ok_Click);
            // 
            // lbl_FolderName
            // 
            this.lbl_FolderName.AutoSize = true;
            this.lbl_FolderName.Font = new System.Drawing.Font("굴림", 14F);
            this.lbl_FolderName.Location = new System.Drawing.Point(12, 51);
            this.lbl_FolderName.Name = "lbl_FolderName";
            this.lbl_FolderName.Size = new System.Drawing.Size(103, 19);
            this.lbl_FolderName.TabIndex = 4;
            this.lbl_FolderName.Text = "폴더 이름 :";
            // 
            // txtFolderPath
            // 
            this.txtFolderPath.Font = new System.Drawing.Font("굴림", 11F);
            this.txtFolderPath.Location = new System.Drawing.Point(120, 10);
            this.txtFolderPath.Name = "txtFolderPath";
            this.txtFolderPath.ReadOnly = true;
            this.txtFolderPath.Size = new System.Drawing.Size(502, 24);
            this.txtFolderPath.TabIndex = 5;
            // 
            // txtFolderName
            // 
            this.txtFolderName.Font = new System.Drawing.Font("굴림", 11F);
            this.txtFolderName.Location = new System.Drawing.Point(120, 49);
            this.txtFolderName.Name = "txtFolderName";
            this.txtFolderName.ReadOnly = true;
            this.txtFolderName.Size = new System.Drawing.Size(502, 24);
            this.txtFolderName.TabIndex = 6;
            this.txtFolderName.TextChanged += new System.EventHandler(this.txtFolderName_TextChanged);
            // 
            // txt_FileName
            // 
            this.txt_FileName.BackColor = System.Drawing.SystemColors.Control;
            this.txt_FileName.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.txt_FileName.Font = new System.Drawing.Font("굴림", 14F);
            this.txt_FileName.Location = new System.Drawing.Point(120, 87);
            this.txt_FileName.Name = "txt_FileName";
            this.txt_FileName.Size = new System.Drawing.Size(138, 22);
            this.txt_FileName.TabIndex = 8;
            // 
            // lbl_FileName
            // 
            this.lbl_FileName.AutoSize = true;
            this.lbl_FileName.Font = new System.Drawing.Font("굴림", 14F);
            this.lbl_FileName.Location = new System.Drawing.Point(12, 89);
            this.lbl_FileName.Name = "lbl_FileName";
            this.lbl_FileName.Size = new System.Drawing.Size(103, 19);
            this.lbl_FileName.TabIndex = 9;
            this.lbl_FileName.Text = "파일 이름 :";
            // 
            // SendFile
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(637, 132);
            this.Controls.Add(this.lbl_FileName);
            this.Controls.Add(this.txt_FileName);
            this.Controls.Add(this.txtFolderName);
            this.Controls.Add(this.txtFolderPath);
            this.Controls.Add(this.lbl_FolderName);
            this.Controls.Add(this.btn_Ok);
            this.Controls.Add(this.btn_Dir);
            this.Controls.Add(this.lbl_FolderPath);
            this.Name = "SendFile";
            this.Text = "SendFile";
            this.Load += new System.EventHandler(this.SendFile_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lbl_FolderPath;
        private System.Windows.Forms.Button btn_Dir;
        private System.Windows.Forms.Button btn_Ok;
        private System.Windows.Forms.Label lbl_FolderName;
        private System.Windows.Forms.TextBox txtFolderPath;
        private System.Windows.Forms.TextBox txtFolderName;
        private System.Windows.Forms.TextBox txt_FileName;
        private System.Windows.Forms.Label lbl_FileName;
    }
}