

#ifndef _D_SYSLOG_H
#define _D_SYSLOG_H 1

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _DPATH_LOG
#undef _DPATH_LOG
#endif

#ifndef LINUX
#define LINUX 0
#endif

#ifndef OS_TYPE
#define OS_TYPE 0
#endif

#if(OS_TYPE == LINUX)
#define _DPATH_LOG	"/dev/slog"
#define MSG_LENGTH 1024
#define PATH_DLOG_DIR "/var/log/slog"
#define GLOBALLEVEL_KEY "global_level"
#else
#define _DPATH_LOG	"C:\\Program Files\\Huawei\\HiAI Foundation\\log"
#define MSG_LENGTH 1024
#define PATH_DLOG_DIR "C:\\Program Files\\Huawei\\HiAI Foundation\\log"
#define GLOBALLEVEL_KEY "global_level"
#endif

#define	DLOG_NULL       (-1)		// don't output log
#define	DLOG_ERROR      0		// error conditions
#define	DLOG_WARN       1		// warning conditions
#define	DLOG_INFO       2		// informational
#define	DLOG_DEBUG      3		// debug-level messages
#define DLOG_EVENT      0x10    // event
#define ENABLEEVENT_KEY   "enableEvent"

typedef struct _dcode {
	const char	*c_name;
	int	c_val;
}DCODE;

// moudle id
enum {
    DLOG = 0,                               // Dlog
    SLOG,                                   // Slog
    IDEDD,                                  // IDE daemon device
    IDEDH,                                  // IDE daemon host
    LOGAGTH,                                // log agent host
    HCCL,                                   // HCCL
    FMK,                                    // Framework
    HIAIENGINE,                             // Matrix
    DVPP,                                   // DVPP
    RUNTIME,                                // Runtime
    CCE,                                    // CCE
#if(OS_TYPE == LINUX)
    HDC,                                   // HDC
#else
    HDCL,                                   // HDCL  windows has a def with the same name HDC, so change HDC to HDCL
#endif
    DRV,                                    // Driver
    MDCCONTROL,                             // Mdc control
    MDCFUSION,                              // Mdc fusion
    MDCLOCATION,                            // Mdc location
    MDCPERCEPTION,                          // Mdc perception
    MDCMOP,
    MDCFSM,
    MDCCOMMON,
    MDCMONITOR,
    MDCBSWP,                                // MDC basesoftware platform
    MDCDEFAULT,                             // MDC UNDEFINE
    MDCSC,                                  // MDC spatial cognition
    MDCBP,
    MDCTF,
    MLL,
    DEVMM,                                  // Dlog memory managent
    KERNEL,                                 // Kernel
    MDCSMCMD,                               // sm_control_cmd
    MDCSCREEN,                              // parking_spot_screen
    LIBMEDIA,                               // Libmedia
    CCECPU,                                 // ai cpu
    ASCENDDK,                                //AscendDK
    ROS,                                    //ROS
    INVLID_MOUDLE_ID
};

#define MAX_MOD_NAME_LEN (32)

#ifdef __MODULE_ID_NAME__
#define SET_MOUDLE_ID_MAP_NAME(x) { #x, x}

