/* FILENAME:    render.cpp
 * LAST UPDATE: 26.07.2021
 * PROGRAMMER:  DK6
 * PURPOSE:     The render class realisation.
 */

#include "render.h"
#include "../anim.h"
#include <cstdio>

typedef mth::vec4<FLT> vec4;
typedef mth::matr<FLT> matr;

namespace degl
{
  /* Render constructor function.
   * ARGUMENTS:
   *   - window handler:
   *       HWND hWnd;
   *   - window width and heigth:
   *       INT W, H;
   * RETURNS: None.
   */
  render::render( HWND &hWnd, INT &W, INT &H ) : hWnd(hWnd), W(W), H(H), Cam()
  {
    INT i;
    UINT nums;
    PIXELFORMATDESCRIPTOR pfd = { 0 };
    HGLRC hRC;
    INT PixelAttribs[] =
    {
      WGL_DRAW_TO_WINDOW_ARB, TRUE,
      WGL_SUPPORT_OPENGL_ARB, TRUE,
      WGL_DOUBLE_BUFFER_ARB, TRUE,
      WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
      WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
      WGL_COLOR_BITS_ARB, 32,
      WGL_DEPTH_BITS_ARB, 32,
      0
    };

    INT ContextAttribs[] =
    {
      WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
      WGL_CONTEXT_MINOR_VERSION_ARB, 6,
      WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
      /* WGL_CONTEXT_CORE_PROFILE_BIT_ARB, */
      0
    };
    hDC = GetDC(hWnd);

    /* OpenGL init: pixel format setup */
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 32;
    i = ChoosePixelFormat(hDC, &pfd);
    DescribePixelFormat(hDC, i, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
    SetPixelFormat(hDC, i, &pfd);

    hGLRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hGLRC);

    /* Initializing GLEW library */
    if (glewInit() != GLEW_OK)
    {
      MessageBox(hWnd, "Error extensions initializing", "Error",
        MB_ICONERROR | MB_OK);
      exit(0);
    }

    if (!(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
    {
      MessageBox(hWnd, "Error: no shaders support", "Error", MB_ICONERROR | MB_OK);
      exit(0);
    }

    /* Enable a new OpenGL profile support */
    wglChoosePixelFormatARB(hDC, PixelAttribs, nullptr, 1, &i, &nums);
    hRC = wglCreateContextAttribsARB(hDC, nullptr, ContextAttribs);

    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(hGLRC);

    hGLRC = hRC;
    wglMakeCurrent(hDC, hGLRC);
    /* Set default OpenGL parameters */
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    glClearColor( 0.3, 0.5, 0.7, 1 );

    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(-1);

    // buffers creation
    CamData = cam_data(vec4(Cam.Loc, W), Cam.VP, vec4(Cam.Right, H), vec4(Cam.Up, 0), vec4(Cam.Dir, 0));
    CamBuffer = CreateBuffers(2, &CamData);
    GlobalData = global_data(matr::Identity());
    GlobalBuffer = CreateBuffers(3, &GlobalData);
  } /* End of 'render::render' function */

  /* Render deconstructor function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  render::~render( VOID )
  {
    //prim_mng::Cl prim mng close..
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(hGLRC);
    ReleaseDC(hWnd, hDC);
  } /* End of 'render::~render' function */

  /* Render resize function.
   * ARGUMENTS:
   *   - new width and height
   * RETURNS: None.
   */
  VOID render::Resize( VOID )
  {
    glViewport(0, 0, render::W, render::H);
    // set proj matrix
  } /* End of 'render::Resize' function */

  /* Render start function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID render::Start( VOID )  // Done??
  {
    static DBL ReloadTime = 0;

    CamData.Update(vec4(Cam.Loc, W), Cam.VP, vec4(Cam.Right, H), vec4(Cam.Up, 0), vec4(Cam.Dir, 0));
    CamBuffer->Update(&CamData);

    if ((ReloadTime += anim::Get().GetGlobalDeltaTime()) > 5)
    {
      UpdateAllMaterial();
      ReloadTime = 0;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  } /* End of 'render::Start' function */

  /* Render end function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID render::End( VOID )
  {
    glFinish();
  } /* End of 'render::End' function */

  /* Render copy new frame function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID render::CopyFrame( VOID )
  {
    wglSwapLayerBuffers(hDC, WGL_SWAP_MAIN_PLANE);
  } /* End of 'render::Copyframe' function */

  /* Primitive drawing function.
   * ARGUMENTS:
   *   - pointer to primitive:
   *       prim *Pr;
   *   - world martix:
   *       mth::matr<FLT> w;
   * RETURNS: None.
   */
  VOID render::Draw( prim *Pr, mth::matr<FLT> W )
  {
    mth::matr<FLT>
      w = Pr->Trans * mth::matr<FLT>::Scale(mth::vec3<FLT>(1)) * W,
      wvp = w * Cam.VP;
    // Update buffer
    GlobalData.Update(wvp);
    GlobalBuffer->Update(&GlobalData);

    INT gl_prim_type = Pr->Type == prim_type::LINE ? GL_LINES :
                      Pr->Type == prim_type::TRIMESH ? GL_TRIANGLES :
                      Pr->Type == prim_type::TRISTRIP ? GL_TRIANGLE_STRIP :
                      GL_POINTS;

    /* making an array of vertices active */
    //glLoadMatrixf(wvp);

    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(-1);

    glUseProgram(Pr->Mtl->GetShader()->GetId());

    Pr->Mtl->Apply(); // In progress..

    glBindVertexArray(Pr->VA);
    if (gl_prim_type == GL_PATCHES)
    {
      glPatchParameteri(GL_PATCH_VERTICES, 16);
      glDrawArrays(gl_prim_type, 0, Pr->NumOfEls);
    }
    else if (Pr->IBuf == 0)
      glDrawArrays(gl_prim_type, 0, Pr->NumOfEls);
    else
    {
      /* making an array of indexes active */
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
      /* drawing */
      glDrawElements(gl_prim_type, Pr->NumOfEls, GL_UNSIGNED_INT, nullptr);
      /* disable index array */
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    /* disable vertex array */
    glBindVertexArray(0);
  } /* End of 'render::Draw' function */

  /* Primitive drawing function.
   * ARGUMENTS:
   *   - pointer to primitive:
   *       prim *Pr;
   *   - file name:
   *       CHAR *File;
   * RETURNS:
   *   (BOOL) boolean value.
   */
  //BOOL render::PrimLoad( prim *Pr, CHAR *File )
  //{
  //  FILE *F;
  //  INT i, nv = 0, nind = 0, size;
  //  degl::vertex::std *V;
  //  INT *Ind;
  //  static CHAR Buf[1000];

  //  fopen_s(&F, File, "r");

  //  if (F == nullptr)
  //    return FALSE;


  //  rewind(F);

  //  /* Count vertexes and indexes */
  //  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  //  {
  //    if (Buf[0] == 'v' && Buf[1] == ' ')
  //      nv++;
  //    else if (Buf[0] == 'f' && Buf[1] == ' ')
  //    {
  //      INT n = 0;

  //      for (i = 1; Buf[i] != 0; i++)
  //        if (isspace(static_cast<UCHAR>(Buf[i - 1])) && !isspace(static_cast<UCHAR>(Buf[i])))
  //          n++;
  //      nind += (n - 2) * 3;
  //    }
  //  }

  //  size = sizeof(degl::vertex::std) * nv + sizeof(INT) * nind;
  //  if ((V = reinterpret_cast<degl::vertex::std *>(malloc(size))) == nullptr)
  //    return FALSE;
  //  Ind = reinterpret_cast<INT *>((V + nv));
  //  memset(V, 0, size);

  //  /* Load primitive */
  //  rewind(F);
  //  nv = 0;
  //  nind = 0;
  //  while (fgets(Buf, sizeof(Buf) - 1, F) != nullptr)
  //  {
  //    if (Buf[0] == 'v' && Buf[1] == ' ')
  //    {
  //      DBL x, y, z;
  //      mth::vec4<FLT> c;

  //      sscanf_s( Buf + 2, "%lf%lf%lf", &x, &y, &z );
  //      V[nv].C = c;
  //      V[nv++].P = mth::vec3<FLT>(x, y, z);
  //    }
  //    else if (Buf[0] == 'f' && Buf[1] == ' ')
  //    {
  //      INT n = 0, n0 = 0, n1 = 0, nc;

  //      for (i = 1; Buf[i] != 0; i++)
  //        if (isspace(static_cast<UCHAR>(Buf[i - 1])) && !isspace(static_cast<UCHAR>(Buf[i])))
  //        {
  //          sscanf_s( Buf + i, "%i", &nc );
  //          if (nc < 0)
  //            nc = nv + nc;
  //          else
  //            nc--;

  //          if (n == 0)
  //            n0 = nc;
  //          else if (n == 1)
  //            n1 = nc;
  //          else
  //          {
  //            Ind[nind++] = n0;
  //            Ind[nind++] = n1;
  //            Ind[nind++] = nc;
  //            n1 = nc;
  //          }
  //          n++;
  //        }
  //    }
  //  }
  //  fclose(F);
  //  PrimCreate<vertex::std>(Pr, prim_type::TRIMESH, V, nv, Ind, nind);
  //  free(V);
  //  return TRUE;
  //} /* End of 'PrimLoad' function */

  /* Primitive free function.
   * ARGUMENTS:
   *   - pointer to primitive:
   *       prim *Pr;
   * RETURNS: None.
   */
  VOID render::PrimFree( prim *Pr )
  {
    glBindVertexArray(Pr->VA);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &Pr->VBuf);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &Pr->VA);


    glDeleteBuffers(1, &Pr->IBuf);

    memset(Pr, 0, sizeof(prim));
  }
}

 /* END OF 'render.cpp' FILE */
