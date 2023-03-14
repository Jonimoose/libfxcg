namespace CalcTyper
{
    partial class CalcTyper
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
            this.components = new System.ComponentModel.Container();
            this.MainMenu1 = new System.Windows.Forms.MainMenu(this.components);
            this.menuItemFile = new System.Windows.Forms.MenuItem();
            this.menuItemOpen = new System.Windows.Forms.MenuItem();
            this.menuItemSave = new System.Windows.Forms.MenuItem();
            this.menuItemSep1 = new System.Windows.Forms.MenuItem();
            this.menuItemExit = new System.Windows.Forms.MenuItem();
            this.menuItemSubPixel = new System.Windows.Forms.MenuItem();
            this.menuItemRGBH = new System.Windows.Forms.MenuItem();
            this.menuItemRGBV = new System.Windows.Forms.MenuItem();
            this.menuItemView = new System.Windows.Forms.MenuItem();
            this.menuItemImageInfo = new System.Windows.Forms.MenuItem();
            this.menuItemSep2 = new System.Windows.Forms.MenuItem();
            this.menuItemZoom = new System.Windows.Forms.MenuItem();
            this.menuItemZoom50 = new System.Windows.Forms.MenuItem();
            this.menuItemZoom100 = new System.Windows.Forms.MenuItem();
            this.menuItemZoom150 = new System.Windows.Forms.MenuItem();
            this.menuItemZoom200 = new System.Windows.Forms.MenuItem();
            this.menuItemZoom300 = new System.Windows.Forms.MenuItem();
            this.menuItemZoom400 = new System.Windows.Forms.MenuItem();
            this.menuItemZoom500 = new System.Windows.Forms.MenuItem();
            this.menuItem1 = new System.Windows.Forms.MenuItem();
            this.menuItemFont = new System.Windows.Forms.MenuItem();
            this.SuspendLayout();
            // 
            // MainMenu1
            // 
            this.MainMenu1.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.menuItemFile,
            this.menuItem1,
            this.menuItemSubPixel,
            this.menuItemView});
            // 
            // menuItemFile
            // 
            this.menuItemFile.Index = 0;
            this.menuItemFile.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.menuItemOpen,
            this.menuItemSave,
            this.menuItemSep1,
            this.menuItemExit});
            this.menuItemFile.Text = "&File";
            // 
            // menuItemOpen
            // 
            this.menuItemOpen.Index = 0;
            this.menuItemOpen.Shortcut = System.Windows.Forms.Shortcut.CtrlO;
            this.menuItemOpen.Text = "&Open";
            this.menuItemOpen.Click += new System.EventHandler(this.menuItemOpen_Click);
            // 
            // menuItemSave
            // 
            this.menuItemSave.Index = 1;
            this.menuItemSave.Shortcut = System.Windows.Forms.Shortcut.CtrlS;
            this.menuItemSave.Text = "&Save";
            this.menuItemSave.Click += new System.EventHandler(this.menuItemSave_Click);
            // 
            // menuItemSep1
            // 
            this.menuItemSep1.Index = 2;
            this.menuItemSep1.Text = "-";
            // 
            // menuItemExit
            // 
            this.menuItemExit.Index = 3;
            this.menuItemExit.Text = "E&xit";
            this.menuItemExit.Click += new System.EventHandler(this.menuItemExit_Click);
            // 
            // menuItemSubPixel
            // 
            this.menuItemSubPixel.Index = 2;
            this.menuItemSubPixel.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.menuItemRGBH,
            this.menuItemRGBV});
            this.menuItemSubPixel.Text = "Sub-Pixel";
            // 
            // menuItemRGBH
            // 
            this.menuItemRGBH.Index = 0;
            this.menuItemRGBH.Text = "RGB Horiz (PC/Prizm)";
            this.menuItemRGBH.Click += new System.EventHandler(this.menuItemRGBH_Click);
            // 
            // menuItemRGBV
            // 
            this.menuItemRGBV.Index = 1;
            this.menuItemRGBV.Text = "RGB Vert (84+ CE)";
            this.menuItemRGBV.Click += new System.EventHandler(this.menuItemRGBV_Click);
            // 
            // menuItemView
            // 
            this.menuItemView.Index = 3;
            this.menuItemView.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.menuItemImageInfo,
            this.menuItemSep2,
            this.menuItemZoom});
            this.menuItemView.Text = "&View";
            // 
            // menuItemImageInfo
            // 
            this.menuItemImageInfo.Enabled = false;
            this.menuItemImageInfo.Index = 0;
            this.menuItemImageInfo.Shortcut = System.Windows.Forms.Shortcut.CtrlI;
            this.menuItemImageInfo.Text = "I&mage Info.";
            // 
            // menuItemSep2
            // 
            this.menuItemSep2.Index = 1;
            this.menuItemSep2.Text = "-";
            // 
            // menuItemZoom
            // 
            this.menuItemZoom.Index = 2;
            this.menuItemZoom.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.menuItemZoom50,
            this.menuItemZoom100,
            this.menuItemZoom150,
            this.menuItemZoom200,
            this.menuItemZoom300,
            this.menuItemZoom400,
            this.menuItemZoom500});
            this.menuItemZoom.Text = "&Zoom";
            // 
            // menuItemZoom50
            // 
            this.menuItemZoom50.Index = 0;
            this.menuItemZoom50.Text = "50%";
            this.menuItemZoom50.Click += new System.EventHandler(this.menuItemZoom50_Click);
            // 
            // menuItemZoom100
            // 
            this.menuItemZoom100.Checked = true;
            this.menuItemZoom100.Index = 1;
            this.menuItemZoom100.Text = "100%";
            this.menuItemZoom100.Click += new System.EventHandler(this.menuItemZoom100_Click);
            // 
            // menuItemZoom150
            // 
            this.menuItemZoom150.Index = 2;
            this.menuItemZoom150.Text = "150%";
            this.menuItemZoom150.Click += new System.EventHandler(this.menuItemZoom150_Click);
            // 
            // menuItemZoom200
            // 
            this.menuItemZoom200.Index = 3;
            this.menuItemZoom200.Text = "200%";
            this.menuItemZoom200.Click += new System.EventHandler(this.menuItemZoom200_Click);
            // 
            // menuItemZoom300
            // 
            this.menuItemZoom300.Index = 4;
            this.menuItemZoom300.Text = "300%";
            this.menuItemZoom300.Click += new System.EventHandler(this.menuItemZoom300_Click);
            // 
            // menuItemZoom400
            // 
            this.menuItemZoom400.Index = 5;
            this.menuItemZoom400.Text = "400%";
            this.menuItemZoom400.Click += new System.EventHandler(this.menuItemZoom400_Click);
            // 
            // menuItemZoom500
            // 
            this.menuItemZoom500.Index = 6;
            this.menuItemZoom500.Text = "500%";
            this.menuItemZoom500.Click += new System.EventHandler(this.menuItemZoom500_Click);
            // 
            // menuItem1
            // 
            this.menuItem1.Index = 1;
            this.menuItem1.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.menuItemFont});
            this.menuItem1.Text = "Font";
            // 
            // menuItemFont
            // 
            this.menuItemFont.Index = 0;
            this.menuItemFont.Text = "Process BMFont Output...";
            this.menuItemFont.Click += new System.EventHandler(this.menuItemFont_Click);
            // 
            // CalcTyper
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Inherit;
            this.BackColor = System.Drawing.SystemColors.AppWorkspace;
            this.ClientSize = new System.Drawing.Size(624, 420);
            this.Menu = this.MainMenu1;
            this.Name = "CalcTyper";
            this.Text = "CalcTyper Tool";
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.ImageProcessing_Paint);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.MainMenu MainMenu1;
        private System.Windows.Forms.MenuItem menuItemFile;
        private System.Windows.Forms.MenuItem menuItemOpen;
        private System.Windows.Forms.MenuItem menuItemSave;
        private System.Windows.Forms.MenuItem menuItemSep1;
        private System.Windows.Forms.MenuItem menuItemExit;
        private System.Windows.Forms.MenuItem menuItemView;
        private System.Windows.Forms.MenuItem menuItemImageInfo;
        private System.Windows.Forms.MenuItem menuItemSep2;
        private System.Windows.Forms.MenuItem menuItemZoom;
        private System.Windows.Forms.MenuItem menuItemZoom50;
        private System.Windows.Forms.MenuItem menuItemZoom100;
        private System.Windows.Forms.MenuItem menuItemZoom150;
        private System.Windows.Forms.MenuItem menuItemZoom200;
        private System.Windows.Forms.MenuItem menuItemZoom300;
        private System.Windows.Forms.MenuItem menuItemZoom400;
        private System.Windows.Forms.MenuItem menuItemZoom500;
        private System.Windows.Forms.MenuItem menuItemSubPixel;
        private System.Windows.Forms.MenuItem menuItemRGBH;
        private System.Windows.Forms.MenuItem menuItemRGBV;
        private System.Windows.Forms.MenuItem menuItem1;
        private System.Windows.Forms.MenuItem menuItemFont;
    }
}

