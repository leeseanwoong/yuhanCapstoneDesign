
namespace CapstoneDProfessor
{
    partial class WhiteList
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
            this.lvWhitelist = new System.Windows.Forms.ListView();
            this.WhiteListColumn = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.SuspendLayout();
            // 
            // lvWhitelist
            // 
            this.lvWhitelist.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.WhiteListColumn});
            this.lvWhitelist.Font = new System.Drawing.Font("굴림", 14F);
            this.lvWhitelist.FullRowSelect = true;
            this.lvWhitelist.GridLines = true;
            this.lvWhitelist.HideSelection = false;
            this.lvWhitelist.Location = new System.Drawing.Point(12, 12);
            this.lvWhitelist.Name = "lvWhitelist";
            this.lvWhitelist.Size = new System.Drawing.Size(243, 534);
            this.lvWhitelist.TabIndex = 0;
            this.lvWhitelist.TileSize = new System.Drawing.Size(228, 28);
            this.lvWhitelist.UseCompatibleStateImageBehavior = false;
            this.lvWhitelist.View = System.Windows.Forms.View.Details;
            this.lvWhitelist.SelectedIndexChanged += new System.EventHandler(this.lvWhitelist_SelectedIndexChanged);
            // 
            // WhiteListColumn
            // 
            this.WhiteListColumn.Text = "WhiteList";
            this.WhiteListColumn.Width = 220;
            // 
            // WhiteList
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(267, 555);
            this.Controls.Add(this.lvWhitelist);
            this.Name = "WhiteList";
            this.Text = "WhiteList";
            this.Load += new System.EventHandler(this.WhiteList_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListView lvWhitelist;
        private System.Windows.Forms.ColumnHeader WhiteListColumn;
    }
}