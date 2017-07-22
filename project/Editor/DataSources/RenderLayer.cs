using System.ComponentModel;

namespace Editor.DataSources
{
    public class RenderLayer : BaseData
    {
        public RenderLayer() : base() {
        }

        public RenderLayer(uint eid, string name) : base(eid, name)
        {
            resolutionType = 0;
            resolutionWidth = 0;
            resolutionHeight = 0;
            depthBuffer = false;
            stencilBuffer = false;
            numColorAttachments = 1;
            shaderProgram = 0;
        }

        private byte resolutionType;
        private int resolutionWidth;
        private int resolutionHeight;
        bool depthBuffer;
        bool stencilBuffer;
        byte numColorAttachments;
        System.UInt32 shaderProgram;

        public byte ResolutionType
        {
            get { return resolutionType; }
            set
            {
                if (resolutionType != value)
                {
                    resolutionType = value;
                    this.OnPropertyChanged("ResolutionType");
                }
            }
        }

        public int ResolutionWidth
        {
            get { return resolutionWidth; }
            set
            {
                if (resolutionWidth != value)
                {
                    resolutionWidth = value;
                    this.OnPropertyChanged("ResolutionWidth");
                }
            }
        }

        public int ResolutionHeight
        {
            get { return resolutionHeight; }
            set
            {
                if (resolutionHeight != value)
                {
                    resolutionHeight = value;
                    this.OnPropertyChanged("ResolutionHeight");
                }
            }
        }

        public bool DepthBuffer
        {
            get { return depthBuffer; }
            set
            {
                if (depthBuffer != value)
                {
                    depthBuffer = value;
                    this.OnPropertyChanged("DepthBuffer");
                }
            }
        }

        public bool StencilBuffer
        {
            get { return stencilBuffer; }
            set
            {
                if (stencilBuffer != value)
                {
                    stencilBuffer = value;
                    this.OnPropertyChanged("StencilBuffer");
                }
            }
        }

        public byte NumColorAttachments
        {
            get { return numColorAttachments; }
            set
            {
                if (numColorAttachments != value)
                {
                    numColorAttachments = value;
                    this.OnPropertyChanged("NumColorAttachments");
                }
            }
        }

        public System.UInt32 ShaderProgram
        {
            get { return shaderProgram; }
            set
            {
                if (shaderProgram != value)
                {
                    shaderProgram = value;
                    this.OnPropertyChanged("ShaderProgram");
                }
            }
        }

        public new event PropertyChangedEventHandler PropertyChanged;

        private void OnPropertyChanged(string info)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(info));
        }

    }
}
