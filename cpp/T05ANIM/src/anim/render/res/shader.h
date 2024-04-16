/* FILENAME:    shader.h
 * LAST UPDATE: 27.07.2021
 * PROGRAMMER:  DK6
 * PURPOSE:     The shader class realisation.
 */

#ifndef __shader_h_
#define __shader_h_

#include "../../../../def.h"
#include <string>
#include <fstream>

namespace degl
{
  template<typename type>
    struct loc_storage;

  class shader
  {
  public:
    static std::string Path;
  public:
    ::std::string Name;

   /* Shader class default constructor
    * ARGUMENTS: None.
    * RETURNS: None.
    */
    shader( VOID ) : ProgId(0)
    {
    } /* End of 'shader' function */

    /* Create shader function
     * ARGUMENTS: ??
     * RETURNS:
     *   (shader &) ref to created shader
     */
    shader & Create( std::string RepoName );

    // helpful function
    INT GetId( VOID )
    {
      return ProgId;
    } /* End of 'GetId' function */

    std::string GetName( VOID )
    {
      return Name;
    }

    /* Shader detach function
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Free( VOID );

    /* Shader uniform set function
     * ARGUMENTS:
     *   - special uniform structure:
     *       (type) Data;
     * RETURNS: 
     *  (INT) uniform location.
     */
    template<typename type>
      VOID SetUniform( loc_storage<type> storage )
      {
        if (storage.loc != -1)
          if constexpr (std::is_convertible_v<FLT, type>)
            glUniform1f(storage.loc, storage.Value);
          else if constexpr (std::is_convertible_v<INT, type>)
            glUniform1i(storage.loc, storage.Value);
          else if constexpr (std::is_convertible_v<mth::vec3<FLT>, type>)
            glUniform3fv(storage.loc, 1, storage.Value);
          else if constexpr (std::is_convertible_v<mth::matr<FLT>, type>)
            glUniformMatrix4fv(storage.loc, 1, FALSE, storage.Value);
      } /* End of 'SetUniform' function */

    /* Shader uniform set function
     * ARGUMENTS:
     *   - special uniform structure:
     *       (type) Data;
     * RETURNS:
     *  (INT) uniform location.
     */
    INT GetLocation( std::string Name );

    /* Shader Update function
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID ShaderUpdate( VOID )
    {
      ProgId = ShaderLoad();
    } /* End of 'ShaderUpdate' function */
  private:
    /* Loading shader function
     * ARGUMENTS:
     *   - shader filename: 
     *       const CHAR *FileName;
     * RETURNS:
     *   (INT) shader program id.
     */
    INT ShaderLoad( VOID );

    /* Shader logging function
     * ARGUMENTS:
     *   - file prefix, shdaer name and text:
     *       std::string Prefix, PartName, Text;
     */
    static VOID ShaderLog( std::string Prefix, std::string PartName, std::string Text );
  protected:
    INT ProgId;
  }; // End of 'shader' class
} // End of 'degl' namespace


#endif // __shader_h_

 
 /* END OF 'shader.h' FILE */
