#ifndef CONTAINER_TRACE_H
#define CONTAINER_TRACE_H

template<typename Container> 
struct container_trace
{
    container_trace(Container & container)
        :m_container(container), m_pos(container.begin()), m_end(container.end())
    {
    }

    Container & m_container;
    typename Container::iterator m_pos;
    typename Container::iterator m_end;
};

#endif