using Medo.Device;
using System;
using System.Diagnostics;
using System.Drawing;
using System.IO.Ports;
using System.Text;
using System.Windows.Forms;

namespace TestElsidi {
    internal partial class MainForm : Form {
        public MainForm() {
            InitializeComponent();
            this.Font = SystemFonts.MessageBoxFont;

            foreach (var portName in SerialPort.GetPortNames()) {
                cmbSerialPort.Items.Add(portName);
            }
            cmbSerialPort.SelectedItem = Settings.LastSerialPort;

            txtText.Text = Settings.LastText;
        }

        Elsidi Device = null;


        private void cmbSerialPort_SelectedIndexChanged(object sender, System.EventArgs e) {
            btnConnect.Enabled = true;
        }

        private void btnConnect_Click(object sender, System.EventArgs e) {
            if (this.Device == null) { //connect
                Settings.LastSerialPort = cmbSerialPort.Text;
                try {
                    this.Device = new Elsidi(cmbSerialPort.Text);
                    this.Device.Open();
                } catch (Exception ex) {
                    this.Device = null;
                    Medo.MessageBox.ShowError(this, ex.Message);
                }
            } else {
                this.Device.Close();
                this.Device = null;
            }

            SetupView();
            if (this.Device != null) {
                txtText.SelectAll();
                txtText.Select();
            } else {
                cmbSerialPort.Select();
            }
        }

        private void txtText_KeyDown(object sender, KeyEventArgs e) {
            if (e.KeyData == (Keys.Control | Keys.A)) {
                txtText.SelectAll();
            }
        }

        private void btnSend_Click(object sender, System.EventArgs e) {
            Settings.LastText = txtText.Text;
            var text = txtText.Text.Replace("\r\n", "\n");
            var sw = new Stopwatch();
            sw.Start();
            this.Device.ClearDisplay();
            foreach (var line in text.Split('\n')) {
                this.Device.SendText(line);
                this.Device.NextLine();
            }
            sw.Stop();
            Debug.WriteLine(sw.ElapsedMilliseconds);
        }

        private void btnClock_Click(object sender, EventArgs e) {
            using (var frm = new ClockForm(this.Device)) {
                frm.ShowDialog(this);
            }
        }

        private void btnStress_Click(object sender, EventArgs e) {
            var sb = new StringBuilder();
            var text = sb.ToString();
            var sw = new Stopwatch();
            sw.Start();
            this.Device.ClearDisplay();
            for (int i = 0; i < 128; i++) {
                char ch = (char)('0' + i % 10);
                this.Device.ReturnHome();
                this.Device.ChangeDdramAddress(i);
                this.Device.SendText(ch.ToString());
            }
            sw.Stop();
            Debug.WriteLine(sw.ElapsedMilliseconds);
        }


        private void btnInit_Click(object sender, EventArgs e) {
            this.Device.SendTextCommand('~', "");
        }

        private void btnAdjust_Click(object sender, EventArgs e) {
            using (var frm = new AdjustForm(this.Device)) {
                frm.ShowDialog(this);
            }
        }


        private void SetupView() {
            if (this.Device != null) { //connected
                btnConnect.Text = "Disconnect";
                cmbSerialPort.Enabled = false;
                txtText.Enabled = true;
                btnSend.Enabled = true;
                btnClock.Enabled = true;
                btnStress.Enabled = true;
                btnInit.Enabled = true;
                btnAdjust.Enabled = true;
                this.AcceptButton = null;
            } else {
                btnConnect.Text = "Connect";
                cmbSerialPort.Enabled = true;
                txtText.Enabled = false;
                btnSend.Enabled = false;
                btnClock.Enabled = false;
                btnStress.Enabled = false;
                btnInit.Enabled = false;
                btnAdjust.Enabled = false;
                this.AcceptButton = btnConnect;
            }
        }

    }
}
