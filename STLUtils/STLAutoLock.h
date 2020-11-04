#pragma once

#include <mutex>

using namespace std;

//======================================================================
// 类型名称:xAutoLock
// 功能描述:临界区的自动锁定和解锁
// 创建日期:2006-03-27
// 修改日期:
// 作    者:胡鹏武
// 备    注:利用对象生存期的特性进行自动解锁和锁定
//			根据对象的生存期的原理,在对象的生成阶段进行临界区锁定的操作;
//			在对象的销毁阶段进行临界区解锁的操作;
//	
// 示    例:
//			BOOL CQueueControlChange::GetControl(CControl &ctrlControl)
//			{
//				// 通过m_mutex.m_strName可以检查死锁的位置
//				xAutoLock AutoLock(m_mutex,"CQueueControlChange");
//
//				int nSize = m_arrControl.GetSize();
//				if (nSize<=0){
//					return FALSE;
//				}
//
//				ctrlControl = m_arrControl[0];
//				
//				m_arrControl.RemoveAt(0);
//
//				return TRUE;
//			}	
//======================================================================

class STLAutoLock
{
public:
	STLAutoLock(mutex& mutex);
	virtual ~STLAutoLock();

private:
	mutex* mutexRef;
};

