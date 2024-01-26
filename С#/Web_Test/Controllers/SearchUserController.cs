using Microsoft.AspNetCore.Mvc;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using WEB_TEST_Portfolio.Data.Interfaces;

namespace WEB_TEST_Portfolio.Controllers
{
    public class SearchUserController:Controller
    {
        private readonly ISearchUser _user;

        public SearchUserController (ISearchUser user)
        {
            _user = user;
        }

        public ViewResult Result(string name)
        {
            ViewBag.Title = "Результат поиска";
            ViewBag.data = _user;
            ViewBag.name = name;
            return View();
        }
    }
}
