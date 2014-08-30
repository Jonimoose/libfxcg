// syscalls are defined in syscall.hpp and syscall.cpp
// this file is used for derived functions

#ifndef __UI_HPP__
#define __UI_HPP__

#include "HEAP_syscalls.h"

enum TFKeySubFunc { fksfNORMAL, fksfNEXTMENU, fksfCYCLE };

enum TInputFlags { ifNONE, ifALWAYS_ALPHA };

typedef struct { int fk_no; unsigned char*fk_bitmap; } TFKDef;

////////// TFKeyRegister //////////
class TFKeyRegister {
  public :
    TFKeyRegister(){
      FKeyDefCount = 0;
      FRegisteredFKeys = 0;
      flags = ifNONE;
    };
    ~TFKeyRegister(){
       if (FRegisteredFKeys) free(FRegisteredFKeys);

    };
    int RegisterFKey( TFKDef*fk_def );
    int Count();
    unsigned char*FKEY_Ptr( int fk_num );

  protected :
	TInputFlags flags;
  private :
    TFKDef*FRegisteredFKeys;
    int FKeyDefCount;
};
////////// TFKeyRegister //////////

////////// TInput //////////
//
class TInput{
  public:
    TInput();
    ~TInput();
    TInputFlags flags;
    unsigned int getkey();
    unsigned int keypressed();
    void SetFKey( const int fk_no, int*fk_nums );
    void ShowFKeys() const;
    int SetFKeyState( const int fk_no, const int value );
  protected:
  private:
    int*FKey[6];
    int FSubFuncNo[6];
    
    unsigned int FProcessKey( unsigned int key );

};
////////// TInput //////////

#endif
