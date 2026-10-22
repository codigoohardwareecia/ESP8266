namespace ESP8266_DIsplay_LCD_Desktop
{
    public partial class Form1 : Form
    {
        private static readonly HttpClient httpClient = new HttpClient();

        public Form1()
        {
            InitializeComponent();
        }

        private async Task SendDataToDevice(string l1, string l2)
        {
            var formData = new Dictionary<string, string>
            {
                { "linha1", l1 },
                { "linha2", l2 }
            };

            var content = new FormUrlEncodedContent(formData);

            HttpResponseMessage response = await httpClient.PostAsync("http://painel-lcd.local/print", content);
        }

        private async void timer1_Tick(object sender, EventArgs e)
        {
            await SendDataToDevice(DateTime.Now.Date.ToShortDateString(), DateTime.Now.ToLongTimeString());
        }
    }
}
