/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

#include "precompiled.h"
#include "def_spire.h"

static Locations SpawnLoc[]=
{
    {-446.788971, 2003.362915, 191.233948},  // 0 Horde ship enter
    {-428.140503, 2421.336914, 191.233078},  // 1 Alliance ship enter
};

struct MANGOS_DLL_DECL instance_icecrown_spire : public ScriptedInstance
{
    instance_icecrown_spire(Map* pMap) : ScriptedInstance(pMap)
    {
        Difficulty = pMap->GetDifficulty();
        Initialize();
    }

    uint8 Difficulty;
    bool needSave;
    std::string strSaveData;

    //Creatures GUID
    uint32 m_auiEncounter[MAX_ENCOUNTERS+1];
    uint64 m_uiMarrogwarGUID;
    uint64 m_uiDeathWhisperGUID;
    uint64 m_uiSaurfangGUID;
    uint64 m_uiRotfaceGUID;
    uint64 m_uiFestergutGUID;
    uint64 m_uiPutricideGUID;
    uint64 m_uiTaldaramGUID;
    uint64 m_uiValanarGUID;
    uint64 m_uiKelesethGUID;
    uint64 m_uiLanathelGUID;
    uint64 m_uiValithriaGUID;
    uint64 m_uiSindragosaGUID;
    uint64 m_uiLichKingGUID;

    uint64 m_uiRimefangGUID;
    uint64 m_uiSpinestalkerGUID;

    uint64 m_uiStinkyGUID;
    uint64 m_uiPreciousGUID;

    uint64 m_uiIcewall1GUID;
    uint64 m_uiIcewall2GUID;
    uint64 m_uiSaurfangDoorGUID;
    uint64 m_uiOratoryDoorGUID;
    uint64 m_uiDeathWhisperElevatorGUID;
    uint64 m_uiOrangePlagueGUID;
    uint64 m_uiGreenPlagueGUID;
    uint64 m_uiSDoorGreenGUID;
    uint64 m_uiSDoorOrangeGUID;
    uint64 m_uiSDoorCollisionGUID;
    uint64 m_uiScientistDoorGUID;
    uint64 m_uiCrimsonDoorGUID;
    uint64 m_uiBloodwingDoorGUID;
    uint64 m_uiCounsilDoor1GUID;
    uint64 m_uiCounsilDoor2GUID;
    uint64 m_uiGreenDragonDoor1GUID;
    uint64 m_uiGreenDragonDoor2GUID;
    uint64 m_uiFrostwingDoorGUID;

    uint64 m_uiValithriaDoor1GUID;
    uint64 m_uiValithriaDoor2GUID;
    uint64 m_uiValithriaDoor3GUID;
    uint64 m_uiValithriaDoor4GUID;

    uint64 m_uiSindragosaDoor1GUID;
    uint64 m_uiSindragosaDoor2GUID;

    uint64 m_uiIceShard1GUID;
    uint64 m_uiIceShard2GUID;
    uint64 m_uiIceShard3GUID;
    uint64 m_uiIceShard4GUID;

    uint64 m_uiFrostyWindGUID;
    uint64 m_uiFrostyEdgeGUID;

    uint64 m_uiFrostmourneGUID;
    uint64 m_uiFrostmourneTriggerGUID;
    uint64 m_uiFrostmourneHolderGUID;

    uint64 m_uiSaurfangCacheGUID;
    uint64 m_uiGunshipArmoryAGUID;
    uint64 m_uiGunshipArmoryHGUID;
    uint64 m_uiValitriaCacheGUID;

    uint64 m_uiGunshipArmoryH_ID;
    uint64 m_uiGunshipArmoryA_ID;

    uint32 m_uiDataCouncilHealth;

    uint32 m_auiEvent;
    uint32 m_auiEventTimer;

    void OpenDoor(uint64 guid)
    {
        if(!guid) return;
        GameObject* pGo = instance->GetGameObject(guid);
        if(pGo) pGo->SetGoState(GO_STATE_ACTIVE_ALTERNATIVE);
    }

    void CloseDoor(uint64 guid)
    {
        if(!guid) return;
        GameObject* pGo = instance->GetGameObject(guid);
        if(pGo) pGo->SetGoState(GO_STATE_READY);
    }

