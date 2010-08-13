/*
 * Copyright (C) 2005-2010 MaNGOS <http://getmangos.com/>
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
#include "ObjectMgr.h"
#include "Language.h"
#include "AccountMgr.h"
#include "SystemConfig.h"
#include "revision.h"
#include "revision_nr.h"
#include "Util.h"
#include "ObjectMgr.h"
#include "VMapFactory.h"

#ifndef sMapMgr
	#include "MapManager.h"
#endif

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
    AccountTypes gmlevel = GetAccessLevel();
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
        full = _FULLVERSION(REVISION_DATE,REVISION_TIME,"1738","|cffffffff|Hurl:" REVISION_ID "|h" REVISION_ID "|h|r");
    else
        full = _FULLVERSION(REVISION_DATE,REVISION_TIME,"1738",REVISION_ID);

    SendSysMessage(full);
    //PSendSysMessage(LANG_USING_SCRIPT_LIB,sWorld.GetScriptsVersion());
    //PSendSysMessage(LANG_USING_WORLD_DB,sWorld.GetDBVersion());
    //PSendSysMessage(LANG_USING_EVENT_AI,sWorld.GetCreatureEventAIVersion());
    PSendSysMessage(LANG_CONNECTED_USERS, activeClientsNum, maxActiveClientsNum, queuedClientsNum, maxQueuedClientsNum);
    PSendSysMessage(LANG_UPTIME, str.c_str());
    PSendSysMessage("Statistics counting: max spell handle time %u ms", sStatMgr.spell_work.first);
    PSendSysMessage("Statistics counting: max spell handle time spell id %u", sStatMgr.spell_work.second);
    //calculate total memory reserved by all SQL storages
    uint32 total_mem = 0;
    total_mem += sCreatureStorage.GetTotalSize() + sCreatureDataAddonStorage.GetTotalSize() +
					sCreatureModelStorage.GetTotalSize() + sCreatureInfoAddonStorage.GetTotalSize() +
					sEquipmentStorage.GetTotalSize() + sGOStorage.GetTotalSize() +
					sItemStorage.GetTotalSize() + sPageTextStore.GetTotalSize() +
					sInstanceTemplate.GetTotalSize();
    PSendSysMessage("Total memory use by SQL storages: %u mb", total_mem / 1024 / 1024);
    PSendSysMessage("sObjectMgr memory usage: %u mb", sObjectMgr.GetMemoryUsage() / 1024 / 1024);
    PSendSysMessage("VMAP memory usage: %u mb", VMAP::VMapFactory::createOrGetVMapManager()->getMemUsage() / 1024 / 1024);
    PSendSysMessage("Grid's map size: %u mb", sMapMgr.GetTotalGridsSize() / 1024 / 1024);
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
    if(GetAccessLevel() > SEC_PLAYER)
    {
        player->SaveToDB();
        SendSysMessage(LANG_PLAYER_SAVED);
        return true;
    }

    // save or plan save after 20 sec (logout delay) if current next save time more this value and _not_ output any messages to prevent cheat planning
    uint32 save_interval = sWorld.getConfig(CONFIG_UINT32_INTERVAL_SAVE);
    if (save_interval==0 || (save_interval > 20*IN_MILLISECONDS && player->GetSaveTimer() <= save_interval - 20*IN_MILLISECONDS))
        player->SaveToDB();

    return true;
}

bool ChatHandler::HandleGMListIngameCommand(const char* /*args*/)
{
    bool first = true;

    {
        HashMapHolder<Player>::ReadGuard g(HashMapHolder<Player>::GetLock());
        HashMapHolder<Player>::MapType &m = sObjectAccessor.GetPlayers();
        for(HashMapHolder<Player>::MapType::const_iterator itr = m.begin(); itr != m.end(); ++itr)
        {
            AccountTypes itr_sec = itr->second->GetSession()->GetSecurity();
            if ((itr->second->isGameMaster() || (itr_sec > SEC_PLAYER && itr_sec <= (AccountTypes)sWorld.getConfig(CONFIG_UINT32_GM_LEVEL_IN_GM_LIST))) &&
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
    }

    if(first)
        SendSysMessage(LANG_GMS_NOT_LOGGED);

    return true;
}

bool ChatHandler::HandleAccountPasswordCommand(const char* args)
{
    // allow use from RA, but not from console (not have associated account id)
    if (!GetAccountId())
        return false;

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

    if (!sAccountMgr.CheckPassword (GetAccountId(), password_old))
    {
        SendSysMessage (LANG_COMMAND_WRONGOLDPASSWORD);
        SetSentErrorMessage (true);
        return false;
    }

    AccountOpResult result = sAccountMgr.ChangePassword(GetAccountId(), password_new);

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
    // allow use from RA, but not from console (not have associated account id)
    if (!GetAccountId())
        return false;

    if (!*args)
    {
        SendSysMessage(LANG_USE_BOL);
        return true;
    }

    std::string argstr = (char*)args;
    if (argstr == "on")
    {
        LoginDatabase.PExecute( "UPDATE account SET locked = '1' WHERE id = '%d'",GetAccountId());
        PSendSysMessage(LANG_COMMAND_ACCLOCKLOCKED);
        return true;
    }

    if (argstr == "off")
    {
        LoginDatabase.PExecute( "UPDATE account SET locked = '0' WHERE id = '%d'",GetAccountId());
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

bool ChatHandler::HandleCharDisplayMainhandCommand(const char* args)
{
    if(!*args)
        return false;
    char* cId = extractKeyFromLink((char*)args,"Hitem");
    if(!cId)
        return false;

    uint32 newItem = (uint32)atol(cId);

    Player* pl = m_session->GetPlayer();

    if(!pl->m_vis)
        pl->m_vis = new Visuals;

    if (pl->HandleChangeSlotModel(PLAYER_VISIBLE_ITEM_16_ENTRYID, newItem, 15))
    {
        pl->m_vis->m_visMainhand = newItem;
        return true;
    }
    else
        return false;
}

bool ChatHandler::HandleCharDisplayHeadCommand(const char* args)
{
    if(!*args)
        return false;
    char* cId = extractKeyFromLink((char*)args,"Hitem");
    if(!cId)
        return false;

    uint32 newItem = (uint32)atol(cId);

    Player* pl = m_session->GetPlayer();

    if(!pl->m_vis)
        pl->m_vis = new Visuals;

    if (pl->HandleChangeSlotModel(PLAYER_VISIBLE_ITEM_1_ENTRYID, newItem, 0))
    {
        pl->m_vis->m_visHead = newItem;
        return true;
    }
    else
        return false;
}

bool ChatHandler::HandleCharDisplayShouldersCommand(const char* args)
{
    if(!*args)
        return false;
    char* cId = extractKeyFromLink((char*)args,"Hitem");
    if(!cId)
        return false;

    uint32 newItem = (uint32)atol(cId);

    Player* pl = m_session->GetPlayer();

    if(!pl->m_vis)
        pl->m_vis = new Visuals;

    if (pl->HandleChangeSlotModel(PLAYER_VISIBLE_ITEM_3_ENTRYID, newItem, 2))
    {
        pl->m_vis->m_visShoulders = newItem;
        return true;
    }
    else
        return false;
}

bool ChatHandler::HandleCharDisplayChestCommand(const char* args)
{
    if(!*args)
        return false;
    char* cId = extractKeyFromLink((char*)args,"Hitem");
    if(!cId)
        return false;

    uint32 newItem = (uint32)atol(cId);

    Player* pl = m_session->GetPlayer();

    if(!pl->m_vis)
        pl->m_vis = new Visuals;

    if (pl->HandleChangeSlotModel(PLAYER_VISIBLE_ITEM_5_ENTRYID, newItem, 4))
    {
        pl->m_vis->m_visChest = newItem;
        return true;
    }
    else
        return false;
}

bool ChatHandler::HandleCharDisplayWaistCommand(const char* args)
{
    if(!*args)
        return false;
    char* cId = extractKeyFromLink((char*)args,"Hitem");
    if(!cId)
        return false;

    uint32 newItem = (uint32)atol(cId);

    Player* pl = m_session->GetPlayer();

    if(!pl->m_vis)
        pl->m_vis = new Visuals;

    if (pl->HandleChangeSlotModel(PLAYER_VISIBLE_ITEM_6_ENTRYID, newItem, 5))
    {
        pl->m_vis->m_visWaist = newItem;
        return true;
    }
    else
        return false;
}

bool ChatHandler::HandleCharDisplayLegsCommand(const char* args)
{
    if(!*args)
        return false;
    char* cId = extractKeyFromLink((char*)args,"Hitem");
    if(!cId)
        return false;

    uint32 newItem = (uint32)atol(cId);

    Player* pl = m_session->GetPlayer();

    if(!pl->m_vis)
        pl->m_vis = new Visuals;

    if (pl->HandleChangeSlotModel(PLAYER_VISIBLE_ITEM_7_ENTRYID, newItem, 6))
    {
        pl->m_vis->m_visLegs = newItem;
        return true;
    }
    else
        return false;
}

bool ChatHandler::HandleCharDisplayFeetCommand(const char* args)
{
    if(!*args)
        return false;
    char* cId = extractKeyFromLink((char*)args,"Hitem");
    if(!cId)
        return false;

    uint32 newItem = (uint32)atol(cId);

    Player* pl = m_session->GetPlayer();

    if(!pl->m_vis)
        pl->m_vis = new Visuals;

    if (pl->HandleChangeSlotModel(PLAYER_VISIBLE_ITEM_8_ENTRYID, newItem, 7))
    {
        pl->m_vis->m_visFeet = newItem;
        return true;
    }
    else
        return false;
}

bool ChatHandler::HandleCharDisplayWristsCommand(const char* args)
{
    if(!*args)
        return false;
    char* cId = extractKeyFromLink((char*)args,"Hitem");
    if(!cId)
        return false;

    uint32 newItem = (uint32)atol(cId);

    Player* pl = m_session->GetPlayer();

    if(!pl->m_vis)
        pl->m_vis = new Visuals;

    if (pl->HandleChangeSlotModel(PLAYER_VISIBLE_ITEM_9_ENTRYID, newItem, 8))
    {
        pl->m_vis->m_visWrists = newItem;
        return true;
    }
    else
        return false;
}

bool ChatHandler::HandleCharDisplayHandsCommand(const char* args) //Суки, покажите свои руки!
{
    if(!*args)
        return false;
    char* cId = extractKeyFromLink((char*)args,"Hitem");
    if(!cId)
        return false;

    uint32 newItem = (uint32)atol(cId);

    Player* pl = m_session->GetPlayer();

    if(!pl->m_vis)
        pl->m_vis = new Visuals;

    if (pl->HandleChangeSlotModel(PLAYER_VISIBLE_ITEM_10_ENTRYID, newItem, 9))
    {
        pl->m_vis->m_visHands = newItem;
        return true;
    }
    else
        return false;
}

bool ChatHandler::HandleCharDisplayBackCommand(const char* args)
{
    if(!*args)
        return false;
    char* cId = extractKeyFromLink((char*)args,"Hitem");
    if(!cId)
        return false;

    uint32 newItem = (uint32)atol(cId);

    Player* pl = m_session->GetPlayer();

    if(!pl->m_vis)
        pl->m_vis = new Visuals;

    if (pl->HandleChangeSlotModel(PLAYER_VISIBLE_ITEM_15_ENTRYID, newItem, 14))
    {
        pl->m_vis->m_visBack = newItem;
        return true;
    }
    else
        return false;
}

bool ChatHandler::HandleCharDisplayOffhandCommand(const char* args)
{
    if(!*args)
        return false;
    char* cId = extractKeyFromLink((char*)args,"Hitem");
    if(!cId)
        return false;

    uint32 newItem = (uint32)atol(cId);

    Player* pl = m_session->GetPlayer();

    if(!pl->m_vis)
        pl->m_vis = new Visuals;

    if (pl->HandleChangeSlotModel(PLAYER_VISIBLE_ITEM_17_ENTRYID, newItem, 16))
    {
        pl->m_vis->m_visOffhand = newItem;
        return true;
    }
    else
        return false;
}

bool ChatHandler::HandleCharDisplayRangedCommand(const char* args)
{
    if(!*args)
        return false;
    char* cId = extractKeyFromLink((char*)args,"Hitem");
    if(!cId)
        return false;

    uint32 newItem = (uint32)atol(cId);

    Player* pl = m_session->GetPlayer();

    if(!pl->m_vis)
        pl->m_vis = new Visuals;

    if (pl->HandleChangeSlotModel(PLAYER_VISIBLE_ITEM_18_ENTRYID, newItem, 17))
    {
        pl->m_vis->m_visRanged = newItem;
        return true;
    }
    else
        return false;
}

bool ChatHandler::HandleVoteMuteCommand(const char* args)
{
    if(sStatMgr.to_mute_GUID)
    {
        SendSysMessage("Voting player is already in progress...");
        SetSentErrorMessage(true);
        return false;
    }

    if(!*args)
        return false;

    std::string name = extractPlayerNameFromLink((char*)args);
    if (name.empty())
        return false;

    Player* pl = sObjectMgr.GetPlayer(name.c_str());
    if(!pl)
    {
        SendSysMessage(LANG_PLAYER_NOT_FOUND);
        SetSentErrorMessage(true);
        return false;
    }

    if(pl->GetSession()->GetSecurity() > SEC_PLAYER)
    {
        SendSysMessage(LANG_YOURS_SECURITY_IS_LOW);
        SetSentErrorMessage(true);
        return false;
    }

    if(pl->GetSession()->m_muteTime)
    {
        SendSysMessage("Already muted.");
        SetSentErrorMessage(true);
        return false;
    }

    sStatMgr.to_mute_GUID = pl->GetGUID();
    sStatMgr.mute_counter = time(NULL) + 60;
    sStatMgr.mute_votes[m_session->GetPlayer()->GetGUIDLow()] = true;
    sStatMgr.mute_chat_team = pl->GetTeam();

    std::stringstream argstr;
    argstr << m_session->GetPlayer()->GetName() << " called a vote to mute player " << pl->GetName() << "\n";
    argstr << "Print \".vote yes\" to agree, \".vote no\" to disagree in next 60 seconds.";
    sWorld.SendTeamText(sStatMgr.mute_chat_team, LANG_SYSTEMMESSAGE, argstr.str().c_str());
}

bool ChatHandler::HandleVoteYesCommand(const char* args)
{
    if(!sStatMgr.to_mute_GUID)
    {
        SendSysMessage("Voting is not in progress...");
        SetSentErrorMessage(true);
        return false;
    }

    if(sStatMgr.mute_votes.find(m_session->GetPlayer()->GetGUIDLow()) != sStatMgr.mute_votes.end())
        return true;

    sStatMgr.mute_votes[m_session->GetPlayer()->GetGUIDLow()] = true;

    std::stringstream argstr;
    argstr << m_session->GetPlayer()->GetName() << " voted yes.";

    WorldPacket data(SMSG_NOTIFICATION, (argstr.str().size()+1));
    data << argstr.str();
    sWorld.SendGlobalMessage(&data, NULL, sStatMgr.mute_chat_team);

    if(sStatMgr.mute_votes.size() >= 10)
    {
        uint8 votes = 0;
        for(std::map<uint32, bool>::const_iterator itr = sStatMgr.mute_votes.begin(); itr != sStatMgr.mute_votes.end(); ++itr)
            if((*itr).second)
                ++votes;

        Player *pl = sObjectMgr.GetPlayer(sStatMgr.to_mute_GUID);
        uint32 account_id = sObjectMgr.GetPlayerAccountIdByGUID(sStatMgr.to_mute_GUID);
        time_t mutetime = time(NULL) + votes*60;

        if(pl)
        {
            pl->GetSession()->m_muteTime = mutetime;
            ChatHandler(pl).PSendSysMessage(LANG_YOUR_CHAT_DISABLED, votes);
        }

        LoginDatabase.PExecute("UPDATE account SET mutetime = " UI64FMTD " WHERE id = '%u'", uint64(mutetime), account_id);
        sStatMgr.to_mute_GUID = 0;
        sStatMgr.mute_counter = 0;
        sStatMgr.mute_votes.clear();

        std::stringstream mutestr;
        if(pl)
            mutestr << pl->GetName();
        else
            mutestr << (uint32)account_id;
        mutestr << " was muted for " << (uint32)votes << " minutes.";
        sWorld.SendTeamText(sStatMgr.mute_chat_team, LANG_SYSTEMMESSAGE, mutestr.str().c_str());
    }
}

bool ChatHandler::HandleVoteNoCommand(const char* args)
{
    if(!sStatMgr.to_mute_GUID)
    {
        SendSysMessage("Voting is not in progress...");
        SetSentErrorMessage(true);
        return false;
    }

    if(m_session->GetPlayer()->GetGUID() == sStatMgr.to_mute_GUID ||  //do not allow called player to vote himself
        sStatMgr.mute_votes.find(m_session->GetPlayer()->GetGUIDLow()) != sStatMgr.mute_votes.end())
        return true;

    sStatMgr.mute_votes[m_session->GetPlayer()->GetGUIDLow()] = false;

    std::stringstream argstr;
    argstr << m_session->GetPlayer()->GetName() << " voted no.";

    WorldPacket data(SMSG_NOTIFICATION, (argstr.str().size()+1));
    data << argstr.str();
    sWorld.SendGlobalMessage(&data, NULL, sStatMgr.mute_chat_team);

    if(sStatMgr.mute_votes.size() >= 10)
    {
        uint8 votes = 0;
        for(std::map<uint32, bool>::const_iterator itr = sStatMgr.mute_votes.begin(); itr != sStatMgr.mute_votes.end(); ++itr)
            if((*itr).second)
                ++votes;

        Player *pl = sObjectMgr.GetPlayer(sStatMgr.to_mute_GUID);
        uint32 account_id = sObjectMgr.GetPlayerAccountIdByGUID(sStatMgr.to_mute_GUID);
        time_t mutetime = time(NULL) + votes*60;

        if(pl)
        {
            pl->GetSession()->m_muteTime = mutetime;
            ChatHandler(pl).PSendSysMessage(LANG_YOUR_CHAT_DISABLED, votes);
        }

        LoginDatabase.PExecute("UPDATE account SET mutetime = " UI64FMTD " WHERE id = '%u'", uint64(mutetime), account_id);
        sStatMgr.to_mute_GUID = 0;
        sStatMgr.mute_counter = 0;
        sStatMgr.mute_votes.clear();

        std::stringstream mutestr;
        if(pl)
            mutestr << pl->GetName();
        else
            mutestr << (uint32)account_id;
        mutestr << " was muted for " << (uint32)votes << " minutes.";
        sWorld.SendTeamText(sStatMgr.mute_chat_team, LANG_SYSTEMMESSAGE, mutestr.str().c_str());
    }
}
