#include <nlohmann/json.hpp>
#include <iostream>
#include <sstream>
#include "helper.hpp"
#include <iomanip>
#include <fstream>

using json = nlohmann::json;
// std::string test = "{"device_code":"eceb930ad7f65035f30a7bd0b6bdd3f286b468e8","user_code":"08B8-3970","verification_uri":"https://github.com/login/device","expires_in":898,"interval":5}""
std::string get_device_code(std::string response){
    json jsonData = json::parse(response);
    return jsonData["device_code"];
}

std::string get_user_code(std::string response){
    json jsonData = json::parse(response);
    return jsonData["user_code"];
}

std::string get_verification_uri(std::string response){
    json jsonData = json::parse(response);
    return jsonData["verification_uri"];
}

std::unordered_map<std::string,std::string> ParseString(std::string res_str){
    std::unordered_map<std::string,std::string> m;
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


std::string base64_encode(const std::string& input) {
    // Create a stringstream to hold the encoded characters
    std::stringstream encodedStream;

    // Create a stream to encode the input string
    std::istringstream inputStream(input);

    // Iterate through every three bytes of input
    while (true) {
        unsigned char buffer[3];
        inputStream.read(reinterpret_cast<char*>(buffer), 3);

        // Break if no more input
        if (inputStream.gcount() == 0) {
            break;
        }

        // Encode the three bytes into four Base64 characters
        encodedStream << static_cast<char>((buffer[0] & 0xfc) >> 2);
        encodedStream << static_cast<char>(((buffer[0] & 0x03) << 4) | ((buffer[1] & 0xf0) >> 4));
        encodedStream << static_cast<char>(((buffer[1] & 0x0f) << 2) | ((buffer[2] & 0xc0) >> 6));
        encodedStream << static_cast<char>(buffer[2] & 0x3f);
    }

    // Pad the encoded string with '=' if needed
    std::string result = encodedStream.str();
    size_t padding = (3 - input.size() % 3) % 3;
    result.append(padding, '=');

    return result;
}


std::string UrlEncode(const std::string& value) {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (auto c : value) {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
        } else {
            escaped << '%' << std::setw(2) << int(static_cast<unsigned char>(c));
        }
    }

    return escaped.str();
}


int writehiddenfiletohome(std::string tokenContent){
    const char* homeDir = getenv("HOME");

    if (homeDir == nullptr || homeDir[0] == '\0') {
        std::cerr << "Failed to retrieve HOME environment variable." << std::endl;
        return 1;
    }

    // Construct the file path to the hidden file in the user's home directory
    std::string hiddenFilePath = std::string(homeDir) + "/.remp_token";

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

std::string readhiddenfilefromhome(){
    const char* homeDir = getenv("HOME");
    std::string hiddenFilePath = std::string(homeDir) + "/.remp_token";

     if (homeDir == nullptr || homeDir[0] == '\0') {
        std::cerr << "Failed to retrieve HOME environment variable." << std::endl;
        return "error";
    }


    // Read from the hidden file
    std::ifstream hiddenInFile(hiddenFilePath);
    if (hiddenInFile.is_open()) {
        std::string tokenContent;
        getline(hiddenInFile, tokenContent);

        hiddenInFile.close();

        // std::cout << "Read from hidden file 'token': " << tokenContent << std::endl;
        return tokenContent;
    } else {
        // std::cerr << "Unable to open the hidden file for reading." << std::endl;
        return "error";
    }

    return "error";
}

bool fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

std::string get_file_contents(std::string file_name) {
    std::ifstream file(file_name);
    
    if (!file.is_open()) {
        // Handle the case when the file cannot be opened
        std::cerr << "Error opening file: " << file_name << std::endl;
        return "";  // Return an empty string to indicate an error
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}