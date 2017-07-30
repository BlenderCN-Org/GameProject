using System.Collections.Generic;
using System.Globalization;
using System.Windows.Controls;
using System.Windows.Media;

namespace Editor
{
    /// <summary>
    /// Interaction logic for DarkSyntaxHighlightedTextbox.xaml
    /// </summary>
    public partial class DarkSyntaxHighlightedTextbox : TextBox
    {
        private SyntaxHighlightTextbox.DrawControl linenumberContext;
        private SyntaxHighlightTextbox.DrawControl renderContext;

        private ScrollViewer scrollView;

        private List<LineBlock> lineblocks;
        private CultureInfo culture;
        private Typeface typeface;

        private int totalLines;

        public DarkSyntaxHighlightedTextbox()
        {
            InitializeComponent();

            totalLines = 1;
            lineblocks = new List<LineBlock>();

            culture = CultureInfo.InvariantCulture;
            typeface = new Typeface(this.FontFamily, this.FontStyle, this.FontWeight, this.FontStretch);

            Loaded += (s, e) =>
            {
                bool built = ApplyTemplate();
                linenumberContext = (SyntaxHighlightTextbox.DrawControl)Template.FindName("LineNumberCanvas", this);
                renderContext = (SyntaxHighlightTextbox.DrawControl)Template.FindName("RenderCanvas", this);
                scrollView = (ScrollViewer)Template.FindName("PART_ContentHost", this);

                linenumberContext.Width = getFormattedTextWidth(string.Format("{0:0000}", totalLines)) + 5;

                scrollView.ScrollChanged += ScrollView_ScrollChanged;
            };

            TextChanged += DarkSyntaxHighlightedTextbox_TextChanged;

            InvalidateMeasure();
            UpdateLayout();
        }

        private void ScrollView_ScrollChanged(object sender, ScrollChangedEventArgs e)
        {
            InvalidateVisual();
        }

        private void DarkSyntaxHighlightedTextbox_TextChanged(object sender, TextChangedEventArgs e)
        {
            IEnumerator<TextChange> it = e.Changes.GetEnumerator();
            it.MoveNext();
            if (it.Current == null)
                return;

            //Console.WriteLine("{0}, {1}, {2}", it.Current.AddedLength, it.Current.Offset, it.Current.RemovedLength);

            InvalidateMeasure();
            UpdateLayout();
            InvalidateVisual();
        }

        protected override void OnRender(DrawingContext drawingContext)
        {
            renderLineBlocks();

            base.OnRender(drawingContext);
        }

        private void renderLineBlocks()
        {
            if (linenumberContext == null)
            {
                return;
            }

            DrawingContext dc = linenumberContext.getContext();

            int firstLine = GetFirstVisibleLineIndex();
            int lastLine = GetLastVisibleLineIndex() + 1;

            int counter = 0;
            double h = 0;

            for (int i = firstLine; i < lastLine; i++)
            {
                FormattedText number = new FormattedText((i + 1).ToString(), culture, System.Windows.FlowDirection.RightToLeft, typeface, FontSize, Brushes.White);
                linenumberContext.Width = getFormattedTextWidth(string.Format("{0:00000}", totalLines)) + 5;
                dc.DrawText(number, new System.Windows.Point(linenumberContext.ActualWidth, 1 + (FontSize * 1.3333 * counter)));
                h = number.Height;
                counter++;
            }

            dc.Close();
        }

        private double getFormattedTextWidth(string text)
        {
            FormattedText ft = new FormattedText(
                text,
                System.Globalization.CultureInfo.InvariantCulture,
                System.Windows.FlowDirection.LeftToRight,
                new Typeface(FontFamily, FontStyle, FontWeight, FontStretch),
                FontSize,
                Brushes.Black);

            ft.Trimming = System.Windows.TextTrimming.None;
            ft.LineHeight = FontSize * 1.3;

            return ft.Width;
        }

        private class LineBlock
        {
        }
    }
}