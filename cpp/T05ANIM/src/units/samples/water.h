/* FILENAME:    water.h
 * LAST UPDATE: 01.08.2021
 * PROGRAMMER:  DK6
 * PURPOSE:     Animation unit (active element).
 */

/*
 * SECOND PopItka
 */

#ifndef __water_h_
#define __water_h_

#include "../../anim/anim.h"

typedef mth::vec3<FLT> vec3;
typedef mth::matr<FLT> matr;
typedef mth::intersections::plane<FLT> plane;

namespace degl
{
  class water : public unit
  {
  private:
    BOOL isRender = FALSE;
    vec3 CornerPoints[8] = 
    {
      {-1, -1, -1}, {-1, -1, 1}, {-1, 1, -1}, {-1, 1, 1},
      {1, -1, -1}, {1, 1, -1}, {1, -1, 1}, {1, 1, 1}
    };
    plane s_base, s_upper, s_lower;
    
  public:
    water( VOID )
    {
      vec3 n {0, 1, 0};
      s_base = plane(vec3(0, 0, 0), n);
      s_upper = plane(vec3(0, 1, 0), n);
      s_lower = plane(vec3(0, -1, 0), n);
    } /* End of 'water' function */

    ~water( VOID )
    {
    } /* End of '~water' function */

    /* Unit self interaction function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Response( degl::anim *Ani ) override
    {
      for (int i = 0; i < 8; i++)
        for (int j = i + 1; j < 8; j++)
        {
        }
    } /* End of 'Response' function */

    /* Specific function for unit render.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Render( degl::anim *Ani ) override
    {
    } /* End of 'Render' function */
  }; /* End of 'water' class */
} /* End of 'degl' namespace */

#endif // !__water_h_


/* END OF 'water.h' FILE */
