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

#include "gui/windowmenu.h"

#include "configuration.h"

#include "input/inputmanager.h"

#include "gui/buttoninfo.h"
#include "gui/buttontext.h"
#include "gui/popupmanager.h"
#include "gui/skin.h"

#include "gui/popups/popupmenu.h"
#include "gui/popups/textpopup.h"

#include "gui/windows/skilldialog.h"

#include "gui/widgets/button.h"

#include "utils/dtor.h"
#include "utils/gettext.h"

#include "debug.h"

WindowMenu *windowMenu = nullptr;

WindowMenu::WindowMenu(const Widget2 *const widget) :
    Container(widget),
    ActionListener(),
    SelectionListener(),
    MouseListener(),
    mSkin(theme ? theme->load("windowmenu.xml", "") : nullptr),
    mPadding(mSkin ? mSkin->getPadding() : 1),
    mSpacing(mSkin ? mSkin->getOption("spacing", 3) : 3),
    mButtons(),
    mButtonTexts(),
    mButtonNames(),
    mHaveMouse(false),
    mAutoHide(1),
    mSmallWindow(mainGraphics->getWidth() < 600)
{
    int x = mPadding;
    int h = 0;

    setFocusable(false);
    // TRANSLATORS: short button name for who is online window.
    addButton(N_("ONL"),
        _("Who is online"), x, h, InputAction::WINDOW_ONLINE);
    // TRANSLATORS: short button name for help window.
    addButton(N_("HLP"),
        _("Help"), x, h, InputAction::WINDOW_HELP);
    // TRANSLATORS: short button name for quests window.
    addButton(N_("QE"),
        _("Quests"), x, h, InputAction::WINDOW_QUESTS);
    // TRANSLATORS: short button name for kill stats window.
    addButton(N_("KS"),
        _("Kill stats"), x, h, InputAction::WINDOW_KILLS);
    addButton(":-)",
        _("Smilies"), x, h, InputAction::WINDOW_EMOTE_SHORTCUT);
    // TRANSLATORS: short button name for chat window.
    addButton(N_("CH"),
        _("Chat"), x, h, InputAction::WINDOW_CHAT,
#ifdef ANDROID
        Visible_true);
#else
        Visible_false);
#endif
    // TRANSLATORS: short button name for status window.
    addButton(N_("STA"),
        _("Status"), x, h, InputAction::WINDOW_STATUS);
    // TRANSLATORS: short button name for equipment window.
    addButton(N_("EQU"),
        _("Equipment"), x, h, InputAction::WINDOW_EQUIPMENT);
    // TRANSLATORS: short button name for inventory window.
    addButton(N_("INV"),
        _("Inventory"), x, h, InputAction::WINDOW_INVENTORY);
    // TRANSLATORS: short button name for cart window.
    addButton(N_("CA"),
        _("Cart"), x, h, InputAction::WINDOW_CART);
    // TRANSLATORS: short button name for map window.
    addButton(N_("MAP"),
        _("Map"), x, h, InputAction::WINDOW_MINIMAP,
        Visible_false);

    if (skillDialog->hasSkills())
    {
        // TRANSLATORS: short button name for skills window.
        addButton(N_("SKI"),
            _("Skills"), x, h, InputAction::WINDOW_SKILL);
    }

    // TRANSLATORS: short button name for social window.
    addButton(N_("SOC"),
    // TRANSLATORS: full button name
        _("Social"), x, h, InputAction::WINDOW_SOCIAL);
    // TRANSLATORS: short button name for shortcuts window.
    addButton(N_("SH"),
        // TRANSLATORS: full button name
        _("Shortcuts"), x, h, InputAction::WINDOW_SHORTCUT);
    // TRANSLATORS: short button name for spells window.
    addButton(N_("SP"),
        // TRANSLATORS: full button name
        _("Spells"), x, h, InputAction::WINDOW_SPELLS);
    // TRANSLATORS: short button name for drops window.
    addButton(N_("DR"),
        // TRANSLATORS: full button name
        _("Drop"), x, h, InputAction::WINDOW_DROP,
        Visible_false);
    // TRANSLATORS: short button name for did you know window.
    addButton(N_("YK"),
        // TRANSLATORS: full button name
        _("Did you know"), x, h, InputAction::WINDOW_DIDYOUKNOW,
        Visible_false);
    // TRANSLATORS: short button name for shop window.
    addButton(N_("SHP"),
        // TRANSLATORS: full button name
        _("Shop"), x, h, InputAction::WINDOW_SHOP,
        Visible_false);
    // TRANSLATORS: short button name for outfits window.
    addButton(N_("OU"),
        // TRANSLATORS: full button name
        _("Outfits"), x, h, InputAction::WINDOW_OUTFIT,
        Visible_false);
    // TRANSLATORS: short button name for updates window.
    addButton(N_("UP"),
        // TRANSLATORS: full button name
        _("Updates"), x, h, InputAction::WINDOW_UPDATER,
        Visible_false);
    // TRANSLATORS: short button name for bank window.
    addButton(N_("BA"),
        // TRANSLATORS: full button name
        _("Bank"), x, h, InputAction::WINDOW_BANK,
        Visible_false);
    // TRANSLATORS: short button name for mail window.
    addButton(N_("MA"),
        // TRANSLATORS: full button name
        _("Mail"), x, h, InputAction::WINDOW_MAIL,
        Visible_false),
    // TRANSLATORS: short button name for debug window.
    addButton(N_("DBG"),
        // TRANSLATORS: full button name
        _("Debug"), x, h, InputAction::WINDOW_DEBUG,
#ifdef ANDROID
        Visible_true);
