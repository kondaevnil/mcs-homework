/* FILE NAME  : mth_matr.h
 * PROGRAMMER : DK66
 * LAST UPDATE: 20.07.2021
 * PURPOSE    : Math matrix module.
 */

#ifndef __mth_matr_h_
#define __mth_matr_h_

#include "mthdef.h"

 /* Space math namespace */
namespace mth
{
  template<class Type>
  class matr_data
  {
  public:
    Type A[4][4]; /* Matrix data */

    /* Matrix constructor.
     * ARGUMENTS: None.
     */
    matr_data( void )
    {
    } /* End of 'matr' function */
  };

  template<class Type>
  class matr : public matr_data<Type>
  {
  private:
    mutable Type InvA[4][4];         /* Inversed matrix */
    mutable BOOL IsInverseEvaluated; /* Inverse matrix evaluated flag */

    /* Evaluate inverse matrix function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID EvaluateInverseMatrix( VOID ) const
    {
      if (IsInverseEvaluated)
        return;
      IsInverseEvaluated = TRUE;

      Type det = !*this;

      /* build adjoint matrix */
      InvA[0][0] =
        MatrDeterm3x3( matr_data<Type>::A[1][1], matr_data<Type>::A[1][2], matr_data<Type>::A[1][3],
          matr_data<Type>::A[2][1], matr_data<Type>::A[2][2], matr_data<Type>::A[2][3],
          matr_data<Type>::A[3][1], matr_data<Type>::A[3][2], matr_data<Type>::A[3][3] );
      InvA[1][0] =
        -MatrDeterm3x3( matr_data<Type>::A[1][0], matr_data<Type>::A[1][2], matr_data<Type>::A[1][3],
          matr_data<Type>::A[2][0], matr_data<Type>::A[2][2], matr_data<Type>::A[2][3],
          matr_data<Type>::A[3][0], matr_data<Type>::A[3][2], matr_data<Type>::A[3][3] );
      InvA[2][0] =
        MatrDeterm3x3( matr_data<Type>::A[1][0], matr_data<Type>::A[1][1], matr_data<Type>::A[1][3],
          matr_data<Type>::A[2][0], matr_data<Type>::A[2][1], matr_data<Type>::A[2][3],
          matr_data<Type>::A[3][0], matr_data<Type>::A[3][1], matr_data<Type>::A[3][3] );
      InvA[3][0] =
        -MatrDeterm3x3( matr_data<Type>::A[1][0], matr_data<Type>::A[1][1], matr_data<Type>::A[1][2],
          matr_data<Type>::A[2][0], matr_data<Type>::A[2][1], matr_data<Type>::A[2][2],
          matr_data<Type>::A[3][0], matr_data<Type>::A[3][1], matr_data<Type>::A[3][2] );

      InvA[0][1] =
        -MatrDeterm3x3( matr_data<Type>::A[0][1], matr_data<Type>::A[0][2], matr_data<Type>::A[0][3],
          matr_data<Type>::A[2][1], matr_data<Type>::A[2][2], matr_data<Type>::A[2][3],
          matr_data<Type>::A[3][1], matr_data<Type>::A[3][2], matr_data<Type>::A[3][3] );
      InvA[1][1] =
        MatrDeterm3x3( matr_data<Type>::A[0][0], matr_data<Type>::A[0][2], matr_data<Type>::A[0][3],
          matr_data<Type>::A[2][0], matr_data<Type>::A[2][2], matr_data<Type>::A[2][3],
          matr_data<Type>::A[3][0], matr_data<Type>::A[3][2], matr_data<Type>::A[3][3] );
      InvA[2][1] =
        -MatrDeterm3x3( matr_data<Type>::A[0][0], matr_data<Type>::A[0][1], matr_data<Type>::A[0][3],
          matr_data<Type>::A[2][0], matr_data<Type>::A[2][1], matr_data<Type>::A[2][3],
          matr_data<Type>::A[3][0], matr_data<Type>::A[3][1], matr_data<Type>::A[3][3] );
      InvA[3][1] =
        MatrDeterm3x3( matr_data<Type>::A[0][0], matr_data<Type>::A[0][1], matr_data<Type>::A[0][2],
          matr_data<Type>::A[2][0], matr_data<Type>::A[2][1], matr_data<Type>::A[2][2],
          matr_data<Type>::A[3][0], matr_data<Type>::A[3][1], matr_data<Type>::A[3][2] );

