/**
* @file ide_daemon_api.h
*
* Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#ifndef __IDE_DAEMON_API_H__
#define __IDE_DAEMON_API_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef void *IDE_SESSION;

#ifndef uint32_t
typedef unsigned int uint32_t;
#endif

#define IDE_DAEMON_IP_LEN (16)

typedef struct tagConnInfo
{
    char ip[IDE_DAEMON_IP_LEN];     //IP地址
    int port;                       //端口号
    int deviceID;                   //设备ID号
}connInfo_t;

typedef enum tagIdeError
{
    IDE_DAEMON_NONE_ERROR = 0,                    //无错误
    IDE_DAEMON_UNKNOW_ERROR = 1,                  //未知错误
    IDE_DAEMON_WRITE_ERROR = 2,                   //写入失败
    IDE_DAEMON_NO_SPACE_ERROR = 3,                //磁盘已满
    IDE_DAEMON_INVALID_PATH_ERROR = 4,            //无效路径
    IDE_DAEMON_INVALID_PARAM_ERROR = 5,           //无效参数
    IDE_DAEMON_TCP_CONNECT_ERROR = 6,             //TCP连接失败
    IDE_DAEMON_TCP_CHANNEL_ERROR = 7,             //TCP通道异常
    IDE_DAEMON_MALLOC_ERROR = 8,                  //申请堆内存失败
    IDE_DAEMON_HDC_CHANNEL_ERROR = 9,             //HDC通路异常
    IDE_DAEMON_CHANNEL_ERROR = 10,                //通路异常
    IDE_DAEMON_MKDIR_ERROR = 11,                  //创建目录失败
    IDE_DAEMON_MEMCPY_ERROR = 12,                 //内存拷贝失败
    IDE_DAEMON_MEMSET_ERROR = 13,                 //内存清零失败
    IDE_DAEMON_INVALID_IP_ERROR = 14,             //无效的IP地址
    IDE_DAEMON_INTERGER_REVERSED_ERROR = 15,      //整形溢出
    IDE_DAEMON_DUMP_QUEUE_FULL = 16,              //dump队列已满
    NR_IDE_DAEMON_ERROR,                          //枚举最大值
}ideError_t;

typedef ideError_t IdeErrorT;

enum IdeDumpFlag
{
    IDE_DUMP_NONE_FLAG  = 0,            //无标志位
};

struct IdeDumpChunk
{
    char                *fileName;      //文件名，绝对路径
    unsigned char       *dataBuf;       //写入的数据Buffer
    unsigned int        bufLen;         //写入的数据Buffer长度
    unsigned int        isLastChunk;    //是否最后一块数据   0:非最后一块数据;1：最后一块数据
    long long           offset;         //文件写入的偏移位   -1为追加形式写入
    enum IdeDumpFlag    flag;           //标志位
};

/*****************************************************************************
 函 数 名  : ideOpenFile
 功能描述  : 打开文件
 输入参数  : connInfo_t *connInfo       连接信息，可传空，用默认连接
             cosnt char *fileName       对端IDE保存的文件名，需要绝对路径
 输出参数  : 无
 返 回 值  : 会话句柄
             NULL: 创建会话失败
             非空: 创建会话成功
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2018年08月30日
    修改内容   : 新生成函数
*****************************************************************************/
IDE_SESSION ideOpenFile(connInfo_t *connInfo, const char *fileName);

/*****************************************************************************
 函 数 名  : ideWriteFile
 功能描述  : 进行远程写文件操作
 输入参数  : IDE_SESSION session: 会话句柄
             void *data:    需要写的数据缓存
             uint32_t len:  数据长度
 输出参数  : 无
 返 回 值  : IDE_DAEMON_NONE_ERROR:     写数据成功
             IDE_DAEMON_UNKNOW_ERROR:   写数据失败
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2018年08月30日
    修改内容   : 新生成函数
*****************************************************************************/
ideError_t ideWriteFile(IDE_SESSION session, const void *data, uint32_t len);

/*****************************************************************************
 函 数 名  : ideCloseFile
 功能描述  : 结束写文件
 输入参数  : IDE_SESSION session: 会话句柄
 输出参数  : 无
 返 回 值  : IDE_DAEMON_NONE_ERROR:     关闭文件成功
             IDE_DAEMON_UNKNOW_ERROR:   关闭文件失败
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2018年08月30日
    修改内容   : 新生成函数
*****************************************************************************/
ideError_t ideCloseFile(IDE_SESSION session);

/*****************************************************************************
 函 数 名  : IdeDumpStart
 功能描述  : 创建Dump通路
 输入参数  : const char *privInfo  私有数据（格式host:port;device_id），由MindStudio传入，可传入NULL，使用默认配置
 输出参数  : 无
 返 回 值  : 会话句柄
             NULL: 创建会话失败
             非空: 创建会话成功
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2019年03月14日
    修改内容   : 新生成函数
*****************************************************************************/
extern IDE_SESSION IdeDumpStart(const char *privInfo);

/*****************************************************************************
 函 数 名  : IdeDumpData
 功能描述  : 进行数据Dump,Dump完成数据落盘后返回
 输入参数  : IDE_SESSION session: 会话句柄
             struct IdeDumpChunk *dumpChunk: Dump的数据结构体
 输出参数  : 无
 返 回 值  : IDE_DAEMON_NONE_ERROR:           写数据成功
             IDE_DAEMON_INVALID_PARAM_ERROR   非法参数
             IDE_DAEMON_UNKNOW_ERROR:         写数据失败
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2019年03月14日
    修改内容   : 新生成函数
*****************************************************************************/
extern IdeErrorT IdeDumpData(IDE_SESSION session, const struct IdeDumpChunk *dumpChunk);

/*****************************************************************************
 函 数 名  : IdeDumpEnd
 功能描述  : 关闭Dump通路
 输入参数  : IDE_SESSION session: 会话句柄
 输出参数  : 无
 返 回 值  : IDE_DAEMON_NONE_ERROR:          关闭会话成功
             IDE_DAEMON_INVALID_PARAM_ERROR  非法参数
             IDE_DAEMON_UNKNOW_ERROR:        关闭会话失败
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2019年03月14日
    修改内容   : 新生成函数
*****************************************************************************/
extern IdeErrorT IdeDumpEnd(IDE_SESSION session);

#ifdef __cplusplus
}
#endif

#endif //__IDE_DAEMON_API_H__
