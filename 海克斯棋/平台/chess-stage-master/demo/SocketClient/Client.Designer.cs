namespace SocketClient
{
    partial class Client
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
            this.connectBtn = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.notify = new System.Windows.Forms.Label();
            this.ableMoveCB = new System.Windows.Forms.ComboBox();
            this.button1 = new System.Windows.Forms.Button();
            this.button8 = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // connectBtn
            // 
            this.connectBtn.Location = new System.Drawing.Point(277, 40);
            this.connectBtn.Name = "connectBtn";
            this.connectBtn.Size = new System.Drawing.Size(122, 30);
            this.connectBtn.TabIndex = 1;
            this.connectBtn.Text = "连接服务器";
            this.connectBtn.UseVisualStyleBackColor = true;
            this.connectBtn.Click += new System.EventHandler(this.connectBtn_Click);
            // 
            // textBox1
            // 
            this.textBox1.AcceptsReturn = true;
            this.textBox1.Location = new System.Drawing.Point(200, 231);
            this.textBox1.MaxLength = 327670;
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(177, 36);
            this.textBox1.TabIndex = 2;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(26, 40);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(65, 12);
            this.label1.TabIndex = 3;
            this.label1.Text = "服务器地址";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(26, 79);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(65, 12);
            this.label2.TabIndex = 3;
            this.label2.Text = "服务器端口";
            // 
            // textBox2
            // 
            this.textBox2.Location = new System.Drawing.Point(116, 37);
            this.textBox2.Name = "textBox2";
            this.textBox2.Size = new System.Drawing.Size(100, 21);
            this.textBox2.TabIndex = 4;
            this.textBox2.Text = "127.0.0.1";
            // 
            // textBox3
            // 
            this.textBox3.Location = new System.Drawing.Point(116, 76);
            this.textBox3.Name = "textBox3";
            this.textBox3.Size = new System.Drawing.Size(53, 21);
            this.textBox3.TabIndex = 4;
            this.textBox3.Text = "10020";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.notify);
            this.groupBox1.Controls.Add(this.ableMoveCB);
            this.groupBox1.Controls.Add(this.button1);
            this.groupBox1.Controls.Add(this.button8);
            this.groupBox1.Controls.Add(this.textBox1);
            this.groupBox1.Controls.Add(this.textBox3);
            this.groupBox1.Controls.Add(this.connectBtn);
            this.groupBox1.Controls.Add(this.textBox2);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(449, 279);
            this.groupBox1.TabIndex = 6;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "TCP客户端区";
            // 
            // notify
            // 
            this.notify.AutoSize = true;
            this.notify.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.notify.ForeColor = System.Drawing.SystemColors.Desktop;
            this.notify.Location = new System.Drawing.Point(149, 191);
            this.notify.Name = "notify";
            this.notify.Size = new System.Drawing.Size(55, 14);
            this.notify.TabIndex = 10;
            this.notify.Text = "label4";
            // 
            // ableMoveCB
            // 
            this.ableMoveCB.FormattingEnabled = true;
            this.ableMoveCB.Location = new System.Drawing.Point(19, 155);
            this.ableMoveCB.Name = "ableMoveCB";
            this.ableMoveCB.Size = new System.Drawing.Size(305, 20);
            this.ableMoveCB.TabIndex = 8;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(345, 155);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 7;
            this.button1.Text = "发送走棋";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // button8
            // 
            this.button8.Location = new System.Drawing.Point(106, 231);
            this.button8.Name = "button8";
            this.button8.Size = new System.Drawing.Size(88, 30);
            this.button8.TabIndex = 6;
            this.button8.Text = "发送关闭api";
            this.button8.UseVisualStyleBackColor = true;
            this.button8.Click += new System.EventHandler(this.button8_Click);
            // 
            // Client
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(469, 303);
            this.Controls.Add(this.groupBox1);
            this.Name = "Client";
            this.Text = "客户端";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.Button connectBtn;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.TextBox textBox3;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button button8;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.ComboBox ableMoveCB;
        private System.Windows.Forms.Label notify;
    }
}