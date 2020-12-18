#pragma once

using namespace std;

/**
 * �������ݵ����ݽӿ�
 *
 * @author h00442047
 * @since 2019��12��10��
 */
class FoxSttyHandler
{
public:
    FoxSttyHandler();
    virtual ~FoxSttyHandler();

public:
    /**
     * ����Accept��Ϣ����һ���ͻ��˽������˵�ʱ�򣬻Ჶ���������
     * ����FoxStty
     *
     * @param key FoxStty
     */
    virtual void handleOpen(const int fd,const char* name);

    /**
     * ����Read��Ϣ�������յ��ͻ��˷��͹���������ʱ���Ჶ���������
     * ����
     *
     * @param key FoxStty
     */
    virtual void handleRead(const int fd, const char* name, const unsigned char* buff, int length);

    /**
     * ����NoRead��Ϣ��SELECT����һ����ʱ������û���յ������ݣ��Ჶ���������
     * ����
     *
     * @param key FoxStty
     */
    virtual void handleNoRead(const int fd, const char* name);

 
    /**
     * ����ر�Socket��Ϣ��������˹رյ�ʱ�򣬻Ჶ���������
     * ����
     *
     * @param key FoxStty
     */
    virtual void handleClosed(const int fd, const char* name);
};

