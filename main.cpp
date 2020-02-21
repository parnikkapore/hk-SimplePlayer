#include <stdio.h>
#include <iostream>
#include <String.h>
#include <MediaKit.h>
#include <StorageKit.h>

void
makeBuffer(void *_btrack, void *_buffer, size_t size, const media_raw_audio_format &format) {
	status_t err;
	BMediaTrack *btrack = (BMediaTrack *) _btrack;
	
	int64 dummy;
	err = btrack->ReadFrames(_buffer, &dummy);
	if(err) {
		printf("Playback failed: %s\n", strerror(err));
		return;
	}
}

void
playWave(char* path) {
	entry_ref* mref = new entry_ref();
	status_t err;
	
	err = get_ref_for_path(path, mref);
	if(err) {
		printf("Cannot open file: %s\n", strerror(err));
		return;
	}
	 
	BMediaFile mfile(mref);
	if(mfile.InitCheck() != B_OK) {
		printf("Cannot read file: %s\n", strerror(mfile.InitCheck()));
		return;
	}
	
	BMediaTrack* mtrk = mfile.TrackAt(0);
	
	media_format fmt;
	err = mtrk->DecodedFormat(&fmt);
	if(err) {
		printf("Format negotiation failed: %s\n", strerror(err));
		return;
	}
	BSoundPlayer plr(&fmt.u.raw_audio, "SimplePlayer playback", makeBuffer, NULL, mtrk);
	plr.Start(); plr.SetHasData(true);
	printf("Playback started!\n");
	
	std::cin.ignore();
	
	printf("Jumping to midpoint");
	int64 midpt = mtrk->CountFrames()/2;
	mtrk->SeekToFrame(&midpt);
	
	std::cin.ignore();
	
	plr.Stop();
}


int
main(int arg_c, char* args[]) {
	BString Hello("Welcome to C++!\n");
	std::cout << Hello;
	
	if(!args[1]) args[1] = "/boot/home/Desktop/Demo/Cloud 9.ogg";
	playWave(args[1]);
	return 0;
}

