/*---------------------------------------------------------------------------------------------

    SH7305 CPG&BSC Register.

    for fx-9860GII-2, fx-CG10/20   v1.01
    
    copyright(c)2014,2015 by sentaro21
    e-mail sentaro21@pm.matrix.jp

-------------------------------------------------------------------------------
CPG(Clock Pulse Generator)                               quote: SH7724 datasheet
-------------------------------------------------------------------------------
    FLL: FLL circuit Multiplication Ratio.
         The FLL circuit multiples the clock frequency(32.768KHz) input from the RTC_CLK.
         default  multiplication rate is 900.
         FLL circuit output  32768Hz*900/2=14.7456MHz
         this is same as old SH3 model oscillatory frequency.

    PLL: PLL circuit Multiplication Ratio.
         base frequency 14.7456MHz at FLL output
         Multiplication Ratio is 1-64
         It becomes half speed when about over 800MHz.
         (PLL Clock is up to about 750MHz)

    IFC: CPU Clock Division Ratio.
    SFC: SHW Clock Division Ratio.
    BFC: bus Clock Division Ratio.
    PFC: I/O Clock Division Ratio.
        Division Ratio  (not same to  SH7724)
        0000: 1/2
        0001: 1/4   IFC dafault
        0010: 1/8   SFC,BFC default
        0011: 1/16  PFC default
        0100: 1/32
        0101: 1/64
    Note.
    IFC >= SFC >= BFC >= PFC
    IFC:SFC  only  1:1 1:2
    this tool adjust automatically.


-------------------------------------------------------------------------------
BSC(Bus State Controller)                                quote: SH7724 datasheet
-------------------------------------------------------------------------------
    register structure for the BCR/WCR similar to SH7724.

    CS0BCR, CS0WCR  : FLASH ROM area
    CS2BCR, CS2WCR  : main  RAM area
    CS5ABCR,CS5AWCR : LCDC
    others unknown.


    CSn Space Bus Contorl Register (CSnBCR) (n=0,2,3,4,5A,5B,6A,6B)

    IWW: Specification for Idol Cycles between Write-Read/Write-Write Cycles.
        000: no idol cycle
        001: 1 idol cycle inserted
        010: 2 idol cycles inserted
        011: 4 idol cycles inserted
        100: 6 idol cycles inserted
        101: 8 idol cycles inserted
        110: 10 idol cycles inserted
        111: 12idol cycles inserted

        (CS0BCR,CS2BCR)@SH7305 default value 2 idol cycles.
        lower frequency can modify more lower setteing.
        it work well effectively.

    IWRWD: Specification for Idol Cycles between Read-Write Cycles in Different Spaces.
        000: no idol cycle
        001: 1 idol cycle inserted
        010: 2 idol cycles inserted
        011: 4 idol cycles inserted
        100: 6 idol cycles inserted
        101: 8 idol cycles inserted
        110: 10 idol cycles inserted
        111: 12idol cycles inserted

        (CS0BCR,CS2BCR)@SH7305 default value 2 idol cycles.
        lower frequency can modify more lower setteing.
        it work well. but an effect is not felt.

    IWRWS: Specification for Idol Cycles between Read-Write Cycles in the Same Spaces.
        000: no idol cycle
        001: 1 idol cycle inserted
        010: 2 idol cycles inserted
        011: 4 idol cycles inserted
        100: 6 idol cycles inserted
        101: 8 idol cycles inserted
        110: 10 idol cycles inserted
        111: 12idol cycles inserted

        (CS0BCR,CS2BCR)@SH7305 default value 2 idol cycles.
        lower frequency can modify more lower setteing.
        it work well. but an effect is not felt.

    IWRRD: Specification for Idol Cycles between Read-Read Cycles in Different Spaces.
        000: no idol cycle
        001: 1 idol cycle inserted
        010: 2 idol cycles inserted
        011: 4 idol cycles inserted
        100: 6 idol cycles inserted
        101: 8 idol cycles inserted
        110: 10 idol cycles inserted
        111: 12idol cycles inserted

        (CS0BCR,CS2BCR)@SH7305 default value 2 idol cycles.
        lower frequency can modify more lower setteing.
        it work well. but an effect is not felt.

    IWRRS:  Specification for Idol Cycles between Read-Read Cycles in the Same Spaces.
        000: no idol cycle
        001: 1 idol cycle inserted
        010: 2 idol cycles inserted
        011: 4 idol cycles inserted
        100: 6 idol cycles inserted
        101: 8 idol cycles inserted
        110: 10 idol cycles inserted
        111: 12idol cycles inserted

        (CS0BCR,CS2BCR)@SH7305 default value 2 idol cycles.
        lower frequency can modify more lower setteing.
        it work well effectively.



    CSn Space Wait Contorl Register (CSnWCR) (n=0,2,3,4,5A,5B,6A,6B)

    WW: Number of Wait Cycles in Write Access
        000: The same cycles as WR settings
        001: 0 cycle
        010: 1 cycle
        011: 2 cycles
        100: 3 cycles
        101: 4 cycles
        110: 5 cycles
        111: 6 cycles

        (CS0WCR,CS2WCR)@SH7305 default value is the same cycles as WR settings.
        I think that it is not necessary to change it.
        but,CS5AWCR work well effectively. (When abnormality happens to LCD)

    WR: Number of Wait Cycles in Read Access
        0000: 0 cycle       1000: 10 cycles
        0001: 1 cycle       1001: 12 cycles
        0010: 2 cycle       1010: 14 cycles
        0011: 3 cycles      1011: 18 cycles
        0100: 4 cycles      1100: 24 cycles
        0101: 5 cycles
        0110: 6 cycles
        0111: 8 cycles

        CS0WCR@SH7305 default value is 3 cycles
        if increment it, and more bus frequency.

        CS2WCR@SH7305 default value is 2 cycles
        if increment it, and more bus frequency.


    SW: Numer of Delay Cycles from Address,CSn Assertion to RD,WE Assertion.
        00: 0.5cycle
        01: 1.5cycles
        10: 2.5cycles
        11: 3.5cycles
        (CS0WCR,CS2WCR)@SH7305 default value is 0.5cycle.


    HW: Delay Cycles from RD WEn Negation to Address,CSn Negation.
        00: 0.5cycle
        01: 1.5cycles
        10: 2.5cycles
        11: 3.5cycles
        (CS0WCR,CS2WCR)@SH7305 default value is 0.5cycle.

---------------------------------------------------------------------------------------------*/

