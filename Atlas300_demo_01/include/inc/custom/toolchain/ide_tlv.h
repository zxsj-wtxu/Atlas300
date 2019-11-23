#ifndef __IDE_TLV_H__
#define __IDE_TLV_H__

enum cmd_class
{
    IDE_EXEC_COMMAND_REQ = 0,
    IDE_SEND_FILE_REQ,
    IDE_DEBUG_REQ,
    IDE_BBOX_REQ,
    IDE_LOG_REQ,
    IDE_PROFILING_REQ,
    IDE_OME_DUMP_REQ,
    IDE_FILE_SYNC_REQ,
    IDE_EXEC_API_REQ,
    IDE_EXEC_HOSTCMD_REQ,
    IDE_DETECT_REQ,
    IDE_FILE_GET_REQ,
    IDE_NV_REQ,             
    IDE_INVALID_REQ,
    NR_IDE_CMD_CLASS,
};

struct tlv_req
{
    enum cmd_class type;
    int dev_id;
    int len;
    char value[0];
};

#endif //__IDE_TLV_H__
