/* FILENAME:    texture.h
 * LAST UPDATE: 29.07.2021
 * PROGRAMMER:  DK6
 * PURPOSE:     The texture class realisation.
 */

#ifndef __texture_h_
#define __texture_h_

#include "../../../../def.h"
#include "../../reader.h"

#include <string>
#include <fstream>

namespace degl
{
  class texture
  {
  private:
    INT W, H;   // texture width and height
    UINT TexId; // texture id
  public:
    std::string Name; // texture unique name

    enum struct tex_type
    {
      TEX_G24, TEX_G32,
    };

    /* Getter width function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (INT) texture width.
     */
    INT GetW( VOID )
    {
      return this->W;
    } /* End of 'GetW' function */

    /* Getter height function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (INT) texture height.
     */
    INT GetH( VOID )
    {
      return this->H;
    } /* End of 'GetH' function */

    /* Getter width function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (INT) texture width.
     */
    INT GetId( VOID )
    {
      return this->TexId;
    } /* End of 'GetId' function */

  public:   // after debug private
    /* G32/G24 loader function.
     * ARGUMENTS:
     *   - file name:
     *       std::string FileName;
     *   - G32/G24 tag:
     *       INT tag;
     * RETURNS: None.
     */
    VOID TextureLoad( std::string FileName, INT tag )
    {
      binary_reader rd("bin/textures/" + FileName);
      if (!rd.IsOk())
        return;

      WORD W, H;
      rd(&W);
      rd(&H);
      if (rd.FileLength != W * H * 4 + 4)
        return;

      // Setup data
      this->W = W;
      this->H = H;
      BYTE *mem = new BYTE[W * H * 4];

      // Load buffer
      rd(mem, W * H * 4);

      TexAddImg(FileName, W, H, tag, mem);

      delete[] mem;
    } /* End of 'TextureLoad' function */

    /* Texture creation function.
     * ARGUMENTS:
     *   - texture name:
     *       std::string Name;
     *   - texture width and height:
     *       INT W, H;
     *   -  components:
     *       INT compo;
     *   - color bits:
     *       VOID *Bits;
     * RETURNS: None.
     */
    VOID TexAddImg( std::string TexName, INT W, INT H, INT C, VOID *Bits )
    {
      FLT mips;
      if ((mips = log(W > H ? H : W) / log(2)) < 1) mips = 1;

      glGenTextures(1, &TexId);
      glBindTexture(GL_TEXTURE_2D, TexId);

      glTexStorage2D(GL_TEXTURE_2D, mips, C == 3 ? GL_RGB8 : C == 4 ? GL_RGBA8 : GL_R8, W, H);
      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, W, H, GL_RGBA, GL_UNSIGNED_BYTE, Bits);
      glGenerateMipmap(GL_TEXTURE_2D);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

      Name = TexName;
      glBindTexture(GL_TEXTURE_2D, 0);
    } /* End of 'TexImgAdd' */

     /* Texture creation G24 function
      * ARGUMENTS:
      *   - filename:
      *       std:string FileName;
      *   - components:
      *       INT comp;
      * RETURNS: 
      *   (texture &) created texture.
      */
    texture & Create( const std::string FileName, tex_type Type)
    {
      switch (Type)
      {
      case degl::texture::tex_type::TEX_G24:
        TextureLoad(FileName, 3);
        break;
      case degl::texture::tex_type::TEX_G32:
        TextureLoad(FileName, 4);
        break;
      default:
        break;
      }
      return *this;
    } /* End of 'Create' function */

    /* Texture creation G24 function
      * ARGUMENTS: None.
      * RETURNS: 
      *   (texture &) created texture.
      */
    texture & Create( INT W, INT H, INT C, BYTE *mem, std::string Name )
    {
      this->W = W;
      this->H = H;

      TexAddImg(Name, W, H, C, mem);
      
      return *this;
    } /* End of 'Create' function */

    /* Texture free function
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Free( VOID )
    {
      glDeleteTextures(1, &TexId);
    } /* End of 'Free' function */
  }; // End of 'texture' class

} // End of 'degl' namespace

#endif // !__texture_h_

// потому что ты одеваешься в хуй пойми что мне стыдно с тобой гулять ты сумашедшая целовала алфальт перепутал кровать тразнулна маминой это не правильно в горле ком обьясняюсь потом я очень рад что ты раздевашес я потому что ты одеваешься в хуй пойми что \ты бесколнченая нюхаять сне спать не спать перестал тебя понимать плачешьс без повода ты еще та пизда в горле ком обьяснюсь потом я очень что ты раздеваешься потому что потому ЧТО  одеваешься в хуй пойдми ЧТО  мне тсдыно с тобой гулять ты сумашедшая целовала асфальт как это понимать как это понимать я очень рад что ты раздеваешься потому что ты одеваешься в хъуй пойми что 

/* END OF 'texture.h' FILE */
