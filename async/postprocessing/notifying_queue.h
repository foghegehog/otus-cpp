#ifndef NOTIFYING_QUEUE_H
#define NOTIFYING_QUEUE_H

#include <condition_variable>
#include <mutex>
#include <queue>

namespace postprocessing{

template<typename T> 
class notifying_queue {

public:

	notifying_queue() { }
	notifying_queue(const notifying_queue& other) = delete;

	T pop() {
        using namespace std;

        unique_lock<mutex> lock(m_mutex);
        m_cv.wait(lock, [this]{ return m_queue.size() > 0; });
		T t = m_queue.front();
		m_queue.pop();
		lock.unlock();
		return t;	
	}

	void put(const T& t) {
        using namespace std;

        {
		    unique_lock<mutex> lock(m_mutex);
		    m_queue.push(t);
        }
		m_cv.notify_all();
	}

private:
	std::queue<T> m_queue;
	std::mutex m_mutex;
	std::condition_variable m_cv;
};

}
#endif