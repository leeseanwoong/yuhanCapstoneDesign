
namespace CapstoneDProfessor
{
    partial class SetExam
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
            this.lblSubName = new System.Windows.Forms.Label();
            this.lblSetTime = new System.Windows.Forms.Label();
            this.lblSetProName = new System.Windows.Forms.Label();
            this.txtSubName = new System.Windows.Forms.TextBox();
            this.txtProName = new System.Windows.Forms.TextBox();
            this.txtTime = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.btnSetOk = new System.Windows.Forms.Button();
            this.btn30 = new System.Windows.Forms.Button();
            this.btn60 = new System.Windows.Forms.Button();
            this.btn90 = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // lblSubName
            // 
            this.lblSubName.AutoSize = true;
            this.lblSubName.Font = new System.Drawing.Font("돋움", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.lblSubName.Location = new System.Drawing.Point(33, 12);
            this.lblSubName.Name = "lblSubName";
            this.lblSubName.Size = new System.Drawing.Size(122, 21);
            this.lblSubName.TabIndex = 0;
            this.lblSubName.Text = "과목명 입력";
            // 
            // lblSetTime
            // 
            this.lblSetTime.AutoSize = true;
            this.lblSetTime.Font = new System.Drawing.Font("돋움", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.lblSetTime.Location = new System.Drawing.Point(33, 55);
            this.lblSetTime.Name = "lblSetTime";
            this.lblSetTime.Size = new System.Drawing.Size(143, 21);
            this.lblSetTime.TabIndex = 0;
            this.lblSetTime.Text = "시험시간 입력";
            // 
            // lblSetProName
            // 
            this.lblSetProName.AutoSize = true;
            this.lblSetProName.Font = new System.Drawing.Font("돋움", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.lblSetProName.Location = new System.Drawing.Point(33, 95);
            this.lblSetProName.Name = "lblSetProName";
            this.lblSetProName.Size = new System.Drawing.Size(143, 21);
            this.lblSetProName.TabIndex = 0;
            this.lblSetProName.Text = "담당교수 입력";
            // 
            // txtSubName
            // 
            this.txtSubName.Font = new System.Drawing.Font("돋움", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.txtSubName.Location = new System.Drawing.Point(194, 8);
            this.txtSubName.Name = "txtSubName";
            this.txtSubName.Size = new System.Drawing.Size(270, 29);
            this.txtSubName.TabIndex = 1;
            // 
            // txtProName
            // 
            this.txtProName.Font = new System.Drawing.Font("돋움", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.txtProName.Location = new System.Drawing.Point(194, 89);
            this.txtProName.Name = "txtProName";
            this.txtProName.Size = new System.Drawing.Size(273, 29);
            this.txtProName.TabIndex = 1;
            // 
            // txtTime
            // 
            this.txtTime.Font = new System.Drawing.Font("돋움", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.txtTime.Location = new System.Drawing.Point(393, 53);
            this.txtTime.Name = "txtTime";
            this.txtTime.Size = new System.Drawing.Size(50, 29);
            this.txtTime.TabIndex = 1;
            this.txtTime.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("돋움", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label5.Location = new System.Drawing.Point(442, 55);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(31, 21);
            this.label5.TabIndex = 0;
            this.label5.Text = "분";
            // 
            // btnSetOk
            // 
            this.btnSetOk.Location = new System.Drawing.Point(37, 141);
            this.btnSetOk.Name = "btnSetOk";
            this.btnSetOk.Size = new System.Drawing.Size(430, 40);
            this.btnSetOk.TabIndex = 2;
            this.btnSetOk.Text = "설정 완료";
            this.btnSetOk.UseVisualStyleBackColor = true;
            this.btnSetOk.Click += new System.EventHandler(this.btnSetOk_Click);
            // 
            // btn30
            // 
            this.btn30.Location = new System.Drawing.Point(193, 52);
            this.btn30.Name = "btn30";
            this.btn30.Size = new System.Drawing.Size(61, 30);
            this.btn30.TabIndex = 3;
            this.btn30.Text = "30분";
            this.btn30.UseVisualStyleBackColor = true;
            this.btn30.Click += new System.EventHandler(this.btn30_Click);
            // 
            // btn60
            // 
            this.btn60.Location = new System.Drawing.Point(260, 52);
            this.btn60.Name = "btn60";
            this.btn60.Size = new System.Drawing.Size(61, 30);
            this.btn60.TabIndex = 4;
            this.btn60.Text = "60분";
            this.btn60.UseVisualStyleBackColor = true;
            this.btn60.Click += new System.EventHandler(this.btn60_Click);
            // 
            // btn90
            // 
            this.btn90.Location = new System.Drawing.Point(326, 52);
            this.btn90.Name = "btn90";
            this.btn90.Size = new System.Drawing.Size(61, 30);
            this.btn90.TabIndex = 5;
            this.btn90.Text = "90분";
            this.btn90.UseVisualStyleBackColor = true;
            this.btn90.Click += new System.EventHandler(this.btn90_Click);
            // 
            // SetExam
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(509, 199);
            this.Controls.Add(this.btn90);
            this.Controls.Add(this.btn60);
            this.Controls.Add(this.btn30);
            this.Controls.Add(this.btnSetOk);
            this.Controls.Add(this.txtProName);
            this.Controls.Add(this.txtTime);
            this.Controls.Add(this.txtSubName);
            this.Controls.Add(this.lblSetProName);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.lblSetTime);
            this.Controls.Add(this.lblSubName);
            this.Name = "SetExam";
            this.Text = "SetExam";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lblSubName;
        private System.Windows.Forms.Label lblSetTime;
        private System.Windows.Forms.Label lblSetProName;
        private System.Windows.Forms.TextBox txtSubName;
        private System.Windows.Forms.TextBox txtProName;
        private System.Windows.Forms.TextBox txtTime;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Button btnSetOk;
        private System.Windows.Forms.Button btn30;
        private System.Windows.Forms.Button btn60;
        private System.Windows.Forms.Button btn90;
    }
}