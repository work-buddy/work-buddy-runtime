#include "OpenAI.hpp"

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
    std::string *str = static_cast<std::string *>(userp);
    str->append(static_cast<char *>(contents), size * nmemb);
    return size * nmemb;
}

std::string OpenAI::base64Encode(const std::string &filePath)
{
    std::ifstream file(filePath, std::ios::binary);
    std::ostringstream oss;
    oss << file.rdbuf();
    std::string fileContent = oss.str();

    BIO *bio = BIO_new(BIO_f_base64());
    BIO *b64 = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    BIO_write(bio, fileContent.data(), fileContent.size());
    BIO_flush(bio);

    BUF_MEM *bufferPtr;
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

        Json::Value content(Json::arrayValue);

        Json::Value textContent;
        textContent["type"] = "text";
        textContent["text"] = message.second;
        content.append(textContent);

        if (!base64Image.empty() && message.first == "user")
        {
            Json::Value imageContent;
            imageContent["type"] = "image_url";
            imageContent["image_url"]["url"] = "data:image/jpeg;base64," + base64Image;
            content.append(imageContent);
        }

        msg["content"] = content;
        messagesJson.append(msg);
    }
    root["messages"] = messagesJson;

    Json::StreamWriterBuilder writer;
    std::string jsonData = Json::writeString(writer, root);

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, ("Authorization: Bearer " + apiKey).c_str());
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(headers);

    if (res != CURLE_OK)
    {
        return "CURL request failed: " + std::string(curl_easy_strerror(res));
    }

    return readBuffer;
}

void OpenAI::chatCompletionAsync(const std::string &model, const std::vector<std::pair<std::string, std::string>> &messages, double temperature, const std::string &base64Image, std::function<void(const std::string &)> callback)
{
    std::thread([this, model, messages, temperature, base64Image, callback]()
                {
        std::string response;

        CURL *curl = curl_easy_init();
        if (curl)
        {
            std::string url = "https://api.openai.com/v1/chat/completions";
            Json::Value root;
            root["model"] = model;
            root["temperature"] = temperature;

            Json::Value messagesJson(Json::arrayValue);
            for (const auto &message : messages)
            {
                Json::Value msg;
                msg["role"] = message.first;

                Json::Value content(Json::arrayValue);

                Json::Value textContent;
                textContent["type"] = "text";
                textContent["text"] = message.second;
                content.append(textContent);

                if (!base64Image.empty() && message.first == "user")
                {
                    Json::Value imageContent;
                    imageContent["type"] = "image_url";
                    imageContent["image_url"]["url"] = "data:image/jpeg;base64," + base64Image;
                    content.append(imageContent);
                }

                msg["content"] = content;
                messagesJson.append(msg);
            }
            root["messages"] = messagesJson;

            Json::StreamWriterBuilder writer;
            std::string jsonData = Json::writeString(writer, root);

            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

            struct curl_slist *headers = nullptr;
            headers = curl_slist_append(headers, ("Authorization: Bearer " + apiKey).c_str());
            headers = curl_slist_append(headers, "Content-Type: application/json");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

            CURLcode res = curl_easy_perform(curl);
            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);

            if (res != CURLE_OK)
            {
                response = "CURL request failed: " + std::string(curl_easy_strerror(res));
            }
        }
        else
        {
            response = "CURL initialization failed";
        }

        callback(response); })
        .detach();
}
