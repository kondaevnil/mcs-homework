/* FILENAME:    mth_matr.h
 * LAST UPDATE: 24.07.2021
 * PROGRAMMER:  DK6
 * PURPOSE:     camera realization.
 */

#ifndef __mth_camera_h_
#define __mth_camera_h_

#include "mth_matr.h"

namespace mth
{
  /* Space camera handle class */
  template<class type>
  class camera
  {
  private:
    /* Update camera project parameters function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID UpdateProj( VOID )
    {
      type ratio_x = Size / 2, ratio_y = Size / 2;

      if (FrameW >= FrameH)
        ratio_x *= (type)FrameW / FrameH;
      else
        ratio_y *= (type)FrameH / FrameW;
      Proj = matr<type>::Frustum( -ratio_x, ratio_x, -ratio_y, ratio_y, ProjDist, FarClip );
    } /* End of 'UpdateProj' function */

    /* Update camera view parameters function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID UpdateView( VOID )
    {
      View = matr<type>::View( Loc, At, Up );
    } /* End of 'UpdateView' function */

  public:
    vec3<type>
      Loc,            // camera location
      Dir, Up, Right, // basis camera directions
      At;             // camera pivot point
    type
      ProjDist,       // near project plane distance
      FarClip,        // far project plane distance
      Size;           // inner project plane rectangle size
    INT
      FrameW, FrameH; // Camera frame size
    matr<type>
      View,           // view matrix
      Proj,           // projection matrix
      VP;             // View and Proj madtrix production

    /* Default constructor */
    camera( VOID ) :
      Loc( 0, 0, 5 ), Dir( 0, 0, -1 ), Up( 0, 1, 0 ), Right( 1, 0, 0 ), At( 0, 0, 0 ),
      ProjDist( 0.1 ), FarClip( 500 ), Size( 0.1 ),
      FrameW( 30 ), FrameH( 30 )
    {
      UpdateProj();
      UpdateView();
      VP = View * Proj;
    } /* End of 'matr' function */

    /* Set project camera parameters function.
     * ARGUMENTS:
     *   - new near project plane size:
     *       type NewSize;
     *   - new project distance:
     *       type NewProjDist;
     *   - new far clip plane distance:
     *       type FarC;
     * RETURNS:
     *   (camera &) self reference.
     */
    camera &SetProj( type NewSize, type NewProjDist, type NewFarClip )
    {
      // Store new projection parameters
      ProjDist = NewProjDist;
      FarClip = NewFarClip;
      Size = NewSize;

      UpdateProj();
      VP = View * Proj;
      return *this;
    } /* End of 'SetProj' function */

    /* Set camera project pixel size function.
     * ARGUMENTS:
     *   - new frame size in pixels:
     *       type NewFrameW, NewFrameH;
     * RETURNS:
     *   (camera &) self reference.
     */
    camera &Resize( type NewFrameW, type NewFrameH )
    {
      FrameW = NewFrameW;
      FrameH = NewFrameH;

      UpdateProj();
      VP = View * Proj;
      return *this;
    } /* End of 'Resize' function */

    /* Set camera location and orientation function.
     * ARGUMENTS:
     *   - new camera location:
     *       const vec3<type> &L;
     *   - new camera pivot point:
     *       const vec3<type> &A;
     *   - new camera approx up direction:
     *       const vec3<type> &U;
     * RETURNS:
     *   (camera &) self reference.
     */
    camera &SetLocAtUp( const vec3<type> &L, const vec3<type> &A, const vec3<type> &U = vec3<type>( 0, 1, 0 ) )
    {
      Loc = L;
      At = A;
      Up = U;

      UpdateView();
      VP = View * Proj;
      return *this;
    } /* End of 'SetLocAtUp' function */

    /* Camera rotate function.
     * ARGUMENTS:
     *   - rotate axis:
     *       const vec3<type> &Axis;
     *   - rotation angle (in degree):
     *       type AngleInDegree;
     * RETURNS:
     *   (camera &) self reference.
     */
    camera &Rotate( const vec3<type> &Axis, type AngleInDegree )
    {
      matr<type> m = matr<type>::Translate( -At ) * matr<type>::Rotate( Axis, AngleInDegree ) * matr<type>::Translate( At );
      Loc = m.TransformPoint( Loc );
      Up = m.TransformVector( Up );
      SetLocAtUp( Loc, At, Up );
      return *this;
    } /* End of 'Rotate' function */

    /* Camera movement function.
     * ARGUMENTS:
     *   - movement directions:
     *       const vec3<type> &Direction;
     * RETURNS:
     *   (camera &) self reference.
     */
    camera &Move( const vec3<type> &Direction )
    {
      Loc += Direction;
      At += Direction;
      SetLocAtUp( Loc, At, Up );
      return *this;
    } /* End of 'Move' function */
  }; /* End of 'camera' class */
} /* end of 'mth' namespace */

#endif // __mth_camera_h_



 /* END OF 'mth_camera.h' FILE */