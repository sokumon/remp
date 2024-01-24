#include<iostream>
#include<curl/curl.h>

// callback for curl writing
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output);

std::string fetch_device_code();

void fetch_access_token(std::string device_code);

void auth();

void create_new_repo(std::string repo_name);

void create_readme(std::string path, std::string contents);

void user_readme_file(std::string user_name,std::string repo_name,std::string file_name);

void default_readme_file(std::string user_name,std::string repo_name);


std::string whoami();




// create new repo 

