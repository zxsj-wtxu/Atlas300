project(host)
 
if(NOT DEFINED ENV{FFMPEG_PATH})
    message(FATAL_ERROR "please define environment variable:FFMPEG_PATH")  
else()
    # message("FFMPEG_PATH $ENV{FFMPEG_PATH}")
    set(FFMPEG_PATH $ENV{FFMPEG_PATH})
endif()
find_package(FFMPEG REQUIRED)

include_directories(${DDK_OPENCV_INCLUDE_DIRS})
message(${DDK_HOST_LIBRARIES})

aux_source_directory(${PROJECT_SRC_ROOT}/StreamDemux StreamDemux_SRC)
add_library(StreamDemux SHARED ${StreamDemux_SRC})
target_include_directories(StreamDemux PUBLIC ${FFMPEG_INCLUDE_DIRS})
target_link_libraries(StreamDemux ${DDK_HOST_LIBRARIES} ${FFMPEG_LIBRARIES})

#aux_source_directory(${PROJECT_SRC_ROOT}/StreamPuller StreamPuller_SRC)
#add_library(StreamPuller SHARED ${StreamPuller_SRC})
#target_include_directories(StreamPuller PUBLIC ${FFMPEG_INCLUDE_DIRS})
#target_link_libraries(StreamPuller ${DDK_HOST_LIBRARIES} ${FFMPEG_LIBRARIES})

aux_source_directory(${PROJECT_SRC_ROOT}/video_decode video_decoder_SRC)
add_library(video_decode SHARED ${video_decoder_SRC})
target_include_directories(video_decode PUBLIC ${FFMPEG_INCLUDE_DIRS})
target_link_libraries(video_decode ${DDK_HOST_LIBRARIES} ${FFMPEG_LIBRARIES})

aux_source_directory(${PROJECT_SRC_ROOT}/DstEngine DstEngine_SRC)
add_library(DstEngine SHARED ${DstEngine_SRC})
target_include_directories(DstEngine PUBLIC ${FFMPEG_INCLUDE_DIRS})
target_link_libraries(DstEngine ${DDK_HOST_LIBRARIES} ${DDK_HOST_OPENCV_LIBRARYS})

aux_source_directory(${PROJECT_SRC_ROOT}/BicycleDetectionModule BicycleDetectionModule_SRC)
aux_source_directory(${PROJECT_SRC_ROOT}/DataTypes DataTypes_SRC)
aux_source_directory(${PROJECT_SRC_ROOT}/Sqlite3Manager Sqlite3Manager_SRC)
aux_source_directory(${PROJECT_SRC_ROOT}/ParamManager ParamManager_SRC)
aux_source_directory(${PROJECT_SRC_ROOT}/GoAhead GoAhead_SRC)
aux_source_directory(${PROJECT_SRC_ROOT}/GraphManager GraphManager_SRC)
aux_source_directory(${PROJECT_SRC_ROOT}/HttpService HttpService_SRC)

add_library(BicycleCensus SHARED ${BicycleDetectionModule_SRC} ${DataTypes_SRC} ${Sqlite3Manager_SRC} ${ParamManager_SRC} ${GoAhead_SRC} ${GraphManager_SRC} ${HttpService_SRC})
target_include_directories(BicycleCensus PUBLIC
    ${PROJECT_SRC_ROOT}/include
    ${PROJECT_SRC_ROOT}/include/curl
    ${PROJECT_SRC_ROOT}/BicycleDetectionModule
    ${PROJECT_SRC_ROOT}/DataTypes
    ${PROJECT_SRC_ROOT}/Sqlite3Manager
    ${PROJECT_SRC_ROOT}/Sqlite3Manager/SQLiteCpp
    ${PROJECT_SRC_ROOT}/ParamManager
    ${PROJECT_SRC_ROOT}/GoAhead
    ${PROJECT_SRC_ROOT}/GraphManager
    ${PROJECT_SRC_ROOT}/HttpService
    )
target_link_libraries(BicycleCensus
    ${DDK_HOST_LIBRARIES}
    ${DDK_PROTOBUF_LIBRARYS}
    ${DDK_HOST_OPENCV_LIBRARYS}
    ${PROJECT_SRC_ROOT}/libs/libgoahead-mbedtls.a
    ${PROJECT_SRC_ROOT}/libs/libmbedtls.a
    ${PROJECT_SRC_ROOT}/libs/libcurl.a
    ${PROJECT_SRC_ROOT}/libs/libssl.a
    ${PROJECT_SRC_ROOT}/libs/libcrypto.a
    -lpthread
    -ldl
    -lm
    -lz
    )

aux_source_directory(${PROJECT_SRC_ROOT} main_src)
add_executable(main ${main_src})
target_include_directories(main PUBLIC
    ${PROJECT_SRC_ROOT}/include
    ${PROJECT_SRC_ROOT}/BicycleDetectionModule
    ${PROJECT_SRC_ROOT}/DataTypes
    ${PROJECT_SRC_ROOT}/Sqlite3Manager
    ${PROJECT_SRC_ROOT}/Sqlite3Manager/SQLiteCpp
    ${PROJECT_SRC_ROOT}/ParamManager)
target_link_libraries(main BicycleCensus ${DDK_HOST_LIBRARIES} ${DDK_PROTOBUF_LIBRARYS} ${DDK_HOST_OPENCV_LIBRARYS} -lpthread -ldl -lm)



