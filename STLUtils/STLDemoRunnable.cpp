#include "STLDemoRunnable.h"

#include <thread>

void STLDemoRunnable::run()
{
    std::thread::id thread_id = std::this_thread::get_id();
    static int i = 0;
    i++;
    printf("thread id = %d, count = %d\n", thread_id,i);
    this_thread::sleep_for(chrono::milliseconds(100 * 1));
}
