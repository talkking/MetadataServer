//
// Created by 顾仁杰 on 2020/5/16.
//

#include "metadata_system.h"
#include "assist.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;

MetadataSystem::MetadataSystem(MetadataServer* owner): metadata_map_(), owner_(owner) {}

MetadataSystem::~MetadataSystem() {
    for (auto it = metadata_map_.begin(); it != metadata_map_.end(); it++) {
        delete it->second;
        metadata_map_.erase(it);
    }
}

void MetadataSystem::wup(const vector<string> &args, string &feedback) {
    feedback = "WUP";
}

void MetadataSystem::pushto(const vector<string>& args, string& feedback) {
    feedback = "Success";
    string ip = args[1];
    int port = stoi(args[2]);
    for (auto & it : metadata_map_) {
        string result;
        string message = "create " + it.second->GetSummary();
        MetadataServer::SendMessageToSlaveIp(ip, port, message, result);
        if (result != "Success" && result != "[ERROR] File ID already exists.") {
            feedback = "Failure";
        }
    }
}

void MetadataSystem::ExecuteCommand(const vector<string>& command_args, string& feedback) {
    if (command_args.empty()) {
        Error("[ERROR] No command arguments received.\n");
    }
    const string& command = command_args[0];
    if (command == "WUP") {
        this->wup(command_args, feedback);
    }
    else if (command == "request") {
        this->request(command_args, feedback);
    }
    else if (command == "delete") {
        this->remove(command_args, feedback);
    }
    else if (command == "create") {
        this->create(command_args, feedback);
    }
    else if (command == "update") {
        this->update(command_args, feedback);
    }
    else if (command == "pushto") {
        this->pushto(command_args, feedback);
    }
    else {
        feedback = "[ERROR] Command " + command + " is not supported.\n";
    }
    std::cout << "[DEBUG] Current metadata states:" << std::endl;
    for (auto & tmp_node : metadata_map_) {
        std::cout << tmp_node.second->GetInfo() << std::endl;
    }
}

void MetadataSystem::request(const std::vector<std::string>& args, std::string& feedback) {
    if (args.size() != 2) {
        feedback = "[ERROR] Request require 1 argument.\n";
        return;
    }
    int id = stoi(args[1]);
    auto it = metadata_map_.find(id);
    if (it == metadata_map_.end()) {
        feedback = "[ERROR] Request failed.";
        return;
    }
    feedback = it->second->GetInfo();
}

void MetadataSystem::remove(const std::vector<std::string>& args, std::string& feedback) {
    if (args.size() != 2) {
        feedback = "[ERROR] Remove requires 1 argument.";
        return;
    }
    int id = stoi(args[1]);
    auto tmp = metadata_map_.find(id);
    if (tmp == metadata_map_.end()) {
        feedback = "[ERROR] Remove failed.";
        return;
    }
    delete tmp->second;
    metadata_map_.erase(tmp);
    feedback = "Success";
}

void MetadataSystem::create(const std::vector<std::string>& args, std::string& feedback) {
    if (args.size() != 8) {
        feedback = "[ERROR] Create requires 7 arguments.";
        return;
    }
    int id = stoi(args[1]);
    int parent = stoi(args[2]);
    const std::string& creation_time = args[3];
    const std::string& lastmodify_time = args[4];
    size_t size = stoi(args[5]);
    const std::string& name = args[6];
    const int type = stoi(args[7]);
    auto* new_metadata = new MetadataNode(id, parent, creation_time, size, lastmodify_time, name, type);
    auto tmp = metadata_map_.find(id);
    if (tmp != metadata_map_.end()) {
        feedback = "[ERROR] File ID already exists.";
        return;
    }
    metadata_map_.insert(std::pair<int, MetadataNode*>(id, new_metadata));
    feedback = "Success";
}

void MetadataSystem::update(const std::vector<std::string>& args, std::string& feedback)
{
    if ((args.size()) % 2 != 0) {
        feedback = "[ERROR] Wrong arguments for update.";
        return;
    }
    int id = stoi(args[1]);
    auto it = metadata_map_.find(id);
    if (it == metadata_map_.end()) {
        feedback = "[ERROR] File ID not found.";
        return;
    }
    MetadataNode* metadata = metadata_map_[id];
    for (int i = 2; i < args.size(); i += 2) {
        if (args[i] == "parent") {
            metadata->set_parent(stoi(args[i + 1]));
        }
        else if (args[i] == "modification_time") {
            metadata->set_modification_time(args[i + 1]);
        }
        else if (args[i] == "name") {
            metadata->set_name(args[i + 1]);
        }
        else {
            feedback = "[ERROR] unknown key value.";
            return;
        }
        feedback = "Success";
    }
}

