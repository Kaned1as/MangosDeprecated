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

/* ScriptData
SDName: boss_sindragosa
SD%Complete: 60%
SDComment: by /dev/rsa
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "def_spire.h"

enum BossSpells
{
// Sindragosa
    SPELL_FROST_AURA_1       = 70084,
    SPELL_CLEAVE_1           = 19983,
    SPELL_TAIL_SMASH         = 71077,
    SPELL_FROST_BREATH_1     = 69649,
    SPELL_PERMEATING_CHILL   = 70107,
    SPELL_UNCHAINED_MAGIC    = 69762,
    SPELL_INSTABILITY        = 69766,
    SPELL_ICY_GRIP           = 70117,
    SPELL_BLISTERING_COLD    = 70123,

    SPELL_FROST_BEACON       = 70126,
    SPELL_ICY_TOMB           = 70157,
    SPELL_ASPHYXATION        = 71665,
    SPELL_FROST_BOMB         = 71053,
    SPELL_FROST_BOMB_TRIGGER = 69846,
    SPELL_FROST_BOMB_VISUAL  = 64624,
    SPELL_FROST_BOMB_VISUAL2 = 69016,
    SPELL_ICE_TOMB_TRIGGER   = 69675,
    SPELL_MYSTIC_BUFFET      = 70128,

    NPC_ICE_TOMB             = 36980,
    NPC_FROST_BOMB           = 37186,

    SPELL_BERSERK            = 47008,

// Rimefang
    SPELL_FROST_AURA         = 71387,
    SPELL_FROST_BREATH       = 71386,
    SPELL_ICY_BLAST          = 71376,
// Spinestalker
    SPELL_BELLOWING_ROAR     = 36922,
    SPELL_CLEAVE             = 40505,
    SPELL_TAIL_SWEEP         = 71369,
};

static Locations SpawnLoc[]=
{
    {4408.052734f, 2484.825439f, 203.374207f},  // 0 Sindragosa spawn
    {4474.239746f, 2484.243896f, 231.0f},       // 1 Sindragosa fly o=3.11
    {4474.239746f, 2484.243896f, 203.380402f},  // 2 Sindragosa fly - ground point o=3.11
};

struct MANGOS_DLL_DECL boss_sindragosaAI : public ScriptedAI
{
    boss_sindragosaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        bsw = new BossSpellWorker(this);
        Reset();
    }

    ScriptedInstance *pInstance;
    BossSpellWorker* bsw;
    uint8 stage;
    Map* pMap;
    uint8 Difficulty;
    uint8 icecount;
    bool MovementStarted;
    Unit* marked[5];

    void Reset()
    {
        if(!pInstance) return;
        bsw->resetTimers();
        stage = 0;
        memset(marked, 0, sizeof(Unit*)*5);

        m_creature->SetRespawnDelay(7*DAY);
        pMap = m_creature->GetMap();
        Difficulty = pMap->GetDifficulty();
        switch (Difficulty) {
                             case RAID_DIFFICULTY_10MAN_NORMAL:
                                       icecount = 2;
                                       break;
                             case RAID_DIFFICULTY_10MAN_HEROIC:
                                       icecount = 2;
                                       break;
                             case RAID_DIFFICULTY_25MAN_NORMAL:
                                       icecount = 5;
                                       break;
                             case RAID_DIFFICULTY_25MAN_HEROIC:
                                       icecount = 5;
                                       break;
                             default:
                                       icecount = 2;
                                       break;
                            }
    }

    void KilledUnit(Unit* pVictim)
    {
    switch (urand(0,1)) {
        case 0:
               DoScriptText(-1631421,m_creature,pVictim);
               break;
        case 1:
               DoScriptText(-1631422,m_creature,pVictim);
               break;
        }
    }

    void JustReachedHome()
    {
        if (!pInstance) return;
        pInstance->SetData(TYPE_SINDRAGOSA, FAIL);
        if (Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_RIMEFANG)))
            pTemp->Respawn();
        if (Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_SPINESTALKER)))
            pTemp->Respawn();
        DoScriptText(-1631422,m_creature);
        m_creature->ForcedDespawn();
    }

    void Aggro(Unit *who) 
    {
        if(!pInstance) return;
           DoScriptText(-1631420,m_creature,who);
        bsw->doCast(SPELL_FROST_AURA_1);
    }

    void JustDied(Unit *killer)
    {
        if(!pInstance) return;
        pInstance->SetData(TYPE_SINDRAGOSA, DONE);
        DoScriptText(-1631423,m_creature,killer);
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if (!pInstance) return;
        if (type != POINT_MOTION_TYPE || !MovementStarted) return;

        if (id == 1) {
                m_creature->GetMotionMaster()->MovementExpired();
                MovementStarted = false;
                }
    }

    void doBlisteringCold()
    {
        bsw->doCast(SPELL_ICY_GRIP);

        Map::PlayerList const &pList = pMap->GetPlayers();
                 if (pList.isEmpty()) return;

        for (Map::PlayerList::const_iterator i = pList.begin(); i != pList.end(); ++i)
              if (Player* player = i->getSource())
                 if (player->isAlive() && player->IsWithinDistInMap(m_creature, 30.0f))
                        {
                        float fPosX, fPosY, fPosZ;
                        m_creature->GetPosition(fPosX, fPosY, fPosZ);
                        m_creature->GetRandomPoint(fPosX, fPosY, fPosZ, 1.0f, fPosX, fPosY, fPosZ);
                        player->NearTeleportTo(fPosX, fPosY, fPosZ+1.0f, (float)(urand(0,6)), true);
                        }
        bsw->doCast(SPELL_BLISTERING_COLD);
    }

    void IceMark()
    {
        memset(marked, 0, sizeof(Unit*)*5);

        for (uint8 i = 0; i < icecount; i++)
            if (marked[i] = bsw->SelectRandomPlayer(SPELL_FROST_BEACON, false, 200.0f))
                bsw->doCast(SPELL_FROST_BEACON, marked[i]);
    }

    void IceBlock()
    {
        for (uint8 i = 0; i < icecount; i++)
            if (marked[i] && marked[i]->isAlive())
            {
                bsw->doCast(SPELL_ICY_TOMB, marked[i]);
                marked[i]->RemoveAurasDueToSpell(SPELL_FROST_BEACON);
                    float fPosX, fPosY, fPosZ;
                marked[i]->GetPosition(fPosX, fPosY, fPosZ);
                Unit* pTemp1 = bsw->doSummon(NPC_ICE_TOMB,fPosX, fPosY, fPosZ);
                if (pTemp1 && pTemp1->IsInWorld())
                    pTemp1->AddThreat(marked[i], 100.0f);
            };

/*        Map::PlayerList const &pList = pMap->GetPlayers();
                 if (pList.isEmpty()) return;

        for(Map::PlayerList::const_iterator i = pList.begin(); i != pList.end(); ++i)
          {
              if (Player* player = i->getSource())
                 {
                  if ( player->isAlive() && (player->HasAura(SPELL_ICY_TOMB)) )
                     {
                        float fPosX, fPosY, fPosZ;
                        player->GetPosition(fPosX, fPosY, fPosZ);
                        if (Unit* pTemp = bsw->doSummon(NPC_ICE_TOMB,fPosX, fPosY, fPosZ))
                            pTemp->AddThreat(player, 100.0f);
                     };
                 };
           };*/
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch(stage)
        {
            case 0: 
                    bsw->timedCast(SPELL_CLEAVE_1, diff);
                    bsw->timedCast(SPELL_TAIL_SMASH, diff);
                    bsw->timedCast(SPELL_FROST_BREATH_1, diff);
                    bsw->timedCast(SPELL_PERMEATING_CHILL, diff);

                    bsw->timedCast(SPELL_UNCHAINED_MAGIC, diff);

                    if (bsw->timedQuery(SPELL_ICY_GRIP, diff))
                         {
                         DoScriptText(-1631426,m_creature);
                         doBlisteringCold();
                         }

                    if (bsw->timedQuery(SPELL_FROST_BEACON, diff) && m_creature->GetHealthPercent() < 85.0f) stage = 1;

                    if (m_creature->GetHealthPercent() < 35.0f) 
                       {
                            bsw->doCast(SPELL_MYSTIC_BUFFET);
                            stage = 9;
                            DoScriptText(-1631429,m_creature);
                       }
            break;
            case 1: 
                    DoScriptText(-1631425,m_creature);
                    IceMark();
                    stage = 2;
                    MovementStarted = true;
                    SetCombatMovement(false);
                    m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
                    m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
                    m_creature->GetMotionMaster()->MovePoint(1, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                    m_creature->HandleEmoteCommand(EMOTE_ONESHOT_FLY_SIT_GROUND_UP);
                    m_creature->AddSplineFlag(SPLINEFLAG_FLYING);
            break;
            case 2: 
                    if (!MovementStarted) {
                           stage = 3;
                           m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_FLY_FALL);
                          };
            break;
            case 3: 
                          stage = 4;
                          IceBlock();
                          m_creature->SetOrientation(3.1f);
            break;
            case 4: 
                    if (bsw->timedQuery(SPELL_FROST_BOMB, diff))
                           if (Unit* pTemp = bsw->SelectRandomPlayerAtRange(300.0f))
                                bsw->doCast(SPELL_FROST_BOMB_TRIGGER, pTemp);

                    bsw->timedCast(SPELL_FROST_BREATH_1, diff);

                    if (bsw->timedQuery(SPELL_FROST_BEACON, diff)) {
                           stage = 5;
                           }
            break;
            case 5: 
                    MovementStarted = true;
                    SetCombatMovement(false);
                    m_creature->GetMotionMaster()->MovePoint(1, SpawnLoc[0].x, SpawnLoc[0].y, SpawnLoc[0].z);
                    stage = 6;
                    m_creature->HandleEmoteCommand(EMOTE_STATE_FLY_SIT_GROUND);
            break;
            case 6: 
                    if (!MovementStarted) {
                           stage = 0;
                           SetCombatMovement(true);
                           m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                           m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 0);
                           m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
                           m_creature->RemoveSplineFlag(SPLINEFLAG_FLYING);
                          };
            break;

            case 9: 
                    bsw->timedCast(SPELL_CLEAVE_1, diff);
                    bsw->timedCast(SPELL_TAIL_SMASH, diff);
                    bsw->timedCast(SPELL_FROST_BREATH_1, diff);
                    bsw->timedCast(SPELL_PERMEATING_CHILL, diff);
                    bsw->timedCast(SPELL_UNCHAINED_MAGIC, diff);

                    if (bsw->timedQuery(SPELL_ICY_GRIP, diff))
                         {
                         DoScriptText(-1631426,m_creature);
                         doBlisteringCold();
                         }

            break;
            default: break;
        }

         if (bsw->timedQuery(SPELL_BERSERK, diff))
                {
                bsw->doCast(SPELL_BERSERK);
                DoScriptText(-1631424,m_creature);
                };

        DoMeleeAttackIfReady();
    }
};


