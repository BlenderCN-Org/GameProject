using System.Linq;
using System.Windows;

namespace Editor.EditorWindows
{
    /// <summary>
    /// Interaction logic for AddItemDialog.xaml
    /// </summary>
    public partial class AddEditItemDialog : Window
    {
        private bool editContent = false;

        public AddEditItemDialog(bool edit = false)
        {
            InitializeComponent();
            editContent = edit;

            if (edit)
            {
                Create.Content = "Edit";
            }
        }

        private void Create_Click(object sender, RoutedEventArgs e)
        {
            AddItemDlgPages.IAddItemBase baseType = PageArea.Children.OfType<AddItemDlgPages.IAddItemBase>().First();

            if (baseType.IsValidData())
            {
                if (editContent)
                {
                    DataSources.BaseData data = baseType.GetItemData() as DataSources.BaseData;
                    if (data != null)
                    {
                        EventHandler.FormArgs arg = new EventHandler.FormArgs();
                        arg.Data = data;
                        arg.FormID = data.EditorID;
                        arg.ObjectType = baseType.GetAddType();

                        EventHandler.EventManager.OnEditFormEvent(arg);
                        EventHandler.EventManager.OnRefreshFormsEvent(true);
                    }
                }
                else
                {
                    EventHandler.GetFormIDArgs formArg = new EventHandler.GetFormIDArgs();
                    formArg.FormID = 0;

                    EventHandler.EventManager.OnGetFormIDEvent(formArg);

                    EventHandler.AddObjectArgs args = new EventHandler.AddObjectArgs()
                    {
                        Name = baseType.GetName(),
                        FormID = formArg.FormID,
                        ObjectType = baseType.GetAddType()
                    };

                    EventHandler.EventManager.OnAddObjectEvent(args);

                    DataSources.BaseData staticObject = baseType.GetItemData() as DataSources.BaseData;
                    if (staticObject != null)
                    {
                        staticObject.listenToEvents = true;
                        staticObject.EditorID = formArg.FormID;
                    }

                    EventHandler.EventManager.OnRefreshFormsEvent(true);
                }
                Close();
            }
            else
            {
                MessageBox.Show(baseType.GetFailMessage());
            }
        }

        private void Cancel_Click(object sender, RoutedEventArgs e)
        {
        }
    }
}