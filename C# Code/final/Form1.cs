using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports; 
namespace final
{
    public partial class Form1 : Form
    {
        SerialPort mySerial = null; 
        public Form1()
        {
            InitializeComponent();
            getCom();
        }
       void getCom()
        {
            String[] ports = SerialPort.GetPortNames();
            comboBox1.Items.AddRange(ports);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (mySerial != null)
                if (mySerial.IsOpen)
                {
                    while (mySerial.BytesToWrite > 0) ;
                    mySerial.Dispose();
                }

            String t = comboBox1.Text.ToString();
            mySerial = new SerialPort(t, 9600, Parity.None, 8, StopBits.One);
            button1.Enabled = false;
            if (!(mySerial.IsOpen))
            {
                mySerial.Open();
            }
            mySerial.DataReceived += new SerialDataReceivedEventHandler(DataHandler);
        }

        private void DataHandler(object sender, SerialDataReceivedEventArgs e)
        {
            mySerial = (SerialPort)sender;
            String data  = mySerial.ReadLine();

            if ("changeState" == data)
            {
                if (label3.Text == "ON")
                    ChangeLabelText(label3, "OFF");
                else
                    ChangeLabelText(label3, "ON");
            }
            else if ("gotoStart" == data)
            {
                ChangeLabelText(label3, "OFF");
                ChangeLabelText(label4, "System isn't Accessed");
            }
            else if ("Thief" == data)
            {
                ChangeLabelText(label4, "     Thief !!!!!     ");
                Invoke(new Action(() => button2.Enabled = true));
            }
            else if ("1234" == data)
            {
                mySerial.Write("Y");
                ChangeLabelText(label4, "System is Accessed");
                ChangeLabelText(label3, "ON");
            }
            else
            {
                mySerial.Write("N");
                ChangeLabelText(label3, "on");
            }
        }
        private void ChangeLabelText(object scr, String txt)
        {
            Label l = (Label)scr;
            Invoke(new Action(() =>l.Text = txt)); 
        }
        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            button1.Enabled = true;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            mySerial.Write("R");
            ChangeLabelText(label4, "System isn't Accessed");
            button2.Enabled = false;
        }
    }
}
