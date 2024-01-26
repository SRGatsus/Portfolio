using System;
using System.Collections.Generic;
using System.Linq;
using System.Management;
using System.Threading.Tasks;
using WEB_TEST_Portfolio.Data.Interfaces;
using WEB_TEST_Portfolio.Data.Models;

namespace WEB_TEST_Portfolio.Data.Mocks
{
    public class MockSearchUser : ISearchUser
    {
        public LocalUser user;

        public bool SearchUser(string userName)
        {
            SelectQuery query = new SelectQuery("Win32_UserAccount");
            ManagementObjectSearcher searcher = new ManagementObjectSearcher(query);
            bool resultSearch = false;
            foreach (ManagementObject envVar in searcher.Get())
            {
                string Fullname = envVar["FullName"].ToString().ToLower();
                string name = envVar["Name"].ToString().ToLower();
                if (name == userName.ToLower() || Fullname == userName.ToLower())
                {
                    resultSearch = true;
                    user = new LocalUser(envVar);
                    break;
                }
            }
            return resultSearch;
        }
    }
}
