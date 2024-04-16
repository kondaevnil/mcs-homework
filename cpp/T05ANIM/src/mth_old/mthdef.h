/* FILENAME:    mthdef.h
 * LAST UPDATE: 20.07.2021
 * PROGRAMMER:  DK6
 * PURPOSE:     My program.
 */

#ifndef __mthdef_h_
#define __mthdef_h_

#include <cmath>

#include <commondf.h>

typedef DOUBLE DBL;

namespace mth
{
  const DBL PI = 3.14159265358979323846;

  template<typename T>
    VOID Swap( T *A, T *B )
    {
      T C = *A;
      *A = *B, *B = C;
    }

  template<typename Type>
    class matr;

  template<typename Type>
    class vec3;

  template<typename Type>
    DBL Lerp( Type A, Type B, DBL X )
    {
      return (B - A) * X + A;
    }
 
  template<typename Type>
    DBL D2R( Type Angle )
    {
      return Angle * (PI / 180.0);
    }

  }

#endif // __mthdef_h_

/* END OF 'mthdef.h' FILE */
