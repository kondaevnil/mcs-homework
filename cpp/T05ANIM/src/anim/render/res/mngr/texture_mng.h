/* FILENAME:    texture_mng.h
 * LAST UPDATE: 27.07.2021
 * PROGRAMMER:  DK6
 * PURPOSE:     The texture manager class realisation.
 */

#ifndef __texture_mng_h_
#define __texture_mng_h_

#include "base_mng.h"
#include "../texture.h"

namespace degl
{
  class texture_mng : public resource_manager<texture, std::string>
  {
  public:
    /* Create shader function.
     * ARGUMENTS:
     *   - shader folder:
     *       const topology::base &Tpl;
     * RETURNS:
     *   (prim *) created primitive interface.
     */                                                                                             // nd)
    texture * TextureCreate( const std::string FileName, texture::tex_type Type = texture::tex_type::TEX_G32 )
    {
      texture Tex, *Tex1;

      if ((Tex1 = Find(FileName)) != nullptr) return Tex1;

      return Add(Tex.Create(FileName, Type));
    } /* End of 'PrimCreate' function */

    /* Create shader function.
     * ARGUMENTS:
     *   - shader folder:
     *       const topology::base &Tpl;
     * RETURNS:
     *   (prim *) created primitive interface.
     */                                                                                             // nd)
    texture * TextureCreate( INT W, INT H, INT C, BYTE *mem, std::string Name )
    {
      texture Tex, *Tex1;

      if ((Tex1 = Find(Name)) != nullptr) return Tex1;

      return Add(Tex.Create(W, H, C, mem, Name));
    } /* End of 'PrimCreate' function */
  };
}

#endif // !__texture_mng_h_




 /* END OF 'texture_mng.h' FILE */
