#pragma once

#include <mutex>

using namespace std;

//======================================================================
// ��������:xAutoLock
// ��������:�ٽ������Զ������ͽ���
// ��������:2006-03-27
// �޸�����:
// ��    ��:������
// ��    ע:���ö��������ڵ����Խ����Զ�����������
//			���ݶ���������ڵ�ԭ��,�ڶ�������ɽ׶ν����ٽ��������Ĳ���;
//			�ڶ�������ٽ׶ν����ٽ��������Ĳ���;
//	
// ʾ    ��:
//			BOOL CQueueControlChange::GetControl(CControl &ctrlControl)
//			{
//				// ͨ��m_mutex.m_strName���Լ��������λ��
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

