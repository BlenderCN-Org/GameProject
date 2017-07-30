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
                    EventHandler.FormArgs args = new EventHandler.FormArgs()
                    {
                        FormID = baseType.GetFormId(),
                        Data = baseType.GetItemData()
                    };
                    EventHandler.EventManager.OnEditFormEvent(args);
                }
                else
                {
                    EventHandler.AddObjectArgs args = new EventHandler.AddObjectArgs()
                    {
                        Name = baseType.GetName(),
                        FormID = baseType.GetFormId(),
                        ObjectType = baseType.GetAddType()
                    };

                    EventHandler.EventManager.OnAddObjectEvent(args);
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