using System.Windows;
using System.Windows.Controls;

namespace Editor.EditorWindows.AddItemDlgPages
{
    /// <summary>
    /// Interaction logic for AddMenuPanel.xaml
    /// </summary>
    public partial class AddMenuPanel : UserControl
    {
        public AddMenuPanel()
        {
            InitializeComponent();
        }

        private void AddMenuButton_Click(object sender, RoutedEventArgs e)
        {
        }

        private void DeleteMenuButton_Click(object sender, RoutedEventArgs e)
        {
        }

        private void MoveDown_Click(object sender, RoutedEventArgs e)
        {
        }

        private void MoveUp_Click(object sender, RoutedEventArgs e)
        {
        }

        private void EnableTrigger_Click(object sender, RoutedEventArgs e)
        {
            Visible.IsChecked = false;
        }

        private void Command_Click(object sender, RoutedEventArgs e)
        {
            StandardControls.SelectCommand cmdDialog = new StandardControls.SelectCommand();
            cmdDialog.Owner = Window.GetWindow(this);
            cmdDialog.ShowDialog();

            if (cmdDialog.DialogResult == true)
            {
                DataSources.CommandType c = cmdDialog.CommandType.SelectedItem as DataSources.CommandType;
                CommandData.Type = c.Type;
                CommandData.Description = c.Description;
            }
        }
    }
}