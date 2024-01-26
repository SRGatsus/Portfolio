using System;
using System.Collections.Generic;
using System.Linq;
using System.Management;
using System.Threading.Tasks;

namespace WEB_TEST_Portfolio.Data.Models
{
    public class LocalUser
    {
        ManagementObject user;

        public LocalUser(ManagementObject user)
        {
            this.user = user;
        }
        public ManagementObject GetUser()
        {
            return user;
        }
        public string GetName()
        {
            return user["Name"].ToString();
        }
        public string GetSID()
        {
            return user["SID"].ToString();
        }
        public string GetFullName()
        {
            return user["FullName"].ToString();
        }
        public string GetCaption()
        {
            return user["Caption"].ToString();
        }
        public string GetDomain()
        {
            return user["Domain"].ToString();
        }

        public string GetDescription()
        {
            return user["Description"].ToString();
        } 
        
        public string GetStatus()
        {
            return user["Status"].ToString();
        }
    }
}
