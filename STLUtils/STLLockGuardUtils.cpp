#include "STLLockGuardUtils.h"

void STLLockGuardUtils::demo()
{
	std::lock_guard<std::mutex> guard(this->lock);

	/*
	* ��Ҫ�ٽ��������Ĵ���
	*/

}
