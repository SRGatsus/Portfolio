using Newtonsoft.Json;
using System;
using System.IO;
using System;
using System.Collections.Generic;
using System.Net;
using Telegram.Bot;
using Telegram.Bot.Args;
using Telegram.Bot.Types.Enums;
using Telegram.Bot.Types.ReplyMarkups;
namespace GismetioBot
{
    class Program
    {

        private static string token { get; set; } = "5027668446:AAFfz5BuuXiygy0QbSbojufAhr63HwC604Y";
        private static  TelegramBotClient client;

        static string NameCity;
        static float tempOfCity;
        static string nameOfCity;

        static string answerOnWether;

        public static void Main(string[] args)
        {
            client = new TelegramBotClient(token) { Timeout = TimeSpan.FromSeconds(10)};

            var me = client.GetMeAsync().Result;
            Console.WriteLine($"Bot_Id: {me.Id} \nBot_Name: {me.FirstName} ");
            
            client.StartReceiving();
            client.OnMessage += Bot_OnMessage;
            Console.ReadKey();
            client.StopReceiving();
        }

        private static async void Bot_OnMessage(object sender, MessageEventArgs e)
        {
            var message = e.Message;
            Console.WriteLine($"Пришло сообщение с текстом: {message.Text}");
            switch (message.Text)
            {
                case "Стикер":
                    await client.SendStickerAsync(
                        chatId: message.Chat.Id,
                        sticker: "https://tlgrm.ru/_/stickers/b50/063/b5006369-8faa-44d7-9f02-1ca97d82cd49/1.webp");
                    break;
                case "Картинка":
                    await client.SendPhotoAsync(
                        chatId: message.Chat.Id,
                        photo: "https://i.pinimg.com/originals/0c/f8/4c/0cf84c4066f2680103643fd2e312f3d1.jpg");
                        
                    break;

                default:
                    NameCity = message.Text;
                    Weather(NameCity);
                    Celsius(tempOfCity);
                
                    await client.SendTextMessageAsync(message.Chat.Id, $"{answerOnWether} \n\n{nameOfCity}: {tempOfCity} °C");    
               
                    Console.WriteLine(message.Text);
                    break;
            }
        }

        public static void Weather(string cityName)
        {
            try
            {
                string url = "https://api.openweathermap.org/data/2.5/weather?q=" + cityName + "&unit=metric&appid=2351aaee5394613fc0d14424239de2bd";
                HttpWebRequest httpWebRequest = (HttpWebRequest) HttpWebRequest.Create(url);
                HttpWebResponse httpWebResponse = (HttpWebResponse)httpWebRequest?.GetResponse();
                string response;

                using (StreamReader streamReader = new StreamReader(httpWebResponse.GetResponseStream()))
                {
                    response = streamReader.ReadToEnd();
                }
                WeatherResponse weatherResponse = JsonConvert.DeserializeObject<WeatherResponse>(response);

                nameOfCity = weatherResponse.Name;
                tempOfCity = weatherResponse.Main.Temp - 273;
            }
            catch (System.Net.WebException)
            {
                Console.WriteLine("Возникло исключение");
                return;
            }
        }

        public static void Celsius(float celsius)
        {
            if (celsius <= 10)
                answerOnWether = "Сегодня холодно одевайся потеплее!";
            else
                answerOnWether = "Сегодня очень жарко, так что можешь одеть маечку и шортики)";
        }
        /*private static IReplyMarkup GetButtons()
        {
            return new ReplyKeyboardMarkup
            {
                Keyboard = new List<List<KeyboardButton>>
                {
                    new List<KeyboardButton>{ new KeyboardButton { Text = "Погода", GismetioSearch}, new KeyboardButton { Text = "Картинка"} },
                    new List<KeyboardButton>{ new KeyboardButton { Text = "123"}, new KeyboardButton { Text = "456"} }
                }
            };
        }   */   
        private static IReplyMarkup GetInlineButtons()
        { 
            var KeyboardButons = new InlineKeyboardButton[][]
            {
                new InlineKeyboardButton[]
                {
                    InlineKeyboardButton.WithCallbackData("سفارش", "return2") ,
                    InlineKeyboardButton.WithCallbackData("بازگشت", "return")
                }
            };

            var replyMarkup = new InlineKeyboardMarkup()
            {

                InlineKeyboard = KeyboardButons
            };
            return replyMarkup;
        }

        private static void GismetioSearch(object sc,CallbackQueryEventArgs e)
        {
            Console.WriteLine("поиск");
        }
    }
}

