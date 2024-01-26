using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using To_Do_List.scripts;

namespace To_Do_List.form
{
    public partial class ToDoList : Form
    {
        private readonly int MIN_LENGTH_TASK_TEXT = 6;
        private readonly int MAX_LENGTH_TASK_TEXT = 20;
        private bool valideTaskText = false;
        public ToDoList()
        {

            _ = ProjectSettings.CheckUserAsync();
            InitializeComponent();
        }

        private async void ButtonCreateTask_ClickAsync(object sender, EventArgs e)
        {
            await ProjectSettings.CheckUserAsync();
            InputTextTask.Text = InputTextTask.Text.Trim(); 
            if (valideTaskText)
            {
                Data.Task task = new Data.Task(-1, -1, InputTextTask.Text, "new");
                _ = ProjectSettings.AddTaskAsync(task);
                await UpdateListAsync();
                InputTextTask.Text = "";
                InputTextTask.ForeColor = System.Drawing.Color.Black;
            }
            else
            {
                ErrorToDoList.SetError(InputTextTask, "Текст менее " + MIN_LENGTH_TASK_TEXT.ToString() + " символов или более " + MAX_LENGTH_TASK_TEXT.ToString());
                valideTaskText = false;
                InputTextTask.ForeColor = System.Drawing.Color.Red;
            }
            _ = ProjectSettings.CheckUserAsync();
        }

        private void ButtonCompletionTask_Click(object sender, EventArgs e)
        {
            _ = ProjectSettings.CheckUserAsync();
            SwitchTask(ListNewTask, ListCompletionTask);
            SwitchTask(ListCompletedTask, ListCompletionTask);
            UpdateListAsync();
            _ = ProjectSettings.CheckUserAsync();
        }

        private void ButtonCompletedTask_Click(object sender, EventArgs e)
        {
            _ = ProjectSettings.CheckUserAsync();
            SwitchTask(ListNewTask, ListCompletedTask);
            SwitchTask(ListCompletionTask, ListCompletedTask);
            UpdateListAsync();
            _ = ProjectSettings.CheckUserAsync();
        }

        private void ButtonResetTasks_Click(object sender, EventArgs e)
        {
            _ = ProjectSettings.CheckUserAsync();
            SwitchTask(ListCompletedTask, ListNewTask);
            SwitchTask(ListCompletionTask, ListNewTask);
            UpdateListAsync();
            _ = ProjectSettings.CheckUserAsync();
        }

        private void SwitchTask(ListBox fromListBox, ListBox toListBox)
        {
            if (fromListBox.SelectedItems.Count > 0)
            {
                object[] items = new object[fromListBox.SelectedItems.Count];
                fromListBox.SelectedItems.CopyTo(items, 0);
                foreach (var item in items)
                {
                    toListBox.Items.Add(item);
                    fromListBox.Items.Remove(item);
                }
                fromListBox.SelectedItems.Clear();
                toListBox.SelectedItems.Clear();
            }
        }

        private void InputTextTask_TextChanged(object sender, EventArgs e)
        {
            MaskedTextBox inputTaskText = (sender as MaskedTextBox);
            string taskText = inputTaskText.Text;
            if (!(taskText.Length >= MIN_LENGTH_TASK_TEXT && taskText.Length <= MAX_LENGTH_TASK_TEXT))
            {
                ErrorToDoList.SetError(inputTaskText, "Текст менее " + MIN_LENGTH_TASK_TEXT.ToString() + " символов или более" + MAX_LENGTH_TASK_TEXT.ToString());
                valideTaskText = false;
                inputTaskText.ForeColor = System.Drawing.Color.Red;
            }
            else
            {
                ErrorToDoList.Clear();
                valideTaskText = true;
                inputTaskText.ForeColor = System.Drawing.Color.Green;
            }
        }

        private void ToDoList_FormClosed(object sender, FormClosedEventArgs e)
        {
            System.Windows.Forms.Application.Exit();
            System.Environment.Exit(1);
        }

        private async Task UpdateListAsync()
        {
            await ProjectSettings.GetTasksAsync();
            ListNewTask.Items.Clear();
            ListCompletionTask.Items.Clear();
            ListCompletedTask.Items.Clear(); 
            foreach (Data.Task task in ProjectSettings.tasks)
            {
                switch (task.state)
                {
                    case "new":
                        ListNewTask.Items.Add(task.GetString());
                        break;
                    case "completion":
                        ListCompletionTask.Items.Add(task.GetString());
                        break;
                    case "completed":
                        ListCompletedTask.Items.Add(task.GetString());
                        break;
                    default:
                        break;
                }
            }
        }

        private async void ToDoList_Load(object sender, EventArgs e)
        {
            await UpdateListAsync();
        }
    }
}
