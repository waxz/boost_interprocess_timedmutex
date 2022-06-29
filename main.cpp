#include <iostream>
#include <thread>


#include "clock_time.h"
#include "suspend.h"
#include "signal_handler.h"

#include "bip_timedmutex.h"
int main() {
    std::cout << "Hello, World!" << std::endl;

      namespace bip = boost::interprocess;

    std::thread t1([ ]{
        bip::managed_shared_memory managed_shm{bip::open_or_create, "shm", 1024};
        int *i = managed_shm.find_or_construct<int>("Integer")();
        common::BipTimedMutex *time_mtx = managed_shm.find_or_construct<common::BipTimedMutex>("time_mtx")();

        time_mtx->lock();
        std::thread::id this_id = std::this_thread::get_id();
        common::Suspend suspend;
        for(int ii = 0 ; ii < 20;ii++){
            suspend.sleep(100);
            std::cout << "thread locking : " << this_id << std::endl;

        }
        ++(*i);
        std::cout << *i << '\n';
        std::cout << "thread quit : " << this_id << std::endl;

        time_mtx->unlock();

    });

    std::thread t2([ ]{
        bip::managed_shared_memory managed_shm{bip::open_or_create, "shm", 1024};
        int *i = managed_shm.find_or_construct<int>("Integer")();
        common::BipTimedMutex *time_mtx = managed_shm.find_or_construct<common::BipTimedMutex>("time_mtx")();

        time_mtx->lock();
        std::thread::id this_id = std::this_thread::get_id();
        common::Suspend suspend;
        for(int ii = 0 ; ii < 10;ii++){
            suspend.sleep(100);
            std::cout << "thread locking : " << this_id << std::endl;

        }
        ++(*i);
        std::cout << *i << '\n';
        std::cout << "thread quit : " << this_id << std::endl;

        time_mtx->unlock();

    });
    std::thread t3([ ]{
        bip::managed_shared_memory managed_shm{bip::open_or_create, "shm", 1024};
        int *i = managed_shm.find_or_construct<int>("Integer")();
        common::BipTimedMutex *time_mtx = managed_shm.find_or_construct<common::BipTimedMutex>("time_mtx")();

        bip::scoped_lock<common::BipTimedMutex> lock(*time_mtx);
        std::thread::id this_id = std::this_thread::get_id();
        common::Suspend suspend;
        for(int ii = 0 ; ii < 10;ii++){
            suspend.sleep(100);
            std::cout << "thread locking : " << this_id << std::endl;

        }
        ++(*i);
        std::cout << *i << '\n';
        std::cout << "thread quit : " << this_id << std::endl;

    });

    t1.join();

    t2.join();
    t3.join();

    common::spin();



    return 0;
}
