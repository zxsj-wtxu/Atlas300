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
    IDE_DAEMON_NONE_ERROR,
    IDE_DAEMON_UNKNOW_ERROR,
    IDE_DAEMON_INVALID_PARAM,
}ideError_t;


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

#ifdef __cplusplus
}
#endif

#endif //__IDE_DAEMON_API_H__
