#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <Shlwapi.h> 
#include "zip.h"
#include "unzip.h"
#include "zlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

using namespace std;

class FoxZipFile
{
public:
	bool nyAddfiletoZip(zipFile zfile, const std::string& fileNameinZip, const std::string& srcfile);
};

