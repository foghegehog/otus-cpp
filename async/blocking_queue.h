#ifndef BLOCKING_QUEUE_H
#define BLOCKING_QUEUE_H

#include <atomic>
#include <functional>
#include <mutex>
#include <queue>
#include <tuple>

template<typename T> 
class blocking_queue {

public:

	blocking_queue() { }
	blocking_queue(const blocking_queue& other) = delete;

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

		return count;
    }
	
	void put(const T& t)
	{
		using namespace std;

		{
			unique_lock<mutex> lock(m_mutex);
			m_queue.push(t);
		}
	}

	bool try_get(T& t)
	{
		using namespace std;

		{
			unique_lock<mutex> lock(m_mutex);
			if (m_queue.size() == 0)
			{
				return false;
			}

			t = move(m_queue.front());
        	m_queue.pop();
			return true;
		}
	}


private:
	std::queue<T> m_queue;
	std::mutex m_mutex;
};
#endif