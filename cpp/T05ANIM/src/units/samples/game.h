/* FILENAME:    game.h
 * LAST UPDATE: 01.08.2021
 * PROGRAMMER:  DK6
 * PURPOSE:     ctrl unit (active element).
 */

#ifndef __game_h_
#define __game_h_

#include "../../anim/anim.h"
#include <array>

typedef mth::intersections::sphere<FLT> sphere;
typedef mth::intersections::ray<FLT> ray;

namespace degl
{
  class game : public unit
  {
  public:
    class enemy
    {
    public:
      sphere sph;
      prim *pr;
      vec3 dir;

      enemy( VOID )
      {
      }

      enemy( sphere sph, prim *pr, vec3 dir ) : sph(sph), pr(pr), dir(dir)
      {
      } /* End of 'enemy' function */

      ~enemy( VOID )
      {
      }
    };

    // params
    static const INT count = 100;
    std::array<enemy, count> enemys;
    prim *aim;

    game( degl::anim *Ani )
    {
      std::vector<vertex::std> v = {{vec3(0)}};                                         // error in defaul material!!!
      topology::base point(prim_type::POINT, v);                                        // error in defaul material!!!
      aim = Ani->PrimCreate(point);
      material::wrapper b = material::GetWrap("Gold");
      material *mtl1 = Ani->MaterialCreate(b, {}, "point");
      aim->SetMaterial(mtl1);
      
      material::wrapper a = material::GetWrap("Silver");
      material *mtl = Ani->MaterialCreate(a, {"brick.g32"}, "default");

      topology::Sphere Topo(40, 40, 3);
      for (INT i = 0; i < count; i++)
      {
        enemys[i].sph = sphere(10, vec3(rand(), rand(), rand()) && vec3(1920, 60, 1080));
        enemys[i].pr = Ani->PrimCreate(Topo, mtl);
        enemys[i].pr->Trans = mth::matr<FLT>::Translate(enemys[i].sph.Center / 2);
        enemys[i].dir = vec3::Rnd0();
      }
    } /* End of 'game' function */

    ~game( VOID ) override
    {
    } /* End of '~game' function */

    /* Unit self interaction function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Response( degl::anim *Ani ) override
    {
      for (INT i = 0; i < count; i++)
      {
        enemys[i].sph.Center = (enemys[i].sph.Center + enemys[i].dir * 0.1);

        if (enemys[i].sph.Center < vec3(0, 0, 0) || enemys[i].sph.Center > vec3(1920, 60, 1080))
          enemys[i].sph.Center = vec3(rand(), rand(), rand()) && vec3(1920, 60, 1080);

        enemys[i].pr->Trans = mth::matr<FLT>::Translate(enemys[i].sph.Center / 2);
      }

      if (Ani->Keys[VK_LBUTTON])
      {
        ray shot(Ani->Cam.Loc, Ani->Cam.Dir);

        for (INT i = 0; i < count; i++)
          if (enemys[i].sph.Intersect(shot))
          {
            enemys[i].sph.Center = vec3(rand(), rand(), rand()) && vec3(1920, 60, 1080);
            enemys[i].pr->Trans = mth::matr<FLT>::Translate(enemys[i].sph.Center / 2);
            break;
          }
      }
    } /* End of 'Response' function */

    /* Specific function for unit render.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Render( degl::anim *Ani ) override
    {
      for (auto enem : enemys)
        Ani->Draw(enem.pr, enem.pr->Trans);
      Ani->Draw(aim, mth::matr<FLT>::Identity());
    } /* End of 'Render' function */
  }; /* End of 'game' class */
} /* End of 'game' namespace */

#endif // !__game_h_

 
 /* END OF 'game.h' FILE */
