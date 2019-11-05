#include <iostream>
#include "MindPublish.h"
#include "BatchImageParaWithScale.h"
#include<stdio.h>
#include<dirent.h>
#include<regex>
#include <fcntl.h>

/**
* @brief: get a list of valid files under the directory
* @[in]: directory, the directory path
* @[in]: fileFormat, a list of format, if file extension match the format, add to the list
* @[out]: filesAbsolutePath, a list of valid files
* @[return]: bool, true if get list of file under the directory successfully. 
*/
bool GetAbsoluteFiles(const std::string& directory, std::vector<std::string>& filesAbsolutePath, const std::vector<std::string>& fileFormat){
    DIR* dir = opendir(directory.c_str());
    if (dir == nullptr){
		HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Common] open directory error!");
        return false;
    }
    struct dirent* d_ent = nullptr;
    char dot[3] = ".";
    char dotdot[6] = "..";
    while((d_ent = readdir(dir)) != nullptr){
        //skip . and ..
        if ((strcmp(d_ent->d_name, dot) == 0) || (strcmp(d_ent->d_name, dotdot) == 0)){
            continue;
        }
        if (d_ent->d_type == DT_DIR){
            //directory
            string newDirectory = directory + std::string("/") + std::string(d_ent->d_name);
            if(directory[directory.length()-1] == '/'){
                newDirectory = directory + std::string(d_ent->d_name);
            }
            if(!GetAbsoluteFiles(newDirectory, filesAbsolutePath,fileFormat)){
                return false;
            }
        }else{
            //file
            std::string sFilename = std::string(d_ent->d_name);
            int index = sFilename.find_last_of('.') ;
            if(index == -1){
                continue;
            }
            std::string fileIndex = sFilename.substr(index + 1);
            for ( std::string format : fileFormat){
                std::regex formatRegex(format);
                //check file is match format
                if (std::regex_match(fileIndex, formatRegex)){
                    string absolutePath = directory + std::string("/") + std::string(d_ent->d_name);
                    if(directory[directory.length()-1] == '/'){
                        absolutePath = directory + std::string(d_ent->d_name); // /a/b/1.txt
                    }
                    HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[Common] get file path %s", absolutePath.c_str());
                    filesAbsolutePath.push_back(absolutePath);
                }
            }
        }
    }
    if(closedir(dir) != 0){
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Common] close directory error!");
        return false;
    }
    return true;
}

/**
* @brief: read file
* @[in]: path, the file path
* @[out]: streamData, the file data
* @[return]: bool, true if read file successfully. 
*/
bool ReadFile(const std::string & path, StreamDataT* streamData){
    if(path == ""){
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Common] file path is null!");
        return false;
    }
    //open file
    FILE * file = fopen(path.c_str(), "r");
    if (nullptr == file)
    {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Common] open file error!");
        return false;
    }
    if (0 != fseek(file, 0, SEEK_END))
    {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Common] fseek to SEEK_END error!");
        if(0 != fclose(file)){
            HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Common] close file error");
        }
        return false;
    }
    //get the file length
    long length = ftell(file);
    if(length <= 0){
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Common] ftell error!");
        if(0 != fclose(file)){
            HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Common] close file error");
        }
        return false;
    }
    unsigned char* buffer = (unsigned char *)malloc(length);
    if(buffer == nullptr){
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Common] alloc error!");
        if(0 != fclose(file)){
            HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Common] close file error");
        }
        return false;
    }
    //seek to head
    if(0 != fseek(file, 0, SEEK_SET)){
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Common] fseek to SEEK_SET error!");
        if(0 != fclose(file)){
            HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Common] close file error");
        }
        free(buffer);
        buffer = nullptr;
        return false;
    }
    //read file
    if(0 == fread(buffer, length, 1, file)){
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Common] fread error!");
        if(0 != fclose(file)){
            HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Common] close file error");
        }
        free(buffer);
        buffer = nullptr;
        return false;
    }
    //close file
    if(0 != fclose(file)){
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Common] close file error");
        free(buffer);
        buffer = nullptr;
        return false;
    }
    streamData->data.reset(buffer);
    streamData->length = length;
    HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[Common] %s size is %d !", path.c_str(), length);
    return true;
}

