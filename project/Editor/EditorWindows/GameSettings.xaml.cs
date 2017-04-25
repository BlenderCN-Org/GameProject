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

            EventHandler.QueryDataArgs args = new EventHandler.QueryDataArgs();
            args.ObjectType = Editor.EventHandler.ObjectTypes.SCRIPT;
            args.ReturnList = new List<DataSources.BaseData>();
            EventHandler.EventManager.OnQueryDataEvent(args);

            foreach (var item in args.ReturnList)
            {
                wnd.SelectedValue.Items.Add(item);
            }

            wnd.ShowDialog();

            if (wnd.DialogResult.HasValue && wnd.DialogResult.Value && wnd.SelectedValue.SelectedValue != null)
            {
                StartupScriptBox.Text = wnd.SelectedValue.SelectedValue.ToString();
            }

        }

        private void MainMenuSelect_Click(object sender, RoutedEventArgs e)
        {
            SelectWindow_dropDown wnd = new SelectWindow_dropDown();
            wnd.Owner = this;
            wnd.Title = "Select Main Menu Scene";

            EventHandler.QueryDataArgs args = new EventHandler.QueryDataArgs();
            args.ObjectType = Editor.EventHandler.ObjectTypes.SCENE;
            args.ReturnList = new List<DataSources.BaseData>();
            EventHandler.EventManager.OnQueryDataEvent(args);

            foreach (var item in args.ReturnList)
            {
                wnd.SelectedValue.Items.Add(item);
            }

            wnd.ShowDialog();

            if (wnd.DialogResult.HasValue && wnd.DialogResult.Value && wnd.SelectedValue.SelectedValue != null)
            {
                MainMenuSceneBox.Text = wnd.SelectedValue.SelectedValue.ToString();
            }
        }

        private void Grid_IsVisibleChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            if (SceneTab.IsVisible)
            {
                sceneList.Items.Clear();

                EventHandler.QueryDataArgs args = new Editor.EventHandler.QueryDataArgs();

                args.ObjectType = Editor.EventHandler.ObjectTypes.SCENE;
                args.ReturnList = new List<DataSources.BaseData>();
                EventHandler.EventManager.OnQueryDataEvent(args);

                foreach (var item in args.ReturnList)
                {
                    sceneList.Items.Add(item);
                }
            }

            if (MenuTab.IsVisible)
            {
                menuList.Items.Clear();

                EventHandler.QueryDataArgs args = new Editor.EventHandler.QueryDataArgs();
                args.ObjectType = Editor.EventHandler.ObjectTypes.DIALOG;
                args.ReturnList = new List<DataSources.BaseData>();
                EventHandler.EventManager.OnQueryDataEvent(args);

                foreach (var item in args.ReturnList)
                {
                    if( item.GetType().Equals(typeof(DataSources.MenuItem)) )
                        menuList.Items.Add(item);
                }

            }
        }

        private void deleteSceneButton_Click(object sender, RoutedEventArgs e)
        {
            ConfirmDialog cfDlg = new ConfirmDialog();
            cfDlg.Owner = this;
            cfDlg.ShowDialog();

            if(cfDlg.DialogResult.HasValue && cfDlg.DialogResult.Value)
            {
                Console.WriteLine("Delete selected item");
            }
            else
            {
                Console.WriteLine("Abort delete");
            }

        }

        private void addSceneButton_Click(object sender, RoutedEventArgs e)
        {
            AddItemDialog addDlg = new AddItemDialog();

            addDlg.Owner = this;

            AddItemDlgPages.AddScene newScenePage = new AddItemDlgPages.AddScene();
            
            addDlg.PageArea.Children.Add(newScenePage);

            addDlg.PageArea.Height = newScenePage.Height;
            addDlg.PageArea.Width = newScenePage.Width;

            addDlg.InvalidateMeasure();
            addDlg.InvalidateArrange();

            addDlg.ShowDialog();
            
        }

        private void addMenuButton_Click(object sender, RoutedEventArgs e)
        {

        }
        
        private void deleteMenuButton_Click(object sender, RoutedEventArgs e)
        {

        }
    }
}
