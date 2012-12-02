using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.Diagnostics;

namespace TestElsidi {
    class App {

        static void Main() {
            using (var x = new Medo.Device.Elsidi("COM1")) {
                x.Open();
                int n = 0;
                x.AddClearDisplay();
                while (true) {
                    n += 1;
                    //x.AddSwithToSecondaryDisplay();
                    //x.AddLiteralText("\r" + n.ToString() + "\t\t" + DateTime.Now.ToLongDateString() + "\t" + DateTime.Now.ToLongTimeString());
                    //x.AddSwithToPrimaryDisplay();
                    //x.AddLiteralText("\r" + n.ToString() + "\t" + DateTime.Now.ToLongDateString().PadLeft(20) + "\t" + "Elsidi[revG]".PadLeft(20) + "\t" + DateTime.Now.ToLongTimeString().PadLeft(20));
                    x.AddLiteralText("\rA123456789B123456789C123456789D123456789E123456789F123456789D123456789F123456789G123456789");
                    x.AddDdramAddressChange(0x20);
                    x.AddLiteralText("XXX");
                    x.AddLiteralText("\r" + n.ToString() + " ");
                    //x.AddLiteralText("\rElsidi [revG]\t" + n.ToString("0").PadLeft(20) + "\t" + DateTime.Now.ToLongDateString().PadLeft(20) + "\t" + DateTime.Now.ToLongTimeString().PadLeft(20));
                    var sw = new Stopwatch();
                    sw.Start();
                    x.Execute();
                    sw.Stop();
                    Console.WriteLine(sw.ElapsedMilliseconds);
                    Thread.Sleep(1000);
                }
            }
        }
    }
}
