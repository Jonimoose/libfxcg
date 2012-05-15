#ifndef __LISTVIEW_HPP__
#define __LISTVIEW_HPP__

#include "StrList.hpp"
#include "UI.hpp"
#include "STD_syscalls.h"
#include "stdlib.h"
#include "CONVERT_syscalls.h"


#undef __LISTVIEW_DEBUG__

#define lvmINIT				0x00000001
#define lvmKEY					0x00010000
#define lvmEMPTY_LIST		0x80000001

#define FLAG_REFRESH		0x00000001
#define FLAG_SHOWFKEYS	0x00000002
#define FLAG_RESET				0x00000004

enum TListViewJump { lvjTOP, lvjBOTTOM };
enum TFKeyType { FKT_BITMAP, FKT_STRING };
enum TViewMode { mvmHEX, mvmASC, mvmADDR, mvmLEN };

typedef char TListViewItem [50];

typedef struct {
	TFKeyType fkt;
	void*p;
} TFKeyBitmap;
typedef TFKeyBitmap TFKeyBitmaps [6];

typedef struct {
  int message;
  int lParam;
  int wParam;
} TMessage;
typedef  int(*TListviewCallback)( TMessage& );	// function pointer int (TMessage&)

////////// TListView //////////
class TListView {
  public:
    TMessage FMessage;
    TListView( int X1 = 1, int Y1 = 1, int X2 = 21, int Y2 = 9 ){
		FMessage.message = lvmINIT;
		FMinY = Y1;
		FMaxY = Y2;
		FMinX = X1;
		FMaxX = X2;
		FYDisp = FMinY;
		FFlags = FLAG_SHOWFKEYS;
		FCallback = 0;
		FCurrentItemId = 1;
		memset( FFKeys, 0, sizeof( FFKeys ) );
	
		memset( FHeading, 0, sizeof( FHeading ) );
	};
    virtual ~TListView();
    int browse();
    int SetHeading( char* heading );
    void Refresh( int mode = 0 );
    int ShowFKeys( void );
    int SetFKey( int idx, const void*value = 0, TFKeyType _fkt = FKT_BITMAP );
    
    virtual int vSourceGetCurrentItem( TListViewItem w ) = 0;
    
    void SetCallback( TListviewCallback callback );
    
  protected:  

    int FCurrentItemId;
    
    virtual int vBeforeBrowse();
    
    virtual int vSourceMove( int diff );
    virtual int vPrevLine() = 0;
    virtual int vNextLine() = 0;
    virtual int vSourceBOL() const = 0;
    virtual int vSourceEOL() const = 0;
    virtual int vSourceJump( TListViewJump dir ) = 0;
    virtual int vShowAll();
    virtual int vProcessMessage();
    virtual int vGetMessage( TMessage&msg );
    virtual int vOnShowCurrentItem();
	int FKeyDefined( int FKeyNo );
	int SetFYDisp( int value );
	TViewMode PViewMode;

  private:
    int FFlags;
    char FHeading[0x55];
	int FX;
	int FY;
    int FMinY;
    int FMaxY;
    int FMinX;
    int FMaxX;
    int FYDisp;
	TFKeyBitmaps FFKeys;
    TListviewCallback FCallback;

    int FProcessMessage();
    void FShowAll();
    void FShowLine( const int iLine, int reverse = 0 );
	void locate( int x, int y );
	void Print( const unsigned char* w );
	void PrintR( const unsigned char* w );
};
////////// TListView //////////

////////// TIntArrayView //////////
class TIntArrayView : public TListView {
  public:
    TIntArrayView( int X1 = 1, int Y1 = 1, int X2 = 21, int Y2 = 9 )  : TListView( X1, Y1, X2, Y2 ) { 
       FSource=0;
       FSize=0; 
    };
    virtual ~TIntArrayView();
    
    int InitArray( int size, int itemsize );
  protected:  
    virtual int vBeforeBrowse();
    virtual int vPrevLine();
    virtual int vNextLine();
    virtual int vSourceGetCurrentItem( TListViewItem w );
    virtual int vSourceBOL() const;
    virtual int vSourceEOL() const;
    virtual int vSourceJump( TListViewJump dir );
    virtual int vProcessMessage();
	int ProcessMessageDemo();
  private:
    int FSize;
    int*FSource;
  
};
////////// TIntArrayView //////////

////////// TStrListView //////////
class TStrListView : public TListView {
  public:
    TStrListView( int X1 = 1, int Y1 = 1, int X2 = 21, int Y2 = 8 )  : TListView( X1, Y1, X2, Y2 ) { 
       FSource=0;
       FPrivateStringList = 0;
       
#ifdef __LISTVIEW_DEBUG__
OBJECT_DEBUG("TStrListView.constr")
#endif

    };
    virtual ~TStrListView(){
    
#ifdef __LISTVIEW_DEBUG__
OBJECT_DEBUG("TStrListView.destr")
#endif

	FDestroyPrivateStringList();
    };
    
    int Assign( TStringList*source );
    int Assign( char*strings );
    
  protected:  
    TStringList*FSource;
    virtual int vBeforeBrowse();
    virtual int vPrevLine();
    virtual int vNextLine();
    virtual int vSourceGetCurrentItem( TListViewItem w );
    virtual int vSourceBOL() const;
    virtual int vSourceEOL() const;
    virtual int vSourceJump( TListViewJump dir );
  private:
    int FPrivateStringList;
    void FDestroyPrivateStringList();
  
};
////////// TStrListView //////////

#endif
