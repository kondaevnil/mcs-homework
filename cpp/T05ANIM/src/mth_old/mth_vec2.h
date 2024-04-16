/* FILENAME:    mth_vec2.h
 * LAST UPDATE: 20.07.2021
 * PROGRAMMER:  DK6
 * PURPOSE:     two component vector realization.
 */

#ifndef __mth_vec2_h_
#define __mth_vec2_h_

#include "mthdef.h"

namespace mth
{
  template<typename Type = DBL>
    class vec2
    {
    private:
      Type X, Y;
    public:
      vec2( VOID ) : X(0), Y(0)
      {
      }

      vec2( Type X, Type Y ) : X(0), Y(0)
      {
      }

      vec2( Type A ) : X(A), Y(A)
      {
      }

      vec2 operator+( const vec2 &V ) const
      {
        return vec2(X + V.X, Y + V.Y);
      }

      vec2 operator-( const vec2 &V ) const
      {
        return vec2(X - V.X, Y - V.Y);
      }

      vec2 operator-( VOID ) const
      {
        return vec2(-X, -Y);
      }

      Type operator!( VOID ) const
      {
        return sqrt(X * X + Y * Y);
      }

      Type operator&( const vec2 &V ) const
      {
        return X * X + Y * Y;
      }

       vec2 operator/( Type N ) const
       {
         return vec2(X / N, Y / N);
       }

       vec2 operator*( Type N ) const
       {
         return vec2(X * N, Y * N);
       }

       vec2<Type> operator*( const vec2 &V ) const
       {
         return vec2(X * V.X, Y * V.Y);
       }

       vec2<Type> operator=( const vec2 &V )
       {
         X = V.X, Y = V.Y;
         return *this;
       }

       vec2<Type> & Normalize( VOID )
       {
         *this /= !*this;
         return *this;
       }

       Type operator[]( INT i ) const
       {
         if (i == 0)
           return X;
         return Y;
       }

       Type & operator[]( INT i )
       {
         if (i == 0)
           return X;
         return Y;
       }

       vec2<Type> & operator+=( const vec2 &V )
       {
         X += V.X, Y += V.Y;
         return *this;
       }

       vec2<Type> & operator-=( const vec2 &V )
       {
         X -= V.X, Y -= V.Y;
         return *this;
       }

       vec2<Type> & operator*=( Type N )
       {
         X *= N, Y *= N;
         return *this;
       }

       vec2<Type> & operator/=( Type N )
       {
         X /= N, Y /= N;
         return *this;
       }

       vec2<Type> & operator*=( const vec2 V )
       {
         X *= V.X, Y *= V.Y;
         return *this;
       }

       vec2<Type> Normalizing( const vec2 V ) const
       {
         return *this / !*this;
       }

       Type Length2( VOID )
       {
         return *this & *this;
       }

       Type Distance( const vec2 &V )
       {
         return !(*this - V);
       }

       static vec2 Zero( VOID )
       {
         return vec2(0);
       }

       operator Type *( VOID )
       {
         return &X;
       }

       vec2<Type> Min( const vec2 v1, const vec2 v2 )
       {
         return vec2(v1.X < v2.X ? v1.X : v2.X, 
                     v1.Y < v2.Y ? v1.Y : v2.Y);
       }

       vec2<Type> Max( const vec2 v1, const vec2 v2 )
       {
         return vec2(v1.X > v2.X ? v1.X : v2.X, 
                     v1.Y > v2.Y ? v1.Y : v2.Y);
       }
    };
}

#endif // __mth_vec2_h_

/* END OF 'mth_vec2.h' FILE */
