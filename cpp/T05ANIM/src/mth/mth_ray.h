/* FILE NAME  : mth_ray.h
 * PROGRAMMER : KS6
 * LAST UPDATE: 20.07.2021
 * PURPOSE    : Ray handle module.
 */

#ifndef __mth_ray_h_
#define __mth_ray_h_

#include "mth_vec3.h"
#include "mth_matr.h"

/* Math library namespace */
namespace mth
{
  /* Intersections calculating namespace */
  namespace intersections
  {
  /* Ray handle class */
  template<class type>
    struct ray
    {
    public:
      vec3<type> Org, Dir; // Vectors setting ray

      ray( vec3<type> Org, vec3<type> Dir ) : Org(Org), Dir(Dir)
      {
      }
    }; /* End of 'sphere' class */
  } /* End of 'intersections' namespace */
} /* end of 'mth' namespace */

#endif /* __mth_ray_h_ */

/* END OF 'mth_ray.h' FILE */