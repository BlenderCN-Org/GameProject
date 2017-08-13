using Editor.DataSources;
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
    /// Interaction logic for BrowseAllForms.xaml
    /// </summary>
    public partial class BrowseAllForms : Window
    {
        public BrowseAllForms()
        {
            InitializeComponent();

            EventHandler.FormView fView = new EventHandler.FormView();

            fView.ReturnList = new List<ViewForm>();
            EventHandler.EventManager.OnGetFormView(fView);

            FormList.ItemsSource = fView.ReturnList;

            CollectionView view = (CollectionView)CollectionViewSource.GetDefaultView(FormList.ItemsSource);
            view.Filter = FormFilter;

            EventHandler.EventManager.onRefreshFormsEvent += new EventHandler<bool>(RefreshEvent);

        }

        bool refreshOk = true;

        public void RefreshEvent(object sender, bool refresh)
        {
            if (refresh && refreshOk)
            {
                EventHandler.FormView fView = new EventHandler.FormView();

                fView.ReturnList = new List<ViewForm>();
                EventHandler.EventManager.OnGetFormView(fView);

                FormList.ItemsSource = fView.ReturnList;

                CollectionView view = (CollectionView)CollectionViewSource.GetDefaultView(FormList.ItemsSource);
                view.Filter = FormFilter;
            }
        }

        private bool FormFilter(object item)
        {
            if (String.IsNullOrEmpty(searchBox.Text))
            {
                return true;
            }
            else
            {
                ViewForm vf = item as ViewForm;
                bool foundInTag = (vf.Tag.IndexOf(searchBox.Text, StringComparison.OrdinalIgnoreCase) >= 0);
                bool foundInId = (vf.EditorID.ToString().IndexOf(searchBox.Text, StringComparison.OrdinalIgnoreCase) >= 0);

                return (foundInId || foundInTag);
            }
        }

        private void searchBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            CollectionViewSource.GetDefaultView(FormList.ItemsSource).Refresh();
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            e.Cancel = true;
            Hide();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            ConfirmDialog cfDlg = new ConfirmDialog();
            cfDlg.Owner = this;
            cfDlg.ShowDialog();

            if ((cfDlg.DialogResult.HasValue == true) && (cfDlg.DialogResult.Value == true))
            {
                if (FormList.SelectedItems.Count > 1)
                {
                    refreshOk = false;
                    EventHandler.EventManager.SendRefresEvent = false;
                    foreach (DataSources.BaseData itm in FormList.SelectedItems )
                    {
                        EventHandler.FormArgs fa = new EventHandler.FormArgs()
                        {
                            FormID = (itm as DataSources.BaseData).EditorID
                        };
                        (itm as DataSources.BaseData).Deleted = true;
                        EventHandler.EventManager.OnDeleteFormEvent(fa);
                    }
                    refreshOk = true;
                    EventHandler.EventManager.SendRefresEvent = true;
                    EventHandler.EventManager.OnRefreshFormsEvent(true);
                }
                else
                {
                    EventHandler.FormArgs fa = new EventHandler.FormArgs()
                    {
                        FormID = (FormList.SelectedItem as DataSources.BaseData).EditorID
                    };
                    (FormList.SelectedItem as DataSources.BaseData).Deleted = true;
                    EventHandler.EventManager.OnDeleteFormEvent(fa);
                }
            }
            else
            {
                Console.WriteLine("Abort delete");
            }

        }

        private void FakeEdit_Click(object sender, RoutedEventArgs e)
        {
            ConfirmDialog cfDlg = new ConfirmDialog();
            cfDlg.Owner = this;
            cfDlg.ConfirmMessage.Text = "Are you sure you want to edit all Selected items?";
            cfDlg.ShowDialog();

            if ((cfDlg.DialogResult.HasValue == true) && (cfDlg.DialogResult.Value == true))
            {
                if (FormList.SelectedItems.Count > 1)
                {
                    refreshOk = false;
                    EventHandler.EventManager.SendRefresEvent = false;
                    foreach (DataSources.BaseData itm in FormList.SelectedItems)
                    {
                        EventHandler.FormArgs fa = new EventHandler.FormArgs()
                        {
                            FormID = (itm as DataSources.BaseData).EditorID
                        };
                        (itm as DataSources.BaseData).Deleted = true;
                        EventHandler.EventManager.OnEditFormEvent(fa);
                    }
                    refreshOk = true;
                    EventHandler.EventManager.SendRefresEvent = true;
                    EventHandler.EventManager.OnRefreshFormsEvent(true);
                }
                else
                {
                    EventHandler.FormArgs fa = new EventHandler.FormArgs()
                    {
                        FormID = (FormList.SelectedItem as DataSources.BaseData).EditorID
                    };
                    (FormList.SelectedItem as DataSources.BaseData).Deleted = true;
                    EventHandler.EventManager.OnEditFormEvent(fa);
                }
            }
            else
            {
                
            }

        }
    }

}
