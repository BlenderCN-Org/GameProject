using System;
using System.Windows;
using System.Windows.Input;

namespace Editor
{
	/// <summary>
	/// Interaction logic for MainWindow.xaml
	/// </summary>
	public partial class MainWindow : Window
	{
		public GameWindowHolder gwh = null;
		
		private TextEditor textEdit = null;
		private GameSettings gameSettings = null;

		public MainWindow()
		{
			InitializeComponent();
			gwh = new GameWindowHolder();
			gwh.TopLevel = false;

			// Create the interop host control.
			System.Windows.Forms.Integration.WindowsFormsHost host =
				new System.Windows.Forms.Integration.WindowsFormsHost();

			// Create the MaskedTextBox control.

			// Assign the MaskedTextBox control as the host control's child.
			host.Child = gwh;

			// Add the interop host control to the Grid
			// control's collection of child controls.
			this.GameWindowGrid.Children.Add(host);

		}

		public IntPtr getGameWindowAreaHandle()
		{
			IntPtr handle = IntPtr.Zero;
			if (gwh.InvokeRequired)
			{
				gwh.Invoke(new Action(() => handle = gwh.Handle));
			}
			else
			{
				handle = gwh.Handle;
			}

			return handle;
		}

		public bool WasResized()
		{
			bool resizeing = gwh.wasResized();
			return resizeing;
		}

		public Size getGameWindowSize()
		{
			return new Size(gwh.ClientSize.Width, gwh.ClientSize.Height);
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
			this.WindowStyle = WindowStyle.SingleBorderWindow;

			this.Close();
		}

		private void TextEditorButton_Click(object sender, RoutedEventArgs e)
		{
			if (textEdit == null)
			{
				textEdit = new TextEditor();
			}
			textEdit.Show();
			textEdit.Focus();
		}

		private void GameSettingsButton_Click(object sender, RoutedEventArgs e)
		{
			if (gameSettings == null)
			{
				gameSettings = new GameSettings();
			}
			gameSettings.Show();
			gameSettings.Focus();
		}

		private void SaveCommand_Executed(object sender, ExecutedRoutedEventArgs e)
		{
			EventHandler.SaveEventArgs sav = new EventHandler.SaveEventArgs();
			sav.FileName = "Foobar";
			sav.PackData = false;
			EventHandler.EventManager.OnSaveEvent(sav);
		}

		private void SaveAndPack_Executed(object sender, ExecutedRoutedEventArgs e)
		{
			EventHandler.SaveEventArgs sav = new EventHandler.SaveEventArgs();
			sav.FileName = "Foobar";
			sav.PackData = true;
			EventHandler.EventManager.OnSaveEvent(sav);
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
		
	}

}
