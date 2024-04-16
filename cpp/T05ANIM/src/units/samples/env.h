/* FILENAME:    env.h
 * LAST UPDATE: 31.07.2021
 * PROGRAMMER:  DK6
 * PURPOSE:     Animation unit (active element).
 */

#ifndef __env_h_
#define __env_h_

#include "../../anim/anim.h"

namespace degl
{
  class hmap : public unit
  {
  private:
    prim *map;
    degl::topology::Hmap topo = degl::topology::Hmap("map.g32");
    prims *house1, *house2, *house3;

  public:
    hmap( INT W, INT H )
    {
      /*degl::topology::grid<vertex::std> T(W, H);*/
      //Pr->Create(T);
    }

    /* The class constructor.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    hmap( degl::anim *Ani, INT W, INT H )
    {
      // hmap
      material::wrapper a = material::GetWrap("Peweter");
      material *mtl = Ani->MaterialCreate(a, {"asd.g32"}, "default");
      map = Ani->PrimCreate(topo, mtl);

      // houses
      house1 = Ani->ModelCreate("./bin/models/naboo.g3dm");
      house1->SetTrans(mth::matr<FLT>::Scale(vec3(0.4)) * mth::matr<FLT>::Translate(vec3(200, 0, 150)));

      house2 = Ani->ModelCreate("./bin/models/Cyprys_House.g3dm");
      house2->SetTrans(mth::matr<FLT>::Scale(vec3(4)) * mth::matr<FLT>::Translate(vec3(500, 0, 350)));

      /*house3 = Ani->ModelCreate("./bin/models/box.g3dm");
      house3->SetTrans(matr::Identity());*/

    } /* End of 'scene' function */

    /* The class destructor.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    ~hmap( VOID ) override
    {
      anim *ani = anim::GetPtr();
      ani->PrimFree(map);
      ani->ModelFree(house1);
      ani->ModelFree(house2);
      //ani->ModelFree(house3);
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
      Ani->Draw(map, mth::matr<FLT>::Identity());
      // kaif)
      house1->Draw();
      house2->Draw();
    } /* End of 'Render' function */
  };
}

#endif // !__env_h_


/* END OF 'env.h' FILE */
