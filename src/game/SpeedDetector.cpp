#include "SpeedDetector.h"
#include "Player.h"

SpeedCheatDetector::SpeedCheatDetector()
{
	biggest_hspeed_dif = 0;
	cheat_threat = 0;
	last_used_speed = 0;
	last_stamp = 0;
        new_stamp = 0;
	delay_stamp = 0;
        flags_test = 0;
}

#define max(a_t, b_t) (((a_t) > (b_t)) ? (a_t) : (b_t))

void SpeedCheatDetector::Check(Player *_player, float dx, float dy)
{
	if (delay_stamp != 0) {
                if (delay_stamp <= new_stamp) {
                        last_stamp = new_stamp;
			delay_stamp = 0;
		}

		return;
	}

        if (flags_test)
            return;

	// simplified: just take the fastest speed. less chance of fuckups too
        float player_speed = max(_player->GetSpeed(MOVE_RUN), _player->GetSpeed(MOVE_SWIM));
        if (_player->IsFlying()) player_speed = max(player_speed, _player->GetSpeed(MOVE_FLIGHT));

        //extended: all speeds
        player_speed = max(player_speed, _player->GetSpeed(MOVE_WALK));
        player_speed = max(player_speed, _player->GetSpeed(MOVE_RUN_BACK));
        player_speed = max(player_speed, _player->GetSpeed(MOVE_SWIM_BACK));
        if (_player->IsFlying()) player_speed = max(player_speed, _player->GetSpeed(MOVE_FLIGHT_BACK));

	if( player_speed - last_used_speed >= 0.1  ||  player_speed - last_used_speed <= -0.1)
	{
		last_used_speed = player_speed;
                last_stamp = new_stamp;
		return;
	}

        if (new_stamp < last_stamp) {
		cheat_threat++;

		delay = 1280;
		return; 
	}

        int time_dif = new_stamp - last_stamp;

        //moved check from movement handler>player : script\teleport moves must be skipped
        if (time_dif == 0)
            return;

	//don't flood with calculations.
        if ( time_dif <= SPDT_SAMPLINGRATE )
		return; 

	//get current speed
        float dist = sqrt(dx*dx + dy*dy);
        float cur_speed = dist / (float)time_dif * 1000.0f;

	//check if we really got a cheater here
	if( cur_speed - SPDT_DETECTION_ERROR > player_speed )
	{
		float hackspeed_diff = cur_speed - player_speed;
		if( hackspeed_diff > biggest_hspeed_dif )
			biggest_hspeed_dif = hackspeed_diff;

                cheat_threat++;

		delay = (hackspeed_diff < 14) ? 20 : (hackspeed_diff < 70) ? 160 : 1280;
	}
	else if( cheat_threat > 0 )
	{
		delay--;
		if (delay == 0) {
			cheat_threat--;

			delay = (biggest_hspeed_dif < 14) ? 20 : (biggest_hspeed_dif < 70) ? 160 : 1280;
		}
	}

        last_stamp = new_stamp;
}

void SpeedCheatDetector::ReportCheater(Player *_player)
{
    if(sWorld.GetMvAnticheatKill() && _player->isAlive())
    {
        _player->DealDamage(_player, _player->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
    }

    if(sWorld.GetMvAnticheatKick())
    {
        _player->GetSession()->KickPlayer();
    }

    if(sWorld.GetMvAnticheatBan() & 1)
    {
        sWorld.BanAccount(BAN_CHARACTER, _player->GetName(), sWorld.GetMvAnticheatBanTime(), "Speedhack Cheat", "Anticheat 2");
    }

    if(sWorld.GetMvAnticheatBan() & 2)
    {
        sWorld.BanAccount(BAN_IP, _player->GetSession()->GetRemoteAddress(), sWorld.GetMvAnticheatBanTime(), "Speedhack Cheat", "Anticheat 2");
    }
}
