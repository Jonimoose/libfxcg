using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;

namespace CalcTyper
{
    public class ImageHandler
    {
        private string _bitmapPath;
        private Bitmap _currentBitmap;
        private Bitmap _bitmapbeforeProcessing;

        public ImageHandler()
        {
        }

        public Bitmap CurrentBitmap
        {
            get
            {
                if (_currentBitmap == null)
                    _currentBitmap = new Bitmap(1, 1);
                return _currentBitmap;
            }
            set { _currentBitmap = value; }
        }

        public Bitmap BitmapBeforeProcessing
        {
            get { return _bitmapbeforeProcessing; }
            set { _bitmapbeforeProcessing = value; }
        }

        public string BitmapPath
        {
            get { return _bitmapPath; }
            set { _bitmapPath = value; }
        }
        
        public void SaveBitmap(string saveFilePath)
        {
            _bitmapPath = saveFilePath;
            if (System.IO.File.Exists(saveFilePath))
                System.IO.File.Delete(saveFilePath);

            if (saveFilePath.ToLower().EndsWith("bmp"))
            {
                _currentBitmap.Save(saveFilePath, System.Drawing.Imaging.ImageFormat.Bmp);
            } else {
                _currentBitmap.Save(saveFilePath, System.Drawing.Imaging.ImageFormat.Png);
            }
        }

        private double luminance(Color forColor)
        {
            return (forColor.R * 0.2126 + forColor.G * 0.7152 + forColor.B * 0.0722) / 255.0;
        }

        private static int clamp(int value, int min, int max)
        {
            return (value < min) ? min : (value > max) ? max : value;
        }

        public Bitmap SubPixel(Bitmap onBitmap, bool isVertical, bool isBGR, bool isText)
        {
            float maxBias = isText ? 0.4f : 0.25f;

            Bitmap temp = (Bitmap)onBitmap;
            if (isVertical) {
                temp.RotateFlip(RotateFlipType.Rotate90FlipNone);
            }

            int newWidth = temp.Width / 3;
            int newHeight = temp.Height / 3;
            int maxWidth = temp.Width - 1;
            int maxHeight = temp.Height - 1;

            Bitmap bmap = new Bitmap(newWidth, newHeight, temp.PixelFormat);
            
            Color[,] colorMat = new Color[7,3];
            double[] lum = new double[7];

            for (int x = 0; x < bmap.Width; ++x)
            {
                for (int y = 0; y < bmap.Height; ++y)
                {
                    int srcX = x * 3;
                    int srcY = y * 3;

                    // determine local energy color matrix 
                    for (int sx = -2; sx < 5; sx++)
                    {
                        for (int sy = 0; sy < 3; sy++)
                        {
                            colorMat[sx+2, sy] = temp.GetPixel(clamp(srcX + sx, 0, maxWidth), clamp(srcY + sy, 0, maxHeight));
                        }
                    }

                    // average color for current pixel
                    double maxRed = 0, maxGreen = 0, maxBlue = 0;
                    double allAvgRed = 0, allAvgGreen = 0, allAvgBlue = 0;

                    for (int sy = 0; sy < 3; sy++)
                    {
                        double avgRed = 0;
                        double avgGreen = 0;
                        double avgBlue = 0;
                        for (int sx = 0; sx < 3; sx++)
                        {
                            Color curColor = colorMat[sx + 2, sy];
                            avgRed += curColor.R;
                            avgGreen += curColor.G;
                            avgBlue += curColor.B;
                        }
                        avgRed = avgRed / 3.0;
                        avgGreen = avgGreen / 3.0;
                        avgBlue = avgBlue / 3.0;

                        allAvgRed += avgRed / 3.0f;
                        allAvgGreen += avgGreen / 3.0f;
                        allAvgBlue += avgBlue / 3.0f;

                        maxRed = Math.Max(maxRed, avgRed);
                        maxGreen = Math.Max(maxGreen, avgGreen);
                        maxBlue = Math.Max(maxBlue, avgBlue);
                    }

                    double red = (maxRed * maxBias + allAvgRed * (1.0 - maxBias)) + (0.5 / 255);
                    double green = (maxGreen * maxBias + allAvgGreen * (1.0 - maxBias)) + (0.5 / 255);
                    double blue = (maxBlue * maxBias + allAvgBlue * (1.0 - maxBias)) + (0.5 / 255);
                
                    // determine the luminance of each local energy column
                    for (int c = 0; c < 7; c++)
                    {
                        lum[c] =
                            Math.Max(Math.Max(luminance(colorMat[c,0]), luminance(colorMat[c,1])), luminance(colorMat[c,2])) * maxBias +
                            (luminance(colorMat[c, 0]) + luminance(colorMat[c, 1]) + luminance(colorMat[c, 2])) / 3.0f * (1.0 - maxBias);
                    }
                    double avgLum = (lum[2] + lum[3] + lum[4]) / 3.0;

                    double[] columnWeight = { 1.0, 2.0, 3.0, 2.0, 1.0 };
                    double divisor = 0.0f;
                    for (int i = 0; i < 5; i++)
                    {
                        divisor += columnWeight[i];
                    }
                    divisor *= avgLum;

                    if (!isBGR)
                    {
                        red *=   (lum[0] * columnWeight[0] + lum[1] * columnWeight[1] + lum[2] * columnWeight[2] + lum[3] * columnWeight[3] + lum[4] * columnWeight[4]) / divisor;
                        green *= (lum[1] * columnWeight[0] + lum[2] * columnWeight[1] + lum[3] * columnWeight[2] + lum[4] * columnWeight[3] + lum[5] * columnWeight[4]) / divisor;
                        blue *=  (lum[2] * columnWeight[0] + lum[3] * columnWeight[1] + lum[4] * columnWeight[2] + lum[5] * columnWeight[3] + lum[6] * columnWeight[4]) / divisor;
                    }
                    else
                    {
                        red *=   (lum[2] * columnWeight[0] + lum[3] * columnWeight[1] + lum[4] * columnWeight[2] + lum[5] * columnWeight[3] + lum[6] * columnWeight[4]) / divisor;
                        green *= (lum[1] * columnWeight[0] + lum[2] * columnWeight[1] + lum[3] * columnWeight[2] + lum[4] * columnWeight[3] + lum[5] * columnWeight[4]) / divisor;
                        blue *=  (lum[0] * columnWeight[0] + lum[1] * columnWeight[1] + lum[2] * columnWeight[2] + lum[3] * columnWeight[3] + lum[4] * columnWeight[4]) / divisor;
                    }

                    Color newColor = System.Drawing.Color.FromArgb(255, (byte)red, (byte)green, (byte)blue);

                    bmap.SetPixel(x, y, newColor);
                }
            }

            if (isVertical)
            {
                bmap.RotateFlip(RotateFlipType.Rotate270FlipNone);
            }

            return (Bitmap)bmap.Clone();
        }
    }
}
