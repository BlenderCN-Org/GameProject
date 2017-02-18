using System;
using System.Windows;

namespace Editor
{
	/// <summary>
	/// Interaction logic for MainWindow.xaml
	/// </summary>
	public partial class MainWindow : Window
	{
		public GameWindowHolder gwh = null;
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
			if (gwh.InvokeRequired) {
				gwh.Invoke(new Action(() => handle = gwh.Handle));
			} else {
				handle = gwh.Handle;
			}

			return handle;
		}

		private void Rectangle_PreviewMouseDown(object sender, System.Windows.Input.MouseButtonEventArgs e)
		{
			if(e.ChangedButton == System.Windows.Input.MouseButton.Left)
				this.DragMove();
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
	}
}