/**
* @brief: replace '/' to '_'  in name
* @[in]: name, the name to replace
*/
void MakeOutputName(std::string& name){
    while(true){
        std::string::size_type pos = name.find("/");
        if(pos != std::string::npos){
            name.replace(pos, 1, "_");
        }else{
            break;
        }
    }
}

/**
* @brief: create result folder
* @[in]: folderPath, the folder path
* @[in]: mode, the permission of folder
* @[return]: mode, the permission of folder
*/
HIAI_StatusT CreateResultFolder(const std::string& folderPath, mode_t mode) {
    int folder_exist = access(folderPath.c_str(), W_OK);
    if (-1 == folder_exist)
    {
        if (mkdir(folderPath.c_str(), mode) == -1)
        {
            HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "Create %s failed !", folderPath.c_str());
            return HIAI_ERROR;
        }
    }
    return HIAI_OK;
}

/**
* @brief: write output data to file
* @[in]: out, the data to write
* @[in]: folder, the folder name in result_files folder
* @[return]: bool, true if write output file successfully. 
*/
bool WriteOutputToFile(const OutputDataT & out, const std::string& folder){
    return WriteOutputToFile(out, 0, folder);
}

/**
* @brief: write output data to file
* @[in]: out, the data to write
* @[in]: index, the index in output list
* @[in]: folder, the folder name in result_files folder
* @[return]: bool, true if write output file successfully. 
*/
bool WriteOutputToFile(const OutputDataT & out, int index, const std::string& folder){
    //create folder
    if (HIAI_OK != CreateResultFolder("result_files", PERMISSION)) {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Common]: create folder result_files error!");
        return HIAI_ERROR;
    }
    if (HIAI_OK != CreateResultFolder("result_files/" + folder, PERMISSION)) {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Common]: create folder result_files/%s error!", folder.c_str());
        return HIAI_ERROR;
    }
    HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[Common] port: %d, size: %d, name: %s", out.id, out.size, out.name.c_str());
    int size = out.size / sizeof(float);
    if(size <= 0){
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Common]: the OutPutT size less than 0!");
        return HIAI_ERROR;
    }
    float* result = nullptr;
    try{
        result = new float[size];
    }catch (const std::bad_alloc& e) {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Common] alloc output data error!");
        return HIAI_ERROR;
    }
    int ret  = memcpy_s(result, sizeof(float)*size, out.data.get(), sizeof(float)*size);
    if(ret != 0){
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Common] memcpy_s output data error!");
        delete[] result;
        result = nullptr;
        return HIAI_ERROR;
    }
    std::string prefix = "result_files/" + folder + "/" + std::to_string(out.id) + "_" + std::to_string(out.frame_id) + "_" + std::to_string(index);
    std::string name(out.name);
    MakeOutputName(name);
    std::string outFileName = prefix + "_" + name + ".txt";
    //open file
    int fd = open(outFileName.c_str(), O_CREAT| O_WRONLY, FIlE_PERMISSION);
    if(fd == -1){
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Common] open file %s error!", outFileName.c_str());
        delete[] result;
        result = nullptr;
        return HIAI_ERROR;
    }
    //write data to file
    for (int k = 0; k < size; k++){
        std::string value = std::to_string(result[k]);
        if(k > 0){
            value = "\n" + value;
        }
        ret = write(fd, value.c_str(), value.length());
        if(ret == -1){
            HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Common] write file error!");
            ret = close(fd);
            if(ret == -1){
                HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Common] close file error!");
            }
            delete[] result;
            result = nullptr;
            return HIAI_ERROR;
        }
    }
    //close file
    ret = close(fd);
    if(ret == -1){
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Common] close file error!");
        delete[] result;
        result = nullptr;
        return HIAI_ERROR;
    }
    delete[] result;
    result = nullptr;
    return HIAI_OK;
}