#include <iostream>
#include <curl/curl.h>

int main() {
    curl_version_info_data *data = curl_version_info(CURLVERSION_NOW);

    if (data) {
        std::cout << "libcurl version: " << data->version << std::endl;

        if (data->protocols) {
            std::cout << "Supported protocols:\n";

            for (size_t i = 0; data->protocols[i] != nullptr; ++i) {
                std::cout << " - " << data->protocols[i] << std::endl;
            }
        } else {
            std::cerr << "Failed to get protocol information." << std::endl;
        }
    } else {
        std::cerr << "Failed to get libcurl version information." << std::endl;
    }

    return 0;
}
