/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2015  The ManaPlus Developers
 *
 *  This file is part of The ManaPlus Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "resources/sdlmusic.h"

#include "logger.h"

#include "debug.h"

SDLMusic::SDLMusic(Mix_Music *const music, SDL_RWops *const rw) :
    Resource(),
    mMusic(music),
    mRw(rw)
{
}

SDLMusic::~SDLMusic()
{
    Mix_FreeMusic(mMusic);
#ifndef USE_SDL2
    if (mRw)
    {
        SDL_RWclose(mRw);
        mRw = nullptr;
    }
#endif
}

Resource *SDLMusic::load(SDL_RWops *const rw)
{
#ifdef USE_SDL2
    if (Mix_Music *const music = Mix_LoadMUSType_RW(rw, MUS_OGG, 1))
    {
        return new SDLMusic(music, nullptr);
    }
#else
    // Mix_LoadMUSType_RW was added without version changed in SDL1.2 :(
    if (Mix_Music *const music = Mix_LoadMUS_RW(rw))
    {
        return new SDLMusic(music, rw);
    }
#endif
    else
    {
        logger->log("Error, failed to load music: %s", Mix_GetError());
        return nullptr;
    }
}

bool SDLMusic::play(const int loops, const int fadeIn)
{
    if (fadeIn > 0)
        return Mix_FadeInMusicPos(mMusic, loops, fadeIn, 0.0);
    else
        return Mix_FadeInMusicPos(mMusic, loops, 0, 0.0);
}
