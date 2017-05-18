using System;
using System.Windows.Controls;
using Editor.EventHandler;

namespace Editor.EditorWindows.AddItemDlgPages
{
    /// <summary>
    /// Interaction logic for AddScene.xaml
    /// </summary>
    public partial class AddScene : UserControl, IAddItemBase
    {
        public AddScene()
        {
            InitializeComponent();
        }

        public ObjectTypes GetAddType()
        {
            return ObjectTypes.SCENE;
        }

        public UInt32 GetFormId()
        {
            //return FormID.Text;
            return 0;
        }

        public string GetName()
        {
            return SceneName.Text;
        }

        public object GetItemData()
        {
            return null;
        }

    }
}
