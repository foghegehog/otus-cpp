#include "postprocessing.h"

namespace postprocessing{

ProcessedBulk::ProcessedBulk(std::vector<handlers::ExecutableCommand>&& commands, time_t bulk_start_time)
    :m_commands(std::move(commands)),
    m_bulk_start_time(bulk_start_time),
    m_postprocessings_count(POSTPROCESSING_HANDLERS)
    {}
}