/*
 * @Author: Jie
 * @LastEditors: Jie
 * @FilePath: /include/filesystem.hpp
 */

#ifndef METADATA_MANAGEMENT_FILE_SYSTEM_H
#define METADATA_MANAGEMENT_FILE_SYSTEM_H


#include <vector>
#include <string>
#include "file_structure.h"
#include "metadata_server.h"
#include "metadata_system.h"
#include "system_class.h"

class FileStructure;
class MetadataServer;

class FileSystem : public SystemClass {
public:
    MetadataServer* master_metadata_server;
    FileSystem(MetadataServer* metadata_server);
    void ExecuteCommand(const std::vector<std::string>& command_args, std::string& feedback);
    ~FileSystem();

private:
    FileStructure* file_structure_;

    void pwd(const std::vector<std::string>& command_args, std::string& feedback);
    void mkdir(const std::vector<std::string>& command_args, std::string& feedback);
    void ls(const std::vector<std::string>& command_args, std::string& feedback);
    void readdir(const std::vector<std::string>& command_args, std::string& feedback);
    void cd(const std::vector<std::string>& command_args, std::string& feedback);
    void mv(const std::vector<std::string>& command_args, std::string& feedback);
    void stat(const std::vector<std::string>& command_args, std::string& feedback);
    void rm(const std::vector<std::string>& command_args, std::string& feedback);
    void touch(const std::vector<std::string>& command_args, std::string& feedback);
    void serverstats(const std::vector<std::string>& command_args, std::string& feedback);
};


#endif //METADATA_MANAGEMENT_FILE_SYSTEM_H