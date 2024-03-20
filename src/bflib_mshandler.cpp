/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_mshandler.cpp
 *     Graphics drawing support sdk class.
 * @par Purpose:
 *     A link between game engine and the DirectDraw library.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     16 Nov 2008 - 21 Nov 2009
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "pre_inc.h"
#include "bflib_mshandler.hpp"

#include <string.h>
#include <stdio.h>

#include "bflib_basics.h"
#include "bflib_mouse.h"
#include "bflib_sprite.h"

#include "keeperfx.hpp"
#include "post_inc.h"

/******************************************************************************/
// Global variables
int volatile lbMouseInstalled = false;
int volatile lbMouseOffline = false;
class MouseStateHandler pointerHandler;

/******************************************************************************/
/**
 * Adjusts point coordinates.
  @param x,y Coorditates to adjust.
 * @return Returns true if the coordinates have changed.
 */
int adjust_point(long *x, long *y)
{
    return false;
}

MouseStateHandler::MouseStateHandler(void)
{
    Release();
}

MouseStateHandler::~MouseStateHandler(void)
{
  SDL_DestroySemaphore(this->semaphore);
}

bool MouseStateHandler::Install(void)
{
    this->installed = true;
    return true;
}

bool MouseStateHandler::IsInstalled(void)
{
    return SDL_SemWait(this->semaphore) == 0;
}

bool MouseStateHandler::Release(void)
{
    if(SDL_SemWait(this->semaphore) < 0) {
      return false;
    }

    lbMouseInstalled = false;
    lbDisplay.MouseSprite = NULL;
    this->installed = false;
    mssprite = NULL;

    pointer.Release();

    mspos.x = 0;
    mspos.y = 0;
    hotspot.x = 0;
    hotspot.y = 0;

    return true;
}

struct TbPoint *MouseStateHandler::GetPosition(void)
{
    if (!this->installed)
      return NULL;
    return &mspos;
}

bool MouseStateHandler::SetMousePosition(long x, long y)
{
    if(SDL_SemWait(this->semaphore) < 0) {
      return false;
    }

    if (!this->SetPosition(x, y))
      return false;

    lbDisplay.MMouseX = this->installed ? mspos.x : x;
    lbDisplay.MMouseY = this->installed ? mspos.y : y;

    return true;
}

bool MouseStateHandler::SetPosition(long x, long y)
{
    long prev_x;
    long prev_y;
    long mx;
    long my;
    if (!this->installed)
      return false;
    // Clip coordinates to our mouse window
    mx = x;
    if (x < lbDisplay.MouseWindowX)
    {
      mx = lbDisplay.MouseWindowX;
    } else
    if (x >= lbDisplay.MouseWindowX+lbDisplay.MouseWindowWidth)
    {
      mx = lbDisplay.MouseWindowWidth + lbDisplay.MouseWindowX - 1;
    }
    my = y;
    if (y < lbDisplay.MouseWindowY)
    {
      my = lbDisplay.MouseWindowY;
    } else
    if ( y >= lbDisplay.MouseWindowHeight+lbDisplay.MouseWindowY)
    {
      my = lbDisplay.MouseWindowHeight + lbDisplay.MouseWindowY - 1;
    }
    // If the coords are unchanged
    if ((mx == lbDisplay.MMouseX) && (my == lbDisplay.MMouseY))
      return true;
    //Change the position
    prev_x = mspos.x;
    mspos.x = mx;
    prev_y = mspos.y;
    mspos.y = my;
    if ((mssprite != NULL) && (this->installed))
    {
      //show_onscreen_msg(5, "POS %3d x %3d CLIP %3d x %3d WINDOW %3d x %3d", x,y,mx,my,lbDisplay.MouseWindowX,lbDisplay.MouseWindowY);
      if (!pointer.OnMove())
      {
        mspos.x = prev_x;
        mspos.y = prev_y;
        return false;
      }
    }
    return true;
}

