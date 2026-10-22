using System.Diagnostics;
using static System.Net.Mime.MediaTypeNames;
using static System.Windows.Forms.AxHost;

namespace ESP8266_Display_4_Segmentos
{
    public class DisplayService
    {
        private static readonly HttpClient _client = new HttpClient();
        private string _url;
        private Context _context = Context.None;
        private readonly System.Threading.Timer _senderData;
        private readonly System.Threading.Timer _updaterContext;
        private readonly System.Threading.Timer _clockTimer;
        private readonly System.Threading.Timer _cronometroTimer;
        private readonly System.Threading.Timer _countDownTimer;
        private readonly Stopwatch _stopwatchChronometer = new Stopwatch();
        private readonly Stopwatch _stopwatchCountdown = new Stopwatch();
        private string _rawContent = string.Empty;
        private string _espContent = string.Empty;
        private Dictionary<string, string> _postData;
        private bool _blinkPoints = false;
        private bool _isConnected = false;
        private TimeSpan _elapsetTimer;
        private TimeSpan _startTime;

        public DisplayService(string url)
        {
            _isConnected = false;
            _url = url;
            _rawContent = string.Empty;
            _espContent = string.Empty;
            _postData = new Dictionary<string, string>();
            _senderData = new System.Threading.Timer(SendToESP, null, Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
            _clockTimer = new System.Threading.Timer(UpdateClockTimer, null, Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
            _cronometroTimer = new System.Threading.Timer(UpdateChronometer, null, Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
            _countDownTimer = new System.Threading.Timer(UpdateCountdown, null, Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
        }

        public void SetContext(Context context)
        {
            _context = context;
            _rawContent = string.Empty;
            _espContent = string.Empty;

            if (_context == Context.None)
            {
                _senderData?.Change(TimeSpan.FromSeconds(1), Timeout.InfiniteTimeSpan);
                _clockTimer?.Change(Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
                _cronometroTimer?.Change(Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
                _countDownTimer?.Change(Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
                return;
            }

            if (_context == Context.Clock)
            {
                _senderData?.Change(TimeSpan.Zero, TimeSpan.FromSeconds(1));
                _clockTimer?.Change(TimeSpan.Zero, TimeSpan.FromSeconds(1));
                _cronometroTimer?.Change(Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
                _countDownTimer?.Change(Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
                _blinkPoints = true;
                return;
            }

            if (_context == Context.Cronometer)
            {
                _senderData?.Change(TimeSpan.Zero, TimeSpan.FromSeconds(1));
                _clockTimer?.Change(Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
                _cronometroTimer?.Change(TimeSpan.Zero, TimeSpan.FromSeconds(1));
                _countDownTimer?.Change(Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
                _blinkPoints = true;
                return;
            }


            if (_context == Context.Countdown)
            {
                _senderData?.Change(TimeSpan.Zero, TimeSpan.FromSeconds(1));
                _clockTimer?.Change(Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
                _cronometroTimer?.Change(Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
                _countDownTimer?.Change(TimeSpan.Zero, TimeSpan.FromSeconds(1));
                _blinkPoints = true;
                return;
            }

            if(_context == Context.Text)
            {
                _senderData?.Change(Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
                _clockTimer?.Change(Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
                _cronometroTimer?.Change(Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
                _countDownTimer?.Change(Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
                _blinkPoints = false;
                return;
            }


            if (_context == Context.Marquee)
            {
                _senderData?.Change(Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
                _clockTimer?.Change(Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
                _cronometroTimer?.Change(Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
                _countDownTimer?.Change(Timeout.InfiniteTimeSpan, Timeout.InfiniteTimeSpan);
                _blinkPoints = false;
                return;
            }
        }

        public string getContentESP()
        {
            return _espContent;
        }

        public string getRawContent()
        {
            return _rawContent;
        }

        public void StartChronometer()
        {
            if (_context == Context.Cronometer)
                _stopwatchChronometer.Start();
        }

        public void StopChronometer()
        {
            if (_context == Context.Cronometer)
                _stopwatchChronometer.Stop();
        }

        public void StartCountDown()
        {
            if (_context == Context.Countdown)
                _stopwatchCountdown.Start();
        }

        public void SetCountdown(string time)
        {
            if (_context == Context.Countdown)
            {
                if (int.TryParse(time, out int valorDigitado))
                {
                    _startTime = TimeSpan.FromSeconds(valorDigitado); ;
                }
            }
        }

        public void StopCountdown()
        {
            if (_context == Context.Countdown)
                _stopwatchCountdown.Stop();
        }

        private async void UpdateClockTimer(object? state)
        {
            _espContent = DateTime.Now.ToString("HHmm");
            _rawContent = DateTime.Now.ToString("HH:mm:ss");
            _blinkPoints = (DateTime.Now.Second % 2 == 0);
        }


        private async void UpdateChronometer(object? state)
        {
            _elapsetTimer = _stopwatchChronometer.Elapsed;
            _rawContent = _elapsetTimer.ToString(@"hh\:mm\:ss");
            _espContent = _elapsetTimer.ToString(@"mmss");
        }

        private async void UpdateCountdown(object? state)
        {
            if (_stopwatchCountdown.IsRunning)
            {
                TimeSpan tempoRestante = _startTime - _stopwatchCountdown.Elapsed;

                if (tempoRestante <= TimeSpan.Zero)
                {
                    _stopwatchCountdown.Reset();
                    _stopwatchCountdown.Stop();
                }

                _rawContent = tempoRestante.ToString(@"hh\:mm\:ss");
                _espContent = tempoRestante.ToString(@"mmss");
            }

        }

        public void SetText(string text)
        {
            if (_context == Context.Text)
            {
                _espContent = text;
                SendToESP(null);
            }
  
        }

        public async void SetMarquee(string text)
        {
            if (_context == Context.Marquee)
            {
                await DisplayMarqueeAsync(text);
            }

        }

        public async Task DisplayMarqueeAsync(string frase, int velocidadeMs = 400)
        {
            if (string.IsNullOrWhiteSpace(frase))
                return;

            // Adiciona 4 espaços no começo e no fim para a frase entrar e sair "lisinha" do display
            string fraseFormatada = "    " + frase + "    ";

            // Calcula até onde o índice pode ir para sempre conseguir pegar 4 caracteres
            int limite = fraseFormatada.Length - 4;

            for (int i = 0; i <= limite; i++)
            {
                // 1. Recorta 4 caracteres a partir da posição atual e atribui à sua variável
                _espContent = fraseFormatada.Substring(i, 4);

                // 2. Chame o envio diretamente (passando null por conta da assinatura do Timer)
                SendToESP(null);

                // 3. Aguarda o tempo necessário antes de dar o próximo "passo" no texto
                await Task.Delay(velocidadeMs);
            }
        }

        private async void SendToESP(object? state)
        {
            try
            {
                _postData.Clear();
                _postData.Add("texto", _espContent);

                if(_blinkPoints)
                    _postData.Add("pontos", "1");

                HttpResponseMessage response = await _client.PostAsync(_url, new FormUrlEncodedContent(_postData));

                if (response.IsSuccessStatusCode)
                    _isConnected = true;
                else
                    _isConnected = false ;
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine("Erro: " + ex.Message);
            }
            finally
            {
                // Se estiver usando o padrão para evitar requisições sobrepostas:
                _senderData?.Change(TimeSpan.FromSeconds(1), Timeout.InfiniteTimeSpan);
            }
        }
        public void Dispose()
        {
            _senderData?.Dispose();
        }

    }

    public enum Context
    {
        None = 0,
        Clock = 1,      
        Cronometer = 2, 
        Countdown = 3,
        Text = 4,
        Marquee = 5
    }
}
