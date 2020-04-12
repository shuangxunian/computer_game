namespace SocketServer
{
    partial class Server
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
            this.label_zt = new System.Windows.Forms.Label();
            this.label_all = new System.Windows.Forms.Label();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.listView1 = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.button1 = new System.Windows.Forms.Button();
            this.textBox_port = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.layerImageBox = new System.Windows.Forms.PictureBox();
            this.chessPanel = new System.Windows.Forms.Panel();
            this.resultLabel = new System.Windows.Forms.Label();
            this.groupBox3.SuspendLayout();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.layerImageBox)).BeginInit();
            this.chessPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // label_zt
            // 
            this.label_zt.AutoSize = true;
            this.label_zt.Location = new System.Drawing.Point(31, 672);
            this.label_zt.Name = "label_zt";
            this.label_zt.Size = new System.Drawing.Size(17, 12);
            this.label_zt.TabIndex = 12;
            this.label_zt.Text = "--";
            // 
            // label_all
            // 
            this.label_all.AutoSize = true;
            this.label_all.Location = new System.Drawing.Point(304, 672);
            this.label_all.Name = "label_all";
            this.label_all.Size = new System.Drawing.Size(89, 12);
            this.label_all.TabIndex = 13;
            this.label_all.Text = "当前在线人数:0";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.textBox1);
            this.groupBox3.Controls.Add(this.label_zt);
            this.groupBox3.Controls.Add(this.label_all);
            this.groupBox3.Controls.Add(this.groupBox2);
            this.groupBox3.Controls.Add(this.button1);
            this.groupBox3.Controls.Add(this.textBox_port);
            this.groupBox3.Controls.Add(this.label1);
            this.groupBox3.Location = new System.Drawing.Point(12, 12);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(414, 687);
            this.groupBox3.TabIndex = 15;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "TCP服务器";
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(33, 389);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(337, 255);
            this.textBox1.TabIndex = 20;
            // 
            // groupBox2
            // 
            this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox2.Controls.Add(this.listView1);
            this.groupBox2.Location = new System.Drawing.Point(9, 54);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(398, 329);
            this.groupBox2.TabIndex = 19;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "收到的消息";
            // 
            // listView1
            // 
            this.listView1.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader3});
            this.listView1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listView1.FullRowSelect = true;
            this.listView1.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.listView1.Location = new System.Drawing.Point(3, 17);
            this.listView1.Name = "listView1";
            this.listView1.ShowItemToolTips = true;
            this.listView1.Size = new System.Drawing.Size(392, 309);
            this.listView1.TabIndex = 10;
            this.listView1.UseCompatibleStateImageBehavior = false;
            this.listView1.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "时间";
            this.columnHeader1.Width = 78;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "客户端";
            this.columnHeader2.Width = 100;
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "消息内容";
            this.columnHeader3.Width = 300;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(208, 16);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 17;
            this.button1.Text = "启动";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // textBox_port
            // 
            this.textBox_port.Location = new System.Drawing.Point(102, 17);
            this.textBox_port.Name = "textBox_port";
            this.textBox_port.Size = new System.Drawing.Size(100, 21);
            this.textBox_port.TabIndex = 16;
            this.textBox_port.Text = "10020";
            // 
            // label1
            // 
            this.label1.Location = new System.Drawing.Point(7, 16);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(87, 23);
            this.label1.TabIndex = 15;
            this.label1.Text = "监听端口：";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // layerImageBox
            // 
            this.layerImageBox.Location = new System.Drawing.Point(0, 0);
            this.layerImageBox.Name = "layerImageBox";
            this.layerImageBox.Size = new System.Drawing.Size(600, 800);
            this.layerImageBox.TabIndex = 16;
            this.layerImageBox.TabStop = false;
            // 
            // chessPanel
            // 
            this.chessPanel.BackColor = System.Drawing.SystemColors.Window;
            this.chessPanel.Controls.Add(this.layerImageBox);
            this.chessPanel.Location = new System.Drawing.Point(453, 12);
            this.chessPanel.Name = "chessPanel";
            this.chessPanel.Size = new System.Drawing.Size(600, 800);
            this.chessPanel.TabIndex = 17;
            // 
            // resultLabel
            // 
            this.resultLabel.AutoSize = true;
            this.resultLabel.Font = new System.Drawing.Font("宋体", 72F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.resultLabel.Location = new System.Drawing.Point(90, 715);
            this.resultLabel.Name = "resultLabel";
            this.resultLabel.Size = new System.Drawing.Size(140, 97);
            this.resultLabel.TabIndex = 18;
            this.resultLabel.Text = ":)";
            // 
            // Server
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1072, 837);
            this.Controls.Add(this.resultLabel);
            this.Controls.Add(this.chessPanel);
            this.Controls.Add(this.groupBox3);
            this.Name = "Server";
            this.Text = "服务器";
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.layerImageBox)).EndInit();
            this.chessPanel.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label_zt;
        private System.Windows.Forms.Label label_all;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.ListView listView1;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.ColumnHeader columnHeader3;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TextBox textBox_port;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.PictureBox layerImageBox;
        private System.Windows.Forms.Panel chessPanel;
        private System.Windows.Forms.Label resultLabel;
    }
}