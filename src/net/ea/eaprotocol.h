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

#ifndef NET_EA_EAPROTOCOL_H
#define NET_EA_EAPROTOCOL_H

namespace Ea
{
    enum
    {
        WALK_SPEED   = 0,
        EXP          = 1,
        JOB_EXP      = 2,
        KARMA        = 3,
        MANNER       = 4,
        HP           = 5,
        MAX_HP       = 6,
        MP           = 7,
        MAX_MP       = 8,
        CHAR_POINTS  = 9,
        LEVEL        = 11,
        SKILL_POINTS = 12,
        STR          = 13,
        AGI          = 14,
        VIT          = 15,
        INT          = 16,
        DEX          = 17,
        LUK          = 18,
        MONEY        = 20,
        EXP_NEEDED   = 22,
        JOB_MOD      = 23,
        TOTAL_WEIGHT = 24,
        MAX_WEIGHT   = 25,
        STR_NEEDED   = 32,
        AGI_NEEDED   = 33,
        VIT_NEEDED   = 34,
        INT_NEEDED   = 35,
        DEX_NEEDED   = 36,
        LUK_NEEDED   = 37,
        ATK          = 41,
        ATK_MOD      = 42,
        MATK         = 43,
        MATK_MOD     = 44,
        DEF          = 45,
        DEF_MOD      = 46,
        MDEF         = 47,
        MDEF_MOD     = 48,
        HIT          = 49,
        FLEE         = 50,
        FLEE_MOD     = 51,
        CRIT         = 52,
        ATTACK_DELAY = 53,
        JOB          = 55
    };
}  // namespace Ea

enum
{
    EA_SPRITE_BASE = 0,
    EA_SPRITE_HAIR,
    EA_SPRITE_WEAPON,
    EA_SPRITE_HEAD_BOTTOM,
    EA_SPRITE_HEAD_TOP,
    EA_SPRITE_HEAD_MID,
    EA_SPRITE_HAIR_COLOR,
    EA_SPRITE_CLOTHES_COLOR,
    EA_SPRITE_SHIELD,
    EA_SPRITE_SHOES,
    EA_SPRITE_BODY,
    EA_SPRITE_FLOOR,
    EA_SPRITE_ROBE,
    EA_SPRITE_EVOL2,
    EA_SPRITE_EVOL3,
    EA_SPRITE_EVOL4,
    EA_SPRITE_EVOL5,
    EA_SPRITE_EVOL6,
    EA_SPRITE_EVOL7,
    EA_SPRITE_EVOL8,
    EA_SPRITE_EVOL9,
    EA_SPRITE_EVOL10,
    EA_SPRITE_EVOL11,
    EA_SPRITE_EVOL12,
    EA_SPRITE_VECTOREND
};

static const int INVENTORY_OFFSET = 2;
static const int STORAGE_OFFSET = 1;

#endif  // NET_EA_EAPROTOCOL_H
