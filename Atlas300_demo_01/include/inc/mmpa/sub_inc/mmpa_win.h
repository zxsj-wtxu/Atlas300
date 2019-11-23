/*
  *Copyright (C) 2018. Huawei Technologies Co., Ltd. All rights reserved.
 *
  * *This program is distributed in the hope that it will be useful,
  * *but WITHOUT ANY WARRANTY; without even the implied warranty of
  * *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
/*****************************************************************************/
/*                                                                          */
/*                                                                          */
/* FileName: mmpa_win.h                                                      */
/*                                                                          */
/* Author:                                                                   */
/*                                                                          */
/* Version: 1.0                                                              */
/*                                                                          */
/* Date: 2017-11                                                             */
/*                                                                          */
/* Description: implement message function                                   */
/*                                                                          */
/* Others:                                                                   */
/*                                                                          */
/* History:                                                                  */
/* 1. Date:                                                                  */
/*   Author:                                                                */
/*   Modification: Create this file                                         */
/*                                                                          */
/*****************************************************************************/

#ifndef MMPA_WIN_MMPA_WIN_H
#define MMPA_WIN_MMPA_WIN_H
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#ifdef MMPA_DLL
#define MMPA_DLL_API __declspec(dllexport)
#else
#define MMPA_DLL_API __declspec(dllimport)
#endif

MMPA_DLL_API extern char *optarg;
MMPA_DLL_API extern int opterr;
MMPA_DLL_API extern int optind;
MMPA_DLL_API extern int optopt;

#pragma section(".CRT$XCU",long,read)
#pragma section(".CRT$XPU",long,read)

typedef  HANDLE    mmMutex_t;
typedef  HANDLE    mmThread;
typedef  HANDLE    mmProcess;
typedef HANDLE mmPollHandle;
typedef HANDLE mmPipeHandle;
typedef HANDLE mmCompletionHandle;

typedef  CRITICAL_SECTION    mmMutexFC;
typedef  CONDITION_VARIABLE  mmCond;

typedef VOID *(*userProcFunc)(VOID *pulArg);
typedef struct{
    userProcFunc procFunc;
    VOID *pulArg;
}mmUserBlock_t;

typedef DWORD mmThreadKey;
typedef  SYSTEMTIME  mmSystemTime_t;

typedef HANDLE mmSem_t;
typedef SOCKET mmSockHandle;
typedef struct sockaddr mmSockAddr;
typedef int mmSocklen_t;
typedef int mmSemTimeout_t;
typedef long mmAtomicType;
typedef DWORD mmExitCode;
typedef int mmKey_t;
typedef HANDLE mmMsgid;

typedef enum {
    DT_DIR = FILE_ATTRIBUTE_DIRECTORY,
}mmDtype;

typedef struct{
    unsigned char d_type;
    char d_name[MAX_PATH]; /* file name (null-terminated) */
}mmDirent;

typedef int(*mmFilter)(const mmDirent *entry);
typedef int(*mmSort)(const mmDirent **a, const mmDirent **b);

typedef struct{
    VOID *sendBuf;
    INT32 sendLen;
}mmIovSegment;
typedef PVOID  mmInAddr;

typedef enum{
    pollTypeRead = 1,    // pipe read
    pollTypeRecv,        // socket recv
    pollTypeIoctl,       // ioctl
}mmPollType;

typedef struct{
    HANDLE completionHandle;
    mmPollType overlapType;
    OVERLAPPED oa;
}mmComPletionKey, *pmmComPletionKey;


typedef struct{
    VOID *priv;             // 用户自定义私有内容
    mmPollHandle bufHandle; // buf对应的handle的值
    mmPollType bufType;     // poll到的数据类型
    VOID *buf;
    UINT32 bufLen;
    UINT32 bufRes;
}mmPollData, *pmmPollData;

typedef VOID(*mmPollBack)(pmmPollData);
typedef struct{
    mmPollHandle handle;           // 需要poll的文件描述符或者句柄handle
    mmPollType pollType;           // 需要poll的操作类型，read or recv or ioctl
    INT32 ioctlCode;               // ioctl操作码，ioctl专用
    mmComPletionKey completionKey; // 默认为空就行，windows下会用到,用于接收区别是哪个handle的数据可读
}mmPollfd;

