using Medo.Device;
using System;
using System.Drawing;
using System.Windows.Forms;

namespace TestElsidi {
    internal partial class ClockForm : Form {
        public ClockForm(Elsidi device) {
            InitializeComponent();
            this.Font = SystemFonts.MessageBoxFont;

            this.Device = device;
        }

        private readonly Elsidi Device;


        private void Form_Load(object sender, EventArgs e) {
            this.Device.ClearDisplay();
            tmrUpdate_Tick(null, null);
        }


        private void tmrUpdate_Tick(object sender, EventArgs e) {
            var time = DateTime.Now;
            var line1 = time.ToLongTimeString();
            var line2 = time.ToShortDateString();

            this.Device.ReturnHome();
            this.Device.SendText(line1);
            this.Device.NextLine();
            this.Device.SendText(line2);

            lblClock.Text = line1 + "\r\t" + line2;
        }

    }
}
