-- Insert comments to scriptdev2 boss_spell_table from WOWD database. 
-- Change WOWD database name if you use this!

CREATE ALGORITHM = TEMPTABLE VIEW `commentlist` 
(`entry` ,`spell`, `comment`)
AS SELECT  `boss_spell_table`.`entry`,
`spellID_N10`,
CONCAT( `creature_template`.`name`,
' : ',
 `wowd_spell`.`SpellName`)
FROM  `boss_spell_table`
INNER JOIN  `creature_template` ON  `creature_template`.`entry` =  `boss_spell_table`.`entry` 
INNER JOIN  `wowd_spell` ON  `wowd_spell`.`id` =  `boss_spell_table`.`spellID_N10`;

UPDATE  `boss_spell_table` SET `comment` =  (SELECT DISTINCT `commentlist`.`comment`
FROM `commentlist` WHERE  `boss_spell_table`.`entry` = `commentlist`.`entry` 
AND  `boss_spell_table`.`spellID_N10` = `commentlist`.`spell`
AND  `boss_spell_table`.`comment` = NULL);
DROP VIEW `commentlist`;
