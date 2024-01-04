//
// Created by 顾仁杰 on 2020/5/16.
//

#ifndef METADATA_MANAGEMENT_FILE_STRUCTURE_H
#define METADATA_MANAGEMENT_FILE_STRUCTURE_H


#include "file_system.h"

#include <vector>
#include <string>

class FileSystem;

class FileStructure
{
public:
    FileStructure(FileSystem *file_system);
    ~FileStructure();
    void GetCurrentDirectory(std::string &feedback);
    void MakeDirectory(const std::string &path, std::string &feedback);
    void List(const std::string &path, bool recursive_flag, std::string &feedback);
    void GoToDirectory(const std::string &path, std::string &feedback);
    void Move(const std::string &src_path, const std::string &dst_path, std::string &feedback);
    void Touch(const std::string &path, std::string &feedback);
    void State(const std::string &path, std::string &feedback);
    void Remove(const std::string &path, bool recursive_flag, std::string &feedback);

private:
    class FileNode {
    public:
        FileNode(FileStructure *file_structure);
        FileNode(FileStructure *file_structure, FileNode *parent, const std::string &name, bool is_file, int size, std::string &feedback);

        FileNode* const& AddChild(const std::string &name, bool is_file, int size, std::string &feedback);
        void AttachTo(FileNode *new_parent, std::string &feedback);
        void set_object_name_(const std::string &new_name, std::string &feedback);
        void RefreshTime(std::string &feedback);
        void RemoveChild(int child_index, std::string &feedback);
        void RemoveChild(FileNode *child_node, std::string &feedback);

        void FindNearbyDirectory(const std::string &name, FileNode *&result);
        int FindChildIndex(const std::string &child_name) const;
        const std::string& get_object_name() const;
        FileNode* const& get_parent() const;
        const std::vector<FileNode*>& get_children() const;
        const int get_object_id() const;
        bool is_file() const;
        void RequestSelf(std::string &feedback) const;

    private:
        FileStructure *const file_structure_;
        static int node_count_;
        const int object_id_;
        std::string object_name_;
        const bool is_file_;
        FileNode *parent_;
        std::vector<FileNode*> children_;

        FileNode(FileStructure *file_structure, bool is_file);
        void DetachSelfFromParent();
        void AttachToNewParent(FileNode *new_parent);
        std::string GenerateCreationMessage() const;
        std::string GenerateUpdateMessage(const std::string &key, const std::string &value) const;
        void MasterCallSlavesHandleThisNode(const std::string &message, std::string &feedback) const;
    };

    FileSystem *file_system_;
    FileNode *root_node_;
    FileNode *current_node_;
    static bool IsValidPath(const std::vector<std::string> &path_args, FileNode *src_node, FileNode *&dst_node);
    static void ParentPath(const std::vector<std::string>& path_args, std::vector<std::string>& result);
    bool CreateDirectory(const std::vector<std::string> &path_args, FileNode *&generated_node, std::string &feedback);
    void RecursiveRemoveChildren(FileNode *start_node, std::string &feedback);
    void RecursiveListObjects(const FileNode * const start_node, std::vector<std::string> &object_names, const std::string &last_level_indent);
};


#endif //METADATA_MANAGEMENT_FILE_STRUCTURE_H
