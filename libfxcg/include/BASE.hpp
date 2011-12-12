#ifndef __BASE_HPP__
#define __BASE_HPP__

#define OBJECT_DEBUG(A) locate( 1,1 );Print( A );PrintLine( "", 21 );locate( 1,2 );PrintHex( (int)this, 8 );PrintLine( "", 21 );Wait();locate( 1,1 );PrintLine( "", 21 );locate( 1,2 );PrintLine( "", 21 );

#define incAPP "app.hpp"
#define incBATTERY "battery.hpp"
#define incBCD "BCD.hpp"
#define incCPU "cpu.hpp"
#define incDISPLAY "display.hpp"
#define incESM "ESM.hpp"
#define incFILES "files.hpp"
#define incFLASH "FX9860G_flashwriter.h"
#define incFLASH2 "FLASH.hpp"
#define incFLASHWRITER "FLASHWRITER_ASM.hpp"
#define incHELPVIEW "HELPVIEW.hpp"
#define incINPUTS "inputs.hpp"
#define incKEYBOARD "keyboard.hpp"
#define incLISTVIEW "LISTVIEW.hpp"
#define incMASK "mask.hpp"
#define incMCS "MCS.hpp"
#define incMEMVIEW "MEMVIEW.hpp"
#define incMENU "menu.hpp"
#define incOS "os.hpp"
#define incRTC "RTC.hpp"
#define incRUNLIB "RUNLIB.hpp"
#define incSERIAL "serial.hpp"
#define incSTORAGEMEM "storagemem.hpp"
#define incSTRLIST "strlist.hpp"
#define incSYSCALL "syscall.hpp"
#define incSYSCALLWRAPPER "syscallwrapper.hpp"
#define incTOOLS "tools.hpp"
#define incTREEMENU "treemenu.hpp"
#define incVIEWTEXT "viewtext.hpp"
#define incUBCTOOLS "UBCTOOLS.hpp"
#define incUI "UI.hpp"
#define incGRAPHFUNC "GRAPHFUNC.hpp"
#define incEXPRSYMVIEW "EXPRSYMVIEW.hpp"
#define incFILEVIEW "FILEVIEW.hpp"
#define incMENUVIEW "MENUVIEW.hpp"
#define incESMVIEW "ESMVIEW.hpp"
#define incFX9860_STRUCTS "FX9860_STRUCTS.hpp"

#define incLIBSUBST "libsubst.hpp"


#endif