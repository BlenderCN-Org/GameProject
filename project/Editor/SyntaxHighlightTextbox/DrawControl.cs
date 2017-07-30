using System.Windows;
using System.Windows.Media;

namespace Editor.SyntaxHighlightTextbox
{
    public class DrawControl : FrameworkElement
    {
        private VisualCollection visuals;
        private DrawingVisual visual;

        public DrawControl()
        {
            visual = new DrawingVisual();

            visuals = new VisualCollection(this);
            visuals.Add(visual);
        }

        public DrawingContext getContext()
        {
            return visual.RenderOpen();
        }

        protected override int VisualChildrenCount
        {
            get { return visuals.Count; }
        }

        protected override Visual GetVisualChild(int index)
        {
            if (index < 0 || index >= visuals.Count)
            {
                throw new System.Exception("Index out of bounds!");
            }

            return visuals[index];
        }
    }
}