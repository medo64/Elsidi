using Medo.Device;
using System.Drawing;
using System.Windows.Forms;

namespace TestElsidi {
    public partial class AdjustForm : Form {
        public AdjustForm(Elsidi device) {
            InitializeComponent();
            this.Font = SystemFonts.MessageBoxFont;

            this.Device = device;
        }

        private readonly Elsidi Device;

        private void btnSave_Click(object sender, System.EventArgs e) {
            this.Device.SetContrast((int)nudContrast.Value, true);
            this.Device.SetBacklight((int)nudBacklight.Value, true);
        }
    }
}
