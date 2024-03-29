# Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
## Process this file with automake to produce Makefile.in

## Sub-directories to parse
SUBDIRS = sql

##7z#BUILT_SOURCES = revision.h
##7z#CLEANFILES = revision.h
## CPP flags for includes, defines, etc.
AM_CPPFLAGS = $(MANGOS_INCLUDES) -I$(srcdir) -I$(srcdir)/../../../dep/include -I$(srcdir)/../../shared/ -I$(srcdir)/../../framework/ -I$(srcdir)/../../game/ -I$(srcdir)/include/ -I$(srcdir)/base/ -DSYSCONFDIR=\"$(sysconfdir)/\"

## Build MaNGOS script library as shared library.
# libmangosscript shared library will later be reused by world server daemon.
lib_LTLIBRARIES = libmangosscript.la
libmangosscript_la_SOURCES = \
ScriptMgr.cpp \
ScriptMgr.h \
config.h \
base/escort_ai.cpp \
base/escort_ai.h \
base/follower_ai.cpp \
base/follower_ai.h \
base/guard_ai.cpp \
base/guard_ai.h \
base/simple_ai.cpp \
base/simple_ai.h \
include/precompiled.cpp \
include/precompiled.h \
include/sc_creature.cpp \
include/sc_creature.h \
include/sc_gossip.h \
include/sc_grid_searchers.cpp \
include/sc_grid_searchers.h \
include/sc_instance.cpp \
include/sc_instance.h \
include/sc_boss_spell_worker.cpp \
include/sc_boss_spell_worker.h \
scripts/battlegrounds/battleground.cpp \
scripts/custom/npc_arena_honor.cpp \
scripts/custom/teleguy.cpp \
scripts/eastern_kingdoms/alterac_mountains.cpp \
scripts/eastern_kingdoms/arathi_highlands.cpp \
scripts/eastern_kingdoms/blasted_lands.cpp \
scripts/eastern_kingdoms/boss_kruul.cpp \
scripts/eastern_kingdoms/burning_steppes.cpp \
scripts/eastern_kingdoms/dun_morogh.cpp \
scripts/eastern_kingdoms/eastern_plaguelands.cpp \
scripts/eastern_kingdoms/elwynn_forest.cpp \
scripts/eastern_kingdoms/eversong_woods.cpp \
scripts/eastern_kingdoms/ghostlands.cpp \
scripts/eastern_kingdoms/hinterlands.cpp \
scripts/eastern_kingdoms/ironforge.cpp \
scripts/eastern_kingdoms/isle_of_queldanas.cpp \
scripts/eastern_kingdoms/loch_modan.cpp \
scripts/eastern_kingdoms/redridge_mountains.cpp \
scripts/eastern_kingdoms/searing_gorge.cpp \
scripts/eastern_kingdoms/silvermoon_city.cpp \
scripts/eastern_kingdoms/silverpine_forest.cpp \
scripts/eastern_kingdoms/stormwind_city.cpp \
scripts/eastern_kingdoms/stranglethorn_vale.cpp \
scripts/eastern_kingdoms/swamp_of_sorrows.cpp \
scripts/eastern_kingdoms/tirisfal_glades.cpp \
scripts/eastern_kingdoms/undercity.cpp \
scripts/eastern_kingdoms/western_plaguelands.cpp \
scripts/eastern_kingdoms/westfall.cpp \
scripts/eastern_kingdoms/wetlands.cpp \
scripts/eastern_kingdoms/blackrock_depths/blackrock_depths.cpp \
scripts/eastern_kingdoms/blackrock_depths/blackrock_depths.h \
scripts/eastern_kingdoms/blackrock_depths/boss_ambassador_flamelash.cpp \
scripts/eastern_kingdoms/blackrock_depths/boss_anubshiah.cpp \
scripts/eastern_kingdoms/blackrock_depths/boss_emperor_dagran_thaurissan.cpp \
scripts/eastern_kingdoms/blackrock_depths/boss_general_angerforge.cpp \
scripts/eastern_kingdoms/blackrock_depths/boss_gorosh_the_dervish.cpp \
scripts/eastern_kingdoms/blackrock_depths/boss_grizzle.cpp \
scripts/eastern_kingdoms/blackrock_depths/boss_high_interrogator_gerstahn.cpp \
scripts/eastern_kingdoms/blackrock_depths/boss_magmus.cpp \
scripts/eastern_kingdoms/blackrock_depths/boss_tomb_of_seven.cpp \
scripts/eastern_kingdoms/blackrock_depths/instance_blackrock_depths.cpp \
scripts/eastern_kingdoms/blackrock_spire/blackrock_spire.h \
scripts/eastern_kingdoms/blackrock_spire/boss_drakkisath.cpp \
scripts/eastern_kingdoms/blackrock_spire/boss_gyth.cpp \
scripts/eastern_kingdoms/blackrock_spire/boss_halycon.cpp \
scripts/eastern_kingdoms/blackrock_spire/boss_highlord_omokk.cpp \
scripts/eastern_kingdoms/blackrock_spire/boss_mother_smolderweb.cpp \
scripts/eastern_kingdoms/blackrock_spire/boss_overlord_wyrmthalak.cpp \
scripts/eastern_kingdoms/blackrock_spire/boss_pyroguard_emberseer.cpp \
scripts/eastern_kingdoms/blackrock_spire/boss_quartermaster_zigris.cpp \
scripts/eastern_kingdoms/blackrock_spire/boss_rend_blackhand.cpp \
scripts/eastern_kingdoms/blackrock_spire/boss_shadow_hunter_voshgajin.cpp \
scripts/eastern_kingdoms/blackrock_spire/boss_the_beast.cpp \
scripts/eastern_kingdoms/blackrock_spire/boss_warmaster_voone.cpp \
scripts/eastern_kingdoms/blackrock_spire/instance_blackrock_spire.cpp \
scripts/eastern_kingdoms/blackwing_lair/boss_broodlord_lashlayer.cpp \
scripts/eastern_kingdoms/blackwing_lair/boss_chromaggus.cpp \
scripts/eastern_kingdoms/blackwing_lair/boss_ebonroc.cpp \
scripts/eastern_kingdoms/blackwing_lair/boss_firemaw.cpp \
scripts/eastern_kingdoms/blackwing_lair/boss_flamegor.cpp \
scripts/eastern_kingdoms/blackwing_lair/boss_nefarian.cpp \
scripts/eastern_kingdoms/blackwing_lair/boss_razorgore.cpp \
scripts/eastern_kingdoms/blackwing_lair/boss_vaelastrasz.cpp \
scripts/eastern_kingdoms/blackwing_lair/boss_victor_nefarius.cpp \
scripts/eastern_kingdoms/blackwing_lair/instance_blackwing_lair.cpp \
scripts/eastern_kingdoms/deadmines/deadmines.cpp \
scripts/eastern_kingdoms/deadmines/deadmines.h \
scripts/eastern_kingdoms/deadmines/instance_deadmines.cpp \
scripts/eastern_kingdoms/karazhan/boss_curator.cpp \
scripts/eastern_kingdoms/karazhan/boss_maiden_of_virtue.cpp \
scripts/eastern_kingdoms/karazhan/boss_midnight.cpp \
scripts/eastern_kingdoms/karazhan/boss_moroes.cpp \
scripts/eastern_kingdoms/karazhan/boss_netherspite.cpp \
scripts/eastern_kingdoms/karazhan/boss_nightbane.cpp \
scripts/eastern_kingdoms/karazhan/boss_prince_malchezaar.cpp \
scripts/eastern_kingdoms/karazhan/boss_shade_of_aran.cpp \
scripts/eastern_kingdoms/karazhan/boss_terestian_illhoof.cpp \
scripts/eastern_kingdoms/karazhan/bosses_opera.cpp \
scripts/eastern_kingdoms/karazhan/instance_karazhan.cpp \
scripts/eastern_kingdoms/karazhan/karazhan.cpp \
scripts/eastern_kingdoms/karazhan/karazhan.h \
scripts/eastern_kingdoms/magisters_terrace/boss_felblood_kaelthas.cpp \
scripts/eastern_kingdoms/magisters_terrace/boss_priestess_delrissa.cpp \
scripts/eastern_kingdoms/magisters_terrace/boss_selin_fireheart.cpp \
scripts/eastern_kingdoms/magisters_terrace/boss_vexallus.cpp \
scripts/eastern_kingdoms/magisters_terrace/instance_magisters_terrace.cpp \
scripts/eastern_kingdoms/magisters_terrace/magisters_terrace.cpp \
scripts/eastern_kingdoms/magisters_terrace/magisters_terrace.h \
scripts/eastern_kingdoms/molten_core/boss_baron_geddon.cpp \
scripts/eastern_kingdoms/molten_core/boss_garr.cpp \
scripts/eastern_kingdoms/molten_core/boss_gehennas.cpp \
scripts/eastern_kingdoms/molten_core/boss_golemagg.cpp \
scripts/eastern_kingdoms/molten_core/boss_lucifron.cpp \
scripts/eastern_kingdoms/molten_core/boss_magmadar.cpp \
scripts/eastern_kingdoms/molten_core/boss_majordomo_executus.cpp \
scripts/eastern_kingdoms/molten_core/boss_ragnaros.cpp \
scripts/eastern_kingdoms/molten_core/boss_shazzrah.cpp \
scripts/eastern_kingdoms/molten_core/boss_sulfuron_harbinger.cpp \
scripts/eastern_kingdoms/molten_core/instance_molten_core.cpp \
scripts/eastern_kingdoms/molten_core/molten_core.cpp \
scripts/eastern_kingdoms/molten_core/molten_core.h \
scripts/eastern_kingdoms/scarlet_enclave/ebon_hold.cpp \
scripts/eastern_kingdoms/scarlet_monastery/boss_arcanist_doan.cpp \
scripts/eastern_kingdoms/scarlet_monastery/boss_azshir_the_sleepless.cpp \
scripts/eastern_kingdoms/scarlet_monastery/boss_bloodmage_thalnos.cpp \
scripts/eastern_kingdoms/scarlet_monastery/boss_herod.cpp \
scripts/eastern_kingdoms/scarlet_monastery/boss_high_inquisitor_fairbanks.cpp \
scripts/eastern_kingdoms/scarlet_monastery/boss_houndmaster_loksey.cpp \
scripts/eastern_kingdoms/scarlet_monastery/boss_interrogator_vishas.cpp \
scripts/eastern_kingdoms/scarlet_monastery/boss_mograine_and_whitemane.cpp \
scripts/eastern_kingdoms/scarlet_monastery/boss_scorn.cpp \
scripts/eastern_kingdoms/scarlet_monastery/boss_headless_horseman.cpp \
scripts/eastern_kingdoms/scarlet_monastery/instance_scarlet_monastery.cpp \
scripts/eastern_kingdoms/scarlet_monastery/scarlet_monastery.h \
scripts/eastern_kingdoms/scholomance/boss_darkmaster_gandling.cpp \
scripts/eastern_kingdoms/scholomance/boss_death_knight_darkreaver.cpp \
scripts/eastern_kingdoms/scholomance/boss_doctor_theolen_krastinov.cpp \
scripts/eastern_kingdoms/scholomance/boss_illucia_barov.cpp \
scripts/eastern_kingdoms/scholomance/boss_instructor_malicia.cpp \
scripts/eastern_kingdoms/scholomance/boss_jandice_barov.cpp \
scripts/eastern_kingdoms/scholomance/boss_kormok.cpp \
scripts/eastern_kingdoms/scholomance/boss_lord_alexei_barov.cpp \
scripts/eastern_kingdoms/scholomance/boss_lorekeeper_polkelt.cpp \
scripts/eastern_kingdoms/scholomance/boss_ras_frostwhisper.cpp \
scripts/eastern_kingdoms/scholomance/boss_the_ravenian.cpp \
scripts/eastern_kingdoms/scholomance/boss_vectus.cpp \
scripts/eastern_kingdoms/scholomance/instance_scholomance.cpp \
scripts/eastern_kingdoms/scholomance/scholomance.h \
scripts/eastern_kingdoms/shadowfang_keep/instance_shadowfang_keep.cpp \
scripts/eastern_kingdoms/shadowfang_keep/shadowfang_keep.cpp \
scripts/eastern_kingdoms/shadowfang_keep/shadowfang_keep.h \
scripts/eastern_kingdoms/stratholme/boss_baron_rivendare.cpp \
scripts/eastern_kingdoms/stratholme/boss_baroness_anastari.cpp \
scripts/eastern_kingdoms/stratholme/boss_cannon_master_willey.cpp \
scripts/eastern_kingdoms/stratholme/boss_dathrohan_balnazzar.cpp \
scripts/eastern_kingdoms/stratholme/boss_magistrate_barthilas.cpp \
scripts/eastern_kingdoms/stratholme/boss_maleki_the_pallid.cpp \
scripts/eastern_kingdoms/stratholme/boss_nerubenkan.cpp \
scripts/eastern_kingdoms/stratholme/boss_order_of_silver_hand.cpp \
scripts/eastern_kingdoms/stratholme/boss_postmaster_malown.cpp \
scripts/eastern_kingdoms/stratholme/boss_ramstein_the_gorger.cpp \
scripts/eastern_kingdoms/stratholme/boss_timmy_the_cruel.cpp \
scripts/eastern_kingdoms/stratholme/instance_stratholme.cpp \
scripts/eastern_kingdoms/stratholme/stratholme.cpp \
scripts/eastern_kingdoms/stratholme/stratholme.h \
scripts/eastern_kingdoms/sunken_temple/instance_sunken_temple.cpp \
scripts/eastern_kingdoms/sunken_temple/sunken_temple.h \
scripts/eastern_kingdoms/sunken_temple/sunken_temple.cpp \
scripts/eastern_kingdoms/sunwell_plateau/boss_brutallus.cpp \
scripts/eastern_kingdoms/sunwell_plateau/boss_kalecgos.cpp \
scripts/eastern_kingdoms/sunwell_plateau/instance_sunwell_plateau.cpp \
scripts/eastern_kingdoms/sunwell_plateau/sunwell_plateau.h \
scripts/eastern_kingdoms/uldaman/boss_ironaya.cpp \
scripts/eastern_kingdoms/uldaman/uldaman.cpp \
scripts/eastern_kingdoms/uldaman/uldaman.h \
scripts/eastern_kingdoms/uldaman/instance_uldaman.cpp \
scripts/eastern_kingdoms/zulaman/boss_akilzon.cpp \
scripts/eastern_kingdoms/zulaman/boss_halazzi.cpp \
scripts/eastern_kingdoms/zulaman/boss_janalai.cpp \
scripts/eastern_kingdoms/zulaman/boss_malacrass.cpp \
scripts/eastern_kingdoms/zulaman/boss_nalorakk.cpp \
scripts/eastern_kingdoms/zulaman/boss_zuljin.cpp \
scripts/eastern_kingdoms/zulaman/instance_zulaman.cpp \
scripts/eastern_kingdoms/zulaman/zulaman.cpp \
scripts/eastern_kingdoms/zulaman/zulaman.h \
scripts/eastern_kingdoms/zulgurub/boss_arlokk.cpp \
scripts/eastern_kingdoms/zulgurub/boss_gahzranka.cpp \
scripts/eastern_kingdoms/zulgurub/boss_grilek.cpp \
scripts/eastern_kingdoms/zulgurub/boss_hakkar.cpp \
scripts/eastern_kingdoms/zulgurub/boss_hazzarah.cpp \
scripts/eastern_kingdoms/zulgurub/boss_jeklik.cpp \
scripts/eastern_kingdoms/zulgurub/boss_jindo.cpp \
scripts/eastern_kingdoms/zulgurub/boss_mandokir.cpp \
scripts/eastern_kingdoms/zulgurub/boss_marli.cpp \
scripts/eastern_kingdoms/zulgurub/boss_renataki.cpp \
scripts/eastern_kingdoms/zulgurub/boss_thekal.cpp \
scripts/eastern_kingdoms/zulgurub/boss_venoxis.cpp \
scripts/eastern_kingdoms/zulgurub/boss_wushoolay.cpp \
scripts/eastern_kingdoms/zulgurub/instance_zulgurub.cpp \
scripts/eastern_kingdoms/zulgurub/zulgurub.h \
scripts/examples/example_creature.cpp \
scripts/examples/example_escort.cpp \
scripts/examples/example_gossip_codebox.cpp \
scripts/examples/example_misc.cpp \
scripts/kalimdor/ashenvale.cpp \
scripts/kalimdor/azshara.cpp \
scripts/kalimdor/azuremyst_isle.cpp \
scripts/kalimdor/bloodmyst_isle.cpp \
scripts/kalimdor/boss_azuregos.cpp \
scripts/kalimdor/darkshore.cpp \
scripts/kalimdor/desolace.cpp \
scripts/kalimdor/dustwallow_marsh.cpp \
scripts/kalimdor/felwood.cpp \
scripts/kalimdor/feralas.cpp \
scripts/kalimdor/moonglade.cpp \
scripts/kalimdor/mulgore.cpp \
scripts/kalimdor/orgrimmar.cpp \
scripts/kalimdor/silithus.cpp \
scripts/kalimdor/stonetalon_mountains.cpp \
scripts/kalimdor/tanaris.cpp \
scripts/kalimdor/teldrassil.cpp \
scripts/kalimdor/the_barrens.cpp \
scripts/kalimdor/thousand_needles.cpp \
scripts/kalimdor/thunder_bluff.cpp \
scripts/kalimdor/ungoro_crater.cpp \
scripts/kalimdor/winterspring.cpp \
scripts/kalimdor/blackfathom_deeps/blackfathom_deeps.h \
scripts/kalimdor/blackfathom_deeps/instance_blackfathom_deeps.cpp \
scripts/kalimdor/caverns_of_time/dark_portal/boss_aeonus.cpp \
scripts/kalimdor/caverns_of_time/dark_portal/boss_chrono_lord_deja.cpp \
scripts/kalimdor/caverns_of_time/dark_portal/boss_temporus.cpp \
scripts/kalimdor/caverns_of_time/dark_portal/dark_portal.cpp \
scripts/kalimdor/caverns_of_time/dark_portal/dark_portal.h \
scripts/kalimdor/caverns_of_time/dark_portal/instance_dark_portal.cpp \
scripts/kalimdor/caverns_of_time/hyjal/boss_archimonde.cpp \
scripts/kalimdor/caverns_of_time/hyjal/hyjal.cpp \
scripts/kalimdor/caverns_of_time/hyjal/hyjal.h \
scripts/kalimdor/caverns_of_time/hyjal/hyjalAI.cpp \
scripts/kalimdor/caverns_of_time/hyjal/hyjalAI.h \
scripts/kalimdor/caverns_of_time/hyjal/instance_hyjal.cpp \
scripts/kalimdor/caverns_of_time/old_hillsbrad/boss_captain_skarloc.cpp \
scripts/kalimdor/caverns_of_time/old_hillsbrad/boss_epoch_hunter.cpp \
scripts/kalimdor/caverns_of_time/old_hillsbrad/boss_leutenant_drake.cpp \
scripts/kalimdor/caverns_of_time/old_hillsbrad/instance_old_hillsbrad.cpp \
scripts/kalimdor/caverns_of_time/old_hillsbrad/old_hillsbrad.cpp \
scripts/kalimdor/caverns_of_time/old_hillsbrad/old_hillsbrad.h \
scripts/kalimdor/maraudon/boss_celebras_the_cursed.cpp \
scripts/kalimdor/maraudon/boss_landslide.cpp \
scripts/kalimdor/maraudon/boss_noxxion.cpp \
scripts/kalimdor/maraudon/boss_princess_theradras.cpp \
scripts/kalimdor/onyxias_lair/boss_onyxia.cpp \
scripts/kalimdor/razorfen_downs/boss_amnennar_the_coldbringer.cpp \
scripts/kalimdor/razorfen_downs/razorfen_downs.cpp \
scripts/kalimdor/razorfen_kraul/instance_razorfen_kraul.cpp \
scripts/kalimdor/razorfen_kraul/razorfen_kraul.h \
scripts/kalimdor/ruins_of_ahnqiraj/boss_ayamiss.cpp \
scripts/kalimdor/ruins_of_ahnqiraj/boss_buru.cpp \
scripts/kalimdor/ruins_of_ahnqiraj/boss_kurinnaxx.cpp \
scripts/kalimdor/ruins_of_ahnqiraj/boss_moam.cpp \
scripts/kalimdor/ruins_of_ahnqiraj/boss_ossirian.cpp \
scripts/kalimdor/ruins_of_ahnqiraj/boss_rajaxx.cpp \
scripts/kalimdor/ruins_of_ahnqiraj/instance_ruins_of_ahnqiraj.cpp \
scripts/kalimdor/ruins_of_ahnqiraj/ruins_of_ahnqiraj.cpp \
scripts/kalimdor/ruins_of_ahnqiraj/ruins_of_ahnqiraj.h \
scripts/kalimdor/temple_of_ahnqiraj/boss_bug_trio.cpp \
scripts/kalimdor/temple_of_ahnqiraj/boss_cthun.cpp \
scripts/kalimdor/temple_of_ahnqiraj/boss_fankriss.cpp \
scripts/kalimdor/temple_of_ahnqiraj/boss_huhuran.cpp \
scripts/kalimdor/temple_of_ahnqiraj/boss_ouro.cpp \
scripts/kalimdor/temple_of_ahnqiraj/boss_sartura.cpp \
scripts/kalimdor/temple_of_ahnqiraj/boss_skeram.cpp \
scripts/kalimdor/temple_of_ahnqiraj/boss_twinemperors.cpp \
scripts/kalimdor/temple_of_ahnqiraj/boss_viscidus.cpp \
scripts/kalimdor/temple_of_ahnqiraj/instance_temple_of_ahnqiraj.cpp \
scripts/kalimdor/temple_of_ahnqiraj/mob_anubisath_sentinel.cpp \
scripts/kalimdor/temple_of_ahnqiraj/temple_of_ahnqiraj.h \
scripts/kalimdor/wailing_caverns/instance_wailing_caverns.cpp \
scripts/kalimdor/wailing_caverns/wailing_caverns.h \
scripts/kalimdor/zulfarrak/zulfarrak.cpp \
scripts/northrend/borean_tundra.cpp \
scripts/northrend/dalaran.cpp \
scripts/northrend/dragonblight.cpp \
scripts/northrend/grizzly_hills.cpp \
scripts/northrend/icecrown.cpp \
scripts/northrend/sholazar_basin.cpp \
scripts/northrend/storm_peaks.cpp \
scripts/northrend/howling_fjord.cpp \
scripts/northrend/zuldrak.cpp \
scripts/northrend/azjol-nerub/ahnkahet/boss_jedoga.cpp \
scripts/northrend/azjol-nerub/ahnkahet/boss_nadox.cpp \
scripts/northrend/azjol-nerub/ahnkahet/boss_taldaram.cpp \
scripts/northrend/azjol-nerub/ahnkahet/boss_volazj.cpp \
scripts/northrend/azjol-nerub/ahnkahet/ahnkahet.h \
scripts/northrend/azjol-nerub/ahnkahet/instance_ahnkahet.cpp \
scripts/northrend/azjol-nerub/azjol-nerub/azjol-nerub.h \
scripts/northrend/azjol-nerub/azjol-nerub/boss_anubarak.cpp \
scripts/northrend/azjol-nerub/azjol-nerub/boss_hadronox.cpp \
scripts/northrend/azjol-nerub/azjol-nerub/boss_krikthir.cpp \
scripts/northrend/azjol-nerub/azjol-nerub/instance_azjol-nerub.cpp \
scripts/northrend/draktharon_keep/draktharon_keep.h \
scripts/northrend/draktharon_keep/boss_dred.cpp \
scripts/northrend/draktharon_keep/boss_novos.cpp \
scripts/northrend/draktharon_keep/boss_tharonja.cpp \
scripts/northrend/draktharon_keep/boss_trollgore.cpp \
scripts/northrend/gundrak/boss_colossus.cpp \
scripts/northrend/gundrak/boss_galdarah.cpp \
scripts/northrend/gundrak/boss_moorabi.cpp \
scripts/northrend/gundrak/boss_sladran.cpp \
scripts/northrend/gundrak/gundrak.h \
scripts/northrend/gundrak/instance_gundrak.cpp \
scripts/northrend/icecrown_citadel/frozen_halls/forge_of_souls/instance_forge_of_souls.cpp \
scripts/northrend/icecrown_citadel/frozen_halls/forge_of_souls/boss_devourer_of_souls.cpp \
scripts/northrend/icecrown_citadel/frozen_halls/forge_of_souls/boss_bronjahm.cpp \
scripts/northrend/icecrown_citadel/frozen_halls/pit_of_saron/instance_pit_of_saron.cpp \
scripts/northrend/icecrown_citadel/frozen_halls/pit_of_saron/boss_forgemaster_gafrost.cpp \
scripts/northrend/icecrown_citadel/frozen_halls/pit_of_saron/boss_scourgelord_tyrannus.cpp \
scripts/northrend/icecrown_citadel/frozen_halls/pit_of_saron/boss_krick_and_ick.cpp \
scripts/northrend/icecrown_citadel/frozen_halls/halls_of_reflection/instance_halls_of_reflection.cpp \
scripts/northrend/icecrown_citadel/frozen_halls/halls_of_reflection/boss_falryn.cpp \
scripts/northrend/icecrown_citadel/frozen_halls/halls_of_reflection/boss_lich_king.cpp \
scripts/northrend/icecrown_citadel/frozen_halls/halls_of_reflection/boss_marwyn.cpp \
scripts/northrend/icecrown_citadel/icecrown_citadel/icecrown_teleport.cpp \
scripts/northrend/icecrown_citadel/icecrown_citadel/instance_icecrown_spire.cpp \
scripts/northrend/icecrown_citadel/icecrown_citadel/boss_deathbringer_saurfang.cpp \
scripts/northrend/icecrown_citadel/icecrown_citadel/boss_lady_deathwhisper.cpp \
scripts/northrend/icecrown_citadel/icecrown_citadel/boss_lord_marrowgar.cpp \
scripts/northrend/icecrown_citadel/icecrown_citadel/blood_prince_council.cpp \
scripts/northrend/icecrown_citadel/icecrown_citadel/boss_blood_queen_lanathel.cpp \
scripts/northrend/icecrown_citadel/icecrown_citadel/boss_festergut.cpp \
scripts/northrend/icecrown_citadel/icecrown_citadel/boss_proffesor_putricide.cpp \
scripts/northrend/icecrown_citadel/icecrown_citadel/boss_rotface.cpp \
scripts/northrend/icecrown_citadel/icecrown_citadel/boss_sindragosa.cpp \
scripts/northrend/icecrown_citadel/icecrown_citadel/boss_the_lich_king.cpp \
scripts/northrend/icecrown_citadel/icecrown_citadel/boss_valithria_dreamwalker.cpp \
scripts/northrend/icecrown_citadel/icecrown_citadel/gunship_battle.cpp \
scripts/northrend/icecrown_citadel/icecrown_citadel/icecrown_spire.cpp \
scripts/northrend/icecrown_citadel/icecrown_citadel/def_spire.h \
scripts/northrend/ruby_sanctum/def_ruby_sanctum.h \
scripts/northrend/ruby_sanctum/instance_ruby_sanctum.cpp \
scripts/northrend/ruby_sanctum/boss_halion.cpp \
scripts/northrend/ruby_sanctum/boss_zarithian.cpp \
scripts/northrend/ruby_sanctum/boss_baltharus.cpp \
scripts/northrend/ruby_sanctum/boss_ragefire.cpp \
scripts/northrend/naxxramas/boss_anubrekhan.cpp \
scripts/northrend/naxxramas/boss_faerlina.cpp \
scripts/northrend/naxxramas/boss_four_horsemen.cpp \
scripts/northrend/naxxramas/boss_gluth.cpp \
scripts/northrend/naxxramas/boss_gothik.cpp \
scripts/northrend/naxxramas/boss_grobbulus.cpp \
scripts/northrend/naxxramas/boss_heigan.cpp \
scripts/northrend/naxxramas/boss_kelthuzad.cpp \
scripts/northrend/naxxramas/boss_loatheb.cpp \
scripts/northrend/naxxramas/boss_maexxna.cpp \
scripts/northrend/naxxramas/boss_noth.cpp \
scripts/northrend/naxxramas/boss_patchwerk.cpp \
scripts/northrend/naxxramas/boss_razuvious.cpp \
scripts/northrend/naxxramas/boss_sapphiron.cpp \
scripts/northrend/naxxramas/boss_thaddius.cpp \
scripts/northrend/naxxramas/naxxramas.h \
scripts/northrend/naxxramas/instance_naxxramas.cpp \
scripts/northrend/nexus/nexus/boss_anomalus.cpp \
scripts/northrend/nexus/nexus/boss_keristrasza.cpp \
scripts/northrend/nexus/nexus/boss_ormorok.cpp \
scripts/northrend/nexus/nexus/boss_telestra.cpp \
scripts/northrend/nexus/nexus/nexus.h \
scripts/northrend/nexus/nexus/instance_nexus.cpp \
scripts/northrend/obsidian_sanctum/boss_sartharion.cpp \
scripts/northrend/obsidian_sanctum/instance_obsidian_sanctum.cpp \
scripts/northrend/obsidian_sanctum/obsidian_sanctum.h \
scripts/northrend/ulduar/halls_of_lightning/boss_bjarngrim.cpp \
scripts/northrend/ulduar/halls_of_lightning/boss_ionar.cpp \
scripts/northrend/ulduar/halls_of_lightning/boss_loken.cpp \
scripts/northrend/ulduar/halls_of_lightning/boss_volkhan.cpp \
scripts/northrend/ulduar/halls_of_lightning/halls_of_lightning.h \
scripts/northrend/ulduar/halls_of_lightning/instance_halls_of_lightning.cpp \
scripts/northrend/ulduar/halls_of_stone/boss_maiden_of_grief.cpp \
scripts/northrend/ulduar/halls_of_stone/boss_sjonnir.cpp \
scripts/northrend/ulduar/halls_of_stone/boss_krystallus.cpp \
scripts/northrend/ulduar/halls_of_stone/halls_of_stone.cpp \
scripts/northrend/ulduar/halls_of_stone/def_halls_of_stone.h \
scripts/northrend/ulduar/halls_of_stone/instance_halls_of_stone.cpp \
scripts/northrend/ulduar/ulduar/boss_algalon.cpp \
scripts/northrend/ulduar/ulduar/boss_auriaya.cpp \
scripts/northrend/ulduar/ulduar/boss_freya.cpp \
scripts/northrend/ulduar/ulduar/boss_hodir.cpp \
scripts/northrend/ulduar/ulduar/boss_ignis.cpp \
scripts/northrend/ulduar/ulduar/boss_iron_council.cpp \
scripts/northrend/ulduar/ulduar/boss_kologarn.cpp \
scripts/northrend/ulduar/ulduar/boss_leviathan.cpp \
scripts/northrend/ulduar/ulduar/boss_mimiron.cpp \
scripts/northrend/ulduar/ulduar/boss_razorscale.cpp \
scripts/northrend/ulduar/ulduar/boss_thorim.cpp \
scripts/northrend/ulduar/ulduar/boss_vezax.cpp \
scripts/northrend/ulduar/ulduar/boss_xt_002.cpp \
scripts/northrend/ulduar/ulduar/boss_yogg_saron.cpp \
scripts/northrend/ulduar/ulduar/ulduar_teleport.cpp \
scripts/northrend/ulduar/ulduar/instance_ulduar.cpp \
scripts/northrend/ulduar/ulduar/ulduar.h \
scripts/northrend/utgarde_keep/utgarde_keep/boss_ingvar.cpp \
scripts/northrend/utgarde_keep/utgarde_keep/boss_keleseth.cpp \
scripts/northrend/utgarde_keep/utgarde_keep/boss_skarvald_and_dalronn.cpp \
scripts/northrend/utgarde_keep/utgarde_keep/instance_utgarde_keep.cpp \
scripts/northrend/utgarde_keep/utgarde_keep/utgarde_keep.cpp \
scripts/northrend/utgarde_keep/utgarde_keep/utgarde_keep.h \
scripts/northrend/utgarde_keep/utgarde_pinnacle/boss_gortok.cpp \
scripts/northrend/utgarde_keep/utgarde_pinnacle/boss_skadi.cpp \
scripts/northrend/utgarde_keep/utgarde_pinnacle/boss_svala.cpp \
scripts/northrend/utgarde_keep/utgarde_pinnacle/boss_ymiron.cpp \
scripts/northrend/utgarde_keep/utgarde_pinnacle/instance_utgarde_pinnacle.cpp \
scripts/northrend/utgarde_keep/utgarde_pinnacle/utgarde_pinnacle.h \
scripts/northrend/vault_of_archavon/boss_archavon.cpp \
scripts/northrend/vault_of_archavon/boss_emalon.cpp \
scripts/northrend/vault_of_archavon/boss_koralon.cpp \
scripts/northrend/vault_of_archavon/boss_toravon.cpp \
scripts/northrend/vault_of_archavon/instance_vault_of_archavon.cpp \
scripts/northrend/vault_of_archavon/vault_of_archavon.h \
scripts/outland/blades_edge_mountains.cpp \
scripts/outland/boss_doomlord_kazzak.cpp \
scripts/outland/boss_doomwalker.cpp \
scripts/outland/hellfire_peninsula.cpp \
scripts/outland/nagrand.cpp \
scripts/outland/netherstorm.cpp \
scripts/outland/shadowmoon_valley.cpp \
scripts/outland/shattrath_city.cpp \
scripts/outland/terokkar_forest.cpp \
scripts/outland/zangarmarsh.cpp \
scripts/outland/auchindoun/auchenai_crypts/boss_exarch_maladaar.cpp \
scripts/outland/auchindoun/mana_tombs/boss_nexusprince_shaffar.cpp \
scripts/outland/auchindoun/mana_tombs/boss_pandemonius.cpp \
scripts/outland/auchindoun/sethekk_halls/boss_darkweaver_syth.cpp \
scripts/outland/auchindoun/sethekk_halls/boss_tailonking_ikiss.cpp \
scripts/outland/auchindoun/sethekk_halls/instance_sethekk_halls.cpp \
scripts/outland/auchindoun/sethekk_halls/sethekk_halls.h \
scripts/outland/auchindoun/shadow_labyrinth/boss_ambassador_hellmaw.cpp \
scripts/outland/auchindoun/shadow_labyrinth/boss_blackheart_the_inciter.cpp \
scripts/outland/auchindoun/shadow_labyrinth/boss_grandmaster_vorpil.cpp \
scripts/outland/auchindoun/shadow_labyrinth/boss_murmur.cpp \
scripts/outland/auchindoun/shadow_labyrinth/instance_shadow_labyrinth.cpp \
scripts/outland/auchindoun/shadow_labyrinth/shadow_labyrinth.h \
scripts/outland/black_temple/black_temple.cpp \
scripts/outland/black_temple/black_temple.h \
scripts/outland/black_temple/boss_bloodboil.cpp \
scripts/outland/black_temple/boss_illidan.cpp \
scripts/outland/black_temple/boss_mother_shahraz.cpp \
scripts/outland/black_temple/boss_reliquary_of_souls.cpp \
scripts/outland/black_temple/boss_shade_of_akama.cpp \
scripts/outland/black_temple/boss_supremus.cpp \
scripts/outland/black_temple/boss_teron_gorefiend.cpp \
scripts/outland/black_temple/boss_warlord_najentus.cpp \
scripts/outland/black_temple/illidari_council.cpp \
scripts/outland/black_temple/instance_black_temple.cpp \
scripts/outland/coilfang_reservoir/serpent_shrine/boss_fathomlord_karathress.cpp \
scripts/outland/coilfang_reservoir/serpent_shrine/boss_hydross_the_unstable.cpp \
scripts/outland/coilfang_reservoir/serpent_shrine/boss_lady_vashj.cpp \
scripts/outland/coilfang_reservoir/serpent_shrine/boss_leotheras_the_blind.cpp \
scripts/outland/coilfang_reservoir/serpent_shrine/boss_morogrim_tidewalker.cpp \
scripts/outland/coilfang_reservoir/serpent_shrine/instance_serpent_shrine.cpp \
scripts/outland/coilfang_reservoir/serpent_shrine/serpent_shrine.h \
scripts/outland/coilfang_reservoir/steam_vault/boss_hydromancer_thespia.cpp \
scripts/outland/coilfang_reservoir/steam_vault/boss_mekgineer_steamrigger.cpp \
scripts/outland/coilfang_reservoir/steam_vault/boss_warlord_kalithresh.cpp \
scripts/outland/coilfang_reservoir/steam_vault/instance_steam_vault.cpp \
scripts/outland/coilfang_reservoir/steam_vault/steam_vault.h \
scripts/outland/coilfang_reservoir/underbog/boss_hungarfen.cpp \
scripts/outland/gruuls_lair/boss_gruul.cpp \
scripts/outland/gruuls_lair/boss_high_king_maulgar.cpp \
scripts/outland/gruuls_lair/gruuls_lair.h \
scripts/outland/gruuls_lair/instance_gruuls_lair.cpp \
scripts/outland/hellfire_citadel/blood_furnace/blood_furnace.h \
scripts/outland/hellfire_citadel/blood_furnace/boss_broggok.cpp \
scripts/outland/hellfire_citadel/blood_furnace/boss_kelidan_the_breaker.cpp \
scripts/outland/hellfire_citadel/blood_furnace/boss_the_maker.cpp \
scripts/outland/hellfire_citadel/blood_furnace/instance_blood_furnace.cpp \
scripts/outland/hellfire_citadel/hellfire_ramparts/boss_nazan_and_vazruden.cpp \
scripts/outland/hellfire_citadel/hellfire_ramparts/boss_omor_the_unscarred.cpp \
scripts/outland/hellfire_citadel/hellfire_ramparts/boss_watchkeeper_gargolmar.cpp \
scripts/outland/hellfire_citadel/hellfire_ramparts/hellfire_ramparts.h \
scripts/outland/hellfire_citadel/hellfire_ramparts/instance_hellfire_ramparts.cpp \
scripts/outland/hellfire_citadel/magtheridons_lair/boss_magtheridon.cpp \
scripts/outland/hellfire_citadel/magtheridons_lair/instance_magtheridons_lair.cpp \
scripts/outland/hellfire_citadel/magtheridons_lair/magtheridons_lair.h \
scripts/outland/hellfire_citadel/shattered_halls/boss_nethekurse.cpp \
scripts/outland/hellfire_citadel/shattered_halls/boss_warbringer_omrogg.cpp \
scripts/outland/hellfire_citadel/shattered_halls/boss_warchief_kargath_bladefist.cpp \
scripts/outland/hellfire_citadel/shattered_halls/instance_shattered_halls.cpp \
scripts/outland/hellfire_citadel/shattered_halls/shattered_halls.h \
scripts/outland/tempest_keep/arcatraz/arcatraz.cpp \
scripts/outland/tempest_keep/arcatraz/arcatraz.h \
scripts/outland/tempest_keep/arcatraz/boss_harbinger_skyriss.cpp \
scripts/outland/tempest_keep/arcatraz/instance_arcatraz.cpp \
scripts/outland/tempest_keep/botanica/boss_high_botanist_freywinn.cpp \
scripts/outland/tempest_keep/botanica/boss_laj.cpp \
scripts/outland/tempest_keep/botanica/boss_warp_splinter.cpp \
scripts/outland/tempest_keep/the_eye/boss_astromancer.cpp \
scripts/outland/tempest_keep/the_eye/boss_kaelthas.cpp \
scripts/outland/tempest_keep/the_eye/boss_void_reaver.cpp \
scripts/outland/tempest_keep/the_eye/instance_the_eye.cpp \
scripts/outland/tempest_keep/the_eye/the_eye.cpp \
scripts/outland/tempest_keep/the_eye/the_eye.h \
scripts/outland/tempest_keep/the_mechanar/boss_gatewatcher_gyrokill.cpp \
scripts/outland/tempest_keep/the_mechanar/boss_gatewatcher_ironhand.cpp \
scripts/outland/tempest_keep/the_mechanar/boss_nethermancer_sepethrea.cpp \
scripts/outland/tempest_keep/the_mechanar/boss_pathaleon_the_calculator.cpp \
scripts/outland/tempest_keep/the_mechanar/instance_mechanar.cpp \
scripts/outland/tempest_keep/the_mechanar/mechanar.h \
scripts/northrend/crusaders_coliseum/trial_of_the_champion/boss_argent_challenge.cpp \
scripts/northrend/crusaders_coliseum/trial_of_the_champion/boss_black_knight.cpp \
scripts/northrend/crusaders_coliseum/trial_of_the_champion/boss_grand_champions.cpp \
scripts/northrend/crusaders_coliseum/trial_of_the_champion/instance_trial_of_the_champion.cpp \
scripts/northrend/crusaders_coliseum/trial_of_the_champion/trial_of_the_champion.cpp \
scripts/northrend/crusaders_coliseum/trial_of_the_champion/trial_of_the_champion.h \
scripts/northrend/crusaders_coliseum/trial_of_the_crusader/boss_anubarak_trial.cpp \
scripts/northrend/crusaders_coliseum/trial_of_the_crusader/boss_faction_champions.cpp \
scripts/northrend/crusaders_coliseum/trial_of_the_crusader/boss_jaraxxus.cpp \
scripts/northrend/crusaders_coliseum/trial_of_the_crusader/boss_northrend_beasts.cpp \
scripts/northrend/crusaders_coliseum/trial_of_the_crusader/boss_twin_valkyr.cpp \
scripts/northrend/crusaders_coliseum/trial_of_the_crusader/instance_trial_of_the_crusader.cpp \
scripts/northrend/crusaders_coliseum/trial_of_the_crusader/trial_of_the_crusader.cpp \
scripts/northrend/crusaders_coliseum/trial_of_the_crusader/trial_of_the_crusader.h \
scripts/northrend/violet_hold/def_violet_hold.h \
scripts/northrend/violet_hold/violet_hold.cpp \
scripts/northrend/violet_hold/boss_cyanigosa.cpp \
scripts/northrend/violet_hold/boss_moragg.cpp \
scripts/northrend/violet_hold/instance_violet_hold.cpp \
scripts/northrend/violet_hold/boss_erekem.cpp \
scripts/northrend/violet_hold/boss_xevozz.cpp \
scripts/northrend/violet_hold/boss_ichoron.cpp \
scripts/northrend/violet_hold/boss_zuramat.cpp \
scripts/northrend/violet_hold/boss_lavanthor.cpp \
scripts/kalimdor/caverns_of_time/culling_of_stratholme/def_culling_of_stratholme.h \
scripts/kalimdor/caverns_of_time/culling_of_stratholme/boss_lord_epoch.cpp \
scripts/kalimdor/caverns_of_time/culling_of_stratholme/culling_of_stratholme.cpp \
scripts/kalimdor/caverns_of_time/culling_of_stratholme/culling_of_stratholmeai.cpp \
scripts/kalimdor/caverns_of_time/culling_of_stratholme/boss_malganis.cpp \
scripts/kalimdor/caverns_of_time/culling_of_stratholme/boss_meathook.cpp \
scripts/kalimdor/caverns_of_time/culling_of_stratholme/instance_culling_of_stratholme.cpp \
scripts/kalimdor/caverns_of_time/culling_of_stratholme/boss_salramm.cpp \
scripts/kalimdor/caverns_of_time/culling_of_stratholme/boss_infinite_corruptor.cpp \
scripts/world/areatrigger_scripts.cpp \
scripts/world/boss_emeriss.cpp \
scripts/world/boss_lethon.cpp \
scripts/world/boss_taerar.cpp \
scripts/world/boss_ysondre.cpp \
scripts/world/go_scripts.cpp \
scripts/world/guards.cpp \
scripts/world/item_scripts.cpp \
scripts/world/mob_generic_creature.cpp \
scripts/world/npc_professions.cpp \
scripts/world/npcs_special.cpp \
scripts/world/spell_scripts.cpp \
system/ScriptLoader.cpp \
system/ScriptLoader.h \
system/system.cpp \
system/system.h


