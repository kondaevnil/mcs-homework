/* FILE NAME  : def.h
 * PROGRAMMER : DK6
 * LAST UPDATE: 22.07.2021
 * PURPOSE    : Simple WinAPI animation project.
 *              Common declaration module.
 */

#ifndef __def_h_
#define __def_h_

//#define GLEW_STATIC
//#include <glew.h>
//
//#include <wglew.h>
//#include <gl/wglext.h>
//
//#pragma comment(lib, "opengl32")

#include "src/mth/mth.h"

 /* Debug memory allocation support */
#ifndef NDEBUG 
# define _CRTDBG_MAP_ALLOC
# include <crtdbg.h>
# define SetDbgMemHooks() \
  _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
  _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))

static class __Dummy
{
public:
  /* Class constructor */
  __Dummy( VOID )
  {
    SetDbgMemHooks();
  } /* End of '__Dummy' constructor */
} __ooppss;
#endif /* _DEBUG */ 

#ifdef _DEBUG 
# ifdef _CRTDBG_MAP_ALLOC 
#   define new new(_NORMAL_BLOCK, __FILE__, __LINE__) 
# endif /* _CRTDBG_MAP_ALLOC */ 
#endif /* _DEBUG */

//#pragma comment(lib, "opengl32")
//#define GLEW_STATIC
//#include <glew.h>
//#include <gl/gl.h>
//#pragma comment(lib, "glew32")

#define GLEW_STATIC
#include <glew.h>

#include <wglew.h>
#include <gl/wglext.h>

#pragma comment(lib, "opengl32")

 /* Large integer types */
typedef long long INT64;
typedef unsigned long long UINT64;
typedef FLOAT FLT;

#endif // __def_h_

/* END OF 'def.h' FILE */
