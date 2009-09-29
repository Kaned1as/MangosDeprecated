/*
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "Common.h"
#include "Database/DatabaseEnv.h"
#include "World.h"
#include "Player.h"
#include "Opcodes.h"
#include "Chat.h"
#include "ObjectAccessor.h"
#include "Language.h"
#include "AccountMgr.h"
#include "SystemConfig.h"
#include "revision.h"
#include "revision_nr.h"
#include "Util.h"

bool ChatHandler::HandleGetFromBackupCommand(const char* args)
{
    //acc, lvl, questsComp, flightPath, spells, deletedSpells, gold, name, race, class
    Player *player=m_session->GetPlayer();
    if (!player)
      return true;

    QueryResult* backup = CharacterDatabase.PQuery("SELECT acc, level, finished_quests, spells, deleted_spells, gold, guid, restored, banned, ready_to_restore FROM charactersBckp WHERE name = '%s' and race = '%u' and class = '%u'", player->GetName(), player->getRace(), player->getClass());
    if (backup)
    {
        Field* backupFld = backup->Fetch();
        uint64 bAccId = backupFld[0].GetUInt64();
        uint32 bLvl = backupFld[1].GetUInt32();
        std::string bFinQuests = backupFld[2].GetCppString();
        std::string bSpells = backupFld[3].GetCppString();
        std::string bDelSpells = backupFld[4].GetCppString();
        uint32 bGold = backupFld[5].GetUInt32();
        uint64 bGuid = backupFld[6].GetUInt64();
        uint8 bRestored = backupFld[7].GetUInt8();
        uint8 bBanned = backupFld[8].GetUInt8();
        uint8 bReady = backupFld[9].GetUInt8();

        if (bRestored)
        {
            PSendSysMessage("Персонаж уже был восстановлен");
            delete backup;
            return true;
        }

        if (bBanned)
        {
            PSendSysMessage("Не пытайтесь восстановить забаненных персонажей");
            delete backup;
            return true;
        }

        if (!bReady)
        {
            PSendSysMessage("Персонаж не синхронизирован. Посетите наш сайт для получения необходимой информации http://wow-russian.ru");
            delete backup;
            return true;
        }

        if (bAccId != m_session->GetAccountId())
        {
            PSendSysMessage("Персонаж не принадлежит вам. ");
            delete backup;
            return true;
        }

        if (player->getLevel() < bLvl)
            HandleCharacterLevel(player, player->GetGUID(), player->getLevel(), bLvl);

        char* end;
        char* start = (char*)bFinQuests.c_str();

        while(true) {
            end = strchr(start,',');
            if(!end)
                break;
            *end=0;

            player->SetQuestStatus(atol(start), QUEST_STATUS_COMPLETE);

            start = end + 1;
        }

        start = (char*)bSpells.c_str();

        while(true) {
            end = strchr(start,',');
            if(!end)
                break;
            *end=0;

            player->learnSpell(atol(start),false);

            start = end + 1;
        }

        start = (char*)bDelSpells.c_str();

        while(true) {
            end = strchr(start,',');
            if(!end)
                break;
            *end=0;

            player->removeSpell(atol(start), false, false);

            start = end + 1;
        }

        player->SetMoney(bGold);

        QueryResult* bItems = CharacterDatabase.PQuery("SELECT entry, slot, count from backupPlrItems where containerslot = '-1' and ownerguid = '%u'", bGuid);
        if (bItems)
        {
            for (uint32 i = 0;i<65535;++i)
              player->RemoveItem(i >> 8, i & 0xFF, false);
            do
            {
                Field* itemsFld = bItems->Fetch();
                uint32 bItmEntry = itemsFld[0].GetUInt32();
                uint32 bItmSlot = itemsFld[1].GetUInt32();
                uint32 bItmCnt = itemsFld[2].GetUInt32();

                //Adding items
                uint32 noSpaceForCount = 0;

                // check space and find places
                ItemPosCountVec dest;
                uint8 msg = player->CanStoreNewItem( NULL_BAG, bItmSlot, dest, bItmEntry, bItmCnt, &noSpaceForCount );
                if( msg != EQUIP_ERR_OK )                               // convert to possible store amount
                    bItmCnt -= noSpaceForCount;

                if( bItmCnt == 0 || dest.empty())                         // can't add any
                    continue;

                Item* item = player->StoreNewItem( dest, bItmEntry, true, Item::GenerateItemRandomPropertyId(bItmEntry));

                if(bItmCnt > 0 && item)
                    player->SendNewItem(item,bItmCnt,false,true);

            } while( bItems->NextRow());

            delete bItems;
        }

        bItems = CharacterDatabase.PQuery("SELECT entry, slot, containerslot, count from backupPlrItems where containerslot > '-1' and ownerguid = '%u'", bGuid);
        if (bItems)
        {
            do
            {
                Field* itemsFld = bItems->Fetch();
                uint32 bItmEntry = itemsFld[0].GetUInt32();
                uint32 bItmSlot = itemsFld[1].GetUInt32();
                uint32 bItmContSlot = itemsFld[2].GetUInt32();
                uint32 bItmCnt = itemsFld[3].GetUInt32();

                //Adding items
                uint32 noSpaceForCount = 0;

                // check space and find places
                ItemPosCountVec dest;
                uint8 msg = player->CanStoreNewItem( bItmSlot, bItmContSlot, dest, bItmEntry, bItmCnt, &noSpaceForCount );
                if( msg != EQUIP_ERR_OK )                               // convert to possible store amount
                    bItmCnt -= noSpaceForCount;

                if( bItmCnt == 0 || dest.empty())                         // can't add any
                    continue;

                Item* item = player->StoreNewItem( dest, bItmEntry, true, Item::GenerateItemRandomPropertyId(bItmEntry));

                if(bItmCnt > 0 && item)
                    player->SendNewItem(item,bItmCnt,false,true);

            } while( bItems->NextRow());

            delete bItems;
        }

        delete backup;

        CharacterDatabase.PExecute("UPDATE charactersBckp SET restored = 1 WHERE name = '%s' and race = '%u' and class = '%u'", player->GetName(), player->getRace(), player->getClass());

        PSendSysMessage("Персонаж восстановлен. Удачной игры");

    } else {
        PSendSysMessage("Персонаж с таким именем классом и рассой не найден в бэкапе");
        return true;
    }
    return true;
}

bool ChatHandler::HandleHelpCommand(const char* args)
{
    char* cmd = strtok((char*)args, " ");
    if(!cmd)
    {
        ShowHelpForCommand(getCommandTable(), "help");
        ShowHelpForCommand(getCommandTable(), "");
    }
    else
    {
        if(!ShowHelpForCommand(getCommandTable(), cmd))
            SendSysMessage(LANG_NO_HELP_CMD);
    }

    return true;
}

bool ChatHandler::HandleCommandsCommand(const char* /*args*/)
{
    ShowHelpForCommand(getCommandTable(), "");
    return true;
}

