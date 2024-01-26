using System;
using System.Text.RegularExpressions;
using System.Windows.Forms;
using To_Do_List.scripts;
using To_Do_List.form;


namespace To_Do_List
{
    public partial class AuthUserForm : Form
    {
        private bool validEmail = true;
        private bool validPassword = true;
        private bool validSecretKey = true;
        private int MIN_COUNT_PASSWORD = 6;
        private int MIN_COUNT_SECRET_KEY = 5;
        public AuthUserForm()
        {
            InitializeComponent();
        }

        private void InputEmail_Leave(object sender, EventArgs e)
        {
            MaskedTextBox inputEmail = (sender as MaskedTextBox);
            string email = inputEmail.Text;
            Regex regex = new Regex(@"^([a-zA-Z0-9_\-\.]+)@((\[[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.)|(([a-zA-Z0-9\-]+\.)+))([a-zA-Z]{2,4}|[0-9]{1,3})(\]?)$",
            RegexOptions.CultureInvariant | RegexOptions.Singleline);
            Console.WriteLine($"The email is {email}");
            bool isValidEmail = regex.IsMatch(email);
            if (!isValidEmail)
            {
                ErrorAuthForm.SetError(inputEmail, "Email не правильное, пример example@example.com");
                validEmail = false;
                inputEmail.ForeColor = System.Drawing.Color.Red;
            }
            else
            {
                ErrorAuthForm.Clear();
                validEmail = true;
                inputEmail.ForeColor = System.Drawing.Color.Green;
            }
        }

        private void InputPassword_Leave(object sender, EventArgs e)
        {
            MaskedTextBox inputPassword = (sender as MaskedTextBox);
            string password = inputPassword.Text;
            if (!(password.Length >= MIN_COUNT_PASSWORD))
            {
                ErrorAuthForm.SetError(inputPassword, "Пароль менее " + MIN_COUNT_PASSWORD.ToString() + " символов");
                validPassword = false;
                inputPassword.ForeColor = System.Drawing.Color.Red;
            }
            else
            {
                ErrorAuthForm.Clear();
                validPassword = true;
                inputPassword.ForeColor = System.Drawing.Color.Green;
            }
        }

        private void InputSecretKey_Leave(object sender, EventArgs e)
        {
            MaskedTextBox inputSecretKey = (sender as MaskedTextBox);
            string secretKey = inputSecretKey.Text;
            if (!(secretKey.Length >= MIN_COUNT_SECRET_KEY))
            {
                ErrorAuthForm.SetError(inputSecretKey, "Секретный ключ менее " + MIN_COUNT_SECRET_KEY.ToString() + " символов");
                validSecretKey = false;
                inputSecretKey.ForeColor = System.Drawing.Color.Red;
            }
            else
            {
                ErrorAuthForm.Clear();
                validSecretKey = true;
                inputSecretKey.ForeColor = System.Drawing.Color.Green;
            }
        }

        private async void buttonAuth_Click(object sender, EventArgs e)
        {
            buttonAuth.Enabled = false;
            if (!validEmail)
            {
                ErrorAuthForm.SetError(InputEmail, "Email не правильное, пример example@example.com");
            }
            if (!validPassword)
            {
                ErrorAuthForm.SetError(InputPassword, "Пароль менее " + MIN_COUNT_PASSWORD.ToString() + " символов");
            }
            if (!validSecretKey)
            {
                ErrorAuthForm.SetError(InputSecretKey, "Секретный ключ менее " + MIN_COUNT_SECRET_KEY.ToString() + " символов");
            }
            if (validEmail && validPassword && validSecretKey)
            {
                ErrorAuthForm.Clear();
                await ProjectSettings.AuthUserAsync(new Data.UserAuth(InputEmail.Text, InputPassword.Text, InputSecretKey.Text));
                if (ProjectSettings.user?.error != null)
                {
                    ErrorLabel.Text = ProjectSettings.user.error;
                }
                else
                {
                    ErrorLabel.Text = "";
                    ToDoList toDoList = new ToDoList();
                    toDoList.Show();
                    this.Hide();
                }
               
            }
            buttonAuth.Enabled = true;
        }
    }

}
