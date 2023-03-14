# prizm-winsim

A full Windows simulator of the Prizm API and syscalls for improving development iteration times.

# Basic directions

Use within tswilliamson's full SDK for easiest use

## Features

### Print functions

- Added [PrintMiniMini](http://prizm.cemetech.net/index.php/PrintMiniMini), [Bdisp_MMPrint](http://prizm.cemetech.net/index.php/Bdisp_MMPrint), and [Bdisp_MMPrintRef](http://prizm.cemetech.net/index.php/Bdisp_MMPrintRef) functions.
  - Bdisp_MMPrint* functions are not included in PrizmSDK v0.3. To use it, you need to compile libfxcg with [gbl08ma's commit](https://github.com/gbl08ma/libfxcg/commit/2a2776f2440579c2fe9ef94c0470108e2af4e058).
- All print functions except PrintCXY use fonts actually used by the calculator.
  - PrintCXY uses fixed-width font, so winsim is still useful for measuring dimensions.
- Properly implements mode_flags in PrintMini, PrintMiniMini, and Bdisp_MMPrint* functions

### Message Box (Popup)

Only [MsgBoxPush](http://prizm.cemetech.net/index.php/MsgBoxPush) & [MsgBoxPop](http://prizm.cemetech.net/index.php/MsgBoxPop) are supported for now.

### Warnings

Prizm calculator can easily crash when something goes wrong. I put warnings when some behavior causes crash. All of them are tested on fx-cg50.

- When you use [Non-blocking_DMA](http://prizm.cemetech.net/index.php/Non-blocking_DMA), you must call DmaWaitNext() before GetKey() or other DoDMAlcdNonblock().
  - Actually making this work needs some tricks, because winsim does not implement Non-blocking_DMA functions on its own. 
  ```
  #if TARGET_WINSIM
  extern bool dma_transfer;
  void DmaWaitNext(void)
  {
      dma_transfer = false;
  }
  void DoDMAlcdNonblockStrip(unsigned y1, unsigned y2)
  {
      if (dma_transfer)
          printf("Warning: You should call DmaWaitNext() before another DoDMAlcdNonblockStrip().\n");
      dma_transfer = true;
      Bdisp_PutDisp_DD();
  }
  ```
- You must call MsgBoxPush() before MsgBoxPop(). Also MsgBoxPop() before another MsgBoxPush().
