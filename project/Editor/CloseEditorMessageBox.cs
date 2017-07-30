using System;
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