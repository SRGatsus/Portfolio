using System;
using System.Collections.Generic;
using Microsoft.AspNetCore.Mvc.ModelBinding;
using System.Linq;
using System.Threading.Tasks;
using System.ComponentModel.DataAnnotations;

namespace WEB_TEST_Portfolio.Data.Models
{
    public class Search
    {
        [Display(Name = "Введите имя")]
        [StringLength(50, MinimumLength = 1)]
        [Required(ErrorMessage = "Длина имени не менее 1 символа")]
        public string name { get; set; }
    }
}
