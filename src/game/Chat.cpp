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
#include "Language.h"
#include "Database/DatabaseEnv.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Log.h"
#include "World.h"
#include "ObjectMgr.h"
#include "ObjectGuid.h"
#include "Player.h"
#include "UpdateMask.h"
#include "Chat.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"
#include "AccountMgr.h"
#include "SpellMgr.h"
#include "PoolManager.h"
#include "GameEventMgr.h"

// Supported shift-links (client generated and server side)
// |color|Hachievement:achievement_id:player_guid:0:0:0:0:0:0:0:0|h[name]|h|r
//                                                                        - client, item icon shift click, not used in server currently
// |color|Harea:area_id|h[name]|h|r
// |color|Hcreature:creature_guid|h[name]|h|r
// |color|Hcreature_entry:creature_id|h[name]|h|r
// |color|Henchant:recipe_spell_id|h[prof_name: recipe_name]|h|r          - client, at shift click in recipes list dialog
// |color|Hgameevent:id|h[name]|h|r
// |color|Hgameobject:go_guid|h[name]|h|r
// |color|Hgameobject_entry:go_id|h[name]|h|r
// |color|Hglyph:glyph_slot_id:glyph_prop_id|h[%s]|h|r                    - client, at shift click in glyphs dialog, GlyphSlot.dbc, GlyphProperties.dbc
// |color|Hitem:item_id:perm_ench_id:gem1:gem2:gem3:0:0:0:0:reporter_level|h[name]|h|r
//                                                                        - client, item icon shift click
// |color|Hitemset:itemset_id|h[name]|h|r
// |color|Hplayer:name|h[name]|h|r                                        - client, in some messages, at click copy only name instead link
// |color|Hquest:quest_id:quest_level|h[name]|h|r                         - client, quest list name shift-click
// |color|Hskill:skill_id|h[name]|h|r
// |color|Hspell:spell_id|h[name]|h|r                                     - client, spellbook spell icon shift-click
// |color|Htalent:talent_id,rank|h[name]|h|r                              - client, talent icon shift-click
// |color|Htaxinode:id|h[name]|h|r
// |color|Htele:id|h[name]|h|r
// |color|Htitle:id|h[name]|h|r
// |color|Htrade:spell_id,cur_value,max_value,unk3int,unk3str|h[name]|h|r - client, spellbook profession icon shift-click

bool ChatHandler::load_command_table = true;

