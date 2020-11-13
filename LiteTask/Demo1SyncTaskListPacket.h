#pragma once

#include "LiteSyncTaskListPacket.h"

class Demo1SyncTaskListPacket :public LiteSyncTaskListPacket {

    /**
     * 执行任务:注意，涉及被多线程访问的属性，都要加锁<br>
     * @param content
     * @return
     */
public:
    TaskResult executFunctionOneTask();

};

