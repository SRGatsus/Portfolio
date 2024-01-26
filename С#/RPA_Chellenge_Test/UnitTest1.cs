using NUnit.Framework;
using OpenQA.Selenium;
using System.IO;

namespace RPA_Chellenge_Test
{
    public class Tests
    {
        private IWebDriver driver;
        [SetUp]
        public void Setup()
        {
            var path = Directory.GetCurrentDirectory();
            driver = new OpenQA.Selenium.Chrome.ChromeDriver(@"D:\portfolio\Ñ#\RPA_Chellenge_Test\resources\chromedriver_win32\");
            driver.Navigate().GoToUrl("https://rpachallenge.com/");
            driver.Manage().Window.Maximize();
        }

        [Test]
        public void Test()
        {
            
        }

        [TearDown]
        public void TearDown()
        {

        }
    }
}