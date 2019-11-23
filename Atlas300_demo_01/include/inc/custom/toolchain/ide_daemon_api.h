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
    IDE_DAEMON_NONE_ERROR,
    IDE_DAEMON_UNKNOW_ERROR,
    IDE_DAEMON_INVALID_PARAM,
}ideError_t;


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

#ifdef __cplusplus
}
#endif

#endif //__IDE_DAEMON_API_H__
