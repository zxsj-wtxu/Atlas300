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
    char ip[IDE_DAEMON_IP_LEN];     //IP��ַ
    int port;                       //�˿ں�
    int deviceID;                   //�豸ID��
}connInfo_t;

typedef enum tagIdeError
{
    IDE_DAEMON_NONE_ERROR = 0,                    //�޴���
    IDE_DAEMON_UNKNOW_ERROR = 1,                  //δ֪����
    IDE_DAEMON_WRITE_ERROR = 2,                   //д��ʧ��
    IDE_DAEMON_NO_SPACE_ERROR = 3,                //��������
    IDE_DAEMON_INVALID_PATH_ERROR = 4,            //��Ч·��
    IDE_DAEMON_INVALID_PARAM_ERROR = 5,           //��Ч����
    IDE_DAEMON_TCP_CONNECT_ERROR = 6,             //TCP����ʧ��
    IDE_DAEMON_TCP_CHANNEL_ERROR = 7,             //TCPͨ���쳣
    IDE_DAEMON_MALLOC_ERROR = 8,                  //������ڴ�ʧ��
    IDE_DAEMON_HDC_CHANNEL_ERROR = 9,             //HDCͨ·�쳣
    IDE_DAEMON_CHANNEL_ERROR = 10,                //ͨ·�쳣
    IDE_DAEMON_MKDIR_ERROR = 11,                  //����Ŀ¼ʧ��
    IDE_DAEMON_MEMCPY_ERROR = 12,                 //�ڴ濽��ʧ��
    IDE_DAEMON_MEMSET_ERROR = 13,                 //�ڴ�����ʧ��
    IDE_DAEMON_INVALID_IP_ERROR = 14,             //��Ч��IP��ַ
    IDE_DAEMON_INTERGER_REVERSED_ERROR = 15,      //�������
    IDE_DAEMON_DUMP_QUEUE_FULL = 16,              //dump��������
    NR_IDE_DAEMON_ERROR,                          //ö�����ֵ
}ideError_t;

typedef ideError_t IdeErrorT;

enum IdeDumpFlag
{
    IDE_DUMP_NONE_FLAG  = 0,            //�ޱ�־λ
};

struct IdeDumpChunk
{
    char                *fileName;      //�ļ���������·��
    unsigned char       *dataBuf;       //д�������Buffer
    unsigned int        bufLen;         //д�������Buffer����
    unsigned int        isLastChunk;    //�Ƿ����һ������   0:�����һ������;1�����һ������
    long long           offset;         //�ļ�д���ƫ��λ   -1Ϊ׷����ʽд��
    enum IdeDumpFlag    flag;           //��־λ
};

/*****************************************************************************
 �� �� ��  : ideOpenFile
 ��������  : ���ļ�
 �������  : connInfo_t *connInfo       ������Ϣ���ɴ��գ���Ĭ������
             cosnt char *fileName       �Զ�IDE������ļ�������Ҫ����·��
 �������  : ��
 �� �� ֵ  : �Ự���
             NULL: �����Ựʧ��
             �ǿ�: �����Ự�ɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2018��08��30��
    �޸�����   : �����ɺ���
*****************************************************************************/
IDE_SESSION ideOpenFile(connInfo_t *connInfo, const char *fileName);

/*****************************************************************************
 �� �� ��  : ideWriteFile
 ��������  : ����Զ��д�ļ�����
 �������  : IDE_SESSION session: �Ự���
             void *data:    ��Ҫд�����ݻ���
             uint32_t len:  ���ݳ���
 �������  : ��
 �� �� ֵ  : IDE_DAEMON_NONE_ERROR:     д���ݳɹ�
             IDE_DAEMON_UNKNOW_ERROR:   д����ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2018��08��30��
    �޸�����   : �����ɺ���
*****************************************************************************/
ideError_t ideWriteFile(IDE_SESSION session, const void *data, uint32_t len);

/*****************************************************************************
 �� �� ��  : ideCloseFile
 ��������  : ����д�ļ�
 �������  : IDE_SESSION session: �Ự���
 �������  : ��
 �� �� ֵ  : IDE_DAEMON_NONE_ERROR:     �ر��ļ��ɹ�
             IDE_DAEMON_UNKNOW_ERROR:   �ر��ļ�ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2018��08��30��
    �޸�����   : �����ɺ���
*****************************************************************************/
ideError_t ideCloseFile(IDE_SESSION session);

/*****************************************************************************
 �� �� ��  : IdeDumpStart
 ��������  : ����Dumpͨ·
 �������  : const char *privInfo  ˽�����ݣ���ʽhost:port;device_id������MindStudio���룬�ɴ���NULL��ʹ��Ĭ������
 �������  : ��
 �� �� ֵ  : �Ự���
             NULL: �����Ựʧ��
             �ǿ�: �����Ự�ɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2019��03��14��
    �޸�����   : �����ɺ���
*****************************************************************************/
extern IDE_SESSION IdeDumpStart(const char *privInfo);

/*****************************************************************************
 �� �� ��  : IdeDumpData
 ��������  : ��������Dump,Dump����������̺󷵻�
 �������  : IDE_SESSION session: �Ự���
             struct IdeDumpChunk *dumpChunk: Dump�����ݽṹ��
 �������  : ��
 �� �� ֵ  : IDE_DAEMON_NONE_ERROR:           д���ݳɹ�
             IDE_DAEMON_INVALID_PARAM_ERROR   �Ƿ�����
             IDE_DAEMON_UNKNOW_ERROR:         д����ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2019��03��14��
    �޸�����   : �����ɺ���
*****************************************************************************/
extern IdeErrorT IdeDumpData(IDE_SESSION session, const struct IdeDumpChunk *dumpChunk);

/*****************************************************************************
 �� �� ��  : IdeDumpEnd
 ��������  : �ر�Dumpͨ·
 �������  : IDE_SESSION session: �Ự���
 �������  : ��
 �� �� ֵ  : IDE_DAEMON_NONE_ERROR:          �رջỰ�ɹ�
             IDE_DAEMON_INVALID_PARAM_ERROR  �Ƿ�����
             IDE_DAEMON_UNKNOW_ERROR:        �رջỰʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2019��03��14��
    �޸�����   : �����ɺ���
*****************************************************************************/
extern IdeErrorT IdeDumpEnd(IDE_SESSION session);

#ifdef __cplusplus
}
#endif

#endif //__IDE_DAEMON_API_H__
