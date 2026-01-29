#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

int speed;
int dnt[1024],fnt[1024],jnt[1024],knt[1024],ont[4096];
char song[64];

int load(char *lvlname){
	FILE *lvl = fopen(lvlname, "r");
	if(lvl == NULL) return 1;
	char line[4096];
	int i = 0;
	for(int l=0; l < 4; l++){
		fgets(line,4096,lvl);
		char *str = strtok(line,",");
		while(str != NULL){
			ont[i] = atoi(str);
			str = strtok(NULL,",");
			if(ont[i] != 0) i++;
		}
	}
	fgets(line,4096,lvl);
	speed = atoi(line);
	fgets(song,64,lvl);
	song[strcspn(song,"\n")] = 0;
	fclose(lvl);
	return 0;
}

void append(int arr[], int note){
	for(int i = 0; i < 1024; i++){
		if(arr[i] == 0){
			arr[i] = note;
			break;
		}
	}
}

void randomize(){
	int or = 4;
	for(int i = 0; i < 4096; i++){
		if(ont[i] == 0) break;
		int r;
		bool g = false;
		while(!g){
			r = rand()%4;
			switch(r){
				case 0:
					g = true;
					for(int ii = 0; ii < 1024; ii++){
						if(dnt[ii] == ont[i]) g = false;
						if(dnt[ii] == 0) break;
					}
					break;
				case 1:
					g = true;
					for(int ii = 0; ii < 1024; ii++){
						if(fnt[ii] == ont[i]) g = false;
						if(fnt[ii] == 0) break;
					}
					break;
				case 2:
					g = true;
					for(int ii = 0; ii < 1024; ii++){
						if(jnt[ii] == ont[i]) g = false;
						if(jnt[ii] == 0) break;
					}
					break;
				case 3:
					g = true;
					for(int ii = 0; ii < 1024; ii++){
						if(knt[ii] == ont[i]) g = false;
						if(knt[ii] == 0) break;
					}
					break;
			}
			if(or == r && rand()%7 > 0) g = false;
		}
		switch(r){
			case 0:
				append(dnt, ont[i]);
				break;
			case 1:
				append(fnt, ont[i]);
				break;
			case 2:
				append(jnt, ont[i]);
				break;
			case 3:
				append(knt, ont[i]);
				break;
		}
		or = r;
	}
}

void order(){
	int min, mi;
	for(int i = 0; i < 4096; i++){
		if(ont[i] == 0) break;
		min = 999999999;
		for(int ii = i; ii < 4096; ii++){
			if(min > ont[ii] && ont[ii] != 0){
				min = ont[ii];
				mi = ii;
			}
		}
		int t = ont[i];
		ont[i] = ont[mi];
		ont[mi] = t;
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

	order();
	randomize();

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
