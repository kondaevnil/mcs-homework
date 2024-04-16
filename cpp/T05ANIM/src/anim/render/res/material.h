/* FILENAME:    material.h
 * LAST UPDATE: 27.07.2021
 * PROGRAMMER:  DK6
 * PURPOSE:     The material class realisation.
 */

#ifndef __material_h_
#define __material_h_

#include "shader.h"
#include "buffer.h"
#include "texture.h"
#include <string>
#include <map>

typedef mth::vec3<FLT> vec3;                            // всм?
typedef mth::vec4<FLT> vec4;                            // всм?
typedef mth::matr<FLT> matr;

namespace degl
{
  template<typename type>
    struct loc_storage
    {
      type Value;
      INT loc;
      std::string Name;                          
                                                 
      VOID SetValue( type Val )                  
      {                                          
        Value = Val;                             
      }                                          
                                                 
      VOID SetValue( type Val, std::string name )
      {                                          
        Value = Val, Name = name;                
      }                                          
                                                 
      VOID SetLoc( INT L )                       
      {                                          
        loc = L;                                 
      }                                          
    };                                           
                                                 
  class material                                 
  {                                              
  public:
    struct wrapper                               
    {                                            
      vec3                                       
        Ka,          /* Ambient coefficient */     
        Kd,          /* Diffuse coefficient */     
        Ks;          /* Specular coefficient */    
      FLT Ph;        /* Phong power coefficient */
      FLT Trans = 1; /* Transparency factor */
      std::string Name;

      /* Structure constructor */
      wrapper( std::string Name = "Default", vec3 Ka = vec3(1),
               vec3 Kd = vec3(0), vec3 Ks = vec3(0), FLT Ph = 0,
               FLT Trans = 0) : Ka(Ka), Kd(Kd), Ks(Ks), Ph(Ph), Trans(Trans), Name(Name)
      {
      } /* End of 'wrapper' function */

      wrapper( std::string Name, vec3 Ka, vec3 Kd, vec3 Ks, FLT Ph ) : Ka(Ka), Kd(Kd), Ks(Ks), Ph(Ph), Name(Name), Trans {}
      {
      }
    }; // End of 'wrapper' structure

    struct material_buffer
    {
      vec4 
        Ka,
        KdTrans,
        KsPh;

      material_buffer( VOID )
      {
      } /* End of 'material_buffer' function */

      material_buffer( wrapper wrap ) : Ka(vec4(wrap.Ka, 0)), KdTrans(vec4(wrap.Kd, wrap.Trans)), KsPh(vec4(wrap.Ks, wrap.Ph))
      {
      } /* End of 'material_buffer' function */
    }; // End of 'material_buffer' structure

    static wrapper GetWrap( std::string Name )
    {
      for (auto wrap : DefaultCoeffs)
      {
        if (wrap.Name == Name)
          return wrap;
      }
      return wrapper();
    }

  private:
    static wrapper DefaultCoeffs[20];
    static material DefaultMaterial;

  private:
    texture * Tex[8];       /* Texture references from texture table (or -1) */
    shader *shd;            /* Linked to material shader */
    material_buffer mbuf;
    buffer *coeffs;

  public:
    std::string Name; // material name
  public:
    /* Material class constructor.
     * ARGUMENTS: None.
     * RETURNES: None
     */
    material( VOID )
    {
    } /* End of 'material' function */

    /* Material class constructor.
     * ARGUMENTS: None.
     * RETURNES: None
     */
    ~material( VOID )
    {
    } /* End of 'material' function */

    /* Material class constructor.
     * ARGUMENTS: None.
     * RETURNES: None
     */
    material( wrapper wrap );

    /* Create material function
     * ARGUMENTS: ??
     * RETURNS:
     *   (material &) ref to created material
     */
    material & Create( wrapper wrap, std::initializer_list<std::string> Tex, std::string ShdName );

    /* Create material function
     * ARGUMENTS: ??
     * RETURNS:
     *   (material &) ref to created material
     */
    material & Create( wrapper wrap, texture * Tex, std::string ShdName );

    /* Get defaul material functon.
     * ARGUMENTS: None.
     * RETURNS:
     *   (material *) default material.
     */
    static material * GetDefault( VOID )
    {
      return &DefaultMaterial;
    }

    /* Material free function
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Free( VOID );

    /* Apply and send material data to gpu
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Apply( VOID );

    /* Update material function
     * ARGUMENTS: 
     */
    VOID MaterialUpdate( VOID ) // In progress
    {
      shd->ShaderUpdate();
    } /* End of 'MaterialUpdate' function */

    shader * GetShader( VOID )
    {
      return this->shd;
    }

  }; // End of 'material' class
} // End of 'degl' namespace

#endif // !__material_h_



/* END OF 'material.h' FILE */
