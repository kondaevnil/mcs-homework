                                     /* FILE NAME  : mth_vec4.h
 * PROGRAMMER : DK6
 * LAST UPDATE: 20.07.2021
 * PURPOSE    : Math vector module.
 */

#ifndef __mth_vec4_h_
#define __mth_vec4_h_

#include <cstdlib>
#include <cmath>

#include "mthdef.h"

/* Space math namespace */
namespace mth
{
  /* 3D vector representation type */
  template<class Type>
    class vec4
    {
    private:
      Type X, Y, Z, W; /* Vector components */

    template<class Type1>
      friend class matr;
    template<class Type1>
      friend class vec3;
    public:
      /* Vector constructor. 
       * ARGUMENTS: None.
       */
      vec4( VOID ) : X(0), Y(0), Z(0), W(0)
      {
      } /* End of 'vec4' function */

      /* Vector constructor.
       * ARGUMENTS:
       *   - vector value:
       *       Type A;
       */
      vec4( Type A ) : X(A), Y(A), Z(A), W(A)
      {
      } /* End of 'vec' function */

      /* Vector constructor.
       * ARGUMENTS:
       *   - vector values:
       *       Type A, B, C;
       */
      vec4( Type A, Type B, Type C, Type D ) : X(A), Y(B), Z(C), W(D)
      {
      } /* End of 'vec4' function */

      vec4( vec3<Type> V, Type A ) : X(V.X), Y(V.Y), Z(V.Z), W(A)
      {
      } /**/

      /* Vector destructor. 
       * ARGUMENTS: None.
       */
      ~vec4( VOID )
      {
      } /* End of '~vec' function */

      /* Get vector component. 
       * ARGUMENTS:
       *   - index of component:
       *       INT I;
       * RETURNS:
       *   (Type) component of vector.
       */
      Type operator[]( INT I ) const
      {
        switch (I)
        {
        case 0:
          return X;
        case 1:
          return Y;
        case 2:
          return Z;
        case 3:
          return W;
        default:
          return 0;
        }
      } /* End of 'operator[]' function */

      /* Get vector component reference. 
       * ARGUMENTS:
       *   - index of component:
       *       INT I;
       * RETURNS:
       *  (Type &) component reference.
       */
      Type & operator[]( INT I )
      {
        switch (I)
        {
        case 0:
          return X;
        case 1:
          return Y;
        case 2:
          return Z;
        case 3:
          return W;
        default:
          return 0;
        }
      } /* End of 'operator[]' function */

      /* Adding vector and change value.
       * ARGUMENTS:
       *   - vector to be added:
       *       const vec4 &V;
       * RETURNS:
       *   (vec4) result vector.
       */
      vec4<Type> operator+=( const vec4<Type> &V )
      {
        return *this = vec4<Type>(V.X + X, V.Y + Y, V.Z + Z, W + V.W);
      } /* End of 'operator+=' function */

      /* Sub vector and change value. 
       * ARGUMENTS:
       *   - vector to be substructed:
       *       const vec4 &V;
       * RETURNS:
       *   (vec4) result vector.
       */
      vec4<Type> operator-=( const vec4<Type> &V )
      {
        return *this = vec4<Type>(V.X - X, V.Y - Y, V.Z - Z, V.W - W);
      } /* End of 'operator-=' function */

      /* Multiply vector with number and change value. 
       * ARGUMENTS:
       *   - number to be multiplyed:
       *       Type N;
       * RETURNS:
       *   (vec4) result vector.
       */
      vec4<Type> operator*=( Type N )
      {
        return *this = vec4<Type>(X * N, Y * N, Z * N, W * N);
      } /* End of 'operator*=' function */

      /* Divide vector with number and change value.
       * ARGUMENTS:
       *   - number to be divided:
       *       Type N;
       * RETURNS:
       *   (vec4) result vector.
       */
      vec4<Type> operator/=( Type N )
      {
        return *this = vec4<Type>(X / N, Y / N, Z / N, W / N);
      } /* End of 'operator/=' function */

      /* Add two vectors. 
       * ARGUMENTS:
       *   - vector to be added:
       *       const vec4 &V;
       * RETURNS:
       *   (vec4) result vector.
       */
      vec4<Type> operator+( const vec4<Type> &V ) const
      {
        return vec4<Type>(V.X + X, V.Y + Y, V.Z + Z, V.W + W);
      } /* End of 'operator+' function */

      /* Sub two vectors.
       * ARGUMENTS:
       *   - vector to be substructed:
       *       const vec4 &V;
       * RETURNS:
       *   (vec4) result vector.
       */
      vec4<Type> operator-( const vec4<Type> &V ) const
      {
        return vec4<Type>(X - V.X, Y - V.Y, Z - V.Z, W - Z.W);
      } /* End of 'operator-' function */

      /* Multiply vector with number. 
       * ARGUMENTS:
       *   - number to be multiplied:
       *       Type N;
       * RETURNS: None.
       *   (vec4) result vector.
       */
      vec4<Type> operator*( Type N ) const
      {
        return vec4<Type>(X * N, Y * N, Z * N, W * N);
      } /* End of 'operator*' function */

      /* Divide vector with number.
       * ARGUMENTS:
       *   - number to be divided:
       *       Type N;
       * RESULT:
       *   (vec4) result vector.
       */
      vec4<Type> operator/( Type N ) const
      {
        return vec4<Type>(X / N, Y / N, Z / N, W / N);
      } /* End of 'operator/' function */

      /* Unar minus operation. 
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec4) result vector.
       */
      vec4<Type> operator-( VOID ) const
      {
        return vec4<Type>(-X, -Y, -Z, -W);
      } /* End of 'operator~' function */

      /* Component vector multiplying. 
       * ARGUMENTS:
       *   - vector to be multyplied:
       *       const vec4 &V;
       * RETURNS:
       *   (vec4) result vector.
       */
      vec4<Type> operator*( const vec4<Type> &V )
      {
        return vec4<Type>(X * V.X, V.Y * Y, Z * V.Z, W * V.W);
      } /* End of 'operator*' function */

      /* Return vector pointer.
       * ARGUMENTS: None.
       * RETURNS:
       *  (Type *) vector pointer.
       */
      operator Type *( VOID )
      {
        return &X;
      } /* End of 'operator' function */

      /* Return vector pointer. 
       * ARGUMENTS: None.
       * RETURNS:
       *   (Type *) vector pointer.
       */
      operator const Type *( VOID ) const
      {
        return &X;
      } /* End of 'operator' function */

    }; /* End of 'vec4' class */

} /* end of 'mth' namespace */

#endif /* __mth_vec4_h_ */

/* END OF 'mth_vec4.h' FILE */
