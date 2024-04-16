/* FILENAME:    sky.h
 * LAST UPDATE: 29.07.2021
 * PROGRAMMER:  DK6
 * PURPOSE:     sky sphere unit (active element).
 */

#ifndef __sky_h_
#define __sky_h_

#include "../../anim/anim.h"
#include <vector>                                                                       // error in defaul material!!!
                                                                                        // error in defaul material!!!
namespace degl                                                                          // error in defaul material!!!
{                                                                                       // error in defaul material!!!
  class sky : public unit                                                               // error in defaul material!!!
  {                                                                                     // error in defaul material!!!
  private:                                                                              // error in defaul material!!!
    prim *Pr;                                                                           // error in defaul material!!!
  public:                                                                               // error in defaul material!!!
    /* The class constructor.                                                           // error in defaul material!!!
     * ARGUMENTS: None.                                                                 // error in defaul material!!!
     * RETURNS: None.                                                                   // error in defaul material!!!
     */                                                                                 // error in defaul material!!!
    sky( degl::anim *Ani )                                                              // error in defaul material!!!
    {                                                                                   // error in defaul material!!!
      std::vector<vertex::std> v = {{vec3(0)}};                                         // error in defaul material!!!
      topology::base point(prim_type::POINT, v);                                        // error in defaul material!!!
      Pr = Ani->PrimCreate(point);                                                      // error in defaul material!!!
                                                                                        // error in defaul material!!!
      material::wrapper wrap("sky");                                                    // error in defaul material!!!
      Pr->SetMaterial(Ani->MaterialCreate(wrap, {"clouds.g32"}, "sky"));                // error in defaul material!!!
    } /* End of 'sky' function */

    /* The class destructor.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    ~sky( VOID ) override
    {
      anim::Get().PrimFree(Pr);
    } /* End of '~sky' function */

    /* Unit self interaction function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Response( degl::anim *Ani ) override
    {
    } /* End of 'Response' function */

    /* Specific function for unit render.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Render( degl::anim *Ani ) override
    {
      glDisable(GL_DEPTH_TEST);
      glDepthMask(FALSE);
      Ani->Draw(Pr, mth::matr<FLT>::Identity());
      glDepthMask(TRUE);
      glEnable(GL_DEPTH_TEST);
    } /* End of 'Render' function */
  };
}


#endif // __sky_h_

/* END OF 'sky.h' FILE */
