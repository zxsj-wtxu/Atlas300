/*
 * Copyright (C) 2018. Huawei Technologies Co., Ltd. All rights reserved.
 *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
/*****************************************************************************/
/*                                                                           */
/*                                                                           */
/* FileName: mmpa_linux.h                                                    */
/*                                                                           */
/* Author:                                                                   */
/*                                                                           */
/* Version: 1.0                                                              */
/*                                                                           */
/* Date: 2017-11                                                             */
/*                                                                           */
/* Description: implement message function                                   */
/*                                                                           */
/* Others:                                                                   */
/*                                                                           */
/* History:                                                                  */
/* 1. Date:                                                                  */
/*    Author:                                                                */
/*    Modification: Create this file                                         */
/*                                                                           */
/*****************************************************************************/

#ifndef MMPA_LINUX_MMPA_LINUX_H
#define MMPA_LINUX_MMPA_LINUX_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

typedef pthread_t              mmThread;
typedef pthread_mutex_t        mmMutex_t;
typedef pthread_cond_t         mmCond;
typedef pthread_mutex_t        mmMutexFC;
typedef signed int             mmProcess;
typedef int mmPollHandle;
typedef int mmPipeHandle;
typedef int mmComPletionKey;
typedef int mmCompletionHandle;

typedef VOID *mmExitCode;
typedef key_t mmKey_t;
typedef int mmMsgid;
typedef struct dirent mmDirent;
typedef int (*mmFilter)(const mmDirent *entry);
typedef int (*mmSort)(const mmDirent **a,const mmDirent **b);

typedef VOID *(*userProcFunc)(VOID *pulArg);

typedef struct{
    userProcFunc procFunc;
    VOID *pulArg;
}mmUserBlock_t;


typedef struct{
    int         wSecond;	 /* Seconds. [0-60] (1 leap second) */
    int         wMinute;	 /* Minutes. [0-59] */
    int         wHour;      /* Hours. [0-23] */
    int         wDay;       /* Day. [1-31] */
    int         wMonth;     /* Month. [1-12] */
    int         wYear;      /* Year */
    int         wDayOfWeek; /* Day of week. [0-6] */
    int         tm_yday;    /* Days in year.[0-365] */
    int         tm_isdst;   /* DST. [-1/0/1] */
    long int    wMilliseconds; /* milliseconds */
}mmSystemTime_t;

typedef sem_t  mmSem_t;
typedef struct sockaddr mmSockAddr;
typedef socklen_t mmSocklen_t;
typedef int mmSockHandle;
typedef timer_t  mmTimer;
typedef pthread_key_t mmThreadKey;

typedef int mmOverLap;

typedef struct{
    UINT32 createFlag;
    INT32 oaFlag;
}mmCreateFlag;

typedef struct{
    VOID *inbuf;
    INT32 inbufLen;
    VOID *outbuf;
    INT32 outbufLen;
    mmOverLap *oa;
}mmIoctlBuf;

typedef struct{
    VOID *sendBuf;
    INT32 sendLen;
}mmIovSegment;
typedef struct in_addr mmInAddr;

typedef int mmAtomicType;

typedef enum{
    pollTypeRead = 1,   // pipe read
    pollTypeRecv,       // socket recv
    pollTypeIoctl,      // ioctl
}mmPollType;

typedef struct{
    VOID *priv;                 // 用户自定义私有内容
    mmPollHandle bufHandle;     // buf对应的handle的值
    mmPollType bufType;         // poll到的数据类型
    VOID *buf;                  // poll到的数据
    UINT32 bufLen;              // poll到的数据长度
    UINT32 bufRes;              // 实际返回长度
}mmPollData, *pmmPollData;

typedef VOID(*mmPollBack)(pmmPollData);
typedef struct{
    mmPollHandle handle;            // 需要poll的文件描述符或者句柄handle
    mmPollType pollType;            // 需要poll的操作类型，
                                    // read or recv or ioctl
    INT32 ioctlCode;                // ioctl操作码，ioctl专用
    mmComPletionKey completionKey;  // 默认为空就行，windows下会用到，
                                    // 用于接收区别是哪个handle的数据可读
}mmPollfd;

typedef struct{
    LONG  tv_sec;
    LONG  tv_usec;
}mmTimeval;

typedef struct{
    INT32 tz_minuteswest;   /* 和greenwich 时间差了多少分钟 */
    INT32 tz_dsttime;       /* type of DST correction */
}mmTimezone;

