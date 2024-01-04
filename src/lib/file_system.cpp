/*
 * @Author: Jie
 * @LastEditors: Jie
 * @FilePath: /src/lib/filesystem.cpp
 */

#include "file_system.h"
#include "assist.h"
#include "metadata_server.h"
#include <map>
#include <vector>
#include <string>


using namespace std;

FileSystem::FileSystem(MetadataServer* metadata_server) {
    file_structure_ = new FileStructure(this);
    master_metadata_server = metadata_server;
}

FileSystem::~FileSystem() {
    delete file_structure_;
}

void FileSystem::pwd(const vector<string>& command_args, string& feedback) {
    file_structure_->GetCurrentDirectory(feedback);
}

void FileSystem::mkdir(const vector<string>& command_args, string& feedback) {
    if (command_args.size() < 2) {
        feedback = "[ERROR] Command \"mkdir\" requires 1 argument.\n";
    }
    else {
        const string& name = command_args[1];
        file_structure_->MakeDirectory(name, feedback);
    }
}

bool CheckOptionArg(const string& arg) {
    if (arg.empty()) {
        Error("[ERROR] arg must not be empty.");
    }
    return arg[0] == '-';
}

void FileSystem::ls(const vector<string>&command_args, string& feedback) {
    string src = ".";
    bool recursive = false;
    int len = command_args.size();
    for (int i = 1; i < len; i++) {
        if (CheckOptionArg(command_args[i])) {
            if (command_args[i] == "-r") {
                recursive = true;
            }
            else {
                feedback = "[ERROR] Argument " + command_args[i] + " is not supported.\n";
            }
        }
        else {
            src = command_args[i];
        }
    }
    file_structure_->List(src, recursive, feedback);
}

void FileSystem::readdir(const vector<string>& command_args, string& feedback) {
    string src = ".";
    if (command_args.size() > 1) {
        src = command_args[1];
    }
    file_structure_->List(command_args[1], false, feedback);
}

void FileSystem::cd(const vector<string>& command_args, string& feedback)
{
    if (command_args.size() == 1) {
        feedback = "[ERROR] Command \"cd\" requires 1 argument.\n";
    }
    else {
        file_structure_->GoToDirectory(command_args[1], feedback);
    }
}

void FileSystem::mv(const vector<string>& command_args, string& feedback) {
    if (command_args.size() < 3) {
        feedback = "[ERROR] Command \"mv\" requires 2 argument.\n";
    }
    else {
        file_structure_->Move(command_args[1], command_args[2], feedback);
    }
}

void FileSystem::stat(const vector<string>& command_args, string& feedback) {
    if (command_args.size() == 1) {
        feedback = "[ERROR] Command \"stat\" requires 1 argument.\n";
    }
    else {
        file_structure_->State(command_args[1], feedback);
    }
}

void FileSystem::touch(const vector<string>& command_args, string& feedback)
{
    if (command_args.size() == 1) {
        feedback = "[ERROR] Command \"touch\" requires 1 argument.\n";
    }
    else {
        file_structure_->Touch(command_args[1], feedback);
    }
}

void FileSystem::rm(const vector<string>& command_args, string& feedback) {
    string src = "";
    bool recursive = false;
    int len = command_args.size();
    for (int i = 1; i < len; ++i) {
        if (CheckOptionArg(command_args[i])) {
            if (command_args[i] == "-r") {
                recursive = true;
            }
            else {
                feedback = "[ERROR] Argument " + command_args[i] + " is not supported.\n";
            }
        }
        else {
            src = command_args[i];
        }
    }
    if (src.empty()) {
        feedback = "[ERROR] Command \"rm\" requires 1 positional arguments.\n";
    }
    else {
        file_structure_->Remove(src, recursive, feedback);
    }
}

void FileSystem::serverstats(const vector<string>& command_args, string& feedback) {
    master_metadata_server->CheckSlaveStats(feedback);
}

void FileSystem::ExecuteCommand(const vector<string>& command_args, string& feedback) {
    if (command_args.empty()) {
        Error("[ERROR] Size of command_args should not be 0.\n");
    }
    const string& command = command_args[0];
    if (command == "pwd") {
        this->pwd(command_args, feedback);
    }
    else if (command == "mkdir") {
        this->mkdir(command_args, feedback);
    }
    else if (command == "ls") {
        this->ls(command_args, feedback);
    }
    else if (command == "readdir") {
        this->readdir(command_args, feedback);
    }
    else if (command == "cd") {
        this->cd(command_args, feedback);
    }
    else if (command == "mv") {
        this->mv(command_args, feedback);
    }
    else if (command == "stat") {
        this->stat(command_args, feedback);
    }
    else if (command == "touch") {
        this->touch(command_args, feedback);
    }
    else if (command == "rm") {
        this->rm(command_args, feedback);
    }
    else if (command == "serverstats") {
        this->serverstats(command_args, feedback);
    }
    else {
        feedback = "[ERROR] Command " + command + " is not supported.\n";
    }
}
