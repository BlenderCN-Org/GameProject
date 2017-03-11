using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Editor
{
	public partial class GameWindowHolder : Form
	{
		bool resized = false;
		public GameWindowHolder()
		{
			InitializeComponent();
		}

		public bool wasResized()
		{
			bool res = resized;
			resized = false;
			return res;
		}

		private void GameWindowHolder_ClientSizeChanged(object sender, EventArgs e)
		{
			base.OnResize(e);
			resized = true;
			Console.Write("Editor resize event\n");

			Console.Write("Size ({0}, {1})\n",ClientSize.Height, ClientSize.Width);
			
		}
	}
}
