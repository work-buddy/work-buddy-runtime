#include "OpenAI.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <jsoncpp/json/json.h>
#include <curl/curl.h>
#include <vector>
#include <utility>
#include <string>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>

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

std::string OpenAI::base64Encode(const std::string &filePath)
{
    std::ifstream file(filePath, std::ios::binary);
    std::ostringstream oss;
    oss << file.rdbuf();
    std::string fileContent = oss.str();

    BIO *bio, *b64;
    BUF_MEM *bufferPtr;

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    BIO_write(bio, fileContent.data(), fileContent.size());
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &bufferPtr);

    std::string encodedData(bufferPtr->data, bufferPtr->length);
    BIO_free_all(bio);

    return encodedData;
}

std::string OpenAI::chatCompletion(const std::string &model, const std::vector<std::pair<std::string, std::string>> &messages, double temperature, const std::string &base64Image)
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

        if (message.first == "user")
        {
            Json::Value content(Json::arrayValue);
            Json::Value textContent;
            textContent["type"] = "text";
            textContent["text"] = message.second;
            content.append(textContent);

            if (!base64Image.empty())
            {
                Json::Value imageContent;
                imageContent["type"] = "image_url";
                imageContent["image_url"]["url"] = "data:image/jpeg;base64," + base64Image;
                content.append(imageContent);
            }

            msg["content"] = content;
        }
        else
        {
            msg["content"] = message.second;
        }

        messagesJson.append(msg);
    }
    root["messages"] = messagesJson;

    // Convert JSON to string
    Json::StreamWriterBuilder writer;
    std::string jsonData = Json::writeString(writer, root);

    // Set CURL options
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, ("Authorization: Bearer " + apiKey).c_str());
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // Perform the request
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        return "CURL request failed: " + std::string(curl_easy_strerror(res));
    }

    // Cleanup
    curl_slist_free_all(headers);

    return readBuffer;
}