timed mutex in boost::interprocess in case of program cash without release lock

```c++
{
    common::BipTimedMutex *time_mtx = managed_shm.find_or_construct<common::BipTimedMutex>("time_mtx")();
    bip::scoped_lock<common::BipTimedMutex> lock(*time_mtx);
}
```

