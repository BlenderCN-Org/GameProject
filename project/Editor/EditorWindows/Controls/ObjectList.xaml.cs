using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Media3D;

namespace Editor.EditorWindows.Controls
{
    /// <summary>
    /// Interaction logic for ObjectList.xaml
    /// </summary>
    public partial class ObjectList : UserControl
    {
        public ObjectList()
        {
            InitializeComponent();

            EventHandler.EventManager.onRefreshFormsEvent += new System.EventHandler<bool>(refreshEvent);
            refreshEvent(null, true);
        }

        public void refreshEvent(object sender, bool refresh)
        {
            if (refresh)
            {
                updateList();
            }
        }

        private void TreeViewItem_PreviewMouseRightButtonDown(object sender, MouseButtonEventArgs e)
        {
            TreeViewItem treeViewItem =
                      VisualUpwardSearch<TreeViewItem>(e.OriginalSource as DependencyObject);

            if (treeViewItem != null)
            {
                treeViewItem.IsSelected = true;
                e.Handled = true;
            }
        }

        private static T VisualUpwardSearch<T>(DependencyObject source) where T : DependencyObject
        {
            DependencyObject returnVal = source;

            while (returnVal != null && !(returnVal is T))
            {
                DependencyObject tempReturnVal = null;
                if (returnVal is Visual || returnVal is Visual3D)
                {
                    tempReturnVal = VisualTreeHelper.GetParent(returnVal);
                }
                if (tempReturnVal == null)
                {
                    returnVal = LogicalTreeHelper.GetParent(returnVal);
                }
                else returnVal = tempReturnVal;
            }

            return returnVal as T;
        }

        private void NewMenuItem_Click(object sender, RoutedEventArgs e)
        {
            bool edit = false;
            AddEditItemDialog dlg = new AddEditItemDialog(edit);
            EditorWindows.AddItemDlgPages.AddStaticObject add = new EditorWindows.AddItemDlgPages.AddStaticObject();
            dlg.PageArea.Children.Add(add);
            dlg.Owner = Window.GetWindow(this);
            dlg.Show();
        }

        private void EditMenuItem_Click(object sender, RoutedEventArgs e)
        {
            bool edit = true;
            AddEditItemDialog dlg = new AddEditItemDialog(edit);
            EditorWindows.AddItemDlgPages.AddStaticObject add = new EditorWindows.AddItemDlgPages.AddStaticObject();
            DataSources.BaseData obj = listView.SelectedItem as DataSources.BaseData;
            if (obj != null)
            {
                add.DataContext = obj.Clone();
            }
            dlg.PageArea.Children.Add(add);
            dlg.Owner = Window.GetWindow(this);
            dlg.Show();
        }

        private void DeleteMenuItem_Click(object sender, RoutedEventArgs e)
        {
            DataSources.BaseData data = listView.Items[listView.SelectedIndex] as DataSources.BaseData;
            if (data != null)
            {
                EventHandler.FormArgs args = new EventHandler.FormArgs();
                args.FormID = data.EditorID;

                EventHandler.EventManager.OnDeleteFormEvent(args);
            }
        }

        private void listSelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            DeleteItem.IsEnabled = false;
            if (listView.SelectedIndex != -1)
            {
                DeleteItem.IsEnabled = true;
            }
        }

        private void treeView_SelectedItemChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            updateList();
        }

        private void updateList()
        {
            listView.SelectedIndex = -1;
            listView.Items.Clear();

            EventHandler.QueryDataArgs args = new EventHandler.QueryDataArgs();
            args.ReturnList = new System.Collections.Generic.List<DataSources.BaseData>();

            if (treeView.SelectedItem == @static)
            {
                System.Console.Write("Static\n");
                args.ObjectType = EventHandler.ObjectTypes.STATIC;
            }
            else if (treeView.SelectedItem == anim)
            {
                System.Console.Write("AnimObj\n");
                args.ObjectType = EventHandler.ObjectTypes.ANIM;
            }
            else if (treeView.SelectedItem == weap)
            {
                System.Console.Write("Weapon\n");
                args.ObjectType = EventHandler.ObjectTypes.WEAPON;
            }
            else if (treeView.SelectedItem == armor)
            {
                System.Console.Write("Armor\n");
                args.ObjectType = EventHandler.ObjectTypes.ARMOR;
            }
            else if (treeView.SelectedItem == ammo)
            {
                System.Console.Write("Ammo\n");
                args.ObjectType = EventHandler.ObjectTypes.AMMO;
            }
            else if (treeView.SelectedItem == dlg)
            {
                System.Console.Write("Dialog\n");
                args.ObjectType = EventHandler.ObjectTypes.DIALOG;
            }
            else if (treeView.SelectedItem == script)
            {
                System.Console.Write("Script\n");
                args.ObjectType = EventHandler.ObjectTypes.SCRIPT;
            }
            else if (treeView.SelectedItem == decal)
            {
                System.Console.Write("Decal\n");
                args.ObjectType = EventHandler.ObjectTypes.DECAL;
            }
            else if (treeView.SelectedItem == audio)
            {
                System.Console.Write("Audio\n");
                args.ObjectType = EventHandler.ObjectTypes.AUDIO;
            }
            else if (treeView.SelectedItem == menu)
            {
                System.Console.Write("Menu\n");
                //args.ObjectType = EventHandler.ObjectTypes.MENU;
            }

            EventHandler.EventManager.OnQueryDataEvent(args);

            foreach (DataSources.BaseData item in args.ReturnList)
            {
                listView.Items.Add(item);
            }
        }

        private void treeView_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            EditMenuItem_Click(sender, null);
        }
    }
}