using System.ComponentModel;

namespace Editor.DataSources
{
    public class StaticObject : BaseData
    {
        public StaticObject() : base()
        {
        }

        public StaticObject(uint eid, string name) : base(eid, name)
        {
        }

        private string meshFile;
        private bool useCollision;

        public string MeshFile
        {
            get { return meshFile; }
            set
            {
                if (meshFile != value)
                {
                    meshFile = value;
                    this.OnPropertyChanged("MeshFile");
                }
            }
        }

        public bool UseCollision
        {
            get { return useCollision; }
            set
            {
                if (useCollision != value)
                {
                    useCollision = value; this.OnPropertyChanged("UseCollision");
                }
            }
        }

        public new event PropertyChangedEventHandler PropertyChanged;

        protected override void OnPropertyChanged(string info)
        {
            base.OnPropertyChanged(info);

            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(info));

            if (listenToEvents)
            {
                // do Edit Event
                EventHandler.FormArgs fa = new EventHandler.FormArgs()
                {
                    FormID = EditorID,
                    Data = this,
                    ObjectType = Editor.EventHandler.ObjectTypes.STATIC
                };
                EventHandler.EventManager.OnEditFormEvent(fa);
            }
        }
    }
}