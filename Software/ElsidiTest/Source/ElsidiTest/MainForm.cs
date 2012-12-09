using Medo.Device;
using System;
using System.Diagnostics;
using System.Drawing;
using System.IO.Ports;
using System.Windows.Forms;

namespace TestElsidi {
    public partial class MainForm : Form {
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

        private void btnSend_Click(object sender, System.EventArgs e) {
            Settings.LastText = txtText.Text;
            var text = txtText.Text.Replace("\r\n", "\n");
            this.Device.AddClearDisplay();
            this.Device.AddLiteralText(text.Replace("\n", "\t"));
            var sw = new Stopwatch();
            sw.Start();
            this.Device.Execute();
            sw.Stop();
            Debug.WriteLine(sw.ElapsedMilliseconds);
        }


        private void SetupView() {
            if (this.Device != null) { //connected
                btnConnect.Text = "Disconnect";
                cmbSerialPort.Enabled = false;
                txtText.Enabled = true;
                btnAdjust.Enabled = true;
                btnSend.Enabled = true;
                this.AcceptButton = null;
            } else {
                btnConnect.Text = "Connect";
                cmbSerialPort.Enabled = true;
                txtText.Enabled = false;
                btnAdjust.Enabled = false;
                btnSend.Enabled = false;
                this.AcceptButton = btnConnect;
            }
        }

    }
}
