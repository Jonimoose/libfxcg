# calctype
Typeface and imaging tools for color calculators using sub-pixel rendering concepts

# Font Generation

First generate a font using BMFont.exe. It has a ton of options for laying out glyphs. You'll want to fit your glyphs in a 256x256 image to construct a 10 kb sized font on the calculator, though you can make them as big as you want based on your needs. Save the font out to its own folder in the calctype/fonts folder OR in your project folder. 

Run the calctype tool (contained in Tool/bin/Release/CalcTyper.exe), and select Font->Process BMFont Output... and point it to your image output by BMFont. The program should load up your font image if successful, and automatically create a .c/.h for inclusion in your project. Be aware that you will have to fix the pathing in the .h file if you intend on using the font in your project folder.

# Sub-pixel image generation

CalcTyper works by converting each 3x3 pixel block of an image to a 1x1 pixel, but with the adjancency information and luminosity of the block baked into the three RGB subpixels commonly found on an LCD display. 

You can utilize this to create "sharper than per pixel" images in addition to fonts. To do this, select File->Open and select an image, then select Sub-Pixel->RGB Horiz to generate an optimized image. Remember that your source image should be 3x3 the dimensions of the final output. The final output can then be saved with File->Save.