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

        AdornerControls.DropAdorner ad = null;

        // function called when mouse enters Grid
        public new void DragEnter()
        {
            if (ad != null)
            {
                AdornerLayer.GetAdornerLayer(DockingGrid).Remove(ad);
            }
            ad = new AdornerControls.DropAdorner(DockingGrid);
            ad.Width = ActualWidth;
            ad.Height = ActualHeight;
            ad.Content = new AdornerControls.AdornerCenterControl();
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
        public void DragDrop( object source )
        {
            if (ad != null)
            {
                AdornerControls.AdornerCenterControl drop = ad.Content as AdornerControls.AdornerCenterControl;

                // we can drop the content into this Grid
                if( drop.pRect != null)
                {
                    Console.WriteLine("Docking window!");
                    DragWindow wnd = source as DragWindow;
                    //DockingGrid.Children.Add();
                    //DockPanel.SetDock(source as DragWindow, Dock.Left);
                }

                AdornerLayer.GetAdornerLayer(DockingGrid).Remove(ad);
            }
        }

        private void Border_MouseMove(object sender, MouseEventArgs e)
        {
            base.OnMouseMove(e);

            DragWindow wnd = (Parent as Grid).Parent as DragWindow;

            if (wnd != null)
            {
                if (e.LeftButton == MouseButtonState.Pressed)
                {
                    wnd.MoveWindow();
                }
            }

        }
    }
}
