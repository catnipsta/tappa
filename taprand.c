#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

int speed;
int dnt[1024],fnt[1024],jnt[1024],knt[1024],dont[1024],font[1024],jont[1024],kont[1024];
char song[64];

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
					dont[i] = atoi(str);
					break;
				case 1:
					font[i] = atoi(str);
					break;
				case 2:
					jont[i] = atoi(str);
					break;
				case 3:
					kont[i] = atoi(str);
					break;
			}
			str = strtok(NULL,",");
			i++;
		}
	}
	fgets(line,4096,lvl);
	speed = atoi(line);
	fgets(song,64,lvl);
	song[strcspn(song,"\n")] = 0;
	fclose(lvl);
	return 0;
}

void dadd(int note){
	for(int i = 0; i < 1024; i++){
		if(dnt[i] == 0){
			dnt[i] = note;
			break;
		}
	}
}

void fadd(int note){
	for(int i = 0; i < 1024; i++){
		if(fnt[i] == 0){
			fnt[i] = note;
			break;
		}
	}
}

void jadd(int note){
	for(int i = 0; i < 1024; i++){
		if(jnt[i] == 0){
			jnt[i] = note;
			break;
		}
	}
}

void kadd(int note){
	for(int i = 0; i < 1024; i++){
		if(knt[i] == 0){
			knt[i] = note;
			break;
		}
	}
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

	srand(time(NULL));

	bool g;
	for(int i = 0; i < 1024; i++){
		int r = rand()%4;
		switch(r){
			case 0:
				g = true;
				for(int ii = 0; ii < 1024; ii++){
					if(dnt[ii] == dont[i]) g = false;
					if(dnt[ii] == 0) break;
				}
				if(g) dadd(dont[i]);
				break;
			case 1:
				g = true;
				for(int ii = 0; ii < 1024; ii++){
					if(fnt[ii] == dont[i]) g = false;
					if(fnt[ii] == 0) break;
				}
				if(g) fadd(dont[i]);
				break;
			case 2:
				g = true;
				for(int ii = 0; ii < 1024; ii++){
					if(jnt[ii] == dont[i]) g = false;
					if(jnt[ii] == 0) break;
				}
				if(g) jadd(dont[i]);
				break;
			case 3:
				g = true;
				for(int ii = 0; ii < 1024; ii++){
					if(knt[ii] == dont[i]) g = false;
					if(knt[ii] == 0) break;
				}
				if(g) kadd(dont[i]);
				break;
		}
	}
	for(int i = 0; i < 1024; i++){
		int r = rand()%4;
		switch(r){
			case 0:
				g = true;
				for(int ii = 0; ii < 1024; ii++){
					if(dnt[ii] == font[i]) g = false;
					if(dnt[ii] == 0) break;
				}
				if(g) dadd(font[i]);
				break;
			case 1:
				g = true;
				for(int ii = 0; ii < 1024; ii++){
					if(fnt[ii] == font[i]) g = false;
					if(fnt[ii] == 0) break;
				}
				if(g) fadd(font[i]);
				break;
			case 2:
				g = true;
				for(int ii = 0; ii < 1024; ii++){
					if(jnt[ii] == font[i]) g = false;
					if(jnt[ii] == 0) break;
				}
				if(g) jadd(font[i]);
				break;
			case 3:
				g = true;
				for(int ii = 0; ii < 1024; ii++){
					if(knt[ii] == font[i]) g = false;
					if(knt[ii] == 0) break;
				}
				if(g) kadd(font[i]);
				break;
		}
	}
	for(int i = 0; i < 1024; i++){
		int r = rand()%4;
		switch(r){
			case 0:
				g = true;
				for(int ii = 0; ii < 1024; ii++){
					if(dnt[ii] == jont[i]) g = false;
					if(dnt[ii] == 0) break;
				}
				if(g) dadd(jont[i]);
				break;
			case 1:
				g = true;
				for(int ii = 0; ii < 1024; ii++){
					if(fnt[ii] == jont[i]) g = false;
					if(fnt[ii] == 0) break;
				}
				if(g) fadd(jont[i]);
				break;
			case 2:
				g = true;
				for(int ii = 0; ii < 1024; ii++){
					if(jnt[ii] == jont[i]) g = false;
					if(jnt[ii] == 0) break;
				}
				if(g) jadd(jont[i]);
				break;
			case 3:
				g = true;
				for(int ii = 0; ii < 1024; ii++){
					if(knt[ii] == jont[i]) g = false;
					if(knt[ii] == 0) break;
				}
				if(g) kadd(jont[i]);
				break;
		}
	}
	for(int i = 0; i < 1024; i++){
		int r = rand()%4;
		switch(r){
			case 0:
				g = true;
				for(int ii = 0; ii < 1024; ii++){
					if(dnt[ii] == kont[i]) g = false;
					if(dnt[ii] == 0) break;
				}
				if(g) dadd(kont[i]);
				break;
			case 1:
				g = true;
				for(int ii = 0; ii < 1024; ii++){
					if(fnt[ii] == kont[i]) g = false;
					if(fnt[ii] == 0) break;
				}
				if(g) fadd(kont[i]);
				break;
			case 2:
				g = true;
				for(int ii = 0; ii < 1024; ii++){
					if(jnt[ii] == kont[i]) g = false;
					if(jnt[ii] == 0) break;
				}
				if(g) jadd(kont[i]);
				break;
			case 3:
				g = true;
				for(int ii = 0; ii < 1024; ii++){
					if(knt[ii] == kont[i]) g = false;
					if(knt[ii] == 0) break;
				}
				if(g) kadd(kont[i]);
				break;
		}
	}

	FILE *file = fopen(argv[1], "w");
	if(file == NULL){
		printf("Error writing to '%s'.\n",argv[1]);
		return 2;
	}

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
	fprintf(file, "\n%d\n%s",speed,song);
	fclose(file);
	return 0;
}
