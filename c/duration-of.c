#include <stdio.h> // for printf

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>

#include "bool.h"


AVFormatContext* av_fmt_ctx;


bool is_frames;


uint64_t get_duration(AVFormatContext* const x){
	return (is_frames) ? x->streams[0]->nb_frames : x->duration;
}


uint64_t duration_of(const char* fp){
	uint64_t n = 0;
	
	if (avformat_open_input(&av_fmt_ctx, fp, NULL, NULL) != 0){
		fprintf(stderr,  "Unable to open video file: %s\n",  fp);
		return 0;
	}
	
	if (avformat_find_stream_info(av_fmt_ctx, NULL) < 0){
		fprintf(stderr,  "Unknown error processing file: %s\n",  fp);
		goto cleanup;
	}
	
	n = get_duration(av_fmt_ctx);
	
	cleanup:
	avformat_close_input(&av_fmt_ctx);
	
	return n;
}


int main(const int argc,  const char** argv){
	av_fmt_ctx = avformat_alloc_context();
	
	is_frames = false;
	
	int i = 1;
	
	if (argv[1][0] == '-'  &&  argv[1][1] == 'f'  &&  argv[1][2] == 0){
		is_frames = true;
		++i;
	}
	
	uint64_t n = 0;
	while(i < argc){
		n += duration_of(argv[i]);
		++i;
	}
	
	if (!is_frames)
		n /= 1000000;
	
	printf("%lu\n",  n);
	
	avformat_free_context(av_fmt_ctx);
	
	return 0;
}