      InvA[0][2] =
        MatrDeterm3x3( matr_data<Type>::A[0][1], matr_data<Type>::A[0][2], matr_data<Type>::A[0][3],
          matr_data<Type>::A[1][1], matr_data<Type>::A[1][2], matr_data<Type>::A[1][3],
          matr_data<Type>::A[3][1], matr_data<Type>::A[3][2], matr_data<Type>::A[3][3] );
      InvA[1][2] =
        -MatrDeterm3x3( matr_data<Type>::A[0][0], matr_data<Type>::A[0][2], matr_data<Type>::A[0][3],
          matr_data<Type>::A[1][0], matr_data<Type>::A[1][2], matr_data<Type>::A[1][3],
          matr_data<Type>::A[3][0], matr_data<Type>::A[3][2], matr_data<Type>::A[3][3] );
      InvA[2][2] =
        MatrDeterm3x3( matr_data<Type>::A[0][0], matr_data<Type>::A[0][1], matr_data<Type>::A[0][3],
          matr_data<Type>::A[1][0], matr_data<Type>::A[1][1], matr_data<Type>::A[1][3],
          matr_data<Type>::A[3][0], matr_data<Type>::A[3][1], matr_data<Type>::A[3][3] );
      InvA[3][2] =
        -MatrDeterm3x3( matr_data<Type>::A[0][0], matr_data<Type>::A[0][1], matr_data<Type>::A[0][2],
          matr_data<Type>::A[1][0], matr_data<Type>::A[1][1], matr_data<Type>::A[1][2],
          matr_data<Type>::A[3][0], matr_data<Type>::A[3][1], matr_data<Type>::A[3][2] );

      InvA[0][3] =
        -MatrDeterm3x3( matr_data<Type>::A[0][1], matr_data<Type>::A[0][2], matr_data<Type>::A[0][3],
          matr_data<Type>::A[1][1], matr_data<Type>::A[1][2], matr_data<Type>::A[1][3],
          matr_data<Type>::A[2][1], matr_data<Type>::A[2][2], matr_data<Type>::A[2][3] );
      InvA[1][3] =
        MatrDeterm3x3( matr_data<Type>::A[0][0], matr_data<Type>::A[0][2], matr_data<Type>::A[0][3],
          matr_data<Type>::A[1][0], matr_data<Type>::A[1][2], matr_data<Type>::A[1][3],
          matr_data<Type>::A[2][0], matr_data<Type>::A[2][2], matr_data<Type>::A[2][3] );
      InvA[2][3] =
        -MatrDeterm3x3( matr_data<Type>::A[0][0], matr_data<Type>::A[0][1], matr_data<Type>::A[0][3],
          matr_data<Type>::A[1][0], matr_data<Type>::A[1][1], matr_data<Type>::A[1][3],
          matr_data<Type>::A[2][0], matr_data<Type>::A[2][1], matr_data<Type>::A[2][3] );
      InvA[3][3] =
        MatrDeterm3x3( matr_data<Type>::A[0][0], matr_data<Type>::A[0][1], matr_data<Type>::A[0][2],
          matr_data<Type>::A[1][0], matr_data<Type>::A[1][1], matr_data<Type>::A[1][2],
          matr_data<Type>::A[2][0], matr_data<Type>::A[2][1], matr_data<Type>::A[2][2] );
      /* divide by determinant */
      InvA[0][0] /= det;
      InvA[0][1] /= det;
      InvA[0][2] /= det;
      InvA[0][3] /= det;

      InvA[1][0] /= det;
      InvA[1][1] /= det;
      InvA[1][2] /= det;
      InvA[1][3] /= det;

      InvA[2][0] /= det;
      InvA[2][1] /= det;
      InvA[2][2] /= det;
      InvA[2][3] /= det;

