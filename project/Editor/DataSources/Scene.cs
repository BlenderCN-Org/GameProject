using System.Collections.Generic;
using System.ComponentModel;

namespace Editor.DataSources
{
    public class Scene : BaseData
    {
        public Scene() : base()
        {
        }

        public Scene(uint eid, string name) : base(eid, name)
        {
        }

        private System.Windows.Media.Color skyColor;
        private bool hasFog = false;
        private System.Windows.Media.Color fogColorFar;
        private System.Windows.Media.Color fogColorNear;
        private bool hasWater = false;

        public System.Windows.Media.Color SkyColor
        {
            get { return skyColor; }
            set
            {
                if (skyColor != value)
                {
                    skyColor = value;
                    this.OnPropertyChanged("SkyColor");
                }
            }
        }

        public bool HasFog
        {
            get { return hasFog; }
            set
            {
                if (hasFog != value)
                {
                    hasFog = value;
                    this.OnPropertyChanged("HasFog");
                }
            }
        }

        public System.Windows.Media.Color FogColorFar
        {
            get { return fogColorFar; }
            set
            {
                if (fogColorFar != value)
                {
                    fogColorFar = value;
                    this.OnPropertyChanged("FogColorFar");
                }
            }
        }

        public System.Windows.Media.Color FogColorNear
        {
            get { return fogColorNear; }
            set
            {
                if (fogColorNear != value)
                {
                    fogColorNear = value;
                    this.OnPropertyChanged("FogColorNear");
                }
            }
        }

        public bool HasWater
        {
            get { return hasWater; }
            set
            {
                if (hasWater != value)
                {
                    hasWater = value;
                    this.OnPropertyChanged("HasWater");
                }
            }
        }

        public List<object> ChildObjects { get; set; }

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
                    ObjectType = Editor.EventHandler.ObjectTypes.SCENE
                };
                EventHandler.EventManager.OnEditFormEvent(fa);
            }

        }
    }
}