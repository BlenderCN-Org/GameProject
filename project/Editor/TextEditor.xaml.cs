using System.Windows;

namespace Editor
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
