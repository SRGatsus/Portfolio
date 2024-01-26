namespace To_Do_List.form
{
    partial class ToDoList
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.panel1 = new System.Windows.Forms.Panel();
            this.flowLayoutPanel1 = new System.Windows.Forms.FlowLayoutPanel();
            this.buttonCreateTask = new System.Windows.Forms.Button();
            this.InputTextTask = new System.Windows.Forms.MaskedTextBox();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.buttonResetTasks = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.ListCompletionTask = new System.Windows.Forms.ListBox();
            this.ListNewTask = new System.Windows.Forms.ListBox();
            this.ListCompletedTask = new System.Windows.Forms.ListBox();
            this.buttonCompletionTask = new System.Windows.Forms.Button();
            this.buttonCompletedTask = new System.Windows.Forms.Button();
            this.ErrorToDoList = new System.Windows.Forms.ErrorProvider(this.components);
            this.panel1.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.ErrorToDoList)).BeginInit();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.flowLayoutPanel1);
            this.panel1.Controls.Add(this.buttonCreateTask);
            this.panel1.Controls.Add(this.InputTextTask);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Margin = new System.Windows.Forms.Padding(0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(900, 50);
            this.panel1.TabIndex = 0;
            // 
            // flowLayoutPanel1
            // 
            this.flowLayoutPanel1.Location = new System.Drawing.Point(0, 50);
            this.flowLayoutPanel1.Name = "flowLayoutPanel1";
            this.flowLayoutPanel1.Size = new System.Drawing.Size(300, 400);
            this.flowLayoutPanel1.TabIndex = 4;
            // 
            // buttonCreateTask
            // 
            this.buttonCreateTask.AutoSize = true;
            this.buttonCreateTask.Font = new System.Drawing.Font("Times New Roman", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.buttonCreateTask.Location = new System.Drawing.Point(549, 10);
            this.buttonCreateTask.MaximumSize = new System.Drawing.Size(150, 29);
            this.buttonCreateTask.MinimumSize = new System.Drawing.Size(150, 29);
            this.buttonCreateTask.Name = "buttonCreateTask";
            this.buttonCreateTask.Size = new System.Drawing.Size(150, 29);
            this.buttonCreateTask.TabIndex = 4;
            this.buttonCreateTask.Text = "Создать задачу";
            this.buttonCreateTask.UseVisualStyleBackColor = true;
            this.buttonCreateTask.Click += new System.EventHandler(this.ButtonCreateTask_ClickAsync);
            // 
            // InputTextTask
            // 
            this.InputTextTask.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.InputTextTask.Font = new System.Drawing.Font("Times New Roman", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.InputTextTask.Location = new System.Drawing.Point(269, 12);
            this.InputTextTask.MaximumSize = new System.Drawing.Size(250, 29);
            this.InputTextTask.MinimumSize = new System.Drawing.Size(150, 29);
            this.InputTextTask.Name = "InputTextTask";
            this.InputTextTask.Size = new System.Drawing.Size(250, 29);
            this.InputTextTask.TabIndex = 2;
            this.InputTextTask.TextChanged += new System.EventHandler(this.InputTextTask_TextChanged);
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 3;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33.33334F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33.33334F));
            this.tableLayoutPanel1.Controls.Add(this.buttonResetTasks, 2, 2);
            this.tableLayoutPanel1.Controls.Add(this.label1, 1, 0);
            this.tableLayoutPanel1.Controls.Add(this.label2, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.label3, 2, 0);
            this.tableLayoutPanel1.Controls.Add(this.ListCompletionTask, 1, 1);
            this.tableLayoutPanel1.Controls.Add(this.ListNewTask, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.ListCompletedTask, 2, 1);
            this.tableLayoutPanel1.Controls.Add(this.buttonCompletionTask, 0, 2);
            this.tableLayoutPanel1.Controls.Add(this.buttonCompletedTask, 1, 2);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.GrowStyle = System.Windows.Forms.TableLayoutPanelGrowStyle.FixedSize;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 50);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 3;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 10F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 70F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 20F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(900, 400);
            this.tableLayoutPanel1.TabIndex = 1;
            // 
            // buttonResetTasks
            // 
            this.buttonResetTasks.AutoSize = true;
            this.buttonResetTasks.Dock = System.Windows.Forms.DockStyle.Fill;
            this.buttonResetTasks.Font = new System.Drawing.Font("Times New Roman", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.buttonResetTasks.Location = new System.Drawing.Point(664, 350);
            this.buttonResetTasks.Margin = new System.Windows.Forms.Padding(65, 30, 20, 20);
            this.buttonResetTasks.MaximumSize = new System.Drawing.Size(150, 29);
            this.buttonResetTasks.MinimumSize = new System.Drawing.Size(150, 29);
            this.buttonResetTasks.Name = "buttonResetTasks";
            this.buttonResetTasks.Size = new System.Drawing.Size(150, 29);
            this.buttonResetTasks.TabIndex = 13;
            this.buttonResetTasks.Text = "Сбросить";
            this.buttonResetTasks.UseVisualStyleBackColor = true;
            this.buttonResetTasks.Click += new System.EventHandler(this.ButtonResetTasks_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label1.Font = new System.Drawing.Font("Times New Roman", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.label1.Location = new System.Drawing.Point(302, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(294, 40);
            this.label1.TabIndex = 5;
            this.label1.Text = "Задачи на выполнение";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label2.Font = new System.Drawing.Font("Times New Roman", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.label2.Location = new System.Drawing.Point(3, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(293, 40);
            this.label2.TabIndex = 6;
            this.label2.Text = "Задачи";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label3.Font = new System.Drawing.Font("Times New Roman", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.label3.Location = new System.Drawing.Point(602, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(295, 40);
            this.label3.TabIndex = 7;
            this.label3.Text = "Завершенные задачи";
            this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // ListCompletionTask
            // 
            this.ListCompletionTask.AllowDrop = true;
            this.ListCompletionTask.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ListCompletionTask.FormattingEnabled = true;
            this.ListCompletionTask.Items.AddRange(new object[] {
            "3232",
            "23",
            "232",
            "23",
            "23",
            "23",
            "2",
            "32",
            "3",
            "23"});
            this.ListCompletionTask.Location = new System.Drawing.Point(302, 43);
            this.ListCompletionTask.Name = "ListCompletionTask";
            this.ListCompletionTask.SelectionMode = System.Windows.Forms.SelectionMode.MultiSimple;
            this.ListCompletionTask.Size = new System.Drawing.Size(294, 274);
            this.ListCompletionTask.TabIndex = 8;
            // 
            // ListNewTask
            // 
            this.ListNewTask.AllowDrop = true;
            this.ListNewTask.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ListNewTask.FormattingEnabled = true;
            this.ListNewTask.Items.AddRange(new object[] {
            "3232",
            "23",
            "232",
            "23",
            "23",
            "23",
            "2",
            "32",
            "3",
            "23"});
            this.ListNewTask.Location = new System.Drawing.Point(3, 43);
            this.ListNewTask.Name = "ListNewTask";
            this.ListNewTask.SelectionMode = System.Windows.Forms.SelectionMode.MultiSimple;
            this.ListNewTask.Size = new System.Drawing.Size(293, 274);
            this.ListNewTask.TabIndex = 9;
            // 
            // ListCompletedTask
            // 
            this.ListCompletedTask.AllowDrop = true;
            this.ListCompletedTask.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ListCompletedTask.FormattingEnabled = true;
            this.ListCompletedTask.Items.AddRange(new object[] {
            "3232",
            "23",
            "232",
            "23",
            "23",
            "23",
            "2",
            "32",
            "3",
            "23"});
            this.ListCompletedTask.Location = new System.Drawing.Point(602, 43);
            this.ListCompletedTask.Name = "ListCompletedTask";
            this.ListCompletedTask.SelectionMode = System.Windows.Forms.SelectionMode.MultiSimple;
            this.ListCompletedTask.Size = new System.Drawing.Size(295, 274);
            this.ListCompletedTask.TabIndex = 10;
            // 
            // buttonCompletionTask
            // 
            this.buttonCompletionTask.AutoSize = true;
            this.buttonCompletionTask.Dock = System.Windows.Forms.DockStyle.Fill;
            this.buttonCompletionTask.Font = new System.Drawing.Font("Times New Roman", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.buttonCompletionTask.Location = new System.Drawing.Point(65, 350);
            this.buttonCompletionTask.Margin = new System.Windows.Forms.Padding(65, 30, 20, 20);
            this.buttonCompletionTask.MaximumSize = new System.Drawing.Size(150, 29);
            this.buttonCompletionTask.MinimumSize = new System.Drawing.Size(150, 29);
            this.buttonCompletionTask.Name = "buttonCompletionTask";
            this.buttonCompletionTask.Size = new System.Drawing.Size(150, 29);
            this.buttonCompletionTask.TabIndex = 11;
            this.buttonCompletionTask.Text = "Выполнять";
            this.buttonCompletionTask.UseVisualStyleBackColor = true;
            this.buttonCompletionTask.Click += new System.EventHandler(this.ButtonCompletionTask_Click);
            // 
            // buttonCompletedTask
            // 
            this.buttonCompletedTask.AutoSize = true;
            this.buttonCompletedTask.Dock = System.Windows.Forms.DockStyle.Fill;
            this.buttonCompletedTask.Font = new System.Drawing.Font("Times New Roman", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.buttonCompletedTask.Location = new System.Drawing.Point(364, 350);
            this.buttonCompletedTask.Margin = new System.Windows.Forms.Padding(65, 30, 20, 20);
            this.buttonCompletedTask.MaximumSize = new System.Drawing.Size(150, 29);
            this.buttonCompletedTask.MinimumSize = new System.Drawing.Size(150, 29);
            this.buttonCompletedTask.Name = "buttonCompletedTask";
            this.buttonCompletedTask.Size = new System.Drawing.Size(150, 29);
            this.buttonCompletedTask.TabIndex = 12;
            this.buttonCompletedTask.Text = "Завершить";
            this.buttonCompletedTask.UseVisualStyleBackColor = true;
            this.buttonCompletedTask.Click += new System.EventHandler(this.ButtonCompletedTask_Click);
            // 
            // ErrorToDoList
            // 
            this.ErrorToDoList.ContainerControl = this;
            // 
            // ToDoList
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(900, 450);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Controls.Add(this.panel1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Name = "ToDoList";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "ToDoList";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.ToDoList_FormClosed);
            this.Load += new System.EventHandler(this.ToDoList_Load);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.ErrorToDoList)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.MaskedTextBox InputTextTask;
        private System.Windows.Forms.Button buttonCreateTask;
        private System.Windows.Forms.FlowLayoutPanel flowLayoutPanel1;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ListBox ListCompletionTask;
        private System.Windows.Forms.ListBox ListNewTask;
        private System.Windows.Forms.ListBox ListCompletedTask;
        private System.Windows.Forms.Button buttonResetTasks;
        private System.Windows.Forms.Button buttonCompletionTask;
        private System.Windows.Forms.Button buttonCompletedTask;
        private System.Windows.Forms.ErrorProvider ErrorToDoList;
    }
}