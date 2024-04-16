/* FILENAME:    shader.cpp
 * LAST UPDATE: 27.07.2021
 * PROGRAMMER:  DK6
 * PURPOSE:     The shader class realisation.
 */

#include "shader.h"
#include "../render.h"
#include <fstream>

std::string degl::shader::Path;

namespace degl
{
    /* Loading shader function
     * ARGUMENTS:
     *   - shader RepoName:
     *       const CHAR *RepoName;
     * RETURNS:
     *   (INT) shader program id.
     */
    INT shader::ShaderLoad( VOID )
    {
      Free();

      struct
      {
        CHAR *Name; /* Shader file prefix name (e.g. "VERT") */
        INT Type;   /* Shader OpenFL type (e.g. GL_VERTEX_SHADER) */
        INT Id;     /* Obtained shader Id from OpenGL */
      } shds[] =
      {
        {const_cast<CHAR *>("vert"), GL_VERTEX_SHADER},
        {const_cast<CHAR *>("geom"), GL_GEOMETRY_SHADER},
        {const_cast<CHAR *>("frag"), GL_FRAGMENT_SHADER},
      };
      INT shdNum = sizeof( shds ) / sizeof( shds[0] ), res, prg = 0;
      CHAR Buf[_MAX_PATH];
      BOOL is_ok = TRUE;
      /*GetCurrentDirectory( sizeof( Buf ), Buf );
      Path = Buf;*/
      Path = "C:/DK6/T05ANIM/";

      for (INT i = 0; i < shdNum; i++)
      {
        std::string path = "bin/shaders/" + Name + "/" + shds[i].Name + ".glsl";
        std::fstream f(path, std::fstream::in);
        std::string shd((std::istreambuf_iterator<CHAR>(f)), std::istreambuf_iterator<CHAR>());

        if (shd.length() == 0)
        {
          if (i == 0 || i == shdNum)
          {
            ShaderLog(Name, shds[i].Name, "Error load file");
            is_ok = FALSE;
            break;
          }
          continue;
        }

        if ((shds[i].Id = glCreateShader(shds[i].Type)) == 0)
        {
          if (i == 0 || i == shdNum - 1)
          {
            ShaderLog(Name, shds[i].Name, "Error create shader" );
            is_ok = FALSE;
            break;
          }
          continue;
        }

        const char *a = shd.c_str();
        glShaderSource(shds[i].Id, 1, &a, nullptr);

        glCompileShader(shds[i].Id);
        glGetShaderiv(shds[i].Id, GL_COMPILE_STATUS, &res);
        if (res != 1)
        {
          glGetShaderInfoLog(shds[i].Id, sizeof(Buf), &res, Buf);
          ShaderLog(Name, shds[i].Name, Buf);
          is_ok = FALSE;
          break;
        }
      }

      if (is_ok)
      {
        if ((prg = glCreateProgram()) == 0)
        {
          ShaderLog(Name, "PROG", "Error create program");
          is_ok = FALSE;
        }
        else
        {
          /* Attach shaders to program */
          for (INT i = 0; i < shdNum; i++)
            if (shds[i].Id != 0)
              glAttachShader(prg, shds[i].Id);
          /* Link shader program */
          glLinkProgram(prg);
          glGetProgramiv(prg, GL_LINK_STATUS, &res);
          if (res != 1)
          {
            glGetProgramInfoLog(prg, sizeof(Buf), &res, Buf);
            ShaderLog(Name, "PROG", Buf);
            is_ok = FALSE;
          }
        }
      }

      if (!is_ok)
      {
        /* Delete all created shaders */
        for (INT i = 0; i < shdNum; i++)
          if (shds[i].Id != 0)
          {
            if (prg)
              glDetachShader(prg, shds[i].Id);
            glDeleteShader(shds[i].Id);
          }
        /* Delete program */
        if (prg)
          glDeleteProgram(prg);
        prg = 0;
      }
      return prg;
    } /* End of 'ShaderLoad' function */

    /* Shader detach function
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID shader::Free( VOID )
    {
      UINT shds[5];
      INT n;

      if (ProgId == 0)
        return;
      glGetAttachedShaders(ProgId, 5, &n, shds);
      for (INT i = 0; i < n; i++)
      {
        glDetachShader(ProgId, shds[i]);
        glDeleteShader(shds[i]);
      }
      glDeleteProgram(ProgId);
    } /* End of 'shaders::Free' function */

    /* Create shader function
     * ARGUMENTS: ??
     * RETURNS:
     *   (shader &) ref to created shader
     */
    shader & shader::Create( std::string RepoName )
    {
      Name = RepoName;
      this->ProgId = ShaderLoad();
      return *this;
    }

    /* Shader uniform set function
     * ARGUMENTS:
     *   - special uniform structure:
     *       (type) Data;
     * RETURNS: None.
     */
    /*template<typename type>
      VOID shader::SetUniform( loc_storage<type> storage )
      {
        INT loc;

        if (storage.loc != -1)
          if constexpr (std::is_convertible_v<FLT, type>)
            glUniform1f(storage.loc, storage.Value);
          else if constexpr (std::is_convertible_v<INT, type>)
            glUniform1i(storage.loc, storage.Value);
          else if constexpr (std::is_convertible_v<mth::vec3<FLT>, type>)
            glUniform3fv(storage.loc, 1, storage.Value);
          else if constexpr (std::is_convertible_v<mth::matr<FLT>, type>)
            glUniformMatrix4fv(storage.loc, 1, FALSE, storage.Value);
      }*/ /* End of 'SetUniform' function */

      /* Shader uniform set function
       * ARGUMENTS:
       *   - special uniform structure:
       *       (type) Data;
       * RETURNS:
       *  (INT) uniform location.
       */
      INT shader::GetLocation( std::string Name )
      {
        return glGetUniformLocation(ProgId, Name.c_str());
      } /* End of 'shader::GetLocation' function */

      /* Shader logging function
       * ARGUMENTS: 
       *   - file prefix, shdaer name and text:
       *       std::string Prefix, PartName, Text;
       */
      VOID shader::ShaderLog( std::string Prefix, std::string PartName, std::string Text )
      {
        std::ofstream("bin/shaders/shd.log", std::ios_base::app) << Prefix << "/" << PartName << "\n" << Text << "\n";
      }



} // End of 'degl' namespace


 /* END OF 'shader.cpp' FILE */
