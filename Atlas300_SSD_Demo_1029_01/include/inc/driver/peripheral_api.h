#ifndef   PERIPHERAL_API_H
#define   PERIPHERAL_API_H
#include <stdint.h>

// ��������ͷ����.
#define  HIAI_MAX_CAMERA_COUNT  (25)


// ����MIC����.
#define  HIAI_MAX_MIC_COUNT  (25)


// ����ͷ֧�ֵķֱ�����ϵ��������.
#define  HIAI_MAX_CAMERARESOLUTION_COUNT  (25)

enum LIBMEDIA_STATUS
{
    LIBMEDIA_STATUS_FAILED = 0,
    LIBMEDIA_STATUS_OK,
};

int MediaLibInit();


// ��ѯоƬ�Ƿ������ͨ.
// chipName  оƬ����.
// ���ڷ���1�����򷵻�0
// ʾ��:
// IsChipAlive("3559") //< ���3559оƬ�Ƿ���λ.
int IsChipAlive(char* chipName);


// ��ѯ����ͷͨ��
// �������е�����ͷID��Ϣ��
// cameraIds ���ڱ���camera id��
// count  cameraIds �ĳ��ȡ����HIAI_MAX_CAMERA_COUNT��.
// ��������ͷͨ������������ͷid����cameraIds��.
uint32_t QueryCameraIds(int* cameraIds, uint32_t *count);



//2. ��ѯ����ͷ״̬.
enum CameraStatus
{
    CAMERA_STATUS_OPEN = 1,         // ����ͷ���ڴ�״̬
    CAMERA_STATUS_CLOSED = 2,       // ����ͷ���ڹر�״̬
    CAMERA_NOT_EXISTS = 3,          // ������ͷ������
    CAMERA_STATUS_UNKOWN = 4,       // ����ͷ״̬δ֪
};

enum CameraStatus  QueryCameraStatus(int cameraId);


// ������ͷ
// �򿪳ɹ�����1�����򷵻�0.
int  OpenCamera(int cameraId);


// ��������ͷ����
enum CameraProperties
{
    CAMERA_PROP_RESOLUTION              =1,          //��Read/Write���ֱ���  �������� CameraResolution* ����Ϊ1
    CAMERA_PROP_FPS                     =2,          //��Read/Write��֡��, ��������Ϊuint32_t
    CAMERA_PROP_IMAGE_FORMAT            =3,          //��Read/Write��֡ͼƬ�ĸ�ʽ.  ��������ΪCameraImageFormat
    CAMERA_PROP_SUPPORTED_RESOLUTION    =4,          //��Read�����ڻ�ȡ����ͷ֧�ֵ����еķֱ����б�.��������ΪCameraResolution*, ���鳤��ΪHIAI_MAX_CAMERARESOLUTION_COUNT
    CAMERA_PROP_CAP_MODE                =5,          //��Read/Write��֡���ݻ�ȡ�ķ�ʽ���������߱���.��������ΪCameraCapMode
    CAMERA_PROP_BRIGHTNESS              =6,          //��Read/Write�����ȣ���������Ϊuint32_t
    CAMERA_PROP_CONTRAST                =7,          //��Read/Write���Աȶȣ���������Ϊuint32_t
    CAMERA_PROP_SATURATION              =8,          //��Read/Write�����Ͷȣ���������Ϊuint32_t
    CAMERA_PROP_HUE                     =9,          //��Read/Write��ɫ������������Ϊuint32_t
    CAMERA_PROP_GAIN                    =10,         //��Read/Write�����棬��������Ϊuint32_t
    CAMERA_PROP_EXPOSURE                =11,         //��Read/Write���ع⣬��������Ϊuint32_t
};

// ����ͷͼƬ��ʽ
enum CameraImageFormat
{
    CAMERA_IMAGE_YUV420_SP = 1,
};

// ����ͷ���ݻ�ȡģʽ
enum CameraCapMode
{
    CAMERA_CAP_ACTIVE  = 1,  // ����ģʽ.
    CAMERA_CAP_PASSIVE = 2,  // ����ģʽ.
};

// �ֱ���.
struct CameraResolution
{
    int width;
    int height;
};

// ��������ͷ����.
// ���óɹ�����1�����򷵻�0
int  SetCameraProperty(int cameraId, enum CameraProperties prop, const void* pInValue);


// ��ȡ����ͷ����.
//
// ��ȡ�ɹ�����1�����򷵻�0
int  GetCameraProperty(int cameraId, enum CameraProperties prop, void* pValue);

// ��ȡ����ͷ����.
// ִ�гɹ�����1�����򷵻�0
typedef int (*CAP_CAMERA_CALLBACK) (const void* pdata, int size, void* param);

// �����ɼ�Camera
// ִ�гɹ�����1�� ���򷵻�0.
int CapCamera(int cameraId, CAP_CAMERA_CALLBACK , void* param);

