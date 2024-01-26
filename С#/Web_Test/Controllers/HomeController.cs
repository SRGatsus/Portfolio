using Microsoft.AspNetCore.Mvc;
using WEB_TEST_Portfolio.Data.Models;


namespace WEB_TEST_Portfolio.Controllers
{
    public class HomeController:Controller
    {
        private  Search _search;

        public HomeController()
        {
            
        }
        public IActionResult Index()
        {
            ViewBag.Title = "Поиск пользователя на ПК";
            return View();
        }
        [HttpPost]
        public IActionResult Index(Search search)
        {
            ViewBag.Title = "Поиск пользователя на ПК";
            if (ModelState.IsValid)
            {
                _search = search;
                return RedirectToAction("Result", "SearchUser", new { name = search.name }); ;
            }
            return View(search);

        }
    }
}
