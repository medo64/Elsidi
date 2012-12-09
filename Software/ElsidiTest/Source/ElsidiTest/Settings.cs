using System;

namespace TestElsidi {
    internal static class Settings {

        public static string LastSerialPort {
            get { return Medo.Configuration.Settings.Read("LastSerialPort", "COM1"); }
            set { Medo.Configuration.Settings.Write("LastSerialPort", value); }
        }

        public static string LastText {
            get { return Medo.Configuration.Settings.Read("LastText", "Elsidi\r\nTest"); }
            set { Medo.Configuration.Settings.Write("LastText", value); }
        }

    }
}