// �����ɼ�Camera
// ������ͷ��ȡһ֡ͼƬ����.
// ִ�гɹ�����1�� ���򷵻�0.
int ReadFrameFromCamera(int cameralId, void* pdata, int* size);


// �ر�����ͷ
// �򿪳ɹ�����1�����򷵻�0.
int CloseCamera(int cameraId);

////////////////////////////////////////////////////
//
//     MIC  Interace
//
///////////////////////////////////////////////////


// ����ͷ���ݻ�ȡģʽ
enum MICCapMode
{
    MIC_CAP_ACTIVE  = 1,  // ����ģʽ.
    MIC_CAP_PASSIVE = 2,  // ����ģʽ.
};


enum AudioSampleRate
{
    MIC_AUDIO_SAMPLE_RATE_8000   = 8000,    /* 8K samplerate*/
    MIC_AUDIO_SAMPLE_RATE_12000  = 12000,   /* 12K samplerate*/
    MIC_AUDIO_SAMPLE_RATE_11025  = 11025,   /* 11.025K samplerate*/
    MIC_AUDIO_SAMPLE_RATE_16000  = 16000,   /* 16K samplerate*/
    MIC_AUDIO_SAMPLE_RATE_22050  = 22050,   /* 22.050K samplerate*/
    MIC_AUDIO_SAMPLE_RATE_24000  = 24000,   /* 24K samplerate*/
    MIC_AUDIO_SAMPLE_RATE_32000  = 32000,   /* 32K samplerate*/
    MIC_AUDIO_SAMPLE_RATE_44100  = 44100,   /* 44.1K samplerate*/
    MIC_AUDIO_SAMPLE_RATE_48000  = 48000,   /* 48K samplerate*/
    MIC_AUDIO_SAMPLE_RATE_64000  = 64000,   /* 64K samplerate*/
    MIC_AUDIO_SAMPLE_RATE_96000  = 96000,   /* 96K samplerate*/
    MIC_AUDIO_SAMPLE_RATE_BUTT,
};

enum AudioSampleNumPerFrame
{
    MIC_SAMPLE_NUM_80   = 80,
    MIC_SAMPLE_NUM_160  = 160,
    MIC_SAMPLE_NUM_240  = 240,
    MIC_SAMPLE_NUM_320  = 320,
    MIC_SAMPLE_NUM_480  = 480,
    MIC_SAMPLE_NUM_1024 = 1024,
    MIC_SAMPLE_NUM_2048 = 2048,
};

enum AudioBitWidth
{
    MIC_AUDIO_BIT_WIDTH_16  = 1,   /* 16bit width*/
    MIC_AUDIO_BIT_WIDTH_24  = 2,   /* 24bit width*/
    MIC_AUDIO_BIT_WIDTH_BUTT,
};


enum AudioMode
{
    MIC_AUDIO_SOUND_MODE_MONO   =0,/*mono*/
    MIC_AUDIO_SOUND_MODE_STEREO =1,/*stereo*/
    MIC_AUDIO_SOUND_MODE_BUTT
};

struct MICProperties
{
    enum AudioSampleRate sample_rate;               //��Read/Write��������, ��������Ϊuint32_t
    enum MICCapMode    cap_mode;                    //��Read/Write��MIC����������ģʽ
    enum AudioBitWidth bit_width;                   //��Read/Write��ÿ��������bitλ��
    enum AudioSampleNumPerFrame frame_sample_rate;  //��Read/Write��ÿ֡����������
    enum AudioMode  sound_mode;                     //��Read/Write������������������.
};

enum MICStatus
{
    MIC_STATUS_OPEN = 1,         // MIC���ڴ�״̬
    MIC_STATUS_CLOSED = 2,       // MIC���ڹر�״̬
    MIC_NOT_EXISTS = 3,          // MIC������
    MIC_STATUS_UNKOWN = 4,       // MIC״̬δ֪
};

enum MICStatus  QueryMICStatus();

// �� MIC
int OpenMIC();

// ����MIC ����.
int SetMICProperty(struct MICProperties *propties);

// ��ȡMIC ����.
int GetMICProperty(struct MICProperties *propties);

typedef int (*CAP_MIC_CALLBACK) (const void* pdata, int size, void* param);

// �����ɼ�MIC
int CapMIC(CAP_MIC_CALLBACK, void* param);

// �����ɼ�ģʽʱ���û�̬������øýӿڴ�MIC�豸��ȡ��Ƶ����
// �����ȡ����ʱ��MIC�豸�����ݻ�û�вɼ��������������û�̬Ӧ�ó���ֱ��һ֡���ݽ�������
// Լ��������memory copy��ʽ���û���������
// pdata �û�̬buffer���û���Ҫ׼���㹻����buffer������һ֡��Ƶ���ݵĳ���
// size �û�̬buffer�Ĵ�С,ִ�гɹ��󣬷���ʵ�����ݳ���
// ִ�гɹ�����1�� ���򷵻�0.
int ReadMicSound(void* pdata, int *size);

// �ر�MIC
int CloseMIC();

#endif

