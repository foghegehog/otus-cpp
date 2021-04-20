#ifndef CONTEXT_H
#define CONTEXT_H

#include <string>
#include <vector>

namespace async{


class Context{
public:
    Context(size_t bulk_size)
        :m_commands(bulk_size)
    {
    }

private:
    std::vector<std::string> m_commands;


};

}

#endif