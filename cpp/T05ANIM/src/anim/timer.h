/* FILENAME:    timer.h
 * LAST UPDATE: 22.07.2021
 * PROGRAMMER:  DK6
 * PURPOSE:     The timer class realisation.
 */

#ifndef __timer_h_
#define __timer_h_

#include "../../def.h"

namespace degl
{
  class timer
  {
  private:
    UINT64
      StartTime,    /* Start program time */
      OldTime,      /* Previous frame time */
      OldTimeFPS,   /* Old time FPS measurement */
      PauseTime,    /* Time during pause period */
      TimePerSec,   /* Timer resolution */
      FrameCounter; /* Frames counter */
  public:
    BOOL IsPause = FALSE;
    DBL FPS = 0;
    DBL GlobalTime = 0;
    DBL GlobalDeltaTime = 0;
    DBL DeltaTime = 0;
    DBL Time = 0;
  public:
    // timer constructor
    timer( VOID )
    {
      LARGE_INTEGER t;

      QueryPerformanceFrequency( &t );
      TimePerSec = t.QuadPart;
      QueryPerformanceCounter( &t );
      StartTime = OldTime = OldTimeFPS = t.QuadPart;
      FrameCounter = 0;
      IsPause = FALSE;
      FPS = 30.0;
      PauseTime = 0;
    }

    /* Timer interframe response function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Response( VOID )
    {
      LARGE_INTEGER t;

      QueryPerformanceCounter( &t );
      /* Global time */
      GlobalTime = (DBL)(t.QuadPart - StartTime) / TimePerSec;
      GlobalDeltaTime = (DBL)(t.QuadPart - OldTime) / TimePerSec;
      /* Time with pause */
      if (IsPause)
      {
        DeltaTime = 0;
        PauseTime += t.QuadPart - OldTime;
      }
      else
      {
        DeltaTime = GlobalDeltaTime;
        Time = static_cast<DBL>(t.QuadPart - PauseTime - StartTime) / TimePerSec;
      }
      /* FPS */
      FrameCounter++;
      if (t.QuadPart - OldTimeFPS > TimePerSec)
      {
        FPS = FrameCounter * TimePerSec / static_cast<DBL>(t.QuadPart - OldTimeFPS);
        OldTimeFPS = t.QuadPart;
        FrameCounter = 0;
      }
      OldTime = t.QuadPart;
    } /* End of 'Response' function */
  };
}

#endif // __timer_h_


 /* END OF 'timer.h' FILE */
