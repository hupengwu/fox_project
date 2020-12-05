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
    virtual void handleOpen(int fd);

    /**
     * ����Read��Ϣ�������յ��ͻ��˷��͹���������ʱ���Ჶ���������
     * ����
     *
     * @param key FoxStty
     */
    virtual void handleRead(int fd, const unsigned char* buff, int length);

    /**
     * ����NoRead��Ϣ��SELECT����һ����ʱ������û���յ������ݣ��Ჶ���������
     * ����
     *
     * @param key FoxStty
     */
    virtual void handleNoRead(int fd);

 
    /**
     * ����ر�Socket��Ϣ��������˹رյ�ʱ�򣬻Ჶ���������
     * ����
     *
     * @param key FoxStty
     */
    virtual void handleClosed(int fd);
};

