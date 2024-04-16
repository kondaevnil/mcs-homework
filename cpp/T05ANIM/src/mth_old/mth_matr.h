/* FILENAME:    mth_matr.h
 * LAST UPDATE: 20.07.2021
 * PROGRAMMER:  DK6
 * PURPOSE:     matrix realization.
 */

#ifndef __mth_matr_h_
#define __mth_matr_h_

#include "mth_vec3.h"

namespace mth
{
  template<typename Type = DBL>
    class matr
    {
    private:
      Type A[4][4];
      mutable Type InvA[4][4];
      mutable BOOL IsInverseMatrix;

      VOID EvaluateInverseMatrix( VOID ) const
      {
        if (IsInverseMatrix)
          return;
        IsInverseMatrix = TRUE;
        InvA = Inverse().A;
      }

    public:
      matr( VOID ) : IsInverseMatrix(FALSE)
      {
      }

      matr( const Type *M ) : IsInverseMatrix( FALSE )
      {
        for (INT i = 0; i < 4; i++)
          for (INT j = 0; j < 4; j++)
            A[i][j] = M[i][j];
      }

      matr( Type A00, Type A01, Type A02, Type A03,
            Type A10, Type A11, Type A12, Type A13,
            Type A20, Type A21, Type A22, Type A23,
            Type A30, Type A31, Type A32, Type A33 ) : IsInverseMatrix( FALSE )
      {
        A[0][0] = A00, A[0][1] = A01, A[0][2] = A02, A[0][3] = A03;
        A[1][0] = A10, A[1][1] = A11, A[1][2] = A12, A[1][3] = A13;
        A[2][0] = A20, A[2][1] = A21, A[2][2] = A22, A[2][3] = A23;
        A[3][0] = A30, A[3][1] = A31, A[3][2] = A32, A[3][3] = A33;
      }

      static matr Identity( VOID )
      {
        return matr(1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1);
      }

      static matr RotateZ( Type AngleInDegree )
      {
        DBL a = D2R(AngleInDegree), c = cos(a), s = sin(a);
        return matr(c, s, 0, 0,
                    -s, c, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1);
      }

      static matr RotateY( Type AngleInDegree )
      {
        DBL a = D2R(AngleInDegree), c = cos(a), s = sin(a);
        return matr(c, 0, -s, 0,
                    0, 1, 0, 0,
                    s, 0, c, 0,
                    0, 0, 0, 1);
      }

      static matr RotateX( Type AngleInDegree )
      {
        DBL a = D2R(AngleInDegree), c = cos(a), s = sin(a);
        return matr(1, 0, 0, 0,
                    0, c, s, 0,
                    0, -s, c, 0,
                    0, 0, 0, 1);
      }

      static matr Translate( const vec3<Type> t )
      {
        matr m = Identity();
        m.A[3][0] = t.X;
        m.A[3][1] = t.Y;
        m.A[3][2] = t.Z;
        return m;
      }

      static matr Scale( const vec3<Type> s )
      {
        matr m = Identity();
        m.A[0][0] = s.X;
        m.A[1][1] = s.Y;
        m.A[2][2] = s.Z;
        return m;
      }

      static matr Rotate( Type AngleInDegree, const vec3<Type> r )
      {
        DBL a = D2R(AngleInDegree), c = cos(a), s = sin(a);
        vec3<Type> R = r.Normalizing();
        matr m(c + R.X * R.X * (1 - c), R.X * R.Y * (1 - c) + R.Z * s, R.X * R.Z * (1 - c) - R.Y * s, 0,
               R.Y * R.X * (1 - c) - R.Z * s, c + R.Y * R.Y * (1 - c), R.Y * R.Z * (1 - c) + R.X * s, 0,
               R.Z * R.X * (1 - c) + R.Y * s, R.Z * R.Y * (1 - c) - R.X * s, c + R.Z * R.Z * (1 - c), 0,
               0, 0, 0, 1);

        return m;
      }

