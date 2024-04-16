/* FILENAME:    stock.h
 * LAST UPDATE: 22.07.2021
 * PROGRAMMER:  DK6
 * PURPOSE:     Data store system (stock).
 */

#ifndef __stock_h_
#define __stock_h_

#include <cassert>


// implementation
template<typename type>
class stock
{
private:
  INT RealSize, MaxSize;
  type *Array;

public:
  stock( void ) : Array( nullptr ), RealSize( 0 ), MaxSize( 0 )
  {
  }
  ~stock( void )
  {
    if (Array != nullptr)
      delete[] Array;
    Array = nullptr;
    RealSize = 0;
    MaxSize = 0;
  }

  stock( const stock<type> &CopyData ) :
    Array( new type[CopyData.RealSize] ),
    RealSize( CopyData.RealSize ),
    MaxSize( CopyData.RealSize )
  {
    for (int i = 0; i < RealSize; i++)
      Array[i] = CopyData.Array[i];
  }

  stock &operator=( const stock<type> &CopyData )
  {
    if (Array != nullptr)
      delete[] Array;
    Array = new type[CopyData.RealSize];
    RealSize = CopyData.RealSize;
    MaxSize = CopyData.RealSize;
    for (int i = 0; i < RealSize; i++)
      Array[i] = CopyData.Array[i];
    return *this;
  }

  int Size( void ) const
  {
    return RealSize;
  }
  stock &Add( const type &NewData )
  {
    if (RealSize >= MaxSize)
    {
      // increase allocated memory
      if (MaxSize == 0)
        MaxSize = 1;
      else
        MaxSize *= 2;

      type *new_bulk = new type[MaxSize];
      if (Array != nullptr)
      {
        // copy old data
        for (int i = 0; i < RealSize; i++)
          new_bulk[i] = Array[i];
        // remove old data
        delete[] Array;
      }
      Array = new_bulk;
    }
    Array[RealSize++] = NewData;
    return *this;
  }
  stock &operator<<( const type &NewData )
  {
    return Add( NewData );
  }

  type &operator[]( int Index )
  {
    assert( Index >= 0 && Index < RealSize );
    return Array[Index];
  }
  type &operator[]( int Index ) const
  {
    assert( Index >= 0 && Index < RealSize );
    return Array[Index];
  }

  void Clear( void )
  {
    RealSize = 0;
  }

  template<typename type2>
  void Walk( const type2 &Walker )
  {
    for (int i = 0; i < RealSize; i++)
      Walker( Array[i] );
  }

  /* Return last stock element function
   * ARGUMENTS: None.
   * RETURNS: 
   *   (type) last stock element.
   */
  type Back( VOID )
  {
    return Array[RealSize - 1];
  } /* END OF 'Back' function */
};


#endif // __stock_h_


/* END OF 'stock.h' FILE */
