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
    /// 
    
    public static class ElementHelper
    {
        public static bool IsUserVisible(this UIElement element)
        {
            if (!element.IsVisible)
                return false;
            var container = VisualTreeHelper.GetParent(element) as FrameworkElement;
            if (container == null) throw new ArgumentNullException("container");

            Rect bounds = element.TransformToAncestor(container).TransformBounds(new Rect(0.0, 0.0, element.RenderSize.Width, element.RenderSize.Height));
            Rect rect = new Rect(0.0, 0.0, container.ActualWidth, container.ActualHeight);
            return rect.IntersectsWith(bounds);
        }
    }

    public partial class GameSettings : Window
    {

        private int selectedSceneItem = -1;
        private int selectedRenderLayerItem = -1;

        public GameSettings()
        {
            InitializeComponent();

            // add loading of general info

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

        private void tabControl_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (e.Source == tabControl)
            {

                // TODO - Make each tab a custom user control so that we can do
                // tabControl.SelectedItem.executeQuery() or something similar
                if (tabControl.SelectedItem == SceneTab)
                {
                    selectedSceneItem = -1;
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

                if (tabControl.SelectedItem == MenuTab)
                {
                    menuList.Items.Clear();

                    EventHandler.QueryDataArgs args = new Editor.EventHandler.QueryDataArgs();
                    args.ObjectType = Editor.EventHandler.ObjectTypes.DIALOG;
                    args.ReturnList = new List<DataSources.BaseData>();
                    EventHandler.EventManager.OnQueryDataEvent(args);

                    foreach (var item in args.ReturnList)
                    {
                        if (item.GetType().Equals(typeof(DataSources.MenuItem)))
                            menuList.Items.Add(item);
                    }

                }

                if (tabControl.SelectedItem == RenderLayersTab)
                {
                    selectedRenderLayerItem = -1;
                    renderLayerList.Items.Clear();

                    EventHandler.QueryDataArgs args = new Editor.EventHandler.QueryDataArgs();
                    args.ObjectType = Editor.EventHandler.ObjectTypes.RENDERLAYER;
                    args.ReturnList = new List<DataSources.BaseData>();
                    EventHandler.EventManager.OnQueryDataEvent(args);

                    foreach (var item in args.ReturnList)
                    {
                        if (item.GetType().Equals(typeof(DataSources.MenuItem)))
                            renderLayerList.Items.Add(item);
                    }

                }

            }
        }

        private void deleteSceneButton_Click(object sender, RoutedEventArgs e)
        {
            ConfirmDialog cfDlg = new ConfirmDialog();
            cfDlg.Owner = this;
            cfDlg.ShowDialog();

            EventHandler.FormArgs fa = new EventHandler.FormArgs()
            {
                FormID = (sceneList.SelectedItem as DataSources.BaseData).EditorID
            };

            (sceneList.SelectedItem as DataSources.BaseData).Deleted = true;

            if (cfDlg.DialogResult.HasValue && cfDlg.DialogResult.Value)
            {
                EventHandler.EventManager.OnDeleteFormEvent(fa);
            }
            else
            {
                Console.WriteLine("Abort delete");
            }

        }

        private void addSceneButton_Click(object sender, RoutedEventArgs e)
        {
            AddEditItemDialog addDlg = new AddEditItemDialog();

            addDlg.Owner = this;

            AddItemDlgPages.AddScene newScenePage = new AddItemDlgPages.AddScene();

            addDlg.PageArea.Children.Add(newScenePage);

            addDlg.PageArea.Height = newScenePage.Height;
            addDlg.PageArea.Width = newScenePage.Width;

            addDlg.InvalidateMeasure();
            addDlg.InvalidateArrange();

            addDlg.ShowDialog();

            DataSources.Scene bd = new DataSources.Scene();

            bd.EditorID = newScenePage.GetFormId();
            bd.Name = newScenePage.GetName();

            sceneList.Items.Add(bd);
            sceneList.SelectedIndex = sceneList.Items.Count - 1;

        }

        private void sceneListSelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if(selectedSceneItem != -1)
            {
                // do Edit Event
                EventHandler.FormArgs fa = new EventHandler.FormArgs();
                fa.FormID = ((DataSources.Scene)sceneList.Items[selectedSceneItem]).EditorID;
                fa.Data = ((DataSources.Scene)sceneList.Items[selectedSceneItem]);
                fa.ObjectType = Editor.EventHandler.ObjectTypes.SCENE;
                EventHandler.EventManager.OnEditFormEvent(fa);
            }

            DeleteSceneItem.IsEnabled = false;
            if (sceneList.SelectedIndex != -1)
            {
                DeleteSceneItem.IsEnabled = true;
                selectedSceneItem = sceneList.SelectedIndex;
            }
        }

        private void addMenuButton_Click(object sender, RoutedEventArgs e)
        {

        }

        private void deleteMenuButton_Click(object sender, RoutedEventArgs e)
        {

        }

        private void addRenderLayerButton_Click(object sender, RoutedEventArgs e)
        {

            EventHandler.GetFormIDArgs formIdArgs = new Editor.EventHandler.GetFormIDArgs();
            formIdArgs.FormID = 0;
            EventHandler.EventManager.OnGetFormIDEvent(formIdArgs);

            DataSources.RenderLayer bd = new DataSources.RenderLayer(formIdArgs.FormID, "New Renderlayer");
            
            renderLayerList.Items.Add(bd);
            renderLayerList.SelectedItem = bd;

            EventHandler.AddObjectArgs args = new EventHandler.AddObjectArgs()
            {
                Name = bd.Name,
                FormID = bd.EditorID,
                ObjectType = Editor.EventHandler.ObjectTypes.RENDERLAYER
            };

            EventHandler.EventManager.OnAddObjectEvent(args);

        }

        private void deleteRenderLayerButton_Click(object sender, RoutedEventArgs e)
        {
            ConfirmDialog cfDlg = new ConfirmDialog();
            cfDlg.Owner = this;
            cfDlg.ShowDialog();

            EventHandler.FormArgs fa = new EventHandler.FormArgs()
            {
                FormID = (renderLayerList.SelectedItem as DataSources.BaseData).EditorID
            };

            (renderLayerList.SelectedItem as DataSources.BaseData).Deleted = true;

            if (cfDlg.DialogResult.HasValue && cfDlg.DialogResult.Value)
            {
                renderLayerList.Items.Remove(renderLayerList.SelectedItem);
                EventHandler.EventManager.OnDeleteFormEvent(fa);
            }
            else
            {
                Console.WriteLine("Abort delete");
            }
        }

        private void renderLayerList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (selectedRenderLayerItem != -1)
            {
                // do Edit Event
                EventHandler.FormArgs fa = new EventHandler.FormArgs();
                fa.FormID = ((DataSources.RenderLayer)renderLayerList.Items[selectedRenderLayerItem]).EditorID;
                fa.Data = ((DataSources.RenderLayer)renderLayerList.Items[selectedRenderLayerItem]);
                fa.ObjectType = Editor.EventHandler.ObjectTypes.RENDERLAYER;
                EventHandler.EventManager.OnEditFormEvent(fa);
            }

            DeleteRenderLayerItem.IsEnabled = false;
            if (renderLayerList.SelectedIndex != -1)
            {
                DeleteRenderLayerItem.IsEnabled = true;
                selectedRenderLayerItem = renderLayerList.SelectedIndex;
            }
        }
    }
}