ChatCommand * ChatHandler::getCommandTable()
{
    static ChatCommand accountSetCommandTable[] =
    {
        { "addon",          SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleAccountSetAddonCommand,     "", NULL },
        { "gmlevel",        SEC_CONSOLE,        true,  &ChatHandler::HandleAccountSetGmLevelCommand,   "", NULL },
        { "password",       SEC_CONSOLE,        true,  &ChatHandler::HandleAccountSetPasswordCommand,  "", NULL },
        { NULL,             0,                  false, NULL,                                           "", NULL }
    };

    static ChatCommand accountCommandTable[] =
    {
        { "characters",     SEC_CONSOLE,        true,  &ChatHandler::HandleAccountCharactersCommand,   "", NULL },
        { "create",         SEC_CONSOLE,        true,  &ChatHandler::HandleAccountCreateCommand,       "", NULL },
        { "delete",         SEC_CONSOLE,        true,  &ChatHandler::HandleAccountDeleteCommand,       "", NULL },
        { "onlinelist",     SEC_CONSOLE,        true,  &ChatHandler::HandleAccountOnlineListCommand,   "", NULL },
        { "lock",           SEC_PLAYER,         true,  &ChatHandler::HandleAccountLockCommand,         "", NULL },
        { "set",            SEC_ADMINISTRATOR,  true,  NULL,                                           "", accountSetCommandTable },
        { "password",       SEC_PLAYER,         true,  &ChatHandler::HandleAccountPasswordCommand,     "", NULL },
        { "",               SEC_PLAYER,         true,  &ChatHandler::HandleAccountCommand,             "", NULL },
        { NULL,             0,                  false, NULL,                                           "", NULL }
    };

    static ChatCommand banCommandTable[] =
    {
        { "account",        SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleBanAccountCommand,          "", NULL },
        { "character",      SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleBanCharacterCommand,        "", NULL },
        { "ip",             SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleBanIPCommand,               "", NULL },
        { NULL,             0,                  false, NULL,                                           "", NULL }
    };

    static ChatCommand baninfoCommandTable[] =
    {
        { "account",        SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleBanInfoAccountCommand,      "", NULL },
        { "character",      SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleBanInfoCharacterCommand,    "", NULL },
        { "ip",             SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleBanInfoIPCommand,           "", NULL },
        { NULL,             0,                  false, NULL,                                           "", NULL }
    };

    static ChatCommand banlistCommandTable[] =
    {
        { "account",        SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleBanListAccountCommand,      "", NULL },
        { "character",      SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleBanListCharacterCommand,    "", NULL },
        { "ip",             SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleBanListIPCommand,           "", NULL },
        { NULL,             0,                  false, NULL,                                           "", NULL }
    };

    static ChatCommand castCommandTable[] =
    {
        { "back",           SEC_ADMINISTRATOR,  false, &ChatHandler::HandleCastBackCommand,            "", NULL },
        { "dist",           SEC_ADMINISTRATOR,  false, &ChatHandler::HandleCastDistCommand,            "", NULL },
        { "self",           SEC_ADMINISTRATOR,  false, &ChatHandler::HandleCastSelfCommand,            "", NULL },
        { "target",         SEC_ADMINISTRATOR,  false, &ChatHandler::HandleCastTargetCommand,          "", NULL },
        { "",               SEC_ADMINISTRATOR,  false, &ChatHandler::HandleCastCommand,                "", NULL },
        { NULL,             0,                  false, NULL,                                           "", NULL }
    };

    static ChatCommand characterDeletedCommandTable[] =
    {
        { "delete",         SEC_CONSOLE,        true,  &ChatHandler::HandleCharacterDeletedDeleteCommand, "", NULL },
        { "list",           SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleCharacterDeletedListCommand,"", NULL },
        { "restore",        SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleCharacterDeletedRestoreCommand,"", NULL },
        { "old",            SEC_CONSOLE,        true,  &ChatHandler::HandleCharacterDeletedOldCommand, "", NULL },
        { NULL,             0,                  false, NULL,                                           "", NULL }
    };

    static ChatCommand characterCommandTable[] =
    {
        { "customize",      SEC_GAMEMASTER,     true,  &ChatHandler::HandleCharacterCustomizeCommand,  "", NULL },
        { "deleted",        SEC_GAMEMASTER,     true,  NULL,                                           "", characterDeletedCommandTable},
        { "erase",          SEC_CONSOLE,        true,  &ChatHandler::HandleCharacterEraseCommand,      "", NULL },
        { "level",          SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleCharacterLevelCommand,      "", NULL },
        { "rename",         SEC_GAMEMASTER,     true,  &ChatHandler::HandleCharacterRenameCommand,     "", NULL },
        { "reputation",     SEC_GAMEMASTER,     true,  &ChatHandler::HandleCharacterReputationCommand, "", NULL },
        { "titles",         SEC_GAMEMASTER,     true,  &ChatHandler::HandleCharacterTitlesCommand,     "", NULL },

        { "head",           SEC_PLAYER,     true,  &ChatHandler::HandleCharDisplayHeadCommand,     "", NULL },
        { "shoulders",      SEC_PLAYER,     true,  &ChatHandler::HandleCharDisplayShouldersCommand,     "", NULL },
        { "chest",          SEC_PLAYER,     true,  &ChatHandler::HandleCharDisplayChestCommand,     "", NULL },
        { "waist",          SEC_PLAYER,     true,  &ChatHandler::HandleCharDisplayWaistCommand,     "", NULL },
        { "legs",           SEC_PLAYER,     true,  &ChatHandler::HandleCharDisplayLegsCommand,     "", NULL },
        { "feet",           SEC_PLAYER,     true,  &ChatHandler::HandleCharDisplayFeetCommand,     "", NULL },
        { "wrists",         SEC_PLAYER,     true,  &ChatHandler::HandleCharDisplayWristsCommand,     "", NULL },
        { "hands",          SEC_PLAYER,     true,  &ChatHandler::HandleCharDisplayHandsCommand,     "", NULL },
        { "back",           SEC_PLAYER,     true,  &ChatHandler::HandleCharDisplayBackCommand,     "", NULL },
        { "mainhand",       SEC_PLAYER,     true,  &ChatHandler::HandleCharDisplayMainhandCommand,     "", NULL },
        { "offhand",        SEC_PLAYER,     true,  &ChatHandler::HandleCharDisplayOffhandCommand,     "", NULL },
        { "ranged",         SEC_PLAYER,     true,  &ChatHandler::HandleCharDisplayRangedCommand,     "", NULL },

        { NULL,             0,                  false, NULL,                                           "", NULL }
    };

    static ChatCommand debugPlayCommandTable[] =
    {
        { "cinematic",      SEC_MODERATOR,      false, &ChatHandler::HandleDebugPlayCinematicCommand,       "", NULL },
        { "movie",          SEC_MODERATOR,      false, &ChatHandler::HandleDebugPlayMovieCommand,           "", NULL },
        { "sound",          SEC_MODERATOR,      false, &ChatHandler::HandleDebugPlaySoundCommand,           "", NULL },
        { NULL,             0,                  false, NULL,                                                "", NULL }
    };

    static ChatCommand debugSendCommandTable[] =
    {
        { "buyerror",       SEC_ADMINISTRATOR,  false, &ChatHandler::HandleDebugSendBuyErrorCommand,        "", NULL },
        { "channelnotify",  SEC_ADMINISTRATOR,  false, &ChatHandler::HandleDebugSendChannelNotifyCommand,   "", NULL },
        { "chatmmessage",   SEC_ADMINISTRATOR,  false, &ChatHandler::HandleDebugSendChatMsgCommand,         "", NULL },
        { "equiperror",     SEC_ADMINISTRATOR,  false, &ChatHandler::HandleDebugSendEquipErrorCommand,      "", NULL },
        { "largepacket",    SEC_ADMINISTRATOR,  false, &ChatHandler::HandleDebugSendLargePacketCommand,     "", NULL },
        { "opcode",         SEC_ADMINISTRATOR,  false, &ChatHandler::HandleDebugSendOpcodeCommand,          "", NULL },
        { "poi",            SEC_ADMINISTRATOR,  false, &ChatHandler::HandleDebugSendPoiCommand,             "", NULL },
        { "qpartymsg",      SEC_ADMINISTRATOR,  false, &ChatHandler::HandleDebugSendQuestPartyMsgCommand,   "", NULL },
        { "qinvalidmsg",    SEC_ADMINISTRATOR,  false, &ChatHandler::HandleDebugSendQuestInvalidMsgCommand, "", NULL },
        { "sellerror",      SEC_ADMINISTRATOR,  false, &ChatHandler::HandleDebugSendSellErrorCommand,       "", NULL },
        { "setphaseshift",  SEC_ADMINISTRATOR,  false, &ChatHandler::HandleDebugSendSetPhaseShiftCommand,   "", NULL },
        { "spellfail",      SEC_ADMINISTRATOR,  false, &ChatHandler::HandleDebugSendSpellFailCommand,       "", NULL },
        { NULL,             0,                  false, NULL,                                                "", NULL }
    };

    static ChatCommand debugCommandTable[] =
    {
        { "anim",           SEC_GAMEMASTER,     false, &ChatHandler::HandleDebugAnimCommand,                "", NULL },
        { "arena",          SEC_ADMINISTRATOR,  false, &ChatHandler::HandleDebugArenaCommand,               "", NULL },
        { "bg",             SEC_ADMINISTRATOR,  false, &ChatHandler::HandleDebugBattlegroundCommand,        "", NULL },
        { "getitemstate",   SEC_ADMINISTRATOR,  false, &ChatHandler::HandleDebugGetItemStateCommand,        "", NULL },
        { "lootrecipient",  SEC_GAMEMASTER,     false, &ChatHandler::HandleDebugGetLootRecipientCommand,    "", NULL },
        { "getvalue",       SEC_ADMINISTRATOR,  false, &ChatHandler::HandleDebugGetValueCommand,            "", NULL },
        { "getitemvalue",   SEC_ADMINISTRATOR,  false, &ChatHandler::HandleDebugGetItemValueCommand,        "", NULL },
        { "Mod32Value",     SEC_ADMINISTRATOR,  false, &ChatHandler::HandleDebugMod32ValueCommand,          "", NULL },
        { "play",           SEC_MODERATOR,      false, NULL,                                                "", debugPlayCommandTable },
        { "send",           SEC_ADMINISTRATOR,  false, NULL,                                                "", debugSendCommandTable },
        { "setaurastate",   SEC_ADMINISTRATOR,  false, &ChatHandler::HandleDebugSetAuraStateCommand,        "", NULL },
        { "setitemvalue",   SEC_ADMINISTRATOR,  false, &ChatHandler::HandleDebugSetItemValueCommand,        "", NULL },
        { "setvalue",       SEC_ADMINISTRATOR,  false, &ChatHandler::HandleDebugSetValueCommand,            "", NULL },
        { "spellcheck",     SEC_CONSOLE,        true,  &ChatHandler::HandleDebugSpellCheckCommand,          "", NULL },
        { "spawnvehicle",   SEC_ADMINISTRATOR,  false, &ChatHandler::HandleDebugSpawnVehicle,               "", NULL },
        { "uws",            SEC_ADMINISTRATOR,  false, &ChatHandler::HandleDebugUpdateWorldStateCommand,    "", NULL },
        { "update",         SEC_ADMINISTRATOR,  false, &ChatHandler::HandleDebugUpdateCommand,              "", NULL },
        { NULL,             0,                  false, NULL,                                                "", NULL }
    };

    static ChatCommand eventCommandTable[] =
    {
        { "list",           SEC_GAMEMASTER,     true,  &ChatHandler::HandleEventListCommand,           "", NULL },
        { "start",          SEC_GAMEMASTER,     true,  &ChatHandler::HandleEventStartCommand,          "", NULL },
        { "stop",           SEC_GAMEMASTER,     true,  &ChatHandler::HandleEventStopCommand,           "", NULL },
        { "startmurlocs",   SEC_GAMEMASTER,     true,  &ChatHandler::HandleStartMurlocsCommand,        "", NULL },
        { "endmurlocs",     SEC_GAMEMASTER,     true,  &ChatHandler::HandleEndMurlocsCommand,          "", NULL },
        { "",               SEC_GAMEMASTER,     true,  &ChatHandler::HandleEventInfoCommand,           "", NULL },
        { NULL,             0,                  false, NULL,                                           "", NULL }
    };

    static ChatCommand gmCommandTable[] =
    {
        { "announce",       SEC_MODERATOR,      true,  &ChatHandler::HandleGMAnnounceCommand,          "", NULL },
        { "chat",           SEC_MODERATOR,      false, &ChatHandler::HandleGMChatCommand,              "", NULL },
        { "fly",            SEC_ADMINISTRATOR,  false, &ChatHandler::HandleGMFlyCommand,               "", NULL },
        { "ingame",         SEC_PLAYER,         true,  &ChatHandler::HandleGMListIngameCommand,        "", NULL },
        { "list",           SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleGMListFullCommand,          "", NULL },
        { "visible",        SEC_MODERATOR,      false, &ChatHandler::HandleGMVisibleCommand,           "", NULL },
        { "",               SEC_MODERATOR,      false, &ChatHandler::HandleGMCommand,                  "", NULL },
        { NULL,             0,                  false, NULL,                                           "", NULL }
    };

    static ChatCommand goCommandTable[] =
    {
        { "creature",       SEC_MODERATOR,      false, &ChatHandler::HandleGoCreatureCommand,          "", NULL },
        { "graveyard",      SEC_MODERATOR,      false, &ChatHandler::HandleGoGraveyardCommand,         "", NULL },
        { "grid",           SEC_MODERATOR,      false, &ChatHandler::HandleGoGridCommand,              "", NULL },
        { "object",         SEC_MODERATOR,      false, &ChatHandler::HandleGoObjectCommand,            "", NULL },
        { "taxinode",       SEC_MODERATOR,      false, &ChatHandler::HandleGoTaxinodeCommand,          "", NULL },
        { "trigger",        SEC_MODERATOR,      false, &ChatHandler::HandleGoTriggerCommand,           "", NULL },
        { "zonexy",         SEC_MODERATOR,      false, &ChatHandler::HandleGoZoneXYCommand,            "", NULL },
        { "xy",             SEC_MODERATOR,      false, &ChatHandler::HandleGoXYCommand,                "", NULL },
        { "xyz",            SEC_MODERATOR,      false, &ChatHandler::HandleGoXYZCommand,               "", NULL },
        { "",               SEC_MODERATOR,      false, &ChatHandler::HandleGoCommand,                  "", NULL },
        { NULL,             0,                  false, NULL,                                           "", NULL }
    };

    static ChatCommand gobjectCommandTable[] =
    {
        { "add",            SEC_GAMEMASTER,     false, &ChatHandler::HandleGameObjectAddCommand,       "", NULL },
        { "delete",         SEC_GAMEMASTER,     false, &ChatHandler::HandleGameObjectDeleteCommand,    "", NULL },
        { "move",           SEC_GAMEMASTER,     false, &ChatHandler::HandleGameObjectMoveCommand,      "", NULL },
        { "near",           SEC_GAMEMASTER,     false, &ChatHandler::HandleGameObjectNearCommand,      "", NULL },
        { "setphase",       SEC_GAMEMASTER,     false, &ChatHandler::HandleGameObjectPhaseCommand,     "", NULL },
        { "target",         SEC_GAMEMASTER,     false, &ChatHandler::HandleGameObjectTargetCommand,    "", NULL },
        { "turn",           SEC_GAMEMASTER,     false, &ChatHandler::HandleGameObjectTurnCommand,      "", NULL },
        { NULL,             0,                  false, NULL,                                           "", NULL }
    };

    static ChatCommand guildCommandTable[] =
    {
        { "create",         SEC_GAMEMASTER,     true,  &ChatHandler::HandleGuildCreateCommand,         "", NULL },
        { "delete",         SEC_GAMEMASTER,     true,  &ChatHandler::HandleGuildDeleteCommand,         "", NULL },
        { "invite",         SEC_GAMEMASTER,     true,  &ChatHandler::HandleGuildInviteCommand,         "", NULL },
        { "uninvite",       SEC_GAMEMASTER,     true,  &ChatHandler::HandleGuildUninviteCommand,       "", NULL },
        { "rank",           SEC_GAMEMASTER,     true,  &ChatHandler::HandleGuildRankCommand,           "", NULL },
        { NULL,             0,                  false, NULL,                                           "", NULL }
    };

    static ChatCommand honorCommandTable[] =
    {
        { "add",            SEC_GAMEMASTER,     false, &ChatHandler::HandleHonorAddCommand,            "", NULL },
        { "addkill",        SEC_GAMEMASTER,     false, &ChatHandler::HandleHonorAddKillCommand,        "", NULL },
        { "update",         SEC_GAMEMASTER,     false, &ChatHandler::HandleHonorUpdateCommand,         "", NULL },
        { NULL,             0,                  false, NULL,                                           "", NULL }
    };

    static ChatCommand instanceCommandTable[] =
    {
        { "listbinds",      SEC_ADMINISTRATOR,  false, &ChatHandler::HandleInstanceListBindsCommand,   "", NULL },
        { "unbind",         SEC_ADMINISTRATOR,  false, &ChatHandler::HandleInstanceUnbindCommand,      "", NULL },
        { "stats",          SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleInstanceStatsCommand,       "", NULL },
        { "savedata",       SEC_ADMINISTRATOR,  false, &ChatHandler::HandleInstanceSaveDataCommand,    "", NULL },
        { NULL,             0,                  false, NULL,                                           "", NULL }
    };

    static ChatCommand learnCommandTable[] =
    {
        { "all",            SEC_ADMINISTRATOR,  false, &ChatHandler::HandleLearnAllCommand,            "", NULL },
        { "all_gm",         SEC_GAMEMASTER,     false, &ChatHandler::HandleLearnAllGMCommand,          "", NULL },
        { "all_crafts",     SEC_GAMEMASTER,     false, &ChatHandler::HandleLearnAllCraftsCommand,      "", NULL },
        { "all_default",    SEC_MODERATOR,      false, &ChatHandler::HandleLearnAllDefaultCommand,     "", NULL },
        { "all_lang",       SEC_MODERATOR,      false, &ChatHandler::HandleLearnAllLangCommand,        "", NULL },
        { "all_myclass",    SEC_ADMINISTRATOR,  false, &ChatHandler::HandleLearnAllMyClassCommand,     "", NULL },
        { "all_mypettalents",SEC_ADMINISTRATOR, false, &ChatHandler::HandleLearnAllMyPetTalentsCommand,"", NULL },
        { "all_myspells",   SEC_ADMINISTRATOR,  false, &ChatHandler::HandleLearnAllMySpellsCommand,    "", NULL },
        { "all_mytalents",  SEC_ADMINISTRATOR,  false, &ChatHandler::HandleLearnAllMyTalentsCommand,   "", NULL },
        { "all_recipes",    SEC_GAMEMASTER,     false, &ChatHandler::HandleLearnAllRecipesCommand,     "", NULL },
        { "",               SEC_ADMINISTRATOR,  false, &ChatHandler::HandleLearnCommand,               "", NULL },
        { NULL,             0,                  false, NULL,                                           "", NULL }
    };

    static ChatCommand listCommandTable[] =
    {
        { "auras",          SEC_ADMINISTRATOR,  false, &ChatHandler::HandleListAurasCommand,           "", NULL },
        { "creature",       SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleListCreatureCommand,        "", NULL },
        { "item",           SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleListItemCommand,            "", NULL },
        { "object",         SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleListObjectCommand,          "", NULL },
        { "talents",        SEC_ADMINISTRATOR,  false, &ChatHandler::HandleListTalentsCommand,         "", NULL },
        { NULL,             0,                  false, NULL,                                           "", NULL }
    };

    static ChatCommand lookupAccountCommandTable[] =
    {
        { "email",          SEC_GAMEMASTER,     true,  &ChatHandler::HandleLookupAccountEmailCommand,  "", NULL },
        { "ip",             SEC_GAMEMASTER,     true,  &ChatHandler::HandleLookupAccountIpCommand,     "", NULL },
        { "name",           SEC_GAMEMASTER,     true,  &ChatHandler::HandleLookupAccountNameCommand,   "", NULL },
        { NULL,             0,                  false, NULL,                                           "", NULL }
    };

    static ChatCommand lookupPlayerCommandTable[] =
    {
        { "account",        SEC_GAMEMASTER,     true,  &ChatHandler::HandleLookupPlayerAccountCommand, "", NULL },
        { "email",          SEC_GAMEMASTER,     true,  &ChatHandler::HandleLookupPlayerEmailCommand,   "", NULL },
        { "ip",             SEC_GAMEMASTER,     true,  &ChatHandler::HandleLookupPlayerIpCommand,      "", NULL },
        { NULL,             0,                  false, NULL,                                           "", NULL }
    };

    static ChatCommand lookupCommandTable[] =
    {
        { "account",        SEC_GAMEMASTER,     true,  NULL,                                           "", lookupAccountCommandTable },
        { "area",           SEC_MODERATOR,      true,  &ChatHandler::HandleLookupAreaCommand,          "", NULL },
        { "creature",       SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleLookupCreatureCommand,      "", NULL },
        { "event",          SEC_GAMEMASTER,     true,  &ChatHandler::HandleLookupEventCommand,         "", NULL },
        { "faction",        SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleLookupFactionCommand,       "", NULL },
        { "item",           SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleLookupItemCommand,          "", NULL },
        { "itemset",        SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleLookupItemSetCommand,       "", NULL },
        { "object",         SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleLookupObjectCommand,        "", NULL },
        { "quest",          SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleLookupQuestCommand,         "", NULL },
        { "player",         SEC_GAMEMASTER,     true,  NULL,                                           "", lookupPlayerCommandTable },
        { "skill",          SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleLookupSkillCommand,         "", NULL },
        { "spell",          SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleLookupSpellCommand,         "", NULL },
        { "taxinode",       SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleLookupTaxiNodeCommand,      "", NULL },
        { "tele",           SEC_MODERATOR,      true,  &ChatHandler::HandleLookupTeleCommand,          "", NULL },
        { "title",          SEC_GAMEMASTER,     true,  &ChatHandler::HandleLookupTitleCommand,         "", NULL },
        { NULL,             0,                  false, NULL,                                           "", NULL }
    };

    static ChatCommand modifyCommandTable[] =
    {
        { "hp",             SEC_MODERATOR,      false, &ChatHandler::HandleModifyHPCommand,            "", NULL },
        { "mana",           SEC_MODERATOR,      false, &ChatHandler::HandleModifyManaCommand,          "", NULL },
        { "rage",           SEC_MODERATOR,      false, &ChatHandler::HandleModifyRageCommand,          "", NULL },
        { "runicpower",     SEC_MODERATOR,      false, &ChatHandler::HandleModifyRunicPowerCommand,    "", NULL },
        { "energy",         SEC_MODERATOR,      false, &ChatHandler::HandleModifyEnergyCommand,        "", NULL },
        { "money",          SEC_MODERATOR,      false, &ChatHandler::HandleModifyMoneyCommand,         "", NULL },
        { "speed",          SEC_MODERATOR,      false, &ChatHandler::HandleModifySpeedCommand,         "", NULL },
        { "swim",           SEC_MODERATOR,      false, &ChatHandler::HandleModifySwimCommand,          "", NULL },
        { "scale",          SEC_MODERATOR,      false, &ChatHandler::HandleModifyScaleCommand,         "", NULL },
        { "bit",            SEC_MODERATOR,      false, &ChatHandler::HandleModifyBitCommand,           "", NULL },
        { "bwalk",          SEC_MODERATOR,      false, &ChatHandler::HandleModifyBWalkCommand,         "", NULL },
        { "fly",            SEC_MODERATOR,      false, &ChatHandler::HandleModifyFlyCommand,           "", NULL },
        { "aspeed",         SEC_MODERATOR,      false, &ChatHandler::HandleModifyASpeedCommand,        "", NULL },
        { "faction",        SEC_MODERATOR,      false, &ChatHandler::HandleModifyFactionCommand,       "", NULL },
        { "spell",          SEC_MODERATOR,      false, &ChatHandler::HandleModifySpellCommand,         "", NULL },
        { "tp",             SEC_MODERATOR,      false, &ChatHandler::HandleModifyTalentCommand,        "", NULL },
        { "mount",          SEC_MODERATOR,      false, &ChatHandler::HandleModifyMountCommand,         "", NULL },
        { "honor",          SEC_MODERATOR,      false, &ChatHandler::HandleModifyHonorCommand,         "", NULL },
        { "rep",            SEC_GAMEMASTER,     false, &ChatHandler::HandleModifyRepCommand,           "", NULL },
        { "arena",          SEC_MODERATOR,      false, &ChatHandler::HandleModifyArenaCommand,         "", NULL },
        { "drunk",          SEC_MODERATOR,      false, &ChatHandler::HandleModifyDrunkCommand,         "", NULL },
        { "standstate",     SEC_GAMEMASTER,     false, &ChatHandler::HandleModifyStandStateCommand,    "", NULL },
        { "morph",          SEC_GAMEMASTER,     false, &ChatHandler::HandleModifyMorphCommand,         "", NULL },
        { "phase",          SEC_ADMINISTRATOR,  false, &ChatHandler::HandleModifyPhaseCommand,         "", NULL },
        { "gender",         SEC_GAMEMASTER,     false, &ChatHandler::HandleModifyGenderCommand,        "", NULL },
        { NULL,             0,                  false, NULL,                                           "", NULL }
    };

    static ChatCommand npcCommandTable[] =
    {
        { "add",            SEC_GAMEMASTER,     false, &ChatHandler::HandleNpcAddCommand,              "", NULL },
        { "additem",        SEC_GAMEMASTER,     false, &ChatHandler::HandleNpcAddVendorItemCommand,    "", NULL },
        { "addmove",        SEC_GAMEMASTER,     false, &ChatHandler::HandleNpcAddMoveCommand,          "", NULL },
        { "allowmove",      SEC_ADMINISTRATOR,  false, &ChatHandler::HandleNpcAllowMovementCommand,    "", NULL },
        { "changeentry",    SEC_ADMINISTRATOR,  false, &ChatHandler::HandleNpcChangeEntryCommand,      "", NULL },
        { "changelevel",    SEC_GAMEMASTER,     false, &ChatHandler::HandleNpcChangeLevelCommand,      "", NULL },
        { "delete",         SEC_GAMEMASTER,     false, &ChatHandler::HandleNpcDeleteCommand,           "", NULL },
        { "delitem",        SEC_GAMEMASTER,     false, &ChatHandler::HandleNpcDelVendorItemCommand,    "", NULL },
        { "factionid",      SEC_GAMEMASTER,     false, &ChatHandler::HandleNpcFactionIdCommand,        "", NULL },
        { "flag",           SEC_GAMEMASTER,     false, &ChatHandler::HandleNpcFlagCommand,             "", NULL },
        { "follow",         SEC_GAMEMASTER,     false, &ChatHandler::HandleNpcFollowCommand,           "", NULL },
        { "info",           SEC_ADMINISTRATOR,  false, &ChatHandler::HandleNpcInfoCommand,             "", NULL },
        { "move",           SEC_GAMEMASTER,     false, &ChatHandler::HandleNpcMoveCommand,             "", NULL },
        { "playemote",      SEC_ADMINISTRATOR,  false, &ChatHandler::HandleNpcPlayEmoteCommand,        "", NULL },
        { "setmodel",       SEC_GAMEMASTER,     false, &ChatHandler::HandleNpcSetModelCommand,         "", NULL },
        { "setmovetype",    SEC_GAMEMASTER,     false, &ChatHandler::HandleNpcSetMoveTypeCommand,      "", NULL },
        { "setphase",       SEC_GAMEMASTER,     false, &ChatHandler::HandleNpcSetPhaseCommand,         "", NULL },
        { "spawndist",      SEC_GAMEMASTER,     false, &ChatHandler::HandleNpcSpawnDistCommand,        "", NULL },
        { "spawntime",      SEC_GAMEMASTER,     false, &ChatHandler::HandleNpcSpawnTimeCommand,        "", NULL },
        { "say",            SEC_MODERATOR,      false, &ChatHandler::HandleNpcSayCommand,              "", NULL },
        { "textemote",      SEC_MODERATOR,      false, &ChatHandler::HandleNpcTextEmoteCommand,        "", NULL },
        { "unfollow",       SEC_GAMEMASTER,     false, &ChatHandler::HandleNpcUnFollowCommand,         "", NULL },
        { "whisper",        SEC_MODERATOR,      false, &ChatHandler::HandleNpcWhisperCommand,          "", NULL },
        { "yell",           SEC_MODERATOR,      false, &ChatHandler::HandleNpcYellCommand,             "", NULL },
        { "tame",           SEC_GAMEMASTER,     false, &ChatHandler::HandleNpcTameCommand,             "", NULL },
        { "setdeathstate",  SEC_GAMEMASTER,     false, &ChatHandler::HandleNpcSetDeathStateCommand,    "", NULL },

        //{ TODO: fix or remove this commands
        { "addweapon",      SEC_ADMINISTRATOR,  false, &ChatHandler::HandleNpcAddWeaponCommand,        "", NULL },
        { "name",           SEC_GAMEMASTER,     false, &ChatHandler::HandleNpcNameCommand,             "", NULL },
        { "subname",        SEC_GAMEMASTER,     false, &ChatHandler::HandleNpcSubNameCommand,          "", NULL },
        //}

        { NULL,             0,                  false, NULL,                                           "", NULL }
    };

    static ChatCommand pdumpCommandTable[] =
    {
        { "load",           SEC_ADMINISTRATOR,  true,  &ChatHandler::HandlePDumpLoadCommand,           "", NULL },
        { "write",          SEC_ADMINISTRATOR,  true,  &ChatHandler::HandlePDumpWriteCommand,          "", NULL },
        { NULL,             0,                  false, NULL,                                           "", NULL }
    };

    static ChatCommand questCommandTable[] =
    {
        { "add",            SEC_ADMINISTRATOR,  false, &ChatHandler::HandleQuestAdd,                   "", NULL },
        { "complete",       SEC_ADMINISTRATOR,  false, &ChatHandler::HandleQuestComplete,              "", NULL },
        { "remove",         SEC_ADMINISTRATOR,  false, &ChatHandler::HandleQuestRemove,                "", NULL },
        { NULL,             0,                  false, NULL,                                           "", NULL }
    };

    static ChatCommand reloadCommandTable[] =
    {
        { "all",            SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleReloadAllCommand,           "", NULL },
        { "all_achievement",SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleReloadAllAchievementCommand,"", NULL },
        { "all_area",       SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleReloadAllAreaCommand,       "", NULL },
        { "all_eventai",    SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleReloadAllEventAICommand,    "", NULL },
        { "all_gossips",    SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleReloadAllGossipsCommand,    "", NULL },
        { "all_item",       SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleReloadAllItemCommand,       "", NULL },
        { "all_locales",    SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleReloadAllLocalesCommand,    "", NULL },
        { "all_loot",       SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleReloadAllLootCommand,       "", NULL },
        { "all_npc",        SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleReloadAllNpcCommand,        "", NULL },
        { "all_quest",      SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleReloadAllQuestCommand,      "", NULL },
        { "all_scripts",    SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleReloadAllScriptsCommand,    "", NULL },
        { "all_spell",      SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleReloadAllSpellCommand,      "", NULL },

        { "config",         SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleReloadConfigCommand,        "", NULL },

        { "achievement_criteria_requirement",SEC_ADMINISTRATOR,true,&ChatHandler::HandleReloadAchievementCriteriaRequirementCommand,"",NULL },
        { "achievement_reward",          SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadAchievementRewardCommand,       "", NULL },
        { "areatrigger_involvedrelation",SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadQuestAreaTriggersCommand,       "", NULL },
        { "areatrigger_tavern",          SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadAreaTriggerTavernCommand,       "", NULL },
        { "areatrigger_teleport",        SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadAreaTriggerTeleportCommand,     "", NULL },
        { "command",                     SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadCommandCommand,                 "", NULL },
        { "creature_ai_scripts",         SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadEventAIScriptsCommand,          "", NULL },
        { "creature_ai_summons",         SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadEventAISummonsCommand,          "", NULL },
        { "creature_ai_texts",           SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadEventAITextsCommand,            "", NULL },
        { "creature_battleground",       SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadBattleEventCommand,             "", NULL },
        { "creature_involvedrelation",   SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadCreatureQuestInvRelationsCommand,"",NULL },
        { "creature_loot_template",      SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadLootTemplatesCreatureCommand,   "", NULL },
        { "creature_questrelation",      SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadCreatureQuestRelationsCommand,  "", NULL },
        { "db_script_string",            SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadDbScriptStringCommand,          "", NULL },
        { "disenchant_loot_template",    SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadLootTemplatesDisenchantCommand, "", NULL },
        { "event_scripts",               SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadEventScriptsCommand,            "", NULL },
        { "fishing_loot_template",       SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadLootTemplatesFishingCommand,    "", NULL },
        { "game_graveyard_zone",         SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadGameGraveyardZoneCommand,       "", NULL },
        { "game_tele",                   SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadGameTeleCommand,                "", NULL },
        { "gameobject_involvedrelation", SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadGOQuestInvRelationsCommand,     "", NULL },
        { "gameobject_loot_template",    SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadLootTemplatesGameobjectCommand, "", NULL },
        { "gameobject_questrelation",    SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadGOQuestRelationsCommand,        "", NULL },
        { "gameobject_scripts",          SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadGameObjectScriptsCommand,       "", NULL },
        { "gameobject_battleground",     SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadBattleEventCommand,             "", NULL },
        { "gossip_menu",                 SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadGossipMenuCommand,              "", NULL },
        { "gossip_menu_option",          SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadGossipMenuOptionCommand,        "", NULL },
        { "gossip_scripts",              SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadGossipScriptsCommand,           "", NULL },
        { "item_enchantment_template",   SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadItemEnchantementsCommand,       "", NULL },
        { "item_loot_template",          SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadLootTemplatesItemCommand,       "", NULL },
        { "item_required_target",        SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadItemRequiredTragetCommand,      "", NULL },
        { "locales_achievement_reward",  SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadLocalesAchievementRewardCommand,"", NULL },
        { "locales_creature",            SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadLocalesCreatureCommand,         "", NULL },
        { "locales_gameobject",          SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadLocalesGameobjectCommand,       "", NULL },
        { "locales_gossip_menu_option",  SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadLocalesGossipMenuOptionCommand, "", NULL },
        { "locales_item",                SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadLocalesItemCommand,             "", NULL },
        { "locales_npc_text",            SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadLocalesNpcTextCommand,          "", NULL },
        { "locales_page_text",           SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadLocalesPageTextCommand,         "", NULL },
        { "locales_points_of_interest",  SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadLocalesPointsOfInterestCommand, "", NULL },
        { "locales_quest",               SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadLocalesQuestCommand,            "", NULL },
        { "mail_level_reward",           SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadMailLevelRewardCommand,       "", NULL },
        { "mail_loot_template",          SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadLootTemplatesMailCommand,       "", NULL },
        { "mangos_string",               SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadMangosStringCommand,            "", NULL },
        { "milling_loot_template",       SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadLootTemplatesMillingCommand,    "", NULL },
        { "npc_gossip",                  SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadNpcGossipCommand,               "", NULL },
        { "npc_spellclick_spells",       SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadSpellClickSpellsCommand,          "",NULL},
        { "npc_trainer",                 SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadNpcTrainerCommand,              "", NULL },
        { "npc_vendor",                  SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadNpcVendorCommand,               "", NULL },
        { "page_text",                   SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadPageTextsCommand,               "", NULL },
        { "pickpocketing_loot_template", SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadLootTemplatesPickpocketingCommand,"",NULL},
        { "points_of_interest",          SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadPointsOfInterestCommand,        "",NULL},
        { "prospecting_loot_template",   SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadLootTemplatesProspectingCommand,"", NULL },
        { "quest_end_scripts",           SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadQuestEndScriptsCommand,         "", NULL },
        { "quest_poi",                   SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadQuestPOICommand,                "", NULL },
        { "quest_start_scripts",         SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadQuestStartScriptsCommand,       "", NULL },
        { "quest_template",              SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadQuestTemplateCommand,           "", NULL },
        { "reference_loot_template",     SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadLootTemplatesReferenceCommand,  "", NULL },
        { "reserved_name",               SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadReservedNameCommand,            "", NULL },
        { "skill_discovery_template",    SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadSkillDiscoveryTemplateCommand,  "", NULL },
        { "skill_extra_item_template",   SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadSkillExtraItemTemplateCommand,  "", NULL },
        { "skill_fishing_base_level",    SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadSkillFishingBaseLevelCommand,   "", NULL },
        { "skinning_loot_template",      SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadLootTemplatesSkinningCommand,   "", NULL },
        { "spell_area",                  SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadSpellAreaCommand,               "", NULL },
        { "spell_bonus_data",            SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadSpellBonusesCommand,            "", NULL },
        { "spell_chain",                 SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadSpellChainCommand,              "", NULL },
        { "spell_elixir",                SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadSpellElixirCommand,             "", NULL },
        { "spell_learn_spell",           SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadSpellLearnSpellCommand,         "", NULL },
        { "spell_loot_template",         SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadLootTemplatesSpellCommand,      "", NULL },
        { "spell_pet_auras",             SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadSpellPetAurasCommand,           "", NULL },
        { "spell_proc_event",            SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadSpellProcEventCommand,          "", NULL },
        { "spell_proc_item_enchant",     SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadSpellProcItemEnchantCommand,    "", NULL },
        { "spell_script_target",         SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadSpellScriptTargetCommand,       "", NULL },
        { "spell_scripts",               SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadSpellScriptsCommand,            "", NULL },
        { "spell_target_position",       SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadSpellTargetPositionCommand,     "", NULL },
        { "spell_threats",               SEC_ADMINISTRATOR, true,  &ChatHandler::HandleReloadSpellThreatsCommand,            "", NULL },

        { NULL,                          0,                 false, NULL,                                                     "", NULL }
    };

    static ChatCommand resetCommandTable[] =
    {
        { "achievements",   SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleResetAchievementsCommand,   "", NULL },
        { "honor",          SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleResetHonorCommand,          "", NULL },
        { "level",          SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleResetLevelCommand,          "", NULL },
        { "specs",          SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleResetSpecsCommand,          "", NULL },
        { "spells",         SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleResetSpellsCommand,         "", NULL },
        { "stats",          SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleResetStatsCommand,          "", NULL },
        { "talents",        SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleResetTalentsCommand,        "", NULL },
        { "all",            SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleResetAllCommand,            "", NULL },
        { NULL,             0,                  false, NULL,                                           "", NULL }
    };

    static ChatCommand sendCommandTable[] =
    {
        { "items",          SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleSendItemsCommand,           "", NULL },
        { "mail",           SEC_MODERATOR,      true,  &ChatHandler::HandleSendMailCommand,            "", NULL },
        { "message",        SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleSendMessageCommand,         "", NULL },
        { "money",          SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleSendMoneyCommand,           "", NULL },
        { NULL,             0,                  false, NULL,                                           "", NULL }
    };

    static ChatCommand serverIdleRestartCommandTable[] =
    {
        { "cancel",         SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleServerShutDownCancelCommand,"", NULL },
        { ""   ,            SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleServerIdleRestartCommand,   "", NULL },
        { NULL,             0,                  false, NULL,                                           "", NULL }
    };

    static ChatCommand serverIdleShutdownCommandTable[] =
    {
        { "cancel",         SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleServerShutDownCancelCommand,"", NULL },
        { ""   ,            SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleServerIdleShutDownCommand,  "", NULL },
        { NULL,             0,                  false, NULL,                                           "", NULL }
    };

    static ChatCommand serverRestartCommandTable[] =
    {
        { "cancel",         SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleServerShutDownCancelCommand,"", NULL },
        { ""   ,            SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleServerRestartCommand,       "", NULL },
        { NULL,             0,                  false, NULL,                                           "", NULL }
    };

    static ChatCommand serverShutdownCommandTable[] =
    {
        { "cancel",         SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleServerShutDownCancelCommand,"", NULL },
        { ""   ,            SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleServerShutDownCommand,      "", NULL },
        { NULL,             0,                  false, NULL,                                           "", NULL }
    };

    static ChatCommand serverLogCommandTable[] =
    {
        { "filter",         SEC_CONSOLE,        true,  &ChatHandler::HandleServerLogFilterCommand,     "", NULL },
        { "level",          SEC_CONSOLE,        true,  &ChatHandler::HandleServerLogLevelCommand,      "", NULL },
        { NULL,             0,                  false, NULL,                                           "", NULL }
    };

    static ChatCommand serverSetCommandTable[] =
    {
        { "motd",           SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleServerSetMotdCommand,       "", NULL },
        { NULL,             0,                  false, NULL,                                           "", NULL }
    };

    static ChatCommand serverCommandTable[] =
    {
        { "corpses",        SEC_GAMEMASTER,     true,  &ChatHandler::HandleServerCorpsesCommand,       "", NULL },
        { "exit",           SEC_CONSOLE,        true,  &ChatHandler::HandleServerExitCommand,          "", NULL },
        { "idlerestart",    SEC_ADMINISTRATOR,  true,  NULL,                                           "", serverIdleRestartCommandTable },
        { "idleshutdown",   SEC_ADMINISTRATOR,  true,  NULL,                                           "", serverShutdownCommandTable },
        { "info",           SEC_PLAYER,         true,  &ChatHandler::HandleServerInfoCommand,          "", NULL },
        { "log",            SEC_CONSOLE,        true,  NULL,                                           "", serverLogCommandTable },
        { "motd",           SEC_PLAYER,         true,  &ChatHandler::HandleServerMotdCommand,          "", NULL },
        { "plimit",         SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleServerPLimitCommand,        "", NULL },
        { "restart",        SEC_ADMINISTRATOR,  true,  NULL,                                           "", serverRestartCommandTable },
        { "shutdown",       SEC_ADMINISTRATOR,  true,  NULL,                                           "", serverShutdownCommandTable },
        { "set",            SEC_ADMINISTRATOR,  true,  NULL,                                           "", serverSetCommandTable },
        { NULL,             0,                  false, NULL,                                           "", NULL }
    };

    static ChatCommand teleCommandTable[] =
    {
        { "add",            SEC_ADMINISTRATOR,  false, &ChatHandler::HandleTeleAddCommand,             "", NULL },
        { "del",            SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleTeleDelCommand,             "", NULL },
        { "name",           SEC_MODERATOR,      true,  &ChatHandler::HandleTeleNameCommand,            "", NULL },
        { "group",          SEC_MODERATOR,      false, &ChatHandler::HandleTeleGroupCommand,           "", NULL },
        { "",               SEC_MODERATOR,      false, &ChatHandler::HandleTeleCommand,                "", NULL },
        { NULL,             0,                  false, NULL,                                           "", NULL }
    };

    static ChatCommand titlesCommandTable[] =
    {
        { "add",            SEC_GAMEMASTER,     false, &ChatHandler::HandleTitlesAddCommand,           "", NULL },
        { "current",        SEC_GAMEMASTER,     false, &ChatHandler::HandleTitlesCurrentCommand,       "", NULL },
        { "remove",         SEC_GAMEMASTER,     false, &ChatHandler::HandleTitlesRemoveCommand,        "", NULL },
        { "setmask",        SEC_GAMEMASTER,     false, &ChatHandler::HandleTitlesSetMaskCommand,       "", NULL },
        { NULL,             0,                  false, NULL,                                           "", NULL }
    };

    static ChatCommand unbanCommandTable[] =
    {
        { "account",        SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleUnBanAccountCommand,      "", NULL },
        { "character",      SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleUnBanCharacterCommand,    "", NULL },
        { "ip",             SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleUnBanIPCommand,           "", NULL },
        { NULL,             0,                  false, NULL,                                           "", NULL }
    };

    static ChatCommand wpCommandTable[] =
    {
        { "show",           SEC_GAMEMASTER,     false, &ChatHandler::HandleWpShowCommand,              "", NULL },
        { "add",            SEC_GAMEMASTER,     false, &ChatHandler::HandleWpAddCommand,               "", NULL },
        { "modify",         SEC_GAMEMASTER,     false, &ChatHandler::HandleWpModifyCommand,            "", NULL },
        { "export",         SEC_ADMINISTRATOR,  false, &ChatHandler::HandleWpExportCommand,            "", NULL },
        { "import",         SEC_ADMINISTRATOR,  false, &ChatHandler::HandleWpImportCommand,            "", NULL },
        { NULL,             0,                  false, NULL,                                           "", NULL }
    };

    static ChatCommand voteCommandTable[] =
    {
        { "mute",           SEC_PLAYER,     false, &ChatHandler::HandleVoteMuteCommand,            "", NULL },
        { "yes",            SEC_PLAYER,     false, &ChatHandler::HandleVoteYesCommand,             "", NULL },
        { "no",             SEC_PLAYER,     false, &ChatHandler::HandleVoteNoCommand,              "", NULL },
        { NULL,             0,              false, NULL,                                           "", NULL }
    };

    static ChatCommand commandTable[] =
    {
        { "account",        SEC_PLAYER,         true,  NULL,                                           "", accountCommandTable  },
        { "cast",           SEC_ADMINISTRATOR,  false, NULL,                                           "", castCommandTable     },
        { "character",      SEC_GAMEMASTER,     true,  NULL,                                           "", characterCommandTable},
        { "debug",          SEC_MODERATOR,      true,  NULL,                                           "", debugCommandTable    },
        { "event",          SEC_GAMEMASTER,     false, NULL,                                           "", eventCommandTable    },
        { "gm",             SEC_MODERATOR,      true,  NULL,                                           "", gmCommandTable       },
        { "honor",          SEC_GAMEMASTER,     false, NULL,                                           "", honorCommandTable    },
        { "go",             SEC_MODERATOR,      false, NULL,                                           "", goCommandTable       },
        { "gobject",        SEC_GAMEMASTER,     false, NULL,                                           "", gobjectCommandTable  },
        { "guild",          SEC_ADMINISTRATOR,  true,  NULL,                                           "", guildCommandTable    },
        { "instance",       SEC_ADMINISTRATOR,  true,  NULL,                                           "", instanceCommandTable },
        { "learn",          SEC_MODERATOR,      false, NULL,                                           "", learnCommandTable    },
        { "list",           SEC_ADMINISTRATOR,  true,  NULL,                                           "", listCommandTable     },
        { "lookup",         SEC_ADMINISTRATOR,  true,  NULL,                                           "", lookupCommandTable   },
        { "modify",         SEC_MODERATOR,      false, NULL,                                           "", modifyCommandTable   },
        { "npc",            SEC_MODERATOR,      false, NULL,                                           "", npcCommandTable      },
        { "pdump",          SEC_ADMINISTRATOR,  true,  NULL,                                           "", pdumpCommandTable    },
        { "quest",          SEC_ADMINISTRATOR,  false, NULL,                                           "", questCommandTable    },
        { "reload",         SEC_ADMINISTRATOR,  true,  NULL,                                           "", reloadCommandTable   },
        { "reset",          SEC_ADMINISTRATOR,  true,  NULL,                                           "", resetCommandTable    },
        { "server",         SEC_ADMINISTRATOR,  true,  NULL,                                           "", serverCommandTable   },
        { "tele",           SEC_MODERATOR,      true,  NULL,                                           "", teleCommandTable     },
        { "titles",         SEC_GAMEMASTER,     false, NULL,                                           "", titlesCommandTable   },
        { "wp",             SEC_GAMEMASTER,     false, NULL,                                           "", wpCommandTable       },
        { "vote",           SEC_PLAYER,         false, NULL,                                           "", voteCommandTable     },
        { "aura",           SEC_ADMINISTRATOR,  false, &ChatHandler::HandleAuraCommand,                "", NULL },
        { "unaura",         SEC_ADMINISTRATOR,  false, &ChatHandler::HandleUnAuraCommand,              "", NULL },
        { "announce",       SEC_MODERATOR,      true,  &ChatHandler::HandleAnnounceCommand,            "", NULL },
        { "notify",         SEC_MODERATOR,      true,  &ChatHandler::HandleNotifyCommand,              "", NULL },
        { "goname",         SEC_MODERATOR,      false, &ChatHandler::HandleGonameCommand,              "", NULL },
        { "namego",         SEC_MODERATOR,      false, &ChatHandler::HandleNamegoCommand,              "", NULL },
        { "groupgo",        SEC_MODERATOR,      false, &ChatHandler::HandleGroupgoCommand,             "", NULL },
        { "commands",       SEC_PLAYER,         true,  &ChatHandler::HandleCommandsCommand,            "", NULL },
        { "demorph",        SEC_GAMEMASTER,     false, &ChatHandler::HandleDeMorphCommand,             "", NULL },
        { "die",            SEC_ADMINISTRATOR,  false, &ChatHandler::HandleDieCommand,                 "", NULL },
        { "revive",         SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleReviveCommand,              "", NULL },
        { "dismount",       SEC_PLAYER,         false, &ChatHandler::HandleDismountCommand,            "", NULL },
        { "gps",            SEC_MODERATOR,      false, &ChatHandler::HandleGPSCommand,                 "", NULL },
        { "guid",           SEC_GAMEMASTER,     false, &ChatHandler::HandleGUIDCommand,                "", NULL },
        { "help",           SEC_PLAYER,         true,  &ChatHandler::HandleHelpCommand,                "", NULL },
        { "itemmove",       SEC_GAMEMASTER,     false, &ChatHandler::HandleItemMoveCommand,            "", NULL },
        { "cooldown",       SEC_ADMINISTRATOR,  false, &ChatHandler::HandleCooldownCommand,            "", NULL },
        { "unlearn",        SEC_ADMINISTRATOR,  false, &ChatHandler::HandleUnLearnCommand,             "", NULL },
        { "distance",       SEC_ADMINISTRATOR,  false, &ChatHandler::HandleGetDistanceCommand,         "", NULL },
        { "recall",         SEC_MODERATOR,      false, &ChatHandler::HandleRecallCommand,              "", NULL },
        { "save",           SEC_PLAYER,         false, &ChatHandler::HandleSaveCommand,                "", NULL },
        { "saveall",        SEC_MODERATOR,      true,  &ChatHandler::HandleSaveAllCommand,             "", NULL },
        { "kick",           SEC_GAMEMASTER,     true,  &ChatHandler::HandleKickPlayerCommand,          "", NULL },
        { "ban",            SEC_ADMINISTRATOR,  true,  NULL,                                           "", banCommandTable      },
        { "unban",          SEC_ADMINISTRATOR,  true,  NULL,                                           "", unbanCommandTable    },
        { "baninfo",        SEC_ADMINISTRATOR,  false, NULL,                                           "", baninfoCommandTable  },
        { "banlist",        SEC_ADMINISTRATOR,  true,  NULL,                                           "", banlistCommandTable  },
        { "start",          SEC_PLAYER,         false, &ChatHandler::HandleStartCommand,               "", NULL },
        { "taxicheat",      SEC_MODERATOR,      false, &ChatHandler::HandleTaxiCheatCommand,           "", NULL },
        { "linkgrave",      SEC_ADMINISTRATOR,  false, &ChatHandler::HandleLinkGraveCommand,           "", NULL },
        { "neargrave",      SEC_ADMINISTRATOR,  false, &ChatHandler::HandleNearGraveCommand,           "", NULL },
        { "explorecheat",   SEC_ADMINISTRATOR,  false, &ChatHandler::HandleExploreCheatCommand,        "", NULL },
        { "hover",          SEC_ADMINISTRATOR,  false, &ChatHandler::HandleHoverCommand,               "", NULL },
        { "levelup",        SEC_ADMINISTRATOR,  false, &ChatHandler::HandleLevelUpCommand,             "", NULL },
        { "showarea",       SEC_ADMINISTRATOR,  false, &ChatHandler::HandleShowAreaCommand,            "", NULL },
        { "hidearea",       SEC_ADMINISTRATOR,  false, &ChatHandler::HandleHideAreaCommand,            "", NULL },
        { "additem",        SEC_ADMINISTRATOR,  false, &ChatHandler::HandleAddItemCommand,             "", NULL },
        { "additemset",     SEC_ADMINISTRATOR,  false, &ChatHandler::HandleAddItemSetCommand,          "", NULL },
        { "bank",           SEC_ADMINISTRATOR,  false, &ChatHandler::HandleBankCommand,                "", NULL },
        { "wchange",        SEC_ADMINISTRATOR,  false, &ChatHandler::HandleChangeWeather,              "", NULL },
        { "ticket",         SEC_GAMEMASTER,     true,  &ChatHandler::HandleTicketCommand,              "", NULL },
        { "delticket",      SEC_GAMEMASTER,     true,  &ChatHandler::HandleDelTicketCommand,           "", NULL },
        { "maxskill",       SEC_ADMINISTRATOR,  false, &ChatHandler::HandleMaxSkillCommand,            "", NULL },
        { "setskill",       SEC_ADMINISTRATOR,  false, &ChatHandler::HandleSetSkillCommand,            "", NULL },
        { "whispers",       SEC_MODERATOR,      false, &ChatHandler::HandleWhispersCommand,            "", NULL },
        { "pinfo",          SEC_GAMEMASTER,     true,  &ChatHandler::HandlePInfoCommand,               "", NULL },
        { "respawn",        SEC_ADMINISTRATOR,  false, &ChatHandler::HandleRespawnCommand,             "", NULL },
        { "send",           SEC_MODERATOR,      true,  NULL,                                           "", sendCommandTable     },
        { "loadscripts",    SEC_ADMINISTRATOR,  true,  &ChatHandler::HandleLoadScriptsCommand,         "", NULL },
        { "mute",           SEC_MODERATOR,      true,  &ChatHandler::HandleMuteCommand,                "", NULL },
        { "unmute",         SEC_MODERATOR,      true,  &ChatHandler::HandleUnmuteCommand,              "", NULL },
        { "movegens",       SEC_ADMINISTRATOR,  false, &ChatHandler::HandleMovegensCommand,            "", NULL },
        { "cometome",       SEC_ADMINISTRATOR,  false, &ChatHandler::HandleComeToMeCommand,            "", NULL },
        { "damage",         SEC_ADMINISTRATOR,  false, &ChatHandler::HandleDamageCommand,              "", NULL },
        { "combatstop",     SEC_GAMEMASTER,     false, &ChatHandler::HandleCombatStopCommand,          "", NULL },
        { "flusharenapoints",SEC_ADMINISTRATOR, false, &ChatHandler::HandleFlushArenaPointsCommand,    "", NULL },
        { "repairitems",    SEC_GAMEMASTER,     true,  &ChatHandler::HandleRepairitemsCommand,         "", NULL },
        { "waterwalk",      SEC_GAMEMASTER,     false, &ChatHandler::HandleWaterwalkCommand,           "", NULL },
        { "quit",           SEC_CONSOLE,        true,  &ChatHandler::HandleQuitCommand,                "", NULL },

        { NULL,             0,                  false, NULL,                                           "", NULL }
    };

    if(load_command_table)
    {
        load_command_table = false;

        QueryResult *result = WorldDatabase.Query("SELECT name,security,help FROM command");
        if (result)
        {
            do
            {
                Field *fields = result->Fetch();
                std::string name = fields[0].GetCppString();

                SetDataForCommandInTable(commandTable, name.c_str(), fields[1].GetUInt16(), fields[2].GetCppString(), name);

            } while(result->NextRow());
            delete result;
        }
    }

    return commandTable;
}

