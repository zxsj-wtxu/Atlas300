/* Copyright (C) 2018. Huawei Technologies Co., Ltd. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the Apache License Version 2.0.You may not use this file except in compliance with the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * Apache License for more details at
 * http://www.apache.org/licenses/LICENSE-2.0
 */
#ifndef DOMI_OMG_GRAPH_GRAPH_H_
#define DOMI_OMG_GRAPH_GRAPH_H_

#include <string>
#include <vector>
#include <memory>
#include "common/types.h"
#include "omg/omg_types.h"
#include "proto/om.pb.h"

namespace calibration {
	class Tensor;
}

namespace domi {
    class Node;
    class Edge;

    class Graph {
    public:
        explicit Graph();
        virtual ~Graph();

        /**
        * @ingroup domi_omg
        * @brief 往Graph内添加节点，并把输入的op_def填充到节点中
        * @param [in] op_def 用于填充节点op_def信息
        * @return Node 添加节点的指针
        *
        */
        Node* AddNode(OpDef* op_def);
        void  AddNode(Node  *node,OpDef  *op_def);

        /**
        * @ingroup domi_omg
        * @brief 从Graph中移除指定节点
        * @param [in] node 待移除节点的指针
        * @return Status 移除结果
        *
        */
        Status RemoveNode(Node* node);

        /**
         * @ingroup domi_omg
         * @brief 将节点孤立
         *        即将该节点的前驱阶段与后继节点相连，并将该节点的边关系删除
         *        仅能处理输入边数小于等于1、输出变数大于等于1的场景
         * @param [in] node 待孤立节点
         * @return Status 孤立结果
         */
        Status IsolateNode(Node* node);

        /**
        * @ingroup domi_omg
        * @brief 将节点孤立
        *        即将该节点的前驱阶段与后继节点相连，并将该节点的边关系删除
        *        能处理输入边数大于1、输出变数大于等于1的场景
        * @param [in] node 待孤立节点
        * @return Status 孤立结果
        */
        Status IsolateMulInutNode(Node* node);


        /**
        * @ingroup domi_omg
        * @brief 合并多输入的的输入边
        * @param [in] node 需要合并的节点
        * @return Status 合并多输入，重新梳理srcindex和dstindex
        */
        Status MergeNodeEdge(Node* node);

        Edge* AddEdge(Node* src, int32_t x, int32_t src_format, Node* dest, int32_t y, int32_t dst_format, bool ctrl = false);

        /**
        * @ingroup domi_omg
        * @brief 为两个有依赖关系的节点添加边
        * @param [in] src 边的源节点
        * @param [in] x 源节点的输出索引
        * @param [in] dest 边的目的节点
        * @param [in] y 目标节点的输入索引
        * @param [in] ctrl 控制边属性
        * @return Edge 添加边的指针
        *
        */
        Edge* AddEdge(Node* src, int32_t x, Node* dest, int32_t y, bool ctrl = false);

        /**
        * @ingroup domi_omg
        * @brief 从Graph中移除指定边
        * @param [in] edge 待移除边的指针
        * @param [in] reindex 是否需要对边重新编号，为了对边进行替换，提供这个参数，默认为true
        * @return Status 移除结果
        */
        Status RemoveEdge(Edge* edge, bool reindex = true);

        /**
        * @ingroup domi_omg
        * @brief 根据节点的名称查找节点
        * @param [in] name 节点的名称
        * @return Node 查找到边的指针
        *
        */
        Node* FindNode(const std::string name);

        /**
        * @ingroup domi_omg
        * @brief 获取该Graph的所有节点
        * @return std::vector<Node*> 获取到的所有节点
        *
        */
        const std::vector<Node*>& GetAllNodes()
        {
            return nodes_;
        }

        /**
        * @ingroup domi_omg
        * @brief 获取该Graph的所有节点
        * @return std::vector<Edge*> 获取到的所有节点
        *
        */
        const std::vector<Edge*>& GetAllEdges()
        {
            return edges_;
        }

        /**
        * @ingroup domi_omg
        * @brief 设置graph名称
        * @return 无
        *
        */
        void SetName(const std::string name)
        {
            name_ = name;
        }

        /**
        * @ingroup domi_omg
        * @brief 获取graph名称
        * @return graph名称
        *
        */
        const std::string& Name()
        {
            return name_;
        }

