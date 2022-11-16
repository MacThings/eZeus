#include "ethreadpool.h"

eThreadPool::eThreadPool() {}

eThreadPool::~eThreadPool() {
    {
        std::unique_lock<std::mutex> lock(mTasksMutex);

        mQuit = true;
        mCv.notify_all();
    }
    for(auto& thread : mThreads) {
        thread.join();
    }
}

void eThreadPool::initialize(const int w, const int h) {
    //const int hc = std::thread::hardware_concurrency();
    const int threads = 1;//hc > 1 ? hc - 1 : 1;
    for(int i = 0; i < threads; i++) {
        const auto b = new eThreadData;
        b->initialize(w, h);
        mThreadData.push_back(b);
        mThreads.emplace_back(std::bind(&eThreadPool::threadEntry, this, b));
    }
}

void eThreadPool::threadEntry(eThreadData* data) {
    eTask* task = nullptr;
    while(!mQuit) {
        {
            std::unique_lock<std::mutex> lock(mTasksMutex);

            while(!mQuit && mTasks.empty()) {
                mCv.wait(lock);
            }

            if(mTasks.empty()) return;

            mBusy++;
            task = mTasks.front();
            mTasks.pop();
        }
        if(task) {
            {
                data->setRunning(true);
                data->updateBoard();
                auto& b = data->board();
                task->run(b);
                data->setRunning(false);
            }
            std::lock_guard lock(mFinishedTasksMutex);
            mFinishedTasks.push_back(task);
            mBusy--;
            mCvFinished.notify_one();
        }
    }
}

void eThreadPool::queueTask(eTask* const task) {
    std::unique_lock<std::mutex> lock(mTasksMutex);
    mTasks.emplace(task);
    mCv.notify_one();
}

void eThreadPool::handleFinished() {
    std::vector<eTask*> tasks;
    {
        std::lock_guard lock(mFinishedTasksMutex);
        std::swap(tasks, mFinishedTasks);
    }
    for(const auto t : tasks) {
        t->finish();
        delete t;
    }
}

void eThreadPool::scheduleUpdate(eGameBoard& board) {
    for(auto& d : mThreadData) {
        d->scheduleUpdate(board);
    }
}

void eThreadPool::waitFinished() {
    std::unique_lock<std::mutex> lock(mTasksMutex);
    mCvFinished.wait(lock, [this]() {
        return mTasks.empty() && (mBusy == 0);
    });
}
