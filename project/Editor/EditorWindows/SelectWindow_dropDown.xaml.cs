using System.Windows;

namespace Editor.EditorWindows
{
    /// <summary>
    /// Interaction logic for SelectWindow_dropDown.xaml
    /// </summary>
    public partial class SelectWindow_dropDown : Window
    {
        public SelectWindow_dropDown()
        {
            InitializeComponent();
        }

        private void OkButtonClick(object sender, RoutedEventArgs e)
        {
            DialogResult = true;
        }

        private void CancelButtonClick(object sender, RoutedEventArgs e)
        {
            DialogResult = false;
        }
    }
}