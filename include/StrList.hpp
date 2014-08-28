#ifndef __STRLIST_HPP__
#define __STRLIST_HPP__

extern "C" {



int StrListCat( unsigned char*, unsigned char* );
unsigned char*StrListNext( unsigned char*current );
int StrListCnt( unsigned char* );
/*
unsigned char*StrListTop( unsigned char* );
unsigned char*StrListBottom( unsigned char* );
unsigned char*StrListPrev( unsigned char*, unsigned char* );
int StrListLen( unsigned char* );
int StrListNearFind( unsigned char*,  unsigned char* );
void StrListSort( unsigned char* );
*/

typedef  int( *TStrListSortFunction )( const char*, const char* );

// example-TStrListSortFunction to sort a stringlist in descending order.
int SortStrDescending( const char*s1, const char*s2 );

////////// TStringList //////////
//
class TStringList{
  public:
    TStringList(){
      Buffer = 0;
      Pos = 0;
      pBOL = 0;
      FSize = 0;
      FForeignSource = 0;
    };
    ~TStringList(){
      FReleaseBuffer();
    };
    char*Buffer;
    int Pos;
    char*pBOL;
    int Load( const unsigned short*fn );
    int AddString( const char*string );
    int PrevLine();
    char* Line();
    int NextLine();
    void GoBeginOfList();
    void GoEndOfList();
    int IsBeginOfList();
    int IsEndOfList();
    int StringLength();
    int Assign( char*c );
	void Sort( TStrListSortFunction sorter = 0 );
    
    int Name( char*c );
    int Value( char*c );
    int Param( int pos, char*c );
  protected:
  private:
    int FForeignSource;
    int FSize;
    char*FSkipChars( const char*c, char*start );
    char*FWaitChars( const char*c, char*start );
    int goEOL();
    int isEOL( const int pos );
    int FReleaseBuffer();

};
////////// TStringList //////////


}
#endif
