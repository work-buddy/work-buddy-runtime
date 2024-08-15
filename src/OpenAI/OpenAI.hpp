#ifndef OPENAI_HPP
#define OPENAI_HPP

#include <string>
#include <vector>
#include <utility>
#include <curl/curl.h>

struct OpenAIMessage
{
    std::string role;
    std::string content;
};

struct OpenAIChoice
{
    int index;
    OpenAIMessage message;
    std::string logprobs;
    std::string finish_reason;
};

struct OpenAIUsage
{
    int prompt_tokens;
    int completion_tokens;
    int total_tokens;
};

struct OpenAIResponse
{
    std::string id;
    std::string object;
    long created;
    std::string model;
    std::vector<OpenAIChoice> choices;
    OpenAIUsage usage;
    std::string system_fingerprint;
};

class OpenAI
{
public:
    OpenAI(const std::string &apiKey);
    ~OpenAI();

    std::string chatCompletion(const std::string &model, const std::vector<std::pair<std::string, std::string>> &messages, double temperature, const std::string &base64Image = "");

private:
    std::string apiKey;
    CURL *curl;

    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
    std::string base64Encode(const std::string &filePath);
};

#endif // OPENAI_HPP