#else
        Visible_false);
#endif
    // TRANSLATORS: short button name for windows list menu.
    addButton(N_("WIN"),
        // TRANSLATORS: full button name
        _("Windows"), x, h, InputAction::SHOW_WINDOWS,
        Visible_false);
    // TRANSLATORS: short button name for setup window.
    addButton(N_("SET"),
        // TRANSLATORS: full button name
        _("Setup"), x, h, InputAction::WINDOW_SETUP);

    x += mPadding - mSpacing;
    if (mainGraphics)
        setDimension(Rect(mainGraphics->mWidth - x, 0, x, h));

    loadButtons();

    addMouseListener(this);
    setVisible(Visible_true);

    config.addListener("autohideButtons", this);
    mAutoHide = config.getIntValue("autohideButtons");
}

WindowMenu::~WindowMenu()
{
    config.removeListener("autohideButtons", this);
    CHECKLISTENERS

    for (std::map <std::string, ButtonInfo*>::iterator
         it = mButtonNames.begin(),
         it_end = mButtonNames.end(); it != it_end; ++it)
    {
        delete (*it).second;
    }
    mButtonNames.clear();
    FOR_EACH (std::vector <Button*>::iterator, it, mButtons)
    {
        Button *const btn = *it;
        if (!btn)
            continue;
        if (!btn->isVisibleLocal())
            delete btn;
    }
    delete_all(mButtonTexts);
    mButtonTexts.clear();
    if (mSkin)
    {
        if (theme)
            theme->unload(mSkin);
        mSkin = nullptr;
    }
}

void WindowMenu::action(const ActionEvent &event)
{
    const std::string &eventId = event.getId();
    const std::map <std::string, ButtonInfo*>::iterator
        it = mButtonNames.find(eventId);
    if (it == mButtonNames.end())
        return;

    const ButtonInfo *const info = (*it).second;
    if (!info)
        return;

    inputManager.executeAction(info->key);
}

void WindowMenu::addButton(const char *const text,
                           const std::string &description,
                           int &restrict x,
                           int &restrict h,
                           const InputActionT key,
                           const Visible visible)
{
    Button *const btn = new Button(this, gettext(text), text, this);
    btn->setPosition(x, mPadding);
    btn->setDescription(description);
    btn->setTag(static_cast<int>(key));
    add(btn);
    btn->setFocusable(false);
    x += btn->getWidth() + mSpacing;
    h = btn->getHeight() + 2 * mPadding;
    mButtons.push_back(btn);
    mButtonNames[text] = new ButtonInfo(btn, key, visible);
    if (key != InputAction::SHOW_WINDOWS)
        mButtonTexts.push_back(new ButtonText(description, key));
}

void WindowMenu::mousePressed(MouseEvent &event)
{
    if (!popupManager)
        return;

    if (event.getButton() == MouseButton::RIGHT)
    {
        if (mSmallWindow)
            return;

        event.consume();
        Button *const btn = dynamic_cast<Button*>(event.getSource());
        if (!btn)
            return;
        if (popupMenu)
        {
            popupMenu->showPopup(getX() + event.getX(),
                getY() + event.getY(), btn);
        }
    }
}