bool ChatHandler::HandleAccountCommand(const char* /*args*/)
{
    AccountTypes gmlevel = m_session->GetSecurity();
    PSendSysMessage(LANG_ACCOUNT_LEVEL, uint32(gmlevel));
    return true;
}

bool ChatHandler::HandleStartCommand(const char* /*args*/)
{
    Player *chr = m_session->GetPlayer();

    if(chr->isInFlight())
    {
        SendSysMessage(LANG_YOU_IN_FLIGHT);
        SetSentErrorMessage(true);
        return false;
    }

    if(chr->isInCombat())
    {
        SendSysMessage(LANG_YOU_IN_COMBAT);
        SetSentErrorMessage(true);
        return false;
    }

    // cast spell Stuck
    chr->CastSpell(chr,7355,false);
    return true;
}

bool ChatHandler::HandleServerInfoCommand(const char* /*args*/)
{
    uint32 activeClientsNum = sWorld.GetActiveSessionCount();
    uint32 queuedClientsNum = sWorld.GetQueuedSessionCount();
    uint32 maxActiveClientsNum = sWorld.GetMaxActiveSessionCount();
    uint32 maxQueuedClientsNum = sWorld.GetMaxQueuedSessionCount();
    std::string str = secsToTimeString(sWorld.GetUptime());

    char const* full;
    if(m_session)
        full = _FULLVERSION(REVISION_DATE,REVISION_TIME,REVISION_NR,"|cffffffff|Hurl:" REVISION_ID "|h" REVISION_ID "|h|r");
    else
        full = _FULLVERSION(REVISION_DATE,REVISION_TIME,REVISION_NR,REVISION_ID);

    SendSysMessage(full);
    //PSendSysMessage(LANG_USING_SCRIPT_LIB,sWorld.GetScriptsVersion());
    //PSendSysMessage(LANG_USING_WORLD_DB,sWorld.GetDBVersion());
    //PSendSysMessage(LANG_USING_EVENT_AI,sWorld.GetCreatureEventAIVersion());
    PSendSysMessage(LANG_CONNECTED_USERS, activeClientsNum, maxActiveClientsNum, queuedClientsNum, maxQueuedClientsNum);
    PSendSysMessage(LANG_UPTIME, str.c_str());

    return true;
}

