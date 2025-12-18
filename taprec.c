#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <ncurses.h>

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

int t, length, speed;
bool d,f,j,k,quit,paused;
int dnt[1024],fnt[1024],jnt[1024],knt[1024];

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
	mvprintw(my-1,mx/2-5,"d  f  j  k");
	mvprintw(my-2,mx/2-5,"_  _  _  _");
	if(d == true) mvprintw(my-2,mx/2-5,"*");
	if(f == true) mvprintw(my-2,mx/2-2,"*");
	if(j == true) mvprintw(my-2,mx/2+1,"*");
	if(k == true) mvprintw(my-2,mx/2+4,"*");
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

void seek(){
	bool waspaused;
	if(!paused) togglepause();
	else waspaused=true;
	echo();
	nodelay(stdscr,false);
	curs_set(1);
	nocbreak();
	int my, mx;
	getmaxyx(stdscr, my, mx);
	mvprintw(my-1,0,"Where to? ");
	refresh();
	char seekto[16];
	getnstr(seekto, 15);
	if(atoi(seekto) != NULL){
		t=atoi(seekto);
		ma_decoder_seek_to_pcm_frame(&decoder, (float)atoi(seekto)/speed*decoder.outputSampleRate);
	}
	if(!waspaused) togglepause();
	for(int x = 0; x < mx; x++) mvprintw(my-1,x," ");
	refresh();
	noecho();
	nodelay(stdscr,true);
	curs_set(0);
	cbreak();
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
		case ' ':
			togglepause();
			break;
		case '/':
			seek();
			break;
	}
}

void notekeep(void){
	if(d == true){
		for(int i = 0; i < 1024; i++){
			if(dnt[i] == 0){
				dnt[i] = t;
				break;
			}
		}
	}
	if(f == true){
		for(int i = 0; i < 1024; i++){
			if(fnt[i] == 0){
				fnt[i] = t;
				break;
			}
		}
	}
	if(j == true){
		for(int i = 0; i < 1024; i++){
			if(jnt[i] == 0){
				jnt[i] = t;
				break;
			}
		}
	}
	if(k == true){
		for(int i = 0; i < 1024; i++){
			if(knt[i] == 0){
				knt[i] = t;
				break;
			}
		}
	}
}

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount){
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == NULL) return;
    ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);
    (void)pInput;
}

int main(int argc, char *argv[]){
	if(argc != 4){
		printf("Pass 1st parameter as output file name, 2nd as level speed, 3rd as sound file.\nFor example,\ntaprec song.tap 20 song.mp3\n");
		return 1;
	}
	speed = atoi(argv[2]);
	FILE *file= fopen(argv[1], "w");
	if(file == NULL){
		printf("Error opening '%s'.\n",argv[1]);
		return 2;
	}

	result = ma_decoder_init_file(argv[3], NULL, &decoder);
        if (result != MA_SUCCESS) {
                printf("Could not load song: %s\n", argv[3]);
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

	ma_decoder_seek_to_pcm_frame(&decoder, 0);
	while(t < length && !quit){
		ma_decoder_get_cursor_in_pcm_frames(&decoder, &frame);
		t++;
		draw();
		notekeep();
		d=false;
		f=false;
		j=false;
		k=false;
		while((float)frame/frames < (float)t/length){
			input();
			ma_decoder_get_cursor_in_pcm_frames(&decoder, &frame);
		}
	}
	endwin();
	ma_device_uninit(&device);
	ma_decoder_uninit(&decoder);

	int i = 0;
	while(dnt[i] != 0){
		if(i == 0) fprintf(file, "%d", dnt[i]);
		else fprintf(file, ",%d", dnt[i]);
		i++;
	}
	fprintf(file, "\n");
	i=0;
	while(fnt[i] != 0){
		if(i == 0) fprintf(file, "%d", fnt[i]);
		else fprintf(file, ",%d", fnt[i]);
		i++;
	}
	fprintf(file, "\n");
	i=0;
	while(jnt[i] != 0){
		if(i == 0) fprintf(file, "%d", jnt[i]);
		else fprintf(file, ",%d", jnt[i]);
		i++;
	}
	fprintf(file, "\n");
	i=0;
	while(knt[i] != 0){
		if(i == 0) fprintf(file, "%d", knt[i]);
		else fprintf(file, ",%d", knt[i]);
		i++;
	}
	fprintf(file, "\n%d\n%s",speed,argv[3]);
	fclose(file);
	return 0;
}
