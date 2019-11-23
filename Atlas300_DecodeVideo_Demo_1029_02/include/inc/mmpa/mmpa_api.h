/*
 * Copyright (C) 2018. Huawei Technologies Co., Ltd. All rights reserved.
 *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
/*****************************************************************************/

#ifndef _MMPA_API_H_
#define _MMPA_API_H_


#define  LINUX    0
#define  WIN      1

#if(OS_TYPE == LINUX) //lint !e553
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <dlfcn.h>
#include <signal.h>
#include <pthread.h>
#include <sys/resource.h>
#include <syslog.h>
#include <dirent.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <net/if.h>
#include <sys/uio.h>
#include <sys/mman.h>
#include <linux/types.h>
#include <linux/hdreg.h>
#include <linux/fs.h>
#include <linux/limits.h>
#include <sys/shm.h>
#include <sys/un.h>
#include <stdarg.h>
#include <limits.h>
#include <sys/utsname.h>
#include <ctype.h>
#include <stddef.h>
#include <dirent.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "./sub_inc/mmpa_typedef_linux.h"
#include "./sub_inc/mmpa_linux.h"
#include "securec.h"
#include <sys/msg.h>
#include <getopt.h>
#include <sys/wait.h>
#include <sys/statvfs.h>
#include <libgen.h>

#endif


#if(OS_TYPE == WIN) //lint !e553
#include <winsock2.h>
#include <winsock.h>
#include "Windows.h"
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <stdio.h>
#include <ws2tcpip.h>
#include <winioctl.h>
#include <WinBase.h>
#include <mswsock.h>
#include <strsafe.h>
#include <signal.h>
#include <time.h>
#include <stdarg.h>
#include "shlwapi.h"
#include <direct.h>
#include "sub_inc/mmpa_typedef_win.h"
#include "sub_inc/mmpa_win.h"

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"mswsock.lib")
#pragma comment(lib,"Kernel32.lib")
#pragma comment(lib,"shlwapi.lib")
#endif

#endif /* _MMPA_API_H_ */

