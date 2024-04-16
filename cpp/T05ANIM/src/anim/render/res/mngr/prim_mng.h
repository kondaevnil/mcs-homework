/* FILENAME:    prim_mng.h
 * LAST UPDATE: 26.07.2021
 * PROGRAMMER:  DK6
 * PURPOSE:     The primitive manager class realisation.
 */

#ifndef __prim_mng_h_
#define __prim_mng_h_

#include "../../primitive.h"
#include "base_mng.h"

namespace degl
{
  /* Primitive manager class */
  class prim_mng : public resource_manager<prim>
  {
  public:
    /* Create primitive function.
     * ARGUMENTS:
     *   - topology base reference:
     *       const topology::base &Tpl;
     * RETURNS:
     *   (prim *) created primitive interface.
     */
    template<class vertex_type>
      prim * PrimCreate( const topology::base<vertex_type> &Tpl )
      {
        prim pr;
        return Add(pr.Create(Tpl));
      } /* End of 'PrimCreate' function */

    /* Create primitive function.
     * ARGUMENTS:
     *   - topology base reference:
     *       const topology::base &Tpl;
     * RETURNS:
     *   (prim *) created primitive interface.
     */
    template<class vertex_type>
      prim * PrimCreate( const topology::base<vertex_type> &Tpl, material *Mtl )
      {
        prim pr;
        return Add(pr.Create(Tpl, Mtl));
      } /* End of 'PrimCreate' function */
  }; // End of 'prim_mng' class

} // End of 'degl' namespace

#endif // !__prim_mng_h_


/* END OF 'prim_mng' FILE */