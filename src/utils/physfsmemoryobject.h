/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2015  The ManaPlus Developers
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

#ifndef UTILS_PHYSFSMEMORYOBJECT_H
#define UTILS_PHYSFSMEMORYOBJECT_H

#ifdef DEBUG_PHYSFS

#include <string>

#include "localconsts.h"

struct PHYSFSMemoryObject final
{
    PHYSFSMemoryObject(const char *const name,
                       const char *const file,
                       const unsigned int line) :
        mName(name),
        mAddFile(strprintf("%s:%u", file, line))
    {
    }

    std::string mName;
    std::string mAddFile;
};

#endif  // DEBUG_PHYSFS
#endif  // UTILS_PHYSFSMEMORYOBJECT_H
