// syscalls are defined in syscall.hpp and syscall.cpp
// this file is used for derived functions

#ifndef __MCS_HPP__
#define __MCS_HPP__

#include "ListView.hpp"

#define MCS_NO_ERROR 0
#define MCS_MEMORY_FULL 0x5F


int DeleteMainMemory( unsigned char*dir, unsigned char*item );
int ReadMainMemory( unsigned char*dir, unsigned char*item, int len, void*buffer );
int WriteMainMemory( unsigned char*dir, unsigned char*item, int len, void*buffer );
int CheckMainMemory( unsigned char*dir, unsigned char*item );

typedef struct {
  unsigned char name[8];
  int offset;
  int length;
  char flags[4];
} TMCSItem;

typedef struct {
  unsigned char name[8];
  TMCSItem*addr;
  short itemcount;
  short wossname;
} TMCSDirEntry;

////////// TMCSView //////////
class TMCSView : public TListView {
    public:
        TMCSView( int X1=1, int Y1=1, int X2=21, int Y2=9 )  : TListView( X1, Y1, X2, Y2 ) { 
            FSource = (TMCSDirEntry*)0x880D36D0;
            FSize = 0x93;
            SetHeading( "Main Memory" );
            FOnCreate();
        }
        virtual ~TMCSView();
            
    protected:  
        virtual int vBeforeBrowse();
        virtual int vPrevLine();
        virtual int vNextLine();
        virtual int vSourceGetCurrentItem( TListViewItem w );
        virtual int vSourceBOL() const;
        virtual int vSourceEOL() const;
        virtual int vSourceJump( TListViewJump dir );
        virtual int vProcessMessage();
        virtual int vShowAll();
    private:
        int FSize;
        TMCSDirEntry*FSource;
        int FSubView() const;
        int FDelete();
        int FOnCreate();
    
};
////////// TMCSView //////////

////////// TMCSItemView //////////
class TMCSItemView : public TListView {
    public:
        TMCSItemView( int X1 = 1, int Y1 = 1, int X2 = 21, int Y2 = 8 ) : TListView( X1, Y1, X2, Y2 ){
            FOnCreate();
        };
        virtual ~TMCSItemView();

        TMCSDirEntry*parent;
        int FOnCreate();
        int InitArray( TMCSItem*pitem, int size );
    
    protected:  
        virtual int vBeforeBrowse();
        virtual int vPrevLine();
        virtual int vNextLine();
        virtual int vSourceGetCurrentItem( TListViewItem w );
        virtual int vSourceBOL() const;
        virtual int vSourceEOL() const;
        virtual int vSourceJump( TListViewJump dir );
        virtual int vProcessMessage();
    
    private:
        int FSize;
        TMCSItem*FSource;
        int FView();
        int FDelete();
};
////////// TMCSItemView //////////


#endif