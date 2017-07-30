using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace Editor.EditorWindows
{
    /// <summary>
    /// Interaction logic for TextEditor.xaml
    /// </summary>
    public partial class TextEditor : Window
    {
        public TextEditor()
        {
            InitializeComponent();
        }

        private void TextEditor_closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            e.Cancel = true;
            Hide();
        }

        private void TabItem_PreviewMouseMove(object sender, MouseEventArgs e)
        {
            if (!(e.OriginalSource is Button))
            {
                var tabItem = e.Source as TabItem;

                if (tabItem == null)
                {
                    tabItem = e.Source as CloseableTab.TextEditorTabItem;
                }

                if (tabItem == null)
                {
                    return;
                }

                if (Mouse.PrimaryDevice.LeftButton == MouseButtonState.Pressed)
                {
                    Console.Write("DragDrop\n");
                    DragDrop.DoDragDrop(tabItem, tabItem, DragDropEffects.All);
                }
            }
        }

        private void TabItem_Drop(object sender, DragEventArgs e)
        {
            if (!(e.OriginalSource is Button))
            {
                var tabItemTarget = e.Source as TabItem;

                if (tabItemTarget == null)
                {
                    tabItemTarget = (e.Source as CloseableTab.CloseableHeader).Parent as TabItem;
                }

                var tabItemSource = e.Data.GetData(typeof(TabItem)) as TabItem;

                if (tabItemSource == null)
                {
                    tabItemSource = e.Data.GetData(typeof(CloseableTab.TextEditorTabItem)) as CloseableTab.TextEditorTabItem;
                }

                if (tabItemTarget != null)
                {
                    if (!tabItemTarget.Equals(tabItemSource))
                    {
                        var tabControl = tabItemTarget.Parent as TabControl;
                        int sourceIndex = tabControl.Items.IndexOf(tabItemSource);
                        int targetIndex = tabControl.Items.IndexOf(tabItemTarget);

                        tabControl.Items.Remove(tabItemSource);
                        tabControl.Items.Insert(targetIndex, tabItemSource);

                        tabControl.Items.Remove(tabItemTarget);
                        tabControl.Items.Insert(sourceIndex, tabItemTarget);

                        tabControl.SelectedItem = tabItemSource;
                    }
                }
            }
        }

        private void MenuItem_Click(object sender, RoutedEventArgs e)
        {
            CloseableTab.TextEditorTabItem tb = new CloseableTab.TextEditorTabItem();
            tabControl.Items.Add(tb);
            tabControl.SelectedItem = tb;
        }
    }
}