ALTER TABLE db_version CHANGE COLUMN required_10044_01_mangos_spell_chain required_10044_02_mangos_spell_proc_event bit;

DELETE FROM  `spell_proc_event` WHERE `entry` IN (
 11120, 12487, 12488, 12598, 12668, 12724, 12725,
 12726, 12727, 12799, 12812, 12813, 12814, 12815,
 12846, 12847, 12848, 12849, 12867, 12872, 12873,
 12958, 12971, 12972, 12973, 12974, 13045, 13046,
 13047, 13048, 13867, 14070, 14071, 14160, 14161,
 14190, 14193, 14194, 14195, 14774, 15338, 15362,
 15363, 16196, 16198, 16235, 16240, 16281, 16282,
 16283, 16284, 16489, 16492, 16923, 16924, 17107,
 17108, 17796, 17801, 17802, 17803, 18073, 18095,
 18120, 19573, 20056, 20057, 20182, 20212, 20213,
 20214, 20215, 20235, 20501, 23695, 25988, 27815,
 27816, 28592, 28593, 29075, 29076, 29179, 29180,
 29444, 29594, 29838, 30295, 30296, 30301, 30302,
 30678, 30679, 31126, 31570, 31835, 31836, 31872,
 31877, 31878, 33145, 33146, 33154, 33192, 33193,
 33882, 33883, 34498, 34499, 34502, 34503, 34859,
 34860, 34938, 34939, 34954, 44443, 44446, 44448,
 44469, 44470, 44471, 44472, 44548, 44549, 45243,
 45244, 46855, 46952, 46953, 47196, 47197, 47202,
 47203, 47204, 47205, 47246, 47247, 47259, 47260,
 47511, 47515, 47517, 47581, 47582, 48484, 48485,
 48499, 48500, 48510, 48511, 48521, 48525, 49503,
 49504, 49529, 49530, 51127, 51128, 51129, 51130,
 51470, 51478, 51479, 51485, 51486, 51557, 51558,
 51563, 51564, 51565, 51566, 51626, 51628, 51629,
 51635, 51636, 51665, 51667, 51668, 51669, 51674,
 51679, 51696, 51700, 51701, 52797, 52798, 52799,
 52800, 53216, 53217, 53222, 53224, 53232, 53237,
 53238, 53259, 53260, 53502, 53503, 53552, 53553,
 53576, 53673, 54151, 54154, 54155, 54486, 54488,
 54489, 54490, 54749, 56343, 56344, 56611, 56612,
 56613, 56614, 56834, 56835, 57472, 57880, 57881,
 58874, 61345, 61346, 63245
);
