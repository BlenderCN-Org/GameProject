using System.ComponentModel;

namespace Editor.DataSources
{
    public class ViewForm : BaseData
    {
        public ViewForm()
        {

        }

        public ViewForm(uint eid, string name) : base(eid, name)
        {
        }

        public ViewForm(uint eid, string name, string _tag) : base(eid, name)
        {
            tag = _tag;
        }

        private string tag;

        public string Tag
        {
            get { return tag; }
            set
            {
                if (tag != value)
                {
                    tag = value;
                    this.OnPropertyChanged("Tag");
                }
            }
        }

        public new event PropertyChangedEventHandler PropertyChanged;

        protected override void OnPropertyChanged(string info)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(info));
        }

    }
}
