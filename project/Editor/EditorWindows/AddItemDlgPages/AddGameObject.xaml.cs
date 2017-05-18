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
using System.Windows.Navigation;
using System.Windows.Shapes;
using Editor.EventHandler;

namespace Editor.EditorWindows.AddItemDlgPages
{
    /// <summary>
    /// Interaction logic for AddStatic.xaml
    /// </summary>
    public partial class AddGameObject : UserControl, IAddItemBase
    {
        public AddGameObject()
        {
            InitializeComponent();
        }

        public ObjectTypes GetAddType()
        {
            throw new NotImplementedException();
        }

        public uint GetFormId()
        {
            return UInt32.Parse(IdControl.ObjectID.Text);
        }

        public object GetItemData()
        {
            throw new NotImplementedException();
        }

        public string GetName()
        {
            return IdControl.ObjectName.Text;
        }
    }
}
