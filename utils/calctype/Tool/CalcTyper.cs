/*
 * Based heavily on the ImageProcessing CodeProject tutorial: 
 * https://www.codeproject.com/Articles/33838/Image-Processing-using-C
 * 
 */
 
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Diagnostics;
using System.Windows.Forms;

namespace CalcTyper
{
    public partial class CalcTyper : Form
    {
        OpenFileDialog oDlg;
        OpenFileDialog fontDlg;
        SaveFileDialog sDlg;
        double zoomFactor = 1.0;
        private MenuItem cZoom;
        ImageHandler imageHandler = new ImageHandler();

        public CalcTyper()
        {
            InitializeComponent();
            oDlg = new OpenFileDialog(); // Open Dialog Initialization
            oDlg.RestoreDirectory = true;
            oDlg.FilterIndex = 1;
            oDlg.Filter = "Image Files (*.jpg,*.gif,*.png,*.bmp)|*.jpg;*.gif;*.png;*.bmp";
            /*************************/
            sDlg = new SaveFileDialog(); // Save Dialog Initialization
            sDlg.RestoreDirectory = true;
            sDlg.FilterIndex = 1;
            sDlg.Filter = "Image Files (*.png,*.bmp)|*.png;*.bmp";
            /*************************/
            fontDlg = new OpenFileDialog(); // Open Dialog Initialization
            fontDlg.RestoreDirectory = true;
            fontDlg.FilterIndex = 1;
            fontDlg.Filter = "BMFont Output Files (*.fnt)|*.fnt";
            /*************************/
            cZoom = menuItemZoom100; // Current Zoom Percentage = 100%
        }

        private void ImageProcessing_Paint(object sender, PaintEventArgs e)
        {
            Graphics g = e.Graphics;
            g.DrawImage(imageHandler.CurrentBitmap, new Rectangle(this.AutoScrollPosition.X, this.AutoScrollPosition.Y, Convert.ToInt32(imageHandler.CurrentBitmap.Width * zoomFactor), Convert.ToInt32(imageHandler.CurrentBitmap.Height * zoomFactor)));
        }

        private void menuItemOpen_Click(object sender, EventArgs e)
        {
            if (DialogResult.OK == oDlg.ShowDialog())
            {
                imageHandler.CurrentBitmap = (Bitmap)Bitmap.FromFile(oDlg.FileName);
                imageHandler.BitmapPath = oDlg.FileName;
                this.AutoScroll = true;
                this.AutoScrollMinSize = new Size(Convert.ToInt32(imageHandler.CurrentBitmap.Width * zoomFactor), Convert.ToInt32(imageHandler.CurrentBitmap.Height * zoomFactor));
                this.Invalidate();
                //ImageInfo imgInfo = new ImageInfo(imageHandler);
                //imgInfo.Show();
            }
        }

        private void menuItemSave_Click(object sender, EventArgs e)
        {
            if (DialogResult.OK == sDlg.ShowDialog())
            {
                imageHandler.SaveBitmap(sDlg.FileName);
            }
        }

        private void menuItemExit_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void menuItemZoom50_Click(object sender, EventArgs e)
        {
            zoomFactor = 0.5;
            cZoom.Checked = false;
            menuItemZoom50.Checked = true;
            cZoom = menuItemZoom50;
            this.AutoScrollMinSize = new Size(Convert.ToInt32(imageHandler.CurrentBitmap.Width * zoomFactor), Convert.ToInt32(imageHandler.CurrentBitmap.Height * zoomFactor));
            this.Invalidate();
        }

        private void menuItemZoom100_Click(object sender, EventArgs e)
        {
            zoomFactor = 1.0;
            cZoom.Checked = false;
            menuItemZoom100.Checked = true;
            cZoom = menuItemZoom100;
            this.AutoScrollMinSize = new Size(Convert.ToInt32(imageHandler.CurrentBitmap.Width * zoomFactor), Convert.ToInt32(imageHandler.CurrentBitmap.Height * zoomFactor));
            this.Invalidate();
        }