      InvA[3][0] /= det;
      InvA[3][1] /= det;
      InvA[3][2] /= det;
      InvA[3][3] /= det;
    } /* End of 'EvaluateInverseMatrix' function */

  public:
    /* Matrix constructor.
     * ARGUMENTS: None.
     */
    matr( void ) : IsInverseEvaluated( false )
    {
    } /* End of 'matr' function */

    /* Matrix constructor.
     * ARGUMENTS:
     *   - Matrix values:
     *       Type a00, Type a01, Type a02, Type a03,
     *       Type a10, Type a11, Type a12, Type a13,
     *       Type a20, Type a21, Type a22, Type a23,
     *       Type a30, Type a31, Type a32, Type a33;
     */
    matr( Type a00, Type a01, Type a02, Type a03,
      Type a10, Type a11, Type a12, Type a13,
      Type a20, Type a21, Type a22, Type a23,
      Type a30, Type a31, Type a32, Type a33 ) : IsInverseEvaluated( false )
    {
      matr_data<Type>::A[0][0] = a00, matr_data<Type>::A[0][1] = a01, matr_data<Type>::A[0][2] = a02, matr_data<Type>::A[0][3] = a03;
      matr_data<Type>::A[1][0] = a10, matr_data<Type>::A[1][1] = a11, matr_data<Type>::A[1][2] = a12, matr_data<Type>::A[1][3] = a13;
      matr_data<Type>::A[2][0] = a20, matr_data<Type>::A[2][1] = a21, matr_data<Type>::A[2][2] = a22, matr_data<Type>::A[2][3] = a23;
      matr_data<Type>::A[3][0] = a30, matr_data<Type>::A[3][1] = a31, matr_data<Type>::A[3][2] = a32, matr_data<Type>::A[3][3] = a33;
    } /* End of matr constructor */

    /* Transorm normal function.
     * ARGUMENTS:
     *   - vector to be transoformed:
     *       const vec3<Type> &N;
     * RETURNS:
     *   (vec3<Type>) result vector.
     */
    vec3<Type> TransformNormal( const vec3<Type> &N ) const
    {
      EvaluateInverseMatrix();
      return vec3<Type>( N.X * InvA[0][0] + N.Y * InvA[0][1] + N.Z * InvA[0][2],
        N.X * InvA[1][0] + N.Y * InvA[1][1] + N.Z * InvA[1][2],
        N.X * InvA[2][0] + N.Y * InvA[2][1] + N.Z * InvA[2][2] );
    } /* End of 'TransformNormal' function */

    /* Get matr identity function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (matr) identity matrix.
     */
    static matr<Type> Identity( VOID )
    {
      return matr<Type>( 1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1 );
    } /* End of 'MatrIdentity' function */

    /* Translate matrix function.
     * ARGUMENTS:
     *   - vector to be translated:
     *       const vec3<Type> &T;
     * RETURNS:
     *   (matr) result matrix.
     */
    static matr<Type> Translate( const vec3<Type> &T )
    {
      return matr<Type>( 1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        T.X, T.Y, T.Z, 1 );
    } /* End of 'Translate' function */

    /* Normalized device coordinates function.
     * ARGUMENTS:
     *   - Screen factors:
     *      Left and Right:
     *        FLT L, R;
     *      Bottom and Top:
     *        FLT B, T;
     *      Near and Far:
     *        FLT N, F;
     * RETURNS:
     *   (MATR) result matrix.
     */
    static matr<Type> Frustum( Type L, Type R, Type B, Type T, Type N, Type F )
    {
      return matr<Type>( 2 * N / (R - L), 0, 0, 0,
        0, 2 * N / (T - B), 0, 0,
        (R + L) / (R - L), (T + B) / (T - B), -(F + N) / (F - N), -1,
        0, 0, -2 * N * F / (F - N), 0 );
    } /* End of 'Frustum' function */

