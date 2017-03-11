namespace Editor
{
	partial class CloseEditorMessageBox
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.label1 = new System.Windows.Forms.Label();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.Cancel = new System.Windows.Forms.Button();
			this.CloseBoth = new System.Windows.Forms.Button();
			this.CloseEditor = new System.Windows.Forms.Button();
			this.groupBox1.SuspendLayout();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label1.Location = new System.Drawing.Point(15, 18);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(231, 15);
			this.label1.TabIndex = 0;
			this.label1.Text = "Are you sure you want to close the editor?";
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.Cancel);
			this.groupBox1.Controls.Add(this.CloseBoth);
			this.groupBox1.Controls.Add(this.CloseEditor);
			this.groupBox1.Location = new System.Drawing.Point(12, 36);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(353, 63);
			this.groupBox1.TabIndex = 1;
			this.groupBox1.TabStop = false;
			// 
			// Cancel
			// 
			this.Cancel.Location = new System.Drawing.Point(236, 19);
			this.Cancel.Name = "Cancel";
			this.Cancel.Size = new System.Drawing.Size(109, 38);
			this.Cancel.TabIndex = 2;
			this.Cancel.Text = "Cancel";
			this.Cancel.UseVisualStyleBackColor = true;
			this.Cancel.Click += new System.EventHandler(this.Cancel_Click);
			// 
			// CloseBoth
			// 
			this.CloseBoth.Location = new System.Drawing.Point(121, 19);
			this.CloseBoth.Name = "CloseBoth";
			this.CloseBoth.Size = new System.Drawing.Size(109, 38);
			this.CloseBoth.TabIndex = 1;
			this.CloseBoth.Text = "Close Game And Editor";
			this.CloseBoth.UseVisualStyleBackColor = true;
			this.CloseBoth.Click += new System.EventHandler(this.CloseBoth_Click);
			// 
			// CloseEditor
			// 
			this.CloseEditor.Location = new System.Drawing.Point(6, 19);
			this.CloseEditor.Name = "CloseEditor";
			this.CloseEditor.Size = new System.Drawing.Size(109, 38);
			this.CloseEditor.TabIndex = 0;
			this.CloseEditor.Text = "Close Editor";
			this.CloseEditor.UseVisualStyleBackColor = true;
			this.CloseEditor.Click += new System.EventHandler(this.CloseEditor_Click);
			// 
			// CloseEditorMessageBox
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.AutoSize = true;
			this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
			this.ClientSize = new System.Drawing.Size(374, 110);
			this.Controls.Add(this.groupBox1);
			this.Controls.Add(this.label1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "CloseEditorMessageBox";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Close Editor?";
			this.groupBox1.ResumeLayout(false);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.Button Cancel;
		private System.Windows.Forms.Button CloseBoth;
		private System.Windows.Forms.Button CloseEditor;
	}
}