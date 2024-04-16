/* FILENAME:    ctrl.h
 * LAST UPDATE: 24.07.2021
 * PROGRAMMER:  DK6
 * PURPOSE:     ctrl unit (active element).
 */

#ifndef __ctrl_h_
#define __ctrl_h_

#include "../../anim/anim.h"

namespace degl
{
  class ctrl : public unit
  {
  public:
    ctrl( VOID )
    {
    }

    /* The class constructor.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    ctrl( degl::anim *Ani )
    {
    } /* End of 'scene' function */

    /* The class destructor.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    ~ctrl( VOID ) override
    {
    } /* End of '~scene' function */

    /* Unit self interaction function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Response( degl::anim *Ani ) override
    {
      if (Ani->Keys['W'] || Ani->Keys['A'] || Ani->Keys['S'] || Ani->Keys['D'])
        Ani->Cam.Move((vec3(Ani->Cam.Dir[0], 0, Ani->Cam.Dir[2]) * ((Ani->Keys['W'] - Ani->Keys['S'])) + 
                          vec3(Ani->Cam.Right[0], 0, Ani->Cam.Right[2]) * ((Ani->Keys['D'] - Ani->Keys['A']))) * 20 * Ani->GlobalDeltaTime);

      if (Ani->Keys[VK_CONTROL] || Ani->Keys[VK_SPACE])
        Ani->Cam.Move(vec3(0, 20 * Ani->GlobalDeltaTime, 0) * (Ani->Keys[VK_SPACE] - Ani->Keys[VK_CONTROL]));

      if (Ani->Keys[VK_RBUTTON])
      {
        FLT Hp, Wp = Hp = Ani->Cam.ProjDist;

        if (Ani->Cam.FrameW > Ani->Cam.FrameH)
          Wp *= static_cast<FLT>(Ani->Cam.FrameW) / Ani->Cam.FrameH;
        else
          Hp *= static_cast<FLT>(Ani->Cam.FrameH) / Ani->Cam.FrameW;

        FLT Dist = !(Ani->Cam.At - Ani->Cam.Loc);

        FLT sx = -Ani->Dx * Wp / Ani->Cam.FrameW * Dist / Ani->Cam.ProjDist;
        FLT sy = Ani->Dy * Hp / Ani->Cam.FrameH * Dist / Ani->Cam.ProjDist;

        Ani->Cam.Move(Ani->Cam.Right * sx +
                          Ani->Cam.Up * sy);
      }

      if (!Ani->Keys[VK_RSHIFT] || Ani->Keys[VK_LEFT] || Ani->Keys[VK_RIGHT])
        Ani->Cam.Rotate(vec3(0, 1, 0), (-Ani->Dx +
                                           (Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT])) * Ani->GlobalDeltaTime * 90);

      if (!Ani->Keys[VK_RSHIFT] || Ani->Keys[VK_UP] || Ani->Keys[VK_DOWN])
        Ani->Cam.Rotate(Ani->Cam.Right, (-Ani->Dy +
                                               (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN])) * Ani->GlobalDeltaTime * 30);

      SetCursorPos(300, 300);
      Ani->InputResponse();
    } /* End of 'Response' function */

    /* Specific function for unit render.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Render( degl::anim *Ani ) override
    {
    } /* End of 'Render' function */
  };
}


#endif // __ctrl_h_
