/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  The Legend of Mazzeroth Development Team
 *  Copyright (C) 2008-2009  The Mana World Development Team
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

#include "gui/popups/skillpopup.h"

#include "gui/gui.h"

#include "gui/fonts/font.h"

#include "gui/widgets/label.h"
#include "gui/widgets/skilldata.h"
#include "gui/widgets/skillinfo.h"
#include "gui/widgets/textbox.h"

#include "debug.h"

SkillPopup *skillPopup = nullptr;

SkillPopup::SkillPopup() :
    Popup("SkillPopup", "skillpopup.xml"),
    mSkillName(new Label(this)),
    mSkillDesc(new TextBox(this)),
    mSkillEffect(new TextBox(this)),
    mLastId(0U)
{
    mSkillName->setFont(boldFont);
    mSkillName->setPosition(0, 0);

    const int fontHeight = getFont()->getHeight();

    mSkillDesc->setEditable(false);
    mSkillDesc->setPosition(0, fontHeight);
    mSkillDesc->setForegroundColorAll(getThemeColor(ThemeColorId::POPUP),
        getThemeColor(ThemeColorId::POPUP_OUTLINE));

    mSkillEffect->setEditable(false);
    mSkillEffect->setPosition(0, 2 * fontHeight);
    mSkillEffect->setForegroundColorAll(getThemeColor(ThemeColorId::POPUP),
        getThemeColor(ThemeColorId::POPUP_OUTLINE));
}

void SkillPopup::postInit()
{
    Popup::postInit();
    add(mSkillName);
    add(mSkillDesc);
    add(mSkillEffect);

    addMouseListener(this);
}

SkillPopup::~SkillPopup()
{
}

void SkillPopup::show(const SkillInfo *const skill)
{
    if (!skill || !skill->data || skill->id == mLastId)
        return;

    mLastId = skill->id;

    mSkillName->setCaption(skill->data->dispName);
    mSkillName->adjustSize();
    mSkillName->setPosition(0, 0);

    std::string description = skill->data->description;
    std::string effect = skill->skillEffect;
    if (description.empty())
    {
        description = effect;
        effect.clear();
    }
    mSkillDesc->setTextWrapped(description, 196);
    mSkillEffect->setTextWrapped(effect, 196);

    int minWidth = mSkillName->getWidth();

    if (mSkillName->getWidth() > minWidth)
        minWidth = mSkillName->getWidth();
    if (mSkillDesc->getMinWidth() > minWidth)
        minWidth = mSkillDesc->getMinWidth();
    if (mSkillEffect->getMinWidth() > minWidth)
        minWidth = mSkillEffect->getMinWidth();

    const int numRowsDesc = mSkillDesc->getNumberOfRows();
    const int numRowsEffect = mSkillEffect->getNumberOfRows();
    const int height = getFont()->getHeight();

    if (skill->skillEffect.empty())
    {
        setContentSize(minWidth, (numRowsDesc + 1) * height);
    }
    else
    {
        setContentSize(minWidth, (numRowsDesc + numRowsEffect + 1) * height);
        mSkillEffect->setPosition(0, (numRowsDesc + 1) * height);
    }

    mSkillDesc->setPosition(0, 1 * height);
}

void SkillPopup::mouseMoved(MouseEvent &event)
{
    Popup::mouseMoved(event);

    // When the mouse moved on top of the popup, hide it
    setVisible(Visible_false);
    mLastId = 0;
}
