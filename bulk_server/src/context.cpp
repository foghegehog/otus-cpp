#include <atomic>
#include <mutex>
#include <thread>

#include "../include/commands_parsing.h"
#include "../include/context.h"


namespace async{

using namespace postprocessing;
using namespace std;

thread Context::s_static_processor_thread = thread([]()
{  
    time_t bulk_time; 
    while(true)
    {
        s_static_processor->ProcessBulk(
            s_static_accumulator->WaitBulk(bulk_time),
            bulk_time);
    }
});

thread Context::s_logger_thread1 = thread(
            &Postprocessing::Run,
            Postprocessing(make_unique<LoggingHandler>("file1"), s_logging_queue));
thread Context::s_logger_thread2 = thread(
            &Postprocessing::Run,
            Postprocessing(make_unique<LoggingHandler>("file2"), s_logging_queue));

thread Context::s_output_thread = thread(
            &Postprocessing::Run, Postprocessing(make_unique<OutputHandler>(), s_output_queue));

void Context::process_command(const std::string& command)
{
    {
        std::lock_guard<std::mutex> lock(m_processing_mutex);
        m_accumulate_handler->HandleCommand(command, this->get_id());
    }
}

int Context::get_id() const
{
    return reinterpret_cast<uintptr_t>(this);
}

void Context::set_bulk_size(size_t bulk_size)
{
    s_static_accumulator->SetBulkSize(bulk_size);
}

Context::~Context()
{
    s_static_accumulator->OnDisconnection(get_id());
}

}