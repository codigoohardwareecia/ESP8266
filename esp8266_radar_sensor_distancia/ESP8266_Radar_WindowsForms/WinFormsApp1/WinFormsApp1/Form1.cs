using System.Text.Json;

namespace WinFormsApp1
{
    public partial class Form1 : Form
    {
        private float angle = 0;
        private HttpClient client = new HttpClient();
        private Bitmap radarBmp;
        private System.Windows.Forms.Timer timer;

        public Form1()
        {
            InitializeComponent();
            this.Text = "Radar Pro - Jarvis";

            radarBmp = new Bitmap(this.Width, this.Height);
            timer = new System.Windows.Forms.Timer { Interval = 10 };
            timer.Tick += Timer_Tick;
            timer.Start();
        }

        private async void Timer_Tick(object sender, EventArgs e)
        {
            try
            {
                string json = await client.GetStringAsync("http://192.168.15.92/data");
                var data = JsonSerializer.Deserialize<RadarData>(json);
                DrawRadar(data.distancia);
            }
            catch {  }
        }
8
        private void DrawRadar(float dist)
        {
            using (Graphics g = Graphics.FromImage(radarBmp))
            {
                g.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias; // Esfumaçado
                g.FillRectangle(new SolidBrush(Color.FromArgb(20, 0, 0, 0)), 0, 0, 500, 500); // Rastro
                g.DrawEllipse(new Pen(Color.FromArgb(50, Color.LimeGreen)), 50, 50, 400, 400);   // Desenhar grade

                float rad = angle * (float)Math.PI / 180;
                float x = 250 + (dist * 1.2f) * (float)Math.Cos(rad);
                float y = 250 + (dist * 1.2f) * (float)Math.Sin(rad);

                g.FillEllipse(Brushes.LimeGreen, x - 3, y - 3, 6, 6);
                g.DrawLine(new Pen(Color.LimeGreen, 2), 250, 250, 250 + 200 * (float)Math.Cos(rad), 250 + 200 * (float)Math.Sin(rad));  // Linha de varredura
            }

            angle = (angle + 5) % 360;

            pictureBox1.Image = radarBmp;
        }

        public class RadarData
        {
            public float distancia { get; set; }
            public string status { get; set; }
        }
    }
}