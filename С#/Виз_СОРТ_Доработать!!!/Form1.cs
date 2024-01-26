using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Practika
{
    public partial class Form1 : Form
    {
        int count = 10;
        Label[] masLabel;
        int SelectedLabel=-1;
        public Form1()
        {
            InitializeComponent();

        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (masLabel!=null)
            {
                for (int i = 0; i < masLabel.Length; i++)
                {
                    Controls.Remove(masLabel[i]);
                }
            }
            masLabel = new Label[count];
            for (int i = 0; i < count; i++)
            {
                masLabel[i] = new Label()
                {
                    Size = new Size(75, 75),
                    Location = new Point(15 + (85 * i), 50),
                    TextAlign = ContentAlignment.MiddleCenter,
                    BackColor = Color.Gray,
                    Font=new Font("Segoe UI",15),
                    TabIndex = i
                };
                masLabel[i].Click += label1_Click;
                Controls.Add(masLabel[i]);
            }
        }

        private void label1_Click(object sender, EventArgs e)
        {
            if (SelectedLabel!=-1)masLabel[SelectedLabel].BackColor = Color.Gray;
            (sender as Label).BackColor = Color.DarkGray;
            SelectedLabel = (sender as Label).TabIndex;
            InputLabel.Text = "";
        }
        private void Sort(int[] mas)
        {
            for (int i = 0; i < mas.Length - 1; i++)
            {
                //поиск минимального числа
                int min = i;
                for (int j = i + 1; j < mas.Length; j++)
                {
                    if (mas[j] < mas[min])
                    {
                        min = j;
                    }
                }
                //обмен элементов
                int temp = mas[min];
                mas[min] = mas[i];
                mas[i] = temp;
            }
        }
        private void trackBar1_Scroll(object sender, EventArgs e)
        {

        }

        private void textBox1_KeyPress(object sender, KeyPressEventArgs e)
        {

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            if(SelectedLabel!=-1)masLabel[SelectedLabel].Text = (sender as TextBox).Text;
        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {
            if((sender as TextBox).Text!="")
            count = Convert.ToInt32((sender as TextBox).Text);
            if (count > 15)
            {
                count = 15;
                (sender as TextBox).Text = Convert.ToString(15);
            }
        }
        int Index = 0;
        int Index2 = 0;
        int min = 0;
        private void Start_Click(object sender, EventArgs e)
        {
            Index = 0;
            Index2 = 0;
            Set.Enabled = false;
            InputLabel.Enabled = false;
            timer1.Enabled = true;
        }
        private void timer1_Tick(object sender, EventArgs e)
        {
            if (Index < masLabel.Length)
            {
                min = Index;
                Point.Location = new Point(masLabel[Index].Location.X, masLabel[Index].Location.Y - Point.Size.Height);
                timer1.Enabled = false;
                Index2 = Index + 1;
                timer2.Enabled = true;
            }
            else
            {
                return;
            }

        }

        private void timer2_Tick(object sender, EventArgs e)
        {
            if (Index2 >= masLabel.Length)
            {
                SwapElemnt();
                timer2.Enabled = false;
                timer1.Enabled = true;
                return;
            }
          //  masLabel[Index2-1].BackColor = Color.Gray;
            if (Convert.ToInt32(masLabel[Index2].Text) < Convert.ToInt32(masLabel[min].Text))
            {
                masLabel[min].BackColor = Color.Gray;
                min = Index2;
                masLabel[Index2].BackColor = Color.Green;
            }
            else
            {
               // masLabel[Index2].BackColor = Color.Red;
            }
            System.Threading.Thread.Sleep(1000);
            Index2++;
        }
        private void SwapElemnt()
        {
            if (Index != min)
            {
                string temp;
                temp = masLabel[min].Text;
                masLabel[min].Text = masLabel[Index].Text;
                masLabel[Index].Text = temp;
                
            }
            masLabel[min].BackColor = Color.Gray;
            Index++;
        }
    }
}
