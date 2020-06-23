中文|[英文](README.md)


EZDVPP对DVPP接口进行了封装，用于简化直接调用DVPP接口的复杂度，主要功能包括：

图像格式转化（包含将YUV转换为JPG，JPEG转换为YUV），图像的crop以及图像的resize。

EZDVPP接口的使用方法如下：

-   图像格式转化
    -   将摄像头获取的YUV格式的数据流转换为JPG格式的数据流。

        ```
        //接口使用示例：YUV转换JPG
        typedef struct DvppOutputInfo {
        //output buffer
        unsigned char *pbuf;
        //size of output buffer
        unsigned int size;
        } DvppOutput;
        Ascend::Utils::DvppProcess *pDvppProcess=nullptr;
        //初始化参数：用于将YUV转化为JPG
        dvppToJpgPara.format = JPGENC_FORMAT_NV12;
        dvppToJpgPara.level = DVPP_TO_JPG_QUALITY_PARAMETER;
        dvppToJpgPara.resolution.height = height;
        dvppToJpgPara.resolution.width = width;
        //实例化类变量
        pDvppProcess = new Ascend::Utils::DvppProcess(dvppToJpgPara);
        //调用转化接口，dvppOutput.pbuf即为转换后的JPG数据
        pDvppProcess->DvppOperationProc(inputbuf,inputbufSize,&dvppOutput)
        ```

    -   JPEG转YUV：将输入JPEG格式的图片转化为YUV图像，其中JPEG \(colorspace: yuv, subsample: 444/422/420/400 \)只支持huffman编码，不支持算术编码，不支持渐进编码，不支持jpeg2000格式

        ```
        接口四：JPEG转化为YUV
        struct DvppJpegDOutput {
            unsigned char *buffer;  // output buffer
            uint32_t buffer_size;  // output buffer size
            uint32_t width;  // the width of output image
            uint32_t height;  // the height of output image
            uint32_t aligned_width;  // the aligned width of output image
            uint32_t aligned_height;  // the aligned height of output image
            VpcInputFormat image_format;  //output image format
        };
        ascend::utils::DvppJpegDInPara dvpp_to_yuv_para;
        dvpp_to_yuv_para.is_convert_yuv420 = true;
        ascend::utils::DvppProcess dvpp_yuv_process(dvpp_to_yuv_para);
        ascend::utils::DvppJpegDOutput dvpp_out_data = { 0 };
        dvpp_yuv_process.DvppJpegDProc(face_image_buff,
                                       face_image_size,
                                       &dvpp_out_data);
        ```


-   图像的crop/resize/格式转化

    将输入的YUV444SP/YUV422SP/YUV420SP/YUV444Packed/YUV422Packed/BGR/RGB/ARGB/ABGR/RGBA/BGRA等格式图像转化为YUV420SP格式的图像，并对图像进行crop与resize操作

    ```
    //接口使用示例：对图像进行格式转换、resize操作（本代码示例未进行crop）。
    struct DvppVpcOutput {
        uint8_t *buffer;
        uint32_t size;
    };
    DvppBasicVpcPara resize_para;
    resize_para.input_image_type = INPUT_BGR;
    
    // get original image size and set to resize parameter
    int32_t width = image_handle->image_info.width;
    int32_t height = image_handle->image_info.height;
    
    // set source resolution ratio
    resize_para.src_resolution.width = width;
    resize_para.src_resolution.height = height;
    
    // crop parameters, only resize, no need crop, so set original image size
    // set crop left-top point (need even number)
    resize_para.crop_left = 0;
    resize_para.crop_up = 0;
    // set crop right-bottom point (need odd number)
    uint32_t crop_right = ((width >> 1) << 1) - 1;
    uint32_t crop_down = ((height >> 1) << 1) - 1;
    resize_para.crop_right = crop_right;
    resize_para.crop_down = crop_down;
    // set destination resolution ratio (need even number)
    uint32_t dst_width = ((image_handle->console_params.model_width) >> 1) << 1;
    uint32_t dst_height = ((image_handle->console_params.model_height) >> 1) << 1;
    resize_para.dest_resolution.width = dst_width;
    resize_para.dest_resolution.height = dst_height;
    
    // set input image align or not
    resize_para.is_input_align = false;
    // call
    DvppProcess dvpp_resize_img(resize_para);
    DvppVpcOutput dvpp_output;
    ret = dvpp_resize_img.DvppBasicVpcProc(
          image_handle->image_info.data.get(), image_handle->image_info.size,
          &dvpp_output);
    ```


