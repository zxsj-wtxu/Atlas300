/**
 * ============================================================================
 *
 * Copyright (C) 2018, Hisilicon Technologies Co., Ltd. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1 Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *   2 Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 *   3 Neither the names of the copyright holders nor the names of the
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * ============================================================================
 */

#include <malloc.h>
#include "ascenddk/ascend_ezdvpp/dvpp_utils.h"

namespace ascend {
namespace utils {

DvppUtils::DvppUtils() {
    // TODO Auto-generated constructor stub

}

DvppUtils::~DvppUtils() {
    // TODO Auto-generated destructor stub
}

int DvppUtils::CheckBasicVpcParam(const uint8_t *input_buf, int32_t input_size,
                                  int32_t output_size, uint8_t *output_buf) {
    // null pointer and extreme value check
    if (input_buf == nullptr || output_buf == nullptr || input_size <= 0
            || output_size <= 0) {
        return kDvppErrorInvalidParameter;
    }

    return kDvppOperationOk;
}

int DvppUtils::CheckJpegChangeToYuvParam(const char *input_buf, int input_size,
                                         jpegd_yuv_data_info *output_data) {
    // null pointer and extreme value check
    if (input_buf == nullptr || input_size <= 0 || output_data == nullptr) {
        return kDvppErrorInvalidParameter;
    }

    return kDvppOperationOk;
}

int DvppUtils::CheckDataSize(int data_size) {
    if (data_size <= 0 || data_size > kAllowedMaxImageMemory) {
        return kDvppErrorCheckMemorySizeFail;
    }

    return kDvppOperationOk;
}

int DvppUtils::CheckIncreaseParam(double hinc, double vinc) {
    // extreme value check
    if (hinc < kMinIncrease || hinc > kMaxIncrease || vinc < kMinIncrease
            || vinc > kMaxIncrease) {
        return kDvppErrorInvalidParameter;
    }
    return kDvppOperationOk;
}

int DvppUtils::CheckImageNeedAlign(int width, int high) {
    // check width and height whether need to align
    if ((width % kVpcWidthAlign) == 0 && (high % kVpcHeightAlign) == 0) {
        return kImageNotNeedAlign;
    }

    return kImageNeedAlign;
}

int DvppUtils::CheckBasicVpcImageFormat(VpcInputFormat input_format,
                                        VpcOutputFormat output_format) {
    if (input_format < INPUT_YUV400
            || input_format > INPUT_YUV420_SEMI_PLANNER_VU_10BIT
            || (output_format != OUTPUT_YUV420SP_UV
                    && output_format != OUTPUT_YUV420SP_VU)) {
        return kDvppErrorInvalidParameter;
    }

    return kDvppOperationOk;
}

int DvppUtils::CheckBasicVpcOutputParam(int width, int height) {
    if ((width % 2 != 0) || (height % 2 != 0)) {
        return kDvppErrorInvalidParameter;
    }
    return kDvppOperationOk;
}

int DvppUtils::CheckBasicVpcCropParam(uint32_t left_offset, uint32_t up_offset,
                                      uint32_t right_offset,
                                      uint32_t down_offset) {
    if ((left_offset % 2 != 0) || (up_offset % 2 != 0)
            || (right_offset % 2 == 0) || (down_offset % 2 == 0)) {
        return kDvppErrorInvalidParameter;
    }

    return kDvppOperationOk;
}

int DvppUtils::AllocInputBuffer(const uint8_t * src_data, int input_size,
                                bool is_input_align, VpcInputFormat format,
                                int width, int high, int &width_stride,
                                int &dest_buffer_size, uint8_t **dest_data) {
    // Dvpp requires the width and high of image must be even, so we need
    // convert the width and high of an odd number into even numbers
    int even_width = (width >> 1) << 1;
    int even_high = (high >> 1) << 1;

    // height of image need 16-byte alignment
    int align_high = ALIGN_UP(even_high, kVpcHeightAlign);
    int ret = kDvppOperationOk;

    switch (format) {
        case INPUT_YUV420_SEMI_PLANNER_UV_10BIT:
        case INPUT_YUV420_SEMI_PLANNER_VU_10BIT:
        case INPUT_YUV420_SEMI_PLANNER_UV:
        case INPUT_YUV420_SEMI_PLANNER_VU: {
            // width of image need 128-byte alignment
            int align_width = ALIGN_UP(even_width, kVpcWidthAlign);
            width_stride = align_width;

            // The memory size of yuv420 image is 1.5 times width * height
            dest_buffer_size = align_width * align_high
                    * DVPP_YUV420SP_SIZE_MOLECULE
                    / DVPP_YUV420SP_SIZE_DENOMINATOR;

            // input data address 128 byte alignment
            *dest_data = (uint8_t *) mmap(
                    0, ALIGN_UP(dest_buffer_size, MAP_2M),
                    PROT_READ | PROT_WRITE,
                    MAP_PRIVATE | MAP_ANONYMOUS | API_MAP_VA32BIT, -1, 0);
            CHECK_MMAP_RESULT(dest_data);

            // alloc yuv420sp buffer
            ret = AllocYuv420SPBuffer(src_data, input_size, is_input_align,
                                      width, align_width, high, align_high,
                                      dest_buffer_size, *dest_data);
            break;
        }
        case INPUT_YUV422_SEMI_PLANNER_UV:
        case INPUT_YUV422_SEMI_PLANNER_VU: {
            // width of image need 128-byte alignment
            int align_width = ALIGN_UP(even_width, kVpcWidthAlign);
            width_stride = align_width;

            // The memory size of yuv422 image is 2 times width * height
            dest_buffer_size = align_width * align_high * kYuv422SPWidthMul;

            // input data address 128 byte alignment
            *dest_data = (uint8_t *) mmap(
                    0, ALIGN_UP(dest_buffer_size, MAP_2M),
                    PROT_READ | PROT_WRITE,
                    MAP_PRIVATE | MAP_ANONYMOUS | API_MAP_VA32BIT, -1, 0);
            CHECK_MMAP_RESULT(dest_data);

            // alloc yuv422sp buffer
            ret = AllocYuv422SPBuffer(src_data, input_size, is_input_align,
                                      width, align_width, high, align_high,
                                      dest_buffer_size, *dest_data);
            break;
        }
        case INPUT_YUV444_SEMI_PLANNER_UV:
        case INPUT_YUV444_SEMI_PLANNER_VU: {
            // y channel width of yuv444sp equals to image width and need
            // 128-byte alignment
            int y_align_width = ALIGN_UP(even_width, kVpcWidthAlign);
            width_stride = y_align_width;

            // uv channel width of yuv444sp is 2 times image width and need
            // 128-byte alignment
            int uv_align_width = ALIGN_UP(even_width * kYuv444SPWidthMul,
                                          kVpcWidthAlign);

            // memory size of yuv444sp = memory size of y channel + memory size
            // of uv channel
            dest_buffer_size = y_align_width * align_high
                    + uv_align_width * align_high;

            // input data address 128 byte alignment
            *dest_data = (uint8_t *) mmap(
                    0, ALIGN_UP(dest_buffer_size, MAP_2M),
                    PROT_READ | PROT_WRITE,
                    MAP_PRIVATE | MAP_ANONYMOUS | API_MAP_VA32BIT, -1, 0);
            CHECK_MMAP_RESULT(dest_data);

            // alloc yuv444sp buffer
            ret = AllocYuv444SPBuffer(src_data, input_size, is_input_align,
                                      width, y_align_width, uv_align_width,
                                      high, align_high, dest_buffer_size,
                                      *dest_data);
            break;
        }
        case INPUT_YUV422_PACKED_YUYV:
        case INPUT_YUV422_PACKED_UYVY:
        case INPUT_YUV422_PACKED_YVYU:
        case INPUT_YUV422_PACKED_VYUY: {
            int src_width = width * kYuv422PackedWidthMul;

            //  The memory size of each row in yuv422 packed is 2 times width of
            //  image
            int yuv422_packed_width = even_width * kYuv422PackedWidthMul;

            // The memory size of each row need 128-byte alignment
            int align_width = ALIGN_UP(yuv422_packed_width, kVpcWidthAlign);
            width_stride = align_width;

            // memory size of yuv422 packed
            dest_buffer_size = align_width * align_high;

            // input data address 128 byte alignment
            *dest_data = (uint8_t *) mmap(
                    0, ALIGN_UP(dest_buffer_size, MAP_2M),
                    PROT_READ | PROT_WRITE,
                    MAP_PRIVATE | MAP_ANONYMOUS | API_MAP_VA32BIT, -1, 0);
            CHECK_MMAP_RESULT(dest_data);

            // alloc yuv422 packed buffer
            ret = AllocYuvOrRgbPackedBuffer(src_data, input_size,
                                            is_input_align, src_width,
                                            yuv422_packed_width, align_width,
                                            high, align_high, dest_buffer_size,
                                            *dest_data);
            break;
        }
        case INPUT_YUV444_PACKED_YUV: {
            int src_width = width * kYuv444PackedWidthMul;

            // The memory size of each row in yuv444 packed is 3 times width of
            // image
            int yuv444_packed_width = even_width * kYuv444PackedWidthMul;

            // The memory size of each row need 128-byte alignment
            int align_width = ALIGN_UP(yuv444_packed_width, kVpcWidthAlign);
            width_stride = align_width;

            // memory size of yuv444 packed
            dest_buffer_size = align_width * align_high;

            // input data address 128 byte alignment
            *dest_data = (uint8_t *) mmap(
                    0, ALIGN_UP(dest_buffer_size, MAP_2M),
                    PROT_READ | PROT_WRITE,
                    MAP_PRIVATE | MAP_ANONYMOUS | API_MAP_VA32BIT, -1, 0);
            CHECK_MMAP_RESULT(dest_data);

            // alloc yuv444 packed buffer
            ret = AllocYuvOrRgbPackedBuffer(src_data, input_size,
                                            is_input_align, src_width,
                                            yuv444_packed_width, align_width,
                                            high, align_high, dest_buffer_size,
                                            *dest_data);
            break;
        }
        case INPUT_RGB:
        case INPUT_BGR: {
            int src_width = width * kRgb888WidthMul;

            // The memory size of each row in rgb888 packed is 3 times width of
            // image
            int rgb888_width = even_width * kRgb888WidthMul;

            // The memory size of each row need 128-byte alignment
            int align_width = ALIGN_UP(rgb888_width, kVpcWidthAlign);
            width_stride = align_width;

            // memory size of rgb888 packed
            dest_buffer_size = align_width * align_high;

            // input data address 128 byte alignment
            *dest_data = (uint8_t *) mmap(
                    0, ALIGN_UP(dest_buffer_size, MAP_2M),
                    PROT_READ | PROT_WRITE,
                    MAP_PRIVATE | MAP_ANONYMOUS | API_MAP_VA32BIT, -1, 0);
            CHECK_MMAP_RESULT(dest_data);

            // alloc rgb888 packed buffer
            ret = AllocYuvOrRgbPackedBuffer(src_data, input_size,
                                            is_input_align, src_width,
                                            rgb888_width, align_width, high,
                                            align_high, dest_buffer_size,
                                            *dest_data);
            break;
        }
        case INPUT_ARGB:
        case INPUT_ABGR:
        case INPUT_RGBA:
        case INPUT_BGRA: {
            int src_width = width * kXrgb888WidthMul;

            // The memory size of each row in xrgb8888 packed is 4 times width
            // of image
            int xrgb8888_width = even_width * kXrgb888WidthMul;

            // The memory size of each row need 128-byte alignment
            int align_width = ALIGN_UP(xrgb8888_width, kVpcWidthAlign);
            width_stride = align_width;

            // memory size of xrgb8888 packed
            dest_buffer_size = align_width * align_high;

            // input data address 128 byte alignment
            *dest_data = (uint8_t *) mmap(
                    0, ALIGN_UP(dest_buffer_size, MAP_2M),
                    PROT_READ | PROT_WRITE,
                    MAP_PRIVATE | MAP_ANONYMOUS | API_MAP_VA32BIT, -1, 0);
            CHECK_MMAP_RESULT(dest_data);

            // alloc xrgb8888 packed buffer
            ret = AllocYuvOrRgbPackedBuffer(src_data, input_size,
                                            is_input_align, src_width,
                                            xrgb8888_width, align_width, high,
                                            align_high, dest_buffer_size,
                                            *dest_data);
            break;
        }
        case INPUT_YUV400: {

            // The memory size of each row need 128-byte alignment
            int align_width = ALIGN_UP(even_width, kVpcWidthAlign);
            width_stride = align_width;

            // yuv400sp image apply for the same size space as yuv420sp image
            dest_buffer_size = align_width * align_high
                    * DVPP_YUV420SP_SIZE_MOLECULE
                    / DVPP_YUV420SP_SIZE_DENOMINATOR;

            // input data address 128 byte alignment
            *dest_data = (uint8_t *) mmap(
                    0, ALIGN_UP(dest_buffer_size, MAP_2M),
                    PROT_READ | PROT_WRITE,
                    MAP_PRIVATE | MAP_ANONYMOUS | API_MAP_VA32BIT, -1, 0);
            CHECK_MMAP_RESULT(dest_data);

            // alloc yuv400sp buffer
            ret = AllocYuv420SPBuffer(src_data, input_size, is_input_align,
                                      width, align_width, high, align_high,
                                      dest_buffer_size, *dest_data);
            break;
        }
        default: {
            ASC_LOG_ERROR(
                    "The current image format is not supported, "
                    "so space cannot be allocated!");
            ret = kDvppErrorInvalidParameter;
            break;
        }
    }

    if (ret == kDvppOperationOk && *dest_data == nullptr) {
        ASC_LOG_ERROR("alloc dest_data memory failed!");
        ret = kDvppErrorMallocFail;
    }

    return ret;
}

int DvppUtils::AllocYuv420SPBuffer(const uint8_t * src_data, int input_size,
                                   bool is_input_align, int width,
                                   int align_width, int high, int align_high,
                                   int dest_buffer_size, uint8_t * dest_data) {
    int ret = EOK;

    // Dvpp requires the width and high of image must be even, so we need
    // convert the width and high of an odd number into even numbers
    int even_width = (width >> 1) << 1;
    int even_high = (high >> 1) << 1;

    // If the input image is aligned , directly copy all memory.
    if ((width == align_width && high == align_high) || is_input_align) {
        ret = memcpy_s(dest_data, dest_buffer_size, src_data, input_size);
        CHECK_CROP_RESIZE_MEMCPY_RESULT(ret, dest_buffer_size, dest_data);
    } else {      // If image is not aligned, memory copy from line to line.
        int remain_buffer_size = dest_buffer_size;

        // y channel data copy
        for (int i = 0; i < even_high; ++i) {
            ret = memcpy_s(dest_data + ((ptrdiff_t) i * align_width),
                           remain_buffer_size, src_data, even_width);
            CHECK_CROP_RESIZE_MEMCPY_RESULT(ret, dest_buffer_size, dest_data);
            remain_buffer_size -= align_width;
            src_data += width;
        }

        dest_data += (ptrdiff_t) (align_high - even_high) * align_width;
        remain_buffer_size -= (align_high - even_high) * align_width;
        src_data += (high - even_high) * width;

        // uv channel data copy
        for (int j = even_high; j < even_high + even_high / 2; ++j) {
            ret = memcpy_s(dest_data + ((ptrdiff_t) j * align_width),
                           remain_buffer_size, src_data, even_width);
            CHECK_CROP_RESIZE_MEMCPY_RESULT(ret, dest_buffer_size, dest_data);
            remain_buffer_size -= align_width;
            src_data += width;
        }
    }

    return kDvppOperationOk;
}

int DvppUtils::AllocYuv422SPBuffer(const uint8_t * src_data, int input_size,
                                   bool is_input_align, int width,
                                   int align_width, int high, int align_high,
                                   int dest_buffer_size, uint8_t * dest_data) {
    int ret = EOK;

    // Dvpp requires the width and high of image must be even, so we need
    // convert the width and high of an odd number into even numbers
    int even_width = (width >> 1) << 1;
    int even_high = (high >> 1) << 1;

    // If the input image is aligned , directly copy all memory.
    if ((width == align_width && high == align_high) || is_input_align) {
        ret = memcpy_s(dest_data, dest_buffer_size, src_data, input_size);
        CHECK_CROP_RESIZE_MEMCPY_RESULT(ret, dest_buffer_size, dest_data);
    } else {      // If image is not aligned, memory copy from line to line.
        int remain_buffer_size = dest_buffer_size;

        // y channel data copy
        for (int i = 0; i < even_high; ++i) {
            ret = memcpy_s(dest_data + ((ptrdiff_t) i * align_width),
                           remain_buffer_size, src_data, even_width);
            CHECK_CROP_RESIZE_MEMCPY_RESULT(ret, dest_buffer_size, dest_data);
            remain_buffer_size -= align_width;
            src_data += width;
        }

        dest_data += (ptrdiff_t) (align_high - even_high) * align_width;
        remain_buffer_size -= (align_high - even_high) * align_width;
        src_data += (high - even_high) * width;

        // uv channel data copy
        for (int j = even_high; j < even_high * 2; ++j) {
            ret = memcpy_s(dest_data + ((ptrdiff_t) j * align_width),
                           remain_buffer_size, src_data, even_width);
            CHECK_CROP_RESIZE_MEMCPY_RESULT(ret, dest_buffer_size, dest_data);
            remain_buffer_size -= align_width;
            src_data += width;
        }
    }
    return kDvppOperationOk;
}

int DvppUtils::AllocYuv444SPBuffer(const uint8_t * src_data, int input_size,
                                   bool is_input_align, int width,
                                   int y_align_width, int uv_align_width,
                                   int high, int align_high,
                                   int dest_buffer_size, uint8_t * dest_data) {
    int ret = EOK;

    // Dvpp requires the width and high of image must be even, so we need
    // convert the width and high of an odd number into even numbers
    int even_width = (width >> 1) << 1;
    int even_high = (high >> 1) << 1;

    // If the input image is aligned , directly copy all memory.
    if ((width == y_align_width && high == align_high) || is_input_align) {
        ret = memcpy_s(dest_data, dest_buffer_size, src_data, input_size);
        CHECK_CROP_RESIZE_MEMCPY_RESULT(ret, dest_buffer_size, dest_data);
    } else {      // If image is not aligned, memory copy from line to line.
        int remain_buffer_size = dest_buffer_size;

        // y channel data copy
        for (int i = 0; i < even_high; ++i) {
            ret = memcpy_s(dest_data + ((ptrdiff_t) i * y_align_width),
                           remain_buffer_size, src_data, even_width);
            CHECK_CROP_RESIZE_MEMCPY_RESULT(ret, dest_buffer_size, dest_data);
            remain_buffer_size -= y_align_width;
            src_data += width;
        }

        dest_data += (ptrdiff_t) align_high * y_align_width;
        remain_buffer_size -= (align_high - even_high) * y_align_width;
        src_data += (high - even_high) * width;

        // uv channel data copy
        for (int j = even_high; j < even_high * 2; ++j) {
            ret = memcpy_s(
                    dest_data + ((ptrdiff_t) (j - even_high) * uv_align_width),
                    remain_buffer_size, src_data,
                    even_width * kYuv444SPWidthMul);
            CHECK_CROP_RESIZE_MEMCPY_RESULT(ret, dest_buffer_size, dest_data);
            remain_buffer_size -= uv_align_width;
            src_data += width * kYuv444SPWidthMul;
        }
    }
    return kDvppOperationOk;
}

int DvppUtils::AllocYuvOrRgbPackedBuffer(const uint8_t * src_data,
                                         int input_size, bool is_input_align,
                                         int src_width, int dest_width,
                                         int dest_align_width, int high,
                                         int align_high, int dest_buffer_size,
                                         uint8_t * dest_data) {
    int ret = EOK;

    // Dvpp requires the width and high of image must be even, so we need
    // convert the width and high of an odd number into even numbers
    int even_high = (high >> 1) << 1;

    // If the input image is aligned , directly copy all memory.
    if ((src_width == dest_align_width && high == align_high)
            || is_input_align) {
        ret = memcpy_s(dest_data, dest_buffer_size, src_data, input_size);
        CHECK_CROP_RESIZE_MEMCPY_RESULT(ret, dest_buffer_size, dest_data);
    } else {      // If image is not aligned, memory copy from line to line.
        int remain_buffer_size = dest_buffer_size;

        // y channel and uv channel data copy
        for (int i = 0; i < even_high; ++i) {
            ret = memcpy_s(dest_data + ((ptrdiff_t) i * dest_align_width),
                           remain_buffer_size, src_data, dest_width);
            CHECK_CROP_RESIZE_MEMCPY_RESULT(ret, dest_buffer_size, dest_data);
            remain_buffer_size -= dest_align_width;
            src_data += src_width;
        }
    }
    return kDvppOperationOk;
}
} /* namespace utils */
} /* namespace ascend */
