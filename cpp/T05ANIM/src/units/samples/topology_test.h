/* FILENAME:    topology_test.h
 * LAST UPDATE: 29.07.2021
 * PROGRAMMER:  DK6
 * PURPOSE:     Animation unit (active element).
 */

#ifndef __topology_test_h_
#define __topology_test_h_

#include "../../anim/anim.h"

namespace degl
{
  class topo_test : public unit
  {
  private:
    prim *Pr, *Pr2;

  public:
    topo_test( INT W, INT H )
    {
      /*degl::topology::grid<vertex::std> T(W, H);*/
      //Pr->Create(T);
    }

    /* The class constructor.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    topo_test( degl::anim *Ani, INT W, INT H )
    {
      //degl::topology::grid<vertex::std> T( W, H );   //
      //Pr = Ani->PrimCreate(T);
      material::wrapper a;
      //Pr->SetMaterial(Ani->MaterialCreate(a, {"brick.g32"}, "FLEX"));

      // new
      //degl::topology::Mebius A(10, 100, 20);
      degl::topology::Sphere A;
      //degl::topology::Box A(vec3(0), vec3(20, 10, 10));
      //degl::topology::Hmap A("hf.g32");
      material *mtl = Ani->MaterialCreate(a, {"brick.g32"}, "FLEX");
      Pr2 = Ani->PrimCreate(A, mtl);
      Pr2->SetMaterial(mtl);
    } /* End of 'scene' function */

    /* The class destructor.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    ~topo_test( VOID ) override
    {
      anim::Get().PrimFree(Pr2);
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
      //Ani->Draw(Pr, mth::matr<FLT>::Identity());
      Ani->Draw(Pr2, mth::matr<FLT>::Identity());
    } /* End of 'Render' function */
  };
}


#endif // __topology_test_h_