typedef struct{
    OVERLAPPED oa;
    HANDLE completionHandle;
    WSABUF DataBuf;
}PRE_IO_DATA, *PPRE_IO_DATA;

typedef OVERLAPPED mmOverLap;

typedef struct{
    UINT32 createFlag;
    INT32 oaFlag; // 非0则支持overlap操作
}mmCreateFlag;


typedef struct{
    VOID *inbuf;
    INT32 inbufLen;
    VOID *outbuf;
    INT32 outbufLen;
    mmOverLap *oa;
}mmIoctlBuf;


typedef struct{
    HANDLE  timerQueue;
    HANDLE  timerHandle;
} mmTimerHandle;

typedef struct{
    LONG  tv_sec;
    LONG  tv_usec;
}mmTimeval;

typedef struct  {
    INT32 tz_minuteswest; /* 和greenwich 时间差了多少分钟 */
    INT32 tz_dsttime;     /* type of DST correction */
}mmTimezone;

typedef struct{
    LONG  tv_sec;
    LONG  tv_nsec;
}mmTimespec;

typedef mmTimerHandle mmTimer;

#define mmTLS  __declspec(thread)

typedef struct stat mmStat_t;
typedef LPSECURITY_ATTRIBUTES mmMode_t;

typedef   int     MODE;

typedef struct {
    const char *name;
    int         has_arg;
    int        *flag;
    int         val;
}mmStructOption;

typedef struct
{
    ULONGLONG totalSize;
    ULONGLONG freeSize;
    ULONGLONG availSize;
}mmDiskSize;

#define M_FILE_RDONLY GENERIC_READ
#define M_FILE_WRONLY GENERIC_WRITE
#define M_FILE_RDWR (GENERIC_READ | GENERIC_WRITE)
#define M_FILE_CREAT OPEN_ALWAYS


#define M_RDONLY _O_RDONLY
#define M_WRONLY _O_WRONLY
#define M_RDWR _O_RDWR
#define M_CREAT _O_CREAT
#define M_BINARY _O_BINARY

#define   M_IREAD   _S_IREAD
#define   M_IRUSR   _S_IREAD
#define   M_IWRITE  _S_IWRITE
#define   M_IWUSR   _S_IWRITE

#define M_MSG_CREAT 1
#define M_MSG_EXCL 2
#define M_MSG_NOWAIT 3

#define M_WAIT_NOHANG 1
#define M_WAIT_UNTRACED 2

#define M_UMASK_USRREAD _S_IREAD
#define M_UMASK_GRPREAD _S_IREAD
#define M_UMASK_OTHREAD _S_IREAD

#define M_UMASK_USRWRITE _S_IWRITE
#define M_UMASK_GRPWRITE _S_IWRITE
#define M_UMASK_OTHWRITE _S_IWRITE

#define M_UMASK_USREXEC 0
#define M_UMASK_GRPEXEC 0
#define M_UMASK_OTHEXEC 0

typedef VOID (*mmPf)(VOID);
#define mmConstructor(x) __declspec(allocate(".CRT$XCU")) mmPf con = x
#define mmDestructor(x) __declspec(allocate(".CRT$XPU")) mmPf de = x

#define MMPA_PRINT_ERROR    ((opterr) && (*options != ':'))
#define MMPA_FLAG_PERMUTE   0x01    /* permute non-options to the end of argv */
#define MMPA_FLAG_ALLARGS   0x02    /* treat non-options as args to option "-1" */
#define MMPA_FLAG_LONGONLY  0x04    /* operate as getopt_long_only */
/* return values */
#define MMPA_BADCH      (INT32)'?'
#define MMPA_BADARG     ((*options == ':') ? (INT32)':' : (INT32)'?')
#define MMPA_INORDER    (INT32)1

#define MMPA_NO_ARGUMENT        0
#define MMPA_REQUIRED_ARGUMENT  1
#define MMPA_OPTIONAL_ARGUMENT  2

#define MMPA_EMSG       ""
#define MMPA_MAX_PATH MAX_PATH

