#include "OpenAI.hpp"
#include <iostream>
#include <sstream>
#include <jsoncpp/json/json.h>

OpenAI::OpenAI(const std::string &apiKey) : apiKey(apiKey)
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
}

OpenAI::~OpenAI()
{
    if (curl)
    {
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

size_t OpenAI::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

std::string OpenAI::chatCompletion(const std::string &model, const std::vector<std::pair<std::string, std::string>> &messages, double temperature)
{
    if (!curl)
    {
        return "CURL initialization failed";
    }

    std::string url = "https://api.openai.com/v1/chat/completions";
    std::string readBuffer;

    Json::Value root;
    root["model"] = model;
    root["temperature"] = temperature;

    Json::Value messagesJson(Json::arrayValue);
    for (const auto &message : messages)
    {
        Json::Value msg;
        msg["role"] = message.first;
        msg["content"] = message.second;
        messagesJson.append(msg);
    }
    root["messages"] = messagesJson;

    Json::StreamWriterBuilder writer;
    std::string jsonData = Json::writeString(writer, root);

    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, ("Authorization: Bearer " + apiKey).c_str());

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    }

    curl_slist_free_all(headers);

    return readBuffer;
}