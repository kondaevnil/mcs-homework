/* FILENAME:    shader_mng.h
 * LAST UPDATE: 26.07.2021
 * PROGRAMMER:  DK6
 * PURPOSE:     The shader_mng class realisation.
 */

#ifndef __shader_mng_h_
#define __shader_mng_h_

#include "../shader.h"
#include "base_mng.h"

namespace degl                                                                         //deead)
{
  class shader_mng : public resource_manager<shader, std::string>
  {
  public:
    /* Create shader function.
     * ARGUMENTS:
     *   - shader folder:
     *       const topology::base &Tpl;
     * RETURNS:
     *   (prim *) created primitive interface.
     */                                                                                             // nd)
    shader * ShaderCreate( std::string RepoName )
    {
      shader shd;

      //if ((shd1 = Find(RepoName)) != nullptr) return shd1;

      return Add(shd.Create(RepoName));

    } /* End of 'PrimCreate' function */
  }; // End of 'shader_mng' class
} // End of 'degl' namespace


#endif // __shader_mng_h_


 /* END OF 'shader_mng.h' FILE */
