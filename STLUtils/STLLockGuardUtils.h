#pragma once

#include <mutex>

using namespace std;

class STLLockGuardUtils
{
public:
	void demo();
private:
	mutex lock;
};

