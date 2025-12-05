#include "../include/common.h"

/*
	0 = Easy
	1 = Hard
*/
#define VERSION 0

#if VERSION == 0
	#define MAX_SIZE 4.5f
	#define MIN_SIZE 0.05f
	#define CB_MAX 3500
#else
	#define MAX_SIZE 1.5f
	#define MIN_SIZE 0.03f
	#define CB_MAX 3500
#endif
static short actors_to_shrink[] = {
	2, 3, 4, 5, 6,  // Kongs
	7, // Krusha, just for good measure
	8, 9, // Rambi & Enguarde
	196, 197, 198, 199, 200, // Cutscene Kongs
	316, // Tag Barrel Kong
	327, // Kong: Krazy KK
	106, 123, // Reflections
};
static short file_init_flags[] = {
	0x17F,
	0x180,
	378,
	383,
	386,
	387,
	388,
	389,
	391,
	384,
	0x1BB,
	0x186,
	0,
	385, // Free DK
};

int isShrinkActor(actorData *test_actor) {
	for (int i = 0; i < sizeof(actors_to_shrink) >> 1; i++) {
		if (test_actor->actorType == actors_to_shrink[i]) {
			return 1;
		}
	}
	return 0;
}

void cFuncLoop(void) {
	/*
		This function is run every frame. Place c code which you are wanting to be run every frame
	*/
	initHack(); // Initialize hack
	tagAnywhere(); // Tag Anywhere every frame functions
	if (grab_lock_timer >= 0) {
		grab_lock_timer += 1;
		if (grab_lock_timer > 10) {
			grab_lock_timer = -1;
		}
	}
	if (tag_locked) {
		tag_locked = 0;
	}
	/*
		CB Scaling
		Min Size: 0.01 - 0 CBs
		Normal Size: 0.15 - 100 CBs
		Max Size: 1.5 - 3500 CBs
	*/
	int cb_count = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 7; j++) {
			cb_count += MovesBase[i].cb_count[j];
		}
	}
	int delta = CB_MAX - cb_count;
	float quad = delta * delta;
	float ratio = ((float)MAX_SIZE) / ((float)(CB_MAX * CB_MAX));
	float dec = ratio * quad;
	float scale = (MAX_SIZE + MIN_SIZE) - dec;
	for (int a = 0; a < LoadedActorCount; a++) {
		actorData *actor = LoadedActorArray[a].actor;
		if (isShrinkActor(actor)) {
			rendering_params *render = actor->render;
			if (render) {
				for (int i = 0; i < 3; i++) {
					render->scale[i] = scale;
				}
			}
		}		
	}
	StorySkip = 1;
	if ((CurrentMap == 0x50) && (DestMap != 0x50) && (Mode == 6)) {
		if (!checkFlag(file_init_flags[0], FLAGTYPE_PERMANENT)) {
			for (int i = 0; i < sizeof(file_init_flags) >> 1; i++) {
				setFlag(file_init_flags[i], 1, FLAGTYPE_PERMANENT);
			}
			#if VERSION == 0
				for (int i = 0; i < 5; i++) {
					MovesBase[i].simian_slam = 1;
					MovesBase[i].special_moves = 3;
					MovesBase[i].instrument_bitfield = 1;
					MovesBase[i].weapon_bitfield = 1;
					MovesBase[i].instrument_energy = 5;
				}
				CollectableBase.Melons = 2;
				CollectableBase.Health = 8;
				CollectableBase.StandardAmmo = 50;
				setFlag(771, 1, FLAGTYPE_PERMANENT); // Open Coin Door
			#else
				for (int i = 0; i < 5; i++) {
					MovesBase[i].simian_slam = 1;
				}
			#endif
		}
		DestMap = 0x22;
		DestExit = 0;
	}
	// Things to speed up the game progression, half all requirements
	if (ObjectModel2Timer < 2) {
		BossReqArray[7] = 1; // Fix KRE
		#if VERSION == 0
			// CBs to unlock bosses
			BossReqArray[0] = 30;
			BossReqArray[1] = 60;
			BossReqArray[2] = 100;
			BossReqArray[3] = 125;
			BossReqArray[4] = 150;
			BossReqArray[5] = 175;
			BossReqArray[6] = 200;
			// GBs to clear B. Locker
			BLockerArray[0] = 1;
			BLockerArray[1] = 3;
			BLockerArray[2] = 8;
			BLockerArray[3] = 15;
			BLockerArray[4] = 25;
			BLockerArray[5] = 32;
			BLockerArray[6] = 40;
			BLockerArray[7] = 50;
			// GBs to clear B. Locker  (Cheaty McCheaterface)
			BLockerCheatArray[0].requirement = 1;
			BLockerCheatArray[1].requirement = 2;
			BLockerCheatArray[2].requirement = 5;
			BLockerCheatArray[3].requirement = 10;
			BLockerCheatArray[4].requirement = 17;
			BLockerCheatArray[5].requirement = 25;
			BLockerCheatArray[6].requirement = 32;
			BLockerCheatArray[7].requirement = 40;
		#endif
	}
}