## magic to include revision data in SD2 version string
##7z#revision.h: FORCE
##7z#	$(top_builddir)/src/tools/genrevision/genrevision $(srcdir)

FORCE:

## libtool settings
# API versioning
# Link against dependencies
# How to increase version info:
# - only bug fixes implemented:
# bump the version to LTMANGOS_CURRENT:LTMANGOS_REVISION+1:LTMANGOS_AGE
# - augmented the interface:
# bump the version to LTMANGOS_CURRENT+1:0:LTMANGOS_AGE+1
# - broken old interface:
# bump the version to LTMANGOS_CURRENT+1:0:0
LTMANGOS_CURRENT = 0
LTMANGOS_REVISION = 0
LTMANGOS_AGE = 0
libmangosscript_la_LIBFLAGS = -version-info $(LTMANGOS_CURRENT):$(LTMANGOS_REVISION):$(LTMANGOS_AGE)

## Additional files to include when running 'make dist'
# Scripts defaults.
EXTRA_DIST = \
	Scripts/sc_default.cpp \
	Scripts/sc_defines.cpp \
	Scripts/sc_defines.h \
	scriptdev2.conf.dist

## Additional files to install
sysconf_DATA = \
	scriptdev2.conf.dist

install-data-hook:
	@list='$(sysconf_DATA)'; for p in $$list; do \
      dest=`echo $$p | sed -e s/.dist//`; \
	  if test -f $(DESTDIR)$(sysconfdir)/$$dest; then \
        echo "$@ will not overwrite existing $(DESTDIR)$(sysconfdir)/$$dest"; \
      else \
	    echo " $(INSTALL_DATA) $$p $(DESTDIR)$(sysconfdir)/$$dest"; \
	    $(INSTALL_DATA) $$p $(DESTDIR)$(sysconfdir)/$$dest; \
	  fi; \
	done

clean-local:
	rm -f $(sysconf_DATA)