_declspec(dllexport) INT32 mmCreateTask(mmThread *pstThreadHandle, mmUserBlock_t *pstFuncBlock);
_declspec(dllexport) INT32 mmJoinTask(mmThread *pstThreadHandle);
_declspec(dllexport) INT32 mmCondInit(mmCond *cv);
_declspec(dllexport) INT32 mmCondLockInit(mmMutexFC *cs);
_declspec(dllexport) INT32 mmCondLock(mmMutexFC *cs);
_declspec(dllexport) INT32 mmCondUnLock(mmMutexFC *cs);
_declspec(dllexport) INT32 mmCondLockDestroy(mmMutexFC *cs);
_declspec(dllexport) INT32 mmCondWait(mmCond *cv, mmMutexFC *cs);
_declspec(dllexport) INT32 mmCondTimedWait(mmCond *cv, mmMutexFC *cs, UINT32 mmMilliSeconds);

_declspec(dllexport) INT32 mmCondNotify(mmCond *cv);
_declspec(dllexport) INT32 mmCondNotifyAll(mmCond *cv);
_declspec(dllexport) INT32 mmCondDestroy(mmCond *cv);
_declspec(dllexport) INT32 mmMutexInit(mmMutex_t *mutex);
_declspec(dllexport) INT32 mmMutexLock(mmMutex_t *mutex);
_declspec(dllexport) INT32 mmMutexUnLock(mmMutex_t *mutex);
_declspec(dllexport) INT32 mmMutexDestroy(mmMutex_t *mutex);
_declspec(dllexport) INT32 mmGetPid(VOID);
_declspec(dllexport) INT32 mmGetTid(VOID);
_declspec(dllexport) INT32 mmGetPidHandle(mmProcess *pstProcessHandle);
_declspec(dllexport) INT32 mmGetLocalTime(mmSystemTime_t *st);
_declspec(dllexport) INT32 mmSemInit(mmSem_t *sem, UINT32 value);
_declspec(dllexport) INT32 mmSemWait(mmSem_t *sem);
_declspec(dllexport) INT32 mmSemPost(mmSem_t *sem);
_declspec(dllexport) INT32 mmSemDestroy(mmSem_t *sem);
_declspec(dllexport) INT32 mmOpen(const CHAR *pathName, INT32 flags);
_declspec(dllexport) INT32 mmOpen2(const CHAR *pathName, INT32 flags, MODE mode);
_declspec(dllexport) INT32 mmClose(INT32 fd);
_declspec(dllexport) INT32 mmWrite(INT32 fd, VOID *mmBuf, UINT32 mmCount);
_declspec(dllexport) INT32 mmRead(INT32 fd, VOID *mmBuf, UINT32 mmCount);
_declspec(dllexport) mmSockHandle mmSocket(INT32 sockFamily, INT32 type, INT32 protocol);
_declspec(dllexport) INT32 mmBind(mmSockHandle sockfd, mmSockAddr *addr, mmSocklen_t addrlen);
_declspec(dllexport) INT32 mmListen(mmSockHandle sockfd, INT32 backlog);
_declspec(dllexport) mmSockHandle mmAccept(mmSockHandle sockfd, mmSockAddr *addr, mmSocklen_t *addrlen);
_declspec(dllexport) INT32 mmConnect(mmSockHandle sockfd, mmSockAddr *addr, mmSocklen_t addrlen);
_declspec(dllexport) INT32 mmCloseSocket(mmSockHandle sockfd);
_declspec(dllexport) INT32 mmSocketRecv(mmSockHandle sockfd, VOID *pstRecvBuf, INT32 recvLen, INT32 recvFlag);
_declspec(dllexport) INT32 mmSocketSend(mmSockHandle sockfd, VOID *pstSendBuf, INT32 sendLen, INT32 sendFlag);
_declspec(dllexport) INT32 mmSAStartup(VOID);
_declspec(dllexport) INT32 mmSACleanup(VOID);
_declspec(dllexport) VOID *mmDlopen(const CHAR *fileName, INT mode);
_declspec(dllexport) VOID *mmDlsym(VOID *handle, CHAR *fileName);
_declspec(dllexport) INT32 mmDlclose(VOID *handle);
_declspec(dllexport) CHAR *mmDlerror(VOID);
_declspec(dllexport) INT32 mmCreateAndSetTimer(mmTimer *pstTimerHandle,
                                               mmUserBlock_t *pstTimerBlock,
                                               UINT mSecond,
                                               UINT period);
