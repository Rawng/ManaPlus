/*
 *  The ManaPlus Client
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

#include "gui/widgets/flowcontainer.h"

#include "debug.h"

FlowContainer::FlowContainer(const Widget2 *const widget,
                             const int boxWidth,
                             const int boxHeight) :
    Container(widget),
    WidgetListener(),
    mBoxWidth(boxWidth),
    mBoxHeight(boxHeight),
    mGridWidth(1),
    mGridHeight(1)
{
    addWidgetListener(this);
    if (!mBoxWidth)
        mBoxWidth = 1;
    if (!mBoxHeight)
        mBoxHeight = 1;
}

void FlowContainer::widgetResized(const Event &event A_UNUSED)
{
    if (getWidth() < mBoxWidth)
    {
        setWidth(mBoxWidth);
        return;
    }

    const int itemCount = static_cast<int>(mWidgets.size());

    if (!mBoxWidth)
        mGridWidth = getWidth();
    else
        mGridWidth = getWidth() / mBoxWidth;

    if (mGridWidth < 1)
        mGridWidth = 1;

    mGridHeight = itemCount / mGridWidth;

    if (itemCount % mGridWidth != 0 || mGridHeight < 1)
        ++mGridHeight;

    int height = mGridHeight * mBoxHeight;

    if (getHeight() != height)
    {
        setHeight(height);
        return;
    }

    int i = 0;
    height = 0;
    for (WidgetList::const_iterator it = mWidgets.begin();
         it != mWidgets.end(); ++it)
    {
        const int x = i % mGridWidth * mBoxWidth;
        (*it)->setPosition(x, height);

        i++;

        if (i % mGridWidth == 0)
            height += mBoxHeight;
    }
}

void FlowContainer::add(Widget *widget)
{
    if (!widget)
        return;

    Container::add(widget);
    widget->setSize(mBoxWidth, mBoxHeight);
    widgetResized(Event(nullptr));
}
