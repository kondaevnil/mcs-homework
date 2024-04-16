/* FILENAME:    anim.cpp
 * LAST UPDATE: 22.07.2021
 * PROGRAMMER:  DK6
 * PURPOSE:     The anim class realisation.
 */

#include "anim.h"
#include <cstdio>

degl::anim degl::anim::Instance;

namespace degl
{
  /* Initialization function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID anim::Init( VOID )
  {
  } /* End of 'Init' function */

  /* Deinitialization function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID anim::Close( VOID )
  {
    //render::~render();
  } /* End of 'Close' function */

  /* ??? function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID anim::Idle( VOID )
  {
    Render();
    render::CopyFrame();
  } /* End of 'Idle' function */

  /* Timer callback function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID anim::Timer( VOID )
  {
    Render();
    render::CopyFrame();
  } /* End of 'Timer' function */

  /* Resize callback function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID anim::Resize( VOID )
  {
    render::Resize();
    anim::render::Cam.Resize(this->win::W, this->win::H);
    Render();
    render::CopyFrame();
  } /* End of 'Resize' function */

  /* Activate callback function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID anim::Activate( VOID )
  {
  } /* End of 'Activate' function */

  /* Erase callback function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID anim::Erase( HDC hDC )
  {
  } /* End of 'Erase' function */

  /* Paint callback function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID anim::Paint( HDC hDC )
  {
    /*hDC = BeginPaint(anim::win::hWnd, &ps);
    EndPaint(anim::win::hWnd, &ps);*/
  } /* End of 'Paint' function */
}

/* END OF 'anim.cpp' FILE */
