namespace ESP8266_Display_4_Segmentos
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            components = new System.ComponentModel.Container();
            timer1 = new System.Windows.Forms.Timer(components);
            btnClock = new Button();
            lblContent = new Label();
            btnCrono = new Button();
            btnChronoStart = new Button();
            btnChronoStop = new Button();
            btnCountDow = new Button();
            btnCountdownPlay = new Button();
            btnCountdownStop = new Button();
            txtStartCountDown = new TextBox();
            btnText = new Button();
            txtDisplaytext = new TextBox();
            btnSetText = new Button();
            btnStartMarque = new Button();
            txtMarquee = new TextBox();
            btnMarque = new Button();
            SuspendLayout();
            // 
            // timer1
            // 
            timer1.Enabled = true;
            timer1.Interval = 1000;
            timer1.Tick += timer1_Tick;
            // 
            // btnClock
            // 
            btnClock.Location = new Point(245, 20);
            btnClock.Name = "btnClock";
            btnClock.Size = new Size(83, 23);
            btnClock.TabIndex = 0;
            btnClock.Text = "Relógio ";
            btnClock.UseVisualStyleBackColor = true;
            btnClock.Click += btnClock_Click;
            // 
            // lblContent
            // 
            lblContent.BackColor = Color.OliveDrab;
            lblContent.Font = new Font("Segoe UI", 36F, FontStyle.Regular, GraphicsUnit.Point, 0);
            lblContent.ForeColor = Color.White;
            lblContent.Location = new Point(12, 9);
            lblContent.Name = "lblContent";
            lblContent.Size = new Size(189, 74);
            lblContent.TabIndex = 1;
            lblContent.Text = "...";
            lblContent.TextAlign = ContentAlignment.MiddleCenter;
            // 
            // btnCrono
            // 
            btnCrono.Location = new Point(245, 60);
            btnCrono.Name = "btnCrono";
            btnCrono.Size = new Size(83, 23);
            btnCrono.TabIndex = 2;
            btnCrono.Text = "Cronometro";
            btnCrono.UseVisualStyleBackColor = true;
            btnCrono.Click += btnCrono_Click;
            // 
            // btnChronoStart
            // 
            btnChronoStart.Location = new Point(337, 61);
            btnChronoStart.Name = "btnChronoStart";
            btnChronoStart.Size = new Size(36, 23);
            btnChronoStart.TabIndex = 3;
            btnChronoStart.Text = ">";
            btnChronoStart.UseVisualStyleBackColor = true;
            btnChronoStart.Click += btnChronoStart_Click;
            // 
            // btnChronoStop
            // 
            btnChronoStop.Location = new Point(379, 61);
            btnChronoStop.Name = "btnChronoStop";
            btnChronoStop.Size = new Size(36, 23);
            btnChronoStop.TabIndex = 4;
            btnChronoStop.Text = "[   ]";
            btnChronoStop.UseVisualStyleBackColor = true;
            btnChronoStop.Click += btnChronoStop_Click;
            // 
            // btnCountDow
            // 
            btnCountDow.Location = new Point(245, 98);
            btnCountDow.Name = "btnCountDow";
            btnCountDow.Size = new Size(83, 23);
            btnCountDow.TabIndex = 5;
            btnCountDow.Text = "Countdown";
            btnCountDow.UseVisualStyleBackColor = true;
            btnCountDow.Click += btnCountDow_Click;
            // 
            // btnCountdownPlay
            // 
            btnCountdownPlay.Location = new Point(337, 98);
            btnCountdownPlay.Name = "btnCountdownPlay";
            btnCountdownPlay.Size = new Size(36, 23);
            btnCountdownPlay.TabIndex = 6;
            btnCountdownPlay.Text = ">";
            btnCountdownPlay.UseVisualStyleBackColor = true;
            btnCountdownPlay.Click += btnCountdownPlay_Click;
            // 
            // btnCountdownStop
            // 
            btnCountdownStop.Location = new Point(379, 98);
            btnCountdownStop.Name = "btnCountdownStop";
            btnCountdownStop.Size = new Size(36, 23);
            btnCountdownStop.TabIndex = 7;
            btnCountdownStop.Text = "[   ]";
            btnCountdownStop.UseVisualStyleBackColor = true;
            btnCountdownStop.Click += btnCountdownStop_Click;
            // 
            // txtStartCountDown
            // 
            txtStartCountDown.Location = new Point(421, 98);
            txtStartCountDown.Name = "txtStartCountDown";
            txtStartCountDown.Size = new Size(42, 23);
            txtStartCountDown.TabIndex = 8;
            // 
            // btnText
            // 
            btnText.Location = new Point(245, 136);
            btnText.Name = "btnText";
            btnText.Size = new Size(83, 23);
            btnText.TabIndex = 9;
            btnText.Text = "Text";
            btnText.UseVisualStyleBackColor = true;
            btnText.Click += btnText_Click;
            // 
            // txtDisplaytext
            // 
            txtDisplaytext.Location = new Point(337, 136);
            txtDisplaytext.MaxLength = 4;
            txtDisplaytext.Name = "txtDisplaytext";
            txtDisplaytext.Size = new Size(84, 23);
            txtDisplaytext.TabIndex = 10;
            // 
            // btnSetText
            // 
            btnSetText.Location = new Point(425, 135);
            btnSetText.Name = "btnSetText";
            btnSetText.Size = new Size(36, 23);
            btnSetText.TabIndex = 11;
            btnSetText.Text = ">";
            btnSetText.UseVisualStyleBackColor = true;
            btnSetText.Click += btnSetText_Click;
            // 
            // btnStartMarque
            // 
            btnStartMarque.Location = new Point(425, 164);
            btnStartMarque.Name = "btnStartMarque";
            btnStartMarque.Size = new Size(36, 23);
            btnStartMarque.TabIndex = 14;
            btnStartMarque.Text = ">";
            btnStartMarque.UseVisualStyleBackColor = true;
            btnStartMarque.Click += btnStartMarque_Click;
            // 
            // txtMarquee
            // 
            txtMarquee.Location = new Point(337, 165);
            txtMarquee.MaxLength = 50;
            txtMarquee.Name = "txtMarquee";
            txtMarquee.Size = new Size(84, 23);
            txtMarquee.TabIndex = 13;
            // 
            // btnMarque
            // 
            btnMarque.Location = new Point(245, 165);
            btnMarque.Name = "btnMarque";
            btnMarque.Size = new Size(83, 23);
            btnMarque.TabIndex = 12;
            btnMarque.Text = "Marquee";
            btnMarque.UseVisualStyleBackColor = true;
            btnMarque.Click += btnMarque_Click;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(473, 215);
            Controls.Add(btnStartMarque);
            Controls.Add(txtMarquee);
            Controls.Add(btnMarque);
            Controls.Add(btnSetText);
            Controls.Add(txtDisplaytext);
            Controls.Add(btnText);
            Controls.Add(txtStartCountDown);
            Controls.Add(btnCountdownStop);
            Controls.Add(btnCountdownPlay);
            Controls.Add(btnCountDow);
            Controls.Add(btnChronoStop);
            Controls.Add(btnChronoStart);
            Controls.Add(btnCrono);
            Controls.Add(lblContent);
            Controls.Add(btnClock);
            Name = "Form1";
            Text = "Form1";
            Load += Form1_Load;
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private System.Windows.Forms.Timer timer1;
        private Button btnClock;
        private Label lblContent;
        private Button btnCrono;
        private Button btnChronoStart;
        private Button btnChronoStop;
        private Button btnCountDow;
        private Button btnCountdownPlay;
        private Button btnCountdownStop;
        private TextBox txtStartCountDown;
        private Button btnText;
        private TextBox txtDisplaytext;
        private Button btnSetText;
        private Button btnStartMarque;
        private TextBox txtMarquee;
        private Button btnMarque;
    }
}
