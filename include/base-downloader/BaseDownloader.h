#pragma once
#include <string>

class BaseDownloader {
public:
    virtual int get(std::string) = 0;
};
