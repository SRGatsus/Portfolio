using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Management;
using WEB_TEST_Portfolio.Data.Models;

namespace WEB_TEST_Portfolio.Data.Interfaces
{
    public interface ISearchUser
    {
        
        public bool SearchUser(string userName);

    }
}
