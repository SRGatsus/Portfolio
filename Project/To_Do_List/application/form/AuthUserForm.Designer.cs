namespace To_Do_List
{
    partial class AuthUserForm
    {
        /// <summary>
        /// Обязательная переменная конструктора.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Освободить все используемые ресурсы.
        /// </summary>
        /// <param name="disposing">истинно, если управляемый ресурс должен быть удален; иначе ложно.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Код, автоматически созданный конструктором форм Windows

        /// <summary>
        /// Требуемый метод для поддержки конструктора — не изменяйте 
        /// содержимое этого метода с помощью редактора кода.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.AuthUser = new System.Windows.Forms.Panel();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.InputSecretKey = new System.Windows.Forms.MaskedTextBox();
            this.buttonAuth = new System.Windows.Forms.Button();
            this.InputPassword = new System.Windows.Forms.MaskedTextBox();
            this.InputEmail = new System.Windows.Forms.MaskedTextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.colorDialog1 = new System.Windows.Forms.ColorDialog();
            this.ErrorAuthForm = new System.Windows.Forms.ErrorProvider(this.components);
            this.ErrorLabel = new System.Windows.Forms.Label();
            this.AuthUser.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.ErrorAuthForm)).BeginInit();
            this.SuspendLayout();
            // 
            // AuthUser
            // 
            this.AuthUser.Controls.Add(this.ErrorLabel);
            this.AuthUser.Controls.Add(this.label4);
            this.AuthUser.Controls.Add(this.label3);
            this.AuthUser.Controls.Add(this.label2);
            this.AuthUser.Controls.Add(this.InputSecretKey);
            this.AuthUser.Controls.Add(this.buttonAuth);
            this.AuthUser.Controls.Add(this.InputPassword);
            this.AuthUser.Controls.Add(this.InputEmail);
            this.AuthUser.Controls.Add(this.label1);
            this.AuthUser.Dock = System.Windows.Forms.DockStyle.Fill;
            this.AuthUser.Location = new System.Drawing.Point(0, 0);
            this.AuthUser.Margin = new System.Windows.Forms.Padding(0);
            this.AuthUser.Name = "AuthUser";
            this.AuthUser.Size = new System.Drawing.Size(380, 287);
            this.AuthUser.TabIndex = 0;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(72, 162);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(91, 13);
            this.label4.TabIndex = 8;
            this.label4.Text = "Секретный ключ";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(72, 108);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(45, 13);
            this.label3.TabIndex = 7;
            this.label3.Text = "Пароль";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(72, 54);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(32, 13);
            this.label2.TabIndex = 6;
            this.label2.Text = "Email";
            // 
            // InputSecretKey
            // 
            this.InputSecretKey.Font = new System.Drawing.Font("Times New Roman", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.InputSecretKey.Location = new System.Drawing.Point(75, 178);
            this.InputSecretKey.Name = "InputSecretKey";
            this.InputSecretKey.PasswordChar = '*';
            this.InputSecretKey.Size = new System.Drawing.Size(250, 29);
            this.InputSecretKey.TabIndex = 5;
            this.InputSecretKey.Text = "12341234";
            this.InputSecretKey.UseSystemPasswordChar = true;
            this.InputSecretKey.Leave += new System.EventHandler(this.InputSecretKey_Leave);
            // 
            // buttonAuth
            // 
            this.buttonAuth.AutoSize = true;
            this.buttonAuth.Font = new System.Drawing.Font("Times New Roman", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.buttonAuth.Location = new System.Drawing.Point(113, 244);
            this.buttonAuth.Name = "buttonAuth";
            this.buttonAuth.Size = new System.Drawing.Size(150, 31);
            this.buttonAuth.TabIndex = 3;
            this.buttonAuth.Text = "Вход";
            this.buttonAuth.UseVisualStyleBackColor = true;
            this.buttonAuth.Click += new System.EventHandler(this.buttonAuth_Click);
            // 
            // InputPassword
            // 
            this.InputPassword.Font = new System.Drawing.Font("Times New Roman", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.InputPassword.Location = new System.Drawing.Point(75, 124);
            this.InputPassword.Name = "InputPassword";
            this.InputPassword.PasswordChar = '*';
            this.InputPassword.Size = new System.Drawing.Size(250, 29);
            this.InputPassword.TabIndex = 2;
            this.InputPassword.Text = "12341234";
            this.InputPassword.UseSystemPasswordChar = true;
            this.InputPassword.Leave += new System.EventHandler(this.InputPassword_Leave);
            // 
            // InputEmail
            // 
            this.InputEmail.Font = new System.Drawing.Font("Times New Roman", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.InputEmail.Location = new System.Drawing.Point(75, 70);
            this.InputEmail.Name = "InputEmail";
            this.InputEmail.Size = new System.Drawing.Size(250, 29);
            this.InputEmail.TabIndex = 1;
            this.InputEmail.Text = "test@test.ru";
            this.InputEmail.Leave += new System.EventHandler(this.InputEmail_Leave);
            // 
            // label1
            // 
            this.label1.Font = new System.Drawing.Font("Times New Roman", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.label1.Location = new System.Drawing.Point(125, 20);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(150, 30);
            this.label1.TabIndex = 1;
            this.label1.Text = "Вход в систему";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // ErrorAuthForm
            // 
            this.ErrorAuthForm.ContainerControl = this;
            // 
            // ErrorLabel
            // 
            this.ErrorLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.ErrorLabel.ForeColor = System.Drawing.Color.Red;
            this.ErrorLabel.Location = new System.Drawing.Point(75, 210);
            this.ErrorLabel.Name = "ErrorLabel";
            this.ErrorLabel.Size = new System.Drawing.Size(250, 21);
            this.ErrorLabel.TabIndex = 9;
            // 
            // AuthUserForm
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Inherit;
            this.BackColor = System.Drawing.Color.White;
            this.ClientSize = new System.Drawing.Size(380, 287);
            this.Controls.Add(this.AuthUser);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.Name = "AuthUserForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Авторизация";
            this.AuthUser.ResumeLayout(false);
            this.AuthUser.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.ErrorAuthForm)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel AuthUser;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.MaskedTextBox InputEmail;
        private System.Windows.Forms.MaskedTextBox InputPassword;
        private System.Windows.Forms.Button buttonAuth;
        private System.Windows.Forms.ColorDialog colorDialog1;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.MaskedTextBox InputSecretKey;
        private System.Windows.Forms.ErrorProvider ErrorAuthForm;
        private System.Windows.Forms.Label ErrorLabel;
    }
}