const char *ChatHandler::GetMangosString(int32 entry) const
{
    return m_session->GetMangosString(entry);
}

uint32 ChatHandler::GetAccountId() const
{
    return m_session->GetAccountId();
}

AccountTypes ChatHandler::GetAccessLevel() const
{
    return m_session->GetSecurity();
}

bool ChatHandler::isAvailable(ChatCommand const& cmd) const
{
    // check security level only for simple  command (without child commands)
    return GetAccessLevel() >= (AccountTypes)cmd.SecurityLevel;
}

bool ChatHandler::HasLowerSecurity(Player* target, uint64 guid, bool strong)
{
    WorldSession* target_session = NULL;
    uint32 target_account = 0;

    if (target)
        target_session = target->GetSession();
    else if (guid)
        target_account = sObjectMgr.GetPlayerAccountIdByGUID(guid);

    if(!target_session && !target_account)
    {
        SendSysMessage(LANG_PLAYER_NOT_FOUND);
        SetSentErrorMessage(true);
        return true;
    }

    return HasLowerSecurityAccount(target_session,target_account,strong);
}

bool ChatHandler::HasLowerSecurityAccount(WorldSession* target, uint32 target_account, bool strong)
{
    AccountTypes target_sec;

    // ignore only for non-players for non strong checks (when allow apply command at least to same sec level)
    if (GetAccessLevel() > SEC_PLAYER && !strong && !sWorld.getConfig(CONFIG_BOOL_GM_LOWER_SECURITY))
        return false;

    if (target)
        target_sec = target->GetSecurity();
    else if (target_account)
        target_sec = sAccountMgr.GetSecurity(target_account);
    else
        return true;                                        // caller must report error for (target==NULL && target_account==0)

    if (GetAccessLevel() < target_sec || (strong && GetAccessLevel() <= target_sec))
    {
        SendSysMessage(LANG_YOURS_SECURITY_IS_LOW);
        SetSentErrorMessage(true);
        return true;
    }

    return false;
}