CreatureAI* GetAI_boss_sindragosa(Creature* pCreature)
{
    return new boss_sindragosaAI(pCreature);
}

struct MANGOS_DLL_DECL mob_ice_tombAI : public ScriptedAI
{
    mob_ice_tombAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        bsw = new BossSpellWorker(this);
        Reset();
    }

    ScriptedInstance *m_pInstance;
    BossSpellWorker* bsw;
    Unit* pVictim;

    void Reset()
    {
        SetCombatMovement(false);
        pVictim = NULL;
    }

    void Aggro(Unit* pWho)
    {
        if (!pVictim && pWho)  {
                        pVictim = pWho;
                        m_creature->SetInCombatWith(pVictim);
                        }
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (uiDamage > m_creature->GetHealth())
            if (pVictim) bsw->doRemove(SPELL_ICY_TOMB,pVictim);
    }

    void KilledUnit(Unit* _Victim)
    {
        if (pVictim) bsw->doRemove(SPELL_ICY_TOMB,pVictim);
    }

    void JustDied(Unit* Killer)
    {
        if (pVictim) bsw->doRemove(SPELL_ICY_TOMB,pVictim);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_pInstance && m_pInstance->GetData(TYPE_SINDRAGOSA) != IN_PROGRESS)
        {
        if (pVictim) bsw->doRemove(SPELL_ICY_TOMB,pVictim);
            m_creature->ForcedDespawn();
        }

    }

};

