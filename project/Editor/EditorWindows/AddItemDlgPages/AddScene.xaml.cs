using Editor.EventHandler;
using System;
using System.Windows.Controls;

namespace Editor.EditorWindows.AddItemDlgPages
{
    /// <summary>
    /// Interaction logic for AddScene.xaml
    /// </summary>
    public partial class AddScene : UserControl, IAddItemBase
    {
        private string failMessage = "";

        public AddScene()
        {
            InitializeComponent();
            GetIDButton_Click(null, null);
        }

        public ObjectTypes GetAddType()
        {
            return ObjectTypes.SCENE;
        }

        public UInt32 GetFormId()
        {
            UInt32 fID;
            try
            {
                fID = UInt32.Parse(FormID.Text);
            }
            catch (Exception)
            {
                fID = 0;
            }

            return fID;
        }

        public string GetName()
        {
            return SceneName.Text;
        }

        public object GetItemData()
        {
            return null;
        }

        private void GetIDButton_Click(object sender, System.Windows.RoutedEventArgs e)
        {
            GetFormIDArgs args = new GetFormIDArgs()
            {
                FormID = 0
            };
            EventManager.OnGetFormIDEvent(args);

            FormID.Text = args.FormID.ToString();
        }

        public bool IsValidData()
        {
            if (SceneName.Text.Length > 0 && FormID.Text.Length > 0)
            {
                return true;
            }
            else if (SceneName.Text.Length == 0)
            {
                failMessage = "Scene Name cannot be empty";
                return false;
            }
            else if (FormID.Text.Length == 0)
            {
                failMessage = "No from ID set";
                return false;
            }
            failMessage = "Unknown fault in AddScene.IsValidData()";
            return false;
        }

        public string GetFailMessage()
        {
            return failMessage;
        }
    }
}