typedef struct{
    LONG  tv_sec;
    LONG  tv_nsec;
}mmTimespec;

typedef struct{
    ULONGLONG totalSize;
    ULONGLONG freeSize;
    ULONGLONG availSize;
}mmDiskSize;

#define mmTLS __thread
typedef struct stat mmStat_t;
typedef mode_t mmMode_t;

typedef mode_t     MODE;

typedef struct option mmStructOption;

#ifdef __ANDROID__
#define S_IREAD S_IRUSR
#define S_IWRITE S_IWUSR
#endif

#define M_FILE_RDONLY O_RDONLY
#define M_FILE_WRONLY O_WRONLY
#define M_FILE_RDWR O_RDWR
#define M_FILE_CREAT O_CREAT


#define M_RDONLY O_RDONLY
#define M_WRONLY O_WRONLY
#define M_RDWR O_RDWR
#define M_CREAT O_CREAT
#define M_BINARY O_RDONLY

#define M_IREAD   S_IREAD
#define M_IRUSR   S_IRUSR
#define M_IWRITE  S_IWRITE
#define M_IWUSR   S_IWUSR
#define FDSIZE    64
#define M_MSG_CREAT IPC_CREAT
#define M_MSG_EXCL (IPC_CREAT | IPC_EXCL)
#define M_MSG_NOWAIT IPC_NOWAIT

#define M_WAIT_NOHANG WNOHANG       // 不阻塞等待
#define M_WAIT_UNTRACED WUNTRACED   // 若子进程进入暂停状态，则马上返回，
                                    // 但子进程的结束状态不予以理会
#define M_UMASK_USRREAD S_IRUSR
#define M_UMASK_GRPREAD S_IRGRP
#define M_UMASK_OTHREAD S_IROTH

#define M_UMASK_USRWRITE S_IWUSR
#define M_UMASK_GRPWRITE S_IWGRP
#define M_UMASK_OTHWRITE S_IWOTH

#define M_UMASK_USREXEC S_IXUSR
#define M_UMASK_GRPEXEC S_IXGRP
#define M_UMASK_OTHEXEC S_IXOTH

#define mmConstructor(x) __attribute__((constructor)) VOID x()
#define mmDestructor(x) __attribute__((destructor)) VOID x()

#define MMPA_MAX_PATH PATH_MAX

extern INT32 mmCreateTask(mmThread *pstThreadHandle,
                          mmUserBlock_t *pstFuncBlock);
extern INT32 mmJoinTask(mmThread *pstThreadHandle);
extern INT32 mmMutexInit(mmMutex_t *mutex);
extern INT32 mmMutexLock(mmMutex_t *mutex);
extern INT32 mmMutexUnLock(mmMutex_t *mutex);
extern INT32 mmMutexDestory(mmMutex_t *mutex);
extern INT32 mmCondInit(mmCond *cv);
extern INT32 mmCondLockInit(mmMutexFC *cs);
extern INT32 mmCondLock(mmMutexFC *cs);
extern INT32 mmCondUnLock(mmMutexFC *cs);
extern INT32 mmCondLockDestroy(mmMutexFC *cs);
extern INT32 mmCondWait(mmCond *cv, mmMutexFC *cs);
extern INT32 mmCondTimedWait(mmCond *cv, mmMutexFC *cs, UINT32 mmMilliSeconds);
extern INT32 mmCondNotify(mmCond *cv);
extern INT32 mmCondNotifyAll(mmCond *cv);
extern INT32 mmCondDestroy(mmCond *cv);
extern INT32 mmGetPid();
extern INT32 mmGetTid();
extern INT32 mmGetPidHandle(mmProcess *pstProcessHandle);
extern INT32 mmGetLocalTime(mmSystemTime_t *st);

extern INT32 mmSemInit(mmSem_t *sem, UINT32 value);
extern INT32 mmSemWait(mmSem_t *sem);
extern INT32 mmSemPost(mmSem_t *sem);
extern INT32 mmSemDestroy(mmSem_t *sem);
extern INT32 mmOpen(const CHAR *pathName, INT32 flags);
extern INT32 mmOpen2(const CHAR *pathName, INT32 flags, MODE mode);
extern INT32 mmClose(INT32 fd);
extern INT32 mmWrite (INT32 fd, VOID *mmBuf, UINT32 mmCount);
extern INT32 mmRead (INT32 fd, VOID *mmBuf, UINT32 mmCount);
extern mmSockHandle mmSocket(INT32 sockFamily, INT32 type, INT32 protocol);
extern INT32 mmBind(mmSockHandle sockfd,
                    mmSockAddr *addr,
                    mmSocklen_t addrlen);
