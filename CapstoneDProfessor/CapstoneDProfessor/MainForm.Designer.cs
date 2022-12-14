
namespace CapstoneDProfessor
{
    partial class MainForm
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.btnsetExam = new System.Windows.Forms.Button();
            this.btnSend = new System.Windows.Forms.Button();
            this.btnStart = new System.Windows.Forms.Button();
            this.btnBlack = new System.Windows.Forms.Button();
            this.btnWhite = new System.Windows.Forms.Button();
            this.dgStudent = new System.Windows.Forms.DataGridView();
            this.lblsubName = new System.Windows.Forms.Label();
            this.lblName = new System.Windows.Forms.Label();
            this.lblTime = new System.Windows.Forms.Label();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.btnEnd = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.lblIpaddress = new System.Windows.Forms.Label();
            this.lblport = new System.Windows.Forms.Label();
            this.hak = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.name = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.state = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.filesend = new System.Windows.Forms.DataGridViewTextBoxColumn();
            ((System.ComponentModel.ISupportInitialize)(this.dgStudent)).BeginInit();
            this.SuspendLayout();
            // 
            // btnsetExam
            // 
            this.btnsetExam.Font = new System.Drawing.Font("굴림", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.btnsetExam.Location = new System.Drawing.Point(533, 119);
            this.btnsetExam.Name = "btnsetExam";
            this.btnsetExam.Size = new System.Drawing.Size(152, 154);
            this.btnsetExam.TabIndex = 2;
            this.btnsetExam.Text = "시험 설정";
            this.btnsetExam.UseVisualStyleBackColor = true;
            this.btnsetExam.Click += new System.EventHandler(this.btnsetExam_Click);
            // 
            // btnSend
            // 
            this.btnSend.Font = new System.Drawing.Font("굴림", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.btnSend.Location = new System.Drawing.Point(696, 119);
            this.btnSend.Name = "btnSend";
            this.btnSend.Size = new System.Drawing.Size(152, 154);
            this.btnSend.TabIndex = 2;
            this.btnSend.Text = "문제 선택";
            this.btnSend.UseVisualStyleBackColor = true;
            this.btnSend.Click += new System.EventHandler(this.btnSend_Click);
            // 
            // btnStart
            // 
            this.btnStart.Font = new System.Drawing.Font("굴림", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.btnStart.Location = new System.Drawing.Point(533, 279);
            this.btnStart.Name = "btnStart";
            this.btnStart.Size = new System.Drawing.Size(232, 154);
            this.btnStart.TabIndex = 2;
            this.btnStart.Text = "시험 시작";
            this.btnStart.UseVisualStyleBackColor = true;
            this.btnStart.Click += new System.EventHandler(this.btnStart_Click);
            // 
            // btnBlack
            // 
            this.btnBlack.Font = new System.Drawing.Font("굴림", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.btnBlack.Location = new System.Drawing.Point(533, 439);
            this.btnBlack.Name = "btnBlack";
            this.btnBlack.Size = new System.Drawing.Size(232, 154);
            this.btnBlack.TabIndex = 2;
            this.btnBlack.Text = "블랙 리스트";
            this.btnBlack.UseVisualStyleBackColor = true;
            this.btnBlack.Click += new System.EventHandler(this.btnBlack_Click);
            // 
            // btnWhite
            // 
            this.btnWhite.Font = new System.Drawing.Font("굴림", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.btnWhite.Location = new System.Drawing.Point(779, 439);
            this.btnWhite.Name = "btnWhite";
            this.btnWhite.Size = new System.Drawing.Size(232, 154);
            this.btnWhite.TabIndex = 2;
            this.btnWhite.Text = "화이트 리스트";
            this.btnWhite.UseVisualStyleBackColor = true;
            this.btnWhite.Click += new System.EventHandler(this.btnWhite_Click);
            // 
            // dgStudent
            // 
            this.dgStudent.AllowUserToAddRows = false;
            this.dgStudent.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.Fill;
            this.dgStudent.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dgStudent.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.hak,
            this.name,
            this.state,
            this.filesend});
            this.dgStudent.Location = new System.Drawing.Point(31, 119);
            this.dgStudent.Name = "dgStudent";
            this.dgStudent.ReadOnly = true;
            this.dgStudent.RowHeadersVisible = false;
            this.dgStudent.RowTemplate.Height = 23;
            this.dgStudent.Size = new System.Drawing.Size(482, 474);
            this.dgStudent.TabIndex = 3;
            this.dgStudent.CellContentClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dgStudent_CellContentClick);
            // 
            // lblsubName
            // 
            this.lblsubName.AutoSize = true;
            this.lblsubName.Font = new System.Drawing.Font("맑은 고딕", 20F, System.Drawing.FontStyle.Bold);
            this.lblsubName.Location = new System.Drawing.Point(24, 16);
            this.lblsubName.Name = "lblsubName";
            this.lblsubName.Size = new System.Drawing.Size(150, 37);
            this.lblsubName.TabIndex = 4;
            this.lblsubName.Text = "과목 이름 :";
            // 
            // lblName
            // 
            this.lblName.AutoSize = true;
            this.lblName.Font = new System.Drawing.Font("맑은 고딕", 20F, System.Drawing.FontStyle.Bold);
            this.lblName.Location = new System.Drawing.Point(21, 65);
            this.lblName.Name = "lblName";
            this.lblName.Size = new System.Drawing.Size(150, 37);
            this.lblName.TabIndex = 5;
            this.lblName.Text = "담당 교수 :";
            this.lblName.Click += new System.EventHandler(this.lblName_Click);
            // 
            // lblTime
            // 
            this.lblTime.AutoSize = true;
            this.lblTime.Font = new System.Drawing.Font("맑은 고딕", 25F, System.Drawing.FontStyle.Bold);
            this.lblTime.Location = new System.Drawing.Point(818, 31);
            this.lblTime.Name = "lblTime";
            this.lblTime.Size = new System.Drawing.Size(88, 46);
            this.lblTime.TabIndex = 6;
            this.lblTime.Text = "시간";
            this.lblTime.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // timer1
            // 
            this.timer1.Interval = 1000;
            this.timer1.Tick += new System.EventHandler(this.Timer_Tick);
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            this.openFileDialog1.FileOk += new System.ComponentModel.CancelEventHandler(this.openFileDialog1_FileOk);
            // 
            // btnEnd
            // 
            this.btnEnd.Font = new System.Drawing.Font("굴림", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.btnEnd.Location = new System.Drawing.Point(779, 279);
            this.btnEnd.Name = "btnEnd";
            this.btnEnd.Size = new System.Drawing.Size(232, 154);
            this.btnEnd.TabIndex = 8;
            this.btnEnd.Text = "시험 종료";
            this.btnEnd.UseVisualStyleBackColor = true;
            this.btnEnd.Click += new System.EventHandler(this.btnEnd_Click);
            // 
            // button1
            // 
            this.button1.Font = new System.Drawing.Font("굴림", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.button1.Location = new System.Drawing.Point(859, 119);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(152, 154);
            this.button1.TabIndex = 9;
            this.button1.Text = "시험 준비";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // lblIpaddress
            // 
            this.lblIpaddress.AutoSize = true;
            this.lblIpaddress.Font = new System.Drawing.Font("맑은 고딕", 20F, System.Drawing.FontStyle.Bold);
            this.lblIpaddress.Location = new System.Drawing.Point(423, 65);
            this.lblIpaddress.Name = "lblIpaddress";
            this.lblIpaddress.Size = new System.Drawing.Size(141, 37);
            this.lblIpaddress.TabIndex = 10;
            this.lblIpaddress.Text = "아 이 피  :";
            // 
            // lblport
            // 
            this.lblport.AutoSize = true;
            this.lblport.Font = new System.Drawing.Font("맑은 고딕", 20F, System.Drawing.FontStyle.Bold);
            this.lblport.Location = new System.Drawing.Point(423, 16);
            this.lblport.Name = "lblport";
            this.lblport.Size = new System.Drawing.Size(141, 37);
            this.lblport.TabIndex = 11;
            this.lblport.Text = "포트번호 :";
            // 
            // hak
            // 
            this.hak.DataPropertyName = "hak";
            this.hak.FillWeight = 119.797F;
            this.hak.HeaderText = "학번";
            this.hak.Name = "hak";
            this.hak.ReadOnly = true;
            // 
            // name
            // 
            this.name.DataPropertyName = "name";
            this.name.FillWeight = 119.797F;
            this.name.HeaderText = "이름";
            this.name.Name = "name";
            this.name.ReadOnly = true;
            // 
            // state
            // 
            this.state.DataPropertyName = "state";
            this.state.FillWeight = 119.797F;
            this.state.HeaderText = "상태";
            this.state.Name = "state";
            this.state.ReadOnly = true;
            // 
            // filesend
            // 
            this.filesend.DataPropertyName = "filesend";
            this.filesend.FillWeight = 40F;
            this.filesend.HeaderText = "전송";
            this.filesend.Name = "filesend";
            this.filesend.ReadOnly = true;
            this.filesend.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            this.filesend.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1040, 600);
            this.Controls.Add(this.lblport);
            this.Controls.Add(this.lblIpaddress);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.btnEnd);
            this.Controls.Add(this.lblTime);
            this.Controls.Add(this.lblName);
            this.Controls.Add(this.lblsubName);
            this.Controls.Add(this.dgStudent);
            this.Controls.Add(this.btnWhite);
            this.Controls.Add(this.btnBlack);
            this.Controls.Add(this.btnStart);
            this.Controls.Add(this.btnSend);
            this.Controls.Add(this.btnsetExam);
            this.Name = "MainForm";
            this.Load += new System.EventHandler(this.MainForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.dgStudent)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Button btnsetExam;
        private System.Windows.Forms.Button btnSend;
        private System.Windows.Forms.Button btnStart;
        private System.Windows.Forms.Button btnBlack;
        private System.Windows.Forms.Button btnWhite;
        private System.Windows.Forms.Label lblsubName;
        private System.Windows.Forms.Label lblName;
        private System.Windows.Forms.Label lblTime;
        private System.Windows.Forms.Timer timer1;
        public System.Windows.Forms.DataGridView dgStudent;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.Button btnEnd;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Label lblIpaddress;
        private System.Windows.Forms.Label lblport;
        private System.Windows.Forms.DataGridViewTextBoxColumn hak;
        private System.Windows.Forms.DataGridViewTextBoxColumn name;
        private System.Windows.Forms.DataGridViewTextBoxColumn state;
        private System.Windows.Forms.DataGridViewTextBoxColumn filesend;
    }
}