bool ChatHandler::hasStringAbbr(const char* name, const char* part)
{
    // non "" command
    if( *name )
    {
        // "" part from non-"" command
        if( !*part )
            return false;

        for(;;)
        {
            if( !*part )
                return true;
            else if( !*name )
                return false;
            else if( tolower( *name ) != tolower( *part ) )
                return false;
            ++name; ++part;
        }
    }
    // allow with any for ""

    return true;
}

void ChatHandler::SendSysMessage(const char *str)
{
    WorldPacket data;

    // need copy to prevent corruption by strtok call in LineFromMessage original string
    char* buf = mangos_strdup(str);
    char* pos = buf;

    while(char* line = LineFromMessage(pos))
    {
        FillSystemMessageData(&data, line);
        m_session->SendPacket(&data);
    }

    delete [] buf;
}

void ChatHandler::SendGlobalSysMessage(const char *str)
{
    // Chat output
    WorldPacket data;

    // need copy to prevent corruption by strtok call in LineFromMessage original string
    char* buf = mangos_strdup(str);
    char* pos = buf;

    while(char* line = LineFromMessage(pos))
    {
        FillSystemMessageData(&data, line);
        sWorld.SendGlobalMessage(&data);
    }

    delete [] buf;
}

void ChatHandler::SendGMSysMessage(const char *str, AccountTypes sec)
{
    // Chat output
    WorldPacket data;

    // need copy to prevent corruption by strtok call in LineFromMessage original string
    char* buf = mangos_strdup(str);
    char* pos = buf;

    while(char* line = LineFromMessage(pos))
    {
        FillSystemMessageData(&data, line);
        sWorld.SendGMGlobalMessage(&data, sec);
    }

    delete [] buf;
}