void WindowMenu::mouseMoved(MouseEvent &event)
{
    mHaveMouse = true;

    if (!textPopup)
        return;

    if (event.getSource() == this)
    {
        textPopup->hide();
        return;
    }

    const Button *const btn = dynamic_cast<const Button *const>(
        event.getSource());

    if (!btn)
    {
        textPopup->hide();
        return;
    }

    const int x = event.getX();
    const int y = event.getY();
    const InputActionT key = static_cast<InputActionT>(btn->getTag());
    const Rect &rect = mDimension;
    if (key != InputAction::NO_VALUE)
    {
        textPopup->show(x + rect.x, y + rect.y, btn->getDescription(),
            // TRANSLATORS: short key name
            strprintf(_("Key: %s"), inputManager.getKeyValueString(
            key).c_str()));
    }
    else
    {
        textPopup->show(x + rect.x, y + rect.y, btn->getDescription());
    }
}

void WindowMenu::mouseExited(MouseEvent& event A_UNUSED)
{
    mHaveMouse = false;
    if (!textPopup)
        return;

    textPopup->hide();
}

void WindowMenu::showButton(const std::string &name,
                            const Visible visible)
{
    const ButtonInfo *const info = mButtonNames[name];
    if (!info || !info->button)
        return;

    info->button->setVisible(visible);
    updateButtons();
    saveButtons();
}

void WindowMenu::updateButtons()
{
    int x = mPadding;
    int h = 0;
    FOR_EACH (std::vector <Button*>::const_iterator, it, mButtons)
        safeRemove(*it);
    const int pad2 = 2 * mPadding;
    FOR_EACH (std::vector <Button*>::iterator, it, mButtons)
    {
        Button *const btn = *it;
        if (!btn)
            continue;
        if (btn->isVisibleLocal())
        {
            btn->setPosition(x, mPadding);
            add(btn);
            x += btn->getWidth() + mSpacing;
            h = btn->getHeight() + pad2;
        }
    }
    x += mPadding - mSpacing;
    if (mainGraphics)
        setDimension(Rect(mainGraphics->mWidth - x, 0, x, h));
}

void WindowMenu::loadButtons()
{
    if (!mSmallWindow)
    {
        if (config.getValue("windowmenu0", "") == "")
        {
            for (std::map <std::string, ButtonInfo*>::iterator
                 it = mButtonNames.begin(),
                 it_end = mButtonNames.end(); it != it_end; ++it)
            {
                const ButtonInfo *const info = (*it).second;
                if (!info || !info->button || info->visible == Visible_true)
                    continue;
                info->button->setVisible(Visible_false);
            }
            updateButtons();
            return;
        }

        for (int f = 0; f < 30; f ++)
        {
            const std::string str = config.getValue(
                "windowmenu" + toString(f), "");
            if (str.empty() || str == "SET")
                continue;
            const ButtonInfo *const info = mButtonNames[str];
            if (!info || !info->button)
                continue;
            info->button->setVisible(Visible_false);
        }
    }
    else
    {
        for (std::map <std::string, ButtonInfo*>::iterator
             it = mButtonNames.begin(),
             it_end = mButtonNames.end(); it != it_end; ++it)
        {
            const ButtonInfo *const info = (*it).second;
            if (!info || !info->button)
                continue;
            Button *const button = info->button;
            const std::string &str = button->getActionEventId();
            button->setVisible(fromBool(
                str == "SET" || str == "WIN", Visible));
        }
    }
    updateButtons();
}

void WindowMenu::saveButtons() const
{
    int i = 0;
    FOR_EACH (std::vector <Button*>::const_iterator, it, mButtons)
    {
        const Button *const btn = *it;
        if (btn && !btn->isVisibleLocal())
        {
            config.setValue("windowmenu" + toString(i),
                btn->getActionEventId());
            i ++;
        }
    }
    for (int f = i; f < 30; f ++)
        config.deleteKey("windowmenu" + toString(f));
}

void WindowMenu::drawChildren(Graphics* graphics)
{
    if (mHaveMouse || !mAutoHide || (mAutoHide == 1
        && mainGraphics && (mSmallWindow || mainGraphics->mWidth > 800)))
    {
        Container::drawChildren(graphics);
    }
}

void WindowMenu::safeDrawChildren(Graphics* graphics)
{
    if (mHaveMouse || !mAutoHide || (mAutoHide == 1
        && mainGraphics && (mSmallWindow || mainGraphics->mWidth > 800)))
    {
        Container::safeDrawChildren(graphics);
    }
}

void WindowMenu::optionChanged(const std::string &name)
{
    if (name == "autohideButtons")
        mAutoHide = config.getIntValue("autohideButtons");
}

#ifdef USE_PROFILER
void WindowMenu::logicChildren()
{
    BLOCK_START("WindowMenu::logicChildren")
    BasicContainer::logicChildren();
    BLOCK_END("WindowMenu::logicChildren")
}
#endif
