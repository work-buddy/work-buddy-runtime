#ifndef OPENAI_HPP
#define OPENAI_HPP

#include <string>
#include <vector>
#include <curl/curl.h>

class OpenAI
{
public:
    OpenAI(const std::string &apiKey);
    ~OpenAI();

    std::string chatCompletion(const std::string &model, const std::vector<std::pair<std::string, std::string>> &messages, double temperature);

private:
    std::string apiKey;
    CURL *curl;

    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
};

#endif // OPENAI_HPP