/* FILENAME:    input.h
 * LAST UPDATE: 22.07.2021
 * PROGRAMMER:  DK6
 * PURPOSE:     The input class realisation.
 */

#ifndef __input_h_
#define __input_h_

#include "../../def.h"

#pragma comment(lib, "winmm")

namespace degl
{
  class mouse
  {  
  public:
    INT X, Y, Z, Dx, Dy, Dz, &Wheel;
  private:
    HWND &hWnd;

  public:
    mouse( HWND &hWnd, INT &WheelRef  ) : Dx(0), Dy(0), Dz(0), Z(0), Wheel(WheelRef), hWnd(hWnd)
    {
      POINT pt;

      GetCursorPos(&pt);
      ScreenToClient(hWnd, &pt);

      X = pt.x, Y = pt.y;
    }

    VOID Response( VOID )
    {
      POINT pt;

      GetCursorPos(&pt);
      ScreenToClient(hWnd, &pt);

      Dx = pt.x - X, Dy = pt.y - Y;
      X = pt.x, Y = pt.y;

      Dz = Wheel, Z += Dz, Wheel = 0;
    }
  };

  class keyboard
  {
  public:
    BYTE Keys[256], KeysOld[256], KeysClick[256];

  public:
    keyboard( VOID )
    {
      GetKeyboardState(Keys);
      memcpy(KeysOld, Keys, 256);
    }

    VOID Response( VOID )
    {
      GetKeyboardState(Keys);
      for (INT i = 0; i < 256; i++)
      {
        Keys[i] >>= 7;
        KeysClick[i] = Keys[i] && !KeysOld[i];
      }
      memcpy(KeysOld, Keys, 256);
    }
  };

  class joystick
  {
  protected:
    BYTE
      But[32], ButOld[32], ButClick[32];
    INT Pov;
    DBL
      X, Y, Z, R;
  public:
    joystick( VOID )
    {
      for (INT i = 0; i < 32; i++)
      {
        ButClick[i] = 0;
        ButOld[i] = But[i];
      }
    }

    VOID Response( VOID )
    {
      if (joyGetNumDevs() > 0)
      {
        JOYCAPS jc;

        if (joyGetDevCaps( JOYSTICKID1, &jc, sizeof( jc ) ) == JOYERR_NOERROR)
        {
          JOYINFOEX ji;

          ji.dwSize = sizeof( JOYINFOEX );
          ji.dwFlags = JOY_RETURNALL;
          if (joyGetPosEx( JOYSTICKID1, &ji ) == JOYERR_NOERROR)
          {
            for (INT i = 0; i < 32; i++)
            {
              But[i] = (ji.dwButtons >> i) & 1;
              ButClick[i] = But[i] && !ButOld[i];
              ButOld[i] = But[i];
            }

            X = 2.0 * (ji.dwXpos - jc.wXmin) / (jc.wXmax - jc.wXmin) - 1;
            Y = 2.0 * (ji.dwYpos - jc.wYmin) / (jc.wYmax - jc.wYmin) - 1;
            Z = 2.0 * (ji.dwZpos - jc.wZmin) / (jc.wZmax - jc.wZmin) - 1;
            R = 2.0 * (ji.dwRpos - jc.wRmin) / (jc.wRmax - jc.wRmin) - 1;

            Pov = ji.dwPOV == 0xFFFF ? -1 : ji.dwPOV / 4500;
          }
        }
      }
    }
  };

  class input : public mouse, public keyboard, public joystick
  {
  public:
    input( HWND &hWnd, INT &WheelRef ) : mouse(hWnd, WheelRef), joystick(), keyboard()
    {
    }

    VOID InputResponse( VOID )
    {
      mouse::Response();
      keyboard::Response();
      joystick::Response();
    }
  };
}

#endif // !__input_h_


 /* END OF 'input.h' FILE */
