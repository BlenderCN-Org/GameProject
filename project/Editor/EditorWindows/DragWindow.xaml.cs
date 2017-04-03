using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace Editor.EditorWindows
{

    /// <summary>
    /// Interaction logic for DragWindow.xaml
    /// </summary>
    /// 

    public partial class DragWindow : Window
    {
        [System.Runtime.InteropServices.DllImport("user32.dll")]
        private static extern IntPtr WindowFromPoint(System.Drawing.Point pnt);

        [System.Runtime.InteropServices.DllImport("user32.dll")]
        private static extern IntPtr GetWindow(IntPtr hWnd, uint uCmd);

        [System.Runtime.InteropServices.DllImport("user32.dll")]
        private static extern bool GetCursorPos(out System.Drawing.Point pnt);

        [System.Runtime.InteropServices.DllImport("user32.dll")]
        private static extern int MapWindowPoints(IntPtr hWndFrom, IntPtr hWndTo, out System.Drawing.Point lpPoints, uint cPoints);

        const int WM_NCHITTEST = 0x0084;
        const int HTTRANSPARENT = -1;

        bool isDraged = false;

        private IntPtr WndProc(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled)
        {

            if (msg == WM_NCHITTEST && isDraged == true)
            {
                //System.Drawing.Point cursorPos;
                //GetCursorPos(out cursorPos);
                //
                //MapWindowPoints(IntPtr.Zero, hwnd, out cursorPos, 1);

                //if (PtInRect(&s_TransparentRect, cursorPos))
                //    return HTTRANSPARENT;
                handled = true;
                return new IntPtr(HTTRANSPARENT);
            }

            return IntPtr.Zero;
        }

        public DragWindow()
        {
            InitializeComponent();
            BorderHeight.Height = new GridLength(0);
            PanelArea.BorderHeight.Height = new GridLength(20);
        }

        protected override void OnSourceInitialized(EventArgs e)
        {
            base.OnSourceInitialized(e);
            HwndSource source = PresentationSource.FromVisual(this) as HwndSource;
            source.AddHook(WndProc);
        }

        public DragWindow(DragWindow d)
        {
            InitializeComponent();
        }

        public void MoveWindow()
        {
            isDraged = true;
            DragMove();
            isDraged = false;

            if (lastWindow != IntPtr.Zero)
            {
                HwndSource src2 = HwndSource.FromHwnd(lastWindow);
                Window wnd2 = src2.RootVisual as Window;
                Controls.DockGrid dock2 = FindChild<Controls.DockGrid>(wnd2.Content as Grid);
                dock2.DragDrop(this);
                lastWindow = IntPtr.Zero;
            }
        }

        private void Border_MouseMove(object sender, MouseEventArgs e)
        {
            base.OnMouseMove(e);
            if (e.LeftButton == MouseButtonState.Pressed)
            {
                isDraged = true;
                DragMove();
                isDraged = false;

                if (lastWindow != IntPtr.Zero)
                {
                    HwndSource src2 = HwndSource.FromHwnd(lastWindow);
                    Window wnd2 = src2.RootVisual as Window;
                    Controls.DockGrid dock2 = FindChild<Controls.DockGrid>(wnd2.Content as Grid);
                    dock2.DragDrop(this);
                    lastWindow = IntPtr.Zero;
                }

                // Package the data.
                //DataObject data = new DataObject();
                //data.SetData("Object", this);

                // Inititate the drag-and-drop operation.
                //DragDrop.DoDragDrop(this, data, DragDropEffects.Copy | DragDropEffects.Move);
            }
        }

        public Point GetMousePosition()
        {
            System.Drawing.Point point = System.Windows.Forms.Control.MousePosition;
            return new Point(point.X, point.Y);
        }

        /// <summary>
        /// Looks for a child control within a parent by name
        /// </summary>
        public static DependencyObject FindChild(DependencyObject parent, string name)
        {
            // confirm parent and name are valid.
            if (parent == null || string.IsNullOrEmpty(name)) return null;

            if (parent is FrameworkElement && (parent as FrameworkElement).Name == name) return parent;

            DependencyObject result = null;

            if (parent is FrameworkElement) (parent as FrameworkElement).ApplyTemplate();

            int childrenCount = VisualTreeHelper.GetChildrenCount(parent);
            for (int i = 0; i < childrenCount; i++)
            {
                var child = VisualTreeHelper.GetChild(parent, i);
                result = FindChild(child, name);
                if (result != null) break;
            }

            return result;
        }

        /// <summary>
        /// Looks for a child control within a parent by type
        /// </summary>
        public static T FindChild<T>(DependencyObject parent)
            where T : DependencyObject
        {
            // confirm parent is valid.
            if (parent == null) return null;
            if (parent is T) return parent as T;

            DependencyObject foundChild = null;

            if (parent is FrameworkElement) (parent as FrameworkElement).ApplyTemplate();

            int childrenCount = VisualTreeHelper.GetChildrenCount(parent);
            for (int i = 0; i < childrenCount; i++)
            {
                var child = VisualTreeHelper.GetChild(parent, i);
                foundChild = FindChild<T>(child);
                if (foundChild != null) break;
            }

            return foundChild as T;
        }

        private IntPtr lastWindow = IntPtr.Zero;

        private void window_LocationChanged(object sender, EventArgs e)
        {
            PresentationSource ps = PresentationSource.FromVisual(this);
            if (ps != null)
            {
                var transform = ps.CompositionTarget.TransformFromDevice;
                var mouse = transform.Transform(GetMousePosition());

                Point p = mouse;

                System.Drawing.Point pt = new System.Drawing.Point((int)p.X, (int)p.Y);

                IntPtr ptr = WindowFromPoint(pt);

                HwndSource src = HwndSource.FromHwnd(ptr);
                if (src != null)
                {
                    Window wnd = src.RootVisual as Window;
                    if (wnd != null)
                    {
                        Grid grid = wnd.Content as Grid;
                        Controls.DockGrid dock = FindChild<Controls.DockGrid>(grid);

                        // execute Drag over
                        if (ptr == lastWindow)
                        {
                            dock.DragOver();
                        }
                        else
                        {
                            if (lastWindow != IntPtr.Zero)
                            {
                                HwndSource src2 = HwndSource.FromHwnd(lastWindow);
                                Window wnd2 = src2.RootVisual as Window;
                                Controls.DockGrid dock2 = FindChild<Controls.DockGrid>(wnd2.Content as Grid);
                                dock2.DragLeave();
                            }
                            dock.DragEnter();
                            lastWindow = ptr;
                        }
                    }
                }
                else
                {
                    if (lastWindow != IntPtr.Zero)
                    {
                        HwndSource src2 = HwndSource.FromHwnd(lastWindow);
                        Window wnd2 = src2.RootVisual as Window;
                        Controls.DockGrid dock2 = FindChild<Controls.DockGrid>(wnd2.Content as Grid);
                        dock2.DragLeave();
                        lastWindow = IntPtr.Zero;
                    }
                }

            }
        }

        /*protected override void OnMouseMove(MouseEventArgs e)
        {
            base.OnMouseMove(e);

            if (e.LeftButton == MouseButtonState.Pressed)
            {   
                // Package the data.
                DataObject data = new DataObject();
                
                // Inititate the drag-and-drop operation.
                DragDrop.DoDragDrop(this, data, DragDropEffects.Copy | DragDropEffects.Move);
            }
        }*/
    }
}
