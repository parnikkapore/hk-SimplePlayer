#include <stdio.h>
#include <iostream>
#include <String.h>
#include <MediaKit.h>

// Wavegen state
typedef struct {
	float val;
	float dir;
} ck_state;

void
makeBuffer(void *pcookie, void *buffer, size_t size, const media_raw_audio_format &format) {
	size_t i,j;
	float *buf = (float*) buffer;
	size_t float_size = size/4;
	uint32 numCh = format.channel_count;
	ck_state *cookie = (ck_state *) pcookie;
	
	// Only accept float
	if(format.format != media_raw_audio_format::B_AUDIO_FLOAT) return;
	
	// fill buffer
	for(i=0; i<float_size; i+=numCh) {
		for(j=0; j<numCh; j++){
			buf[i+j] = cookie->val;
		}
		if((cookie->dir == 1.0) && (cookie->val >= 1.0)) cookie->dir = -1.0;
		else if((cookie->dir == -1.0) && (cookie->val <= -1.0)) cookie->dir = 1.0;
		
		cookie->val += cookie->dir*(1.0/64.0);
	}
}

void
playWave() {
	ck_state cookie;
	cookie.val = 0.0; cookie.dir = 1.0;
	
	BSoundPlayer plr("SimplePlayer playback", makeBuffer, NULL, &cookie);
	plr.Start(); plr.SetHasData(true);
}


int
main() {
	BString Hello("Welcome to C++!\n");
	std::cout << Hello;
	playWave();
	scanf("\n");
	return 0;
}

