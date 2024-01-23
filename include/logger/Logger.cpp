#include "Logger.h"

#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <thread>

void Logger::error(const std::string& msg) {
    this->logMessage(LogLevel::Error, msg);
}

void Logger::warn(const std::string& msg) {
    this->logMessage(LogLevel::Warn, msg);
}

void Logger::info(const std::string& msg) {
    this->logMessage(LogLevel::Info, msg);
}

void Logger::logMessage(LogLevel level, std::string msg) {
    std::lock_guard<std::mutex> guard(this->logMtx);
    std::string logLevel;

    switch (level) {
        case Error:
            logLevel = "[ERROR] ";
            break;
        case Warn:
            logLevel = "[WARN] ";
            break;
        case Info:
            logLevel = "[INFO] ";
            break;
    }

    std::cout << Logger::getTimestamp() << ": " << "thread: "
              << std::this_thread::get_id() << " -- " << logLevel << msg << "\n";

    this->logStream << Logger::getTimestamp() << ": " << "thread: "
                    << std::this_thread::get_id() << " -- " << logLevel << msg << "\n";
}

std::string Logger::getTimestamp() {
    auto now = std::chrono::system_clock::now();

    auto nowTimeT = std::chrono::system_clock::to_time_t(now);

    struct tm nowTM{};
    localtime_r(&nowTimeT, &nowTM);

    std::stringstream ss;
    ss << std::put_time(&nowTM, "%FT%T%z");

    return ss.str();
}
