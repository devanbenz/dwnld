#include "HttpDownloader.h"

#include <iostream>
#include <thread>
#include <vector>

int main(int argv, char *argc[]) {
    if (argv < 3) {
        std::cout << "Usage: dwnld -f <files-to-dl>.txt -l <log-file>.txt -d <download-dir>\n";
        return 1;
    }

    HttpDownloader& dl = HttpDownloader::instance();
    std::ifstream file("/Users/devan/Documents/Projects/dwnld/list.txt");
    std::string line;
    std::vector<std::thread> threads;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            threads.emplace_back([&dl, line]() {
                dl.get(line);
            });
        }
        file.close();
    } else {
        std::cerr << "Unable to open file" << std::endl;
    }

    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    return 0;
}