bool ChatHandler::HandleDismountCommand(const char* /*args*/)
{
    //If player is not mounted, so go out :)
    if (!m_session->GetPlayer( )->IsMounted())
    {
        SendSysMessage(LANG_CHAR_NON_MOUNTED);
        SetSentErrorMessage(true);
        return false;
    }

    if(m_session->GetPlayer( )->isInFlight())
    {
        SendSysMessage(LANG_YOU_IN_FLIGHT);
        SetSentErrorMessage(true);
        return false;
    }

    m_session->GetPlayer()->Unmount();
    m_session->GetPlayer()->RemoveSpellsCausingAura(SPELL_AURA_MOUNTED);
    return true;
}

bool ChatHandler::HandleSaveCommand(const char* /*args*/)
{
    Player *player=m_session->GetPlayer();

    // save GM account without delay and output message (testing, etc)
    if(m_session->GetSecurity() > SEC_PLAYER)
    {
        player->SaveToDB();
        SendSysMessage(LANG_PLAYER_SAVED);
        return true;
    }

    // save or plan save after 20 sec (logout delay) if current next save time more this value and _not_ output any messages to prevent cheat planning
    uint32 save_interval = sWorld.getConfig(CONFIG_INTERVAL_SAVE);
    if(save_interval==0 || save_interval > 20*IN_MILISECONDS && player->GetSaveTimer() <= save_interval - 20*IN_MILISECONDS)
        player->SaveToDB();

    return true;
}

bool ChatHandler::HandleGMListIngameCommand(const char* /*args*/)
{
    bool first = true;

    HashMapHolder<Player>::MapType &m = HashMapHolder<Player>::GetContainer();
    HashMapHolder<Player>::MapType::const_iterator itr = m.begin();
    for(; itr != m.end(); ++itr)
    {
        AccountTypes itr_sec = itr->second->GetSession()->GetSecurity();
        if ((itr->second->isGameMaster() || itr_sec > SEC_PLAYER && itr_sec <= sWorld.getConfig(CONFIG_GM_LEVEL_IN_GM_LIST)) &&
            (!m_session || itr->second->IsVisibleGloballyFor(m_session->GetPlayer())))
        {
            if(first)
            {
                SendSysMessage(LANG_GMS_ON_SRV);
                first = false;
            }

            SendSysMessage(GetNameLink(itr->second).c_str());
        }
    }

    if(first)
        SendSysMessage(LANG_GMS_NOT_LOGGED);

    return true;
}

bool ChatHandler::HandleAccountPasswordCommand(const char* args)
{
    if(!*args)
        return false;

    char *old_pass = strtok ((char*)args, " ");
    char *new_pass = strtok (NULL, " ");
    char *new_pass_c  = strtok (NULL, " ");

    if (!old_pass || !new_pass || !new_pass_c)
        return false;

    std::string password_old = old_pass;
    std::string password_new = new_pass;
    std::string password_new_c = new_pass_c;

    if (password_new != password_new_c)
    {
        SendSysMessage (LANG_NEW_PASSWORDS_NOT_MATCH);
        SetSentErrorMessage (true);
        return false;
    }

    if (!accmgr.CheckPassword (m_session->GetAccountId(), password_old))
    {
        SendSysMessage (LANG_COMMAND_WRONGOLDPASSWORD);
        SetSentErrorMessage (true);
        return false;
    }

    AccountOpResult result = accmgr.ChangePassword(m_session->GetAccountId(), password_new);

    switch(result)
    {
        case AOR_OK:
            SendSysMessage(LANG_COMMAND_PASSWORD);
            break;
        case AOR_PASS_TOO_LONG:
            SendSysMessage(LANG_PASSWORD_TOO_LONG);
            SetSentErrorMessage(true);
            return false;
        case AOR_NAME_NOT_EXIST:                            // not possible case, don't want get account name for output
        default:
            SendSysMessage(LANG_COMMAND_NOTCHANGEPASSWORD);
            SetSentErrorMessage(true);
            return false;
    }

    return true;
}

bool ChatHandler::HandleAccountLockCommand(const char* args)
{
    if (!*args)
    {
        SendSysMessage(LANG_USE_BOL);
        return true;
    }

    std::string argstr = (char*)args;
    if (argstr == "on")
    {
        loginDatabase.PExecute( "UPDATE account SET locked = '1' WHERE id = '%d'",m_session->GetAccountId());
        PSendSysMessage(LANG_COMMAND_ACCLOCKLOCKED);
        return true;
    }

    if (argstr == "off")
    {
        loginDatabase.PExecute( "UPDATE account SET locked = '0' WHERE id = '%d'",m_session->GetAccountId());
        PSendSysMessage(LANG_COMMAND_ACCLOCKUNLOCKED);
        return true;
    }

    SendSysMessage(LANG_USE_BOL);
    return true;
}

/// Display the 'Message of the day' for the realm
bool ChatHandler::HandleServerMotdCommand(const char* /*args*/)
{
    PSendSysMessage(LANG_MOTD_CURRENT, sWorld.GetMotd());
    return true;
}
