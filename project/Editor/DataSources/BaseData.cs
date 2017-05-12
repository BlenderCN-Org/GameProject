using System;
using System.ComponentModel;

namespace Editor.DataSources
{
    public class BaseData : INotifyPropertyChanged
    {
        
        public BaseData()
        {
        }

        public BaseData(uint eid, string name)
        {
            EditorID = eid;
            Name = name;
        }


        public uint EditorID { get; set; }

        public string Name { get; set; }

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
