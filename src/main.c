#include "../include/common.h"

#define MAX_SIZE 1.5f
#define MIN_SIZE 0.01f
#define CB_MAX 3500

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
	if (Player) {
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
		rendering_params *render = Player->rendering_param_pointer;
		if (render) {
			for (int i = 0; i < 3; i++) {
				render->scale[i] = scale;
			}
		}
	}
}
