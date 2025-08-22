#pragma once
#ifndef SAVE_LOADER_H
#define SAVE_LOADER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype> 
#include "../headers/Pstring.h"
#include "../headers/object.h"
#include "../headers/camera.h"

#include <glm/glm.hpp>

class saveLoader {

public:
	saveLoader();
	std::vector<Object> loadSave(const std::string& savePath, Camera& cam);
private:
	void readFile(std::string path);
	std::vector<std::string> lines;
    float safeStof(const std::string& str) {
        try {
            //remove any whitespace or unwanted characters first
            std::string cleanStr;
            for (char c : str) {
                if (std::isdigit(c) || c == '.' || c == '-' || c == '+') {
                    cleanStr += c;
                }
            }
            if (cleanStr.empty()) return 0.0f;
            return std::stof(cleanStr);
        }
        catch (...) {
            std::cerr << "Warning: Failed to convert '" << str << "' to float, using 0.0" << std::endl;
            return 0.0f;
        }
    }
};

#endif