        private void menuItemZoom150_Click(object sender, EventArgs e)
        {
            zoomFactor = 1.5;
            cZoom.Checked = false;
            menuItemZoom150.Checked = true;
            cZoom = menuItemZoom150;
            this.AutoScrollMinSize = new Size(Convert.ToInt32(imageHandler.CurrentBitmap.Width * zoomFactor), Convert.ToInt32(imageHandler.CurrentBitmap.Height * zoomFactor));
            this.Invalidate();
        }

        private void menuItemZoom200_Click(object sender, EventArgs e)
        {
            zoomFactor = 2.0;
            cZoom.Checked = false;
            menuItemZoom200.Checked = true;
            cZoom = menuItemZoom200;
            this.AutoScrollMinSize = new Size(Convert.ToInt32(imageHandler.CurrentBitmap.Width * zoomFactor), Convert.ToInt32(imageHandler.CurrentBitmap.Height * zoomFactor));
            this.Invalidate();

        }

        private void menuItemZoom300_Click(object sender, EventArgs e)
        {
            zoomFactor = 3.0;
            cZoom.Checked = false;
            menuItemZoom300.Checked = true;
            cZoom = menuItemZoom300;
            this.AutoScrollMinSize = new Size(Convert.ToInt32(imageHandler.CurrentBitmap.Width * zoomFactor), Convert.ToInt32(imageHandler.CurrentBitmap.Height * zoomFactor));
            this.Invalidate();

        }

        private void menuItemZoom400_Click(object sender, EventArgs e)
        {
            zoomFactor = 4.0;
            cZoom.Checked = false;
            menuItemZoom400.Checked = true;
            cZoom = menuItemZoom400;
            this.AutoScrollMinSize = new Size(Convert.ToInt32(imageHandler.CurrentBitmap.Width * zoomFactor), Convert.ToInt32(imageHandler.CurrentBitmap.Height * zoomFactor));
            this.Invalidate();
        }

        private void menuItemZoom500_Click(object sender, EventArgs e)
        {
            zoomFactor = 5.0;
            cZoom.Checked = false;
            menuItemZoom500.Checked = true;
            cZoom = menuItemZoom500;
            this.AutoScrollMinSize = new Size(Convert.ToInt32(imageHandler.CurrentBitmap.Width * zoomFactor), Convert.ToInt32(imageHandler.CurrentBitmap.Height * zoomFactor));
            this.Invalidate();
        }

        private void menuItemRGBH_Click(object sender, EventArgs e)
        {
            imageHandler.CurrentBitmap = imageHandler.SubPixel(imageHandler.CurrentBitmap, false, false, false);
            this.AutoScroll = true;
            this.AutoScrollMinSize = new Size(Convert.ToInt32(imageHandler.CurrentBitmap.Width * zoomFactor), Convert.ToInt32(imageHandler.CurrentBitmap.Height * zoomFactor));
            this.Invalidate();
        }

        private void menuItemRGBV_Click(object sender, EventArgs e)
        {
            imageHandler.CurrentBitmap = imageHandler.SubPixel(imageHandler.CurrentBitmap, true, false, false);
            this.AutoScroll = true;
            this.AutoScrollMinSize = new Size(Convert.ToInt32(imageHandler.CurrentBitmap.Width * zoomFactor), Convert.ToInt32(imageHandler.CurrentBitmap.Height * zoomFactor));
            this.Invalidate();
        }

        public struct CharInfo
        {
            public int id;
            public int x;
            public int y;
            public int width;
            public int height;
            public int xoffset;
            public int yoffset;
            public int xadvance;
            public int offset;
            public List<byte> data;
        }

