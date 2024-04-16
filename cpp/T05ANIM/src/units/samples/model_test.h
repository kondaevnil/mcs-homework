/* FILENAME:    test_unit.h
 * LAST UPDATE: 24.07.2021
 * PROGRAMMER:  DK6
 * PURPOSE:     Animation unit (active element).
 */

#ifndef __model_test_h_
#define __model_test_h_

#include "../../anim/anim.h"

namespace degl
{
  class model_test : public unit
  {
  private:
    prims *model;

  public:
    /* The class constructor.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    model_test( degl::anim *Ani )
    {
      model = Ani->ModelCreate("./bin/models/btr.g3dm");
      model->SetTrans(mth::matr<FLT>::Scale(vec3(1)));
    } /* End of 'scene' function */

    /* The class destructor.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    ~model_test( VOID ) override
    {
      anim::Get().ModelFree(model);
    } /* End of '~scene' function */

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
      model->Draw();
    } /* End of 'Render' function */
  };
}

#endif // !__model_test_h_

/* END OF 'model_test.h' FILE */
