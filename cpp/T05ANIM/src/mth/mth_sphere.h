/* FILE NAME  : mth_sphere.h
 * PROGRAMMER : KS6
 * LAST UPDATE: 20.07.2021
 * PURPOSE    : Sphere handle module.
 */

#ifndef __mth_sphere_h_
#define __mth_sphere_h_

#include "mth_vec3.h"
#include "mth_matr.h"

/* Math library namespace */
namespace mth
{
  /* Types swap function.
   * ARGUMENTS:
   *   - varables to swap:
   *       type &A, type &B;
   * RETURNS: None.
   */
  template<typename type>
  VOID Swap(type &A, type &B)
  {
    type tmp = B;
    B = A;
    A = tmp;
  } // End of 'Swap' function

  /* Intersections calculating namespace */
  namespace intersections
  {
  /* Sphere handle class */
  template<class type>
    class sphere
    {
    public:
      vec3<type> Center; // Sphere central point
      type Rad;          // Sphere radius

      /* Class default contructor.
       * ARGUMENTS: None.
       */
      sphere( VOID )
      {
      } // End of 'sphere' function

      /* Class constructor.
       * ARGUMENTS:
       *   - radius:
       *       type Radius;
       *   - center:
       *       vec3<type> Center;
       */
      sphere( type Radius, vec3<type> Center ) : Center(Center), Rad(Radius)
      {
      } // End of 'sphere' function

      /* Sphere and ray intersection function. 
       * ARGUMENTS:
       *   - ray:
       *       const ray<type> &Ray;
       *   - veariable to set intersection point in:
       *       vec3<type> *IntersectionPoint = nullptr;
       * RETURNS:
       *   (BOOL) intersection flag.
       */
      BOOL Intersect( ray<type> &Ray, vec3<type> *IntersectionPoint = nullptr ) const 
      {
        vec3<type> a = Center - Ray.Org;
        type OC2 = a & a;
        type OK = a & Ray.Dir;
        if (OK < 0)
          return FALSE;
        type OK2 = OK * OK;

        type H2 = Rad - (OC2 - OK2); 
        if (H2 < 0)
          return FALSE;
        if (OC2 < Rad * Rad)
        {
          if (IntersectionPoint != nullptr)
            *IntersectionPoint = Ray.Dir * (OK + sqrt(H2));
          return TRUE;
        }

        if (IntersectionPoint != nullptr)
          *IntersectionPoint = Ray.Dir * (OK - sqrt(H2));
        return TRUE; 
      } // End of 'Intersect' function
    };
  }
}

#endif /* __mth_sphere_h_ */

/* END OF 'mth_sphere.h' FILE */