        private CharInfo buildCharData(CharInfo glyph) {
            // pad to the left by 1 pixel
            if (glyph.x > 0) { 
                glyph.width += 1;
                glyph.xoffset -= 1;
                glyph.x -= 1;
            }

            // width must be divisible by 3:
            if (glyph.width % 3 != 0) {
                glyph.width += 3 - glyph.width % 3;

                int imageW = imageHandler.CurrentBitmap.Width;
                while (glyph.x + glyph.width > imageW)
                {
                    glyph.x--;
                    glyph.xoffset -= 1;
                }
            }

            // height must be divisible by 3:
            if (glyph.height % 3 != 0)
            {
                glyph.yoffset -= 3 - glyph.height % 3;
                glyph.height += 3 - glyph.height % 3;
            }

            int srcBottom = glyph.yoffset + glyph.height;


            Rectangle cropArea = new Rectangle(glyph.x, glyph.y, glyph.width, glyph.height);

            Bitmap subbitmap = imageHandler.CurrentBitmap.Clone(cropArea, imageHandler.CurrentBitmap.PixelFormat);
            subbitmap = imageHandler.SubPixel(subbitmap, false, false, true);


            // glyphs are packed 3:3:2
            glyph.data = new List<byte>();
            for (int y = 0; y < subbitmap.Height; y++)
            {
                for (int x = 0; x < subbitmap.Width; x++)
                {
                    int packed = 0;
                    Color c = subbitmap.GetPixel(x, y);
                    packed = (c.R & 0xE0) | ((c.G & 0xE0) >> 3) | ((c.B & 0xC0) >> 6);
                    glyph.data.Add((byte) packed);
                }
            }

            glyph.width = subbitmap.Width;
            glyph.height = subbitmap.Height;

            // trim empty rows from glyph

            // top row
            bool foundEmpty = true;
            while (foundEmpty && glyph.data.Count != 0)
            {
                for (int i = 0; i < glyph.width; i++) {
                    if (glyph.data[i] != 0)
                    {
                        foundEmpty = false;
                        break;
                    }
                }
                if (foundEmpty)
                {
                    glyph.data.RemoveRange(0, glyph.width);
                    glyph.height--;
                    glyph.yoffset += 3;
                }
            }

            // bottom row
            foundEmpty = true;
            while (foundEmpty && glyph.data.Count != 0)
            {
                for (int i = 0; i < glyph.width; i++)
                {
                    if (glyph.data[glyph.data.Count - i - 1] != 0)
                    {
                        foundEmpty = false;
                        break;
                    }
                }
                if (foundEmpty)
                {
                    glyph.data.RemoveRange(glyph.data.Count - glyph.width, glyph.width);
                    glyph.height--;
                }
            }

            // left side
            foundEmpty = true;
            while (foundEmpty && glyph.data.Count != 0)
            {
                for (int i = 0; i < glyph.height; i++)
                {
                    if (glyph.data[glyph.width * i] != 0)
                    {
                        foundEmpty = false;
                        break;
                    }
                }
                if (foundEmpty)
                {
                    for (int i = 0; i < glyph.height; i++)
                    {
                        glyph.data.RemoveAt(i * (glyph.width - 1));
                    }
                    glyph.width--;
                    glyph.xoffset += 3;
                }
            }

            // right side
            foundEmpty = true;
            while (foundEmpty && glyph.data.Count != 0)
            {
                for (int i = 1; i <= glyph.height; i++)
                {
                    if (glyph.data[glyph.width * i - 1] != 0)
                    {
                        foundEmpty = false;
                        break;
                    }
                }
                if (foundEmpty)
                {
                    for (int i = 0; i < glyph.height; i++)
                    {
                        glyph.data.RemoveAt(i * (glyph.width - 1) + glyph.width - 1);
                    }
                    glyph.width--;
                }
            }
            
            return glyph;
        }

