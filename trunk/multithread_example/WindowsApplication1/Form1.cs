using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Threading;

namespace WindowsApplication1
{
    public partial class Form1 : Form
    {
        delegate void SetLogCallback(string text);

        Thread theThread=null;

        public Form1()
        {
            InitializeComponent();
            //Control.CheckForIllegalCrossThreadCalls = false;
        }

     


        private void WorkerThreadMethod()
        {
            int i=0;
            while (true)
            {
                // lock on the current instance of the class for thread #1
                lock (this)
                {
                    
                    LogLine("counter: {0}" + i);
                    //_threadOutput = "Hello Thread1";
                    Thread.Sleep(1000);  // simulate a lot of processing
                    // tell the user what thread we are in thread #1
                   
                } // lock released  for thread #1 here
            }
        }
         // This method demonstrates a pattern for making thread-safe
        // calls on a Windows Forms control. 
        //
        // If the calling thread is different from the thread that
        // created the TextBox control, this method creates a
        // SetTextCallback and calls itself asynchronously using the
        // Invoke method.
        //
        // If the calling thread is the same as the thread that created
        // the TextBox control, the Text property is set directly. 

        private void LogLine(string text)
        {
            // InvokeRequired required compares the thread ID of the
            // calling thread to the thread ID of the creating thread.
            // If these threads are different, it returns true.
            if (this.textBox1.InvokeRequired)
            {    
                SetLogCallback d = new SetLogCallback(LogLine);
                this.Invoke(d, new object[] { text });
            }
            else
            {
                this.textBox1.AppendText(text + Environment.NewLine);
            }
        }


        private void Form1_Shown(object sender, EventArgs e)
        {
            
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (theThread != null) 
            { 
                theThread.Abort(); 
            }
        }

        private void startToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (theThread == null)
            {
                theThread = new Thread(new ThreadStart(WorkerThreadMethod));
            }
            theThread.Start();

        }

        private void stopToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (theThread != null)
            {
                theThread.Abort();
            }
        }




    }
}