#pragma once

#include "BaseDownloader.h"
#include "Logger.h"

class HttpDownloader : public BaseDownloader {
private:
    HttpDownloader();
    Logger logger;
    std::mutex mtx;
    std::string getHostOrFileToDl(const std::string&);
    ~HttpDownloader();
public:
    int get(std::string) override;
    static size_t libCurlWriteCallback(void*, size_t, size_t, std::ofstream);
    static HttpDownloader& instance();
};
