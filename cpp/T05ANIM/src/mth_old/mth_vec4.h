/* FILENAME:    mth_vec4.h
 * LAST UPDATE: 20.07.2021
 * PROGRAMMER:  DK6
 * PURPOSE:     four component vector realization.
 */

#ifndef __mth_vec4_h_
#define __mth_vec4_h_

#include "mthdef.h"

namespace mth
{
  template<typename Type = DBL>
    class vec4
    {
    private:
      Type X, Y, Z, W;
    public:
      vec4( VOID ) : X(0), Y(0), Z(0), W(0)
      {
      }

      vec4( Type X, Type Y, Type Z, Type W ) : X(0), Y(0), Z(0), W(0)
      {
      }

      vec4( Type A ) : X(A), Y(A), Z(A), W(A)
      {
      }

      vec4 operator+( const vec4 &V ) const
      {
        return vec4(X + V.X, Y + V.Y, Z + V.Z + V.W);
      }

      vec4 operator-( const vec4 &V ) const
      {
        return vec4(X - V.X, Y - V.Y, Z - V.Z - V.W);
      }

      vec4 operator-( VOID ) const
      {
        return vec4(-X, -Y, -Z, -W);
      }

      Type operator!( VOID ) const
      {
        return sqrt(X * X + Y * Y + Z * Z + W * W);
      }

      Type operator&( const vec4 &V ) const
      {
        return X * X + Y * Y + Z * Z + W * W;
      }

       vec4 operator/( Type N ) const
       {
         return vec4(X / N, Y / N, Z / N, W / N);
       }

       vec4 operator*( Type N ) const
       {
         return vec4(X * N, Y * N, Z * N, W * N);
       }

       vec4<Type> operator*( const vec4 &V ) const
       {
         return vec4(X * V.X, Y * V.Y, Z * V.Z, W * V.W);
       }

       vec4<Type> operator=( const vec4 &V )
       {
         X = V.X, Y = V.Y, Z = V.Z, W = V.W;
         return *this;
       }

       vec4<Type> & Normalize( VOID )
       {
         *this /= !*this;
         return *this;
       }

       Type operator[]( INT i ) const
       {
         if (i == 0)
           return X;
         else if (i == 1)
           return Y;
         else if (i == 2)
           return Z;
         return W;
       }

       Type & operator[]( INT i )
       {
         if (i == 0)
           return X;
         else if (i == 1)
           return Y;
         else if (i == 2)
           return Z;
         return W;
       }

       vec4<Type> & operator+=( const vec4 &V )
       {
         X += V.X, Y += V.Y, Z += V.Z, W += V.W;
         return *this;
       }

       vec4<Type> & operator-=( const vec4 &V )
       {
         X -= V.X, Y -= V.Y, Z -= V.Z, W -= V.W;
         return *this;
       }

       vec4<Type> & operator*=( Type N )
       {
         X *= N, Y *= N, Z *= N, W *= N;
         return *this;
       }

       vec4<Type> & operator/=( Type N )
       {
         X /= N, Y /= N, Z /= N, W /= N;
         return *this;
       }

       vec4<Type> & operator*=( const vec4 V )
       {
         X *= V.X, Y *= V.Y, Z *= V.Z, W *= V.W;
         return *this;
       }

       vec4<Type> Normalizing( const vec4 V ) const
       {
         return *this / !*this;
       }

       Type Length2( VOID )
       {
         return *this & *this;
       }

       Type Distance( const vec4 &V )
       {
         return !(*this - V);
       }

       static vec4 Zero( VOID )
       {
         return vec4(0);
       }

       operator Type *( VOID )
       {
         return &X;
       }

       vec4<Type> Min( const vec4 v1, const vec4 v2 )
       {
         return vec4(v1.X < v2.X ? v1.X : v2.X, 
                     v1.Y < v2.Y ? v1.Y : v2.Y,
                     v1.Z < v2.Z ? v1.Z : v2.Z,
                     v1.W < v2.W ? v1.W : v2.W );
       }

       vec4<Type> Max( const vec4 v1, const vec4 v2 )
       {
         return vec4(v1.X > v2.X ? v1.X : v2.X, 
                     v1.Y > v2.Y ? v1.Y : v2.Y,
                     v1.Z > v2.Z ? v1.Z : v2.Z,
                     v1.W > v2.W ? v1.W : v2.W );
       }
    };
}

#endif // __mth_vec4_h_

/* END OF 'mth_vec4.h' FILE */
