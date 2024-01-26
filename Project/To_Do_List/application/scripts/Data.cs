using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace To_Do_List.scripts
{
    public static class Data
    {
        public class UserAuth
        {
            public string email { get; set; }
            public string password { get; set; }
            public string secretKey { get; set; }
            public UserAuth(string email = null, string password = null, string secretKey = null)
            {
                this.email = email;
                this.password = password;
                this.secretKey = secretKey;
            }

        }
        public class User
        {
            public string token { get;private set; }
            public string error { get; set; }
            public User(string token, string error)
            {
                this.token = token;
                this.error = error;
            }

        }
        public class Task
        {
            public int user_id { get; private set; }
            public int task_id { get; private set; }
            public string text { get; private set; }
            public string state { get; private set; }
            public Task(int User_id, int Task_id, string Text , string State)
            {
                this.user_id = User_id;
                this.task_id = Task_id;
                this.text = Text;
                this.state = State;
            }
            public string GetString()
            {
                return task_id.ToString() + ") " + text;
            }

        }
    }
}
