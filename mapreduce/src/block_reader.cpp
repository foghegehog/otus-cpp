#include "../include/block_reader.h"

#include <sstream>

bool file_block_reader::get_next_line(std::string& line)
{
    if (!m_filestream.is_open() || m_is_eof || (m_bytes_read >= m_length))
    {
        return false;
    }

    if (!std::getline(m_filestream, line))
    {
        m_is_eof = true;
    }
    else
    {
        m_bytes_read += line.size();
    }

    return !m_is_eof && (m_bytes_read < m_length);
}