struct st_cpg {
    union {                           // struct FRQCRA   similar to SH7724
        unsigned long LONG;           // long Word Access
        struct {                      // Bit  Access
            unsigned long KICK :1;    //    KICK      
            unsigned long      :1;    //              
            unsigned long STC  :6;    //    STC       
            unsigned long IFC  :4;    //    IFC       
            unsigned long      :4;    //              
            unsigned long SFC  :4;    //    SFC       
            unsigned long BFC  :4;    //    BFC       
            unsigned long      :4;    //              
            unsigned long PFC  :4;    //    PFC       
            }   BIT;
        }   FRQCRA;

    unsigned long FRQCRB;       // 0xA41500004
    unsigned long FCLKACR;      // 0xA41500008
    unsigned long FCLKBCR;      // 0xA4150000C
    unsigned long unknownA41500010;
    unsigned long unknownA41500014;
    unsigned long IRDACLKCR;    // 0xA41500018
    unsigned long unknownA4150001C;
    unsigned long unknownA41500020;
    unsigned long PLLCR;        // 0xA41500024
    unsigned long unknownA41500028;
    unsigned long unknownA4150002C;
    unsigned long unknownA41500030;
    unsigned long unknownA41500034;
    unsigned long unknownA41500038;
    unsigned long SPUCLKCR;     // 0xA4150003C
    unsigned long unknownA41500040;
    unsigned long unknownA41500044;
    unsigned long VCLKCR;       // 0xA41500048
    unsigned long unknownA4150004C;
        
