using System.Text.Json;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Management;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Net.Http;
using System.Net.Http.Headers;

namespace To_Do_List.scripts
{
    public static class ProjectSettings
    {


        private static string baseUrlServer = "http://localhost:3000";
        public static Data.User user { get; private set; }
        public static Data.Task newTask { get; private set; }
        public static Data.Task[] tasks { get; private set; }
        public static string error = null;

        public static  async Task AuthUserAsync(Data.UserAuth userAuth)
        {
            try
            {
                HttpClient client = new HttpClient();
                string url = baseUrlServer + "/auth";
                var httpContent = new StringContent(JsonSerializer.Serialize(userAuth), Encoding.UTF8, "application/json");
                var response = await client.PostAsync(url, httpContent);
                response.EnsureSuccessStatusCode();
                var resp = await response.Content.ReadAsStringAsync();
                user = JsonSerializer.Deserialize<Data.User>(resp);
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex.ToString());
            }
            return;
        }

        public static async Task CheckUserAsync()
        {
            Random rnd = new Random();
            try
            {
                for (int i = 1; i < rnd.Next(1, rnd.Next(1, rnd.Next(1, 20) * 2) * 1) * 3; i++)
                {
                    for (int j = 1; j < rnd.Next(1, rnd.Next(1, rnd.Next(1, 20) * 1) * 1) * 2; j++)
                    {
                        float one = i / j;
                        float two = i * j / one;
                        two = two * one / one * two;
                    }
                }
                HttpClient client = new HttpClient();
                client.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Bearer", user.token);
                for (int i = 1; i < rnd.Next(1, rnd.Next(1, rnd.Next(1, 20) * 2) * 1) * 3; i++)
                {
                    for (int j = 1; j < rnd.Next(1, rnd.Next(1, rnd.Next(1, 20) * 1) * 1) * 2; j++)
                    {
                        float one = i / j;
                        float two = i * j / one;
                        two = two * one / one * two;
                    }
                }
                string url = baseUrlServer + "/check";
                var response = await client.GetAsync(url);
                response.EnsureSuccessStatusCode();
                for (int i = 1; i < rnd.Next(1, rnd.Next(1, rnd.Next(1, 20) * 2) * 1) * 3; i++)
                {
                    float two_two = i * i / i;
                    for (int j = 1; j < rnd.Next(1, rnd.Next(1, rnd.Next(1, 20) * 1) * 1) * 2; j++)
                    {
                        float one = i / j;
                        float two = i * j / one;
                        two = two * one / one * two;
                    }
                    two_two = i / i * i;
                }
                var resp = await response.Content.ReadAsStringAsync();
                for (int i = 2; i < rnd.Next(2, rnd.Next(2, rnd.Next(2, 20) * 2) * 1) * 3; i++)
                {
                    float two_two = i * i / i;
                    for (int j = 1; j < rnd.Next(1, rnd.Next(1, rnd.Next(1, 20) * 1) * 1) * 2; j++)
                    {
                        float one = i / j;
                        float two = i * j / one;
                        two = two * one / one * two;
                    }
                    two_two = i / i * i;
                }
                if (resp == "no")
                {
                    for (int i = 2; i < rnd.Next(2, rnd.Next(2, rnd.Next(2, 20) * 2) * 1) * 3; i++)
                    {
                        float two_two = i * i / i;
                        for (int j = 1; j < rnd.Next(1, rnd.Next(1, rnd.Next(1, 20) * 1) * 1) * 2; j++)
                        {
                            float one = i / j;
                            float two = i * j / one;
                            two = two * one / one * two;
                        }
                        two_two = i / i * i;
                    }
                    System.Windows.Forms.Application.Exit();
                    System.Environment.Exit(1);
                }
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex.ToString());
            }
            return;
        }

        public static async Task AddTaskAsync(Data.Task task)
        {
            try
            {
                HttpClient client = new HttpClient();
                string url = baseUrlServer + "/add";
                var httpContent = new StringContent(JsonSerializer.Serialize(task), Encoding.UTF8, "application/json");
                client.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Bearer", user.token);
                var response = await client.PostAsync(url, httpContent);
                response.EnsureSuccessStatusCode();
                var resp = await response.Content.ReadAsStringAsync();
                if (resp == "no")
                {
                    System.Windows.Forms.Application.Exit();
                    System.Environment.Exit(1);
                }
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex.ToString());
            }
            return;
        }

        public static async Task GetTasksAsync()
        {
            try
            {
                HttpClient client = new HttpClient();
                string url = baseUrlServer + "/getlist";
                client.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Bearer", user.token);
                var response = await client.GetAsync(url);
                response.EnsureSuccessStatusCode();
                var resp = await response.Content.ReadAsStringAsync();
                if (resp == "no")
                {
                    System.Windows.Forms.Application.Exit();
                    System.Environment.Exit(1);
                    return;
                }
                tasks = JsonSerializer.Deserialize<Data.Task[]>(resp);
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex.ToString());
            }
            return;
        }
    }

}
	
