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
    /// Interaction logic for AddItemDialog.xaml
    /// </summary>
    public partial class AddItemDialog : Window
    {
        public AddItemDialog()
        {
            InitializeComponent();
        }

        private void Create_Click(object sender, RoutedEventArgs e)
        {
            AddItemDlgPages.IAddItemBase baseType = PageArea.Children.OfType<AddItemDlgPages.IAddItemBase>().First();

            EventHandler.AddObjectArgs args = new EventHandler.AddObjectArgs()
            {
                Name = baseType.GetName(),
                FormID = baseType.GetFormId(),
                ObjectType = baseType.GetAddType()
            };

            EventHandler.EventManager.OnAddObjectEvent(args);

            Close();

        }

        private void Cancel_Click(object sender, RoutedEventArgs e)
        {

        }
    }
}
