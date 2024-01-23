#pragma once

#include <filesystem>
#include <fstream>

enum LogLevel {
    Error,
    Warn,
    Info
};

class Logger {
private:
    std::ofstream logStream;
    std::mutex logMtx;

    void logMessage(LogLevel, std::string);
    static std::string getTimestamp();;
public:
    explicit Logger(const std::filesystem::path& logFile) {
        if (this->logStream.is_open()) {
            this->logStream.close();
        }
        this->logStream.open(logFile, std::ios_base::app);
    };

    void error(const std::string&);
    void warn(const std::string&);
    void info(const std::string&);

    ~Logger() {
        if (this->logStream.is_open()) {
            this->logStream.close();
        }
    }
};