        private void menuItemFont_Click(object sender, EventArgs e)
        {
            if (DialogResult.OK == fontDlg.ShowDialog())
            {
                string fontFile = fontDlg.FileName;
                Trace.WriteLine("Opened " + fontFile);
                string line;

                string dir = System.IO.Path.GetDirectoryName(fontFile);
                string fileBase = System.IO.Path.GetFileNameWithoutExtension(fontFile);
                char[] delims = { ' ', '\t', '=' };

                // main font characteristics
                int lineHeight = 0;
                int lineBase = 0;
                int spaceWidth = 0;
                bool[] usedChars = new bool[256];
                for (int i = 0; i < 256; i++)
                {
                    usedChars[i] = false;
                }

                List<CharInfo> chars = new List<CharInfo>();
                
                System.IO.StreamReader file = new System.IO.StreamReader(fontFile);
                while ((line = file.ReadLine()) != null)
                {
                    Trace.WriteLine("Processing " + line);
                    if (line.StartsWith("info") || line.StartsWith("chars"))
                    {
                        // just information, we ignore it
                    }
                    else if (line.StartsWith("common"))
                    {
                        // common info for the font
                        string[] args = line.Substring(7).Split(delims);
                        for (int i = 0; i < args.Length; i++)
                        {
                            if (args[i] == "lineHeight")
                            {
                                lineHeight = Convert.ToInt32(args[i + 1]);
                            }
                            else if (args[i] == "base")
                            {
                                lineBase = Convert.ToInt32(args[i + 1]);
                            }
                            else if (args[i] == "pages")
                            {
                                int numPages = Convert.ToInt32(args[i + 1]);
                                if (numPages != 1)
                                {
                                    MessageBox.Show("Invalid number of pages (" + args[i + 1] + "). Only 1 page supported.");
                                    return;
                                }
                            }
                        }
                    }
                    else if (line.StartsWith("page"))
                    {
                        // page info for the font (only supporting one page at the moment)
                        string[] args = line.Substring(5).Split(delims);
                        for (int i = 0; i < args.Length; i++)
                        {
                            if (args[i] == "file")
                            {
                                string pageFile = args[i + 1].Trim('\"');
                                string imagePath = dir + "\\" + pageFile;

                                // load page as current bitmap
                                imageHandler.CurrentBitmap = (Bitmap)Bitmap.FromFile(imagePath);
                                imageHandler.BitmapPath = imagePath;
                                this.AutoScroll = true;
                                this.AutoScrollMinSize = new Size(Convert.ToInt32(imageHandler.CurrentBitmap.Width * zoomFactor), Convert.ToInt32(imageHandler.CurrentBitmap.Height * zoomFactor));
                                this.Invalidate();
                            }
                        }
                    }
                    else if (line.StartsWith("char"))
                    {
                        // character info, the real meat of it
                        CharInfo info = new CharInfo();

                        string[] args = line.Substring(5).Split(delims);
                        for (int i = 0; i < args.Length; i++)
                        {
                            if (args[i] == "id")
                            {
                                info.id = Convert.ToInt32(args[i + 1]);
                            }
                            else if (args[i] == "x")
                            {
                                info.x = Convert.ToInt32(args[i + 1]);
                            }
                            else if (args[i] == "y")
                            {
                                info.y = Convert.ToInt32(args[i + 1]);
                            }
                            else if (args[i] == "width")
                            {
                                info.width = Convert.ToInt32(args[i + 1]);
                            }
                            else if (args[i] == "height")
                            {
                                info.height = Convert.ToInt32(args[i + 1]);
                            }
                            else if (args[i] == "xoffset")
                            {
                                info.xoffset = Convert.ToInt32(args[i + 1]);
                            }
                            else if (args[i] == "yoffset")
                            {
                                info.yoffset = Convert.ToInt32(args[i + 1]);
                            }
                            else if (args[i] == "xadvance")
                            {
                                info.xadvance = Convert.ToInt32(args[i + 1]);
                            }
                        }

                        // special case for space
                        if (info.id == 32)
                        {
                            spaceWidth = info.xadvance;
                            continue;
                        }

                        // ignore chars outside of 0-255
                        if (info.id >= 0 && info.id <= 255 && !usedChars[info.id])
                        {
                            // enable bit
                            usedChars[info.id] = true;
                            chars.Add(buildCharData(info));
                        }
                    }
                }
                file.Close();

                if (chars.Count > 0)
                {
                    // check to see if the font is monospaced (all characters will have same xadvance)
                    int checkAdvance = chars[0].xadvance;
                    bool isMonospaced = true;
                    for (int i = 1; i < chars.Count; i++)
                    {
                        if (chars[i].xadvance != checkAdvance)
                        {
                            isMonospaced = false;
                        }
                    }

                    // if the font is monospaced, all xadvance values should be on the pixel barrier
                    if (isMonospaced && checkAdvance % 3 != 0)
                    {
                        int toSub = checkAdvance % 3;
                        for (int i = 0; i < chars.Count; i++)
                        {
                            CharInfo c = chars[i];
                            c.xadvance -= toSub;
                            chars[i] = c;
                        }
                        spaceWidth -= toSub;
                    }

                    // create our output .c/.h files containing the pertinent data
                    string cFile = fontDlg.FileName.Substring(0, fontFile.Length - 3) + "c";
                    string hFile = fontDlg.FileName.Substring(0, fontFile.Length - 3) + "h";

                    string headerComment =
                        "/* Generated font file for CalcType (Prizm) */";
                  
                    System.IO.StreamWriter hWriter = new System.IO.StreamWriter(hFile);
                    hWriter.WriteLine(headerComment);
                    hWriter.WriteLine("#pragma once");
                    hWriter.WriteLine("");
                    hWriter.WriteLine("#include \"../../calctype.h\"");
                    hWriter.WriteLine("");
                    hWriter.WriteLine("extern const CalcTypeFont " + fileBase + ";");
                    hWriter.WriteLine("");
                    hWriter.Close();

                    System.IO.StreamWriter cWriter = new System.IO.StreamWriter(cFile);
                    cWriter.WriteLine(headerComment);
                    cWriter.WriteLine("#include \"" + fileBase + ".h\"");
                    cWriter.WriteLine("");
                    cWriter.WriteLine("const unsigned char __charData_" + fileBase + "[] = {");

                    // build line for each character data
                    int[] offsets = new int[256];
                    int curOffset = 0;
                    for (int i  = 33; i < 256; i++)
                    {
                        if (usedChars[i])
                        {
                            offsets[i] = curOffset;
                            int index = -1;
                            for (int j = 0; j < chars.Count; j++)
                            {
                                if (chars[j].id == i)
                                {
                                    index = j;
                                    break;
                                }
                            }

                            string charLine = "\t";
                            if (chars[index].xoffset < 0)
                            {
                                charLine += (256 + chars[index].xoffset) + ",";
                            } else
                            {
                                charLine += chars[index].xoffset + ",";
                            }
                            if (chars[index].yoffset < -2)
                            {
                                charLine += (256 + chars[index].yoffset / 3) + ",";
                            }
                            else
                            {
                                charLine += (chars[index].yoffset / 3) + ",";
                            }
                            charLine += chars[index].xadvance + ",";
                            charLine += chars[index].width + ",";
                            charLine += chars[index].height + ",";
                            for (int j = 0; j < chars[index].data.Count; j++) {
                                charLine += chars[index].data[j] + ",";
                            }

                            charLine += "  // " + i;
                            cWriter.WriteLine(charLine);
                            curOffset += 5 + chars[index].data.Count;
                        } else {
                            offsets[i] = 0xFFFF;
                        }
                    }

                    cWriter.WriteLine("};");
                    cWriter.WriteLine("");
                    cWriter.WriteLine("const CalcTypeFont " + fileBase + " = {");
                    cWriter.WriteLine("\t" + ((lineHeight + 1) / 3) + ",  // height");
                    cWriter.WriteLine("\t" + ((lineBase + 1) / 3) + ",  // base");
                    cWriter.WriteLine("\t" + spaceWidth + ",  // space");
                    cWriter.WriteLine("\t__charData_" + fileBase + ",  // char data");

                    // each character offset
                    cWriter.WriteLine("\t{");
                    for (int i = 32; i < 256; i++)
                    {
                        cWriter.WriteLine("\t\t" + offsets[i] + ",");
                    }
                    cWriter.WriteLine("\t}");
                    cWriter.WriteLine("};");
                    cWriter.Close();
                }
                else
                {
                    MessageBox.Show("Could not find any chars to add");
                }
            }
        }
    }
}