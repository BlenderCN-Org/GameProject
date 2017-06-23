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
            deleted = false;
        }

        public BaseData(uint eid, string name)
        {
            EditorID = eid;
            Name = name;
        }

        private uint editorID;
        private string name;
        private bool deleted;

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
                    this.OnPropertyChanged("NameWState");
                }
            }
        }

        public string NameWState
        {
            get { return name + (deleted ? " D" : ""); }
        }

        public bool Deleted
        {
            get { return deleted; }
            set
            {
                if(deleted != value)
                {
                    deleted = value;
                    this.OnPropertyChanged("Deleted");
                    this.OnPropertyChanged("NameWState");
                }
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;

        public override string ToString()
        {
            return Name + " ( " + EditorID + " )" + (deleted ? " D" : "");
        }

        private void OnPropertyChanged(string info)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(info));
        }
    }
}
