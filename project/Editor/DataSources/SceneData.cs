using System;
using System.ComponentModel;

namespace Editor.DataSources
{
    public class SceneData : INotifyPropertyChanged
    {
        
        public SceneData()
        {
        }

        public SceneData(string eid, string name)
        {
            EditorID = eid;
            Name = name;
        }


        public string EditorID { get; set; }

        public string Name { get; set; }

        public event PropertyChangedEventHandler PropertyChanged;

        private void OnPropertyChanged(string info)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(info));
        }
    }
}
