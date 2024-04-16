/* FILE NAME  : mth_vec2.h
 * PROGRAMMER : DK6
 * LAST UPDATE: 20.07.2021
 * PURPOSE    : Math vector module.
 */

#ifndef __mth_vec2_h_
#define __mth_vec2_h_

#include <cstdlib>
#include <cmath>

#include "mthdef.h"

/* Space math namespace */
namespace mth
{
  /* 3D vector representation type */
  template<class Type>
    class vec2
    {
    private:
      Type X, Y; /* Vector components */

    template<class Type1>
      friend class matr;
    public:
      /* Vector constructor. 
       * ARGUMENTS: None.
       */
      vec2( VOID ) : X(0), Y(0)
      {
      } /* End of 'vec2' function */

      /* Vector constructor.
       * ARGUMENTS:
       *   - vector value:
       *       Type A;
       */
      vec2( Type A ) : X(A), Y(A)
      {
      } /* End of 'vec' function */

      /* Vector constructor.
       * ARGUMENTS:
       *   - vector values:
       *       Type A, B, C;
       */
      vec2( Type A, Type B ) : X(A), Y(B)
      {
      } /* End of 'vec2' function */

      /* Vector destructor. 
       * ARGUMENTS: None.
       */
      ~vec2( VOID )
      {
      } /* End of '~vec' function */

      /* Return zero vector. 
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec2) zero vector.
       */
      static vec2<Type> Zero( VOID )
      {
        return vec2<Type>(0);
      } /* End of 'Zero' function */

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
        default:
          return 0;
        }
      } /* End of 'operator[]' function */

      /* Adding vector and change value.
       * ARGUMENTS:
       *   - vector to be added:
       *       const vec2 &V;
       * RETURNS:
       *   (vec2) result vector.
       */
      vec2<Type> operator+=( const vec2<Type> &V )
      {
        return *this = vec2<Type>(V.X + X, V.Y + Y);
      } /* End of 'operator+=' function */

      /* Sub vector and change value. 
       * ARGUMENTS:
       *   - vector to be substructed:
       *       const vec2 &V;
       * RETURNS:
       *   (vec2) result vector.
       */
      vec2<Type> operator-=( const vec2<Type> &V )
      {
        return *this = vec2<Type>(V.X - X, V.Y - Y);
      } /* End of 'operator-=' function */

      /* Multiply vector with number and change value. 
       * ARGUMENTS:
       *   - number to be multiplyed:
       *       Type N;
       * RETURNS:
       *   (vec2) result vector.
       */
      vec2<Type> operator*=( Type N )
      {
        return *this = vec2<Type>(X * N, Y * N);
      } /* End of 'operator*=' function */

      /* Divide vector with number and change value.
       * ARGUMENTS:
       *   - number to be divided:
       *       Type N;
       * RETURNS:
       *   (vec2) result vector.
       */
      vec2<Type> operator/=( Type N )
      {
        return *this = vec2<Type>(X / N, Y / N);
      } /* End of 'operator/=' function */

      /* Add two vectors. 
       * ARGUMENTS:
       *   - vector to be added:
       *       const vec2 &V;
       * RETURNS:
       *   (vec2) result vector.
       */
      vec2<Type> operator+( const vec2<Type> &V ) const
      {
        return vec2<Type>(V.X + X, V.Y + Y);
      } /* End of 'operator+' function */

      /* Sub two vectors.
       * ARGUMENTS:
       *   - vector to be substructed:
       *       const vec2 &V;
       * RETURNS:
       *   (vec2) result vector.
       */
      vec2<Type> operator-( const vec2<Type> &V ) const
      {
        return vec2<Type>(X - V.X, Y - V.Y);
      } /* End of 'operator-' function */

      /* Multiply vector with number. 
       * ARGUMENTS:
       *   - number to be multiplied:
       *       Type N;
       * RETURNS: None.
       *   (vec2) result vector.
       */
      vec2<Type> operator*( Type N ) const
      {
        return vec2<Type>(X * N, Y * N);
      } /* End of 'operator*' function */

      /* Divide vector with number.
       * ARGUMENTS:
       *   - number to be divided:
       *       Type N;
       * RESULT:
       *   (vec2) result vector.
       */
      vec2<Type> operator/( Type N ) const
      {
        return vec2<Type>(X / N, Y / N);
      } /* End of 'operator/' function */

      /* Unar minus operation. 
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec2) result vector.
       */
      vec2<Type> operator-( VOID ) const
      {
        return vec2<Type>(-X, -Y);
      } /* End of 'operator~' function */

      /* Component vector multiplying. 
       * ARGUMENTS:
       *   - vector to be multyplied:
       *       const vec2 &V;
       * RETURNS:
       *   (vec2) result vector.
       */
      vec2<Type> operator*( const vec2<Type> &V )
      {
        return vec2<Type>(X * V.X, V.Y * Y);
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

    }; /* End of 'vec2' class */

} /* end of 'mth' namespace */

#endif /* __mth_vec2_h_ */

/* END OF 'mth_vec2.h' FILE */
