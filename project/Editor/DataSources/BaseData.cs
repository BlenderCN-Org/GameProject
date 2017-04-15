using System;
using System.ComponentModel;

namespace Editor.DataSources
{
    public class BaseData : INotifyPropertyChanged
    {
        
        public BaseData()
        {
        }

        public BaseData(string eid, string name)
        {
            EditorID = eid;
            Name = name;
        }


        public string EditorID { get; set; }

        public string Name { get; set; }

        public event PropertyChangedEventHandler PropertyChanged;

        public override string ToString()
        {

            if (EditorID != null && EditorID.Length > 0)
                return Name + " ( " + EditorID + " )";
            else
                return Name + " ( No ID found )";
        }

        private void OnPropertyChanged(string info)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(info));
        }
    }
}
