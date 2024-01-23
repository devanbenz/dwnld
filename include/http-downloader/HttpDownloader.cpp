#include "HttpDownloader.h"

#include <curl/curl.h>
#include <sstream>
#include <regex>

HttpDownloader::HttpDownloader() :
logger(std::filesystem::path("/Users/devan/Documents/Projects/dwnld/service.log")) {}

HttpDownloader &HttpDownloader::instance() {
    static HttpDownloader instance;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    return instance;
}

HttpDownloader::~HttpDownloader() {
    this->logger.info("closing libcurl");
    curl_global_cleanup();
}

int HttpDownloader::get(std::string url) {
    std::lock_guard<std::mutex> guard(this->mtx);

    CURL *handle = curl_easy_init();
    std::ofstream fileStream;
    std::ostringstream filePath;
    std::string fileName = this->getHostOrFileToDl(url);

    filePath << "/Users/devan/Documents/Projects/dwnld/files/" << fileName;
    fileStream.open(filePath.str());

    auto curlSetup = curl_easy_setopt(handle, CURLOPT_URL, url.c_str());

    if (curlSetup != CURLcode::CURLE_OK) {
        this->logger.error("curl options did not get set properly");
        return 1;
    }

    std::ostringstream logMsg;
    logMsg << "curl options set successfully. Url: " << url;
    this->logger.info(logMsg.str());

    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, libCurlWriteCallback);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &fileStream);

    auto res = curl_easy_perform(handle);

    if (res != CURLcode::CURLE_OK) {
        this->logger.error("error sending curl transport");
        return 1;
    }

    this->logger.info("wrote data to file from curl transport: " + filePath.str());
    return 0;
}

size_t HttpDownloader::libCurlWriteCallback(void *contents, size_t size, size_t nmemb, std::ofstream stream) {
    size_t realSz = size * nmemb;
    stream.write(static_cast<char *>(contents), realSz);
    return realSz;
}

std::string HttpDownloader::getHostOrFileToDl(const std::string& url) {
        std::regex urlPattern(R"(^(?:https?:\/\/)?(?:www\.)?([^\/:]+)(?:\/(?:[^\/]+\/)*([^\/?#]+))?$)");
        std::smatch matches;

        if (std::regex_match(url, matches, urlPattern)) {
            std::string host = matches[1];
            std::string lastItem = matches[2];

            this->logger.info(host);
            this->logger.info(lastItem);

            if (lastItem.empty() || lastItem.find('.') == std::string::npos) {
                return host + ".html";
            } else {
                return lastItem;
            }
        }

        std::ostringstream ss;
        ss << "could not parse url properly: " << url;
        this->logger.error(ss.str());
        throw std::runtime_error("error parsing url");
}


