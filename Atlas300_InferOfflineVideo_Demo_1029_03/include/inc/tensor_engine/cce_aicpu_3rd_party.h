#ifndef __CCE_AICPU_3RD_PARTY_H__
#define __CCE_AICPU_3RD_PARTY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <blas_device_api.h>
#include <nn_device_api.h>

#ifndef __aicpu__
#define __aicpu__ [aicpu]
#endif

#ifndef CCE_TVM_MAX
#define CCE_TVM_MAX(a, b)  (((a) > (b)) ? (a) : (b))
#endif

extern __aicpu__ void *aicpu_malloc(unsigned int size);
extern __aicpu__ void aicpu_free(void * ptr);
extern __aicpu__ double log(double x);
extern __aicpu__ double exp(double x);
extern __aicpu__ double round(double x);
extern __aicpu__ double floor(double x);
extern __aicpu__ double ceil(double x);
extern __aicpu__ double trunc(double x);
extern __aicpu__ double sqrt(double x);

extern __aicpu__ float logf(float x);
extern __aicpu__ float expf(float x);
extern __aicpu__ float roundf(float x);
extern __aicpu__ float floorf(float x);
extern __aicpu__ float ceilf(float x);
extern __aicpu__ float truncf(float x);
extern __aicpu__ float sqrtf(float x);

#ifdef __cplusplus
}
#endif

#endif /* __CCE_AICPU_3RD_PARTY_H__ */
