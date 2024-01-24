#include <iostream>
#include <curl/curl.h>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t total_size = size * nmemb;
    output->append(static_cast<char*>(contents), total_size);
    return total_size;
}

int main() {
    // Initialize libcurl
    CURL* curl;
    CURLcode res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (res != CURLE_OK) {
        std::cerr << "Failed to initialize libcurl: " << curl_easy_strerror(res) << std::endl;
        return 1;
    }

    // Create a libcurl handle
    curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to create libcurl handle." << std::endl;
        return 1;
    }

    // Set the target URL
    const char* url = "https://www.example.com";
    curl_easy_setopt(curl, CURLOPT_URL, url);

    // Set the callback function to handle the response
    std::string response;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    // Perform the request
    res = curl_easy_perform(curl);

    // Check for errors
    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    } else {
        // Print the response
        std::cout << "Response: " << response << std::endl;
    }

    // Clean up
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}