void ChatHandler::SendSysMessage(int32 entry)
{
    SendSysMessage(GetMangosString(entry));
}

void ChatHandler::PSendSysMessage(int32 entry, ...)
{
    const char *format = GetMangosString(entry);
    va_list ap;
    char str [2048];
    va_start(ap, entry);
    vsnprintf(str,2048,format, ap );
    va_end(ap);
    SendSysMessage(str);
}

void ChatHandler::PSendSysMessage(const char *format, ...)
{
    va_list ap;
    char str [2048];
    va_start(ap, format);
    vsnprintf(str,2048,format, ap );
    va_end(ap);
    SendSysMessage(str);
}

bool ChatHandler::ExecuteCommandInTable(ChatCommand *table, const char* text, const std::string& fullcmd)
{
    char const* oldtext = text;
    std::string cmd = "";

    while (*text != ' ' && *text != '\0')
    {
        cmd += *text;
        ++text;
    }

    while (*text == ' ') ++text;

    for(uint32 i = 0; table[i].Name != NULL; ++i)
    {
        if( !hasStringAbbr(table[i].Name, cmd.c_str()) )
            continue;

        // select subcommand from child commands list
        if(table[i].ChildCommands != NULL)
        {
            if(!ExecuteCommandInTable(table[i].ChildCommands, text, fullcmd))
            {
                if(text && text[0] != '\0')
                    SendSysMessage(LANG_NO_SUBCMD);
                else
                    SendSysMessage(LANG_CMD_SYNTAX);

                ShowHelpForCommand(table[i].ChildCommands,text);
                SetSentErrorMessage(true);
            }

            return true;
        }

        // must be available and have handler
        if(!table[i].Handler || !isAvailable(table[i]))
            continue;

        SetSentErrorMessage(false);
        // table[i].Name == "" is special case: send original command to handler
        if((this->*(table[i].Handler))(strlen(table[i].Name)!=0 ? text : oldtext))
        {
            if(table[i].SecurityLevel > SEC_PLAYER)
            {
                // chat case
                if (m_session)
                {
                    Player* p = m_session->GetPlayer();
                    ObjectGuid sel_guid = p->GetSelection();
                    sLog.outCommand(GetAccountId(),"Command: %s [Player: %s (Account: %u IP: %s) X: %f Y: %f Z: %f Map: %u Selected: %s]",
                        fullcmd.c_str(),p->GetName(),GetAccountId(), m_session->GetRemoteAddress().c_str(),p->GetPositionX(),p->GetPositionY(),p->GetPositionZ(),p->GetMapId(),
                        sel_guid.GetString().c_str());

                    if(m_session->GetSecurity() < SEC_ADMINISTRATOR)
                    {
                        //                                             дата : время : команда : игрок : его акк : selected : x : y : z : map                                 Comm  ACC  Pl-r    X     Y     Z   sType   sId   Map
                        CharacterDatabase.PExecute("INSERT INTO gmlog_commands (command, account, player, posX, posY, posZ, selected_type, selected_guid, map, time) VALUES ('%s', '%u', '%s', '%f', '%f', '%f', '%s', '%u', '%u', NOW())",
                            fullcmd.c_str(), m_session->GetAccountId(), p->GetName(), p->GetPositionX(), p->GetPositionY(), p->GetPositionZ(), sel_guid.GetString().c_str(), GUID_LOPART(p->GetGUID()), p->GetMapId());
                    }
                }
                else                                        // 0 account -> console
                {
                    sLog.outCommand(GetAccountId(),"Command: %s [Account: %u from %s]",
                        fullcmd.c_str(),GetAccountId(),GetAccountId() ? "RA-connection" : "Console");
                }
            }
        }
        // some commands have custom error messages. Don't send the default one in these cases.
        else if(!HasSentErrorMessage())
        {
            if(!table[i].Help.empty())
                SendSysMessage(table[i].Help.c_str());
            else
                SendSysMessage(LANG_CMD_SYNTAX);
            SetSentErrorMessage(true);
        }

        return true;
    }

    return false;
}

bool ChatHandler::SetDataForCommandInTable(ChatCommand *table, const char* text, uint32 security, std::string const& help, std::string const& fullcommand )
{
    std::string cmd = "";

    while (*text != ' ' && *text != '\0')
    {
        cmd += *text;
        ++text;
    }

    while (*text == ' ') ++text;

    for(uint32 i = 0; table[i].Name != NULL; i++)
    {
        // for data fill use full explicit command names
        if( table[i].Name != cmd )
            continue;

        // select subcommand from child commands list (including "")
        if(table[i].ChildCommands != NULL)
        {
            if(SetDataForCommandInTable(table[i].ChildCommands, text, security, help, fullcommand))
                return true;
            else if(*text)
                return false;

            // fail with "" subcommands, then use normal level up command instead
        }
        // expected subcommand by full name DB content
        else if(*text)
        {
            sLog.outErrorDb("Table `command` have unexpected subcommand '%s' in command '%s', skip.",text,fullcommand.c_str());
            return false;
        }

        if(table[i].SecurityLevel != security)
            DETAIL_LOG("Table `command` overwrite for command '%s' default security (%u) by %u",fullcommand.c_str(),table[i].SecurityLevel,security);

        table[i].SecurityLevel = security;
        table[i].Help          = help;
        return true;
    }

    // in case "" command let process by caller
    if(!cmd.empty())
    {
        if(table==getCommandTable())
            sLog.outErrorDb("Table `command` have not existed command '%s', skip.",cmd.c_str());
        else
            sLog.outErrorDb("Table `command` have not existed subcommand '%s' in command '%s', skip.",cmd.c_str(),fullcommand.c_str());
    }

    return false;
}

int ChatHandler::ParseCommands(const char* text)
{
    ASSERT(text);
    ASSERT(*text);

    //if(m_session->GetSecurity() == 0)
    //    return 0;

    /// chat case (.command or !command format)
    if (m_session)
    {
        if(text[0] != '!' && text[0] != '.')
            return 0;
    }

    /// ignore single . and ! in line
    if (strlen(text) < 2)
        return 0;

    /// ignore messages staring from many dots.
    if ((text[0] == '.' && text[1] == '.') || (text[0] == '!' && text[1] == '!'))
        return 0;

    /// skip first . or ! (in console allowed use command with . and ! and without its)
    if (text[0] == '!' || text[0] == '.')
        ++text;

    std::string fullcmd = text;                             // original `text` can't be used. It content destroyed in command code processing.

    if (!ExecuteCommandInTable(getCommandTable(), text, fullcmd))
    {
        SendSysMessage(LANG_NO_CMD);
        SetSentErrorMessage(true);
    }

    return 1;
}

