#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>

std::string get_device_code(std::string response);

std::string get_user_code(std::string response);

std::string get_verification_uri(std::string response);

std::unordered_map<std::string,std::string> ParseString(std::string res_str);

std::string UrlEncode(const std::string& value);

std::string base64_encode(const std::string& input);

int writehiddenfiletohome(std::string secret);

std::string readhiddenfilefromhome();

bool fileExists(const std::string& filename);

std::string get_file_contents(std::string file_name);