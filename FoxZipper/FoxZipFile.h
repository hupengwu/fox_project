#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "zstr.hpp"

using namespace std;

class FoxZipFile
{
public:
    static void decompressFiles(const std::vector<std::string>& fileList, const std::string& outputFile);

    static void compressFiles(const std::vector<std::string>& fileList, const std::string& outputFile);
private:
    static void catStream(std::istream& is, std::ostream& os);
};

