using Editor.EventHandler;
using System;
using System.Windows.Controls;

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

        public string GetFailMessage()
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

        public bool IsValidData()
        {
            throw new NotImplementedException();
        }
    }
}