        /**
        * @ingroup domi_omg
        * @brief 对该Graph进行拓扑排序
        * @return Status 拓扑排序执行结果
        *
        */
        Status TopologicalSorting();

        /**
        * @ingroup domi_omg
        * @brief 判断该Graph是否合法(包括是否有环、是否有孤立子图、拓扑排序和节点个数)
        * @return bool 是否合法
        *
        */
        bool IsValid();

        /**
        * @ingroup domi_omg
        * @brief 打印整个graph
        * @return 无
        *
        */
        void Print();

        /**
         * @name   InsertNodeInEdge
         * @brief  在一条边上插入一个节点
         * @param  [in] Edge * e
         * @param  [in] std::unique_ptr<OpDef> op_def
         * @return domi::Status
         */
        Status InsertNodeInEdge(Edge* e, std::unique_ptr<OpDef> op_def);
        Status InsertNodeInEdge(Edge* e, Node* node);

        /**
         * @name   InsertNodeInEdge
         * @brief  在一条边上插入多个节点
         * @param  [in] Edge * e
         * @param  [in] std::vector<OpDef * > & vec_op_def
         * @return domi::Status
         */
        Status InsertNodeInEdge(Edge* e, std::vector<OpDef*>& vec_op_def);

        /**
         * @brief  控制边Index号
         */
        static const int32_t kControlSlot;

        /**
        * @ingroup domi_omg
        * @brief 获取该Graph的所有共享权值的layer集合
        * @return std::map<std::vector<std::string>, std::vector<std::string>> 获取到的所有节点
        *
        */
        const std::map<std::vector<std::string>, std::vector<std::string>>& GetShareParamLayer()
        {
            return params_share_map_;
        }
        /**
        * @ingroup domi_omg
        * @brief 设置该Graph的所有共享权值的layer集合
        * @return
        *
        */
        void SetShareParamLayer(const std::map<std::vector<std::string>, std::vector<std::string>> params_share_map)
        {
            params_share_map_ = params_share_map;
        }
    private:
        Status SortNodes(std::vector<Node*> & stack,std::vector<uint32_t>& in_edge_num);
    private:
        //网络名
        std::string name_;
        //该Graph是否合法标志
        bool is_valid_flag_;
        //判断指定的节点是否合法
        bool IsValidNode(Node* node);
        //该Graph包含所有的节点
        std::vector<Node*> nodes_;
        //该Graph包含所有的边
        std::vector<Edge*> edges_;
        //参数共享layer合集
        std::map<std::vector<std::string>, std::vector<std::string>> params_share_map_;
    };

    /**
    * @ingroup domi_omg
    * @brief 获取节点名称
    * @return std::string 节点名称
    */
    class Node {
    public:
        Node();
        virtual ~Node();
        /**
        * @ingroup domi_omg
        * @brief 获取节点名称
        * @return std::string 节点名称
        *
        */
        const std::string& Name() const
        {
            return op_def_->name();
        }
        /**
        * @ingroup domi_omg
        * @brief 获取节点ID
        * @return uint32_t 节点ID
        *
        */
        uint32_t Id()
        {
            return id_;
        }

        /**
        * @ingroup domi_omg
        * @brief 设置节点ID
        * @param [in] uint32_t 节点ID
        * @return void
        */
        void SetId(uint32_t new_id)
        {
            id_ = new_id;
        }

        /**
        * @ingroup domi_omg
        * @brief 获取该节点的输出边全集
        * @return std::vector<Edge*> 该节点的输出边全集
        *
        */
        const std::vector<Edge*>& OutEdges()
        {
            return out_edges_;
        }

        /**
        * @ingroup domi_omg
        * @brief 获取该节点的输出控制边
        * @return std::vector<Edge*> 该节点的输出边
        *
        */
        const std::vector<Edge*>& OutControlEdges()
        {
            return out_control_edges_;
        }
        /**
        * @ingroup domi_omg
        * @brief 获取该节点的输出普通边
        * @return std::vector<Edge*> 该节点的输出普通边
        *
        */
        const std::vector<Edge*>& OutNormalEdges()
        {
            return out_normal_edges_;
        }
        /**
        * @ingroup domi_omg
        * @brief 获取该节点的输入边全集
        * @return std::vector<Edge*> 该节点的输入边全集
        *
        */
        const std::vector<Edge*>& InEdges() const
        {
            return in_edges_;
        }

