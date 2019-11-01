/*
 * Copyright (C) 2018. Huawei Technologies Co., Ltd. All rights reserved.
 *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
/*****************************************************************************/
#ifndef MMPA_TYPEDEF_WIN_H
#define MMPA_TYPEDEF_WIN_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#ifndef FALSE
#define FALSE                     0
#endif

#ifndef TRUE
#define TRUE                      1
#endif

#define EN_OK                   0
#define EN_ERR                  1
#define EN_ERROR                (-1)
#define EN_INVALID_PARAM         (-2)

#define HANDLE_INVALID_VALUE    (-1)
#define INVALID_SOCKET_HANDLE   INVALID_SOCKET
#define MMPA_MEM_MAX_LEN       (0x7fffffff)
#define MMPA_PROCESS_ERROR     (0x7fffffff)

#define MMPA_ONE_THOUSAND         1000
#define MMPA_COMPUTER_BEGIN_YEAR  1900
#define SUMMER_TIME_OR_NOT         (-1)
#define MMPA_ZERO                   0
#define MMPA_VALUE_ONE              1
#define MMPA_SOCKET_MAIN_EDITION    2
#define MMPA_SOCKET_SECOND_EDITION  0
#define MMPA_PIPE_BUF_SIZE       1024
#define MMPA_MAX_SCANDIR_COUNT  1024
#define MAX_IOVEC_SIZE              32
#define MMPA_PIPE_COUNT           2
#define MMPA_THREADNAME_SIZE      16

#define MMPA_MAX_NI               19
#define MMPA_MIDDLE_NI               5
#define MMPA_LOW_NI               (-5)
#define MMPA_MIN_NI               (-20)

#define MMPA_MAX_THREAD_PIO       99
#define MMPA_MIDDLE_THREAD_PIO       66
#define MMPA_LOW_THREAD_PIO       33
#define MMPA_MIN_THREAD_PIO       1



#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#endif /* _MMPA_TYPEDEF_WIN_H_ */

