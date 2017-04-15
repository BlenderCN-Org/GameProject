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

        public string GetFormId()
        {
            return FormID.Text;
        }

        public string GetName()
        {
            return SceneName.Text;
        }
    }
}
