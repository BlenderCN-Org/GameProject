using System;
using System.ComponentModel;

namespace Editor.DataSources
{
    public class BaseData : INotifyPropertyChanged
    {
        
        public BaseData()
        {
            Name = "Test";
            EditorID = 123;
        }

        public BaseData(uint eid, string name)
        {
            EditorID = eid;
            Name = name;
        }

        private uint editorID;
        private string name;

        public uint EditorID
        {
            get { return editorID; }
            set {
                if (editorID != value)
                {
                    editorID = value;
                    this.OnPropertyChanged("EditorID");
                }
            }
        }

        public string Name
        {
            get { return name; }
            set
            {
                if (name != value)
                {
                    name = value;
                    this.OnPropertyChanged("Name");
                }
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;

        public override string ToString()
        {
            return Name + " ( " + EditorID + " )";
        }

        private void OnPropertyChanged(string info)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(info));
        }
    }
}