    /* Ortho projection matrix function.
     * ARGUMENTS:
     *   - Screen factors:
     *      Left and Right:
     *        FLT L, R;
     *      Bottom and Top:
     *        FLT B, T;
     *      Near and Far:
     *        FLT N, F;
     * RETURNS:
     *   (MATR) viewer matrix.
     */
    static matr<Type> Ortho( Type L, Type R, Type B, Type T, Type N, Type F )
    {
      return matr<Type>( 2 / (R - L), 0, 0, 0,
        0, 2 / (T - B), 0, 0,
        0, 0, -2 / (F - N), 0,
        -(R + L) / (R - L), -(T + B) / (T - B), -(N + F) / (F - N), 1 );
    } /* End of 'Ortho' function */

    /* Matrix look at viwer setup function.
     * ARGUMENTS:
     *   - Positoin:
     *      VEC Loc;
     *   - Where we looking for:
     *      VEC At;
     *   - Direction to up:
     *      VEC Up1;
     * RETURNS:
     *   (MATR) result matrix.
     */
    static matr<Type> View( vec3<Type> Loc, vec3<Type> At, vec3<Type> Up1 )
    {
      vec3<Type>
        D, /* Dir - direction */
        U, /* Up */
        R; /* Right */

      D = ~(At - Loc);
      R = ~(D % Up1);
      U = R % D;

      return matr<Type>( R.X, U.X, -D.X, 0,
        R.Y, U.Y, -D.Y, 0,
        R.Z, U.Z, -D.Z, 0,
        -(Loc & R), -(Loc & U), Loc & D, 1 );
    } /* End of 'View' function */

    /* Transpose matrix function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (matr) result matrix.
     */
    matr<Type> Transpose( VOID ) const
    {
      return matr<Type>( matr_data<Type>::A[0][0], matr_data<Type>::A[1][0], matr_data<Type>::A[2][0], matr_data<Type>::A[3][0],
        matr_data<Type>::A[0][1], matr_data<Type>::A[1][1], matr_data<Type>::A[2][1], matr_data<Type>::A[3][1],
        matr_data<Type>::A[0][2], matr_data<Type>::A[1][2], matr_data<Type>::A[2][2], matr_data<Type>::A[3][2],
        matr_data<Type>::A[0][3], matr_data<Type>::A[1][3], matr_data<Type>::A[2][3], matr_data<Type>::A[3][3] );
    } /* End of 'Transpose' function */

    /* Mulpiply matrixes function.
     * ARGUMENTS:
     *   - matrix to be multyplied:
     *       const matr &M;
     * RETURNS:
     *   (matr) result matrix.
     */
    matr<Type> operator*( const matr<Type> &M ) const
    {
      matr<Type> r;
      INT k;

      for (INT i = 0; i < 4; i++)
        for (INT j = 0; j < 4; j++)
          for (k = 0, r.matr_data<Type>::A[i][j] = 0; k < 4; k++)
            r.matr_data<Type>::A[i][j] += matr_data<Type>::A[i][k] * M.matr_data<Type>::A[k][j];
      return r;
    } /* End of 'operator*' function */

    /* Calculate matrix 3x3 determinant
     * ARGUMENTS:
     *   - matrix:
     *       Type A11, Type A12, Type A13,
     *       Type A21, Type A22, Type A23,
     *       Type A31, Type A32, Type A33
     * RETURNS:
     *   (Type) matrix determinant.
     */
    static Type MatrDeterm3x3( Type A11, Type A12, Type A13,
      Type A21, Type A22, Type A23,
      Type A31, Type A32, Type A33 )
    {
      return A11 * A22 * A33 - A11 * A23 * A32 - A12 * A21 * A33 +
        A12 * A23 * A31 + A13 * A21 * A32 - A13 * A22 * A31;
    } /* End of 'MatrDeterm3x3' function */

