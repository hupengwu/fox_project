#pragma once

#include <string>

/*
* 
uuid/uuid.h是第三方库，要在linux上使用sudo apt-get install uuid-dev命令安装，才可以编译该代码
并且在工程的编译链接参数中要带上-luuid参数
*/

class UUIDUtils
{
public:
	static std::string generateUUID();
};