bool ChatHandler::isValidChatMessage(const char* message)
{
/*

valid examples:
|cffa335ee|Hitem:812:0:0:0:0:0:0:0:70|h[Glowing Brightwood Staff]|h|r
|cff808080|Hquest:2278:47|h[The Platinum Discs]|h|r
|cffffd000|Htrade:4037:1:150:1:6AAAAAAAAAAAAAAAAAAAAAAOAADAAAAAAAAAAAAAAAAIAAAAAAAAA|h[Engineering]|h|r
|cff4e96f7|Htalent:2232:-1|h[Taste for Blood]|h|r
|cff71d5ff|Hspell:21563|h[Command]|h|r
|cffffd000|Henchant:3919|h[Engineering: Rough Dynamite]|h|r
|cffffff00|Hachievement:546:0000000000000001:0:0:0:-1:0:0:0:0|h[Safe Deposit]|h|r
|cff66bbff|Hglyph:21:762|h[Glyph of Bladestorm]|h|r

| will be escaped to ||
*/

    if(strlen(message) > 255)
        return false;

    const char validSequence[6] = "cHhhr";
    const char* validSequenceIterator = validSequence;

    // more simple checks
    if (sWorld.getConfig(CONFIG_UINT32_CHAT_STRICT_LINK_CHECKING_SEVERITY) < 3)
    {
        const std::string validCommands = "cHhr|";

        while(*message)
        {
            // find next pipe command
            message = strchr(message, '|');

            if(!message)
                return true;

            ++message;
            char commandChar = *message;
            if(validCommands.find(commandChar) == std::string::npos)
                return false;

            ++message;
            // validate sequence
            if(sWorld.getConfig(CONFIG_UINT32_CHAT_STRICT_LINK_CHECKING_SEVERITY) == 2)
            {
                if(commandChar == *validSequenceIterator)
                {
                    if (validSequenceIterator == validSequence+4)
                        validSequenceIterator = validSequence;
                    else
                        ++validSequenceIterator;
                }
                else if(commandChar != '|')
                    return false;
            }
        }
        return true;
    }

    std::istringstream reader(message);
    char buffer[256];

    uint32 color;

    ItemPrototype const* linkedItem;
    Quest const* linkedQuest;
    SpellEntry const *linkedSpell;
    AchievementEntry const* linkedAchievement;
    ItemRandomPropertiesEntry const* itemProperty;
    ItemRandomSuffixEntry const* itemSuffix;

    while(!reader.eof())
    {
        if (validSequence == validSequenceIterator)
        {
            linkedItem = NULL;
            linkedQuest = NULL;
            linkedSpell = NULL;
            linkedAchievement = NULL;
            itemProperty = NULL;
            itemSuffix = NULL;

            reader.ignore(255, '|');
        }
        else if(reader.get() != '|')
        {
            DEBUG_LOG("ChatHandler::isValidChatMessage sequence aborted unexpectedly");
            return false;
        }

        // pipe has always to be followed by at least one char
        if ( reader.peek() == '\0')
        {
            DEBUG_LOG("ChatHandler::isValidChatMessage pipe followed by \\0");
            return false;
        }

        // no further pipe commands
        if (reader.eof())
            break;

        char commandChar;
        reader >> commandChar;

        // | in normal messages is escaped by ||
        if (commandChar != '|')
        {
            if(commandChar == *validSequenceIterator)
            {
                if (validSequenceIterator == validSequence+4)
                    validSequenceIterator = validSequence;
                else
                    ++validSequenceIterator;
            }
            else
            {
                DEBUG_LOG("ChatHandler::isValidChatMessage invalid sequence, expected %c but got %c", *validSequenceIterator, commandChar);
                return false;
            }
        }
        else if(validSequence != validSequenceIterator)
        {
            // no escaped pipes in sequences
            DEBUG_LOG("ChatHandler::isValidChatMessage got escaped pipe in sequence");
            return false;
        }

        switch (commandChar)
        {
            case 'c':
                color = 0;
                // validate color, expect 8 hex chars
                for(int i=0; i<8; i++)
                {
                    char c;
                    reader >> c;
                    if(!c)
                    {
                        DEBUG_LOG("ChatHandler::isValidChatMessage got \\0 while reading color in |c command");
                        return false;
                    }

                    color <<= 4;
                    // check for hex char
                    if(c >= '0' && c <='9')
                    {
                        color |= c-'0';
                        continue;
                    }
                    if(c >= 'a' && c <='f')
                    {
                        color |= 10+c-'a';
                        continue;
                    }
                    DEBUG_LOG("ChatHandler::isValidChatMessage got non hex char '%c' while reading color", c);
                    return false;
                }
                break;
            case 'H':
                // read chars up to colon  = link type
                reader.getline(buffer, 256, ':');

                if (strcmp(buffer, "item") == 0)
                {
                    // read item entry
                    reader.getline(buffer, 256, ':');

                    linkedItem = ObjectMgr::GetItemPrototype(atoi(buffer));
                    if(!linkedItem)
                    {
                        DEBUG_LOG("ChatHandler::isValidChatMessage got invalid itemID %u in |item command", atoi(buffer));
                        return false;
                    }

                    if (color != ItemQualityColors[linkedItem->Quality])
                    {
                        DEBUG_LOG("ChatHandler::isValidChatMessage linked item has color %u, but user claims %u", ItemQualityColors[linkedItem->Quality],
                                color);
                        return false;
                    }

                    // the itementry is followed by several integers which describe an instance of this item

                    // position relative after itemEntry
                    const uint8 randomPropertyPosition = 6;

                    int32 propertyId = 0;
                    bool negativeNumber = false;
                    char c;
                    for(uint8 i=0; i<randomPropertyPosition; ++i)
                    {
                        propertyId = 0;
                        negativeNumber = false;
                        while((c = reader.get())!=':')
                        {
                            if(c >='0' && c<='9')
                            {
                                propertyId*=10;
                                propertyId += c-'0';
                            } else if(c == '-')
                                negativeNumber = true;
                            else
                                return false;
                        }
                    }
                    if (negativeNumber)
                        propertyId *= -1;

                    if (propertyId > 0)
                    {
                        itemProperty = sItemRandomPropertiesStore.LookupEntry(propertyId);
                        if (!itemProperty)
                            return false;
                    }
                    else if(propertyId < 0)
                    {
                        itemSuffix = sItemRandomSuffixStore.LookupEntry(-propertyId);
                        if (!itemSuffix)
                            return false;
                    }

                    // ignore other integers
                    while ((c >= '0' && c <= '9') || c== ':')
                    {
                        reader.ignore(1);
                        c = reader.peek();
                    }
                }
                else if(strcmp(buffer, "quest") == 0)
                {
                    // no color check for questlinks, each client will adapt it anyway
                    uint32 questid= 0;
                    // read questid
                    char c = reader.peek();
                    while(c >='0' && c<='9')
                    {
                        reader.ignore(1);
                        questid *= 10;
                        questid += c-'0';
                        c = reader.peek();
                    }

                    linkedQuest = sObjectMgr.GetQuestTemplate(questid);

                    if(!linkedQuest)
                    {
                        DEBUG_LOG("ChatHandler::isValidChatMessage Questtemplate %u not found", questid);
                        return false;
                    }
                    c = reader.peek();
                    // level
                    while(c !='|' && c!='\0')
                    {
                        reader.ignore(1);
                        c = reader.peek();
                    }
                }
                else if(strcmp(buffer, "trade") == 0)
                {
                    if(color != CHAT_LINK_COLOR_TRADE)
                        return false;

                    // read spell entry
                    reader.getline(buffer, 256, ':');
                    linkedSpell = sSpellStore.LookupEntry(atoi(buffer));
                    if(!linkedSpell)
                        return false;

                    char c = reader.peek();
                    // base64 encoded stuff
                    while(c !='|' && c!='\0')
                    {
                        reader.ignore(1);
                        c = reader.peek();
                    }
                }
                else if(strcmp(buffer, "talent") == 0)
                {
                    // talent links are always supposed to be blue
                    if(color != CHAT_LINK_COLOR_TALENT)
                        return false;

                    // read talent entry
                    reader.getline(buffer, 256, ':');
                    TalentEntry const *talentInfo = sTalentStore.LookupEntry(atoi(buffer));
                    if(!talentInfo)
                        return false;

                    linkedSpell = sSpellStore.LookupEntry(talentInfo->RankID[0]);
                    if(!linkedSpell)
                        return false;

                    char c = reader.peek();
                    // skillpoints? whatever, drop it
                    while(c !='|' && c!='\0')
                    {
                        reader.ignore(1);
                        c = reader.peek();
                    }
                }
                else if(strcmp(buffer, "spell") == 0)
                {
                    if(color != CHAT_LINK_COLOR_SPELL)
                        return false;

                    uint32 spellid = 0;
                    // read spell entry
                    char c = reader.peek();
                    while(c >='0' && c<='9')
                    {
                        reader.ignore(1);
                        spellid *= 10;
                        spellid += c-'0';
                        c = reader.peek();
                    }
                    linkedSpell = sSpellStore.LookupEntry(spellid);
                    if(!linkedSpell)
                        return false;
                }
                else if(strcmp(buffer, "enchant") == 0)
                {
                    if(color != CHAT_LINK_COLOR_ENCHANT)
                        return false;

                    uint32 spellid = 0;
                    // read spell entry
                    char c = reader.peek();
                    while(c >='0' && c<='9')
                    {
                        reader.ignore(1);
                        spellid *= 10;
                        spellid += c-'0';
                        c = reader.peek();
                    }
                    linkedSpell = sSpellStore.LookupEntry(spellid);
                    if(!linkedSpell)
                        return false;
                }
                else if(strcmp(buffer, "achievement") == 0)
                {
                    if(color != CHAT_LINK_COLOR_ACHIEVEMENT)
                        return false;
                    reader.getline(buffer, 256, ':');
                    uint32 achievementId = atoi(buffer);
                    linkedAchievement = sAchievementStore.LookupEntry(achievementId);

                    if(!linkedAchievement)
                        return false;

                    char c = reader.peek();
                    // skip progress
                    while(c !='|' && c!='\0')
                    {
                        reader.ignore(1);
                        c = reader.peek();
                    }
                }
                else if(strcmp(buffer, "glyph") == 0)
                {
                    if(color != CHAT_LINK_COLOR_GLYPH)
                        return false;

                    // first id is slot, drop it
                    reader.getline(buffer, 256, ':');
                    uint32 glyphId = 0;
                    char c = reader.peek();
                    while(c>='0' && c <='9')
                    {
                        glyphId *= 10;
                        glyphId += c-'0';
                        reader.ignore(1);
                        c = reader.peek();
                    }
                    GlyphPropertiesEntry const* glyph = sGlyphPropertiesStore.LookupEntry(glyphId);
                    if (!glyph)
                        return false;

                    linkedSpell = sSpellStore.LookupEntry(glyph->SpellId);

                    if (!linkedSpell)
                        return false;
                }
                else
                {
                    DEBUG_LOG("ChatHandler::isValidChatMessage user sent unsupported link type '%s'", buffer);
                    return false;
                }
                break;
            case 'h':
                // if h is next element in sequence, this one must contain the linked text :)
                if (*validSequenceIterator == 'h')
                {
                    // links start with '['
                    if (reader.get() != '[')
                    {
                        DEBUG_LOG("ChatHandler::isValidChatMessage link caption doesn't start with '['");
                        return false;
                    }
                    reader.getline(buffer, 256, ']');

                    // verify the link name
                    if (linkedSpell)
                    {
                        // spells with that flag have a prefix of "$PROFESSION: "
                        if (linkedSpell->Attributes & SPELL_ATTR_TRADESPELL)
                        {
                            // lookup skillid
                            SkillLineAbilityMapBounds bounds = sSpellMgr.GetSkillLineAbilityMapBounds(linkedSpell->Id);
                            if (bounds.first == bounds.second)
                            {
                                return false;
                            }

                            SkillLineAbilityEntry const *skillInfo = bounds.first->second;

                            if (!skillInfo)
                            {
                                return false;
                            }

                            SkillLineEntry const *skillLine = sSkillLineStore.LookupEntry(skillInfo->skillId);
                            if (!skillLine)
                            {
                                return false;
                            }

                            for(uint8 i=0; i<MAX_LOCALE; ++i)
                            {
                                uint32 skillLineNameLength = strlen(skillLine->name[i]);
                                if (skillLineNameLength > 0 && strncmp(skillLine->name[i], buffer, skillLineNameLength) == 0)
                                {
                                    // found the prefix, remove it to perform spellname validation below
                                    // -2 = strlen(": ")
                                    uint32 spellNameLength = strlen(buffer)-skillLineNameLength-2;
                                    memmove(buffer, buffer+skillLineNameLength+2, spellNameLength+1);
                                }
                            }
                        }
                        bool foundName = false;
                        for(uint8 i=0; i<MAX_LOCALE; ++i)
                        {
                            if (*linkedSpell->SpellName[i] && strcmp(linkedSpell->SpellName[i], buffer) == 0)
                            {
                                foundName = true;
                                break;
                            }
                        }
                        if (!foundName)
                            return false;
                    }
                    else if (linkedQuest)
                    {
                        if (linkedQuest->GetTitle() != buffer)
                        {
                            QuestLocale const *ql = sObjectMgr.GetQuestLocale(linkedQuest->GetQuestId());

                            if (!ql)
                            {
                                DEBUG_LOG("ChatHandler::isValidChatMessage default questname didn't match and there is no locale");
                                return false;
                            }

                            bool foundName = false;
                            for(uint8 i=0; i<ql->Title.size(); i++)
                            {
                                if (ql->Title[i] == buffer)
                                {
                                    foundName = true;
                                    break;
                                }
                            }
                            if (!foundName)
                            {
                                DEBUG_LOG("ChatHandler::isValidChatMessage no quest locale title matched");
                                return false;
                            }
                        }
                    }
                    else if(linkedItem)
                    {
                        char* const* suffix = itemSuffix?itemSuffix->nameSuffix:(itemProperty?itemProperty->nameSuffix:NULL);

                        std::string expectedName = std::string(linkedItem->Name1);
                        if (suffix)
                        {
                            expectedName += " ";
                            expectedName += suffix[LOCALE_enUS];
                        }

                        if (expectedName != buffer)
                        {
                            ItemLocale const *il = sObjectMgr.GetItemLocale(linkedItem->ItemId);

                            bool foundName = false;
                            for(uint8 i=LOCALE_koKR; i<MAX_LOCALE; ++i)
                            {
                                int8 dbIndex = sObjectMgr.GetIndexForLocale(LocaleConstant(i));
                                if (dbIndex == -1 || il == NULL || (size_t)dbIndex >= il->Name.size())
                                    // using strange database/client combinations can lead to this case
                                    expectedName = linkedItem->Name1;
                                else
                                    expectedName = il->Name[dbIndex];
                                if (suffix)
                                {
                                    expectedName += " ";
                                    expectedName += suffix[i];
                                }
                                if ( expectedName == buffer)
                                {
                                    foundName = true;
                                    break;
                                }
                            }
                            if (!foundName)
                            {
                                DEBUG_LOG("ChatHandler::isValidChatMessage linked item name wasn't found in any localization");
                                return false;
                            }
                        }
                    }
                    else if (linkedAchievement)
                    {
                        bool foundName = false;
                        for(uint8 i=0; i<MAX_LOCALE; ++i)
                        {
                            if (*linkedAchievement->name[i] && strcmp(linkedAchievement->name[i], buffer) == 0)
                            {
                                foundName = true;
                                break;
                            }
                        }
                        if (!foundName)
                            return false;
                    }
                    // that place should never be reached - if nothing linked has been set in |H
                    // it will return false before
                    else
                        return false;
                }
                break;
            case 'r':
            case '|':
                // no further payload
                break;
            default:
                DEBUG_LOG("ChatHandler::isValidChatMessage got invalid command |%c", commandChar);
                return false;
        }
    }

    // check if every opened sequence was also closed properly
    if(validSequence != validSequenceIterator)
        DEBUG_LOG("ChatHandler::isValidChatMessage EOF in active sequence");

    return validSequence == validSequenceIterator;
}

bool ChatHandler::ShowHelpForSubCommands(ChatCommand *table, char const* cmd, char const* subcmd)
{
    std::string list;
    for(uint32 i = 0; table[i].Name != NULL; ++i)
    {
        // must be available (ignore handler existence for show command with possibe avalable subcomands
        if(!isAvailable(table[i]))
            continue;

        /// for empty subcmd show all available
        if( *subcmd && !hasStringAbbr(table[i].Name, subcmd))
            continue;

        if(m_session)
            list += "\n    ";
        else
            list += "\n\r    ";

        list += table[i].Name;

        if(table[i].ChildCommands)
            list += " ...";
    }

    if(list.empty())
        return false;

    if(table==getCommandTable())
    {
        SendSysMessage(LANG_AVIABLE_CMD);
        PSendSysMessage("%s",list.c_str());
    }
    else
        PSendSysMessage(LANG_SUBCMDS_LIST,cmd,list.c_str());

    return true;
}

