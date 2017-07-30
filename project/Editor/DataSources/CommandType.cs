using System;
using System.ComponentModel;

namespace Editor.DataSources
{
    public class CommandType : INotifyPropertyChanged
    {
        private int type;
        private string description;

        public int Type
        {
            get
            {
                return type;
            }
            set
            {
                if (type != value)
                {
                    type = value;
                    this.OnPropertyChanged("Command");
                }
            }
        }

        public string Description
        {
            get
            {
                return description;
            }
            set
            {
                if (description != value)
                {
                    description = value;
                    this.OnPropertyChanged("Command");
                }
            }
        }

        public string Command => ToString();

        public event PropertyChangedEventHandler PropertyChanged;

        protected virtual void OnPropertyChanged(String propertyName)
        {
            if (this.PropertyChanged != null)
                this.PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
        }

        public override string ToString()
        {
            return Description;
        }
    }
}