using System.Collections.Generic;
using System.Windows.Controls;

namespace Editor.EditorWindows.Controls
{
    /// <summary>
    /// Interaction logic for SceneWindow.xaml
    /// </summary>
    public partial class SceneWindow : UserControl
    {
        public SceneWindow()
        {
            InitializeComponent();

            EventHandler.EventManager.onRefreshFormsEvent += new System.EventHandler<bool>(refreshEvent);
            refreshEvent(null, true);
        }

        public void refreshEvent(object sender, bool refresh)
        {
            EventHandler.QueryDataArgs args = new Editor.EventHandler.QueryDataArgs();

            SceneList.Items.Clear();

            args.ObjectType = Editor.EventHandler.ObjectTypes.SCENE;
            args.ReturnList = new List<DataSources.BaseData>();
            EventHandler.EventManager.OnQueryDataEvent(args);

            foreach (var item in args.ReturnList)
            {
                SceneList.Items.Add(item);
            }

        }
    }
}