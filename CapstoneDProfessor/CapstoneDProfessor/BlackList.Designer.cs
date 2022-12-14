
namespace CapstoneDProfessor
{
    partial class BlackList
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
            this.lvBlacklist = new System.Windows.Forms.ListView();
            this.BlackListColumn = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.SuspendLayout();
            // 
            // lvBlacklist
            // 
            this.lvBlacklist.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.BlackListColumn});
            this.lvBlacklist.Font = new System.Drawing.Font("굴림", 14F);
            this.lvBlacklist.FullRowSelect = true;
            this.lvBlacklist.GridLines = true;
            this.lvBlacklist.HideSelection = false;
            this.lvBlacklist.Location = new System.Drawing.Point(12, 12);
            this.lvBlacklist.Name = "lvBlacklist";
            this.lvBlacklist.Size = new System.Drawing.Size(225, 534);
            this.lvBlacklist.TabIndex = 0;
            this.lvBlacklist.UseCompatibleStateImageBehavior = false;
            this.lvBlacklist.View = System.Windows.Forms.View.Details;
            this.lvBlacklist.SelectedIndexChanged += new System.EventHandler(this.listView1_SelectedIndexChanged);
            // 
            // BlackListColumn
            // 
            this.BlackListColumn.Text = "BlackList";
            this.BlackListColumn.Width = 221;
            // 
            // BlackList
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(251, 564);
            this.Controls.Add(this.lvBlacklist);
            this.Name = "BlackList";
            this.Text = "BlackList";
            this.Load += new System.EventHandler(this.BlackList_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListView lvBlacklist;
        private System.Windows.Forms.ColumnHeader BlackListColumn;
    }
}