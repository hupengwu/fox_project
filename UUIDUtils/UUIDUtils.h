#pragma once

#include <string>

/*
* 
uuid/uuid.h�ǵ������⣬Ҫ��linux��ʹ��sudo apt-get install uuid-dev���װ���ſ��Ա���ô���
�����ڹ��̵ı������Ӳ�����Ҫ����-luuid����
*/

class UUIDUtils
{
public:
	static std::string generateUUID();
};

