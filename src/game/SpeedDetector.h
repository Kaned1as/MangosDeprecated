#ifndef _SPEED_DETECTOR_H_
#define _SPEED_DETECTOR_H_

#define SPDT_SAMPLINGRATE		450   // there is no point flooding calculations
#define SPDT_DETECTION_ERROR		1     // +(units per sec) detection error
#define CHEAT_ALARMS_TO_TRIGGER_CHEAT	sWorld.GetMvAnticheatAlarmCount()   // if x alarms stack up over time then it is time to kick the player

#include "World.h"

class SpeedCheatDetector
{
	public:
		SpeedCheatDetector();
                void		Check(Player *_player, float dx, float dy); // update the detector with new values
                inline char	IsCheatDetected(){ return cheat_threat >= CHEAT_ALARMS_TO_TRIGGER_CHEAT; }
		inline void     ResetThreat(){ cheat_threat = 0; }
                void		Skip(uint32 time) { delay_stamp = new_stamp + time; }
                void            UpdateMvData(uint32 time, uint32 flgs) { new_stamp = time; flags_test = flgs; }
                void		ReportCheater(Player *_player);

	private:
                uint32                  flags_test;
		uint32			last_stamp;
                uint32                  new_stamp;
		uint32			delay_stamp;		//for delaying detector
                signed int		cheat_threat;		//don't draw quick conclusions. If player is suspicios over time then kill him
		float			last_used_speed;	//we reset if speed changed since our last measure
		float			biggest_hspeed_dif;
                uint32			delay;			//delay to decrement threat
};

#endif
