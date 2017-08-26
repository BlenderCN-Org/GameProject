using Editor.EventHandler;
using System.Windows.Controls;

namespace Editor.EditorWindows.AddItemDlgPages
{
    /// <summary>
    /// Interaction logic for AddStatic.xaml
    /// </summary>
    public partial class AddStaticObject : UserControl, IAddItemBase
    {
        public AddStaticObject()
        {
            InitializeComponent();
            DataSources.StaticObject obj = new DataSources.StaticObject();
            obj.listenToEvents = false;
            DataContext = obj;
        }

        public ObjectTypes GetAddType()
        {
            return ObjectTypes.STATIC;
        }

        public string GetFailMessage()
        {
            return "";
        }

        public uint GetFormId()
        {
            return 0;
        }

        public object GetItemData()
        {
            return DataContext;
        }

        public string GetName()
        {
            return "";
        }

        public bool IsValidData()
        {
            return true;
        }
    }
}