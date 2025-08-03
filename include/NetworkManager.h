#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <functional>
#include <thread>
#include <mutex>
#include <atomic>
#include <curl/curl.h>
#include <json/json.h>

class NetworkManager
{
public:
    // Callback types
    using ConnectionCallback = std::function<void(bool)>;
    using ResponseCallback = std::function<void(const std::string&)>;
    using SearchCallback = std::function<void(const Json::Value&)>;
    using AIResponseCallback = std::function<void(const std::string&)>;
    using DownloadCallback = std::function<void(const std::string&)>;
    using UploadCallback = std::function<void(bool)>;
    using ErrorCallback = std::function<void(const std::string&)>;

    explicit NetworkManager();
    ~NetworkManager();
    
    // Connection management
    void check_internet_connection();
    bool is_connected() const;
    void set_api_key(const std::string &key);
    
    // Callback setters
    void set_connection_callback(ConnectionCallback callback);
    void set_response_callback(ResponseCallback callback);
    void set_search_callback(SearchCallback callback);
    void set_ai_response_callback(AIResponseCallback callback);
    void set_download_callback(DownloadCallback callback);
    void set_upload_callback(UploadCallback callback);
    void set_error_callback(ErrorCallback callback);
    
    // API calls
    void search_web(const std::string &query);
    void query_ai(const std::string &prompt, const std::string &context = "");
    void download_code(const std::string &repository);
    void upload_learning_data(const Json::Value &data);
    
    // Custom requests
    void make_request(const std::string &url, const std::string &method = "GET", 
                     const Json::Value &data = Json::Value());
    void make_post_request(const std::string &url, const Json::Value &data);
    void make_get_request(const std::string &url);

private:
    // CURL callback function
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp);
    
    void setup_curl();
    void cleanup_curl();
    void handle_network_error(CURLcode error);
    std::string format_api_request(const std::string &prompt, const std::string &context);
    Json::Value parse_response(const std::string &data);
    std::string format_search_results(const Json::Value &results);
    
    // Threading functions
    void connection_check_loop();
    void process_request_queue();
    
    struct Request {
        std::string url;
        std::string method;
        Json::Value data;
        std::string type;
        std::function<void(const std::string&)> callback;
    };
    
    CURL* curl_handle;
    std::atomic<bool> connected{false};
    std::string api_key;
    std::string user_agent;
    
    // API endpoints
    std::string search_api_url;
    std::string ai_api_url;
    std::string code_api_url;
    
    // Callbacks
    ConnectionCallback connection_callback;
    ResponseCallback response_callback;
    SearchCallback search_callback;
    AIResponseCallback ai_response_callback;
    DownloadCallback download_callback;
    UploadCallback upload_callback;
    ErrorCallback error_callback;
    
    // Threading
    std::thread connection_thread;
    std::thread request_thread;
    std::mutex request_mutex;
    std::vector<Request> request_queue;
    std::atomic<bool> should_stop{false};
    
    // Request tracking
    int max_retries;
    int current_retries;
};

#endif // NETWORKMANAGER_H