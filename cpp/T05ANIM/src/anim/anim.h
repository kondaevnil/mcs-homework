/* FILENAME:    anim.h
 * LAST UPDATE: 23.07.2021
 * PROGRAMMER:  DK6
 * PURPOSE:     The anim class declaration.
 */

#ifndef __anim_h_
#define __anim_h_

#include "../win/win.h"
#include "../stock/stock.h"
#include "./input.h"
#include "timer.h"
#include "render/render.h"

namespace degl
{
  class anim;
  
  class unit
  {
  protected:
    // unit name
    // ...

  public:
    /* The base unit class destructor.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual ~unit( VOID )
    {
    }

    /* Unit self interaction function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual VOID Response( degl::anim *Ani )
    {
    }


    /* Specific function for base unit class render.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual VOID Render( degl::anim *Ani )
    {
    }
  };

  class scene : public unit
  {
  private:
    stock<unit *> Scene;
    INT Count;

  public:
    scene( VOID ) : Count( 0 )
    {
    }

    /* The scene class constructor.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    scene( degl::anim *Ani ) : Count( 0 )
    {
    } /* End of 'scene' function */

    /* The scene class destructor.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    ~scene( VOID ) override
    {
      Scene.Walk( [this]( unit *Uni )
        {
          delete Uni, Count--;
        } );
    } /* End of '~scene' function */

    /* Unit self interaction function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Response( degl::anim *Ani ) override
    {
      Scene.Walk( [Ani]( unit *Uni )
        {
          Uni->Response( Ani );
        } );
    } /* End of 'Response' function */

    /* Specific function for scene (unit) render.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Render( degl::anim *Ani ) override
    {
      Scene.Walk( [Ani]( unit *Uni )
        {
          Uni->Render( Ani );
        } );
    } /* End of 'Render' function */

    scene &operator<<( unit *Uni )
    {
      this->Scene << Uni, Count++;
      return *this;
    } /* End of 'operator<<' function */
  };
  
  class anim : public win, public timer, public input, public render
  {
    // anim constructor
    // TODO
  private:
    anim( HINSTANCE hInst = GetModuleHandle(nullptr) ) : win(hInst), timer(), input(win::hWnd, win::MouseWheel ), render(win::hWnd, win::W, win::H)
    {
    }
    static anim Instance;
    scene *Scene = new scene;

    ~anim( VOID )
    {
      delete Scene;
    }
  public:
    // derived callbacks
    VOID Init( VOID ) override;
    VOID Close( VOID ) override;
    VOID Timer( VOID ) override;
    VOID Idle( VOID ) override;
    VOID Resize( VOID ) override;
    VOID Activate( VOID ) override;
    VOID Erase( HDC hDC ) override;
    VOID Paint( HDC hDC ) override;
    // ... etc..

    // single tone dependent
    static anim & Get( VOID )
    {
      return Instance;
    }

    static anim * GetPtr( VOID )
    {
      return &Instance;
    }

    anim & operator<<( unit *Uni )
    {
      *Scene << Uni;
      return *this;
    }

    VOID Render( VOID )
    {
      timer::Response();
      input::InputResponse();
      Scene->Response(this);
      render::Start();
      Scene->Render(this);
      render::End();
    }


    // Time getters
    /*
     *
     */
    DBL GetGlobalDeltaTime( VOID )
    {
      return this->GlobalDeltaTime;
    } /* End of 'GetGlobalDelta */
  };
}

#endif // __anim_h_


/* END OF 'anim.h' FILE */
