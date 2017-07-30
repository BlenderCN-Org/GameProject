using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace Editor.EditorWindows.AddItemDlgPages.StandardControls
{
    /// <summary>
    /// Interaction logic for SceneInfo.xaml
    /// </summary>
    public partial class SceneInfo : UserControl
    {
        public SceneInfo()
        {
            InitializeComponent();
            DataContext = new DataSources.Scene();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            System.Windows.Forms.ColorDialog colorDialog = new System.Windows.Forms.ColorDialog();

            System.Windows.Forms.DialogResult res = colorDialog.ShowDialog();

            if (res == System.Windows.Forms.DialogResult.OK)
            {
                if (sender == SkyButton)
                {
                    System.Drawing.Color c = colorDialog.Color;
                    ((SolidColorBrush)skyColor.Fill).Color = Color.FromArgb(c.A, c.R, c.G, c.B);
                    //((DataSources.Scene)DataContext).SkyColor = Color.FromArgb(c.A, c.R, c.G, c.B);
                    //int b = 0;
                }
                else if (sender == FogButton)
                {
                    System.Drawing.Color c = colorDialog.Color;
                    ((SolidColorBrush)FogColor.Fill).Color = Color.FromArgb(c.A, c.R, c.G, c.B);
                    //((DataSources.Scene)DataContext).FogColorNear = Color.FromArgb(c.A, c.R, c.G, c.B);
                }
                else if (sender == FogButton_Copy)
                {
                    System.Drawing.Color c = colorDialog.Color;
                    ((SolidColorBrush)FogColor1.Fill).Color = Color.FromArgb(c.A, c.R, c.G, c.B);
                    //((DataSources.Scene)DataContext).FogColorFar = Color.FromArgb(c.A, c.R, c.G, c.B);
                }
                //skyColor.Fill = new SolidColorBrush(Color.FromArgb(c.A, c.R, c.G, c.B));
            }
        }
    }
}