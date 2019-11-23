#ifndef MIND_PUBLISH_H_
#define MIND_PUBLISH_H_

#include "hiaiengine/data_type.h"
#include "hiaiengine/data_type_reg.h"
#include "hiaiengine/status.h"
#include <unistd.h>
#include <sys/stat.h>
#include "hiaiengine/ai_tensor.h"

//the type for publish input
enum Type{
   PICINFO, //the image path list
   BINARYINFO,//the stream data list
   NUMPYINFO,//the nntensor data list
   BININFO,//the raw path list
};

//the struct for raw format image, the data type must be float
typedef struct BinArry{
	std::vector<std::string> file_list; //the list of file path
	int32_t height;
   	int32_t width;
	int32_t channel;
}BinArryT;

//the stream data
typedef struct StreamData{
	std::shared_ptr<u_int8_t> data;
	int32_t length; // the data length
}StreamDataT;

//the input data for publish
typedef struct InputDataForPublish{
    //the vector size must be equal to the number of output ports for PublishInput
    std::vector<std::vector<std::string>> img_name_list; //the image path list for PICINFO
	std::vector<std::vector<StreamDataT>> img_binary_list;//the image binary list  for BINARYINFO
	std::vector<std::shared_ptr<hiai::AINeuralNetworkBuffer>> nn_tensor_list;//the nntensor list for NUMPYINFO
	std::vector<BinArryT> bin_arry;//the raw path list for BININFO
    enum Type type;
 }InputDataForPublishT;

//the output data for each image
typedef struct OutputData
{
    int32_t size; //the data size
    std::string name;// the model output name
    int32_t id;//the port index
    uint32_t frame_id;//the image frame id
    std::shared_ptr<u_int8_t> data;//the output data
}OutputDataT;

//the output data for publish
typedef struct OutputDataForPublish {
    //the vector size must be equal to the number of input ports for PublishOutput
    std::vector<OutputDataT> output_data_vec;// a list of OutputDataT
}OutputDataForPublishT;

const std::vector<std::string> PUBLISH_SUPPORT_FILES = {"png","jpg", "jpeg", "bmp", "PNG", "JPG", "JPEG", "BMP"};

/**
* @brief: get a list of valid files under the directory
* @[in]: directory, the directory path
* @[in]: fileFormat, a list of format, if file extension match the format, add to the list
* @[out]: filesAbsolutePath, a list of valid files
* @[return]: bool, true if get list of file under the directory successfully. 
*/
bool GetAbsoluteFiles(const std::string& directory, std::vector<std::string>& filesAbsolutePath, const std::vector<std::string>& fileFormat);

/**
* @brief: read file
* @[in]: path, the file path
* @[out]: streamData, the file data
* @[return]: bool, true if read file successfully. 
*/
bool ReadFile(const std::string & path, StreamDataT* streamData);

/**
* @brief: write output data to file
* @[in]: out, the data to write
* @[in]: index, the index in output list
* @[in]: folder, the folder name in result_files folder
* @[return]: bool, true if write output file successfully. 
*/
bool WriteOutputToFile(const OutputDataT & out, int index, const std::string& folder);

/**
* @brief: write output data to file
* @[in]: out, the data to write
* @[in]: folder, the folder name in result_files folder
* @[return]: bool, true if write output file successfully. 
*/
bool WriteOutputToFile(const OutputDataT & out,  const std::string& folder);
#endif
