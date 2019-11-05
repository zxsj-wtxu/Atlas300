/**
 * Copyright (C) 2018. Huawei Technologies Co., Ltd. All rights reserved.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 */

#ifndef VDEC_HIAI_H
#define VDEC_HIAI_H

#include <memory>
#include "export_macro.h"
class HIAI_DATA_SP
{
public:
    HIAI_DATA_SP()
    {
        frameIndex = 0;
        frameBuffer = NULL;
        frameSize = 0;
    }
    virtual ~HIAI_DATA_SP()
    {
    }
    void setFrameIndex(unsigned long long index)
    {
        frameIndex = index;
    }
    unsigned long long getFrameIndex()
    {
        return frameIndex;
    }
    void setFrameBuffer(void * frameBuff)
    {
        frameBuffer = frameBuff;
    }
    void * getFrameBuffer()
    {
        return frameBuffer;
    }
    void setFrameSize(unsigned int size)
    {
        frameSize = size;
    }
    unsigned int getFrameSize()
    {
        return frameSize;
    }
private:
    unsigned long long frameIndex;
    void * frameBuffer;
    unsigned int frameSize;
    HIAI_DATA_SP(const HIAI_DATA_SP& ); 
    const HIAI_DATA_SP& operator= (const HIAI_DATA_SP&); 
};

typedef struct FRAME
{
    int height;
    int width;
    int realHeight;
    int realWidth;
    unsigned char* buffer;
    int buffer_size;
    unsigned int offset_payload_y;
    unsigned int offset_payload_c;
    unsigned int offset_head_y;
    unsigned int offset_head_c;
    unsigned int stride_payload;
    unsigned int stride_head;
    unsigned short bitdepth;
    char video_format[10];
    char image_format[10];
    //bool end_flag;
}FRAME;

enum ERRTYPE{
    //decoder state error
    ERR_INVALID_STATE = 0x10001,
    //hardware error
    ERR_HARDWARE,
    //scd decode fail
    ERR_SCD_CUT_FAIL,
    //vdm decode fail
    ERR_VDM_DECODE_FAIL,
    //alloc common memory fail
    ERR_ALLOC_MEM_FAIL,
    //Dynamic frame storage alloc fail
    ERR_ALLOC_DYNAMIC_MEM_FAIL,
    //config in and out port fail
    ERR_ALLOC_IN_OR_OUT_PORT_MEM_FAIL,
    //bit stream error
    ERR_BITSTREAM,
    //input video foramt error
    ERR_VIDEO_FORMAT,
    //output image foramt error
    ERR_IMAGE_FORMAT,
    //callback function is null error
    ERR_CALLBACK,
    //input buffer is null error
    ERR_INPUT_BUFFER,
    //input buffer size is zero error
    ERR_INBUF_SIZE,
    //create fill the buffer done thread failed
    ERR_THREAD_CREATE_FBD_FAIL,
    //create instance failed
    ERR_CREATE_INSTANCE_FAIL,
    //decoder init failed
    ERR_INIT_DECODER_FAIL,
    //get channel handle failed
    ERR_GET_CHANNEL_HANDLE_FAIL,
    //component set failed
    ERR_COMPONENT_SET_FAIL,
    //compare name failed
    ERR_COMPARE_NAME_FAIL,
    //other error
    ERR_OTHER
};

typedef struct VDECERR{
    ERRTYPE errType;
    unsigned short channelId;
}VDECERR;




#endif //VDEC_HIAI
