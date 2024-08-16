#ifndef OPENAI_HPP
#define OPENAI_HPP

#include <string>
#include <vector>
#include <functional>
#include <jsoncpp/json/json.h>
#include <curl/curl.h>
#include <future>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#include <fstream>
#include <sstream>

class OpenAI
{
public:
    explicit OpenAI(const std::string &apiKey);
    ~OpenAI();

    std::string chatCompletion(const std::string &model, const std::vector<std::pair<std::string, std::string>> &messages, double temperature, const std::string &base64Image = "");
    void chatCompletionAsync(const std::string &model, const std::vector<std::pair<std::string, std::string>> &messages, double temperature, const std::string &base64Image, std::function<void(const std::string &)> callback);

private:
    std::string apiKey;
    CURL *curl;

    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
    std::string base64Encode(const std::string &filePath);
};

#endif // OPENAI_HPP
