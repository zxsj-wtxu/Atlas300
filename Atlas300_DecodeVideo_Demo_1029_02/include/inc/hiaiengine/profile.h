/**
* @file profile.h
*
* Copyright (C) <2018>  <Huawei Technologies Co., Ltd.>. All Rights Reserved.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#ifndef INC_HIAIENGINE_PROFILE_H_
#define INC_HIAIENGINE_PROFILE_H_


#include <mutex>
#include <string>
#include <map>
#include <set>
#include <iterator>
#include <vector>
#include <condition_variable>
#include <fstream>
#include <memory>
#include <thread>
#include <mmpa/mmpa_api.h>

#ifndef HIAI_LIB_VISIBILITY
#if defined _WIN32 || defined __CYGWIN__
#if (defined(HIAIENGINE_MATRIX_DEVICE_LIB) && !defined(HIAIENGINE_MINI_RC))
#define HIAI_LIB_HOST_VISIBILITY
#else
#define HIAI_LIB_HOST_VISIBILITY __declspec(dllexport)
#endif
#if (defined(HIAIENGINE_MATRIX_HOST_LIB) & !defined(HIAIENGINE_MINI_RC))
#define HIAI_LIB_DEVICE_VISIBILITY
#else
#define HIAI_LIB_DEVICE_VISIBILITY __declspec(dllexport)
#endif
#define HIAI_LIB_VISIBILITY __declspec(dllexport)
#define HIAI_LIB_INTERNAL
#else
#if (defined(HIAIENGINE_MATRIX_DEVICE_LIB) && !defined(HIAIENGINE_MINI_RC))
#define HIAI_LIB_HOST_VISIBILITY  __attribute__ ((visibility ("hidden")))
#else
#define HIAI_LIB_HOST_VISIBILITY  __attribute__ ((visibility ("default")))
#endif
#if (defined(HIAIENGINE_MATRIX_HOST_LIB) & !defined(HIAIENGINE_MINI_RC))
#define HIAI_LIB_DEVICE_VISIBILITY  __attribute__ ((visibility ("hidden")))
#else
#define HIAI_LIB_DEVICE_VISIBILITY  __attribute__ ((visibility ("default")))
#endif
#define HIAI_LIB_VISIBILITY __attribute__ ((visibility ("default")))
#define HIAI_LIB_INTERNAL __attribute__ ((visibility ("hidden")))
#endif
#endif

namespace hiai {
    // 纳秒乘子
    constexpr uint64_t NANOSECOND_MULTI = 1000000000UL;
    constexpr uint32_t MAX_PROFILE_NUM = 1000;
    typedef enum {
        PROFILESTATIC_START = 0,
        PROFILESTATIC_END
    } StaticTimeType;

    typedef struct {
        uint32_t threadID;     // 线程号
        uint32_t profileType;  // 统计数据类型
        uint64_t requestID;    // Profiling用来区分每次统计数据的请求号
        uint64_t startTime;    // 统计开始时间
        uint64_t endTime;      // 统计结束时间
        uint64_t reqID;        // HiAIEngine进行数据处理的请求号
        StaticTimeType timeType;
    } StaticInfo;

    class ProfileStatistic {
      public:
        /**
        * @ingroup hiaiengine
        * @brief ProfileStatistic构造函数
        */
        HIAI_LIB_VISIBILITY ProfileStatistic();

        HIAI_LIB_VISIBILITY ~ProfileStatistic() {
        }

        /** @ingroup ProfileStatistic
        *  @brief  ProfileStatisticString拿到性能统计数据
        *  @param [in]graphId : 需要输出性能数据的graphId
        *  @return 性能统计数据
        */
        HIAI_LIB_VISIBILITY std::string ProfileStatisticString(const uint32_t& graphId);

        /**
        * @ingroup hiaiengine
        * @brief ProfileSingleEngine 性能统计单一Engine
        * @param [in ]profileInfoVec:profile信息列表
        */
        HIAI_LIB_VISIBILITY std::string  ProfileSingleEngine(
            const std::vector<StaticInfo>& profileInfoVec);

        /**
        * @ingroup hiaiengine
        * @brief RecordStartTime 设置记录开始时间
        * @param [in ]profileType:profile类型
        */
        HIAI_LIB_VISIBILITY void RecordStartTime(const uint32_t& profileType);

        /**
        * @ingroup hiaiengine
        * @brief RecordEndTime 设置记录结束时间
        * @param [in ]profileType:profile类型
        * @param [in ]reqID:请求ID
        */
        HIAI_LIB_VISIBILITY void RecordEndTime(const uint32_t& profileType,
            const uint64_t& reqID = 0);

        /**
        * @ingroup hiaiengine
        * @brief RecordGraphStartTime 设置Graph记录开始时间
        * @param [in ] graphId : 需要记录的Graph开始时间的GraphId
        */
        HIAI_LIB_VISIBILITY void RecordGraphStartTime(const uint32_t& graphId);

        /**
        * @ingroup hiaiengine
        * @brief RecordGraphEndTime 设置记录Graph结束时间
        * @param [in ] graphId : 需要记录的Graph结束时间的GraphId
        */
        HIAI_LIB_VISIBILITY void RecordGraphEndTime(
            const uint32_t& graphId);

        /**
        * @ingroup hiaiengine
        * @brief RegisterType 注册性能统计类型
        * @param [in ]typeID:类型ID
        * @param [in ]profileTypeDes:性能统计的目标类型
        */
        HIAI_LIB_VISIBILITY void RegisterType(const uint32_t& typeID,
            const std::string& profileTypeDes);

        /**
        * @ingroup hiaiengine
        * @brief AddEngineID 增加Engine ID
        * @param [in ]threadID:线程ID
        * @param [in ]engineName:Engine名称
        * @param [in ]graphId:graph编号
        */
        HIAI_LIB_VISIBILITY void AddEngineID(const uint32_t& threadID,
            const std::string& engineName, const uint32_t& graphId);

        /**
        * @ingroup hiaiengine
        * @brief NeedWrite 是否需要写文件 判断是否需要些Profile
        * return 是否需要
        */
        HIAI_LIB_VISIBILITY bool NeedWrite();

        /**
        * @ingroup hiaiengine
        * @brief SetProfileSwitch
        * @param [in ]switchForProfile:Profile开关
        */
        HIAI_LIB_VISIBILITY static void SetProfileSwitch(bool switchForProfile);

        /**
        * @ingroup hiaiengine
        * @brief IsProfileEnable
        * @param return是否打开Profiling的标志位的引用
        */
        HIAI_LIB_VISIBILITY static bool& IsProfileEnable();

        /**
        * @ingroup hiaiengine
        * @brief StopWait 停止等待,释放掉等待变量
        */
        HIAI_LIB_VISIBILITY void StopWait();

      public:
        /**
        * @ingroup hiaiengine
        * @brief ProfileStatistic单例类获得实例的接口
        * return ProfileStatistic单例类实例指针
        */
        HIAI_LIB_VISIBILITY static ProfileStatistic* GetInstance();
        /**
        * @ingroup hiaiengine
        * @brief ProfileGetCurrentTime 获得当前时间
        * return 当前时间
        */
        HIAI_LIB_VISIBILITY uint64_t ProfileGetCurrentTime();

        /**
        * @ingroup hiaiengine
        * @brief GetMutex 获得锁
        * return 锁
        */
        HIAI_LIB_VISIBILITY static std::mutex& GetMutex();

        /**
        * @ingroup hiaiengine
        * @brief GetMutex 获得锁
        * return 锁
        */
        HIAI_LIB_VISIBILITY static std::mutex& GetUpdateMutex();
      private:
        HIAI_LIB_INTERNAL ProfileStatistic(const ProfileStatistic&) = delete;
        HIAI_LIB_INTERNAL ProfileStatistic(ProfileStatistic&&) = delete;
        HIAI_LIB_INTERNAL ProfileStatistic& operator=(const ProfileStatistic&) = delete;
        HIAI_LIB_INTERNAL ProfileStatistic& operator=(ProfileStatistic&&) = delete;
      private:
        std::map<uint32_t, uint64_t> graphStartTimesMap;
        std::map<uint32_t, uint64_t> graphEndTimesMap;
        std::map<uint32_t, std::vector<StaticInfo> > profileStartMap;
        std::vector<StaticInfo> profileEndVec;
        // 存放threadID和engine的engineName的映射关系
        std::map<uint32_t, std::string> engineMap;
        // 存放threadID和engine所属的graphID的映射关系
        std::map<uint32_t, uint32_t> graphMap;
        std::map<uint64_t, uint64_t> requestIdMap;
        std::map<uint32_t, std::string> profileTypeStrings;
        std::mutex updateProfileMutex;
        std::condition_variable updateProfileCv;
    };

    class ProfileTypeRegister {
      public:
        /**
        * @ingroup hiaiengine
        * @brief ProfileTypeRegister
        * @param [in]ProfileTypeRegister
        * @param [in]desc注册profile_type的描述
        */
        HIAI_LIB_VISIBILITY ProfileTypeRegister(const uint32_t& profileType,
            const std::string& profileDes) {
            ProfileStatistic::GetInstance()->RegisterType(
                profileType, profileDes);
        }
      public:
        HIAI_LIB_VISIBILITY ProfileTypeRegister() {}
        HIAI_LIB_VISIBILITY ~ProfileTypeRegister() {}
      private:
        HIAI_LIB_INTERNAL ProfileTypeRegister(const ProfileTypeRegister&) = delete;
        HIAI_LIB_INTERNAL ProfileTypeRegister(ProfileTypeRegister&&) = delete;
        HIAI_LIB_INTERNAL ProfileTypeRegister& operator=(const ProfileTypeRegister&) = delete;
        HIAI_LIB_INTERNAL ProfileTypeRegister& operator=(ProfileTypeRegister&&) = delete;
    };

    // 全流程打点
    /**
    * @ingroup hiaiengine
    * @brief GetMutex 获得锁
    * return 锁
    */
    inline std::mutex& ProfileStatistic::GetMutex() {
        static std::mutex mutexHandle;
        return mutexHandle;
    }


    inline std::mutex& ProfileStatistic::GetUpdateMutex()
    {
        static std::mutex updateMutex;
        return updateMutex;
    }
    /**
    * @ingroup hiaiengine
    * @brief ProfileStatistic单例类获得实例的接口
    * return ProfileStatistic单例类实例指针
    */
    inline ProfileStatistic* ProfileStatistic::GetInstance() {
        static ProfileStatistic instance;
        return &instance;
    }

    /**
    * @ingroup hiaiengine
    * @brief ProfileStatistic构造函数
    */
    inline ProfileStatistic::ProfileStatistic() {}

    /**
    * @ingroup hiaiengine
    * @brief SetProfileSwitch 设置性能统计开关
    * @param [in ] 性能统计开关数值
    */
    inline void ProfileStatistic::SetProfileSwitch(bool switchForProfile) {
        IsProfileEnable() = switchForProfile;
        return;
    }

    /**
    * @ingroup hiaiengine
    * @brief IsProfileEnable
    * @param return是否打开Profiling的标志位的引用
    */
    inline bool& ProfileStatistic::IsProfileEnable() {
        static bool setProfileSwitch = false;
        return setProfileSwitch;
    }

    /** @ingroup hiaiengine_profile _statistic
    *  @brief  Statistic::StopWait, 释放掉等待变量
    */
    inline void ProfileStatistic::StopWait() {
        std::unique_lock<std::mutex> lk(GetUpdateMutex());
        updateProfileCv.notify_all();
    }

    /** @ingroup hiaiengine_profile _statistic
    *  @brief  Statistic::NeedWrite 判断是否需要些Profile
    *  @param [in]
    *  @return bool
    */
    inline bool ProfileStatistic::NeedWrite() {
        // 如果profile数据不够多， 则不需要进行数据写入，阻塞等待
        std::unique_lock<std::mutex> lk(GetUpdateMutex());
        if (profileEndVec.size() < MAX_PROFILE_NUM) {
            updateProfileCv.wait(lk);
        }
        return true;
    }

    /**
    * @ingroup hiaiengine
    * @brief RecordStartTime 设置记录开始时间
    * @param [in ]profileType:profile类型
    */
    inline void ProfileStatistic::RecordStartTime(const uint32_t& profileType) {
        // 如果Profile未开，无需继续Profile数据
        if (!IsProfileEnable()) {
            return;
        }
        uint64_t currentTime = ProfileGetCurrentTime();
        uint32_t currentTid  = mmGetTid();
        uint64_t findID = ((uint64_t)currentTid << 32) + profileType;
        StaticInfo profilestaticInfo = { 0 };
        profilestaticInfo.threadID = currentTid;
        profilestaticInfo.timeType = PROFILESTATIC_START;
        profilestaticInfo.startTime = currentTime;
        profilestaticInfo.endTime = 0;
        profilestaticInfo.profileType = profileType;

        {
            std::lock_guard<std::mutex> lock(GetMutex());
            // 记录并计算 requestID
            std::map<uint64_t, uint64_t>::iterator iter =
                requestIdMap.find(findID);
            if (iter != requestIdMap.end()) {
                uint64_t& requestID = iter->second;
                requestID++;
                profilestaticInfo.requestID = requestID;
            } else {
                uint64_t requestID = (0xFFFFFFFF & (uint64_t)currentTid) << 32;
                profilestaticInfo.requestID = requestID;
                requestIdMap[findID] = requestID;
            }

            std::map<uint32_t, std::vector<StaticInfo> >::iterator
                profileIter = profileStartMap.find(profileType);
            if (profileIter != profileStartMap.end()) {
                std::vector<StaticInfo>& profileStartVec =
                    profileIter->second;
                profileStartVec.push_back(profilestaticInfo);
            } else {
                std::vector<StaticInfo> profileStartVec;
                profileStartVec.push_back(profilestaticInfo);
                profileStartMap[profileType] = profileStartVec;
            }
        }
        return;
    }

    /**
    * @ingroup hiaiengine
    * @brief RecordEndTime 设置记录结束时间
    * @param [in ]profileType:profile类型
    * @param [in ]reqID:请求ID
    */
    inline void ProfileStatistic::RecordEndTime(const uint32_t& profileType,
        const uint64_t& reqID) {
        // 如果Profile未开，无需继续Profile数据
        if (!IsProfileEnable()) {
            return;
        }
        uint64_t currentTime = ProfileGetCurrentTime();
        uint32_t currentTid  = mmGetTid();
        uint64_t findID = ((uint64_t)currentTid << 32) + profileType;

        StaticInfo profilestaticInfo = { 0 };
        profilestaticInfo.threadID = currentTid;
        profilestaticInfo.timeType = PROFILESTATIC_END;
        profilestaticInfo.startTime = 0;
        profilestaticInfo.endTime = currentTime;
        profilestaticInfo.profileType = profileType;
        profilestaticInfo.reqID = reqID;
        // 将info push进ProfileEndVec
        {
            std::lock_guard<std::mutex> lock(GetMutex());
            // 获取记录上次的request_id
            std::map<uint64_t, uint64_t>::iterator iter =
                requestIdMap.find(findID);
            if (iter != requestIdMap.end()) {
                profilestaticInfo.requestID = iter->second;
            } else {
                profilestaticInfo.requestID = 0;
            }

            profileEndVec.push_back(profilestaticInfo);
        }
        // 如果Vec数量大于max profile num, 则发送通知，放掉阻塞
        {
            std::unique_lock<std::mutex> lk(GetUpdateMutex());
            if (profileEndVec.size() >= MAX_PROFILE_NUM) {
                updateProfileCv.notify_all();
            }
        }
        return;
    }

    /**
    * @ingroup hiaiengine
    * @brief RecordGraphStartTime 设置Graph记录开始时间
    * @param [in ] graphId : 需要记录的Graph开始时间的GraphId
    */
    inline void ProfileStatistic::RecordGraphStartTime(const uint32_t& graphId) {
        uint64_t currentTime = ProfileGetCurrentTime();
        std::lock_guard<std::mutex> lock(GetMutex());
        graphStartTimesMap[graphId] = currentTime;
        return;
    }

    /**
    * @ingroup hiaiengine
    * @brief RecordGraphEndTime 设置记录Graph结束时间
    * @param [in ]graphId : 需要记录的Graph结束时间的GraphId
    */
    inline void ProfileStatistic::RecordGraphEndTime(const uint32_t& graphId) {
        uint64_t currentTime = ProfileGetCurrentTime();
        std::lock_guard<std::mutex> lock(GetMutex());
        graphEndTimesMap[graphId] = currentTime;
        return;
    }

    /**
    * @ingroup hiaiengine
    * @brief RegisterType 注册性能统计类型
    * @param [in ]typeID:类型ID
    * @param [in ]profileTypeDes:性能统计的目标类型
    */
    inline void ProfileStatistic::RegisterType(const uint32_t& typeID,
        const std::string& profileTypeDes) {
        std::lock_guard<std::mutex> lock(GetMutex());
        std::map<uint32_t, std::string>::iterator iter =
            profileTypeStrings.find(typeID);
        if (iter == profileTypeStrings.end()) {
            profileTypeStrings[typeID] = profileTypeDes;
        }
        return;
    }

    /**
    * @ingroup hiaiengine
    * @brief AddEngineID 增加Engine ID
    * @param [in ]requestID:请求ID
    * @param [in ]engineName:Engine名称
    */
    inline void ProfileStatistic::AddEngineID(const uint32_t& threadID,
        const std::string& engineName, const uint32_t& graphId) {
        std::lock_guard<std::mutex> lock(GetMutex());
        std::map<uint32_t, std::string>::iterator iter =
            engineMap.find(threadID);
        if (iter == engineMap.end()) {
            engineMap[threadID] = engineName;
            graphMap[threadID] = graphId;
        }
        return;
    }

    /** @ingroup Statistic
    *  @brief  ProfileSingleEngine: 统计单个Engine的所有Profile时间
    *  @param [in]
    *  @return HIAI_StatusT
    */
    inline std::string ProfileStatistic::ProfileSingleEngine
        (const std::vector<StaticInfo>& profileInfoVec) {
        // 对于所有预设的Profile_type: Full_process,
        // Dvpp_process, AiModelmanager_process, 预先分配vector，加快运算
        // 所有自定义的profile_type，只能通过计算
        std::string staticEngineString;
        for (auto& endProfileInfo : profileInfoVec) {
            std::map<uint32_t, std::vector<StaticInfo> >::iterator
                profileStartIter =
                profileStartMap.find(endProfileInfo.profileType);
            // 未找到， 跳到下一条
            if (profileStartIter == profileStartMap.end()) {
                continue;
            }

            // 匹配到对应的map，从map中找寻能够匹配到的Item
            std::vector<StaticInfo>& profileTypeInfo =
                profileStartIter->second;
            std::vector<StaticInfo>::iterator profileIter =
                profileTypeInfo.begin();
            while (profileIter != profileTypeInfo.end()) {
                if ((profileIter->requestID == endProfileInfo.requestID) &&
                    (profileIter->threadID == endProfileInfo.threadID)) {
                    if ("full_process" == profileTypeStrings
                        [endProfileInfo.profileType]){
                        staticEngineString += "request_id="
                            + std::to_string(endProfileInfo.reqID)+"\n";
                    }
                    staticEngineString +=
                        profileTypeStrings[endProfileInfo.profileType]
                        + "=" + std::to_string(profileIter->startTime)
                        + "," + std::to_string(endProfileInfo.endTime)
                        + "," + std::to_string(endProfileInfo.endTime
                        - profileIter->startTime) + "\n";
                    // 如果命中， 则删除该项
                    profileTypeInfo.erase(profileIter);
                    break;
                }
                profileIter++;
            }
        }
        return staticEngineString;
    }

    /** @ingroup ProfileStatistic
    *  @brief  ProfileStatisticString拿到性能统计数据
    *  @param [in]countGraph : 是否需要Graph数据
    *  @return 拿到性能统计数据
    */
    inline std::string ProfileStatistic::ProfileStatisticString(const uint32_t& graphId) {
        std::string statisticString = "";
        uint32_t calIndex = 100;
        // 按照engine thread_id归类
        std::map<std::string, std::vector<StaticInfo> >
            profileStaticItem;

        std::lock_guard<std::mutex> lock(GetMutex());
        if (engineMap.size() == 0) {
            return "";
        }
        auto profileEndVecIter = profileEndVec.begin();
        while (profileEndVecIter != profileEndVec.end()) {
            const std::string& engineName = engineMap[profileEndVecIter->threadID];
            uint32_t recordGraphId = graphMap[profileEndVecIter->threadID];
            if ((!engineName.empty()) && (recordGraphId == graphId)) {
                std::map<std::string, std::vector<StaticInfo> >::
                    iterator iter = profileStaticItem.find(engineName);
                if (iter != profileStaticItem.end()) {
                    std::vector<StaticInfo>& profileStaticItemVec =
                    iter->second;
                    profileStaticItemVec.push_back(*profileEndVecIter);
                } else {
                    std::vector<StaticInfo> profileStaticItemVec;
                    profileStaticItemVec.push_back(*profileEndVecIter);
                    profileStaticItem[engineName] = profileStaticItemVec;
                }
                profileEndVec.erase(profileEndVecIter);
                continue;
            }
            profileEndVecIter++;
        }

        // 逐个engine计算
        for (auto& profileItem: profileStaticItem) {
            calIndex++;
            std::vector<StaticInfo>& infoVec = profileItem.second;
            statisticString += "[" +
                std::to_string(infoVec[0].threadID) + "]" + "\n";
            statisticString += "engine_name=" + profileItem.first + "\n";
            statisticString += ProfileSingleEngine(infoVec);
        }

        // Graph Run time
        if ((graphStartTimesMap.count(graphId) != 0) && (graphEndTimesMap.count(graphId) != 0)) {
            calIndex++;
            statisticString += "[" + std::to_string(calIndex) + "]" + "\n" +
                "graph_run_time=" + std::to_string(graphStartTimesMap[graphId]) +
                "," + std::to_string(graphEndTimesMap[graphId]) +
                "," + std::to_string(graphEndTimesMap[graphId] - graphStartTimesMap[graphId]) + "\n";

            graphStartTimesMap.erase(graphId);
            graphEndTimesMap.erase(graphId);
        }

        return statisticString;
    }

    /**
    * @ingroup hiaiengine
    * @brief ProfileGetCurrentTime 获得当前时间
    * return 当前时间
    */
    inline uint64_t ProfileStatistic::ProfileGetCurrentTime() {
        mmTimespec spec = mmGetTickCount();
        return static_cast<uint64_t>(NANOSECOND_MULTI *
            ((uint64_t)spec.tv_sec) + (uint64_t)spec.tv_nsec);
    }
}