    /* Calculate matrix determinant.
     * ARGUMENTS: None.
     * RETURNS:
     *   (Type) matrix determinant.
     */
    Type operator!( VOID ) const
    {
      return
        matr_data<Type>::A[0][0] * MatrDeterm3x3( matr_data<Type>::A[1][1], matr_data<Type>::A[1][2], matr_data<Type>::A[1][3],
          matr_data<Type>::A[2][1], matr_data<Type>::A[2][2], matr_data<Type>::A[2][3],
          matr_data<Type>::A[3][1], matr_data<Type>::A[3][2], matr_data<Type>::A[3][3] ) +
        -matr_data<Type>::A[0][1] * MatrDeterm3x3( matr_data<Type>::A[1][0], matr_data<Type>::A[1][2], matr_data<Type>::A[1][3],
          matr_data<Type>::A[2][0], matr_data<Type>::A[2][2], matr_data<Type>::A[2][3],
          matr_data<Type>::A[3][0], matr_data<Type>::A[3][2], matr_data<Type>::A[3][3] ) +
        matr_data<Type>::A[0][2] * MatrDeterm3x3( matr_data<Type>::A[1][0], matr_data<Type>::A[1][1], matr_data<Type>::A[1][3],
          matr_data<Type>::A[2][0], matr_data<Type>::A[2][1], matr_data<Type>::A[2][3],
          matr_data<Type>::A[3][0], matr_data<Type>::A[3][1], matr_data<Type>::A[3][3] ) +
        -matr_data<Type>::A[0][3] * MatrDeterm3x3( matr_data<Type>::A[1][0], matr_data<Type>::A[1][1], matr_data<Type>::A[1][2],
          matr_data<Type>::A[2][0], matr_data<Type>::A[2][1], matr_data<Type>::A[2][2],
          matr_data<Type>::A[3][0], matr_data<Type>::A[3][1], matr_data<Type>::A[3][2] );
    } /* End of 'operator!' function */

    /* Evaluate inverse matrix.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Inverse( VOID ) const
    {
      EvaluateInverseMatrix();
    } /* End of 'MatrInverse' function */

    /* Return matr scale.
     * ARGUMENTS:
     *   - vector:
     *       const vec3<Type> &S;
     * RETURNS:
     *   (matr) scale matrix.
     */
    static matr<Type> Scale( const vec3<Type> &S )
    {
      return matr<Type>( S.X, 0, 0, 0,
        0, S.Y, 0, 0,
        0, 0, S.Y, 0,
        0, 0, 0, 1 );
    } /* End of 'MatrScale' function */

    /* Rotate matrix function.
     * ARGUMENTS:
     *   - angle in degrees:
     *       Type AngleInDegree;
     *   - vector to matrix:
     *       vec3 T;
     * RETURNS:
     *   (matr) result matrix.
     */
    static matr<Type> Rotate( vec3<Type> V, Type AngleInDegree )
    {
      FLT a = Deg2Rad( AngleInDegree ), c = cos( a ), s = sin( a );
      V.Normalize();

      return matr<Type>( c + V.X * V.X * (1 - c), V.X * V.Y * (1 - c) + V.Z * s, V.X * V.Z * (1 - c) - V.Y * s, 0,
        V.Y * V.X * (1 - c) - V.Z * s, c + V.Y * V.Y * (1 - c), V.Y * V.Z * (1 - c) + V.X * s, 0,
        V.Z * V.X * (1 - c) + V.Y * s, V.Z * V.Y * (1 - c) - V.X * s, c + V.Z * V.Z * (1 - c), 0,
        0, 0, 0, 1 );
    } /* End of 'MatrRotate' function */

    /* Rotate matrix by X axis function.
     * ARGUMENTS:
     *   - angle in degrees:
     *       Type AngleInDegree;
     * RETURNS:
     *   (matr) result matrix.
     */
    static matr<Type> RotateX( Type AngleInDegree )
    {
      FLT a = Deg2Rad( AngleInDegree ), c = cos( a ), s = sin( a );

      return matr<Type>( 1, 0, 0, 0,
        0, c, s, 0,
        0, -s, c, 0,
        0, 0, 0, 1 );
    } /* End of 'MatrRotateX' function */

    /* Rotate matrix by Y axis function.
     * ARGUMENTS:
     *   - angle in degrees:
     *       Type AngleInDegree;
     * RETURNS:
     *   (matr) result matrix.
     */
    static matr<Type> RotateY( Type AngleInDegree )
    {
      FLT a = Deg2Rad( AngleInDegree ), c = cos( a ), s = sin( a );

      return matr<Type>( c, 0, -s, 0,
        0, 1, 0, 0,
        s, 0, c, 0,
        0, 0, 0, 1 );
    } /* End of 'MatrRotateY' function */