extern INT32 mmListen(mmSockHandle sockfd, INT32 backlog);
extern mmSockHandle mmAccept (mmSockHandle sockfd,
                              mmSockAddr *addr,
                              mmSocklen_t *addrlen);
extern INT32 mmConnect(mmSockHandle sockfd,
                       mmSockAddr *addr,
                       mmSocklen_t addrlen);
extern INT32 mmCloseSocket(mmSockHandle sockfd);
extern INT32 mmSAStartup();
extern INT32 mmSACleanup();
extern VOID* mmDlopen(const CHAR *fileName,INT mode);
extern VOID* mmDlsym(VOID *handle, CHAR *funcName);
extern INT32 mmDlclose(VOID *handle);
extern CHAR* mmDlerror();
extern INT32 mmCreateAndSetTimer(mmTimer *pstTimerHandle,
                                mmUserBlock_t *pstTimerBlock,
                                UINT mSecond,
                                UINT period);
extern INT32 mmDeleteTimer(mmTimer stTimerHandle);
extern INT32 mmStatGet(const CHAR *path,  mmStat_t *buffer);
extern INT32 mmMkdir(const CHAR *lpPathName, mmMode_t mode);
extern INT32 mmSleep(UINT32 millseconds);
extern INT32 mmMutexDestroy(mmMutex_t *mutex);
extern INT32 mmSocketSend(mmSockHandle sockfd,
                          VOID *pstSendBuf,
                          INT32 sendLen,
                          INT32 sendFlag);

extern INT32 mmSocketRecv(mmSockHandle sockfd,
                          VOID *pstRecvBuf,
                          INT32 recvLen,
                          INT32 recvFlag);

extern INT32 mmCreateTaskWithAttr(mmThread *pstThreadHandle,
                                  mmUserBlock_t *pstFuncBlock);
extern INT32 mmGetProcessPrio(mmProcess pid);
extern INT32 mmSetProcessPrio(mmProcess pid,INT32 processPrio);
extern INT32 mmGetThreadPrio(mmThread *pstThreadHandle);
extern INT32 mmSetThreadPrio(mmThread *pstThreadHandle, INT32 threadPrio);
extern INT32 mmAccess(const CHAR *lpPathName);
extern INT32 mmRmdir(const CHAR *lpPathName);

extern INT32 mmIoctl(mmProcess fd,INT32 ioctlCode,mmIoctlBuf *bufPtr);
extern INT32 mmSemTimedWait(mmSem_t *sem ,int timeout);
extern INT32 mmWritev(mmProcess fd,mmIovSegment *iov,INT32 iovcnt);
extern VOID mmMb();
extern INT32 mmInetAton(const CHAR *addrStr, mmInAddr *addr);


extern mmProcess  mmOpenFile(const CHAR *fileName,
                             UINT32 access,
                             mmCreateFlag fileFlag);
extern INT32 mmReadFile(mmProcess fileId, VOID *buffer,INT32 len);
extern INT32 mmWriteFile(mmProcess fileId, VOID *buffer,INT32 len);
extern INT32 mmCloseFile(mmProcess fileId);

extern mmAtomicType mmSetData(mmAtomicType *ptr, mmAtomicType value);
extern mmAtomicType mmValueInc(mmAtomicType *ptr, mmAtomicType value);
extern mmAtomicType mmValueSub(mmAtomicType *ptr, mmAtomicType value);
extern INT32 mmCreateTaskWithDetach(mmThread *pstThreadHandle,
                                    mmUserBlock_t *pstFuncBlock);

/* 以下3个接口待删除 */
extern INT32 mmCreateNamedPipe(mmPipeHandle pipe[], CHAR *pipeName[],INT32 waitMode);
extern INT32 mmOpenNamePipe(mmPipeHandle pipe[],CHAR *pipeName[],INT32 waitMode);
extern VOID mmCloseNamedPipe(mmPipeHandle namedPipe[]);

extern INT32 mmCreatePipe(mmPipeHandle pipe[], CHAR *pipeName[], UINT32 pipeCount,INT32 waitMode);
extern INT32 mmOpenPipe(mmPipeHandle pipe[], CHAR *pipeName[],UINT32 pipeCount, INT32 waitMode);
extern VOID mmClosePipe(mmPipeHandle pipe[],UINT32 pipeCount);