    void OpenAllDoors()
    {
        if (m_auiEncounter[1] == DONE) {
                                        OpenDoor(m_uiIcewall1GUID);
                                        OpenDoor(m_uiIcewall2GUID);
                            if (GameObject* pGO = instance->GetGameObject(m_uiDeathWhisperElevatorGUID))
                            {
                              pGO->SetUInt32Value(GAMEOBJECT_LEVEL, 0);
                              pGO->SetGoState(GO_STATE_READY);
                            }
                                        };
        if (m_auiEncounter[2] == DONE) {
                                       };
        if (m_auiEncounter[5] == DONE) OpenDoor(m_uiSDoorOrangeGUID);
        if (m_auiEncounter[6] == DONE) OpenDoor(m_uiSDoorGreenGUID);
        if (m_auiEncounter[6] == DONE && m_auiEncounter[5] == DONE) OpenDoor(m_uiSDoorCollisionGUID);
        if (m_auiEncounter[7] == DONE) OpenDoor(m_uiBloodwingDoorGUID);
        if (m_auiEncounter[8] == DONE) {
                                        OpenDoor(m_uiCounsilDoor1GUID);
                                        OpenDoor(m_uiCounsilDoor2GUID);
                                        };
        if (m_auiEncounter[9] == DONE) OpenDoor(m_uiFrostwingDoorGUID);
        if (m_auiEncounter[10] == DONE) OpenDoor(m_uiValithriaDoor2GUID);
        if (m_auiEncounter[11] == DONE) {
                                        OpenDoor(m_uiSindragosaDoor2GUID);
                                        OpenDoor(m_uiSindragosaDoor1GUID);
                                        };

    }

    void Initialize()
    {
        for (uint8 i = 0; i < MAX_ENCOUNTERS; ++i)
            m_auiEncounter[i] = NOT_STARTED;

        m_auiEncounter[0] = 0;

        m_uiMarrogwarGUID = 0;
        m_uiDeathWhisperGUID = 0;
        m_uiSaurfangGUID = 0;
        m_uiSaurfangCacheGUID = 0;
        m_uiGunshipArmoryAGUID = 0;
        m_uiGunshipArmoryHGUID = 0;
        m_uiIcewall1GUID = 0;
        m_uiIcewall2GUID = 0;
        m_uiSDoorOrangeGUID = 0;
        m_uiSDoorGreenGUID = 0;
        m_uiBloodwingDoorGUID = 0;
        m_uiSDoorCollisionGUID = 0;
        m_auiEvent = 0;
        m_auiEventTimer = 1000;
        switch (Difficulty) {
                             case RAID_DIFFICULTY_10MAN_NORMAL:
                                       m_uiGunshipArmoryH_ID = GO_GUNSHIP_ARMORY_H_10;
                                       m_uiGunshipArmoryA_ID = GO_GUNSHIP_ARMORY_A_10;
                                       break;
                             case RAID_DIFFICULTY_10MAN_HEROIC:
                                       m_uiGunshipArmoryH_ID = GO_GUNSHIP_ARMORY_H_10H;
                                       m_uiGunshipArmoryA_ID = GO_GUNSHIP_ARMORY_A_10H;
                                       break;
                             case RAID_DIFFICULTY_25MAN_NORMAL:
                                       m_uiGunshipArmoryH_ID = GO_GUNSHIP_ARMORY_H_25;
                                       m_uiGunshipArmoryA_ID = GO_GUNSHIP_ARMORY_A_25;
                                       break;
                             case RAID_DIFFICULTY_25MAN_HEROIC:
                                       m_uiGunshipArmoryH_ID = GO_GUNSHIP_ARMORY_H_25H;
                                       m_uiGunshipArmoryA_ID = GO_GUNSHIP_ARMORY_A_25H;
                                       break;
                             default:
                                       m_uiGunshipArmoryH_ID = 0;
                                       m_uiGunshipArmoryA_ID = 0;
                                       break;
                             };
    }

    void OnPlayerEnter(Player *m_player)
    {
        OpenAllDoors();
    }

