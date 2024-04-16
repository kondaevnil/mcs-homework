/* FILENAME:    render.h
 * LAST UPDATE: 23.07.2021
 * PROGRAMMER:  DK6
 * PURPOSE:     The render class declaration.
 */

#ifndef __render_h_
#define __render_h_

#include "../../../def.h"

// manager system includes block
#include "res/mngr/prim_mng.h"
#include "res/mngr/shader_mng.h"
#include "res/mngr/material_mng.h"
#include "res/mngr/texture_mng.h"
#include "res/primitives.h"
#include "res/buffer.h"

typedef mth::vec4<FLT> vec4;
typedef mth::matr<FLT> matr;

namespace degl
{
  struct cam_data
  {
    vec4 CamLocFrameW;
    vec4 CamRightFrameH;
    vec4 CamUp;
    vec4 CamDir;
    mth::matr_data<FLT> VP;

    cam_data( VOID )
    {
    }

    cam_data( vec4 CamLocFrameW,
              matr VP,
              vec4 CamRightFrameH,
              vec4 CamUp,
              vec4 CamDir) : CamLocFrameW(CamLocFrameW), VP(VP), CamRightFrameH(CamRightFrameH),
                             CamUp(CamUp), CamDir(CamDir)
    {
    }

    VOID Update( vec4 CamLocFrameW,
                 matr VP,
                 vec4 CamRightFrameH,
                 vec4 CamUp,
                 vec4 CamDir )
    {
      this->CamLocFrameW = CamLocFrameW;
      this->CamRightFrameH = CamRightFrameH;
      this->CamUp = CamUp;
      this->CamDir = CamDir;
      this->VP = VP;
    } /* End of 'Update' function */
  };

  struct global_data
  {
    mth::matr_data<FLT> W;

    global_data( VOID )
    {
    }

    global_data( matr m ) : W(m)
    {
    } /* End of 'global_data' function */

    VOID Update( matr m )
    {
      this->W = m;
    } /* End of 'Update' function */
  };

  class render : public prim_mng, public shader_mng, public material_mng,
                 public texture_mng, public model_manager, public buffer_manager
  {
  private:
    HWND &hWnd;           // window handler
    HDC hDC;              // descriptor context handler
    INT &W, &H;           // screen width and height
    HGLRC hGLRC;          //
    buffer *CamBuffer;    // my own buffer!!
    buffer *GlobalBuffer;  // flex buffer!!
    cam_data CamData;
    global_data GlobalData;

  public:
    mth::camera<FLT> Cam; // camera

  public:
    /* Render constructor function.
     * ARGUMENTS:
     *   - window handler:
     *       HWND hWnd;
     *   - window width and heigth:
     *       INT W, H;
     * RETURNS: None.
     */
    render( HWND &hWnd, INT &W, INT &H );

    /* Render deconstructor function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    ~render( VOID );

    /* Render resize function.
     * ARGUMENTS:
     *   - new width and height
     * RETURNS: None.
     */
     VOID Resize( VOID );

     /* Render start function.
      * ARGUMENTS: None.
      * RETURNS: None.
      */
     VOID Start( VOID );

     /* Render end function.
      * ARGUMENTS: None.
      * RETURNS: None.
      */
     VOID End( VOID );

     /* Render copy new frame function.
      * ARGUMENTS: None.
      * RETURNS: None.
      */
     VOID CopyFrame( VOID );

     /* Primitive create function.
      * ARGUMENTS:
      *   - pointer to primitive:
      *       prim *Pr;
      *   - vertex array:
      *       INT *V;
      *   - number of vertecies:
      *       INT NumOfV;
      *   - index array:
      *       INT *I;
      *   - number of indecies:
      *       INT NumOfI;
      * RETURNS: None.
      */
     //template<typename vertex>
     //  static VOID PrimCreate( prim *Pr, prim_type PrimType, vertex *V, INT NoV, INT *I, INT NoI )
     //  {
     //    Pr->Type = PrimType;

     //    if (V != nullptr && NoV != 0)
     //    {
     //      glGenBuffers( 1, &Pr->VBuf );
     //      glGenVertexArrays( 1, &Pr->VA );

     //      glBindVertexArray( Pr->VA );
     //      glBindBuffer( GL_ARRAY_BUFFER, Pr->VBuf );
     //      glBufferData( GL_ARRAY_BUFFER, sizeof( degl::vertex::std ) * NoV, V, GL_STATIC_DRAW );

     //      // Setup data order due to vertex description string
     //      const std::string dsc = vertex::Description;

     //      // Setup data order due to vertex description string
     //      // for (auto c = vertex::Description.begin(); c != vertex::Description.end(); c++)
     //      INT attr = 0, offset = 0;
     //      for (auto c = dsc.begin(); c != dsc.end(); c++)
     //        if (*c == 'f' || *c == 'i')
     //        {
     //          CHAR type = *c++;
     //          INT n = *c - '0';

     //          if (n < 0 || n > 9)
     //            break;
     //          // Enable used attributes
     //          glEnableVertexAttribArray(attr);
     //          glVertexAttribPointer(attr, n, type == 'f' ? GL_FLOAT : GL_INT, FALSE,
     //            sizeof(vertex), (VOID *)offset);
     //          offset += n * 4;
     //          attr++;
     //        }
     //        else
     //          break;
     //    }

     //    if (I != nullptr && NoI != 0)
     //    {
     //      glGenBuffers( 1, &Pr->IBuf );
     //      glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf );
     //      glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( INT ) * NoI, I, GL_STATIC_DRAW );
     //      Pr->NumOfEls = NoI;
     //    }
     //    else
     //      Pr->NumOfEls = NoV;
     //  } /* End of 'PrimCreate' function */

     /* Primitive drawing function.
      * ARGUMENTS:
      *   - pointer to primitive:
      *       prim *Pr;
      *   - world martix:
      *       mth::matr<FLT> w;
      * RETURNS: None.
      */
     VOID Draw( prim *Pr, mth::matr<FLT> W );

     /* Primitive drawing function.
      * ARGUMENTS:
      *   - pointer to primitive:
      *       prim *Pr;
      *   - file name:
      *       CHAR *File;
      * RETURNS:
      *   (BOOL) boolean value.
      */
     static BOOL PrimLoad( prim *Pr, CHAR *File );

     /* Primitive free function.
      * ARGUMENTS:
      *   - pointer to primitive:
      *       prim *Pr;
      * RETURNS: None.
      */
     static VOID PrimFree( prim *Pr );

  }; // End of 'render' class
}

#endif // __render_h_


/* END OF 'render.h' FILE */
