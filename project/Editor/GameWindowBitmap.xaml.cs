using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
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

namespace Editor
{
    /// <summary>
    /// Interaction logic for GameWindowBitmap.xaml
    /// </summary>
    public partial class GameWindowBitmap : UserControl
    {

        [DllImport("user32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool GetCursorPos(ref Win32Point pt);

        [StructLayout(LayoutKind.Sequential)]
        internal struct Win32Point
        {
            public Int32 X;
            public Int32 Y;
        };

        [DllImport("user32.dll")]
        static extern uint MapVirtualKey(uint uCode, uint uMapType);

        public EventHandler.IEditWindow editWindow = null;

        public GameWindowBitmap()
        {
            InitializeComponent();

            //byte[] a = new byte[(int)ActualWidth * (int)ActualHeight];
            //for (int i = 0; i < a.Length; i++)
            //{
            //    a[i] = (byte)i;
            //}
            //
            //var pixelFormat = PixelFormats.Gray8;
            //var bpp = (pixelFormat.BitsPerPixel + 7) / 8;
            //
            //var bitmap = BitmapSource.Create((int)ActualWidth, (int)ActualHeight, 0, 0, pixelFormat, null, a, (int)ActualWidth * bpp);
            //
            //RenderedBitmap.Source = bitmap;


        }

        bool resized = false;

        public bool wasResized()
        {
            bool res = resized;
            resized = false;
            return res;
        }

        private void UserControl_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            //base.size(e);
            resized = true;

            Console.Write("Editor resize event\n");

            Console.Write("Size ({0}, {1})\n", ActualWidth, ActualHeight);
            if (editWindow != null)
            {
                editWindow.WindowResizeCallback((int)ActualWidth, (int)ActualHeight);
            }
        }

        /*private void GameWindowHolder_ClientSizeChanged(object sender, EventArgs e)
        {
            //base.OnResize(e);
            //resized = true;
            //Console.Write("Editor resize event\n");
            //
            //Console.Write("Size ({0}, {1})\n", ClientSize.Height, ClientSize.Width);

        }*/

        private void SetImage(byte[] ptr, UInt32 wid, UInt32 hei)
        {
            var pixelFormat = PixelFormats.Bgr32;
            var bpp = (pixelFormat.BitsPerPixel + 7) / 8;

            var bitmap = BitmapSource.Create((int)wid, (int)hei, 0, 0, pixelFormat, null, ptr, (int)wid * bpp);

            RenderedBitmap.Source = bitmap;
            //Width = wid;
            //Height = hei;
        }

        public void SetBitMap(IntPtr ptr, UInt32 wid, UInt32 hei)
        {
            if (wid != 0 && hei != 0)
            {
                // copy buffer data before invoking
                byte[] buffer = new byte[wid * hei * 4];
                Marshal.Copy(ptr, buffer, 0, buffer.Length);

                if (!RenderedBitmap.Dispatcher.CheckAccess())
                {
                    RenderedBitmap.Dispatcher.BeginInvoke(new Action(() => { SetImage(buffer, wid, hei); }));
                }
                else
                {
                    SetImage(buffer, wid, hei);
                }
            }
        }

        private void RenderedBitmap_GotFocus(object sender, RoutedEventArgs e)
        {
            if (editWindow != null)
            {
                editWindow.WindowFocusCallback(true);
            }
        }

        private void RenderedBitmap_LostFocus(object sender, RoutedEventArgs e)
        {
            if (editWindow != null)
            {
                editWindow.WindowFocusCallback(false);
            }
        }

        private void RenderedBitmap_MouseMove(object sender, MouseEventArgs e)
        {
            if (editWindow != null)
            {

                Win32Point w32Mouse = new Win32Point();
                GetCursorPos(ref w32Mouse);
                editWindow.WindowMouseMoveCallback(w32Mouse.X, w32Mouse.Y);
            }
        }

        private void RenderedBitmap_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (editWindow != null)
            {
                int button = (int)e.ChangedButton;
                bool down = true;
                int modkey = 0;
                editWindow.WindowMouseButtonCallback(button, down, modkey);
            }
        }

        private void RenderedBitmap_MouseUp(object sender, MouseButtonEventArgs e)
        {
            if (editWindow != null)
            {
                int button = (int)e.ChangedButton;
                bool down = false;
                int modkey = 0;
                editWindow.WindowMouseButtonCallback(button, down, modkey);
            }
        }

        private void UserControl_MouseEnter(object sender, MouseEventArgs e)
        {
            Keyboard.Focus(this);
            if (editWindow != null)
            {
                editWindow.WindowFocusCallback(true);
            }
        }

        private void UserControl_MouseLeave(object sender, MouseEventArgs e)
        {
            Keyboard.Focus(null);
            if (editWindow != null)
            {
                editWindow.WindowFocusCallback(false);
            }
        }

        private void UserControl_KeyDown(object sender, KeyEventArgs e)
        {
            if (editWindow != null)
            {
                var virtualKey = (uint)KeyInterop.VirtualKeyFromKey(e.Key);
                var keyCode = MapVirtualKey(virtualKey, 0);

                int key = (int)keyCode;
                bool down = true;
                int modkey = 0;
                editWindow.WindowKeyboardCallback(key, down, modkey);
            }
        }

        private void UserControl_KeyUp(object sender, KeyEventArgs e)
        {
            if (editWindow != null)
            {
                var virtualKey = (uint)KeyInterop.VirtualKeyFromKey(e.Key);
                var keyCode = MapVirtualKey(virtualKey, 0);

                int key = (int)keyCode;
                bool down = false;
                int modkey = 0;
                editWindow.WindowKeyboardCallback(key, down, modkey);
            }
        }
    }
}

/*
byte[] buffer = ... // must be at least 10000 bytes long in this example

var width = 100; // for example
var height = 100; // for example
var dpiX = 96d;
var dpiY = 96d;
var pixelFormat = PixelFormats.Gray8; // grayscale bitmap
var bytesPerPixel = (pixelFormat.BitsPerPixel + 7) / 8; // == 1 in this example
var stride = bytesPerPixel * width; // == width in this example

var bitmap = BitmapSource.Create(width, height, dpiX, dpiY,
                                 pixelFormat, null, buffer, stride);

    */
