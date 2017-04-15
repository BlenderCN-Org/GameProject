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
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Editor.EditorWindows.Controls
{
    /// <summary>
    /// Interaction logic for DockGrid.xaml
    /// </summary>
    public partial class DockGrid : UserControl
    {
        public DockGrid()
        {
            InitializeComponent();
            BorderHeight.Height = new GridLength(0);
        }

        public void EnableBorder(bool enable)
        {
            if (enable)
            {
                BorderHeight.Height = new GridLength(20);
            }
            else
            {
                BorderHeight.Height = new GridLength(0);
            }
        }
        bool tempAllowMove = false;
        AdornerControls.DropAdorner ad = null;

        // function called when mouse enters Grid
        public new void DragEnter()
        {
            if (ad != null)
            {
                AdornerLayer.GetAdornerLayer(DockingGrid).Remove(ad);
            }
            ad = new AdornerControls.DropAdorner(DockingGrid)
            {
                Width = ActualWidth,
                Height = ActualHeight,
                Content = new AdornerControls.AdornerCenterControl()
            };
            AdornerLayer.GetAdornerLayer(DockingGrid).Add(ad);
        }

        // function called when leaving Grid
        public new void DragLeave()
        {
            if (ad != null)
            {
                AdornerLayer.GetAdornerLayer(DockingGrid).Remove(ad);
            }
        }

        // function called while we are draging mouse inside Grid
        public new void DragOver()
        {
            //if (ad != null)
            //{
            //    AdornerLayer.GetAdornerLayer(DockingGrid).Remove(ad);
            //}
            //ad = new AdornerControls.DropAdorner(DockingGrid);
            //ad.Width = ActualWidth;
            //ad.Height = ActualHeight;
            //ad.Content = new AdornerControls.AdornerCenterControl();
            //AdornerLayer.GetAdornerLayer(DockingGrid).Add(ad);

        }

        // function called when we release drag inside Grid
        public void DragDrop(object source)
        {
            if (ad != null)
            {
                AdornerControls.AdornerCenterControl drop = ad.Content as AdornerControls.AdornerCenterControl;

                // we can drop the content into this Grid
                if (drop.GetDrop() == AdornerControls.AdornerCenterControl.dropLocation.center)
                {
                    // center drop should add tab control things
                    Console.WriteLine("Docking window! center");
                    DragWindow wnd = source as DragWindow;
                    DockGrid panel = wnd.PanelArea;
                    (panel.Parent as Grid).Children.Remove(panel);
                    DockingGrid.Children.Add(panel);
                    wnd.Close();
                }
                else if (drop.GetDrop() == AdornerControls.AdornerCenterControl.dropLocation.top)
                {
                    Console.WriteLine("Docking window! top");
                    DragWindow wnd = source as DragWindow;
                    DockGrid panel = wnd.PanelArea;
                    (panel.Parent as Grid).Children.Remove(panel);
                    DockingGrid.Children.Add(panel);
                    wnd.Close();

                }
                else if (drop.GetDrop() == AdornerControls.AdornerCenterControl.dropLocation.bottom)
                {
                    Console.WriteLine("Docking window! bottom");
                    DragWindow wnd = source as DragWindow;
                    DockGrid panel = wnd.PanelArea;
                    (panel.Parent as Grid).Children.Remove(panel);
                    DockingGrid.Children.Add(panel);
                    wnd.Close();
                }
                else if (drop.GetDrop() == AdornerControls.AdornerCenterControl.dropLocation.left)
                {
                    Console.WriteLine("Docking window! left");
                    DragWindow wnd = source as DragWindow;
                    DockGrid panel = wnd.PanelArea;
                    (panel.Parent as Grid).Children.Remove(panel);
                    DockingGrid.Children.Add(panel);
                    wnd.Close();
                }
                else if (drop.GetDrop() == AdornerControls.AdornerCenterControl.dropLocation.right)
                {
                    Console.WriteLine("Docking window! right");
                    DragWindow wnd = source as DragWindow;
                    DockGrid panel = wnd.PanelArea;
                    (panel.Parent as Grid).Children.Remove(panel);
                    DockingGrid.Children.Add(panel);
                    wnd.Close();
                }

                AdornerLayer.GetAdornerLayer(DockingGrid).Remove(ad);
            }
        }

        private void Border_MouseMove(object sender, MouseEventArgs e)
        {
            DragWindow wnd = (Parent as Grid)?.Parent as DragWindow;

            if (wnd != null)
            {
                if (e.LeftButton == MouseButtonState.Pressed && tempAllowMove)
                {
                    wnd.MoveWindow();
                    tempAllowMove = false;
                }
            }
        }

        private void Border_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            base.OnMouseMove(e);

            DragWindow wnd = (Parent as Grid)?.Parent as DragWindow;

            if (wnd != null)
            {
                if (e.LeftButton == MouseButtonState.Pressed)
                {
                    wnd.MoveWindow();
                }
            }
            else
            {
                Grid panel = Parent as Grid;
                if (panel != null && e.LeftButton == MouseButtonState.Pressed)
                {
                    // remove my self from the panel

                    Point renderedLocation = this.PointToScreen(new Point(0, 0));

                    panel.Children.Remove(this);

                    wnd = new DragWindow();
                    DockGrid grid = wnd.PanelArea;
                    Grid gridParent = grid.Parent as Grid;
                    gridParent.Children.Remove(grid);
                    gridParent.Children.Add(this);

                    wnd.PanelArea = this;

                    wnd.Left = renderedLocation.X;
                    wnd.Top = renderedLocation.Y;
                    wnd.Width = ActualWidth;
                    wnd.Height = ActualHeight;
                    wnd.Show();
                    tempAllowMove = true;
                }
            }
        }
    }
}
