#include "StreamMessage.h"
StreamMessage::StreamMessage()
{
	senders = (struct sender_data*)av_mallocz_array(1, sizeof(*senders));
	receivers = (struct receiver_data*)av_mallocz_array(1, sizeof(*receivers));
}

StreamMessage::~StreamMessage()
{
	av_thread_message_queue_free(&queue);
	av_freep(&senders);
	av_freep(&receivers);
}

int StreamMessage::H264StreamMessageRefresh()
{
	av_thread_message_flush(receivers[0].queue);
	return 0;
}

int StreamMessage::H264StreamMessageQueueInit(int max_queue_size)
{
	int ret = av_thread_message_queue_alloc(&queue, max_queue_size, sizeof(struct H264StreamMessage));
	av_thread_message_queue_set_free_func(queue, FreeH264StreamMessage);

	senders[0].queue = queue;
	receivers[0].queue = queue;

	return ret;
}

int StreamMessage::H264StreamMessageQueueSend(const AVPacket& pkt)
{
	//return StreamMessageQueueSend(pkt);
	struct H264StreamMessage msg;
	msg.magic = MAGIC;
	msg.size = pkt.size;
	msg.data = new uint8_t[msg.size];
	memcpy(msg.data, pkt.data, msg.size);
	int ret = av_thread_message_queue_send(senders[0].queue, &msg, 1);
	if (ret == AVERROR(EAGAIN)) {
		av_thread_message_queue_send(senders[0].queue, &msg, 1);
	}else if (ret < 0) {
		delete msg.data;
		printf("av_thread_message_queue_send failed, error:%d\n", ret);
	}
	return ret;
}

int StreamMessage::H264StreamMessageQueueRecv(unsigned char*& pdataOut, unsigned int& framesize)
{
	//return StreamMessageQueueRecv(pdataOut, framesize);
	struct H264StreamMessage msg;
	int ret = av_thread_message_queue_recv(receivers[0].queue, &msg, 0);
	if (ret < 0) {
		printf("av_thread_message_queue_recv, error:%d = %d\n", ret, AVERROR(EAGAIN));
		return -1;
	}
	framesize = (unsigned int)msg.size;
	memcpy(pdataOut, msg.data, framesize);
	av_thread_message_flush(receivers[0].queue);
	return 0;
}

void StreamMessage::FreeH264StreamMessage(void* arg)
{
	struct H264StreamMessage* msg = (struct H264StreamMessage*)arg;
	av_assert0(msg->magic == MAGIC);
	delete msg->data;
}