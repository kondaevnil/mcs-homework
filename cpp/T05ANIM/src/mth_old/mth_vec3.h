/* FILENAME:    mth_vec3.h
 * LAST UPDATE: 20.07.2021
 * PROGRAMMER:  DK6
 * PURPOSE:     three component vector realization.
 */

#ifndef __mth_vec3_h_
#define __mth_vec3_h_

#include "mthdef.h"

namespace mth
{
  template<typename Type = DBL>
    class vec3
    {
    private:
      Type X, Y, Z;
      template<typename Type1>
        friend class matr;
    public:
      vec3( VOID ) : X(0), Y(0), Z(0)
      {
      }

      vec3( Type X, Type Y, Type Z ) : X(X), Y(Y), Z(Z)
      {
      }

      vec3( Type A ) : X(A), Y(A), Z(A)
      {
      }

      vec3 operator+( const vec3 &V ) const
      {
        return vec3(X + V.X, Y + V.Y, Z + V.Z);
      }

      vec3 operator-( const vec3 &V ) const
      {
        return vec3(X - V.X, Y - V.Y, Z - V.Z);
      }

      vec3 operator-( VOID ) const
      {
        return vec3(-X, -Y, -Z);
      }

      Type operator!( VOID ) const
      {
        return sqrt( *this & *this );
      }

      Type operator&( const vec3 &V ) const
      {
        return X * X + Y * Y + Z * Z;
      }

       vec3 operator/( Type N ) const
       {
         return vec3(X / N, Y / N, Z / N);
       }

       vec3 operator*( Type N ) const
       {
         return vec3(X * N, Y * N, Z * N);
       }

       vec3<Type> operator*( const vec3 &V ) const
       {
         return vec3(X * V.X, Y * V.Y, Z * V.Z);
       }

       vec3<Type> operator=( const vec3 &V )
       {
         X = V.X, Y = V.Y, Z = V.Z;
         return *this;
       }

       vec3<Type> & Normalize( VOID )
       {
         *this /= !(*this);
         return *this;
       }

       Type operator[]( INT i ) const
       {
         if (i == 0)
           return X;
         else if (i == 1)
           return Y;
         return Z;
       }

       Type & operator[]( INT i )
       {
         if (i == 0)
           return X;
         else if (i == 1)
           return Y;
         return Z;
       }

       vec3<Type> & operator+=( const vec3 &V )
       {
         X += V.X, Y += V.Y, Z += V.Z;
         return *this;
       }

       vec3<Type> & operator-=( const vec3 &V )
       {
         X -= V.X, Y -= V.Y, Z -= V.Z;
         return *this;
       }

       vec3<Type> & operator*=( Type N )
       {
         X *= N, Y *= N, Z *= N;
         return *this;
       }

       vec3<Type> & operator/=( Type N )
       {
         X /= N, Y /= N, Z /= N;
         return *this;
       }

       vec3<Type> & operator*=( const vec3 V )
       {
         X *= V.X, Y *= V.Y, Z *= V.Z;
         return *this;
       }

       vec3<Type> Normalizing( const vec3 V ) const
       {
         return *this / !(*this);
       }

       Type Length2( VOID )
       {
         return *this & *this;
       }

       Type Distance( const vec3 &V )
       {
         return !(*this - V);
       }

       static vec3 Zero( VOID )
       {
         return vec3(0);
       }

       operator Type *( VOID )
       {
         return &X;
       }

       vec3<Type> operator%( const vec3 &V ) const
       {
         return vec3<Type>( Y * V.Z - V.Y * Z,
           Z * V.X - V.Z * X,
           X * V.Y - V.X * Y );
       }

       vec3<Type> & operator%=( const vec3 &V )
       {
         X = Y * V.Z - V.Y * Z, Y = Z * V.X - V.Z * X, Z = X * V.Y - V.X * Y;
         return *this;
       }

       vec3<Type> Min( const vec3 v1, const vec3 v2 )
       {
         return vec3(v1.X < v2.X ? v1.X : v2.X, 
                     v1.Y < v2.Y ? v1.Y : v2.Y,
                     v1.Z < v2.Z ? v1.Z : v2.Z);
       }

       vec3<Type> Max( const vec3 v1, const vec3 v2 )
       {
         return vec3(v1.X > v2.X ? v1.X : v2.X, 
                     v1.Y > v2.Y ? v1.Y : v2.Y,
                     v1.Z > v2.Z ? v1.Z : v2.Z);
       }

       vec3<Type> operator*( matr<Type> m )
       {
         return Transform4x4(m);
       }

       vec3<Type> Transform4x4( matr<Type> m )
       {
         Type w = X + m.A[0][3] + Y * m.A[1][3] + Z * m.A[2][3] + m.A[3][3];

         return vec3((X * m.A[0][0] + Y * m.A[1][0] + Z * m.A[2][0] + m.A[3][0]) / w,
                     (X * m.A[0][1] + Y * m.A[1][1] + Z * m.A[2][1] + m.A[3][1]) / w, 
                     (X * m.A[0][2] + Y * m.A[1][2] + Z * m.A[2][2] + m.A[3][2]) / w);
       }

       /*vec3<Type> TransformNormal( const vec3<Type> &N ) const
       {
         EvaluateInverseMatrix();
         return vec3<Type>(N.X * InvA[0][0] + N.Y * InvA[0][1] + N.Z * InvA[0][2],
                           N.X * InvA[1][0] + N.Y * InvA[1][1] + N.Z * InvA[1][2],
                           N.X * InvA[2][0] + N.Y * InvA[2][1] + N.Z * InvA[2][2]);
       }*/

       vec3 TransformPoint( const matr<Type> m ) const
       {
         return vec3(X * m.A[0][0] + Y * m.A[1][0] + Z * m.A[2][0] + m.A[3][0],
                     X * m.A[0][1] + Y * m.A[1][1] + Z * m.A[2][1] + m.A[3][1],
                     X * m.A[0][2] + Y * m.A[1][2] + Z * m.A[2][2] + m.A[3][2]);
       }

       vec3 TransformVector( const matr<Type> m ) const
       {
         return vec3(X * m.A[0][0] + Y * m.A[1][0] + Z * m.A[2][0],
                     X * m.A[0][1] + Y * m.A[1][1] + Z * m.A[2][1],
                     X * m.A[0][2] + Y * m.A[1][2] + Z * m.A[2][2]);
       }
    };
}

#endif // __mth_vec3_h_

/* END OF 'mth_vec3.h' FILE */