#define PROFILESTATIC_GRAPH_STARTTIME(graphId)                                 \
        hiai::ProfileStatistic::GetInstance()->                                \
            RecordGraphStartTime(graphId);                                     \

#define PROFILESTATIC_STARTTIME(profileType)                                   \
        hiai::ProfileStatistic::GetInstance()->                                \
            RecordStartTime(profileType);                                      \

#define PROFILESTATIC_GRAPH_ENDTIME(graphId)                                   \
        hiai::ProfileStatistic::GetInstance()->                                \
            RecordGraphEndTime(graphId);                                       \

#define PROFILESTATIC_ENDTIME(profileType)                                     \
        hiai::ProfileStatistic::GetInstance()->                                \
            RecordEndTime(profileType);                                        \

#define PROFILESTATIC_ENGINE_ENDTIME(profileType, reqID)                       \
        hiai::ProfileStatistic::GetInstance()->                                \
            RecordEndTime(profileType, reqID);                                 \

#define PROFILESTATIC_ADDENGINE(threadID, engineName, graphId)                 \
        hiai::ProfileStatistic::GetInstance()->                                \
            AddEngineID(threadID, engineName, graphId);                        \

#define PROFILESTATIC_REGTYPE(profileID, profileType, profileDes)              \
    constexpr uint32_t profileType =                                           \
        ((0xFFFF & ((uint16_t)profileID)) << 16) | (0x0FFF & __COUNTER__);     \
    const hiai::ProfileTypeRegister g_##profileType##_profiletype(             \
        profileType, profileDes);