    bool IsEncounterInProgress() const
    {
        for(uint8 i = 1; i < MAX_ENCOUNTERS-3 ; ++i)
            if (m_auiEncounter[i] == IN_PROGRESS) return true;

        return false;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case NPC_LORD_MARROWGAR:
                         m_uiMarrogwarGUID = pCreature->GetGUID();
                         break;
            case NPC_LADY_DEATHWHISPER:
                          m_uiDeathWhisperGUID = pCreature->GetGUID();
                          break;
            case NPC_DEATHBRINGER_SAURFANG:
                          m_uiSaurfangGUID = pCreature->GetGUID();
                          break;
            case NPC_FESTERGUT:
                          m_uiFestergutGUID = pCreature->GetGUID();
                          break;
            case NPC_ROTFACE:
                          m_uiRotfaceGUID = pCreature->GetGUID();
                          break;
            case NPC_PROFESSOR_PUTRICIDE:
                          m_uiPutricideGUID = pCreature->GetGUID();
                          break;
            case NPC_TALDARAM:
                          m_uiTaldaramGUID = pCreature->GetGUID();
                          break;
            case NPC_VALANAR:
                          m_uiValanarGUID = pCreature->GetGUID();
                          break;
            case NPC_KELESETH:
                          m_uiKelesethGUID = pCreature->GetGUID();
                          break;
            case NPC_LANATHEL:
                          m_uiLanathelGUID = pCreature->GetGUID();
                          break;
            case NPC_VALITHRIA:
                          m_uiValithriaGUID = pCreature->GetGUID();
                          break;
            case NPC_SINDRAGOSA:
                          m_uiSindragosaGUID = pCreature->GetGUID();
                          break;
            case NPC_LICH_KING:
                          m_uiLichKingGUID = pCreature->GetGUID();
                          break;
            case NPC_RIMEFANG:
                          m_uiRimefangGUID = pCreature->GetGUID();
                          break;
            case NPC_SPINESTALKER:
                          m_uiSpinestalkerGUID = pCreature->GetGUID();
                          break;
            case NPC_STINKY:
                          m_uiStinkyGUID = pCreature->GetGUID();
                          break;
            case NPC_PRECIOUS:
                          m_uiPreciousGUID = pCreature->GetGUID();
                          break;
            case NPC_FROSTMOURNE_TRIGGER:
                          m_uiFrostmourneTriggerGUID = pCreature->GetGUID(); break;
            case NPC_FROSTMOURNE_HOLDER:
                          m_uiFrostmourneHolderGUID = pCreature->GetGUID(); break;
        }
    }

    void OnObjectCreate(GameObject* pGo)
    {
        switch(pGo->GetEntry())
        {
            case GO_ICEWALL_1:
                         m_uiIcewall1GUID = pGo->GetGUID();
                         break;
            case GO_ICEWALL_2:
                         m_uiIcewall2GUID = pGo->GetGUID();
                         break;
            case GO_ORATORY_DOOR:
                         m_uiOratoryDoorGUID = pGo->GetGUID();
                         break;
            case GO_DEATHWHISPER_ELEVATOR:
                         m_uiDeathWhisperElevatorGUID = pGo->GetGUID();
                         break;
            case GO_SAURFANG_DOOR:
                         m_uiSaurfangDoorGUID = pGo->GetGUID();
                         break;
            case GO_ORANGE_PLAGUE:
                         m_uiOrangePlagueGUID = pGo->GetGUID();
                         break;
            case GO_GREEN_PLAGUE:
                         m_uiGreenPlagueGUID = pGo->GetGUID();
                         break;
            case GO_SCIENTIST_DOOR_GREEN:
                         m_uiSDoorGreenGUID = pGo->GetGUID();
                         break;
            case GO_SCIENTIST_DOOR_ORANGE:
                         m_uiSDoorOrangeGUID = pGo->GetGUID();
                         break;
            case GO_SCIENTIST_DOOR_COLLISION:
                         m_uiSDoorCollisionGUID = pGo->GetGUID();
                         break;
            case GO_SCIENTIST_DOOR:
                         m_uiScientistDoorGUID = pGo->GetGUID();
                         break;
            case GO_CRIMSON_HALL_DOOR:
                         m_uiCrimsonDoorGUID = pGo->GetGUID();
                         break;
            case GO_BLOODWING_DOOR:
                         m_uiBloodwingDoorGUID = pGo->GetGUID();
                         break;
            case GO_COUNCIL_DOOR_1:
                         m_uiCounsilDoor1GUID = pGo->GetGUID();
                         break;
            case GO_COUNCIL_DOOR_2:
                         m_uiCounsilDoor2GUID = pGo->GetGUID();
                         break;
            case GO_FROSTWING_DOOR:
                         m_uiFrostwingDoorGUID = pGo->GetGUID();
                         break;
            case GO_GREEN_DRAGON_DOOR_1:
                         m_uiGreenDragonDoor1GUID = pGo->GetGUID();
                         break;
            case GO_GREEN_DRAGON_DOOR_2:
                         m_uiGreenDragonDoor2GUID = pGo->GetGUID();
                         break;
            case GO_VALITHRIA_DOOR_1:
                         m_uiValithriaDoor1GUID = pGo->GetGUID();
                         break;
            case GO_VALITHRIA_DOOR_2:
                         m_uiValithriaDoor2GUID = pGo->GetGUID();
                         break;
            case GO_VALITHRIA_DOOR_3:
                         m_uiValithriaDoor3GUID = pGo->GetGUID();
                         break;
            case GO_VALITHRIA_DOOR_4:
                         m_uiValithriaDoor4GUID = pGo->GetGUID();
                         break;
            case GO_SINDRAGOSA_DOOR_1:
                         m_uiSindragosaDoor1GUID = pGo->GetGUID();
                         break;
            case GO_SINDRAGOSA_DOOR_2:
                         m_uiSindragosaDoor2GUID = pGo->GetGUID();
                         break;
            case GO_SAURFANG_CACHE_10:
                                  if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                                  m_uiSaurfangCacheGUID = pGo->GetGUID();
                                  break;
            case GO_SAURFANG_CACHE_25:
                                  if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                                  m_uiSaurfangCacheGUID = pGo->GetGUID();
                                  break;
            case GO_SAURFANG_CACHE_10_H:
                                  if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                                  m_uiSaurfangCacheGUID = pGo->GetGUID();
                                  break;
            case GO_SAURFANG_CACHE_25_H:
                                  if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                                  m_uiSaurfangCacheGUID = pGo->GetGUID();
                                  break;
            case GO_GUNSHIP_ARMORY_A_10:
                                  if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                                  m_uiGunshipArmoryAGUID = pGo->GetGUID();
                                  break;
            case GO_GUNSHIP_ARMORY_A_25:
                                  if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                                  m_uiGunshipArmoryAGUID = pGo->GetGUID();
                                  break;
            case GO_GUNSHIP_ARMORY_A_10H:
                                  if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                                  m_uiGunshipArmoryAGUID = pGo->GetGUID();
                                  break;
            case GO_GUNSHIP_ARMORY_A_25H:
                                  if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                                  m_uiGunshipArmoryAGUID = pGo->GetGUID();
                                  break;
            case GO_GUNSHIP_ARMORY_H_10:
                                  if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                                  m_uiGunshipArmoryHGUID = pGo->GetGUID();
                                  break;
            case GO_GUNSHIP_ARMORY_H_25:
                                  if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                                  m_uiGunshipArmoryHGUID = pGo->GetGUID();
                                  break;
            case GO_GUNSHIP_ARMORY_H_10H:
                                  if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                                  m_uiGunshipArmoryHGUID = pGo->GetGUID();
                                  break;
            case GO_GUNSHIP_ARMORY_H_25H:
                                  if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                                  m_uiGunshipArmoryHGUID = pGo->GetGUID();
                                  break;
            case GO_DREAMWALKER_CACHE_10:
                                  if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                                  m_uiValitriaCacheGUID = pGo->GetGUID();
                                  break;
            case GO_DREAMWALKER_CACHE_25:
                                  if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                                  m_uiValitriaCacheGUID = pGo->GetGUID();
                                  break;
            case GO_DREAMWALKER_CACHE_10_H:
                                  if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                                  m_uiValitriaCacheGUID = pGo->GetGUID();
                                  break;
            case GO_DREAMWALKER_CACHE_25_H:
                                  if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                                  m_uiValitriaCacheGUID = pGo->GetGUID();
                                  break;
            case GO_ICESHARD_1:
                                  m_uiIceShard1GUID = pGo->GetGUID();
                                  break;
            case GO_ICESHARD_2:
                                  m_uiIceShard2GUID = pGo->GetGUID();
                                  break;
            case GO_ICESHARD_3:
                                  m_uiIceShard3GUID = pGo->GetGUID();
                                  break;
            case GO_ICESHARD_4:
                                  m_uiIceShard4GUID = pGo->GetGUID();
                                  break;
            case GO_FROSTY_WIND:
                                  m_uiFrostyWindGUID = pGo->GetGUID();
                                  break;
            case GO_FROSTY_EDGE:
                                  m_uiFrostyEdgeGUID = pGo->GetGUID();
                                  break;
        }
        OpenAllDoors();
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        if (uiType > m_auiEncounter[0] && uiData == DONE) m_auiEncounter[0] = uiType;
        switch(uiType)
        {
            case TYPE_TELEPORT:
                break;
            case TYPE_MARROWGAR:
                m_auiEncounter[1] = uiData;
                if (uiData == DONE)
                {
                    OpenDoor(m_uiIcewall1GUID);
                    OpenDoor(m_uiIcewall2GUID);
                    if (GameObject* pGO = instance->GetGameObject(m_uiDeathWhisperElevatorGUID))
                    {
                        pGO->SetUInt32Value(GAMEOBJECT_LEVEL, 0);
                        pGO->SetGoState(GO_STATE_READY);
                    }
                }
                break;
             case TYPE_DEATHWHISPER:
                m_auiEncounter[2] = uiData;
                if (uiData == DONE) {}
                break;
             case TYPE_FLIGHT_WAR:
                if (uiData == DONE && m_auiEncounter[3] != DONE  ) {
                                 if (GameObject* pChest = instance->GetGameObject(m_uiGunshipArmoryAGUID))
                                     if (pChest && !pChest->isSpawned()) {
                                          pChest->SetRespawnTime(7*DAY);
                                      };

                                 if (GameObject* pChest = instance->GetGameObject(m_uiGunshipArmoryHGUID))
                                     if (pChest && !pChest->isSpawned()) {
                                          pChest->SetRespawnTime(7*DAY);
                                      };
                                };
                m_auiEncounter[3] = uiData;
                break;
             case TYPE_SAURFANG:
                m_auiEncounter[4] = uiData;
                if (uiData == DONE) {
                OpenDoor(m_uiSaurfangDoorGUID);
                                 if (GameObject* pChest = instance->GetGameObject(m_uiSaurfangCacheGUID))
                                     if (pChest && !pChest->isSpawned()) {
                                          pChest->SetRespawnTime(7*DAY);
                                      };
                                };
                break;
             case TYPE_FESTERGUT:
                m_auiEncounter[5] = uiData;
                if (uiData == IN_PROGRESS) CloseDoor(m_uiOrangePlagueGUID);
                                      else OpenDoor(m_uiOrangePlagueGUID);
                if (uiData == DONE)  {
                                     OpenDoor(m_uiSDoorOrangeGUID);
                                     if (m_auiEncounter[6] == DONE) OpenDoor(m_uiSDoorCollisionGUID);
                                     }
                break;
             case TYPE_ROTFACE:
                m_auiEncounter[6] = uiData;
                if (uiData == IN_PROGRESS) CloseDoor(m_uiGreenPlagueGUID);
                                      else OpenDoor(m_uiGreenPlagueGUID);
                if (uiData == DONE) {
                                     OpenDoor(m_uiSDoorGreenGUID);
                                     if (m_auiEncounter[5] == DONE) OpenDoor(m_uiSDoorCollisionGUID);
                                     }
                break;
             case TYPE_PUTRICIDE:
                m_auiEncounter[7] = uiData;
                if (uiData == IN_PROGRESS) CloseDoor(m_uiScientistDoorGUID);
                                      else OpenDoor(m_uiScientistDoorGUID);
                if (uiData == DONE) OpenDoor(m_uiBloodwingDoorGUID);
                break;
             case TYPE_BLOOD_COUNCIL:
                m_auiEncounter[8] = uiData;
                if (uiData == DONE) {
                                     OpenDoor(m_uiCounsilDoor1GUID);
                                     OpenDoor(m_uiCounsilDoor2GUID);
                                    }
                break;
             case TYPE_LANATHEL:
                m_auiEncounter[9] = uiData;
                if (uiData == DONE)  OpenDoor(m_uiFrostwingDoorGUID);
                break;
             case TYPE_VALITHRIA:
                m_auiEncounter[10] = uiData;
                if (uiData == DONE) {
                OpenDoor(m_uiGreenDragonDoor2GUID);
                                 if (GameObject* pChest = instance->GetGameObject(m_uiValitriaCacheGUID))
                                     if (pChest && !pChest->isSpawned()) {
                                          pChest->SetRespawnTime(7*DAY);
                                      };
                                };
                break;
             case TYPE_SINDRAGOSA:
                m_auiEncounter[11] = uiData;
                if (uiData == DONE) {
                                     OpenDoor(m_uiSindragosaDoor1GUID);
                                     OpenDoor(m_uiSindragosaDoor2GUID);
                                    }
                break;
             case TYPE_LICH_KING:
                m_auiEncounter[12] = uiData;
                break;
             case TYPE_ICECROWN_QUESTS:
                m_auiEncounter[13] = uiData;
                break;
             case TYPE_COUNT:
                m_auiEncounter[14] = uiData;
                uiData = NOT_STARTED;
                break;
             case DATA_BLOOD_COUNCIL_HEALTH:     m_uiDataCouncilHealth = uiData;
                                                 uiData = NOT_STARTED;
                                                 break;
             case TYPE_EVENT:            m_auiEvent = uiData; uiData = NOT_STARTED; break;
             case TYPE_EVENT_TIMER:      m_auiEventTimer = uiData; uiData = NOT_STARTED; break;
        }

        if (uiData == DONE)
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;

            for(uint8 i = 0; i < MAX_ENCOUNTERS; ++i)
                saveStream << m_auiEncounter[i] << " ";

            strSaveData = saveStream.str();

            SaveToDB();
            OUT_SAVE_INST_DATA_COMPLETE;
        }
    }

    const char* Save()
    {
        return strSaveData.c_str();
    }

    uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
             case TYPE_DIFFICULTY:    return Difficulty;
             case TYPE_TELEPORT:      return m_auiEncounter[0];
             case TYPE_MARROWGAR:     return m_auiEncounter[1];
             case TYPE_DEATHWHISPER:  return m_auiEncounter[2];
             case TYPE_FLIGHT_WAR:    return m_auiEncounter[3];
             case TYPE_SAURFANG:      return m_auiEncounter[4];
             case TYPE_FESTERGUT:     return m_auiEncounter[5];
             case TYPE_ROTFACE:       return m_auiEncounter[6];
             case TYPE_PUTRICIDE:     return m_auiEncounter[7];
             case TYPE_BLOOD_COUNCIL: return m_auiEncounter[8];
             case TYPE_LANATHEL:      return m_auiEncounter[9];
             case TYPE_VALITHRIA:     return m_auiEncounter[10];
             case TYPE_SINDRAGOSA:    return m_auiEncounter[11];
             case TYPE_LICH_KING:     return m_auiEncounter[12];
             case TYPE_ICECROWN_QUESTS:  return m_auiEncounter[13];
             case TYPE_COUNT:         return m_auiEncounter[14];
             case DATA_BLOOD_COUNCIL_HEALTH:     return m_uiDataCouncilHealth;
             case TYPE_EVENT:         return m_auiEvent;
             case TYPE_EVENT_TIMER:   return m_auiEventTimer;
             case TYPE_EVENT_NPC:     switch (m_auiEvent)
                                         {
                                          case 12030:
                                          case 12050:
                                          case 12051:
                                          case 12052:
                                          case 12053:
                                          case 12070:
                                          case 12090:
                                          case 12110:
                                          case 12130:
                                          case 12150:
                                          case 12170:
                                          case 13110:
                                          case 13130:
                                          case 13131:
                                          case 13132:
                                          case 13150:
                                          case 13170:
                                          case 13190:
                                          case 13210:
                                          case 13230:
                                          case 13250:
                                          case 13270:
                                          case 13290:
                                          case 13310:
                                          case 13330:
                                          case 13350:
                                          case 13370:
                                          case 14010:
                                          case 14030:
                                          case 14050:
                                          case 14070:
                                                 return NPC_TIRION;
                                                 break;

                                          case 12000:
                                          case 12020:
                                          case 12040:
                                          case 12041:
                                          case 12042:
                                          case 12043:
                                          case 12060:
                                          case 12080:
                                          case 12100:
                                          case 12120:
                                          case 12200:
                                          case 13000:
                                          case 13020:
                                          case 13040:
                                          case 13060:
                                          case 13080:
                                          case 13100:
                                          case 13120:
                                          case 13140:
                                          case 13160:
                                          case 13180:
                                          case 13200:
                                          case 13220:
                                          case 13240:
                                          case 13260:
                                          case 13280:
                                          case 13300:
                                          case 14000:
                                                 return NPC_LICH_KING;
                                                 break;
                                          case 500:
                                          case 510:
                                          case 550:
                                          case 560:
                                          case 600:
                                          case 610:
                                          case 650:
                                          case 660:
                                                 return NPC_PROFESSOR_PUTRICIDE;
                                                 break;

                                          case 800:
                                                 return NPC_LANATHEL;
                                                 break;

                                          default:
                                                 break;
                                          };

        }
        return 0;
    }

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
            case NPC_LORD_MARROWGAR:          return m_uiMarrogwarGUID;
            case NPC_LADY_DEATHWHISPER:       return m_uiDeathWhisperGUID;
            case NPC_DEATHBRINGER_SAURFANG:   return m_uiSaurfangGUID;
            case NPC_FESTERGUT:               return m_uiFestergutGUID;
            case NPC_ROTFACE:                 return m_uiRotfaceGUID;
            case NPC_PROFESSOR_PUTRICIDE:     return m_uiPutricideGUID;
            case NPC_TALDARAM:                return m_uiTaldaramGUID;
            case NPC_VALANAR:                 return m_uiValanarGUID;
            case NPC_KELESETH:                return m_uiKelesethGUID;
            case NPC_LANATHEL:                return m_uiLanathelGUID;
            case NPC_VALITHRIA:               return m_uiValithriaGUID;
            case NPC_SINDRAGOSA:              return m_uiSindragosaGUID;
            case NPC_LICH_KING:               return m_uiLichKingGUID;
            case NPC_RIMEFANG:                return m_uiRimefangGUID;
            case NPC_SPINESTALKER:            return m_uiSpinestalkerGUID;
            case NPC_STINKY:                  return m_uiStinkyGUID;
            case NPC_PRECIOUS:                return m_uiPreciousGUID;
            case GO_SCIENTIST_DOOR_ORANGE:    return m_uiSDoorOrangeGUID;
            case GO_SCIENTIST_DOOR_GREEN:     return m_uiSDoorGreenGUID;
            case GO_SCIENTIST_DOOR_COLLISION: return m_uiSDoorCollisionGUID;
            case GO_BLOODWING_DOOR:           return m_uiBloodwingDoorGUID;
            case GO_VALITHRIA_DOOR_1:         return m_uiValithriaDoor1GUID;
            case GO_VALITHRIA_DOOR_2:         return m_uiValithriaDoor2GUID;
            case GO_VALITHRIA_DOOR_3:         return m_uiValithriaDoor3GUID;
            case GO_VALITHRIA_DOOR_4:         return m_uiValithriaDoor4GUID;
            case GO_ICESHARD_1:               return m_uiIceShard1GUID;
            case GO_ICESHARD_2:               return m_uiIceShard2GUID;
            case GO_ICESHARD_3:               return m_uiIceShard3GUID;
            case GO_ICESHARD_4:               return m_uiIceShard4GUID;
            case GO_FROSTY_WIND:              return m_uiFrostyWindGUID;
            case GO_FROSTY_EDGE:              return m_uiFrostyEdgeGUID;
            case NPC_FROSTMOURNE_TRIGGER:     return m_uiFrostmourneTriggerGUID;
            case NPC_FROSTMOURNE_HOLDER:      return m_uiFrostmourneHolderGUID;
        }
        return 0;
    }

    void Load(const char* chrIn)
    {
        if (!chrIn)
        {
            OUT_LOAD_INST_DATA_FAIL;
            return;
        }

        OUT_LOAD_INST_DATA(chrIn);

        std::istringstream loadStream(chrIn);

        for(uint8 i = 0; i < MAX_ENCOUNTERS; ++i)
        {
            loadStream >> m_auiEncounter[i];

            if (m_auiEncounter[i] == IN_PROGRESS && i >= 1)
                m_auiEncounter[i] = NOT_STARTED;
        }

        OUT_LOAD_INST_DATA_COMPLETE;
        OpenAllDoors();
    }
};

InstanceData* GetInstanceData_instance_icecrown_spire(Map* pMap)
{
    return new instance_icecrown_spire(pMap);
}


void AddSC_instance_icecrown_spire()
{
    Script* pNewScript;
    pNewScript = new Script;
    pNewScript->Name = "instance_icecrown_spire";
    pNewScript->GetInstanceData = &GetInstanceData_instance_icecrown_spire;
    pNewScript->RegisterSelf();
}
