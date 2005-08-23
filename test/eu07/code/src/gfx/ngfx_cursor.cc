#define N_IMPLEMENTS nGfxServer
//------------------------------------------------------------------------------
//  ngfx_cursor.cc
//  (C) 2002 RadonLabs GmbH
//------------------------------------------------------------------------------
#include "gfx/ngfxserver.h"
#include "kernel/nfileserver2.h"


//------------------------------------------------------------------------------
/**
    Activate and show the current mouse cursor. Should be implemented
    by subclass.
*/
void
nGfxServer::ShowCursor()
{
    this->cursorShown = true;
}

//------------------------------------------------------------------------------
/**
    Hide the current mouse cursor. Should be implemented by a specific subclass.
*/
void
nGfxServer::HideCursor()
{
    this->cursorShown = false;
}

//------------------------------------------------------------------------------
