﻿namespace TestElsidi {
    partial class AdjustForm {
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
            this.lblBacklight = new System.Windows.Forms.Label();
            this.nudBacklight = new System.Windows.Forms.NumericUpDown();
            this.lblContrast = new System.Windows.Forms.Label();
            this.nudContrast = new System.Windows.Forms.NumericUpDown();
            this.btnSave = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.nudBacklight)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudContrast)).BeginInit();
            this.SuspendLayout();
            // 
            // lblBacklight
            // 
            this.lblBacklight.AutoSize = true;
            this.lblBacklight.Location = new System.Drawing.Point(12, 42);
            this.lblBacklight.Name = "lblBacklight";
            this.lblBacklight.Size = new System.Drawing.Size(69, 17);
            this.lblBacklight.TabIndex = 10;
            this.lblBacklight.Text = "Backlight:";
            // 
            // nudBacklight
            // 
            this.nudBacklight.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.nudBacklight.Location = new System.Drawing.Point(110, 40);
            this.nudBacklight.Name = "nudBacklight";
            this.nudBacklight.Size = new System.Drawing.Size(60, 22);
            this.nudBacklight.TabIndex = 11;
            this.nudBacklight.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.nudBacklight.ValueChanged += new System.EventHandler(this.nudBacklight_ValueChanged);
            // 
            // lblContrast
            // 
            this.lblContrast.AutoSize = true;
            this.lblContrast.Location = new System.Drawing.Point(12, 14);
            this.lblContrast.Name = "lblContrast";
            this.lblContrast.Size = new System.Drawing.Size(65, 17);
            this.lblContrast.TabIndex = 8;
            this.lblContrast.Text = "Contrast:";
            // 
            // nudContrast
            // 
            this.nudContrast.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.nudContrast.Location = new System.Drawing.Point(110, 12);
            this.nudContrast.Name = "nudContrast";
            this.nudContrast.Size = new System.Drawing.Size(60, 22);
            this.nudContrast.TabIndex = 9;
            this.nudContrast.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.nudContrast.Value = new decimal(new int[] {
            50,
            0,
            0,
            0});
            this.nudContrast.ValueChanged += new System.EventHandler(this.nudContrast_ValueChanged);
            // 
            // btnSave
            // 
            this.btnSave.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnSave.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.btnSave.Location = new System.Drawing.Point(14, 80);
            this.btnSave.Margin = new System.Windows.Forms.Padding(3, 15, 3, 3);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(75, 23);
            this.btnSave.TabIndex = 12;
            this.btnSave.Text = "Save";
            this.btnSave.UseVisualStyleBackColor = true;
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
            // 
            // btnCancel
            // 
            this.btnCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(95, 80);
            this.btnCancel.Margin = new System.Windows.Forms.Padding(3, 15, 3, 3);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(75, 23);
            this.btnCancel.TabIndex = 13;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            // 
            // AdjustForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.btnCancel;
            this.ClientSize = new System.Drawing.Size(182, 115);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnSave);
            this.Controls.Add(this.lblBacklight);
            this.Controls.Add(this.nudBacklight);
            this.Controls.Add(this.lblContrast);
            this.Controls.Add(this.nudContrast);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "AdjustForm";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Adjust";
            this.Load += new System.EventHandler(this.Form_Load);
            ((System.ComponentModel.ISupportInitialize)(this.nudBacklight)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudContrast)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lblBacklight;
        private System.Windows.Forms.NumericUpDown nudBacklight;
        private System.Windows.Forms.Label lblContrast;
        private System.Windows.Forms.NumericUpDown nudContrast;
        private System.Windows.Forms.Button btnSave;
        private System.Windows.Forms.Button btnCancel;
    }
}