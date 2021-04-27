#include "postprocessing.h"

namespace postprocessing{

void Postprocessing::Run()
{
    while(true)
    {
        auto bulk = m_queue->pop();
        m_handler->Handle(bulk);
    }
}

}