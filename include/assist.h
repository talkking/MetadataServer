/*
 * @Author: Jie
 * @LastEditors: Jie
 * @FilePath: /include/assist.hpp
 */

#ifndef METADATA_MANAGEMENT_ASSIST_H
#define METADATA_MANAGEMENT_ASSIST_H

#include <vector>
#include <string>


void ListIp();

// Split the input string at every delimiter, return a vector<string> containing the split results.
std::vector<std::string> SplitString(const std::string &in_string, const std::string &delimiters = std::string(" \t\r\n"));

void Error(const char* message);

#endif //METADATA_MANAGEMENT_ASSIST_H