// poll相关接口
extern mmCompletionHandle mmCreateCompletionPort();
extern VOID mmCloseCompletionPort(mmCompletionHandle handle);
extern INT32 mmPoll(mmPollfd *fds,
                    INT32 fdCount,
                    INT32 timeout,
                    mmCompletionHandle handleIOCP,
                    pmmPollData pPolledData,
                    mmPollBack pollBack);
extern INT32 mmGetErrorCode();
extern INT32 mmGetTimeOfDay(mmTimeval *tv,mmTimezone *tz);
extern mmTimespec mmGetTickCount();
extern INT32 mmGetRealPath(CHAR *path, CHAR *realPath); // 待删除
extern INT32 mmRealPath(const CHAR *path, CHAR *realPath, INT32 realPathLen);

extern INT32 mmDup2(INT32 oldFd,INT32 newFd);

extern INT32 mmUnlink(const CHAR *filename);

extern INT32 mmChmod(const CHAR *filename,INT32 mode);

extern INT32 mmFileno(FILE *stream);

extern INT32 mmScandir(const CHAR *path,
                       mmDirent ***entry_list,
                       mmFilter filterFunc,
                       mmSort sort);

extern VOID mmScandirFree(mmDirent **entry_list,INT32 count);

extern mmMsgid mmMsgCreate(mmKey_t key, INT32 msgFlag);

extern mmMsgid mmMsgOpen(mmKey_t key,INT32 msgFlag);

extern INT32 mmMsgSnd(mmMsgid msqid,VOID *buf,INT32 bufLen,INT32 msgFlag);

extern INT32 mmMsgRcv(mmMsgid msqid,VOID *buf,INT32 bufLen,INT32 msgFlag);

extern INT32 mmMsgClose(mmMsgid msqid);

extern INT32 mmLocalTimeR(const time_t *timep, struct tm *result);

extern INT32 mmGetOpt(int argc, char * const * argv, const char * opts);
extern INT32 mmGetOptLong(int argc, char * const * argv, const char *opts,
                   const mmStructOption *longOpts, int *longIndex);

extern LONG  mmLseek(INT32 fd,INT64 offset,INT32 seekFlag);
extern INT32 mmFtruncate(mmProcess fd,UINT32 length);

extern INT32 mmTlsCreate(mmThreadKey *key, VOID (*destructor)(VOID*));
extern INT32 mmTlsSet(mmThreadKey key, const VOID *value);
extern VOID* mmTlsGet(mmThreadKey key);
extern INT32 mmTlsDelete(mmThreadKey key);
extern INT32 mmGetOsType();

extern INT32 mmFsync(mmProcess fd);
extern INT32 mmChdir(const CHAR *path);
extern INT32 mmUmask(INT32 pmode);
extern INT32 mmThreadKill(mmThread id);
extern INT32 mmWaitPid(mmProcess pid,int *status,int options);

extern INT32 mmGetCwd(CHAR *buffer, INT32 maxLen);
extern INT32 mmGetEnv(const CHAR *name,CHAR *value, UINT32 len);
extern INT32 mmSetEnv(const CHAR *name,const CHAR *value,INT32 overwrite);
extern CHAR *mmStrTokR(CHAR *str, const CHAR *delim, CHAR **saveptr);
extern CHAR* mmDirName(CHAR *path);
extern CHAR* mmBaseName(CHAR *path);
extern INT32 mmGetDiskFreeSpace(const char* path,mmDiskSize *diskSize);

/*
 * 功能:设置mmCreateTask创建的线程名
 * 输入:pstThreadHandle:线程ID
 *      name:线程名,name实际长度必须<MMPA_THREADNAME_SIZE
 * 入参错误返回EN_INVALID_PARAM,执行成功返回EN_OK，执行失败返回EN_ERROR
 */
 extern INT32 mmSetThreadName(mmThread *pstThreadHandle, const CHAR* name);

/*
 * 功能:获取线程名
 * 输入:pstThreadHandle:线程ID
 *      size:存放线程名的缓存长度
 * 输出:name:线程名由用户分配缓存,缓存长度必须>=MMPA_THREADNAME_SIZE
 * 入参错误返回EN_INVALID_PARAM,执行成功返回EN_OK，执行失败返回EN_ERROR
 */
 extern INT32 mmGetThreadName(mmThread *pstThreadHandle, CHAR* name,INT32 size);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* _MMPA_LINUX_MMPA_LINUX_H_ */

