#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>
#include <ncurses.h>

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

int t, length, score, speed, ntl;
bool d,f,j,k,quit,paused;
int dnt[1024],fnt[1024],jnt[1024],knt[1024];
char song[64];

ma_result result;
ma_decoder decoder;
ma_device_config deviceConfig;
ma_device device;

void draw(void){
	int my, mx;
	getmaxyx(stdscr, my, mx);
	for(int y = 0; y < my; y++){
		for(int x = 0; x < mx; x++){
			mvprintw(y,x," ");
		}
	}
	mvprintw(0,0,"time: %d/%d",t,length);
	mvprintw(1,0,"score: %d",score);
	mvprintw(0,mx/2-5,"d  f  j  k");
	mvprintw(1,mx/2-5,"_  _  _  _");
	for(int i = 0; i < 1024; i++){
		if(dnt[i]-t < my-1 && dnt[i]-t >= 1) mvprintw(dnt[i]-t+1,mx/2-5,"o");
		if(fnt[i]-t < my-1 && fnt[i]-t >= 1) mvprintw(fnt[i]-t+1,mx/2-2,"o");
		if(jnt[i]-t < my-1 && jnt[i]-t >= 1) mvprintw(jnt[i]-t+1,mx/2+1,"o");
		if(knt[i]-t < my-1 && knt[i]-t >= 1) mvprintw(knt[i]-t+1,mx/2+4,"o");
		if(dnt[i] == 0 && fnt[i] == 0 && jnt[i] == 0 && knt[i] == 0) break;
	}
	if(d == true) mvprintw(1,mx/2-5,"*");
	if(f == true) mvprintw(1,mx/2-2,"*");
	if(j == true) mvprintw(1,mx/2+1,"*");
	if(k == true) mvprintw(1,mx/2+4,"*");
	refresh();
}

void togglepause(){
	if(paused){
		paused=false;
		ma_device_start(&device);
	}
	else{
		paused=true;
		ma_device_stop(&device);
	}
	mvprintw(0,0,"time: %d/%d (paused)",t,length);
        refresh();
}

void input(void){
	switch(getch()){
		case 'd':
			d = true;
			break;
		case 'f':
			f = true;
			break;
		case 'j':
			j = true;
			break;
		case 'k':
			k = true;
			break;
		case 'q':
			quit = true;
			if(paused) togglepause();
			break;
		case 'r':
			score = -69420;
			break;
		case ' ':
			togglepause();
			break;
	}
}
void scorekeep(void){
	bool ed=false,ef=false,ej=false,ek=false;
	for(int i = 0; i < 1024; i++){
		if(dnt[i]-t >= -ntl && dnt[i]-t <= ntl) ed = true;
		if(fnt[i]-t >= -ntl && fnt[i]-t <= ntl) ef = true;
		if(jnt[i]-t >= -ntl && jnt[i]-t <= ntl) ej = true;
		if(knt[i]-t >= -ntl && knt[i]-t <= ntl) ek = true;
		if(dnt[i]-t >= -ntl && dnt[i]-t <= ntl && dnt[i] != 0 && d == true){
			score++;
			dnt[i] = -99;
		}
		else if(dnt[i]-t == -(ntl+1) && dnt[i] != 0) score--;
		if(fnt[i]-t >= -ntl && fnt[i]-t <= ntl && fnt[i] != 0 && f == true){
			score++;
			fnt[i] = -99;
		}
		else if(fnt[i]-t == -(ntl+1) && fnt[i] != 0) score--;
		if(jnt[i]-t >= -ntl && jnt[i]-t <= ntl && jnt[i] != 0 && j == true){
			score++;
			jnt[i] = -99;
		}
		else if(jnt[i]-t == -(ntl+1) && jnt[i] != 0) score--;
		if(knt[i]-t >= -ntl && knt[i]-t <= ntl && knt[i] != 0 && k == true){
			score++;
			knt[i] = -99;
		}
		else if(knt[i]-t == -(ntl+1) && knt[i] != 0) score--;
		if(dnt[i] == 0 && fnt[i] == 0 && jnt[i] == 0 && knt[i] == 0) break;
	}
	if(d == true && ed == false) score--;
	if(f == true && ef == false) score--;
	if(j == true && ej == false) score--;
	if(k == true && ek == false) score--;
}

int load(char *lvlname){
	FILE *lvl = fopen(lvlname, "r");
	if(lvl == NULL) return 1;
	char line[4096];
	for(int l=0; l < 4; l++){
		fgets(line,4096,lvl);
		char *str = strtok(line,",");
		int i = 0;
		while(str != NULL){
			switch(l){
				case 0:
					dnt[i] = atoi(str);
					break;
				case 1:
					fnt[i] = atoi(str);
					break;
				case 2:
					jnt[i] = atoi(str);
					break;
				case 3:
					knt[i] = atoi(str);
					break;
			}
			str = strtok(NULL,",");
			i++;
		}
	}
	fgets(line,4096,lvl);
	speed = atoi(line);
	ntl=round((float)speed/10);
	if(ntl < 1) ntl = 1;
	fgets(song,64,lvl);
	song[strcspn(song,"\n")] = 0;
	fclose(lvl);
	return 0;
}

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount){
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == NULL) return;
    ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);
    (void)pInput;
}

int main(int argc, char *argv[]){
	if(argv[1] == NULL){
		printf("No level file provided.\n");
		return 1;
	}
	if(load(argv[1]) != 0){
		printf("Failed to open file '%s'.\n", argv[1]);
		return 1;
	}

	result = ma_decoder_init_file(song, NULL, &decoder);
	if (result != MA_SUCCESS) {
		printf("Could not load song: %s\n", song);
		return -2;
	}
	deviceConfig = ma_device_config_init(ma_device_type_playback);
	deviceConfig.playback.format   = decoder.outputFormat;
	deviceConfig.playback.channels = decoder.outputChannels;
	deviceConfig.sampleRate        = decoder.outputSampleRate;
	deviceConfig.dataCallback      = data_callback;
	deviceConfig.pUserData         = &decoder;
	if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) {
		printf("Failed to open playback device.\n");
		ma_decoder_uninit(&decoder);
		return -3;
	}
	if (ma_device_start(&device) != MA_SUCCESS) {
		printf("Failed to start playback device.\n");
		ma_device_uninit(&device);
		ma_decoder_uninit(&decoder);
		return -4;
	}
        ma_uint64 frames;
        ma_decoder_get_length_in_pcm_frames(&decoder, &frames);
	length=frames/decoder.outputSampleRate*speed;
        ma_uint64 frame;

	initscr();
	cbreak();
	noecho();
	nodelay(stdscr,true);
	curs_set(0);

	while(t < length && !quit){
		if(load(argv[1]) != 0){
			endwin();
			printf("Failed to open file '%s'.\n", argv[1]);
			return 1;
		}
		t=0;
		score=0;
		d=false;
		f=false;
		j=false;
		k=false;
	        ma_decoder_seek_to_pcm_frame(&decoder, 0);
		while(score >= -13 && t < length && !quit){
                	ma_decoder_get_cursor_in_pcm_frames(&decoder, &frame);
			t++;
			draw();
			scorekeep();
			d=false;
			f=false;
			j=false;
			k=false;
			while((float)frame/frames < (float)t/length){
				input();
                		ma_decoder_get_cursor_in_pcm_frames(&decoder, &frame);
			}
		}
	}
	endwin();
	ma_device_uninit(&device);
	ma_decoder_uninit(&decoder);
	printf("song: %s\ntime: %d/%d\nscore: %d\n",song,t,length,score);
	return 0;
}
