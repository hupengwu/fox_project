#pragma once

using namespace std;

/**
 * 捕获数据的数据接口
 *
 * @author h00442047
 * @since 2019年12月10日
 */
class FoxSttyHandler
{
public:
    FoxSttyHandler();
    virtual ~FoxSttyHandler();

public:
    /**
     * 处理Accept消息：当一个客户端接入服务端的时候，会捕获到这个动作
     * 对象：FoxStty
     *
     * @param key FoxStty
     */
    virtual void handleOpen(int fd);

    /**
     * 处理Read消息：当接收到客户端发送过来的数据时，会捕获到这个动作
     * 对象：
     *
     * @param key FoxStty
     */
    virtual void handleRead(int fd, const unsigned char* buff, int length);

    /**
     * 处理NoRead消息：SELECT超过一定的时间间隔都没有收到该数据，会捕获到这个动作
     * 对象：
     *
     * @param key FoxStty
     */
    virtual void handleNoRead(int fd);

 
    /**
     * 处理关闭Socket消息：当服务端关闭的时候，会捕获到这个动作
     * 对象：
     *
     * @param key FoxStty
     */
    virtual void handleClosed(int fd);
};

