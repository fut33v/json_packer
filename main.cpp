// json_packer assignment
// Author: Ilya Fateev

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <iostream>
#include <fstream>
#include <atomic>
#include <unordered_map>

#include "nlohmann/json.hpp"
#include "BinaryWriter.h"

using json = nlohmann::json;

std::atomic<bool> g_interrupted = false;

void SignalHandler(int s)
{
    if (s == SIGINT) {
        std::cout << "interrupted!" << std::endl;
        g_interrupted = true;
        return;
    }
    printf("Caught signal %d\n", s);
    exit(1);
}

void SetupSignalHandler()
{
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = SignalHandler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);
}


// TODO: get test file with python
// TODO: create docker file for debian system

int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cout << "usage: json_packer <input_filename> <output_filename>" << std::endl;
        return EXIT_FAILURE;
    }

    SetupSignalHandler();

    std::string fnameInput = argv[1];
    std::string fnameOutput = argv[2];
    std::ifstream inputFile(fnameInput);

    if (!inputFile.is_open()) {
        std::cout << "failed to open input file '" << fnameInput << "'" << std::endl;
        return EXIT_FAILURE;
    }

    std::unique_ptr<BinaryWriter> writer;
    try {
        writer = std::make_unique<BinaryWriter>(fnameOutput);
    } catch (std::exception &e) {
        std::cout << "failed to open output file '" << fnameOutput << "'" << std::endl;
        return EXIT_FAILURE;
    }

    std::unordered_map<std::string, int32_t> keysMap;
    std::string line;
    while (std::getline(inputFile, line) && !g_interrupted) {
        std::cout << line << std::endl; // TODO: remove
        try {
            json data = json::parse(line);
            if (!data.is_object()) {
                std::cout << "parsed line '" << line << "' is not json object" << std::endl;
                continue;
            }
            for (json::iterator it = data.begin(); it != data.end(); ++it) {
                if (!keysMap.contains(it.key())) {
                    keysMap[it.key()] = keysMap.size();
                }
                int32_t intKey = keysMap[it.key()];

                // std::cout << it.key() << " : " << it.value() << "\n";

                if (it.value().is_number_integer()) {
                    writer->WriteRecordElement(intKey, it.value().get<int>());
                }
                if (it.value().is_boolean()) {
                    writer->WriteRecordElement(intKey, it.value().get<bool>());
                }
                if (it.value().is_number_float()) {
                    writer->WriteRecordElement(intKey, it.value().get<float>());
                }
                if (it.value().is_string()) {
                    writer->WriteRecordElement(intKey, it.value().get<std::string>());
                }
            }
        } catch (json::exception &e) {
            std::cout << "failed to parse json line '" << line << "'" << std::endl;
            std::cout << e.what() << std::endl;
        }
    }

    for (auto it = keysMap.begin(); it != keysMap.end(); ++it) {
        std::cout << (*it).first << " " << (*it).second << std::endl;
    }


}