_declspec(dllexport) INT32 mmDeleteTimer(mmTimer stTimerHandle);
_declspec(dllexport) INT32 mmStatGet(const CHAR *path, mmStat_t *buffer);
_declspec(dllexport) INT32 mmMkdir(const CHAR *lpPathName, mmMode_t mode);
_declspec(dllexport) INT32 mmSleep(UINT32 millseconds);
_declspec(dllexport) INT32 mmCreateTaskWithAttr(mmThread *pstThreadHandle, mmUserBlock_t *pstFuncBlock);
_declspec(dllexport) INT32 mmGetProcessPrio(mmProcess pid);
_declspec(dllexport) INT32 mmSetProcessPrio(mmProcess pid,INT32 processPrio);
_declspec(dllexport) INT32 mmGetThreadPrio(mmThread *pstThreadHandle);
_declspec(dllexport) INT32 mmSetThreadPrio(mmThread *pstThreadHandle, INT32 threadPrio);
_declspec(dllexport) INT32 mmAccess(const CHAR *lpPathName);
_declspec(dllexport) INT32 mmRmdir(const CHAR *lpPathName);

_declspec(dllexport) INT32 mmIoctl(mmProcess fd, INT32 ioctlCode, mmIoctlBuf *bufPtr);
_declspec(dllexport) INT32 mmSemTimedWait(mmSem_t *sem ,int timeout);
_declspec(dllexport) INT32 mmWritev(mmSockHandle fd, mmIovSegment *iov, INT32 iovcnt);
_declspec(dllexport) VOID mmMb();
_declspec(dllexport) INT32 mmInetAton(const CHAR *addrStr, mmInAddr *addr);


_declspec(dllexport) mmProcess  mmOpenFile(const CHAR *fileName, UINT32 access, mmCreateFlag fileFlag);
_declspec(dllexport) INT32 mmReadFile(mmProcess fileId, VOID *buffer, INT32 len);
_declspec(dllexport) INT32 mmWriteFile(mmProcess fileId, VOID *buffer, INT32 len);
_declspec(dllexport) INT32 mmCloseFile(mmProcess fileId);

_declspec(dllexport) mmAtomicType mmSetData(mmAtomicType *ptr, mmAtomicType value);
_declspec(dllexport) mmAtomicType mmValueInc(mmAtomicType *ptr, mmAtomicType value);
_declspec(dllexport) mmAtomicType mmValueSub(mmAtomicType *ptr, mmAtomicType value);
_declspec(dllexport) INT32 mmCreateTaskWithDetach(mmThread *pstThreadHandle, mmUserBlock_t *pstFuncBlock);

/* 以下3个接口待删除 */
_declspec(dllexport) INT32 mmCreateNamedPipe(mmPipeHandle pipe[],CHAR  *pipeName[],INT32 waitMode);
_declspec(dllexport) INT32 mmOpenNamePipe(mmPipeHandle pipe[], CHAR *pipeName[], INT32 waitMode);
_declspec(dllexport) VOID mmCloseNamedPipe(mmPipeHandle namedPipe[]);

_declspec(dllexport) INT32 mmCreatePipe(mmPipeHandle pipe[], CHAR *pipeName[], UINT32 pipeCount,INT32 waitMode);
_declspec(dllexport) INT32 mmOpenPipe(mmPipeHandle pipe[], CHAR *pipeName[],UINT32 pipeCount, INT32 waitMode);
_declspec(dllexport) VOID mmClosePipe(mmPipeHandle pipe[],UINT32 pipeCount);

_declspec(dllexport) mmCompletionHandle mmCreateCompletionPort();
_declspec(dllexport) VOID mmCloseCompletionPort(mmCompletionHandle handle);
_declspec(dllexport) INT32 mmPoll(mmPollfd *fds,
                                  INT32 fdCount,
                                  INT32 timeout,
                                  mmCompletionHandle handleIOCP,
                                  pmmPollData pPolledData,
                                  mmPollBack pollBack);