CreatureAI* GetAI_mob_ice_tomb(Creature* pCreature)
{
    return new mob_ice_tombAI(pCreature);
}

struct MANGOS_DLL_DECL mob_frost_bombAI : public ScriptedAI
{
    mob_frost_bombAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance *m_pInstance;
    uint32 boom_timer;
    uint32 visual_timer;
    bool   finita;

    void Reset()
    {
        SetCombatMovement(false);
        visual_timer = 6000;
        boom_timer = 9000;
        finita = false;
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
//        m_creature->SetDisplayId(15880);
        m_creature->SetDisplayId(22523);
//        m_creature->SetDisplayId(19075);
    }

    void AttackStart(Unit *pWho)
    {
        return;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_pInstance && m_pInstance->GetData(TYPE_SINDRAGOSA) != IN_PROGRESS)
            m_creature->ForcedDespawn();

        if (finita)
        {
            m_creature->CastSpell(m_creature, SPELL_FROST_BOMB, true);
            m_creature->ForcedDespawn();
        }

        if (visual_timer <= uiDiff)
        {
            m_creature->CastSpell(m_creature, SPELL_FROST_BOMB_VISUAL, true);
            visual_timer= DAY;
        }
        else visual_timer -= uiDiff;

        if (boom_timer <= uiDiff)
        {
            m_creature->CastSpell(m_creature,SPELL_FROST_BOMB_VISUAL2,false);
            finita = true;
        }
        else boom_timer -= uiDiff;
    }

};