#define PROFILE_MODEL_ENGINE    0x0101  // Hiaiengine Profile Mode

// Full Process
PROFILESTATIC_REGTYPE(PROFILE_MODEL_ENGINE, PROFILESTATIC_ENGINE_FULL_PROCESS,
    "full_process");
// DVPP Process
PROFILESTATIC_REGTYPE(PROFILE_MODEL_ENGINE, PROFILESTATIC_DVPP_PROCESS,
    "dvpp_process");
// AIModelManager Process
PROFILESTATIC_REGTYPE(PROFILE_MODEL_ENGINE,
    PROFILESTATIC_AIMODELMANAGER_PROCESS, "aimodelmanager_process");
// Graph RunTime
PROFILESTATIC_REGTYPE(PROFILE_MODEL_ENGINE,
    PROFILESTATIC_GRAPH_RUN, "graph_run_time");

PROFILESTATIC_REGTYPE(PROFILE_MODEL_ENGINE,
    PROFILESTATIC_DATA_INPUT, "aimodelmanager_datainput");

PROFILESTATIC_REGTYPE(PROFILE_MODEL_ENGINE,
    PROFILESTATIC_AIMODELMANAGER_PREDICT, "aimodelmanager_predict");

#endif  // INC_HIAIENGINE_PROFILE_H_