_declspec(dllexport) INT32 mmGetErrorCode();
_declspec(dllexport) INT32 mmGetTimeOfDay(mmTimeval *tv,mmTimezone *tz);
_declspec(dllexport) mmTimespec mmGetTickCount();
_declspec(dllexport) INT32 mmGetRealPath(CHAR *path, CHAR *realPath);
_declspec(dllexport) INT32 mmRealPath(const CHAR *path,CHAR *realPath,INT32 realPathLen);


_declspec(dllexport) INT32 mmDup2(INT32 oldFd, INT32 newFd);
_declspec(dllexport) INT32 mmUnlink(const CHAR *filename);
_declspec(dllexport) INT32 mmChmod(const CHAR *filename, INT32 mode);
_declspec(dllexport) INT32 mmFileno(FILE *stream);
_declspec(dllexport) INT32 mmScandir(const CHAR *path, mmDirent ***entryList, mmFilter filterFunc, mmSort sort);
_declspec(dllexport) VOID mmScandirFree(mmDirent **entryList, INT32 count);

_declspec(dllexport) mmMsgid mmMsgCreate(mmKey_t key, INT32 msgFlag);
_declspec(dllexport) mmMsgid mmMsgOpen(mmKey_t key, INT32 msgFlag);
_declspec(dllexport) INT32 mmMsgRcv(mmMsgid msqid, VOID *buf, INT32 bufLen, INT32 msgFlag);
_declspec(dllexport) INT32 mmMsgSnd(mmMsgid msqid, VOID *buf, INT32 bufLen, INT32 msgFlag);

_declspec(dllexport) INT32 mmMsgClose(mmMsgid msqid);

_declspec(dllexport) INT32 mmLocalTimeR(const time_t *timep,struct tm *result);
_declspec(dllexport) INT32 mmGetOpt(int argc, char *const *argv, const char *opts);
_declspec(dllexport) INT32 mmGetOptLong(INT32 argc, CHAR * const * argv, const CHAR *opts,
                                        const mmStructOption *longopts, INT32 *longindex);

_declspec(dllexport) LONG mmLseek(INT32 fd, INT64 offset, INT32 seekFlag);
_declspec(dllexport) INT32 mmFtruncate(mmProcess fd, UINT32 length);

_declspec(dllexport) INT32 mmTlsCreate(mmThreadKey *key, VOID(*destructor)(VOID*));
_declspec(dllexport) INT32 mmTlsSet(mmThreadKey key, const VOID *value);
_declspec(dllexport) VOID *mmTlsGet(mmThreadKey key);
_declspec(dllexport) INT32 mmTlsDelete(mmThreadKey key);
_declspec(dllexport) INT32 mmGetOsType();

_declspec(dllexport) INT32 mmFsync(mmProcess fd);

_declspec(dllexport) INT32 mmChdir(const CHAR *path);
_declspec(dllexport) INT32 mmUmask(INT32 pmode);
_declspec(dllexport) INT32 mmWaitPid(mmProcess pid, int *status, int options);

_declspec(dllexport) INT32  mmGetCwd(CHAR *buffer, INT32 maxLen);
_declspec(dllexport) CHAR *mmStrTokR(CHAR *str, const CHAR *delim, CHAR **saveptr);

_declspec(dllexport) INT32 mmGetEnv(const CHAR* name, CHAR* value, UINT32 len);
_declspec(dllexport) INT32 mmSetEnv(const CHAR* name, const CHAR* value, INT32 overwrite);
_declspec(dllexport) CHAR *mmDirName(CHAR *path);
_declspec(dllexport) CHAR *mmBaseName(CHAR *path);
_declspec(dllexport) INT32 mmGetDiskFreeSpace(const char* path,mmDiskSize *diskSize);

_declspec(dllexport) INT32 mmSetThreadName(mmThread *pstThreadHandle, const CHAR* name);
_declspec(dllexport) INT32 mmGetThreadName(mmThread *pstThreadHandle, CHAR* name,INT32 size);

#ifdef __cplusplus
#if __cplusplus
}
#endif / *__cpluscplus */
#endif / *__cpluscplus */

#endif / *_MMPA_WIN_MMPA_WIN_H_ */

