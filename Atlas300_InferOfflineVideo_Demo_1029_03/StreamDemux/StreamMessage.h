#pragma once
#ifndef __STREAM_MESSAGE__
#define __STREAM_MESSAGE__
#define MAGIC 0xdeadc0de
#include <iostream>
#include <string>
#include <queue>
#include <mutex>
extern "C" {
#include "libavfilter/avfilter.h"
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/imgutils.h"
#include "libavutil/samplefmt.h"
#include "libavutil/timestamp.h"
#include "libavutil/avassert.h"
#include "libavutil/avstring.h"
#include "libavutil/frame.h"
#include "libavutil/threadmessage.h"
}

struct sender_data {
	AVThreadMessageQueue* queue;
};

/* same as sender_data but shuffled for testing purpose */
struct receiver_data {
	AVThreadMessageQueue* queue;
};

struct H264StreamMessage {
	uint8_t* data;
	int size;
	int magic;
};

struct AVFrameMessage {
	AVFrame* frame;
	int magic;
};

class StreamMessage {
public:
	StreamMessage();
	~StreamMessage();
	int H264StreamMessageRefresh();
	int H264StreamMessageQueueInit(int max_queue_size);
	int H264StreamMessageQueueSend(const AVPacket& pkt);
	int H264StreamMessageQueueRecv(unsigned char*& pdataOut, unsigned int& framesize);
private:
	struct sender_data* senders;
	struct receiver_data* receivers;

	AVThreadMessageQueue* queue = NULL;

	static void FreeH264StreamMessage(void* arg);

	int max_queue_size = 25;
};

#endif