      matr<Type> operator*( const matr<Type> m1 ) const
      {
        matr<Type> m;
        INT k;

        for (INT i = 0; i < 4; i++)
          for (INT j = 0; j < 4; j++)
            for (k = 0, m.A[i][j] = 0; k < 4; k++)
              m.A[i][j] += A[i][k] * m1.A[k][j];
        return m;
      }

      matr & Transpose( VOID )
      {
        for (INT i = 0; i < 4; i++)
          for (INT j = 0; j < 4; j++)
            Swap(&A[i][j], &A[j][i]);
      }

      matr Transposing( VOID ) const
      {
        matr m;

        for (INT i = 0; i < 4; i++)
          for (INT j = 0; j < 4; j++)
            m.A[i][j] = A[j][i];
        return m;
      }

      static Type Determ3x3( Type A11, Type A12, Type A13,
                             Type A21, Type A22, Type A23,
                             Type A31, Type A32, Type A33)
      {
        return A11 * A22 * A33 - A11 * A23 * A32 - A12 * A21 * A33 +
          A12 * A23 * A31 + A13 * A21 * A32 - A13 * A22 * A31;
      }

      Type operator!( VOID )
      {
        return
          A[0][0] * Determ3x3(A[1][1], A[1][2], A[1][3],
                              A[2][1], A[2][2], A[2][3],
                              A[3][1], A[3][2], A[3][3]) +
          -A[0][1] * Determ3x3(A[1][0], A[1][2], A[1][3],
                               A[2][0], A[2][2], A[2][3],
                               A[3][0], A[3][2], A[3][3]) +
          A[0][2] * Determ3x3(A[1][0], A[1][1], A[1][3],
                              A[2][0], A[2][1], A[2][3],
                              A[3][0], A[3][1], A[3][3]) +
          -A[0][3] * Determ3x3(A[1][0], A[1][1], A[1][2],
                               A[2][0], A[2][1], A[2][2],
                               A[3][0], A[3][1], A[3][2]);
      }

      matr Inverse( VOID )
      {
        matr r;
        Type det = !*this;


        r.M[0][0] =
          Determ3x3(A[1][1], A[1][2], A[1][3],
                    A[2][1], A[2][2], A[2][3],
                    A[3][1], A[3][2], A[3][3]);
        r.M[1][0] =
          -Determ3x3(A[1][0], A[1][2], A[1][3],
                     A[2][0], A[2][2], A[2][3],
                     A[3][0], A[3][2], A[3][3]);
        r.M[2][0] =
          Determ3x3(A[1][0], A[1][1], A[1][3],
                    A[2][0], A[2][1], A[2][3],
                    A[3][0], A[3][1], A[3][3]);
        r.M[3][0] =
          -Determ3x3(A[1][0], A[1][1], A[1][2],
                     A[2][0], A[2][1], A[2][2],
                     A[3][0], A[3][1], A[3][2]);
 
        r.M[0][1] =
          -Determ3x3(A[0][1], A[0][2], A[0][3],
                     A[3][1], A[3][2], A[3][3]);
                     A[2][1], A[2][2], A[2][3],
        r.M[1][1] =
          Determ3x3(A[0][0], A[0][2], A[0][3],
                    A[2][0], A[2][2], A[2][3],
                    A[3][0], A[3][2], A[3][3]);
        r.M[2][1] =
          -Determ3x3(A[0][0], A[0][1], A[0][3],
                     A[2][0], A[2][1], A[2][3],
                     A[3][0], A[3][1], A[3][3]);
        r.M[3][1] =
          Determ3x3(A[0][0], A[0][1], A[0][2],
                    A[2][0], A[2][1], A[2][2],
                    A[3][0], A[3][1], A[3][2]);
 
        r.M[0][2] =
          Determ3x3(A[0][1], A[0][2], A[0][3],
                    A[1][1], A[1][2], A[1][3],
                    A[3][1], A[3][2], A[3][3]);
        r.M[1][2] =
          -Determ3x3(A[0][0], A[0][2], A[0][3],
                     A[1][0], A[1][2], A[1][3],
                     A[3][0], A[3][2], A[3][3]);
        r.M[2][2] =
          Determ3x3(A[0][0], A[0][1], A[0][3],
                    A[1][0], A[1][1], A[1][3],
                    A[3][0], A[3][1], A[3][3]);
        r.M[3][2] =
          -Determ3x3(A[0][0], A[0][1], A[0][2],
                     A[1][0], A[1][1], A[1][2],
                     A[3][0], A[3][1], A[3][2]);
 
        r.M[0][3] =
          -Determ3x3(A[0][1], A[0][2], A[0][3],
                     A[1][1], A[1][2], A[1][3],
                     A[2][1], A[2][2], A[2][3]);
        r.M[1][3] =
          Determ3x3(A[0][0], A[0][2], A[0][3],
                    A[1][0], A[1][2], A[1][3],
                    A[2][0], A[2][2], A[2][3]);
        r.M[2][3] =
          -Determ3x3(A[0][0], A[0][1], A[0][3],
                     A[1][0], A[1][1], A[1][3],
                     A[2][0], A[2][1], A[2][3]);
        r.M[3][3] =
          Determ3x3(A[0][0], A[0][1], A[0][2],
                    A[1][0], A[1][1], A[1][2],
                    A[2][0], A[2][1], A[2][2]);

        r.A[0][0] /= det, r.A[0][1] /= det, r.A[0][2] /= det, r.A[0][3] /= det;
        r.A[1][0] /= det, r.A[1][1] /= det, r.A[1][2] /= det, r.A[1][3] /= det;
        r.A[2][0] /= det, r.A[2][1] /= det, r.A[2][2] /= det, r.A[2][3] /= det;
        r.A[3][0] /= det, r.A[3][1] /= det, r.A[3][2] /= det, r.A[3][3] /= det;

        return r;
      }

