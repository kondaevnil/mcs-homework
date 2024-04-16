/* FILENAME:    test_unit.h
 * LAST UPDATE: 24.07.2021
 * PROGRAMMER:  DK6
 * PURPOSE:     Animation unit (active element).
 */

#ifndef __test_unit_h_
#define __test_unit_h_

#include "../../anim/anim.h"

namespace degl
{
  class test_unit : public unit
  {
  private:
    prim Pr;

  public:
    test_unit( VOID )
    {
      degl::vertex::std V[]
      {
        {mth::vec3<FLT>(0), mth::vec2<FLT>(0), mth::vec3<FLT>(0, 0, 1), mth::vec4<FLT>(1, 1, 1, 1)},
        {mth::vec3<FLT>(2, 0, 0), mth::vec2<FLT>(0), mth::vec3<FLT>(0, 0, 1), mth::vec4<FLT>(1, 1, 1, 1)},
        {mth::vec3<FLT>(0, 1, 0), mth::vec2<FLT>(0), mth::vec3<FLT>(0, 0, 1), mth::vec4<FLT>(1, 1, 1, 1)},
        {mth::vec3<FLT>(2, 1, 0), mth::vec2<FLT>(0), mth::vec3<FLT>(0, 0, 1), mth::vec4<FLT>(1, 1, 1, 1)},
      };

      INT I[]
      {
        0, 1, 2,
        2, 1, 3
      };

      //render::PrimCreate(&Pr, prim::prim_type::TRISTRIP, V, 4, I, 6);
      render::PrimLoad(&Pr, const_cast<CHAR *>("Z:/T05ANIM/bin/cow.obj"));
    }

    /* The class constructor.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    test_unit( degl::anim *Ani )
    {
    } /* End of 'scene' function */

    /* The class destructor.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    ~test_unit( VOID ) override
    {
      render::PrimFree(&Pr);
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
      Ani->Draw(&Pr, mth::matr<FLT>::Identity());
    } /* End of 'Render' function */
  };
}


#endif // __test_unit_h_