        /**
        * @ingroup domi_omg
        * @brief 获取该节点的输入控制边全集
        * @return std::vector<Edge*> 该节点的输入控制边全集
        *
        */
        const std::vector<Edge*>& InControlEdges()
        {
            return in_control_edges_;
        }

        /**
        * @ingroup domi_omg
        * @brief 获取该节点的输入控制边全集
        * @return std::vector<Edge*> 该节点的输入控制边全集
        *
        */
        const std::vector<Edge*>& InNormalEdges()
        {
            return in_normal_edges_;
        }


        /**
        * @ingroup domi_omg
        * @brief 获取该节点的输出Tensor(tensorDescriptor_+内存地址)
        * @return std::vector<Tensor*> 该节点的输出边
        */
        const std::vector<calibration::Tensor*>& GetOutputTensors();

        /**
        * @ingroup domi_omg
        * @brief 设置该节点的输出Tensor(tensorDescriptor_+内存地址)
        * @return void
        */
        void SetOutputTensors(std::vector<calibration::Tensor*> output_tensors);

        /**
        * @ingroup domi_omg
        * @brief 获取该节点的输入Tensor(tensorDescriptor_+内存地址)
        * @return std::vector<Tensor*> 该节点的输入边
        */
        const std::vector<calibration::Tensor*>& GetInputTensors();

        /**
        * @ingroup domi_omg
        * @brief 设置该节点的输入Tensor(tensorDescriptor_+内存地址)
        * @return void
        */
        void SetInputTensors(std::vector<calibration::Tensor*> input_tensors);

        /**
         * @ingroup domi_omg
         * @brief 设置该节点所在的Graph
         * @param graph
         * @return void
         */
        void SetGraph(Graph* graph);

        /**
         * @ingroup domi_omg
         * @brief 获取该节点的Graph信息
         * @return Graph* 该节点的Graph指针
         */
        const Graph* GetGraph() const;

        /**
         * @ingroup domi_omg
         * @brief 获取该节点的Graph信息
         * @return Graph* 该节点的Graph指针
         */
        Graph* GetGraph();

        /**
         * @ingroup domi_omg
         * @brief 获取该节点的op_def信息
         * @return OpDef* 该节点的op_def指针
         *
         */
        const OpDef* GetOpDef() const
        {
            return op_def_;
        }

        /**
         * @ingroup domi_omg
         * @brief 获取该节点的op_def信息
         * @return OpDef* 该节点的op_def指针
         *
         */
        OpDef* GetOpDef()
        {
            return op_def_;
        }
        /**
        * @ingroup domi_omg
        * @brief get const op nodes
        * @return vector of const node
        *
        */
        std::vector<Node*> GetConstInputs();

        /**
        * @ingroup domi_omg
        * @brief 从Node的InEdge中移除指定边
        * @param [in] edge 待移除边的指针
        * @param [in] reindex 是否对剩下的边进行重新编号，默认为true
        * @return Status 移除结果
        */
        void RemoveInEdge(Edge* edge, bool reindex = true);

        /**
        * @ingroup domi_omg
        * @brief 从Node的OutEdge中移除指定边
        * @param [in] edge 待移除边的指针
        * @param [in] reindex 是否对剩下的边进行重新编号，默认为true
        * @return Status 移除结果
        */
        void RemoveOutEdge(Edge* edge, bool reindex = true);

        /**
        * @ingroup domi_omg
        * @brief 删除控制或普通边
        *
        */
        void RemoveControlOrNormalEdge(std::vector<Edge*>& control_edges, std::vector<Edge*>& normal_edges, Edge* edge);

        /**
        * @ingroup domi_omg
        * @brief 向发送事件列表中新增指定的event_id
        * @param [in] node event_id 事件id
        * @return void
        *
        */
        void AddSendEventId(uint32_t event_id) { send_event_id_list_.push_back(event_id); }

        /**
        * @ingroup domi_omg
        * @brief 向接收事件列表中新增指定的event_id
        * @param [in] node event_id 事件id
        * @return void
        *
        */
        void AddRecvEventId(uint32_t event_id) { recv_event_id_list_.push_back(event_id); }

        /**
        * @ingroup domi_omg
        * @brief 获取发送事件列表
        * @return const std::vector<uint32_t>& 列表
        *
        */
        const std::vector<uint32_t>& GetSendEventIdList() { return send_event_id_list_; }

