using System;
using System.Globalization;
using System.Windows.Controls;
using System.Windows.Data;

namespace Editor.EditorWindows.AddItemDlgPages.StandardControls
{
    /// <summary>
    /// Interaction logic for RenderLayerInfo.xaml
    /// </summary>
    ///

    public class EnumByteConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return value.Equals((byte)parameter);
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return ((bool)value) ? (byte)parameter : Binding.DoNothing;
        }
    }

    public enum ResolutionType : byte
    {
        Dynamic = 0,
        Fixed = 1
    }

    public partial class RenderLayerInfo : UserControl
    {
        public RenderLayerInfo()
        {
            InitializeComponent();
        }
    }
}