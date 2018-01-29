﻿using System;
using System.ComponentModel;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Media3D;

namespace Editor
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    ///

    public partial class MainWindow : Window, IDisposable
    {
        private EditorWindows.TextEditor textEdit = null;
        private EditorWindows.GameSettings gameSettings = null;
        private EditorWindows.BrowseAllForms formBrowser = null;

        public EventHandler<EventHandler.CloseArgs> onCloseEvent;

        public MainWindow()
        {
            InitializeComponent();

            EditorWindows.ShaderEditor shaderEditor = new EditorWindows.ShaderEditor();
            shaderEditor.Show();
            shaderEditor.Owner = this;

            //EditorWindows.AddEditItemDialog addDlg = new EditorWindows.AddEditItemDialog();
            //
            //addDlg.PageArea.Children.Add(bm);
            //
            //addDlg.Show();

            //EditorWindows.DragWindow wnd1 = new EditorWindows.DragWindow();
            //EditorWindows.DragWindow wnd2 = new EditorWindows.DragWindow();
            //
            //wnd1.PanelArea.DockingGrid.Children.Add(new EditorWindows.Controls.ObjectList());
            //wnd2.PanelArea.DockingGrid.Children.Add(new EditorWindows.Controls.ObjectList());
            //
            //wnd1.Show();
            //wnd2.Show();

            //EditorWindows.AddEditItemDialog dlg = new EditorWindows.AddEditItemDialog();
            //EditorWindows.AddItemDlgPages.StandardControls.ColorPicker colorPicker = new EditorWindows.AddItemDlgPages.StandardControls.ColorPicker();
            //dlg.PageArea.Children.Add(colorPicker);
            //dlg.Show();
        }

        ~MainWindow()
        {
            textEdit.Close();
            gameSettings.Close();
            formBrowser.Close();
        }

        protected override void OnClosing(CancelEventArgs e)
        {
            base.OnClosing(e);
            e.Cancel = true;
        }

        public void invokeShow()
        {
            if (!Dispatcher.CheckAccess())
            {
                Dispatcher.Invoke(new Action(() =>
                {
                    Show();
                })
                );
            }
            else
            {
                Show();
            }
        }

        public void SetEditWindow(EventHandler.IEditWindow editWindow)
        {
            gameBitMap.editWindow = editWindow;

            double w = 0.0;
            double h = 0.0;
            if (!gameBitMap.Dispatcher.CheckAccess())
            {
                gameBitMap.Dispatcher.Invoke(new Action(() =>
                {
                    w = gameBitMap.ActualWidth;
                    h = gameBitMap.ActualHeight;
                })
                );
            }
            else
            {
                w = gameBitMap.ActualWidth;
                h = gameBitMap.ActualHeight;
            }

            editWindow.WindowResizeCallback((int)w, (int)h);
        }

        public void DrawGameWindowPixels(IntPtr pixels, UInt32 width, UInt32 height)
        {
            gameBitMap.SetBitMap(pixels, width, height);
        }

        public bool WasResized()
        {
            bool resizeing = gameBitMap.wasResized();
            return resizeing;
        }

        public Size getGameWindowSize()
        {
            double w = 0.0;
            double h = 0.0;
            if (!gameBitMap.Dispatcher.CheckAccess())
            {
                gameBitMap.Dispatcher.Invoke(new Action(() =>
                {
                    w = gameBitMap.ActualWidth;
                    h = gameBitMap.ActualHeight;
                })
                );
            }
            else
            {
                w = gameBitMap.ActualWidth;
                h = gameBitMap.ActualHeight;
            }
            return new Size(w, h);
        }

        private void Rectangle_PreviewMouseDown(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            if (e.ChangedButton == System.Windows.Input.MouseButton.Left)
            {
                if (e.ClickCount == 2)
                {
                    Btnmax_Click(null, null);
                }
                else
                {
                    this.DragMove();
                }
            }
        }

        private void Btnmin_Click(object sender, RoutedEventArgs e)
        {
            this.WindowStyle = WindowStyle.SingleBorderWindow;
            this.WindowState = WindowState.Minimized;
            this.WindowStyle = WindowStyle.None;
        }

        private void Btnmax_Click(object sender, RoutedEventArgs e)
        {
            this.WindowStyle = WindowStyle.SingleBorderWindow;
            if (this.WindowState == WindowState.Maximized)
                this.WindowState = WindowState.Normal;
            else
                this.WindowState = WindowState.Maximized;
            this.WindowStyle = WindowStyle.None;
        }

        private void Btnclose_Click(object sender, RoutedEventArgs e)
        {
            EventHandler.CloseArgs close = new EventHandler.CloseArgs();
            close.close = EventHandler.CloseType.CLOSE;

            onCloseEvent?.Invoke(null, close);

            if (close.close == EventHandler.CloseType.CLOSE)
            {
                this.Close();
            }
            else if (close.close == EventHandler.CloseType.HIDE)
            {
                Hide();
            }
        }

        private void TextEditorButton_Click(object sender, RoutedEventArgs e)
        {
            if (textEdit == null)
            {
                textEdit = new EditorWindows.TextEditor();
                textEdit.Owner = this;
            }
            textEdit.Show();
            textEdit.Focus();
        }

        private void GameSettingsButton_Click(object sender, RoutedEventArgs e)
        {
            if (gameSettings == null)
            {
                gameSettings = new EditorWindows.GameSettings();
                gameSettings.Owner = this;
            }
            gameSettings.Show();
            gameSettings.Focus();
        }

        private void SaveCommand_Executed(object sender, ExecutedRoutedEventArgs e)
        {
            EventHandler.SaveEventArgs sav = new EventHandler.SaveEventArgs();
            sav.PackData = false;
            EventHandler.EventManager.OnSaveEvent(sav);
        }

        private void SaveAndPack_Executed(object sender, ExecutedRoutedEventArgs e)
        {
            EventHandler.SaveEventArgs sav = new EventHandler.SaveEventArgs();
            sav.PackData = true;
            EventHandler.EventManager.OnSaveEvent(sav);
        }

        protected virtual void Dispose(bool managed)
        {
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        private void TreeViewItem_PreviewMouseRightButtonDown(object sender, MouseButtonEventArgs e)
        {
            TreeViewItem treeViewItem =
                      VisualUpwardSearch<TreeViewItem>(e.OriginalSource as DependencyObject);

            if (treeViewItem != null)
            {
                treeViewItem.IsSelected = true;
                e.Handled = true;
            }
        }

        private static T VisualUpwardSearch<T>(DependencyObject source) where T : DependencyObject
        {
            DependencyObject returnVal = source;

            while (returnVal != null && !(returnVal is T))
            {
                DependencyObject tempReturnVal = null;
                if (returnVal is Visual || returnVal is Visual3D)
                {
                    tempReturnVal = VisualTreeHelper.GetParent(returnVal);
                }
                if (tempReturnVal == null)
                {
                    returnVal = LogicalTreeHelper.GetParent(returnVal);
                }
                else returnVal = tempReturnVal;
            }

            return returnVal as T;
        }

        private void ObjectBrowser(object sender, ExecutedRoutedEventArgs e)
        {
            if (formBrowser == null)
            {
                formBrowser = new EditorWindows.BrowseAllForms();
                formBrowser.Owner = this;
            }
            formBrowser.Show();
        }
    }

    public static class MainWindowCommands
    {
        public static readonly RoutedUICommand SaveAndPack = new RoutedUICommand
                (
                        "S_ave And Pack",
                        "SaveAndPack",
                        typeof(MainWindowCommands),
                        new InputGestureCollection()
                        {
                                        new KeyGesture(Key.S, ModifierKeys.Control | ModifierKeys.Shift)
                        }
                );

        public static readonly RoutedUICommand ObjectBrowser = new RoutedUICommand
                (
                        "ObjectBrowser",
                        "ObjectBrowser",
                        typeof(MainWindowCommands),
                        new InputGestureCollection()
                        {
                        }
                );
    }
}