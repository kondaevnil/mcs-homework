/* FILENAME:    material_mng.h
 * LAST UPDATE: 27.07.2021
 * PROGRAMMER:  DK6
 * PURPOSE:     The material manager realisation.
 */

#ifndef __material_mng_h_
#define __material_mng_h_

#include "../material.h"
#include "base_mng.h"

namespace degl
{
  class material_mng : public resource_manager<material, std::string>
  {
  public:
    /* Create material function
     * ARGUMENTS:
     *   - material coefficients:
     *       material::wrapper wrap;
     *   - texture names:
     *       std::initializer_list<std::string> Tex;
     *   -  shader name:
     *       std::string ShdName
     * RETURNS:
     *   (material *) created material.
     */
    material * MaterialCreate( material::wrapper wrap, std::initializer_list<std::string> Tex, std::string ShdName )
    {
      material mtl, *mtl1;

      if ((mtl1 = Find(wrap.Name)) != nullptr) return mtl1;

      return Add(mtl.Create(wrap, Tex, ShdName));
    }

    /* Create material function
     * ARGUMENTS:
     *   - material coefficients:
     *       material::wrapper wrap;
     *   - texture names:
     *       std::initializer_list<std::string> Tex;
     *   -  shader name:
     *       std::string ShdName
     * RETURNS:
     *   (material *) created material.
     */
    material * MaterialCreate( material::wrapper wrap, texture * Tex, std::string ShdName )
    {
      material mtl, *mtl1;

      if ((mtl1 = Find(wrap.Name)) != nullptr) return mtl1;

      return Add(mtl.Create(wrap, Tex, ShdName));
    }

    /* Material update function
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID UpdateAllMaterial( VOID )
    {
      for (auto mtl : Stock)
      {
        mtl.second.MaterialUpdate();
      }
    } /* End of 'UpdateAllMaterial' function */
  }; // End of 'material_mng' class
} // End of 'degl' namespace

#endif // !__material_mng_h_


 /* END OF 'material_mng.h' FILE */
