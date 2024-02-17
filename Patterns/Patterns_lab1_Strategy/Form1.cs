using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Patterns_lab1_Strategy
{
    public partial class Form1 : Form
    {
        private Checker checker;
        public Form1()
        {
            InitializeComponent();
            button1.Click += check;
            radioButton1.CheckedChanged += choose1;
            radioButton2.CheckedChanged += choose2;
            radioButton3.CheckedChanged += choose3;
        }

        private void check(object? sender, EventArgs e)
        {
            var text = textBox1.Text;
            int val;
            if (Int32.TryParse(text, out val))
            {
                var res = checker?.Check(val);
                label1.Text = res;
            }
            else
                label1.Text = "Хм...";
        }

        private void choose1(object sender, EventArgs e)
        {
            checker = new(new OneDigitAlgo());
        }

        private void choose2(object sender, EventArgs e)
        {
            checker = new(new TwoDigitAlgo());
        }

        private void choose3(object sender, EventArgs e)
        {
            checker = new(new ThreeDigitAlgo());
        }
    }
}
