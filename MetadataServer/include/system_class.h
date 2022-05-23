//
// Created by 顾仁杰 on 2020/5/16.
//

#ifndef METADATA_MANAGEMENT_SYSTEM_CLASS_H
#define METADATA_MANAGEMENT_SYSTEM_CLASS_H


#include <string>
#include <vector>

class SystemClass {
public:
    virtual void ExecuteCommand(const std::vector<std::string>& command_args, std::string& feedback) = 0;
};


#endif //METADATA_MANAGEMENT_SYSTEM_CLASS_H
