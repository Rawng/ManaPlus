/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#ifndef SIMPLEANIMAION_H
#define SIMPLEANIMAION_H

#include "utils/xml.h"

class Animation;
class Graphics;
class Image;

struct Frame;

/**
 * This class is a leightweight alternative to the AnimatedSprite class.
 * It hosts a looping animation without actions and directions.
 */
class SimpleAnimation
{
    public:
        /**
         * Creates a simple animation with an already created \a animation.
         * Takes ownership over the given animation.
         */
        SimpleAnimation(Animation *animation);

        /**
         * Creates a simple animation that creates its animation from XML Data.
         */
        SimpleAnimation(xmlNodePtr animationNode,
                        const std::string& dyePalettes = std::string());

        ~SimpleAnimation();

        void setFrame(int frame);

        int getLength() const;

        void update(int timePassed);

        bool draw(Graphics *graphics, int posX, int posY) const;

        /**
         * Resets the animation.
         */
        void reset();

        Image *getCurrentImage() const;

    private:
        void initializeAnimation(xmlNodePtr animationNode,
                                const std::string& dyePalettes = std::string());

        /** The hosted animation. */
        Animation *mAnimation;

        /** Time in game ticks the current frame is shown. */
        int mAnimationTime;

        /** Index of current animation phase. */
        int mAnimationPhase;

        /** Current animation phase. */
        Frame *mCurrentFrame;

        /**  Tell whether the animation is ready */
        bool mInitialized;
};

#endif