      vec3<Type> TransformNormal( const vec3<Type> &N ) const
      {
        EvaluateInverseMatrix();
        return vec3<Type>( N.X * InvA[0][0] + N.Y * InvA[0][1] + N.Z * InvA[0][2],
                           N.X * InvA[1][0] + N.Y * InvA[1][1] + N.Z * InvA[1][2],
                           N.X * InvA[2][0] + N.Y * InvA[2][1] + N.Z * InvA[2][2] );
      }

      static matr<Type> Frustum( Type L, Type R, Type B, Type T, Type N, Type F )
      {
        return matr<Type>( 2 * N / (R - L), 0, 0, 0,
          0, 2 * N / (T - B), 0, 0,
          (R + L) / (R - L), (T + B) / (T - B), -(F + N) / (F - N), -1,
          0, 0, -2 * N * F / (F - N), 0 );
      }

      static matr Ortho( Type L, Type R, Type B, Type T, Type N, Type F )
      {
        return matr(2 / (R - L),                 0,                             0,               0,
                    0,                           2 / (T - B),                   0,               0,
                    0,                           0,                            -2 / (F - N),     0,
                   -(R + L) / (R - L),          -(T + B) / (T - B),          -(N + F) / (F - N), 1);
      }

      static matr View( const vec3<Type> Loc, const vec3<Type> At, const vec3<Type> Up1 )
      {
        vec3<Type> D, U, R;
        D = At - Loc, R = D % Up1, U = R % D;
        D.Normalize(), R.Normalize();
        return matr(R.X, U.X, -D.X, 0,
                    R.Y, U.Y, -D.Y, 0,
                    R.Z, U.Z, -D.Z, 0,
                    -(Loc & R), -(Loc & U), (Loc & D), 1);
      }

      operator Type *(VOID)
      {
        return A[0];
      }

      /* Return vector pointer.
       * ARGUMENTS: None.
       * RETURNS:
       *   (Type *) vector pointer.
       */
      operator const Type *(VOID) const
      {
        return A[0];
      } /* End of 'operator' function */
  };
}

#endif // __mth_matr_h_


/* END OF 'mth_matr.h' FILE */
