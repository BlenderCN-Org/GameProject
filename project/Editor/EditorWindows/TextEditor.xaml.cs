using System.ComponentModel.Design;
using System.Windows;
using System.Windows.Forms;
using System.Windows.Forms.Integration;

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
    }
}
