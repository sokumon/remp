#include "request.hpp"
#include "helper.hpp"
#include "template.hpp"

#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <sstream>
#include <nlohmann/json.hpp>
#include <cppcodec/base64_default_rfc4648.hpp>

using json = nlohmann::json;
using base64 = cppcodec::base64_rfc4648;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}


std::string fetch_device_code(){
    CURL *curl;
    CURLcode res;
    std::string responseData;
    curl = curl_easy_init();
    if(curl){
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_URL, "https://github.com/login/device/code");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_mime *mime;
    curl_mimepart *part;
    mime = curl_mime_init(curl);
    part = curl_mime_addpart(mime);
    curl_mime_name(part, "client_id");
    curl_mime_data(part, "Iv1.3042183b68c00c00", CURL_ZERO_TERMINATED);
    curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
    res = curl_easy_perform(curl);
    curl_mime_free(mime);
    }
    curl_easy_cleanup(curl);
    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        return "error while fetching device code";
    } else {
        return responseData;
    }
}


std::string fetch_access_token(const char * device_code){
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    std::string responseData;
    if(curl) {
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_URL, "https://github.com/login/oauth/access_token");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Cookie: _octo=GH1.1.827914842.1702826611; logged_in=no");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_mime *mime;
    curl_mimepart *part;
    mime = curl_mime_init(curl);
    part = curl_mime_addpart(mime);
    curl_mime_name(part, "client_id");
    curl_mime_data(part, "Iv1.3042183b68c00c00", CURL_ZERO_TERMINATED);
    part = curl_mime_addpart(mime);
    curl_mime_name(part, "device_code");
    curl_mime_data(part, device_code , CURL_ZERO_TERMINATED);
    part = curl_mime_addpart(mime);
    curl_mime_name(part, "grant_type");
    curl_mime_data(part, "urn:ietf:params:oauth:grant-type:device_code", CURL_ZERO_TERMINATED);
    curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
    res = curl_easy_perform(curl);
    curl_mime_free(mime);
    }
    curl_easy_cleanup(curl);
    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        return "error while fetching device code";
    } else {
        return responseData;
    }
}

void auth(){
    std::cout << "hello";
    std::string res = fetch_device_code();
    // std::cout << res;
    std::string dev_code = get_device_code(res);
    std::string user_code = get_user_code(res);
    std::string visit_this  = get_verification_uri(res);

    std::cout << "Go to this url on your browser: " << visit_this << std::endl;
    std::cout << "Type this code : " << user_code << std::endl; 

    std::unordered_map<std::string,std::string> res_map;

    while(1){
        std::string res_string = fetch_access_token(dev_code.c_str());
    
        res_map = ParseString(res_string);

        // std::cout << res_string << std::endl;

        if (res_map["error"] != ""){
            std::string error_type = res_map["error"];
            std::cout << error_type << std::endl; 
            
            if (error_type == "authorization_pending") {
                std::this_thread::sleep_for(std::chrono::milliseconds(5000));
            } else if (error_type == "slow_down") {
                std::this_thread::sleep_for(std::chrono::milliseconds(10000));
            } else if (error_type == "expired_token") {
                std::cout << "Run 'remp -a' again" << std::endl;
            } else if (error_type == "access_denied") {
                std::cout << "User denied access" << std::endl;
                exit(1);
            } else {
                std::cout << res_string << std::endl;
                exit(1);
            }
        }else{
            // access token write it in a file 

            std::unordered_map<std::string,std::string> at_map;
            at_map = ParseString(res_string);
            if(at_map["access_token"] != ""){
                std::string access_token = at_map["access_token"];
                // setenv("REMP_AT",access_token.c_str(), true);
                writehiddenfiletohome(access_token);
                std::cout << "Its done";
                exit(0);
            }
        }
    }
    
}