    union {                           // struct FLLFRQ   similar to SH7724
        unsigned long LONG;           // long Word Access
        struct {                      // Bit  Access
            unsigned long      :16;   //              
            unsigned long SELXM:2;    //   SELXM      
            unsigned long      :3;    //              
            unsigned long FLF  :11;   //    FLF       
            }   BIT;
        }   FLLFRQ;
        
    unsigned long unknownA41500054;
    unsigned long unknownA41500058;
    unsigned long unknownA4150005C;
    unsigned long LSTATS;
};

struct st_bsc {
    unsigned long CMNCR;
    union {                           // struct CS0BCR   similar to SH7724
        unsigned long LONG;           // long Word Access
        struct {                      // Bit  Access
            unsigned long      :1;    //              
            unsigned long IWW  :3;    //   IWW        
            unsigned long IWRWD:3;    //   IWRWD      
            unsigned long IWRWS:3;    //   IWRWS      
            unsigned long IWRRD:3;    //   IWRRD      
            unsigned long IWRRS:3;    //              
            unsigned long TYPE :4;    //   TYPE       
            unsigned long      :1;    //              
            unsigned long BSZ  :2;    //   BSZ        
            unsigned long      :9;    //              
            }   BIT;
        }   CS0BCR;
    union {                           // struct CS2BCR   similar to SH7724
        unsigned long LONG;           // long Word Access
        struct {                      // Bit  Access
            unsigned long      :1;    //              
            unsigned long IWW  :3;    //   IWW        
            unsigned long IWRWD:3;    //   IWRWD      
            unsigned long IWRWS:3;    //   IWRWS      
            unsigned long IWRRD:3;    //   IWRRD      
            unsigned long IWRRS:3;    //              
            unsigned long TYPE :4;    //   TYPE       
            unsigned long      :1;    //              
            unsigned long BSZ  :2;    //   BSZ        
            unsigned long      :9;    //              
            }   BIT;
        }   CS2BCR;
    union {                           // struct CS3BCR   similar to SH7724
        unsigned long LONG;           // long Word Access
        struct {                      // Bit  Access
            unsigned long      :1;    //              
            unsigned long IWW  :3;    //   IWW        
            unsigned long IWRWD:3;    //   IWRWD      
            unsigned long IWRWS:3;    //   IWRWS      
            unsigned long IWRRD:3;    //   IWRRD      
            unsigned long IWRRS:3;    //              
            unsigned long TYPE :4;    //   TYPE       
            unsigned long      :1;    //              
            unsigned long BSZ  :2;    //   BSZ        
            unsigned long      :9;    //              
            }   BIT;
        }   CS3BCR;
    union {                           // struct CS4BCR   similar to SH7724
        unsigned long LONG;           // long Word Access
        struct {                      // Bit  Access
            unsigned long      :1;    //              
            unsigned long IWW  :3;    //   IWW        
            unsigned long IWRWD:3;    //   IWRWD      
            unsigned long IWRWS:3;    //   IWRWS      
            unsigned long IWRRD:3;    //   IWRRD      
            unsigned long IWRRS:3;    //              
            unsigned long TYPE :4;    //   TYPE       
            unsigned long      :1;    //              
            unsigned long BSZ  :2;    //   BSZ        
            unsigned long      :9;    //              
            }   BIT;
        }   CS4BCR;
    union {                           // struct CS5ABCR   similar to SH7724
        unsigned long LONG;           // long Word Access
        struct {                      // Bit  Access
            unsigned long      :1;    //              
            unsigned long IWW  :3;    //   IWW        
            unsigned long IWRWD:3;    //   IWRWD      
            unsigned long IWRWS:3;    //   IWRWS      
            unsigned long IWRRD:3;    //   IWRRD      
            unsigned long IWRRS:3;    //              
            unsigned long TYPE :4;    //   TYPE       
            unsigned long      :1;    //              
            unsigned long BSZ  :2;    //   BSZ        
            unsigned long      :9;    //              
            }   BIT;
        }   CS5ABCR;
    union {                           // struct CS5BBCR   similar to SH7724
        unsigned long LONG;           // long Word Access
        struct {                      // Bit  Access
            unsigned long      :1;    //              
            unsigned long IWW  :3;    //   IWW        
            unsigned long IWRWD:3;    //   IWRWD      
            unsigned long IWRWS:3;    //   IWRWS      
            unsigned long IWRRD:3;    //   IWRRD      
            unsigned long IWRRS:3;    //              
            unsigned long TYPE :4;    //   TYPE       
            unsigned long      :1;    //              
            unsigned long BSZ  :2;    //   BSZ        
            unsigned long      :9;    //              
            }   BIT;
        }   CS5BBCR;
    union {                           // struct CS6ABCR   similar to SH7724
        unsigned long LONG;           // long Word Access
        struct {                      // Bit  Access
            unsigned long      :1;    //              
            unsigned long IWW  :3;    //   IWW        
            unsigned long IWRWD:3;    //   IWRWD      
            unsigned long IWRWS:3;    //   IWRWS      
            unsigned long IWRRD:3;    //   IWRRD      
            unsigned long IWRRS:3;    //              
            unsigned long TYPE :4;    //   TYPE       
            unsigned long      :1;    //              
            unsigned long BSZ  :2;    //   BSZ        
            unsigned long      :9;    //              
            }   BIT;
        }   CS6ABCR;
    union {                           // struct CS6BBCR   similar to SH7724
        unsigned long LONG;           // long Word Access
        struct {                      // Bit  Access
            unsigned long      :1;    //              
            unsigned long IWW  :3;    //   IWW        
            unsigned long IWRWD:3;    //   IWRWD      
            unsigned long IWRWS:3;    //   IWRWS      
            unsigned long IWRRD:3;    //   IWRRD      
            unsigned long IWRRS:3;    //              
            unsigned long TYPE :4;    //   TYPE       
            unsigned long      :1;    //              
            unsigned long BSZ  :2;    //   BSZ        
            unsigned long      :9;    //              
            }   BIT;
        }   CS6BBCR;
        
