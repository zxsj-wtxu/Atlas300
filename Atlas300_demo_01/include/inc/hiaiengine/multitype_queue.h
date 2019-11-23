/**
* @file multitype_queue.h
*
* Copyright (C) <2018>  <Huawei Technologies Co., Ltd.>. All Rights Reserved.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#ifndef INC_HIAIENGINE_MULTITYPE_QUEUE_H_
#define INC_HIAIENGINE_MULTITYPE_QUEUE_H_

#include <queue>
#include <memory>
#include <map>
#include <mutex>
#include <chrono>
#include "hiaiengine/log.h"

namespace hiai {

    /**
    * @Name: MultiTypeQueue
    * @Description: 支持多种类型的消息队列
    *
    */
    class MultiTypeQueue {
    public:

        using MultiQueueTimeType = std::chrono::time_point<
            std::chrono::system_clock, std::chrono::milliseconds>;

        HIAI_LIB_VISIBILITY ~MultiTypeQueue() {}
        /**
        * @ingroup hiaiengine
        * @brief: 构造 MultiTypeQueue
        * @param [in]que_num： 指定队列个数
        * @param [in]maxQueLen: 指定每个队列的最大长度
        * @param [in]durationMs: 指定队列成员从入对到出对最大时间间隔。
        * 当某个成员从入对(Push)起，超过duration_ms 时间后还未出对
        * (（Pop)，则该成员会被自动删除
        */
        HIAI_LIB_VISIBILITY explicit MultiTypeQueue(uint32_t queNum,
            uint32_t maxQueLen = 1024, uint32_t durationMs = 0) :
            qumNum_(queNum),
            durationMs_(durationMs),
            maxQueLen_(maxQueLen),
            isAutoDel_(durationMs > 0) {}

        /**
        * @ingroup hiaiengine
        * @brief: PushData 往指定队列插入数据
        * @param [in]q_idx： 队列编号（0 ~ queNum-1）
        * @param [in]dataPtr: 队列函数指针
        * @param [out]bool: 成功则返回true，否则返回false(例如队列满等）
        */
        HIAI_LIB_VISIBILITY bool PushData(uint32_t qIndex,
            const std::shared_ptr<void>& dataPtr) {
            if (dataPtr == nullptr) {
                return false;
            }

            if (qIndex >= qumNum_) {
                return false;
            }

            std::lock_guard<std::mutex> guard(mutex_);
            auto iter = multiQueue_.find(qIndex);

            std::shared_ptr<std::queue<std::pair<MultiQueueTimeType,
                std::shared_ptr<void> > > > qPointer = nullptr;
            if (iter == multiQueue_.end()) {
                qPointer = std::make_shared< std::queue <std::pair <
                MultiQueueTimeType, std::shared_ptr<void> > > >();
                try {
                    qPointer = std::make_shared< std::queue <std::pair <
                        MultiQueueTimeType, std::shared_ptr<void> > > >();
                } catch(...) {
                    HIAI_ENGINE_LOG(HIAI_MEMORY_EXHAUSTED,
                       "std::makeshard error");
                    return false;
                }
                if (qPointer == nullptr) {
                    HIAI_ENGINE_LOG(HIAI_MEMORY_EXHAUSTED,
                       "std::makeshard error");
                    return false;
                }
                multiQueue_[qIndex] = qPointer;
            } else {
                qPointer = (*iter).second;
            }

            if (qPointer->size() >= maxQueLen_) {
                return false;
            }

            auto curTime = std::chrono::time_point_cast<
                std::chrono::milliseconds>(
                std::chrono::system_clock::now());
            qPointer->push(std::make_pair(curTime, dataPtr));
            return true;
        }

        /**
        * @ingroup hiaiengine
        * @brief: FrontData 读取指定队列头部数据（单并不从队列中删除）
        * @param [in]q_idx： 队列编号（0 ~ queNum-1）
        * @param [out]dataPtr: 队列函数指针
        * @param [out]bool: 成功则返回true，否则返回false(例如队列为空等）
        */
        HIAI_LIB_VISIBILITY bool FrontData(uint32_t qIndex,
            std::shared_ptr<void>& dataPtr) {
            std::lock_guard<std::mutex> guard(mutex_);
            auto iter = multiQueue_.find(qIndex);
            if (iter == multiQueue_.end()) {
                return false;
            }
            std::shared_ptr<std::queue<std::pair<MultiQueueTimeType,
                std::shared_ptr<void> > > > qPointer = (*iter).second;

            if (qPointer->empty()) {
                return false;
            }

            dataPtr = (qPointer->front()).second;
            return true;
        }

        /**
        * @ingroup hiaiengine
        * @brief: PopData 读取指定队列头部数据，并从队列中删除
        * @param [in]q_idx： 队列编号（0 ~ queNum-1）
        * @param [out]dataPtr: 队列函数指针
        * @param [out]bool: 成功则返回true，否则返回false(例如队列为空等）
        */
        HIAI_LIB_VISIBILITY bool PopData(uint32_t qIndex,
            std::shared_ptr<void>& dataPtr) {
            std::lock_guard<std::mutex> guard(mutex_);
            auto iter = multiQueue_.find(qIndex);
            if (iter == multiQueue_.end()) {
                return false;
            }

            //队列为空时返回false
            auto qPointer =  (*iter).second;
            if (qPointer->empty()) {
                return false;
            }

            //根据需要排除超时数据
            auto curTime = std::chrono::time_point_cast<
                std::chrono::milliseconds>(
                std::chrono::system_clock::now());
            auto quePair = qPointer->front();
            if (isAutoDel_) {
                std::chrono::milliseconds diffTime(
                    curTime - quePair.first);
                while (diffTime.count() >= durationMs_) {
                    HIAI_ENGINE_LOG("MultiTypeQueue delete queue element "
                        "when max_duration_time is %u ms "
                        "and diff time is %d ms",
                        durationMs_, diffTime.count());
                    qPointer->pop();
                    if (qPointer->empty()) {
                        return false;
                    }
                    quePair = qPointer->front();
                    diffTime = std::chrono::milliseconds(
                        curTime - quePair.first);
                }
            }

            dataPtr = (qPointer->front()).second;
            qPointer->pop();
            return true;
        }

        /**
        * @ingroup hiaiengine
        * @brief: PopAllData 读取所有队列头部数据。
        *         仅当所有队列头部都有数据时才读取成功，并删除队列头部数据。
        *         否则，返回失败但并不删除任何数据。
        *        该函数仅当只有一个队列时才使用。
        * @param [out]arg1: 队列函数指针
        * @param [out]bool: 仅当所有队列头部都有数据时才读取成功并返回true，
        * 否则返回false
        */
        template<typename T1>
        HIAI_LIB_VISIBILITY bool PopAllData(std::shared_ptr<T1>& arg1) {
            if (qumNum_ != 1) {
                return false;
            }

            std::shared_ptr<void> arg;

            if (!PopData(0, arg)) {
                return false;
            }

            arg1 = std::static_pointer_cast<T1>(arg);
            return true;
        }

        /**
        * @ingroup hiaiengine
        * @brief: PopAllData 读取所有队列头部数据。
        *         仅当所有队列头部都有数据时才读取成功，并删除队列头部数据。
        *         否则，返回失败但并不删除任何数据。
        *        该函数仅当只有两个队列时才使用。
        * @param [out]arg1, arg2: 队列函数指针
        * @param [out]bool: 仅当所有队列头部都有数据时才读取成功并返回true，
        * 否则返回false
        */
        template<typename T1, typename T2>
        HIAI_LIB_VISIBILITY bool PopAllData(std::shared_ptr<T1>& arg1,
            std::shared_ptr<T2>& arg2) {
            if (qumNum_ != 2) {
                return false;
            }

            std::vector<std::shared_ptr<void>> args;
            if (!PopAllData(args)) {
                return false;
            }

            arg1 = std::static_pointer_cast<T1>(args[0]);
            arg2 = std::static_pointer_cast<T2>(args[1]);
            return true;
        }

        /**
        * @ingroup hiaiengine
        * @brief: PopAllData 读取所有队列头部数据。
        *         仅当所有队列头部都有数据时才读取成功，并删除队列头部数据。
        *         否则，返回失败但并不删除任何数据。
        *        该函数仅当只有三个队列时才使用。
        * @param [out]arg1, arg2， arg3: 队列函数指针
        * @param [out]bool: 仅当所有队列头部都有数据时才读取成功并返回true，
        * 否则返回false
        */
        template<typename T1, typename T2, typename T3>
        HIAI_LIB_VISIBILITY bool PopAllData(std::shared_ptr<T1>& arg1,
            std::shared_ptr<T2>& arg2, std::shared_ptr<T3>& arg3) {
            if (qumNum_ != 3) {
                return false;
            }

            std::vector<std::shared_ptr<void>> args;
            if (!PopAllData(args)) {
                return false;
            }

            arg1 = std::static_pointer_cast<T1>(args[0]);
            arg2 = std::static_pointer_cast<T2>(args[1]);
            arg3 = std::static_pointer_cast<T3>(args[2]);

            return true;
        }

        /**
        * @ingroup hiaiengine
        * @brief: PopAllData 读取所有队列头部数据。
        *         仅当所有队列头部都有数据时才读取成功，并删除队列头部数据。
        *         否则，返回失败但并不删除任何数据。
        *        该函数仅当只有四个队列时才使用。
        * @param [out]arg1, arg2， arg3， arg4: 队列函数指针
        * @param [out]bool: 仅当所有队列头部都有数据时才读取成功并返回true，
        * 否则返回false
        */    
        template<typename T1, typename T2, typename T3, typename T4>
        HIAI_LIB_VISIBILITY bool PopAllData(std::shared_ptr<T1>& arg1,
            std::shared_ptr<T2>& arg2, std::shared_ptr<T3>& arg3,
            std::shared_ptr<T4>& arg4) {
            if (qumNum_ != 4) {
                return false;
            }

            std::vector<std::shared_ptr<void>> args;
            if (!PopAllData(args)) {
                return false;
            }

            arg1 = std::static_pointer_cast<T1>(args[0]);
            arg2 = std::static_pointer_cast<T2>(args[1]);
            arg3 = std::static_pointer_cast<T3>(args[2]);
            arg4 = std::static_pointer_cast<T4>(args[3]);
            return true;
        }

        /**
        * @ingroup hiaiengine
        * @brief: PopAllData 读取所有队列头部数据。
        *         仅当所有队列头部都有数据时才读取成功，并删除队列头部数据。
        *         否则，返回失败但并不删除任何数据。
        *        该函数仅当只有五个队列时才使用。
        * @param [out]arg1, arg2， arg3， arg4， arg5: 队列函数指针
        * @param [out]bool: 仅当所有队列头部都有数据时才读取成功并返回true，
        * 否则返回false
        */
        template<typename T1, typename T2, typename T3,
                 typename T4, typename T5>
        HIAI_LIB_VISIBILITY bool PopAllData(std::shared_ptr<T1>& arg1,
            std::shared_ptr<T2>& arg2, std::shared_ptr<T3>& arg3,
            std::shared_ptr<T4>& arg4, std::shared_ptr<T5>& arg5) {
            if (qumNum_ != 5) {
                return false;
            }

            std::vector<std::shared_ptr<void>> args;
            if (!PopAllData(args)) {
                return false;
            }

            arg1 = std::static_pointer_cast<T1>(args[0]);
            arg2 = std::static_pointer_cast<T2>(args[1]);
            arg3 = std::static_pointer_cast<T3>(args[2]);
            arg4 = std::static_pointer_cast<T4>(args[3]);
            arg5 = std::static_pointer_cast<T5>(args[4]);
            return true;
        }

        /**
        * @ingroup hiaiengine
        * @brief: PopAllData 读取所有队列头部数据。
        *         仅当所有队列头部都有数据时才读取成功，并删除队列头部数据。
        *         否则，返回失败但并不删除任何数据。
        *        该函数仅当只有六个队列时才使用。
        * @param [out]arg1, arg2， arg3， arg4, arg5, arg6: 队列函数指针
        * @param [out]bool: 仅当所有队列头部都有数据时才读取成功并返回true，
        * 否则返回false
        */
        template<typename T1, typename T2, typename T3,
            typename T4, typename T5, typename T6>
        HIAI_LIB_VISIBILITY bool PopAllData(std::shared_ptr<T1>& arg1,
            std::shared_ptr<T2>& arg2, std::shared_ptr<T3>& arg3,
            std::shared_ptr<T4>& arg4, std::shared_ptr<T5>& arg5,
            std::shared_ptr<T6>& arg6) {
            if (qumNum_ != 6) {
                return false;
            }

            std::vector<std::shared_ptr<void>> args;
            if (!PopAllData(args)) {
                return false;
            }

            arg1 = std::static_pointer_cast<T1>(args[0]);
            arg2 = std::static_pointer_cast<T2>(args[1]);
            arg3 = std::static_pointer_cast<T3>(args[2]);
            arg4 = std::static_pointer_cast<T4>(args[3]);
            arg5 = std::static_pointer_cast<T5>(args[4]);
            arg6 = std::static_pointer_cast<T6>(args[5]);
            return true;
        }

        /**
        * @ingroup hiaiengine
        * @brief: PopAllData 读取所有队列头部数据。
        *         仅当所有队列头部都有数据时才读取成功，并删除队列头部数据。
        *         否则，返回失败但并不删除任何数据。
        *        该函数仅当只有七个队列时才使用。
        * @param [out]arg1, arg2， arg3， arg4, arg5, arg6, arg7: 队列函数指针
        * @param [out]bool: 仅当所有队列头部都有数据时才读取成功并返回true，
        * 否则返回false
        */
        template<typename T1, typename T2, typename T3,
            typename T4, typename T5, typename T6, typename T7>
        HIAI_LIB_VISIBILITY bool PopAllData(std::shared_ptr<T1>& arg1,
            std::shared_ptr<T2>& arg2, std::shared_ptr<T3>& arg3,
            std::shared_ptr<T4>& arg4, std::shared_ptr<T5>& arg5,
            std::shared_ptr<T6>& arg6, std::shared_ptr<T7>& arg7) {
            if (qumNum_ != 7) {
                return false;
            }

            std::vector<std::shared_ptr<void>> args;
            if (!PopAllData(args)) {
                return false;
            }

            arg1 = std::static_pointer_cast<T1>(args[0]);
            arg2 = std::static_pointer_cast<T2>(args[1]);
            arg3 = std::static_pointer_cast<T3>(args[2]);
            arg4 = std::static_pointer_cast<T4>(args[3]);
            arg5 = std::static_pointer_cast<T5>(args[4]);
            arg6 = std::static_pointer_cast<T6>(args[5]);
            arg7 = std::static_pointer_cast<T7>(args[6]);
            return true;
        }

        /**
        * @ingroup hiaiengine
        * @brief: PopAllData 读取所有队列头部数据。
        *         仅当所有队列头部都有数据时才读取成功，并删除队列头部数据。
        *         否则，返回失败但并不删除任何数据。
        *        该函数仅当只有八个队列时才使用。
        * @param [out]arg1, arg2， arg3， arg4, arg5, arg6, arg7, arg8:
        *  队列函数指针
        * @param [out]bool: 仅当所有队列头部都有数据时才读取成功并返回true，
        * 否则返回false
        */
        template<typename T1, typename T2, typename T3,
            typename T4, typename T5, typename T6,
            typename T7, typename T8>
        HIAI_LIB_VISIBILITY bool PopAllData(std::shared_ptr<T1>& arg1,
            std::shared_ptr<T2>& arg2, std::shared_ptr<T3>& arg3,
            std::shared_ptr<T4>& arg4, std::shared_ptr<T5>& arg5,
            std::shared_ptr<T6>& arg6, std::shared_ptr<T7>& arg7,
            std::shared_ptr<T8>& arg8) {
            if (qumNum_ != 8) {
                return false;
            }

            std::vector<std::shared_ptr<void>> args;
            if (!PopAllData(args)) {
                return false;
            }

            arg1 = std::static_pointer_cast<T1>(args[0]);
            arg2 = std::static_pointer_cast<T2>(args[1]);
            arg3 = std::static_pointer_cast<T3>(args[2]);
            arg4 = std::static_pointer_cast<T4>(args[3]);
            arg5 = std::static_pointer_cast<T5>(args[4]);
            arg6 = std::static_pointer_cast<T6>(args[5]);
            arg7 = std::static_pointer_cast<T7>(args[6]);
            arg8 = std::static_pointer_cast<T8>(args[7]);
            return true;
        }

        /**
        * @ingroup hiaiengine
        * @brief: PopAllData 读取所有队列头部数据。
        *         仅当所有队列头部都有数据时才读取成功，并删除队列头部数据。
        *         否则，返回失败但并不删除任何数据。
        *        该函数仅当只有九个队列时才使用。
        * @param [out]arg1, arg2， arg3， arg4, arg5, arg6, arg7, arg8, arg9: 
        * 队列函数指针
        * @param [out]bool: 仅当所有队列头部都有数据时才读取成功并返回true，
        * 否则返回false
        */
        template<typename T1, typename T2, typename T3,
            typename T4, typename T5, typename T6,
            typename T7, typename T8, typename T9>
        HIAI_LIB_VISIBILITY bool PopAllData(std::shared_ptr<T1>& arg1,
            std::shared_ptr<T2>& arg2, std::shared_ptr<T3>& arg3,
            std::shared_ptr<T4>& arg4, std::shared_ptr<T5>& arg5,
            std::shared_ptr<T6>& arg6, std::shared_ptr<T7>& arg7,
            std::shared_ptr<T8>& arg8, std::shared_ptr<T9>& arg9) {
            if (qumNum_ != 9) {
                return false;
            }

            std::vector<std::shared_ptr<void>> args;
            if (!PopAllData(args)) {
                return false;
            }

            arg1 = std::static_pointer_cast<T1>(args[0]);
            arg2 = std::static_pointer_cast<T2>(args[1]);
            arg3 = std::static_pointer_cast<T3>(args[2]);
            arg4 = std::static_pointer_cast<T4>(args[3]);
            arg5 = std::static_pointer_cast<T5>(args[4]);
            arg6 = std::static_pointer_cast<T6>(args[5]);
            arg7 = std::static_pointer_cast<T7>(args[6]);
            arg8 = std::static_pointer_cast<T8>(args[7]);
            arg9 = std::static_pointer_cast<T9>(args[8]);
            return true;
        }

        /**
        * @ingroup hiaiengine
        * @brief: PopAllData 读取所有队列头部数据。
        *         仅当所有队列头部都有数据时才读取成功，并删除队列头部数据。
        *         否则，返回失败但并不删除任何数据。
        *        该函数仅当只有九个队列时才使用。
        * @param [out]arg1, arg2， arg3， arg4, arg5, arg6, arg7, arg8, 
        * arg9, arg10: 队列函数指针
        * @param [out]bool: 仅当所有队列头部都有数据时才读取成功并返回true，
        * 否则返回false
        */
        template<typename T1, typename T2, typename T3,
            typename T4, typename T5, typename T6,
            typename T7, typename T8, typename T9, typename T10>
        HIAI_LIB_VISIBILITY bool PopAllData(std::shared_ptr<T1>& arg1,
            std::shared_ptr<T2>& arg2, std::shared_ptr<T3>& arg3,
            std::shared_ptr<T4>& arg4, std::shared_ptr<T5>& arg5,
            std::shared_ptr<T6>& arg6, std::shared_ptr<T7>& arg7,
            std::shared_ptr<T8>& arg8, std::shared_ptr<T9>& arg9,
            std::shared_ptr<T10>& arg10) {
            if (qumNum_ != 10) {
                return false;
            }

            std::vector<std::shared_ptr<void>> args;
            if (!PopAllData(args)) {
                return false;
            }

            arg1 = std::static_pointer_cast<T1>(args[0]);
            arg2 = std::static_pointer_cast<T2>(args[1]);
            arg3 = std::static_pointer_cast<T3>(args[2]);
            arg4 = std::static_pointer_cast<T4>(args[3]);
            arg5 = std::static_pointer_cast<T5>(args[4]);
            arg6 = std::static_pointer_cast<T6>(args[5]);
            arg7 = std::static_pointer_cast<T7>(args[6]);
            arg8 = std::static_pointer_cast<T8>(args[7]);
            arg9 = std::static_pointer_cast<T9>(args[8]);
            arg10 = std::static_pointer_cast<T10>(args[9]);
            return true;
        }

        /**
        * @ingroup hiaiengine
        * @brief: PopAllData 读取所有队列头部数据。
        *         仅当所有队列头部都有数据时才读取成功，并删除队列头部数据。
        *         否则，返回失败但并不删除任何数据。
        *        该函数仅当只有十一个队列时才使用。
        * @param [out]arg1, arg2， arg3， arg4, arg5, arg6, arg7, arg8,
        * arg9, arg10, arg11: 队列函数指针
        * @param [out]bool: 仅当所有队列头部都有数据时才读取成功并返回true，
        * 否则返回false
        */
        template<typename T1, typename T2, typename T3,
            typename T4, typename T5, typename T6,
            typename T7, typename T8, typename T9, typename T10,
            typename T11>
        HIAI_LIB_VISIBILITY bool PopAllData(std::shared_ptr<T1>& arg1,
            std::shared_ptr<T2>& arg2, std::shared_ptr<T3>& arg3,
            std::shared_ptr<T4>& arg4, std::shared_ptr<T5>& arg5,
            std::shared_ptr<T6>& arg6, std::shared_ptr<T7>& arg7,
            std::shared_ptr<T8>& arg8, std::shared_ptr<T9>& arg9,
            std::shared_ptr<T10>& arg10, std::shared_ptr<T11>& arg11) {
            if (qumNum_ != 11) {
                return false;
            }

            std::vector<std::shared_ptr<void>> args;
            if (!PopAllData(args)) {
                return false;
            }

            arg1 = std::static_pointer_cast<T1>(args[0]);
            arg2 = std::static_pointer_cast<T2>(args[1]);
            arg3 = std::static_pointer_cast<T3>(args[2]);
            arg4 = std::static_pointer_cast<T4>(args[3]);
            arg5 = std::static_pointer_cast<T5>(args[4]);
            arg6 = std::static_pointer_cast<T6>(args[5]);
            arg7 = std::static_pointer_cast<T7>(args[6]);
            arg8 = std::static_pointer_cast<T8>(args[7]);
            arg9 = std::static_pointer_cast<T9>(args[8]);
            arg10 = std::static_pointer_cast<T10>(args[9]);
            arg11 = std::static_pointer_cast<T11>(args[10]);
            return true;
        }

        /**
        * @ingroup hiaiengine
        * @brief: PopAllData 读取所有队列头部数据。
        *         仅当所有队列头部都有数据时才读取成功，并删除队列头部数据。
        *         否则，返回失败但并不删除任何数据。
        *        该函数仅当只有十二个队列时才使用。
        * @param [out]arg1, arg2， arg3， arg4, arg5, arg6, arg7, arg8,
        * arg9, arg10, arg11, arg12: 队列函数指针
        * @param [out]bool: 仅当所有队列头部都有数据时才读取成功并返回true，
        * 否则返回false
        */
        template<typename T1, typename T2, typename T3,
            typename T4, typename T5, typename T6,
            typename T7, typename T8, typename T9, typename T10,
            typename T11, typename T12>
        HIAI_LIB_VISIBILITY bool PopAllData(std::shared_ptr<T1>& arg1,
            std::shared_ptr<T2>& arg2, std::shared_ptr<T3>& arg3,
            std::shared_ptr<T4>& arg4, std::shared_ptr<T5>& arg5,
            std::shared_ptr<T6>& arg6, std::shared_ptr<T7>& arg7,
            std::shared_ptr<T8>& arg8, std::shared_ptr<T9>& arg9,
            std::shared_ptr<T10>& arg10, std::shared_ptr<T11>& arg11,
            std::shared_ptr<T12>& arg12) {
            if (qumNum_ != 12) {
                return false;
            }

            std::vector<std::shared_ptr<void>> args;
            if (!PopAllData(args)) {
                return false;
            }

            arg1 = std::static_pointer_cast<T1>(args[0]);
            arg2 = std::static_pointer_cast<T2>(args[1]);
            arg3 = std::static_pointer_cast<T3>(args[2]);
            arg4 = std::static_pointer_cast<T4>(args[3]);
            arg5 = std::static_pointer_cast<T5>(args[4]);
            arg6 = std::static_pointer_cast<T6>(args[5]);
            arg7 = std::static_pointer_cast<T7>(args[6]);
            arg8 = std::static_pointer_cast<T8>(args[7]);
            arg9 = std::static_pointer_cast<T9>(args[8]);
            arg10 = std::static_pointer_cast<T10>(args[9]);
            arg11 = std::static_pointer_cast<T11>(args[10]);
            arg12 = std::static_pointer_cast<T12>(args[11]);
            return true;
        }

        /**
        * @ingroup hiaiengine
        * @brief: PopAllData 读取所有队列头部数据。
        *         仅当所有队列头部都有数据时才读取成功，并删除队列头部数据。
        *         否则，返回失败但并不删除任何数据。
        *        该函数仅当只有十三个队列时才使用。
        * @param [out]arg1, arg2， arg3， arg4, arg5, arg6, arg7, arg8,
        * arg9, arg10, arg11, arg12, arg13: 队列函数指针
        * @param [out]bool: 仅当所有队列头部都有数据时才读取成功并返回true，
        * 否则返回false
        */
        template<typename T1, typename T2, typename T3,
            typename T4, typename T5, typename T6,
            typename T7, typename T8, typename T9, typename T10,
            typename T11, typename T12, typename T13>
        HIAI_LIB_VISIBILITY bool PopAllData(std::shared_ptr<T1>& arg1,
            std::shared_ptr<T2>& arg2, std::shared_ptr<T3>& arg3,
            std::shared_ptr<T4>& arg4, std::shared_ptr<T5>& arg5,
            std::shared_ptr<T6>& arg6, std::shared_ptr<T7>& arg7,
            std::shared_ptr<T8>& arg8, std::shared_ptr<T9>& arg9,
            std::shared_ptr<T10>& arg10, std::shared_ptr<T11>& arg11,
            std::shared_ptr<T12>& arg12, std::shared_ptr<T13>& arg13) {
            if (qumNum_ != 13) {
                return false;
            }

            std::vector<std::shared_ptr<void>> args;
            if (!PopAllData(args)) {
                return false;
            }

            arg1 = std::static_pointer_cast<T1>(args[0]);
            arg2 = std::static_pointer_cast<T2>(args[1]);
            arg3 = std::static_pointer_cast<T3>(args[2]);
            arg4 = std::static_pointer_cast<T4>(args[3]);
            arg5 = std::static_pointer_cast<T5>(args[4]);
            arg6 = std::static_pointer_cast<T6>(args[5]);
            arg7 = std::static_pointer_cast<T7>(args[6]);
            arg8 = std::static_pointer_cast<T8>(args[7]);
            arg9 = std::static_pointer_cast<T9>(args[8]);
            arg10 = std::static_pointer_cast<T10>(args[9]);
            arg11 = std::static_pointer_cast<T11>(args[10]);
            arg12 = std::static_pointer_cast<T12>(args[11]);
            arg13 = std::static_pointer_cast<T13>(args[12]);
            return true;
        }

        /**
        * @ingroup hiaiengine
        * @brief: PopAllData 读取所有队列头部数据。
        *         仅当所有队列头部都有数据时才读取成功，并删除队列头部数据。
        *         否则，返回失败但并不删除任何数据。
        *        该函数仅当只有十四个队列时才使用。
        * @param [out]arg1, arg2， arg3， arg4, arg5, arg6, arg7, arg8,
        * arg9, arg10, arg11, arg12, arg13, arg14: 队列函数指针
        * @param [out]bool: 仅当所有队列头部都有数据时才读取成功并返回true，
        * 否则返回false
        */
        template<typename T1, typename T2, typename T3,
            typename T4, typename T5, typename T6,
            typename T7, typename T8, typename T9, typename T10,
            typename T11, typename T12, typename T13,
            typename T14>
        HIAI_LIB_VISIBILITY bool PopAllData(std::shared_ptr<T1>& arg1,
            std::shared_ptr<T2>& arg2, std::shared_ptr<T3>& arg3,
            std::shared_ptr<T4>& arg4, std::shared_ptr<T5>& arg5,
            std::shared_ptr<T6>& arg6, std::shared_ptr<T7>& arg7,
            std::shared_ptr<T8>& arg8, std::shared_ptr<T9>& arg9,
            std::shared_ptr<T10>& arg10, std::shared_ptr<T11>& arg11,
            std::shared_ptr<T12>& arg12, std::shared_ptr<T13>& arg13,
            std::shared_ptr<T14>& arg14) {
            if (qumNum_ != 14) {
                return false;
            }

            std::vector<std::shared_ptr<void>> args;
            if (!PopAllData(args)) {
                return false;
            }

            arg1 = std::static_pointer_cast<T1>(args[0]);
            arg2 = std::static_pointer_cast<T2>(args[1]);
            arg3 = std::static_pointer_cast<T3>(args[2]);
            arg4 = std::static_pointer_cast<T4>(args[3]);
            arg5 = std::static_pointer_cast<T5>(args[4]);
            arg6 = std::static_pointer_cast<T6>(args[5]);
            arg7 = std::static_pointer_cast<T7>(args[6]);
            arg8 = std::static_pointer_cast<T8>(args[7]);
            arg9 = std::static_pointer_cast<T9>(args[8]);
            arg10 = std::static_pointer_cast<T10>(args[9]);
            arg11 = std::static_pointer_cast<T11>(args[10]);
            arg12 = std::static_pointer_cast<T12>(args[11]);
            arg13 = std::static_pointer_cast<T13>(args[12]);
            arg14 = std::static_pointer_cast<T14>(args[13]);
            return true;
        }

        /**
        * @ingroup hiaiengine
        * @brief: PopAllData 读取所有队列头部数据。
        *         仅当所有队列头部都有数据时才读取成功，并删除队列头部数据。
        *         否则，返回失败但并不删除任何数据。
        *        该函数仅当只有十五个队列时才使用。
        * @param [out]arg1, arg2， arg3， arg4, arg5, arg6, arg7, arg8,
        * arg9, arg10, arg11, arg12, arg13, arg14, arg15: 队列函数指针
        * @param [out]bool: 仅当所有队列头部都有数据时才读取成功并返回true，
        * 否则返回false
        */
        template<typename T1, typename T2, typename T3,
            typename T4, typename T5, typename T6,
            typename T7, typename T8, typename T9, typename T10,
            typename T11, typename T12, typename T13,
            typename T14, typename T15>
        HIAI_LIB_VISIBILITY bool PopAllData(std::shared_ptr<T1>& arg1,
            std::shared_ptr<T2>& arg2, std::shared_ptr<T3>& arg3,
            std::shared_ptr<T4>& arg4, std::shared_ptr<T5>& arg5,
            std::shared_ptr<T6>& arg6, std::shared_ptr<T7>& arg7,
            std::shared_ptr<T8>& arg8, std::shared_ptr<T9>& arg9,
            std::shared_ptr<T10>& arg10, std::shared_ptr<T11>& arg11,
            std::shared_ptr<T12>& arg12, std::shared_ptr<T13>& arg13,
            std::shared_ptr<T14>& arg14, std::shared_ptr<T15>& arg15) {
            if (qumNum_ != 15) {
                return false;
            }

            std::vector<std::shared_ptr<void>> args;
            if (!PopAllData(args)) {
                return false;
            }

            arg1 = std::static_pointer_cast<T1>(args[0]);
            arg2 = std::static_pointer_cast<T2>(args[1]);
            arg3 = std::static_pointer_cast<T3>(args[2]);
            arg4 = std::static_pointer_cast<T4>(args[3]);
            arg5 = std::static_pointer_cast<T5>(args[4]);
            arg6 = std::static_pointer_cast<T6>(args[5]);
            arg7 = std::static_pointer_cast<T7>(args[6]);
            arg8 = std::static_pointer_cast<T8>(args[7]);
            arg9 = std::static_pointer_cast<T9>(args[8]);
            arg10 = std::static_pointer_cast<T10>(args[9]);
            arg11 = std::static_pointer_cast<T11>(args[10]);
            arg12 = std::static_pointer_cast<T12>(args[11]);
            arg13 = std::static_pointer_cast<T13>(args[12]);
            arg14 = std::static_pointer_cast<T14>(args[13]);
            arg15 = std::static_pointer_cast<T15>(args[14]);
            return true;
        }

        /**
        * @ingroup hiaiengine
        * @brief: PopAllData 读取所有队列头部数据。
        *         仅当所有队列头部都有数据时才读取成功，并删除队列头部数据。
        *         否则，返回失败但并不删除任何数据。
        *        该函数仅当只有十六个队列时才使用。
        * @param [out]arg1, arg2， arg3， arg4, arg5, arg6, arg7, arg8,
        * arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16: 队列函数指针
        * @param [out]bool: 仅当所有队列头部都有数据时才读取成功并返回true，
        * 否则返回false
        */
        template<typename T1, typename T2, typename T3,
            typename T4, typename T5, typename T6,
            typename T7, typename T8, typename T9, typename T10,
            typename T11, typename T12, typename T13,
            typename T14, typename T15, typename T16>
        HIAI_LIB_VISIBILITY bool PopAllData(std::shared_ptr<T1>& arg1,
            std::shared_ptr<T2>& arg2, std::shared_ptr<T3>& arg3,
            std::shared_ptr<T4>& arg4, std::shared_ptr<T5>& arg5,
            std::shared_ptr<T6>& arg6, std::shared_ptr<T7>& arg7,
            std::shared_ptr<T8>& arg8, std::shared_ptr<T9>& arg9,
            std::shared_ptr<T10>& arg10, std::shared_ptr<T11>& arg11,
            std::shared_ptr<T12>& arg12, std::shared_ptr<T13>& arg13,
            std::shared_ptr<T14>& arg14, std::shared_ptr<T15>& arg15,
            std::shared_ptr<T16>& arg16) {
            if (qumNum_ != 16) {
                return false;
            }

            std::vector<std::shared_ptr<void>> args;
            if (!PopAllData(args)) {
                return false;
            }

            arg1 = std::static_pointer_cast<T1>(args[0]);
            arg2 = std::static_pointer_cast<T2>(args[1]);
            arg3 = std::static_pointer_cast<T3>(args[2]);
            arg4 = std::static_pointer_cast<T4>(args[3]);
            arg5 = std::static_pointer_cast<T5>(args[4]);
            arg6 = std::static_pointer_cast<T6>(args[5]);
            arg7 = std::static_pointer_cast<T7>(args[6]);
            arg8 = std::static_pointer_cast<T8>(args[7]);
            arg9 = std::static_pointer_cast<T9>(args[8]);
            arg10 = std::static_pointer_cast<T10>(args[9]);
            arg11 = std::static_pointer_cast<T11>(args[10]);
            arg12 = std::static_pointer_cast<T12>(args[11]);
            arg13 = std::static_pointer_cast<T13>(args[12]);
            arg14 = std::static_pointer_cast<T14>(args[13]);
            arg15 = std::static_pointer_cast<T15>(args[14]);
            arg16 = std::static_pointer_cast<T16>(args[15]);
            return true;
        }

        protected:
        HIAI_LIB_INTERNAL bool PopAllData(std::vector<
            std::shared_ptr<void>>& args) {
            auto curTime = std::chrono::time_point_cast<
                std::chrono::milliseconds>(
                std::chrono::system_clock::now());
            std::lock_guard<std::mutex> guard(mutex_);
            for (uint32_t index = 0; index < qumNum_; ++index) {
                auto iter = multiQueue_.find(index);
                if (iter == multiQueue_.end()) {
                    return false;
                }
                if ((*iter).second->empty()) {
                    return false;
                }
                auto quePair = (*iter).second->front();
                if (isAutoDel_) {
                    std::chrono::milliseconds diffTime(
                        curTime - quePair.first);
                    while (diffTime.count() >= durationMs_) {
                        HIAI_ENGINE_LOG("MultiTypeQueue delete queue element "
                            "when max_duration_time is %u ms "
                            "and diff time is %d ms",
                            durationMs_, diffTime.count());
                        (*iter).second->pop();
                        if ((*iter).second->empty()) {
                            return false;
                        }
                        quePair = (*iter).second->front();
                        diffTime = std::chrono::milliseconds(
                            curTime - quePair.first);
                    }
                }
                args.push_back(quePair.second);
            }

            for (uint32_t index = 0; index < qumNum_; ++index) {
                auto iter = multiQueue_.find(index);
                (*iter).second->pop();
            }
            return true;
        }
      private:
        HIAI_LIB_INTERNAL MultiTypeQueue(const MultiTypeQueue&) = delete;
        HIAI_LIB_INTERNAL MultiTypeQueue(MultiTypeQueue&&) = delete;
        HIAI_LIB_INTERNAL MultiTypeQueue& operator=(const MultiTypeQueue&) = delete;
        HIAI_LIB_INTERNAL MultiTypeQueue& operator=(MultiTypeQueue&&) = delete;
      private:
        uint32_t qumNum_;
        uint32_t durationMs_;
        uint32_t maxQueLen_;
        std::map<uint32_t,
            std::shared_ptr<std::queue<std::pair<MultiQueueTimeType,
            std::shared_ptr<void> > > > > multiQueue_;
        std::mutex mutex_;
        bool isAutoDel_;
    };
}

#endif  // INC_HIAIENGINE_MULTITYPE_QUEUE_H_
