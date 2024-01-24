#define CURL_STATICLIB
#include <iostream>
#include <curl/curl.h>
#include <fstream>
#include <string>
#include <filesystem>
#include <map>
#include <cstring>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int writehiddenfiletohome(std::string tokenContent){
    const char* homeDir = getenv("HOME");

    if (homeDir == nullptr || homeDir[0] == '\0') {
        std::cerr << "Failed to retrieve HOME environment variable." << std::endl;
        return 1;
    }

    // Construct the file path to the hidden file in the user's home directory
    std::string hiddenFilePath = std::string(homeDir) + "/.token";

    // Create and write to the hidden file
    std::ofstream hiddenOutFile(hiddenFilePath);
    if (hiddenOutFile.is_open()) {
        hiddenOutFile << tokenContent << std::endl;
        hiddenOutFile.close();
        // std::cout << "Hidden file 'token' written successfully to: " << hiddenFilePath << std::endl;
    } else {
        // std::cerr << "Unable to open the hidden file for writing." << std::endl;
        return 1;
    }

    return 0;
}
// Callback function to write received data
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

int count = 0 ; 
std::map<std::string,std::string> ParseString(std::string res_str){
    std::map<std::string,std::string> m;
    std::vector<std::string> pairs;
    std::string delimiter = "&";
    int pos;
    while ((pos = res_str.find('&')) != std::string::npos) {
        std::string token = res_str.substr(0, pos);
        pairs.push_back(token);
        res_str.erase(0,pos+1);
    }

    pairs.push_back(res_str);

    for (std::string i: pairs){
        m[i.substr(0,i.find("="))] = i.substr(i.find("=") + 1); 
    }

    return m;


    
}

// void create_new_repo(std::string name){
//     std::stringstream data;
//     data << "{\"name\":\"";
//     data << name;
//     data << "\"}";

//     std::cout << data.str();
// }


int main() {
    // Initialize libcurl
    // CURL *curl;
    // CURLcode res;
    // std::string responseData;
    // curl = curl_easy_init();
    // if(curl) {
    // curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    // curl_easy_setopt(curl, CURLOPT_URL, "https://github.com/login/device/code");
    // curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    // curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
    // curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    // curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);
    // struct curl_slist *headers = NULL;
    // headers = curl_slist_append(headers, "Accept: application/json");
    // curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    // curl_mime *mime;
    // curl_mimepart *part;
    // mime = curl_mime_init(curl);
    // part = curl_mime_addpart(mime);
    // curl_mime_name(part, "client_id");
    // curl_mime_data(part, "Iv1.3042183b68c00c00", CURL_ZERO_TERMINATED);
    // curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
    // res = curl_easy_perform(curl);
    // curl_mime_free(mime);
    // }
    // curl_easy_cleanup(curl);

    // // Check for errors
    // if (res != CURLE_OK) {
    //     std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    // } else {
    //     // Print the received data
    //     std::cout << "Received data:\n" << responseData << std::endl;
    // }

    // std::string test = "{"device_code":"eceb930ad7f65035f30a7bd0b6bdd3f286b468e8","user_code":"08B8-3970","verification_uri":"https://github.com/login/device","expires_in":898,"interval":5}"'
    // std::cout << test;

    const std::string filePath = "test.json";  // Replace with your file path

    // if (!std::filesystem::exists(filePath)) {
    //     std::cerr << "File not found." << std::endl;
    //     return 1;
    // }

    std::ifstream inputFile(filePath);

    if (!inputFile.is_open()) {
        std::cerr << "Error opening the file." << std::endl;
        return 1;
    }

    std::string fileContents((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

    json jsonData = json::parse(fileContents);

    std::string name = jsonData["verification_uri"];
    // std::cout << name;
    inputFile.close();

//     CURL *curl;
// CURLcode res;
// curl = curl_easy_init();
//  std::string responseData;
// if(curl) {
//   curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
//   curl_easy_setopt(curl, CURLOPT_URL, "https://github.com/login/oauth/access_token");
//   curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
//   curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
//       curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
//     curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);
//   struct curl_slist *headers = NULL;
//   headers = curl_slist_append(headers, "Cookie: _octo=GH1.1.827914842.1702826611; logged_in=no");
//   curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
//   curl_mime *mime;
//   curl_mimepart *part;
//   mime = curl_mime_init(curl);
//   part = curl_mime_addpart(mime);
//   curl_mime_name(part, "client_id");
//   curl_mime_data(part, "Iv1.3042183b68c00c00", CURL_ZERO_TERMINATED);
//   part = curl_mime_addpart(mime);
//   curl_mime_name(part, "device_code");
//   curl_mime_data(part, "eceb930ad7f65035f30a7bd0b6bdd3f286b468e8", CURL_ZERO_TERMINATED);
//   part = curl_mime_addpart(mime);
//   curl_mime_name(part, "grant_type");
//   curl_mime_data(part, "urn:ietf:params:oauth:grant-type:device_code", CURL_ZERO_TERMINATED);
//   curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
//   res = curl_easy_perform(curl);
//   curl_mime_free(mime);
// }
// curl_easy_cleanup(curl);
//     if (res != CURLE_OK) {
//         std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
//     } else {
//         // Print the received data
//         std::cout << "Received data:\n" << responseData << std::endl;
//     }


// std::string testing = "error=authorization_pending&error_description=The+authorization+request+is+still+pending.&error_uri=https%3A%2F%2Fdocs.github.com%2Fdevelopers%2Fapps%2Fauthorizing-oauth-apps%23error-codes-for-the-device-flow";
// std::map<std::string,std::string> res_map = ParseString(testing);
// std::cout << res_map["error"] <<std::endl;
    // create_new_repo("remp");
    // std::cout << getenv("HOME") << std::endl;
    // std::stringstream data;
    // data << "{\"name\":\"";
    // data << name;
    // data << "\"}";
    // std::cout << data.str();
    // std::cout << "hello win32";
    std::cout << getenv("APPDATA");
    // writehiddenfiletohome("sohma");
    return 0;
}

