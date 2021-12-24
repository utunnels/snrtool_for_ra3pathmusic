#include "simex.h"
#include <iostream>
#include <stdio.h>
#include <string>

void testfunction(char * path) {
	SIMEX_init();
	int type = SIMEX_id(path, 0);
	SINSTANCE* instance;
	SIMEX_open(path, 0, type, &instance);
	SINSTANCE* instance2;
	char buf[256];
	strcpy_s(buf, path);
	int l = strlen(buf);
	buf[l - 4] = '\0';
	strcat_s(buf, ".wav");
	SIMEX_create(buf, wave, &instance2);
	SINFO* info;
	SIMEX_info(instance, &info, 0);
	if (info)
	{
		SIMEX_read(instance, info, 0);
		printf("%d %d", info->sound[0]->num_samples, info->sound[0]->sample_rate); 
		info->sound[0]->codec = s16l_int;
		SIMEX_write(instance2, info, 0);
		SIMEX_freesinfo(info);
	}
	SIMEX_close(instance);
	SIMEX_wclose(instance2);
	SIMEX_shutdown();
}

int main(int argc, char* argv[])
{
	int cc = argc > 1 ? std::stoi(argv[1]) : s16b_int;
	char* infile = argc > 2 ? argv[2] : "input.wav";
	char* outfile = argc > 3 ? argv[3] : "output";
	int l = strlen(infile);
	if (l > 4 && strcmp(infile + l - 4, ".snr")==0) {
		testfunction(infile);
		return 0;
	}
	SIMEX_init();
	int type = SIMEX_id(infile, 0);
	SINSTANCE* instance;
	SIMEX_open(infile, 0, type, &instance);
	SINSTANCE* instance2;
	SIMEX_create(outfile, sndplayer, &instance2);
	SINFO* info;
	SIMEX_info(instance, &info, 0);
	if (info)
	{
		SIMEX_read(instance, info, 0);
		info->sound[0]->codec = (unsigned char)cc;// ealayer3_int;
		info->sound[0]->playloc = PLAYLOC_STREAM;
		SIMEX_write(instance2, info, 0);
		SIMEX_freesinfo(info);
	}
    SIMEX_close(instance);
    SIMEX_wclose(instance2);
    SIMEX_shutdown();
	return 0;
}
