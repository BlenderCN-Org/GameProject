using System;
using System.Collections.Generic;
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
using System.Windows.Shapes;

namespace Editor.EditorWindows.AddItemDlgPages.StandardControls
{
    /// <summary>
    /// Interaction logic for SelectCommand.xaml
    /// </summary>
    public partial class SelectCommand : Window
    {
        public SelectCommand()
        {
            InitializeComponent();
            UpdateCommandBox();
        }

        private void UpdateCommandBox()
        {
            if (Command != null)
            {
                Command.Items.Clear();

                DataSources.CommandType type = CommandType.SelectedItem as DataSources.CommandType;

                int b = 0;
            }

        }

        private void ComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            UpdateCommandBox();
        }
        
        private void Cancel_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = false;
        }

        private void Ok_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = true;
        }
    }
}