    /* Rotate matrix by Z axis function.
     * ARGUMENTS:
     *   - angle in degrees:
     *       FLT AngleInDegree;
     * RETURNS:
     *   (matr) result matrix.
     */
    static matr<Type> RotateZ( Type AngleInDegree )
    {
      FLT a = Deg2Rad( AngleInDegree ), c = cos( a ), s = sin( a );

      return matr<Type>( c, s, 0, 0,
        -s, c, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1 );
    } /* End of 'MatrRotateZ' function */

    /* Transform point position.
     * ARGUMENTS:
     *   - vectors to be dot multiplied:
     *       vec3 V;
     * RETURNS:
     *   (vec3) result vector.
     */
    vec3<Type> TransformPoint( const vec3<Type> &V ) const
    {
      return vec3<Type>( V.X * matr_data<Type>::A[0][0] + V.Y * matr_data<Type>::A[1][0] + V.Z * matr_data<Type>::A[2][0] + matr_data<Type>::A[3][0],
        V.X * matr_data<Type>::A[0][1] + V.Y * matr_data<Type>::A[1][1] + V.Z * matr_data<Type>::A[2][1] + matr_data<Type>::A[3][1],
        V.X * matr_data<Type>::A[0][2] + V.Y * matr_data<Type>::A[1][2] + V.Z * matr_data<Type>::A[2][2] + matr_data<Type>::A[3][2] );
    } /* End of 'TransformPoint' function */

    /* Transform point position.
     * ARGUMENTS:
     *   - vectors to be dot multiplied:
     *       vec3 V;
     * RETURNS:
     *   (vec3) result vector.
     */
    vec3<Type> TransformVector( const vec3<Type> &V ) const
    {
      return vec3<Type>( V.X * matr_data<Type>::A[0][0] + V.Y * matr_data<Type>::A[1][0] + V.Z * matr_data<Type>::A[2][0],
        V.X * matr_data<Type>::A[0][1] + V.Y * matr_data<Type>::A[1][1] + V.Z * matr_data<Type>::A[2][1],
        V.X * matr_data<Type>::A[0][2] + V.Y * matr_data<Type>::A[1][2] + V.Z * matr_data<Type>::A[2][2] );
    } /* End of 'TransformPoint' function */

    /* Multiply matrix and vector.
     * ARGUMENTS:
     *   - multiplyong vector:
     *       vec3 V;
     * RETURNS:
     *   (matr) result matrix.
     */
    vec3<Type> Transform4x4( const vec3<Type> &V ) const
    {
      FLT w = V.X * matr_data<Type>::A[0][3] + V.Y * matr_data<Type>::A[1][3] + V.Z * matr_data<Type>::A[2][3] + matr_data<Type>::A[3][3];

      return vec3<Type>( (V.X * matr_data<Type>::A[0][0] + V.Y * matr_data<Type>::A[1][0] + V.Z * matr_data<Type>::A[2][0] + matr_data<Type>::A[3][0]) / w,
        (V.X * matr_data<Type>::A[0][1] + V.Y * matr_data<Type>::A[1][1] + V.Z * matr_data<Type>::A[2][1] + matr_data<Type>::A[3][1]) / w,
        (V.X * matr_data<Type>::A[0][2] + V.Y * matr_data<Type>::A[1][2] + V.Z * matr_data<Type>::A[2][2] + matr_data<Type>::A[3][2]) / w );
    } /* End of 'VecMulMatr' function */

          /* Return vector pointer.
     * ARGUMENTS: None.
     * RETURNS:
     *  (Type *) vector pointer.
     */
    operator Type *(VOID)
    {
      return matr_data<Type>::A[0];
    } /* End of 'operator' function */

    /* Return vector pointer.
     * ARGUMENTS: None.
     * RETURNS:
     *   (Type *) vector pointer.
     */
    operator const Type *(VOID) const
    {
      return matr_data<Type>::A[0];
    } /* End of 'operator' function */
  };

} /* end of 'mth' namespace */

#endif /* __mth_matr_h_ */

/* END OF 'mth_matr.h' FILE */
