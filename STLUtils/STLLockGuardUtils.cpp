#include "STLLockGuardUtils.h"

void STLLockGuardUtils::demo()
{
	std::lock_guard<std::mutex> guard(this->lock);

	/*
	* 需要临界区保护的代码
	*/

}
