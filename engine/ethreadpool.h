#ifndef ETHREADPOOL_H
#define ETHREADPOOL_H

#include "etask.h"

#include "thread/ethreaddata.h"

#include <queue>
#include <thread>
#include <mutex>
#include <functional>
#include <condition_variable>

class eThreadPool {
public:
    eThreadPool(eGameBoard& board);
    ~eThreadPool();

    void initialize(const int w, const int h);
    void addBoard(const eCityId cid);

    void queueTask(eTask* const task);
    void handleFinished();

    void scheduleDataUpdate();

    bool finished();
    void waitFinished();
private:
    void threadEntry(eThreadData* data);

    int mTaskId = 0;

    eGameBoard& mBoard;

    bool mInterrupted = false;
    bool mQuit = false;

    std::mutex mFinishedTasksMutex;
    std::vector<eTask*> mFinishedTasks;

    std::vector<std::thread> mThreads;
    std::vector<eThreadData*> mThreadData;

    std::vector<eTask*> mWaitingTasks;
};

#endif // ETHREADPOOL_H
