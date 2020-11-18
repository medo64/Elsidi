using Medo.Device;
using System;
using System.Drawing;
using System.Windows.Forms;

namespace TestElsidi {
    internal partial class AdjustForm : Form {

        public AdjustForm(Elsidi device) {
            InitializeComponent();
            this.Font = SystemFonts.MessageBoxFont;

            foreach (Control control in this.grpLcdProperties.Controls) {
                erp.SetIconAlignment(control, ErrorIconAlignment.MiddleLeft);
                erp.SetIconPadding(control, SystemInformation.BorderSize.Width);
            }

            this.Device = device;
        }

        private readonly Elsidi Device;


        private void Form_Load(object sender, System.EventArgs e) {
            var contrast = this.Device.GetContrast();
            if (contrast > -1) { nudContrast.Value = contrast; }

            var backlight = this.Device.GetBacklight();
            if (backlight > -1) { nudBacklight.Value = backlight; }

            var size = this.Device.GetDisplaySize();
            if (size != Size.Empty) {
                nudWidth.Value = size.Width;
                nudHeight.Value = size.Height;
            }

            var busWidth = this.Device.GetDisplayBusWidth();
            switch (busWidth) {
                case 8: dudDataBusWidth.SelectedIndex = 0; break;
                case 4: dudDataBusWidth.SelectedIndex = 1; break;
            }
        }


        private void nudContrast_ValueChanged(object sender, System.EventArgs e) {
            this.Device.SetContrast((int)nudContrast.Value);
        }

        private void nudBacklight_ValueChanged(object sender, System.EventArgs e) {
            this.Device.SetBacklight((int)nudBacklight.Value);
        }

        private void nudWidthHeight_ValueChanged(object sender, System.EventArgs e) {
            erp.SetError(nudWidth, null);
            erp.SetError(nudHeight, null);
            btnSave.Enabled = true;
            try {
                this.Device.SetDisplaySize(new Size((int)nudWidth.Value, (int)nudHeight.Value));
            } catch (ArgumentException ex) {
                erp.SetError(nudWidth, ex.Message);
                erp.SetError(nudHeight, ex.Message);
                btnSave.Enabled = false;
            }
        }

        private void dudDataBusWidth_SelectedItemChanged(object sender, System.EventArgs e) {
            switch (dudDataBusWidth.SelectedIndex) {
                case 0: this.Device.SetDisplayBusWidth(8); break;
                case 1: this.Device.SetDisplayBusWidth(4); break;
            }
        }


        private void btnSave_Click(object sender, System.EventArgs e) {
            this.Device.SetContrast((int)nudContrast.Value, true);
            this.Device.SetBacklight((int)nudBacklight.Value, true);
            this.Device.SetDisplaySize(new Size((int)nudWidth.Value, (int)nudHeight.Value), true);
            switch (dudDataBusWidth.SelectedIndex) {
                case 0: this.Device.SetDisplayBusWidth(8, true); break;
                case 1: this.Device.SetDisplayBusWidth(4, true); break;
            }
        }

    }
}
