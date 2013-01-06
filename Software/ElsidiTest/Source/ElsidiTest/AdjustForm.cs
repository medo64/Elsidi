using Medo.Device;
using System.Drawing;
using System.Windows.Forms;

namespace TestElsidi {
    internal partial class AdjustForm : Form {
        public AdjustForm(Elsidi device) {
            InitializeComponent();
            this.Font = SystemFonts.MessageBoxFont;

            this.Device = device;
        }

        private readonly Elsidi Device;


        private void Form_Load(object sender, System.EventArgs e) {
            var contrast = this.Device.GetContrast();
            if (contrast > -1) { nudContrast.Value = contrast; }

            var backlight = this.Device.GetBacklight();
            if (backlight > -1) { nudBacklight.Value = backlight; }
        }


        private void nudContrast_ValueChanged(object sender, System.EventArgs e) {
            this.Device.SetContrast((int)nudContrast.Value);
        }

        private void nudBacklight_ValueChanged(object sender, System.EventArgs e) {
            this.Device.SetBacklight((int)nudBacklight.Value);
        }


        private void btnSave_Click(object sender, System.EventArgs e) {
            this.Device.SetContrast((int)nudContrast.Value, true);
            this.Device.SetBacklight((int)nudBacklight.Value, true);
        }

    }
}
