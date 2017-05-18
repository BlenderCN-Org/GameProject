using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

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
            IdControl.ObjectLabel.Content = "Menu:";
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

            if(cmdDialog.DialogResult == true)
            {
                DataSources.CommandType c = cmdDialog.CommandType.SelectedItem as DataSources.CommandType;
                CommandData.Type = c.Type;
                CommandData.Description = c.Description;
            }

        }
    }
}
