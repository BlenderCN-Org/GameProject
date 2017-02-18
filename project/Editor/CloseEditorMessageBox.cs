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
	public partial class CloseEditorMessageBox : Form
	{
		public CloseEditorMessageBox()
		{
			InitializeComponent();
		}

		private void CloseEditor_Click(object sender, EventArgs e)
		{
			this.DialogResult = DialogResult.Yes;
		}

		private void CloseBoth_Click(object sender, EventArgs e)
		{
			this.DialogResult = DialogResult.No;
		}

		private void Cancel_Click(object sender, EventArgs e)
		{
			this.DialogResult = DialogResult.Cancel;
		}
	}
}
