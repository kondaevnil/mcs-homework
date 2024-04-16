/* FILE NAME  : mthdef.h
 * PROGRAMMER : DK6
 * LAST UPDATE: 20.07.2021
 * PURPOSE    : Math definitions module.
 */


#ifndef WIN32
  #define WIN32
  #define _CRT_SECURE_NO_WARNINGS
  #include <commondf.h>
  #undef WIN32
#else
  #define _CRT_SECURE_NO_WARNINGS
  #include <commondf.h>
#endif /* WIN32 */

#ifndef __mthdef_h_
#define __mthdef_h_

#include <cmath>

#include <cassert>

#include <windows.h>

typedef DOUBLE DBL;
typedef FLOAT FLT;

#define PI 3.14159265358979323846

namespace mth
{
  template <typename Type> class vec2;
  template <typename Type> class vec3;
  template <typename Type> class vec4;
  template <typename Type> class matr;

  // Getting smallest number function
  template <typename T>
  T Min( T Num1, T Num2 )
  {
    return Num1 < Num2 ? Num1 : Num2;
  }

  template <typename T>
  T Max( T Num1, T Num2 )
  {
    return Num1 > Num2 ? Num1 : Num2;
  }

  template <typename T>
  T Deg2Rad( T AngleInDegree )
  {
    return AngleInDegree * (PI / 180);
  }

  template <typename T>
  T Rad2Deg( T AngleInRadians )
  {
    return AngleInRadians * (180 / PI);
  }

  template <typename T>
  T Clamp( T Num, T Min = 0, T Max = 1 )
  {
    return Num < Min ? Min : Num > Max ? Max : Num;
  }

  template <typename T>
  T Lerp( T Start, T End, T Current )
  {
    return Start + (End - Start) * Current;
  }
}

#endif // !__mthdef_h_

/* END OF 'mthdef.h' FILE */
