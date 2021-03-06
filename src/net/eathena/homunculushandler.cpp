/*
 *  The ManaPlus Client
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

#include "net/eathena/homunculushandler.h"

#include "actormanager.h"
#include "notifymanager.h"

#include "being/homunculusinfo.h"
#include "being/playerinfo.h"

#include "enums/resources/notifytypes.h"

#include "gui/windows/skilldialog.h"

#include "net/serverfeatures.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"

#include "resources/iteminfo.h"

#include "resources/db/itemdb.h"

#include "utils/stringutils.h"

#include "debug.h"

extern Net::HomunculusHandler *homunculusHandler;

namespace EAthena
{

HomunculusHandler::HomunculusHandler() :
    MessageHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_HOMUNCULUS_SKILLS,
        SMSG_HOMUNCULUS_DATA,
        SMSG_HOMUNCULUS_INFO,
        SMSG_HOMUNCULUS_SKILL_UP,
        SMSG_HOMUNCULUS_FOOD,
        0
    };
    handledMessages = _messages;
    homunculusHandler = this;
}

void HomunculusHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_HOMUNCULUS_SKILLS:
            processHomunculusSkills(msg);
            break;

        case SMSG_HOMUNCULUS_DATA:
            processHomunculusData(msg);
            break;

        case SMSG_HOMUNCULUS_INFO:
            processHomunculusInfo(msg);
            break;

        case SMSG_HOMUNCULUS_SKILL_UP:
            processHomunculusSkillUp(msg);
            break;

        case SMSG_HOMUNCULUS_FOOD:
            processHomunculusFood(msg);
            break;

        default:
            break;
    }
}

void HomunculusHandler::processHomunculusSkills(Net::MessageIn &msg)
{
    if (skillDialog)
        skillDialog->hideSkills(SkillOwner::Homunculus);

    const int count = (msg.readInt16("len") - 4) / 37;
    for (int f = 0; f < count; f ++)
    {
        const int skillId = msg.readInt16("skill id");
        const SkillType::SkillType inf = static_cast<SkillType::SkillType>(
            msg.readInt16("inf"));
        msg.readInt16("unused");
        const int level = msg.readInt16("skill level");
        const int sp = msg.readInt16("sp");
        const int range = msg.readInt16("range");
        const std::string name = msg.readString(24, "skill name");
        const Modifiable up = fromBool(msg.readUInt8("up flag"), Modifiable);
        PlayerInfo::setSkillLevel(skillId, level);
        if (skillDialog)
        {
            if (!skillDialog->updateSkill(skillId, range, up, inf, sp))
            {
                skillDialog->addSkill(SkillOwner::Homunculus,
                    skillId, name, level, range, up, inf, sp);
            }
        }
    }
    if (skillDialog)
        skillDialog->updateModels();
}

void HomunculusHandler::processHomunculusData(Net::MessageIn &msg)
{
    msg.readUInt8("unused");
    const int cmd = msg.readUInt8("state");
    const BeingId id = msg.readBeingId("homunculus id");
    Being *const dstBeing = actorManager->findBeing(id);
    const int data = msg.readInt32("data");
    if (!cmd)  // pre init
    {
        HomunculusInfo *const info = new HomunculusInfo;
        info->id = id;
        PlayerInfo::setHomunculus(info);
        PlayerInfo::setHomunculusBeing(dstBeing);
        return;
    }
    HomunculusInfo *const info = PlayerInfo::getHomunculus();
    if (!info)
        return;
    switch (cmd)
    {
        case 1:  // intimacy
            info->intimacy = data;
            break;
        case 2:  // hunger
            info->hungry = data;
            break;
        case 3:  // accesory
            info->equip = data;
            break;
        default:
            break;
    }
}

void HomunculusHandler::processHomunculusInfo(Net::MessageIn &msg)
{
    const std::string name = msg.readString(24, "name");
    msg.readUInt8("flags");  // 0x01 - renamed, 0x02 - vaporize, 0x04 - alive
    const int level = msg.readInt16("level");
    const int hungry = msg.readInt16("hungry");
    const int intimacy = msg.readInt16("intimacy");
    const int equip = msg.readInt16("equip");
    msg.readInt16("atk");
    msg.readInt16("matk");
    msg.readInt16("hit");
    msg.readInt16("luk/3 or crit/10");
    msg.readInt16("def");
    msg.readInt16("mdef");
    msg.readInt16("flee");
    msg.readInt16("attack speed");
    msg.readInt16("hp");
    msg.readInt16("max hp");
    msg.readInt16("sp");
    msg.readInt16("max sp");
    msg.readInt32("exp");
    msg.readInt32("next exp");
    msg.readInt16("skill points");
    const int range = msg.readInt16("attack range");

    HomunculusInfo *const info = PlayerInfo::getHomunculus();
    if (!info)  // we can't find homunculus being because id is missing
        return;
    Being *const dstBeing = actorManager->findBeing(info->id);

    info->name = name;
    info->level = level;
    info->range = range;
    info->hungry = hungry;
    info->intimacy = intimacy;
    info->equip = equip;
    PlayerInfo::setHomunculusBeing(dstBeing);
}

void HomunculusHandler::processHomunculusSkillUp(Net::MessageIn &msg)
{
    const int skillId = msg.readInt16("skill id");
    const int level = msg.readInt16("level");
    const int sp = msg.readInt16("sp");
    const int range = msg.readInt16("range");
    const Modifiable up = fromBool(msg.readUInt8("up flag"), Modifiable);

    if (skillDialog && PlayerInfo::getSkillLevel(skillId) != level)
        skillDialog->playUpdateEffect(skillId);
    PlayerInfo::setSkillLevel(skillId, level);
    if (skillDialog)
    {
        if (!skillDialog->updateSkill(skillId, range,
            up, SkillType::Unknown, sp))
        {
            skillDialog->addSkill(SkillOwner::Homunculus,
                skillId, "", level,
                range, up, SkillType::Unknown, sp);
        }
    }
}

void HomunculusHandler::processHomunculusFood(Net::MessageIn &msg)
{
    const int flag = msg.readUInt8("fail");
    const int itemId = msg.readInt16("food id");
    if (flag)
    {
        NotifyManager::notify(NotifyTypes::HOMUNCULUS_FEED_OK);
    }
    else
    {
        const std::string name = strprintf("[@@%d|%s@@]", itemId,
            ItemDB::get(itemId).getName().c_str());
        NotifyManager::notify(NotifyTypes::HOMUNCULUS_FEED_FAIL, name);
    }
}

void HomunculusHandler::setName(const std::string &name) const
{
    createOutPacket(CMSG_HOMUNCULUS_SET_NAME);
    outMsg.writeString(name, 24, "name");
}

void HomunculusHandler::moveToMaster() const
{
    const BeingId id = PlayerInfo::getHomunculusId();
    if (id == BeingId_zero)
        return;
    createOutPacket(CMSG_HOMMERC_MOVE_TO_MASTER);
    outMsg.writeBeingId(id, "homunculus id");
}

void HomunculusHandler::move(const int x, const int y) const
{
    const BeingId id = PlayerInfo::getHomunculusId();
    if (id == BeingId_zero)
        return;
    createOutPacket(CMSG_HOMMERC_MOVE_TO);
    outMsg.writeBeingId(id, "homunculus id");
    outMsg.writeCoordinates(static_cast<uint16_t>(x),
        static_cast<uint16_t>(y),
        0U, "position");
}

void HomunculusHandler::attack(const BeingId targetId,
                               const Keep keep) const
{
    const BeingId id = PlayerInfo::getHomunculusId();
    if (id == BeingId_zero)
        return;
    createOutPacket(CMSG_HOMMERC_ATTACK);
    outMsg.writeBeingId(id, "homunculus id");
    outMsg.writeBeingId(targetId, "target id");
    outMsg.writeInt8(static_cast<int8_t>(keep == Keep_true ? 1 : 0), "keep");
}

void HomunculusHandler::feed() const
{
    createOutPacket(CMSG_HOMUNCULUS_MENU);
    outMsg.writeInt16(0, "type");
    outMsg.writeInt8(1, "command");  // feed
}

void HomunculusHandler::fire() const
{
    createOutPacket(CMSG_HOMUNCULUS_MENU);
    outMsg.writeInt16(0, "type");
    outMsg.writeInt8(2, "command");  // delete
}

void HomunculusHandler::talk(const std::string &restrict text) const
{
    if (!serverFeatures->haveMovePet())
        return;
    if (text.empty())
        return;
    std::string msg = text;
    if (msg.size() > 500)
        msg = msg.substr(0, 500);
    const size_t sz = msg.size();

    createOutPacket(CMSG_HOMMERC_TALK);
    outMsg.writeInt16(static_cast<int16_t>(sz + 4 + 1), "len");
    outMsg.writeString(msg, static_cast<int>(sz), "message");
    outMsg.writeInt8(0, "zero byte");
}

void HomunculusHandler::emote(const uint8_t emoteId) const
{
    if (!serverFeatures->haveMovePet())
        return;
    createOutPacket(CMSG_HOMMERC_EMOTE);
    outMsg.writeInt8(emoteId, "emote id");
}

void HomunculusHandler::setDirection(const unsigned char type) const
{
    if (!serverFeatures->haveMovePet())
        return;
    createOutPacket(CMSG_HOMMERC_DIRECTION);
    outMsg.writeInt32(0, "pet id");
    outMsg.writeInt8(0, "head direction");
    outMsg.writeInt8(0, "unused");
    outMsg.writeInt8(MessageOut::toServerDirection(type),
        "pet direction");
}

}  // namespace EAthena
