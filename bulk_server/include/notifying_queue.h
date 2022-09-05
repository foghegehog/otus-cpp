#ifndef NOTIFYING_QUEUE_H
#define NOTIFYING_QUEUE_H

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>

namespace async{

template<typename T> 
class notifying_queue {

public:

	notifying_queue() { }
	notifying_queue(const notifying_queue& other) = delete;

	bool try_pop(T& t) {
        using namespace std;

        unique_lock<mutex> lock(m_mutex);
        m_cv.wait(lock, [this]{ return (m_queue.size() > 0) || m_stopping.load(); });

		if (m_queue.size() > 0)
		{
			t = std::move(m_queue.front());
			m_queue.pop();
			return true;
		}
		
		return false;	
	}

	void put(const T& t) {
        using namespace std;

        {
		    unique_lock<mutex> lock(m_mutex);
		    m_queue.push(t);
        }
		m_cv.notify_one();
	}

	template<typename S>
    size_t fill(S state, std::function<T(S&)> generator, std::function<bool(const S&)> is_finish){
        using namespace std;

		int count = 0;
        {
		    unique_lock<mutex> lock(m_mutex);
		    while(!is_finish(state))
            {
                m_queue.push(generator(state));
				count++;
            }            
        }

		m_cv.notify_one();
		return count;
    }

	void notify_stopping()
	{
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_stopping.store(true, std::memory_order_seq_cst);
		}

		m_cv.notify_all();
	}

private:
	std::queue<T> m_queue;
	std::atomic_bool m_stopping = false;
	std::mutex m_mutex;
	std::condition_variable m_cv;
};

}
#endif