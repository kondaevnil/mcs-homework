/* FILENAME:    main.cpp
 * LAST UPDATE: 21.07.2021
 * PROGRAMMER:  DK6
 * PURPOSE:     Program entry point.
 */

#include "../anim/anim.h"
#include "../units/samples/unit_container.h"

/*
 * DEBUG INCLUDES:
 */
#include "../anim/render/res/texture.h" 

//#include "../units/samples/test_unit.h"

/* The main program function.
 * ARGUMENTS:
 *   - handle of application instance:
 *       HINSTANCE hInstance;
 *   - dummy handle of previous application instance (not used):
 *       HINSTANCE hPrevInstance;
 *   - command line string:
 *       CHAR *CmdLine;
 *   - show window command parameter (see SW_***):
 *       INT CmdShow;
 * RETURNS:
 *   (INT) Error level for operation system (0 for success).
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
  CHAR *CmdLine, INT ShowCmd )
{
  /*degl::anim::ani(hInstance);
  ani.Run();*/

  /*degl::texture flex;
  flex.TextureLoad("brick.g32", 4);*/

  degl::anim &a = degl::anim::Get();
  /*degl::unit *b = new degl::test_unit;*/  // TODO: Topology loading models. Check render::PrimCreate!!
  degl::unit *c = new degl::ctrl;
  degl::unit *d = new degl::topo_test(&a, 100, 200);
  degl::unit *f = new degl::sky(&a);
  degl::unit *g = new degl::model_test(&a);
  //degl::unit *h = new degl::hmap(&a, 10, 10);
  degl::unit *i = new degl::game(&a);

  //a << c << d << f;
  a << f << c << d << i << g;
  //a << f << c;

  degl::anim::Get().Run();

  return 30;
} /* End of 'WinMain' function */

/* END OF 'main.cpp' FILE */