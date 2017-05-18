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
        }

        void TreeViewItem_PreviewMouseRightButtonDown(object sender, MouseButtonEventArgs e)
        {
            TreeViewItem treeViewItem =
                      VisualUpwardSearch<TreeViewItem>(e.OriginalSource as DependencyObject);

            if (treeViewItem != null)
            {
                treeViewItem.IsSelected = true;
                e.Handled = true;
            }

        }

        static T VisualUpwardSearch<T>(DependencyObject source) where T : DependencyObject
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
            dlg.PageArea.Children.Add(new EditorWindows.AddItemDlgPages.AddGameObject());
            dlg.Show();
        }

        private void EditMenuItem_Click(object sender, RoutedEventArgs e)
        {
            bool edit = true;
            AddEditItemDialog dlg = new AddEditItemDialog(edit);
            dlg.PageArea.Children.Add(new EditorWindows.AddItemDlgPages.AddGameObject());
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
            if(listView.SelectedIndex != -1)
            {
                DeleteItem.IsEnabled = true;
            }
        }
    }
}
