/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
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

#include "net/ea/gamehandler.h"

#include "client.h"
#include "game.h"
#include "notifymanager.h"

#include "enums/resources/notifytypes.h"

#include "gui/windows/okdialog.h"

#include "gui/widgets/createwidget.h"

#include "net/messagein.h"

#include "utils/gettext.h"

#include "debug.h"

namespace Ea
{

std::string GameHandler::mMap;
BeingId GameHandler::mCharID = BeingId_zero;

GameHandler::GameHandler() :
    Net::GameHandler()
{
    mMap.clear();
    mCharID = BeingId_zero;
}

void GameHandler::who() const
{
}

void GameHandler::setMap(const std::string &map)
{
    mMap = map.substr(0, map.rfind("."));
}

void GameHandler::processWhoAnswer(Net::MessageIn &msg)
{
    NotifyManager::notify(NotifyTypes::ONLINE_USERS,
        msg.readInt32("users count"));
}

void GameHandler::processCharSwitchResponse(Net::MessageIn &msg)
{
    if (msg.readUInt8("response"))
        client->setState(STATE_SWITCH_CHARACTER);
}

void GameHandler::processMapQuitResponse(Net::MessageIn &msg)
{
    if (msg.readUInt8("response"))
    {
        CREATEWIDGET(OkDialog,
            // TRANSLATORS: error header
            _("Game"),
            // TRANSLATORS: error message
            _("Request to quit denied!"),
            // TRANSLATORS: ok dialog button
            _("OK"),
            DialogType::ERROR,
            Modal_true,
            ShowCenter_true,
            nullptr,
            260);
    }
}

void GameHandler::clear()
{
    mMap.clear();
    mCharID = BeingId_zero;
}

void GameHandler::initEngines() const
{
    if (!mMap.empty())
        Game::instance()->changeMap(mMap);
}

}  // namespace Ea