void create_new_repo(std::string repo_name){
    json jsonObject;

    jsonObject["name"] = repo_name;
    std::string req_data = jsonObject.dump();

    std::stringstream access_token;

    CURL *curl;
    CURLcode res;

    std::string responseData;

    long code;
    curl = curl_easy_init();
    if(curl) {
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);
    curl_easy_setopt(curl, CURLOPT_URL, "https://api.github.com/user/repos");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
    
    struct curl_slist *headers = NULL;
    access_token << "Authorization: Bearer ";
    access_token << readhiddenfilefromhome();
    headers = curl_slist_append(headers, "Accept: application/vnd.github+json");
    headers = curl_slist_append(headers, "User-Agent: sokumon");
    headers = curl_slist_append(headers, access_token.str().c_str());
    headers = curl_slist_append(headers, "X-GitHub-Api-Version: 2022-11-28");
    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    const char *data = req_data.c_str();
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
    res = curl_easy_perform(curl);
    }
    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    } else {
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
        std::cout << code << std::endl;
        if (code == 422){
            std::cout << "A repo with the name exists" << std::endl;
            exit(1);
        }

        if (code == 201){
            std::cout << "Repo is created" << std::endl;
        }else{
            std::cout << "Run 'remp -a' again" << std::endl;
            exit(1);
        }

    }
    curl_easy_cleanup(curl);
}


std::string whoami(){
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    std::string responseData;
    long code;
    std::stringstream access_token;


    access_token << "Authorization: Bearer ";
    access_token << readhiddenfilefromhome();
    if(curl) {
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(curl, CURLOPT_URL, "https://api.github.com/user");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/vnd.github+json");
    headers = curl_slist_append(headers, "User-Agent: sokumon");
    headers = curl_slist_append(headers, access_token.str().c_str());
    headers = curl_slist_append(headers, "X-GitHub-Api-Version: 2022-11-28");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    res = curl_easy_perform(curl);
    }

    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    } else {
        // std::cout << responseData << std::endl;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
        if(code == 200){
            json jsonData = json::parse(responseData);
            return jsonData["login"];
        }

    }


    curl_easy_cleanup(curl);

    return "error";

}




void create_new_readme(std::string path,std::string content){
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    std::string responseData;
    std::stringstream access_token;

    json jsonObject;

    jsonObject["message"] = "first commit";

    jsonObject["content"] = base64::encode(content);

    std::string req_data = jsonObject.dump();

    long code;
    // std::cout << req_data;
     
    if(curl) {
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(curl, CURLOPT_URL, path.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
    struct curl_slist *headers = NULL;
    access_token << "Authorization: Bearer ";
    access_token << readhiddenfilefromhome();
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);
    headers = curl_slist_append(headers, "User-Agent: sokumon");
    headers = curl_slist_append(headers, "Accept: application/vnd.github+json");
    headers = curl_slist_append(headers, access_token.str().c_str());
    headers = curl_slist_append(headers, "X-GitHub-Api-Version: 2022-11-28");
    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    const char *data = req_data.c_str();
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
    res = curl_easy_perform(curl);
    }

    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    } else {
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
        std::cout << code << std::endl;
        if (code == 422){
            std::cout << "A repo with the name exists" << std::endl;
            exit(1);
        }

        if (code == 201){
            std::cout << "Readme File is added" << std::endl;
        }

    }

    curl_easy_cleanup(curl);
}


void default_readme_file(std::string user_name,std::string repo_name){
    default_template(repo_name);
    std::string data_readme = fetch_readme();

    // "https://api.github.com/repos/OWNER/REPO/contents/PATH"
    std::stringstream url;
    url << "https://api.github.com/repos/";
    url << user_name;
    url << "/";
    url << repo_name;
    url << "/";
    url << "contents/README.md";


    create_new_readme(url.str(),data_readme);
}

void user_readme_file(std::string user_name,std::string repo_name,std::string file_name){
    std::string data_readme = get_file_contents(file_name);
    
    
    std::stringstream url;
    url << "https://api.github.com/repos/";
    url << user_name;
    url << "/";
    url << repo_name;
    url << "/";
    url << "contents/README.md";

    create_new_readme(url.str(),data_readme);
}