bool ChatHandler::ShowHelpForCommand(ChatCommand *table, const char* cmd)
{
    if(*cmd)
    {
        for(uint32 i = 0; table[i].Name != NULL; ++i)
        {
            // must be available (ignore handler existence for show command with possibe avalable subcomands
            if(!isAvailable(table[i]))
                continue;

            if( !hasStringAbbr(table[i].Name, cmd) )
                continue;

            // have subcommand
            char const* subcmd = (*cmd) ? strtok(NULL, " ") : "";

            if(table[i].ChildCommands && subcmd && *subcmd)
            {
                if(ShowHelpForCommand(table[i].ChildCommands, subcmd))
                    return true;
            }

            if(!table[i].Help.empty())
                SendSysMessage(table[i].Help.c_str());

            if(table[i].ChildCommands)
                if(ShowHelpForSubCommands(table[i].ChildCommands,table[i].Name,subcmd ? subcmd : ""))
                    return true;

            return !table[i].Help.empty();
        }
    }
    else
    {
        for(uint32 i = 0; table[i].Name != NULL; ++i)
        {
            // must be available (ignore handler existence for show command with possibe avalable subcomands
            if(!isAvailable(table[i]))
                continue;

            if(strlen(table[i].Name))
                continue;

            if(!table[i].Help.empty())
                SendSysMessage(table[i].Help.c_str());

            if(table[i].ChildCommands)
                if(ShowHelpForSubCommands(table[i].ChildCommands,"",""))
                    return true;

            return !table[i].Help.empty();
        }
    }

    return ShowHelpForSubCommands(table,"",cmd);
}

//Note: target_guid used only in CHAT_MSG_WHISPER_INFORM mode (in this case channelName ignored)
void ChatHandler::FillMessageData( WorldPacket *data, WorldSession* session, uint8 type, uint32 language, const char *channelName, uint64 target_guid, const char *message, Unit *speaker)
{
    uint32 messageLength = (message ? strlen(message) : 0) + 1;

    data->Initialize(SMSG_MESSAGECHAT, 100);                // guess size
    *data << uint8(type);
    if ((type != CHAT_MSG_CHANNEL && type != CHAT_MSG_WHISPER) || language == LANG_ADDON)
        *data << uint32(language);
    else
        *data << uint32(LANG_UNIVERSAL);

    switch(type)
    {
        case CHAT_MSG_SAY:
        case CHAT_MSG_PARTY:
        case CHAT_MSG_PARTY_LEADER:
        case CHAT_MSG_RAID:
        case CHAT_MSG_GUILD:
        case CHAT_MSG_OFFICER:
        case CHAT_MSG_YELL:
        case CHAT_MSG_WHISPER:
        case CHAT_MSG_CHANNEL:
        case CHAT_MSG_RAID_LEADER:
        case CHAT_MSG_RAID_WARNING:
        case CHAT_MSG_BG_SYSTEM_NEUTRAL:
        case CHAT_MSG_BG_SYSTEM_ALLIANCE:
        case CHAT_MSG_BG_SYSTEM_HORDE:
        case CHAT_MSG_BATTLEGROUND:
        case CHAT_MSG_BATTLEGROUND_LEADER:
            target_guid = session ? session->GetPlayer()->GetGUID() : 0;
            break;
        case CHAT_MSG_MONSTER_SAY:
        case CHAT_MSG_MONSTER_PARTY:
        case CHAT_MSG_MONSTER_YELL:
        case CHAT_MSG_MONSTER_WHISPER:
        case CHAT_MSG_MONSTER_EMOTE:
        case CHAT_MSG_RAID_BOSS_WHISPER:
        case CHAT_MSG_RAID_BOSS_EMOTE:
        case CHAT_MSG_BATTLENET:
        {
            *data << uint64(speaker->GetGUID());
            *data << uint32(0);                             // 2.1.0
            *data << uint32(strlen(speaker->GetName()) + 1);
            *data << speaker->GetName();
            ObjectGuid listener_guid;
            *data << listener_guid;
            if (!listener_guid.IsEmpty() && !listener_guid.IsPlayer())
            {
                *data << uint32(1);                         // string listener_name_length
                *data << uint8(0);                          // string listener_name
            }
            *data << uint32(messageLength);
            *data << message;
            *data << uint8(0);
            return;
        }
        default:
            if (type != CHAT_MSG_WHISPER_INFORM && type != CHAT_MSG_IGNORED && type != CHAT_MSG_DND && type != CHAT_MSG_AFK)
                target_guid = 0;                            // only for CHAT_MSG_WHISPER_INFORM used original value target_guid
            break;
    }

    *data << uint64(target_guid);                           // there 0 for BG messages
    *data << uint32(0);                                     // can be chat msg group or something

    if (type == CHAT_MSG_CHANNEL)
    {
        ASSERT(channelName);
        *data << channelName;
    }

    *data << uint64(target_guid);
    *data << uint32(messageLength);
    *data << message;
    if(session != 0 && type != CHAT_MSG_WHISPER_INFORM && type != CHAT_MSG_DND && type != CHAT_MSG_AFK)
        *data << uint8(session->GetPlayer()->chatTag());
    else
        *data << uint8(0);
}

Player * ChatHandler::getSelectedPlayer()
{
    if(!m_session)
        return NULL;

    uint64 guid  = m_session->GetPlayer()->GetSelection();

    if (guid == 0)
        return m_session->GetPlayer();

    return sObjectMgr.GetPlayer(guid);
}

Unit* ChatHandler::getSelectedUnit()
{
    if(!m_session)
        return NULL;

    uint64 guid = m_session->GetPlayer()->GetSelection();

    if (guid == 0)
        return m_session->GetPlayer();

    return ObjectAccessor::GetUnit(*m_session->GetPlayer(),guid);
}

Creature* ChatHandler::getSelectedCreature()
{
    if(!m_session)
        return NULL;

    return m_session->GetPlayer()->GetMap()->GetCreatureOrPetOrVehicle(m_session->GetPlayer()->GetSelection());
}

char* ChatHandler::extractKeyFromLink(char* text, char const* linkType, char** something1)
{
    // skip empty
    if(!text)
        return NULL;

    // skip speces
    while(*text==' '||*text=='\t'||*text=='\b')
        ++text;

    if(!*text)
        return NULL;

    // return non link case
    if(text[0]!='|')
        return strtok(text, " ");

    // [name] Shift-click form |color|linkType:key|h[name]|h|r
    // or
    // [name] Shift-click form |color|linkType:key:something1:...:somethingN|h[name]|h|r

    char* check = strtok(text, "|");                        // skip color
    if(!check)
        return NULL;                                        // end of data

    char* cLinkType = strtok(NULL, ":");                    // linktype
    if(!cLinkType)
        return NULL;                                        // end of data

    if(strcmp(cLinkType,linkType) != 0)
    {
        strtok(NULL, " ");                                  // skip link tail (to allow continue strtok(NULL,s) use after retturn from function
        SendSysMessage(LANG_WRONG_LINK_TYPE);
        return NULL;
    }

    char* cKeys = strtok(NULL, "|");                        // extract keys and values
    char* cKeysTail = strtok(NULL, "");

    char* cKey = strtok(cKeys, ":|");                       // extract key
    if(something1)
        *something1 = strtok(NULL, ":|");                   // extract something

    strtok(cKeysTail, "]");                                 // restart scan tail and skip name with possible spaces
    strtok(NULL, " ");                                      // skip link tail (to allow continue strtok(NULL,s) use after return from function
    return cKey;
}

char* ChatHandler::extractKeyFromLink(char* text, char const* const* linkTypes, int* found_idx, char** something1)
{
    // skip empty
    if(!text)
        return NULL;

    // skip speces
    while(*text==' '||*text=='\t'||*text=='\b')
        ++text;

    if(!*text)
        return NULL;

    // return non link case
    if(text[0]!='|')
        return strtok(text, " ");

    // [name] Shift-click form |color|linkType:key|h[name]|h|r
    // or
    // [name] Shift-click form |color|linkType:key:something1:...:somethingN|h[name]|h|r
    // or
    // [name] Shift-click form |linkType:key|h[name]|h|r

    char* tail;

    if(text[1]=='c')
    {
        char* check = strtok(text, "|");                    // skip color
        if(!check)
            return NULL;                                    // end of data

        tail = strtok(NULL, "");                            // tail
    }
    else
        tail = text+1;                                      // skip first |

    char* cLinkType = strtok(tail, ":");                    // linktype
    if(!cLinkType)
        return NULL;                                        // end of data

    for(int i = 0; linkTypes[i]; ++i)
    {
        if(strcmp(cLinkType,linkTypes[i]) == 0)
        {
            char* cKeys = strtok(NULL, "|");                // extract keys and values
            char* cKeysTail = strtok(NULL, "");

            char* cKey = strtok(cKeys, ":|");               // extract key
            if(something1)
                *something1 = strtok(NULL, ":|");           // extract something

            strtok(cKeysTail, "]");                         // restart scan tail and skip name with possible spaces
            strtok(NULL, " ");                              // skip link tail (to allow continue strtok(NULL,s) use after return from function
            if(found_idx)
                *found_idx = i;
            return cKey;
        }
    }

    strtok(NULL, " ");                                      // skip link tail (to allow continue strtok(NULL,s) use after return from function
    SendSysMessage(LANG_WRONG_LINK_TYPE);
    return NULL;
}

GameObject* ChatHandler::GetObjectGlobalyWithGuidOrNearWithDbGuid(uint32 lowguid,uint32 entry)
{
    if(!m_session)
        return NULL;

    Player* pl = m_session->GetPlayer();

    GameObject* obj = pl->GetMap()->GetGameObject(MAKE_NEW_GUID(lowguid, entry, HIGHGUID_GAMEOBJECT));

    if(!obj && sObjectMgr.GetGOData(lowguid))                   // guid is DB guid of object
    {
        MaNGOS::GameObjectWithDbGUIDCheck go_check(*pl,lowguid);
        MaNGOS::GameObjectSearcher<MaNGOS::GameObjectWithDbGUIDCheck> checker(pl,obj,go_check);
        Cell::VisitGridObjects(pl,checker, pl->GetMap()->GetVisibilityDistance());
    }

    return obj;
}

enum SpellLinkType
{
    SPELL_LINK_SPELL   = 0,
    SPELL_LINK_TALENT  = 1,
    SPELL_LINK_ENCHANT = 2,
    SPELL_LINK_TRADE   = 3,
    SPELL_LINK_GLYPH   = 4
};

static char const* const spellKeys[] =
{
    "Hspell",                                               // normal spell
    "Htalent",                                              // talent spell
    "Henchant",                                             // enchanting recipe spell
    "Htrade",                                               // profession/skill spell
    "Hglyph",                                               // glyph
    0
};

uint32 ChatHandler::extractSpellIdFromLink(char* text)
{
    // number or [name] Shift-click form |color|Henchant:recipe_spell_id|h[prof_name: recipe_name]|h|r
    // number or [name] Shift-click form |color|Hglyph:glyph_slot_id:glyph_prop_id|h[%s]|h|r
    // number or [name] Shift-click form |color|Hspell:spell_id|h[name]|h|r
    // number or [name] Shift-click form |color|Htalent:talent_id,rank|h[name]|h|r
    // number or [name] Shift-click form |color|Htrade:spell_id,skill_id,max_value,cur_value|h[name]|h|r
    int type = 0;
    char* param1_str = NULL;
    char* idS = extractKeyFromLink(text,spellKeys,&type,&param1_str);
    if(!idS)
        return 0;

    uint32 id = (uint32)atol(idS);

    switch(type)
    {
        case SPELL_LINK_SPELL:
            return id;
        case SPELL_LINK_TALENT:
        {
            // talent
            TalentEntry const* talentEntry = sTalentStore.LookupEntry(id);
            if(!talentEntry)
                return 0;

            int32 rank = param1_str ? (uint32)atol(param1_str) : 0;
            if(rank >= MAX_TALENT_RANK)
                return 0;

            if(rank < 0)
                rank = 0;

            return talentEntry->RankID[rank];
        }
        case SPELL_LINK_ENCHANT:
        case SPELL_LINK_TRADE:
            return id;
        case SPELL_LINK_GLYPH:
        {
            uint32 glyph_prop_id = param1_str ? (uint32)atol(param1_str) : 0;

            GlyphPropertiesEntry const* glyphPropEntry = sGlyphPropertiesStore.LookupEntry(glyph_prop_id);
            if(!glyphPropEntry)
                return 0;

            return glyphPropEntry->SpellId;
        }
    }

    // unknown type?
    return 0;
}

GameTele const* ChatHandler::extractGameTeleFromLink(char* text)
{
    // id, or string, or [name] Shift-click form |color|Htele:id|h[name]|h|r
    char* cId = extractKeyFromLink(text,"Htele");
    if(!cId)
        return NULL;

    // id case (explicit or from shift link)
    if(cId[0] >= '0' || cId[0] >= '9')
        if(uint32 id = atoi(cId))
            return sObjectMgr.GetGameTele(id);

    return sObjectMgr.GetGameTele(cId);
}

enum GuidLinkType
{
    SPELL_LINK_PLAYER     = 0,                              // must be first for selection in not link case
    SPELL_LINK_CREATURE   = 1,
    SPELL_LINK_GAMEOBJECT = 2
};

static char const* const guidKeys[] =
{
    "Hplayer",
    "Hcreature",
    "Hgameobject",
    NULL
};

uint64 ChatHandler::extractGuidFromLink(char* text)
{
    int type = 0;

    // |color|Hcreature:creature_guid|h[name]|h|r
    // |color|Hgameobject:go_guid|h[name]|h|r
    // |color|Hplayer:name|h[name]|h|r
    char* idS = extractKeyFromLink(text,guidKeys,&type);
    if(!idS)
        return 0;

    switch(type)
    {
        case SPELL_LINK_PLAYER:
        {
            std::string name = idS;
            if(!normalizePlayerName(name))
                return 0;

            if(Player* player = sObjectMgr.GetPlayer(name.c_str()))
                return player->GetGUID();

            if(uint64 guid = sObjectMgr.GetPlayerGUIDByName(name))
                return guid;

            return 0;
        }
        case SPELL_LINK_CREATURE:
        {
            uint32 lowguid = (uint32)atol(idS);

            if(CreatureData const* data = sObjectMgr.GetCreatureData(lowguid) )
                return MAKE_NEW_GUID(lowguid,data->id,HIGHGUID_UNIT);
            else
                return 0;
        }
        case SPELL_LINK_GAMEOBJECT:
        {
            uint32 lowguid = (uint32)atol(idS);

            if(GameObjectData const* data = sObjectMgr.GetGOData(lowguid) )
                return MAKE_NEW_GUID(lowguid,data->id,HIGHGUID_GAMEOBJECT);
            else
                return 0;
        }
    }

    // unknown type?
    return 0;
}

