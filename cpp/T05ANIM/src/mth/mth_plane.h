/* FILE NAME  : mth_plane.h
 * PROGRAMMER : DK6
 * LAST UPDATE: 20.07.2021
 * PURPOSE    : Sphere handle module.
 */

#ifndef __mth_plane_h_
#define __mth_plane_h_

#include "mth_vec3.h"
#include "mth_matr.h"
#include "mth_ray.h"

/* Math library namespace */
namespace mth
{
  /* Intersections calculating namespace */
  namespace intersections
  {
  template<class type>
    class plane
    {
    public:
      vec3<type> n;
      type d;

      plane( VOID )
      {
      }

      plane( type d, vec3<type> n ) : n(n), d(d)
      {
      }

      plane( vec3<type> p, vec3<type> n ) : n(n), d(n&p)
      {
      }

      BOOL Intersect( ray<type> &Ray, vec3<type> *IntersectionPoint = nullptr ) const
      {
        type t = Ray.Dir & n;

        if (t == 0)
          return false;
        IntersectionPoint = Ray.Org + Ray.Dir * (d - n & Ray.Org) / t;
        return true;
      } // End of 'Intersect' function

      type side(vec3<type> p)
      {
        return (n&p - d);
      }
    };
  }
}

#endif /* __mth_plane_h_ */

/* END OF 'mth_plane.h' FILE */