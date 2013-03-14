namespace TestElsidi {
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
            this.components = new System.ComponentModel.Container();
            this.btnSave = new System.Windows.Forms.Button();
            this.btnClose = new System.Windows.Forms.Button();
            this.grpLcdProperties = new System.Windows.Forms.GroupBox();
            this.lblHeight = new System.Windows.Forms.Label();
            this.nudHeight = new System.Windows.Forms.NumericUpDown();
            this.lblWidth = new System.Windows.Forms.Label();
            this.nudWidth = new System.Windows.Forms.NumericUpDown();
            this.lblDataBusWidth = new System.Windows.Forms.Label();
            this.dudDataBusWidth = new System.Windows.Forms.DomainUpDown();
            this.lblBacklight = new System.Windows.Forms.Label();
            this.nudBacklight = new System.Windows.Forms.NumericUpDown();
            this.lblContrast = new System.Windows.Forms.Label();
            this.nudContrast = new System.Windows.Forms.NumericUpDown();
            this.erp = new System.Windows.Forms.ErrorProvider(this.components);
            this.grpLcdProperties.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudHeight)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudWidth)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudBacklight)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudContrast)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.erp)).BeginInit();
            this.SuspendLayout();
            // 
            // btnSave
            // 
            this.btnSave.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnSave.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.btnSave.Location = new System.Drawing.Point(54, 203);
            this.btnSave.Margin = new System.Windows.Forms.Padding(3, 15, 3, 3);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(75, 23);
            this.btnSave.TabIndex = 1;
            this.btnSave.Text = "Save";
            this.btnSave.UseVisualStyleBackColor = true;
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
            // 
            // btnClose
            // 
            this.btnClose.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnClose.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnClose.Location = new System.Drawing.Point(135, 203);
            this.btnClose.Margin = new System.Windows.Forms.Padding(3, 15, 3, 3);
            this.btnClose.Name = "btnClose";
            this.btnClose.Size = new System.Drawing.Size(75, 23);
            this.btnClose.TabIndex = 2;
            this.btnClose.Text = "Close";
            this.btnClose.UseVisualStyleBackColor = true;
            // 
            // grpLcdProperties
            // 
            this.grpLcdProperties.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.grpLcdProperties.Controls.Add(this.lblHeight);
            this.grpLcdProperties.Controls.Add(this.nudHeight);
            this.grpLcdProperties.Controls.Add(this.lblWidth);
            this.grpLcdProperties.Controls.Add(this.nudWidth);
            this.grpLcdProperties.Controls.Add(this.lblDataBusWidth);
            this.grpLcdProperties.Controls.Add(this.dudDataBusWidth);
            this.grpLcdProperties.Controls.Add(this.lblBacklight);
            this.grpLcdProperties.Controls.Add(this.nudBacklight);
            this.grpLcdProperties.Controls.Add(this.lblContrast);
            this.grpLcdProperties.Controls.Add(this.nudContrast);
            this.grpLcdProperties.Location = new System.Drawing.Point(12, 12);
            this.grpLcdProperties.Name = "grpLcdProperties";
            this.grpLcdProperties.Size = new System.Drawing.Size(198, 173);
            this.grpLcdProperties.TabIndex = 0;
            this.grpLcdProperties.TabStop = false;
            this.grpLcdProperties.Text = "LCD properties";
            // 
            // lblHeight
            // 
            this.lblHeight.AutoSize = true;
            this.lblHeight.Location = new System.Drawing.Point(6, 113);
            this.lblHeight.Name = "lblHeight";
            this.lblHeight.Size = new System.Drawing.Size(53, 17);
            this.lblHeight.TabIndex = 8;
            this.lblHeight.Text = "Height:";
            // 
            // nudHeight
            // 
            this.nudHeight.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.nudHeight.Location = new System.Drawing.Point(132, 111);
            this.nudHeight.Maximum = new decimal(new int[] {
            4,
            0,
            0,
            0});
            this.nudHeight.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nudHeight.Name = "nudHeight";
            this.nudHeight.Size = new System.Drawing.Size(60, 22);
            this.nudHeight.TabIndex = 9;
            this.nudHeight.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.nudHeight.Value = new decimal(new int[] {
            4,
            0,
            0,
            0});
            this.nudHeight.ValueChanged += new System.EventHandler(this.nudWidthHeight_ValueChanged);
            // 
            // lblWidth
            // 
            this.lblWidth.AutoSize = true;
            this.lblWidth.Location = new System.Drawing.Point(6, 85);
            this.lblWidth.Name = "lblWidth";
            this.lblWidth.Size = new System.Drawing.Size(48, 17);
            this.lblWidth.TabIndex = 6;
            this.lblWidth.Text = "Width:";
            // 
            // nudWidth
            // 
            this.nudWidth.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.nudWidth.Location = new System.Drawing.Point(132, 83);
            this.nudWidth.Margin = new System.Windows.Forms.Padding(3, 9, 3, 3);
            this.nudWidth.Maximum = new decimal(new int[] {
            80,
            0,
            0,
            0});
            this.nudWidth.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nudWidth.Name = "nudWidth";
            this.nudWidth.Size = new System.Drawing.Size(60, 22);
            this.nudWidth.TabIndex = 7;
            this.nudWidth.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.nudWidth.Value = new decimal(new int[] {
            20,
            0,
            0,
            0});
            this.nudWidth.ValueChanged += new System.EventHandler(this.nudWidthHeight_ValueChanged);
            // 
            // lblDataBusWidth
            // 
            this.lblDataBusWidth.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.lblDataBusWidth.AutoSize = true;
            this.lblDataBusWidth.Location = new System.Drawing.Point(6, 147);
            this.lblDataBusWidth.Name = "lblDataBusWidth";
            this.lblDataBusWidth.Size = new System.Drawing.Size(105, 17);
            this.lblDataBusWidth.TabIndex = 4;
            this.lblDataBusWidth.Text = "Data bus width:";
            // 
            // dudDataBusWidth
            // 
            this.dudDataBusWidth.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.dudDataBusWidth.Items.Add("8");
            this.dudDataBusWidth.Items.Add("4");
            this.dudDataBusWidth.Location = new System.Drawing.Point(132, 145);
            this.dudDataBusWidth.Margin = new System.Windows.Forms.Padding(3, 9, 3, 3);
            this.dudDataBusWidth.Name = "dudDataBusWidth";
            this.dudDataBusWidth.Size = new System.Drawing.Size(60, 22);
            this.dudDataBusWidth.TabIndex = 5;
            this.dudDataBusWidth.Text = "8";
            this.dudDataBusWidth.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.dudDataBusWidth.SelectedItemChanged += new System.EventHandler(this.dudDataBusWidth_SelectedItemChanged);
            // 
            // lblBacklight
            // 
            this.lblBacklight.AutoSize = true;
            this.lblBacklight.Location = new System.Drawing.Point(6, 51);
            this.lblBacklight.Name = "lblBacklight";
            this.lblBacklight.Size = new System.Drawing.Size(69, 17);
            this.lblBacklight.TabIndex = 2;
            this.lblBacklight.Text = "Backlight:";
            // 
            // nudBacklight
            // 
            this.nudBacklight.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.nudBacklight.Location = new System.Drawing.Point(132, 49);
            this.nudBacklight.Name = "nudBacklight";
            this.nudBacklight.Size = new System.Drawing.Size(60, 22);
            this.nudBacklight.TabIndex = 3;
            this.nudBacklight.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.nudBacklight.ValueChanged += new System.EventHandler(this.nudBacklight_ValueChanged);
            // 
            // lblContrast
            // 
            this.lblContrast.AutoSize = true;
            this.lblContrast.Location = new System.Drawing.Point(6, 23);
            this.lblContrast.Name = "lblContrast";
            this.lblContrast.Size = new System.Drawing.Size(65, 17);
            this.lblContrast.TabIndex = 0;
            this.lblContrast.Text = "Contrast:";
            // 
            // nudContrast
            // 
            this.nudContrast.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.nudContrast.Location = new System.Drawing.Point(132, 21);
            this.nudContrast.Name = "nudContrast";
            this.nudContrast.Size = new System.Drawing.Size(60, 22);
            this.nudContrast.TabIndex = 1;
            this.nudContrast.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.nudContrast.Value = new decimal(new int[] {
            50,
            0,
            0,
            0});
            this.nudContrast.ValueChanged += new System.EventHandler(this.nudContrast_ValueChanged);
            // 
            // erp
            // 
            this.erp.ContainerControl = this;
            // 
            // AdjustForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.btnClose;
            this.ClientSize = new System.Drawing.Size(222, 238);
            this.Controls.Add(this.grpLcdProperties);
            this.Controls.Add(this.btnClose);
            this.Controls.Add(this.btnSave);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "AdjustForm";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Adjust";
            this.Load += new System.EventHandler(this.Form_Load);
            this.grpLcdProperties.ResumeLayout(false);
            this.grpLcdProperties.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudHeight)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudWidth)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudBacklight)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudContrast)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.erp)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btnSave;
        private System.Windows.Forms.Button btnClose;
        private System.Windows.Forms.GroupBox grpLcdProperties;
        private System.Windows.Forms.Label lblDataBusWidth;
        private System.Windows.Forms.DomainUpDown dudDataBusWidth;
        private System.Windows.Forms.Label lblBacklight;
        private System.Windows.Forms.NumericUpDown nudBacklight;
        private System.Windows.Forms.Label lblContrast;
        private System.Windows.Forms.NumericUpDown nudContrast;
        private System.Windows.Forms.Label lblHeight;
        private System.Windows.Forms.NumericUpDown nudHeight;
        private System.Windows.Forms.Label lblWidth;
        private System.Windows.Forms.NumericUpDown nudWidth;
        private System.Windows.Forms.ErrorProvider erp;
    }
}