        /**
        * @ingroup domi_omg
        * @brief 获取接收事件列表
        * @return const std::vector<uint32_t>& 列表
        *
        */
        const std::vector<uint32_t>& GetRecvEventIdList() { return recv_event_id_list_; }
    private:
        // 私有赋值操作符/拷贝构造函数，不允许赋值/拷贝操作，避免成员重复释放
        Node& operator=(const Node &node) { (void)node;return *this;  }
        Node(const Node &node){ (void)node;}

    public:
        Graph* graph_;
        OpDef* op_def_;
    private:
        friend class Graph;
        uint32_t id_;
        std::vector<Edge*> in_edges_;    //输入边全集
        std::vector<Edge*> in_control_edges_;    //输入控制边
        std::vector<Edge*> in_normal_edges_;    //输入普通边

        std::vector<Edge*> out_edges_;    //输出边全集
        std::vector<Edge*> out_control_edges_;    //输出控制边
        std::vector<Edge*> out_normal_edges_;    //输出普通边

        std::vector<uint32_t> send_event_id_list_;
        std::vector<uint32_t> recv_event_id_list_;
        std::vector<calibration::Tensor*> input_tensors_;
        std::vector<calibration::Tensor*> output_tensors_;
    };

    //Graph中边的类定义
    class Edge {
    public:
        Edge()
            : src_(nullptr),
              dst_(nullptr),
              src_out_index_(0),
              dst_in_index_(0),
              control(false),
              src_out_switch_index_(-1),
              src_format_(-1),
              dst_format_(-1)
         {}
        ~Edge() {}

        /**
        * @ingroup domi_omg
        * @brief 获取边的源节点指针
        * @return Node*
        *
        */
        Node* Src() const { return src_; }

        /**
        * @ingroup domi_omg
        * @brief 获取边的目标节点指针
        * @return Node*
        *
        */
        Node* Dst() const { return dst_; }

        /**
        * @ingroup domi_omg
        * @brief 获取边的源节点索引
        * @return const int
        *
        */
        int SrcIndex() { return src_out_index_; }

        /**
        * @ingroup domi_omg
        * @brief 获取边的目标节点索引
        * @return const int
        *
        */
        int DstIndex() { return dst_in_index_; }

        /**
        * @ingroup domi_omg
        * @brief 设置边的目标节点索引
        * @return void
        *
        */
        void SetDstIndex(int32_t new_index) { dst_in_index_ = new_index; }

        /**
        * @ingroup domi_omg
        * @brief 获取边属性是否为控制边
        * @return true/false
        *
        */
        int IsControl() { return control; }

        /**
        * @ingroup domi_omg
        * @brief 获取边属性是否为控制边
        * @return true/false
        *
        */
        void SetControl(bool ctrl) { control = ctrl; }

        /**
        * @ingroup domi_omg
        * @brief 获取边属性是否为控制边
        * @return true/false
        *
        */
        int32_t GetSwtichOutIndex() { return src_out_switch_index_; }
         /**
        * @ingroup domi_omg
        * @brief 获取边的源节点format
        * @return const int
        *
        */
        int32_t SrcFormat() { return src_format_; }
        /**
        * @ingroup domi_omg
        * @brief 设置边的目标节源节点format
        * @return void
        *
        */
        void SetSrcFormat(int32_t src_format) { src_format_ = src_format; }
         /**
        * @ingroup domi_omg
        * @brief 获取边的目标节点format
        * @return const int
        *
        */
        int32_t DstFormat() { return dst_format_; }
        /**
        * @ingroup domi_omg
        * @brief 设置边的目标节源节点format
        * @return void
        *
        */
        void SetDstFormat(int32_t dst_format) { dst_format_ = dst_format; }
    public:
        Node* src_;
        Node* dst_;
    private:
        friend class Graph;
        friend class Node;

        // -1为控制边，从0开始，该index应与OpBuilder中GetOutputMemory的输出index一致
        int32_t src_out_index_;
        // -1为控制边，从0开始，该index应与OpBuilder中GetInputMemory的输出index一致
        int32_t dst_in_index_;
        // 控制边属性
        bool control;
        // Switch Node的输出真实index，因为swtich输出为控制边时，输入和输出index置-1，需保留原有index
        int32_t src_out_switch_index_;

         //源的format
        int32_t src_format_;
        //目的网元format
        int32_t dst_format_;
    };


}  // namespace domi

#endif  // DOMI_OMG_GRAPH_GRAPH_H_
