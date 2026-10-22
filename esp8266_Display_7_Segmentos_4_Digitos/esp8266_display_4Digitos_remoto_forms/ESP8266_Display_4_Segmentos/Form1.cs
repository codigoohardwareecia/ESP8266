namespace ESP8266_Display_4_Segmentos
{
    public partial class Form1 : Form
    {
        DisplayService _displayService;

        public Form1()
        {
            InitializeComponent();
            _displayService = new DisplayService("http://192.168.15.150/set-display");
            _displayService.SetContext(Context.None);
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void btnClock_Click(object sender, EventArgs e)
        {
            _displayService.SetContext(Context.Clock);
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            lblContent.Text = _displayService.getContentESP();
        }

        private void btnCrono_Click(object sender, EventArgs e)
        {
            _displayService.SetContext(Context.Cronometer);
        }

        private void btnChronoStart_Click(object sender, EventArgs e)
        {
            _displayService.StartChronometer();
        }

        private void btnChronoStop_Click(object sender, EventArgs e)
        {
            _displayService.StopChronometer();
        }

        private void btnCountDow_Click(object sender, EventArgs e)
        {
            _displayService.SetContext(Context.Countdown);
        }

        private void btnCountdownPlay_Click(object sender, EventArgs e)
        {
            _displayService.SetCountdown(txtStartCountDown.Text);
            _displayService.StartCountDown();
        }

        private void btnCountdownStop_Click(object sender, EventArgs e)
        {
            _displayService.StopCountdown();
        }

        private void btnSetText_Click(object sender, EventArgs e)
        {
            _displayService.SetText(txtDisplaytext.Text);
        }

        private void btnText_Click(object sender, EventArgs e)
        {
            _displayService.SetContext(Context.Text);

        }

        private void btnMarque_Click(object sender, EventArgs e)
        {
            _displayService.SetContext(Context.Marquee);
        }

        private void btnStartMarque_Click(object sender, EventArgs e)
        {
            _displayService.SetMarquee(txtMarquee.Text);
        }
    }
}
