﻿namespace TestElsidi {
    partial class MainForm {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing) {
            if (disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            this.lblSerialPort = new System.Windows.Forms.Label();
            this.cmbSerialPort = new System.Windows.Forms.ComboBox();
            this.btnConnect = new System.Windows.Forms.Button();
            this.txtText = new System.Windows.Forms.TextBox();
            this.btnSend = new System.Windows.Forms.Button();
            this.btnAdjust = new System.Windows.Forms.Button();
            this.btnStress = new System.Windows.Forms.Button();
            this.btnClock = new System.Windows.Forms.Button();
            this.btnInit = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // lblSerialPort
            // 
            this.lblSerialPort.AutoSize = true;
            this.lblSerialPort.Location = new System.Drawing.Point(12, 15);
            this.lblSerialPort.Name = "lblSerialPort";
            this.lblSerialPort.Size = new System.Drawing.Size(77, 17);
            this.lblSerialPort.TabIndex = 0;
            this.lblSerialPort.Text = "Serial port:";
            // 
            // cmbSerialPort
            // 
            this.cmbSerialPort.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.cmbSerialPort.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbSerialPort.FormattingEnabled = true;
            this.cmbSerialPort.Location = new System.Drawing.Point(95, 12);
            this.cmbSerialPort.Name = "cmbSerialPort";
            this.cmbSerialPort.Size = new System.Drawing.Size(249, 24);
            this.cmbSerialPort.TabIndex = 1;
            this.cmbSerialPort.SelectedIndexChanged += new System.EventHandler(this.cmbSerialPort_SelectedIndexChanged);
            // 
            // btnConnect
            // 
            this.btnConnect.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnConnect.Enabled = false;
            this.btnConnect.Location = new System.Drawing.Point(350, 12);
            this.btnConnect.Name = "btnConnect";
            this.btnConnect.Size = new System.Drawing.Size(100, 23);
            this.btnConnect.TabIndex = 2;
            this.btnConnect.Text = "Connect";
            this.btnConnect.UseVisualStyleBackColor = true;
            this.btnConnect.Click += new System.EventHandler(this.btnConnect_Click);
            // 
            // txtText
            // 
            this.txtText.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtText.Enabled = false;
            this.txtText.Location = new System.Drawing.Point(12, 42);
            this.txtText.Multiline = true;
            this.txtText.Name = "txtText";
            this.txtText.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.txtText.Size = new System.Drawing.Size(438, 70);
            this.txtText.TabIndex = 3;
            this.txtText.WordWrap = false;
            this.txtText.KeyDown += new System.Windows.Forms.KeyEventHandler(this.txtText_KeyDown);
            // 
            // btnSend
            // 
            this.btnSend.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.btnSend.Enabled = false;
            this.btnSend.Location = new System.Drawing.Point(12, 118);
            this.btnSend.Name = "btnSend";
            this.btnSend.Size = new System.Drawing.Size(75, 23);
            this.btnSend.TabIndex = 4;
            this.btnSend.Text = "Send";
            this.btnSend.UseVisualStyleBackColor = true;
            this.btnSend.Click += new System.EventHandler(this.btnSend_Click);
            // 
            // btnAdjust
            // 
            this.btnAdjust.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnAdjust.Enabled = false;
            this.btnAdjust.Location = new System.Drawing.Point(375, 118);
            this.btnAdjust.Name = "btnAdjust";
            this.btnAdjust.Size = new System.Drawing.Size(75, 23);
            this.btnAdjust.TabIndex = 8;
            this.btnAdjust.Text = "Adjust";
            this.btnAdjust.UseVisualStyleBackColor = true;
            this.btnAdjust.Click += new System.EventHandler(this.btnAdjust_Click);
            // 
            // btnStress
            // 
            this.btnStress.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.btnStress.Enabled = false;
            this.btnStress.Location = new System.Drawing.Point(180, 118);
            this.btnStress.Name = "btnStress";
            this.btnStress.Size = new System.Drawing.Size(75, 23);
            this.btnStress.TabIndex = 6;
            this.btnStress.Text = "Stress";
            this.btnStress.UseVisualStyleBackColor = true;
            this.btnStress.Click += new System.EventHandler(this.btnStress_Click);
            // 
            // btnClock
            // 
            this.btnClock.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.btnClock.Enabled = false;
            this.btnClock.Location = new System.Drawing.Point(99, 118);
            this.btnClock.Margin = new System.Windows.Forms.Padding(9, 3, 3, 3);
            this.btnClock.Name = "btnClock";
            this.btnClock.Size = new System.Drawing.Size(75, 23);
            this.btnClock.TabIndex = 5;
            this.btnClock.Text = "Clock";
            this.btnClock.UseVisualStyleBackColor = true;
            this.btnClock.Click += new System.EventHandler(this.btnClock_Click);
            // 
            // btnInit
            // 
            this.btnInit.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnInit.Enabled = false;
            this.btnInit.Location = new System.Drawing.Point(288, 118);
            this.btnInit.Margin = new System.Windows.Forms.Padding(3, 3, 9, 3);
            this.btnInit.Name = "btnInit";
            this.btnInit.Size = new System.Drawing.Size(75, 23);
            this.btnInit.TabIndex = 7;
            this.btnInit.Text = "Reinit";
            this.btnInit.UseVisualStyleBackColor = true;
            this.btnInit.Click += new System.EventHandler(this.btnInit_Click);
            // 
            // MainForm
            // 
            this.AcceptButton = this.btnConnect;
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(462, 153);
            this.Controls.Add(this.btnInit);
            this.Controls.Add(this.btnClock);
            this.Controls.Add(this.btnStress);
            this.Controls.Add(this.btnAdjust);
            this.Controls.Add(this.btnSend);
            this.Controls.Add(this.txtText);
            this.Controls.Add(this.btnConnect);
            this.Controls.Add(this.cmbSerialPort);
            this.Controls.Add(this.lblSerialPort);
            this.MinimumSize = new System.Drawing.Size(480, 200);
            this.Name = "MainForm";
            this.Text = "Elsidi test";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lblSerialPort;
        private System.Windows.Forms.ComboBox cmbSerialPort;
        private System.Windows.Forms.Button btnConnect;
        private System.Windows.Forms.TextBox txtText;
        private System.Windows.Forms.Button btnSend;
        private System.Windows.Forms.Button btnAdjust;
        private System.Windows.Forms.Button btnStress;
        private System.Windows.Forms.Button btnClock;
        private System.Windows.Forms.Button btnInit;
    }
}