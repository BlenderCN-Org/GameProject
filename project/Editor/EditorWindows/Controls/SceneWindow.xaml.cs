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

        private void SceneList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            DataSources.Scene item = SceneList.SelectedItem as DataSources.Scene;
            if (item != null)
            {
                EventHandler.FormArgs fa = new EventHandler.FormArgs();
                fa.FormID = item.EditorID;
                fa.ObjectType = EventHandler.ObjectTypes.SCENE;

                EventHandler.EventManager.OnSceneChangeEvent(fa);
            }
        }
    }
}