static DCODE moduleIdName[] = 
{
	SET_MOUDLE_ID_MAP_NAME(DLOG),
	SET_MOUDLE_ID_MAP_NAME(SLOG),
	SET_MOUDLE_ID_MAP_NAME(IDEDD),
	SET_MOUDLE_ID_MAP_NAME(IDEDH),
	SET_MOUDLE_ID_MAP_NAME(LOGAGTH),
	SET_MOUDLE_ID_MAP_NAME(HCCL),
	SET_MOUDLE_ID_MAP_NAME(FMK),
	SET_MOUDLE_ID_MAP_NAME(HIAIENGINE),
	SET_MOUDLE_ID_MAP_NAME(DVPP),
	SET_MOUDLE_ID_MAP_NAME(RUNTIME),
	SET_MOUDLE_ID_MAP_NAME(CCE),
#if(OS_TYPE == LINUX)
	SET_MOUDLE_ID_MAP_NAME(HDC),
#else
	SET_MOUDLE_ID_MAP_NAME(HDCL),
#endif
	SET_MOUDLE_ID_MAP_NAME(DRV),
	SET_MOUDLE_ID_MAP_NAME(MDCCONTROL),
	SET_MOUDLE_ID_MAP_NAME(MDCFUSION),
	SET_MOUDLE_ID_MAP_NAME(MDCLOCATION),
	SET_MOUDLE_ID_MAP_NAME(MDCPERCEPTION),
	SET_MOUDLE_ID_MAP_NAME(MDCMOP),
	SET_MOUDLE_ID_MAP_NAME(MDCFSM),
	SET_MOUDLE_ID_MAP_NAME(MDCCOMMON),
	SET_MOUDLE_ID_MAP_NAME(MDCMONITOR),
	SET_MOUDLE_ID_MAP_NAME(MDCBSWP),
	SET_MOUDLE_ID_MAP_NAME(MDCDEFAULT),
	SET_MOUDLE_ID_MAP_NAME(MDCSC),
	SET_MOUDLE_ID_MAP_NAME(MDCBP),
	SET_MOUDLE_ID_MAP_NAME(MDCTF),
	SET_MOUDLE_ID_MAP_NAME(MLL),
	SET_MOUDLE_ID_MAP_NAME(DEVMM),
	SET_MOUDLE_ID_MAP_NAME(KERNEL),
	SET_MOUDLE_ID_MAP_NAME(MDCSMCMD),
	SET_MOUDLE_ID_MAP_NAME(MDCSCREEN),
	SET_MOUDLE_ID_MAP_NAME(LIBMEDIA),
	SET_MOUDLE_ID_MAP_NAME(CCECPU),
	SET_MOUDLE_ID_MAP_NAME(ASCENDDK),
	SET_MOUDLE_ID_MAP_NAME(ROS),
	{NULL, -1}
};

static DCODE _levelName[] = {
	{"NULL",      DLOG_NULL},
	{"ERROR",     DLOG_ERROR},
	{"WARNING",   DLOG_WARN},
	{"INFO",      DLOG_INFO},
	{"DEBUG",     DLOG_DEBUG},
	{"EVENT",     DLOG_EVENT},
	{NULL, -1}
};
#endif

#if(OS_TYPE == LINUX)
extern void dlog_error(int module_id, const char *fmt, ...);
extern void dlog_warn(int module_id, const char *fmt, ...);
extern void dlog_info(int module_id, const char *fmt, ...);
extern void dlog_debug(int module_id, const char *fmt, ...);
extern void dlog_event(int module_id, const char *fmt, ...);
extern void dlog_init(void);

/*****************************************************************************
Prototype      : dlog_getlevel
Description    : get the module level

Output         : enable_event 1:enable  0 :unenable
Return Value   : module level
-1          // don't output log
0           // error conditions
1           // warning conditions
2           // informational
3           // debug-level messages
*****************************************************************************/
extern int dlog_getlevel(int module_id, int* enable_event);
#else
_declspec(dllexport)  void dlog_error(int module_id, const char *fmt, ...);
_declspec(dllexport)  void dlog_warn(int module_id, const char *fmt, ...);
_declspec(dllexport)  void dlog_info(int module_id, const char *fmt, ...);
_declspec(dllexport)  void dlog_debug(int module_id, const char *fmt, ...);
_declspec(dllexport)  void dlog_event(int module_id, const char *fmt, ...);
_declspec(dllexport)  void dlog_init(void);

/*****************************************************************************
Prototype      : dlog_getlevel
Description    : get the module level

Output         : enable_event 1:enable  0 :unenable
Return Value   : module level
-1          // don't output log
0           // error conditions
1           // warning conditions
2           // informational
3           // debug-level messages
*****************************************************************************/
_declspec(dllexport)  int dlog_getlevel(int module_id, int* enable_event);
#endif
#ifdef __cplusplus
}
#endif
#endif /* sys/slog.h */