enum LocationLinkType
{
    LOCATION_LINK_PLAYER            = 0,                    // must be first for selection in not link case
    LOCATION_LINK_TELE              = 1,
    LOCATION_LINK_TAXINODE          = 2,
    LOCATION_LINK_CREATURE          = 3,
    LOCATION_LINK_GAMEOBJECT        = 4,
    LOCATION_LINK_CREATURE_ENTRY    = 5,
    LOCATION_LINK_GAMEOBJECT_ENTRY  = 6
};

static char const* const locationKeys[] =
{
    "Htele",
    "Htaxinode",
    "Hplayer",
    "Hcreature",
    "Hgameobject",
    "Hcreature_entry",
    "Hgameobject_entry",
    NULL
};

bool ChatHandler::extractLocationFromLink(char* text, uint32& mapid, float& x, float& y, float& z)
{
    int type = 0;

    // |color|Hplayer:name|h[name]|h|r
    // |color|Htele:id|h[name]|h|r
    // |color|Htaxinode:id|h[name]|h|r
    // |color|Hcreature:creature_guid|h[name]|h|r
    // |color|Hgameobject:go_guid|h[name]|h|r
    // |color|Hcreature_entry:creature_id|h[name]|h|r
    // |color|Hgameobject_entry:go_id|h[name]|h|r
    char* idS = extractKeyFromLink(text,locationKeys,&type);
    if(!idS)
        return false;

    switch(type)
    {
        // it also fail case
        case LOCATION_LINK_PLAYER:
        {
            // not link and not name, possible coordinates/etc
            if (isNumeric(idS[0]))
                return false;

            std::string name = idS;
            if(!normalizePlayerName(name))
                return false;

            if(Player* player = sObjectMgr.GetPlayer(name.c_str()))
            {
                mapid = player->GetMapId();
                x = player->GetPositionX();
                y = player->GetPositionY();
                z = player->GetPositionZ();
                return true;
            }

            if(uint64 guid = sObjectMgr.GetPlayerGUIDByName(name))
            {
                // to point where player stay (if loaded)
                float o;
                bool in_flight;
                return Player::LoadPositionFromDB(mapid, x, y, z, o, in_flight, guid);
            }

            return false;
        }
        case LOCATION_LINK_TELE:
        {
            uint32 id = (uint32)atol(idS);
            GameTele const* tele = sObjectMgr.GetGameTele(id);
            if (!tele)
                return false;
            mapid = tele->mapId;
            x = tele->position_x;
            y = tele->position_y;
            z = tele->position_z;
            return true;
        }
        case LOCATION_LINK_TAXINODE:
        {
            uint32 id = (uint32)atol(idS);
            TaxiNodesEntry const* node = sTaxiNodesStore.LookupEntry(id);
            if (!node)
                return false;
            mapid = node->map_id;
            x = node->x;
            y = node->y;
            z = node->z;
            return true;
        }
        case LOCATION_LINK_CREATURE:
        {
            uint32 lowguid = (uint32)atol(idS);

            if(CreatureData const* data = sObjectMgr.GetCreatureData(lowguid) )
            {
                mapid = data->mapid;
                x = data->posX;
                y = data->posY;
                z = data->posZ;
                return true;
            }
            else
                return false;
        }
        case LOCATION_LINK_GAMEOBJECT:
        {
            uint32 lowguid = (uint32)atol(idS);

            if(GameObjectData const* data = sObjectMgr.GetGOData(lowguid) )
            {
                mapid = data->mapid;
                x = data->posX;
                y = data->posY;
                z = data->posZ;
                return true;
            }
            else
                return false;
        }
        case LOCATION_LINK_CREATURE_ENTRY:
        {
            uint32 id = (uint32)atol(idS);

            if (sObjectMgr.GetCreatureTemplate(id))
            {
                FindCreatureData worker(id, m_session ? m_session->GetPlayer() : NULL);

                sObjectMgr.DoCreatureData(worker);

                if (CreatureDataPair const* dataPair = worker.GetResult())
                {
                    mapid = dataPair->second.mapid;
                    x = dataPair->second.posX;
                    y = dataPair->second.posY;
                    z = dataPair->second.posZ;
                    return true;
                }
                else
                    return false;
            }
            else
                return false;
        }
        case LOCATION_LINK_GAMEOBJECT_ENTRY:
        {
            uint32 id = (uint32)atol(idS);

            if (sObjectMgr.GetGameObjectInfo(id))
            {
                FindGOData worker(id, m_session ? m_session->GetPlayer() : NULL);

                sObjectMgr.DoGOData(worker);

                if (GameObjectDataPair const* dataPair = worker.GetResult())
                {
                    mapid = dataPair->second.mapid;
                    x = dataPair->second.posX;
                    y = dataPair->second.posY;
                    z = dataPair->second.posZ;
                    return true;
                }
                else
                    return false;
            }
            else
                return false;
        }
    }

    // unknown type?
    return false;
}

std::string ChatHandler::extractPlayerNameFromLink(char* text)
{
    // |color|Hplayer:name|h[name]|h|r
    char* name_str = extractKeyFromLink(text,"Hplayer");
    if(!name_str)
        return "";

    std::string name = name_str;
    if(!normalizePlayerName(name))
        return "";

    return name;
}

bool ChatHandler::extractPlayerTarget(char* args, Player** player, uint64* player_guid /*=NULL*/,std::string* player_name /*= NULL*/)
{
    if (args && *args)
    {
        std::string name = extractPlayerNameFromLink(args);
        if (name.empty())
        {
            SendSysMessage(LANG_PLAYER_NOT_FOUND);
            SetSentErrorMessage(true);
            return false;
        }

        Player* pl = sObjectMgr.GetPlayer(name.c_str());

        // if allowed player pointer
        if(player)
            *player = pl;

        // if need guid value from DB (in name case for check player existence)
        uint64 guid = !pl && (player_guid || player_name) ? sObjectMgr.GetPlayerGUIDByName(name) : 0;

        // if allowed player guid (if no then only online players allowed)
        if(player_guid)
            *player_guid = pl ? pl->GetGUID() : guid;

        if(player_name)
            *player_name = pl || guid ? name : "";
    }
    else
    {
        Player* pl = getSelectedPlayer();
        // if allowed player pointer
        if(player)
            *player = pl;
        // if allowed player guid (if no then only online players allowed)
        if(player_guid)
            *player_guid = pl ? pl->GetGUID() : 0;

        if(player_name)
            *player_name = pl ? pl->GetName() : "";
    }

    // some from req. data must be provided (note: name is empty if player not exist)
    if((!player || !*player) && (!player_guid || !*player_guid) && (!player_name || player_name->empty()))
    {
        SendSysMessage(LANG_PLAYER_NOT_FOUND);
        SetSentErrorMessage(true);
        return false;
    }

    return true;
}

void ChatHandler::extractOptFirstArg(char* args, char** arg1, char** arg2)
{
    char* p1 = strtok(args, " ");
    char* p2 = strtok(NULL, " ");

    if(!p2)
    {
        p2 = p1;
        p1 = NULL;
    }

    if(arg1)
        *arg1 = p1;

    if(arg2)
        *arg2 = p2;
}

char* ChatHandler::extractQuotedArg( char* args )
{
    if(!*args)
        return NULL;

    if(*args=='"')
        return strtok(args+1, "\"");
    else
    {
        char* space = strtok(args, "\"");
        if(!space)
            return NULL;
        return strtok(NULL, "\"");
    }
}

uint32 ChatHandler::extractAccountId(char* args, std::string* accountName /*= NULL*/, Player** targetIfNullArg /*= NULL*/)
{
    uint32 account_id = 0;

    ///- Get the account name from the command line
    char* account_str = args ? strtok (args," ") : NULL;

    if (!account_str)
    {
        if (!targetIfNullArg)
            return 0;

        /// only target player different from self allowed (if targetPlayer!=NULL then not console)
        Player* targetPlayer = getSelectedPlayer();
        if (!targetPlayer)
            return 0;

        account_id = targetPlayer->GetSession()->GetAccountId();

        if (accountName)
            sAccountMgr.GetName(account_id, *accountName);

        if (targetIfNullArg)
            *targetIfNullArg = targetPlayer;

        return account_id;
    }

    std::string account_name;

    if (isNumeric(account_str))
    {
        long id = atol(account_str);
        if (id <= 0 || ((unsigned long)id) >= std::numeric_limits<uint32>::max())
        {
            PSendSysMessage(LANG_ACCOUNT_NOT_EXIST,account_str);
            SetSentErrorMessage(true);
            return 0;
        }

        account_id = uint32(id);

        if (!sAccountMgr.GetName(account_id, account_name))
        {
            PSendSysMessage(LANG_ACCOUNT_NOT_EXIST,account_str);
            SetSentErrorMessage(true);
            return 0;
        }
    }
    else
    {
        account_name = account_str;
        if (!AccountMgr::normalizeString(account_name))
        {
            PSendSysMessage(LANG_ACCOUNT_NOT_EXIST,account_name.c_str());
            SetSentErrorMessage(true);
            return 0;
        }

        account_id = sAccountMgr.GetId(account_name);
        if (!account_id)
        {
            PSendSysMessage(LANG_ACCOUNT_NOT_EXIST,account_name.c_str());
            SetSentErrorMessage(true);
            return 0;
        }
    }

    if (accountName)
        *accountName = account_name;

    if (targetIfNullArg)
        *targetIfNullArg = NULL;

    return account_id;
}

bool ChatHandler::needReportToTarget(Player* chr) const
{
    Player* pl = m_session->GetPlayer();
    return pl != chr && pl->IsVisibleGloballyFor(chr);
}

LocaleConstant ChatHandler::GetSessionDbcLocale() const
{
    return m_session->GetSessionDbcLocale();
}

int ChatHandler::GetSessionDbLocaleIndex() const
{
    return m_session->GetSessionDbLocaleIndex();
}

const char *CliHandler::GetMangosString(int32 entry) const
{
    return sObjectMgr.GetMangosStringForDBCLocale(entry);
}

uint32 CliHandler::GetAccountId() const
{
    return m_accountId;
}

AccountTypes CliHandler::GetAccessLevel() const
{
    return m_loginAccessLevel;
}

bool CliHandler::isAvailable(ChatCommand const& cmd) const
{
    // skip non-console commands in console case
    if (!cmd.AllowConsole)
        return false;

    // normal case
    return GetAccessLevel() >= (AccountTypes)cmd.SecurityLevel;
}

void CliHandler::SendSysMessage(const char *str)
{
    m_print(m_callbackArg, str);
    m_print(m_callbackArg, "\r\n");
}

std::string CliHandler::GetNameLink() const
{
    return GetMangosString(LANG_CONSOLE_COMMAND);
}

bool CliHandler::needReportToTarget(Player* /*chr*/) const
{
    return true;
}

LocaleConstant CliHandler::GetSessionDbcLocale() const
{
    return sWorld.GetDefaultDbcLocale();
}

int CliHandler::GetSessionDbLocaleIndex() const
{
    return sObjectMgr.GetDBCLocaleIndex();
}

// Check/ Output if a NPC or GO (by guid) is part of a pool or game event
template <typename T>
void ChatHandler::ShowNpcOrGoSpawnInformation(uint32 guid)
{
    if (uint16 pool_id = sPoolMgr.IsPartOfAPool<T>(guid))
    {
        uint16 top_pool_id = sPoolMgr.IsPartOfTopPool<Pool>(pool_id);
        if (!top_pool_id || top_pool_id == pool_id)
            PSendSysMessage(LANG_NPC_GO_INFO_POOL, pool_id);
        else
            PSendSysMessage(LANG_NPC_GO_INFO_TOP_POOL, pool_id, top_pool_id);

        if (int16 event_id = sGameEventMgr.GetGameEventId<Pool>(top_pool_id))
        {
            GameEventMgr::GameEventDataMap const& events = sGameEventMgr.GetEventMap();
            GameEventData const& eventData = events[std::abs(event_id)];

            if (event_id > 0)
                PSendSysMessage(LANG_NPC_GO_INFO_POOL_GAME_EVENT_S, top_pool_id, std::abs(event_id), eventData.description.c_str());
            else
                PSendSysMessage(LANG_NPC_GO_INFO_POOL_GAME_EVENT_D, top_pool_id, std::abs(event_id), eventData.description.c_str());
        }
    }
    else if (int16 event_id = sGameEventMgr.GetGameEventId<T>(guid))
    {
        GameEventMgr::GameEventDataMap const& events = sGameEventMgr.GetEventMap();
        GameEventData const& eventData = events[std::abs(event_id)];

        if (event_id > 0)
            PSendSysMessage(LANG_NPC_GO_INFO_GAME_EVENT_S, std::abs(event_id), eventData.description.c_str());
        else
            PSendSysMessage(LANG_NPC_GO_INFO_GAME_EVENT_D, std::abs(event_id), eventData.description.c_str());
    }
}

// Prepare ShortString for a NPC or GO (by guid) with pool or game event IDs
template <typename T>
std::string ChatHandler::PrepareStringNpcOrGoSpawnInformation(uint32 guid)
{
    std::string str = "";
    if (uint16 pool_id = sPoolMgr.IsPartOfAPool<T>(guid))
    {
        uint16 top_pool_id = sPoolMgr.IsPartOfTopPool<T>(guid);
        if (int16 event_id = sGameEventMgr.GetGameEventId<Pool>(top_pool_id))
        {
            char buffer[100];
            const char* format = GetMangosString(LANG_NPC_GO_INFO_POOL_EVENT_STRING);
            sprintf(buffer, format, pool_id, event_id);
            str = buffer;
        }
        else
        {
            char buffer[100];
            const char* format = GetMangosString(LANG_NPC_GO_INFO_POOL_STRING);
            sprintf(buffer, format, pool_id);
            str = buffer;
        }
    }
    else if (int16 event_id = sGameEventMgr.GetGameEventId<T>(guid))
    {
        char buffer[100];
        const char* format = GetMangosString(LANG_NPC_GO_INFO_EVENT_STRING);
        sprintf(buffer, format, event_id);
        str = buffer;
    }

    return str;
}

// Instantiate template for helper function
template void ChatHandler::ShowNpcOrGoSpawnInformation<Creature>(uint32 guid);
template void ChatHandler::ShowNpcOrGoSpawnInformation<GameObject>(uint32 guid);

template std::string ChatHandler::PrepareStringNpcOrGoSpawnInformation<Creature>(uint32 guid);
template std::string ChatHandler::PrepareStringNpcOrGoSpawnInformation<GameObject>(uint32 guid);