bool MouseStateHandler::SetMouseWindow(long x, long y,long width, long height)
{
    if(SDL_SemWait(this->semaphore) < 0) {
      return false;
    }

    lbDisplay.MouseWindowX = x;
    lbDisplay.MouseWindowY = y;
    lbDisplay.MouseWindowWidth = width;
    lbDisplay.MouseWindowHeight = height;
    adjust_point(&lbDisplay.MMouseX, &lbDisplay.MMouseY);
    adjust_point(&lbDisplay.MouseX, &lbDisplay.MouseY);

    return true;
}

bool MouseStateHandler::GetMouseWindow(struct TbRect *windowRect)
{
    if(SDL_SemWait(this->semaphore) < 0) {
      return false;
    }

    windowRect->left = lbDisplay.MouseWindowX;
    windowRect->top = lbDisplay.MouseWindowY;
    windowRect->right = lbDisplay.MouseWindowX+lbDisplay.MouseWindowWidth;
    windowRect->bottom = lbDisplay.MouseWindowY+lbDisplay.MouseWindowHeight;

    return true;
}

const struct TbSprite *MouseStateHandler::GetPointer(void)
{
    if (!this->installed)
      return NULL;
    return mssprite;
}

bool MouseStateHandler::SetPointer(const struct TbSprite *spr, struct TbPoint *point)
{
    if (!this->installed)
      return false;
    mssprite = spr;
    hotspot.y = 0;
    hotspot.x = 0;
    if ((spr != NULL) && (spr->SWidth != 0) && (spr->SHeight != 0))
    {
      if (point != NULL)
      {
        hotspot.x = point->x;
        hotspot.y = point->y;
      }
      pointer.Initialise(spr, &mspos, &hotspot);
      if ((mssprite != NULL) && (this->installed))
      {
        pointer.OnMove();
      }
    } else
    {
      pointer.Release();
      mssprite = NULL;
    }
    return true;
}

bool MouseStateHandler::SetMousePointerAndOffset(const struct TbSprite *mouseSprite, long x, long y)
{
    if(SDL_SemWait(this->semaphore) < 0) {
      return false;
    }
    
    struct TbPoint point;
    
    if (mouseSprite == lbDisplay.MouseSprite)
      return true;
    
    if (mouseSprite != NULL)
    {
        if ((mouseSprite->SWidth > 64) || (mouseSprite->SHeight > 64))
        {
            WARNLOG("Mouse pointer too large");
            return false;
        }
    }
    
    lbDisplay.MouseSprite = mouseSprite;
    point.x = x;
    point.y = y;
    
    return this->SetPointer(mouseSprite, &point);
}

bool MouseStateHandler::SetMousePointer(const struct TbSprite *mouseSprite)
{
    if(SDL_SemWait(this->semaphore) < 0) {
      return false;
    }

    if (mouseSprite == lbDisplay.MouseSprite)
      return true;

    if (mouseSprite != NULL)
      if ( (mouseSprite->SWidth > 64) || (mouseSprite->SHeight > 64) )
      {
        WARNLOG("Mouse pointer too large");
        return false;
      }

    lbDisplay.MouseSprite = mouseSprite;
    this->SetPointer(mouseSprite, NULL);

    return true;
}

bool MouseStateHandler::SetPointerOffset(long x, long y)
{
    if(SDL_SemWait(this->semaphore) < 0) {
      return false;
    }

    if (this->installed)
      pointer.SetHotspot(x, y);

    return true;
}

struct TbPoint *MouseStateHandler::GetPointerOffset(void)
{
    return &hotspot;
}

bool MouseStateHandler::PointerBeginSwap(void)
{
    if(SDL_SemWait(this->semaphore) < 0) {
      return false;
    }

    if ((!lbMouseInstalled) || (lbMouseOffline))
      return true;

    if ((mssprite != NULL) && (this->installed))
    {
      swap = 1;
      pointer.OnBeginSwap();
    }

    return true;
}

bool MouseStateHandler::PointerEndSwap(void)
{
    if (!lbMouseInstalled)
      return true;

    if ((mssprite != NULL) && (this->installed))
    {
      if (swap)
      {
        swap = false;
        pointer.OnEndSwap();
      }
    }

    SDL_SemPost(this->semaphore);
    return true;
}
/******************************************************************************/