    union {                           // struct CS0WCR   similar to SH7724
        unsigned long LONG;           // long Word Access
        struct {                      // Bit  Access
            unsigned long      :11;   //              
            unsigned long BAS  :1;    //   BAS        
            unsigned long      :1;    //              
            unsigned long WW   :3;    //    WW  
            unsigned long ADRS :1;    //    ADRSFIX   
            unsigned long      :2;    //              
            unsigned long SW   :2;    //    SW        
            unsigned long WR   :4;    //    WR  
            unsigned long WM   :1;    //    WM        
            unsigned long      :4;    //              
            unsigned long HW   :2;    //    HW        
            }   BIT;
        }   CS0WCR;
    union {                           // struct CS2WCR   similar to SH7724
        unsigned long LONG;           // long Word Access
        struct {                      // Bit  Access
            unsigned long      :11;   //              
            unsigned long BAS  :1;    //   BAS        
            unsigned long      :1;    //              
            unsigned long WW   :3;    //    WW  
            unsigned long ADRS :1;    //    ADRSFIX   
            unsigned long      :2;    //              
            unsigned long SW   :2;    //    SW        
            unsigned long WR   :4;    //    WR  
            unsigned long WM   :1;    //    WM        
            unsigned long      :4;    //              
            unsigned long HW   :2;    //    HW        
            }   BIT;
        }   CS2WCR;
    union {                           // struct CS3WCR   similar to SH7724
        unsigned long LONG;           // long Word Access
        struct {                      // Bit  Access
            unsigned long      :11;   //              
            unsigned long BAS  :1;    //   BAS        
            unsigned long      :1;    //              
            unsigned long WW   :3;    //    WW  
            unsigned long ADRS :1;    //    ADRSFIX   
            unsigned long      :2;    //              
            unsigned long SW   :2;    //    SW        
            unsigned long WR   :4;    //    WR  
            unsigned long WM   :1;    //    WM        
            unsigned long      :4;    //              
            unsigned long HW   :2;    //    HW        
            }   BIT;
        }   CS3WCR;
    union {                           // struct CS4WCR   similar to SH7724
        unsigned long LONG;           // long Word Access
        struct {                      // Bit  Access
            unsigned long      :11;   //              
            unsigned long BAS  :1;    //   BAS        
            unsigned long      :1;    //              
            unsigned long WW   :3;    //    WW  
            unsigned long ADRS :1;    //    ADRSFIX   
            unsigned long      :2;    //              
            unsigned long SW   :2;    //    SW        
            unsigned long WR   :4;    //    WR  
            unsigned long WM   :1;    //    WM        
            unsigned long      :4;    //              
            unsigned long HW   :2;    //    HW        
            }   BIT;
        }   CS4WCR;
    union {                           // struct CS5AWCR   similar to SH7724
        unsigned long LONG;           // long Word Access
        struct {                      // Bit  Access
            unsigned long      :11;   //              
            unsigned long BAS  :1;    //   BAS        
            unsigned long      :1;    //              
            unsigned long WW   :3;    //    WW  
            unsigned long ADRS :1;    //    ADRSFIX   
            unsigned long      :2;    //              
            unsigned long SW   :2;    //    SW        
            unsigned long WR   :4;    //    WR  
            unsigned long WM   :1;    //    WM        
            unsigned long      :4;    //              
            unsigned long HW   :2;    //    HW        
            }   BIT;
        }   CS5AWCR;
    union {                           // struct CS5BWCR   similar to SH7724
        unsigned long LONG;           // long Word Access
        struct {                      // Bit  Access
            unsigned long      :11;   //              
            unsigned long BAS  :1;    //   BAS        
            unsigned long      :1;    //              
            unsigned long WW   :3;    //    WW  
            unsigned long ADRS :1;    //    ADRSFIX   
            unsigned long      :2;    //              
            unsigned long SW   :2;    //    SW        
            unsigned long WR   :4;    //    WR  
            unsigned long WM   :1;    //    WM        
            unsigned long      :4;    //              
            unsigned long HW   :2;    //    HW        
            }   BIT;
        }   CS5BWCR;
    union {                           // struct CS6AWCR   similar to SH7724
        unsigned long LONG;           // long Word Access
        struct {                      // Bit  Access
            unsigned long      :11;   //              
            unsigned long BAS  :1;    //   BAS        
            unsigned long      :1;    //              
            unsigned long WW   :3;    //    WW  
            unsigned long ADRS :1;    //    ADRSFIX   
            unsigned long      :2;    //              
            unsigned long SW   :2;    //    SW        
            unsigned long WR   :4;    //    WR  
            unsigned long WM   :1;    //    WM        
            unsigned long      :4;    //              
            unsigned long HW   :2;    //    HW        
            }   BIT;
        }   CS6AWCR;
    union {                           // struct CS6BWCR   similar to SH7724
        unsigned long LONG;           // long Word Access
        struct {                      // Bit  Access
            unsigned long      :11;   //              
            unsigned long BAS  :1;    //   BAS        
            unsigned long      :1;    //              
            unsigned long WW   :3;    //    WW  
            unsigned long ADRS :1;    //    ADRSFIX   
            unsigned long      :2;    //              
            unsigned long SW   :2;    //    SW        
            unsigned long WR   :4;    //    WR  
            unsigned long WM   :1;    //    WM        
            unsigned long      :4;    //              
            unsigned long HW   :2;    //    HW        
            }   BIT;
        }   CS6BWCR;
    
};

#define CPG     (*(volatile struct st_cpg  *)0xA4150000)    // FRQCRA    Address
#define BSC     (*(volatile struct st_bsc  *)0xFEC10000)    // CMNCR     Address