CreatureAI* GetAI_mob_frost_bomb(Creature* pCreature)
{
    return new mob_frost_bombAI(pCreature);
}

struct MANGOS_DLL_DECL mob_rimefangAI : public ScriptedAI
{
    mob_rimefangAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        bsw = new BossSpellWorker(this);
        Reset();
    }

    ScriptedInstance *pInstance;
    BossSpellWorker* bsw;
    Creature* pBrother;

    void Reset()
    {
        if(!pInstance) return;
        pBrother = NULL;
        pInstance->SetData(TYPE_SINDRAGOSA, NOT_STARTED);
        bsw->resetTimers();
        m_creature->SetRespawnDelay(DAY);
    }

    void JustReachedHome()
    {
        if (pInstance) pInstance->SetData(TYPE_SINDRAGOSA, FAIL);
    }

    void Aggro(Unit *who) 
    {
        if(!pInstance) return;
        pInstance->SetData(TYPE_SINDRAGOSA, IN_PROGRESS);
        pBrother = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_SPINESTALKER));
        if (pBrother && !pBrother->isAlive()) pBrother->Respawn();
        if (pBrother) pBrother->SetInCombatWithZone();
        bsw->doCast(SPELL_FROST_AURA);
    }

    void JustDied(Unit *killer)
    {
        if(!pInstance) return;
        if (pBrother && !pBrother->isAlive())
                 m_creature->SummonCreature(NPC_SINDRAGOSA, SpawnLoc[0].x, SpawnLoc[0].y, SpawnLoc[0].z, 3.17f, TEMPSUMMON_MANUAL_DESPAWN, DESPAWN_TIME);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        bsw->timedCast(SPELL_FROST_BREATH, diff);

        bsw->timedCast(SPELL_ICY_BLAST, diff);

        DoMeleeAttackIfReady();
    }
};


CreatureAI* GetAI_mob_rimefang(Creature* pCreature)
{
    return new mob_rimefangAI(pCreature);
}

struct MANGOS_DLL_DECL mob_spinestalkerAI : public ScriptedAI
{
    mob_spinestalkerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        bsw = new BossSpellWorker(this);
        Reset();
    }

    ScriptedInstance *pInstance;
    BossSpellWorker* bsw;
    Creature* pBrother;

    void Reset()
    {
        if(!pInstance) return;
        pBrother = NULL;
        pInstance->SetData(TYPE_SINDRAGOSA, NOT_STARTED);
        bsw->resetTimers();
        m_creature->SetRespawnDelay(DAY);
    }

    void JustReachedHome()
    {
        if (pInstance) pInstance->SetData(TYPE_SINDRAGOSA, FAIL);
    }

    void Aggro(Unit *who) 
    {
        if(!pInstance) return;
        pInstance->SetData(TYPE_SINDRAGOSA, IN_PROGRESS);
        pBrother = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_RIMEFANG));
        if (pBrother && !pBrother->isAlive()) pBrother->Respawn();
        if (pBrother) pBrother->SetInCombatWithZone();
    }

    void JustDied(Unit *killer)
    {
        if(!pInstance) return;
        if (pBrother && !pBrother->isAlive())
                 m_creature->SummonCreature(NPC_SINDRAGOSA, SpawnLoc[0].x, SpawnLoc[0].y, SpawnLoc[0].z, 3.17f, TEMPSUMMON_MANUAL_DESPAWN, DESPAWN_TIME);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        bsw->timedCast(SPELL_BELLOWING_ROAR, diff);

        bsw->timedCast(SPELL_CLEAVE, diff);

        bsw->timedCast(SPELL_TAIL_SWEEP, diff);

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_spinestalker(Creature* pCreature)
{
    return new mob_spinestalkerAI(pCreature);
}

void AddSC_boss_sindragosa()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_sindragosa";
    newscript->GetAI = &GetAI_boss_sindragosa;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_rimefang";
    newscript->GetAI = &GetAI_mob_rimefang;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_spinestalker";
    newscript->GetAI = &GetAI_mob_spinestalker;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_ice_tomb";
    newscript->GetAI = &GetAI_mob_ice_tomb;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_frost_bomb";
    newscript->GetAI = &GetAI_mob_frost_bomb;
    newscript->RegisterSelf();

}
