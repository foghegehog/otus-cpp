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

	const T& front() {
        using namespace std;

        unique_lock<mutex> lock(m_mutex);
        m_cv.wait(lock, [this]{ return m_queue.size() > 0; });
		return m_queue.front();
	}

	void put(T&& t) {
        using namespace std;

        {
		    unique_lock<mutex> lock(m_mutex);
		    m_queue.push(move(t));
        }
		m_cv.notify_one();
	}

private:
	std::queue<T> m_queue;
	std::mutex m_mutex;
	std::condition_variable m_cv;
};

}
#endif