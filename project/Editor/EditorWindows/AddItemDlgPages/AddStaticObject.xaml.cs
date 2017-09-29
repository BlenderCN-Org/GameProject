using Editor.EventHandler;
using System.IO;
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

        private void BrowseMesh_Click(object sender, System.Windows.RoutedEventArgs e)
        {
            System.Windows.Forms.OpenFileDialog fileDialog = new System.Windows.Forms.OpenFileDialog();
            fileDialog.DefaultExt = ".mesh";
            fileDialog.Filter = "Model files (*.mesh) | *.mesh";
            fileDialog.Title = "Open";
            fileDialog.InitialDirectory = Directory.GetCurrentDirectory();
            System.Windows.Forms.DialogResult result = fileDialog.ShowDialog();

            if(result == System.Windows.Forms.DialogResult.OK)
            {
                (DataContext as DataSources.StaticObject).MeshFile = fileDialog.FileName.Replace(Directory.GetCurrentDirectory()+"\\", "");
            }
        }
    }
}