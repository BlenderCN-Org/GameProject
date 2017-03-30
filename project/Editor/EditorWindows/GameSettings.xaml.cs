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

namespace Editor.EditorWindows
{
	/// <summary>
	/// Interaction logic for GameSettings.xaml
	/// </summary>
	public partial class GameSettings : Window
	{
		public GameSettings()
		{
			InitializeComponent();
		}

		private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
		{
			e.Cancel = true;
			Hide();
		}

        private void StartupScriptSelect_Click(object sender, RoutedEventArgs e)
        {
            SelectWindow_dropDown wnd = new SelectWindow_dropDown();
            wnd.Owner = this;
            wnd.Title = "Select Starup Script";
            wnd.SelectedValue.Items.Add("Script");
            wnd.ShowDialog();

            if(wnd.DialogResult.HasValue && wnd.DialogResult.Value)
            {
                StartupScriptBox.Text = wnd.SelectedValue.SelectedValue.ToString();
            }

        }

        private void MainMenuSelect_Click(object sender, RoutedEventArgs e)
        {
            SelectWindow_dropDown wnd = new SelectWindow_dropDown();
            wnd.Owner = this;
            wnd.Title = "Select Main Menu Scene";
            wnd.SelectedValue.Items.Add("Scene");
            wnd.ShowDialog();

            if (wnd.DialogResult.HasValue && wnd.DialogResult.Value)
            {
                MainMenuSceneBox.Text = wnd.SelectedValue.SelectedValue.ToString();
            }
        }
    }
}
