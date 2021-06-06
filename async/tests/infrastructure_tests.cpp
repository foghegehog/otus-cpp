#include "../blocking_queue.h"
#include "../postprocessing/notifying_queue.h"
#include "../commands_parsing.h"
#include "../handlers_factory.h"
#include "../handlers/accumulator.h"
#include "../handlers/control_unit.h"
#include "../postprocessing/bulk_writer.h"

#include <gtest/gtest.h>
#include <memory>
#include <sstream>
#include <vector>

TEST(Infrastructure, HandlersChain)
{
    using namespace handlers;
    using namespace std;

    vector<ExecutableCommand> commands = { ExecutableCommand("cmd1"), ExecutableCommand("cmd2"), ExecutableCommand("cmd3") };  
    auto accumulator = std::make_shared<handlers::Accumulator>(); 
    auto control_unit = std::make_shared<handlers::ControlUnit>(commands.size());
    auto handlers = std::move(create_handlers_chain(accumulator, control_unit));

    for (const auto& cmd: commands)
    {
        handlers->PassThrough(cmd.Text);
    }

    ASSERT_EQ(accumulator->GetCommandsStoredCount(), commands.size());
    ASSERT_TRUE(control_unit->ShouldClearProcessedBulk());
}

TEST(Infrastructure, OutputFormatting)
{
    using namespace handlers;
    using namespace std;

    vector<ExecutableCommand> commands = { ExecutableCommand("cmd1"), ExecutableCommand("cmd2"), ExecutableCommand("cmd3") };  
    stringstream outstream;
    postprocessing::BulkWriter writer(outstream);
    writer.WriteBulk(commands);

    ASSERT_EQ(outstream.str(), "bulk: cmd1, cmd2, cmd3");

}

TEST(Infrastructure, ComandsParsing)
{
    std::vector<std::string> original_commands = {"h3_1", "h3_2", "{", "h3_a", "h3_b"};
    std::stringstream concatenated;
    char separator = '\0';
    for (const auto& cmd: original_commands)
    {
        (separator ? (concatenated << separator) : concatenated) << cmd;
        separator = '\n';
    }

    std::vector<std::string> parsed_commands;
    auto str = concatenated.str();
    const char * buffer = str.data();
    size_t chars_count = strlen(buffer);
    const char * buffer_end = buffer + chars_count - 1;
    separator = '\n';
    
    for(const char * pos = commands_parsing::skip_separator(buffer, buffer_end, separator);
        !commands_parsing::is_buffer_end(pos, buffer_end); )
    {
        parsed_commands.push_back(commands_parsing::extract_command(pos, buffer_end, separator));
    }

    ASSERT_EQ(original_commands, parsed_commands);
}

TEST(Infrastructure, EmptyComandsParsing)
{
    const char separator = '\n';
    const char * buffer = "\n\n\n";
    size_t chars_count = strlen(buffer);
    const char * buffer_end = buffer + chars_count - 1;
    std::vector<std::string> commands;
    for(auto pos = commands_parsing::skip_separator(buffer, buffer_end, separator);
        !commands_parsing::is_buffer_end(pos, buffer_end); )
    {
        commands.push_back(commands_parsing::extract_command(pos, buffer_end, separator));
    }

    ASSERT_TRUE(commands.empty());
}

TEST(Infrastructure, BlockingQueueFillig)
{
    blocking_queue<int> queue;
    const size_t count = 25;
    int c = 0;
    auto is_finish = [](size_t c)
    { 
        return c >= count; 
    };

    auto generator = [](int& c){
        return c++;
    };

    auto generated = queue.fill<int>(c, generator, is_finish);
    ASSERT_EQ(generated, count);
    for (size_t i = 0; i < generated; i++)
    {
        ASSERT_TRUE(queue.try_get(c));
        ASSERT_EQ(c, i);
    }
}

TEST(Infrastructure, NotifyingQueue)
{
    using namespace postprocessing;
    notifying_queue<int> queue;
    const int count = 3;
    for (auto c = 0; c < count; c++)
    {
        queue.put(c);
    }

    int value;
    for (auto c = 0; c < count; c++)
    {
        ASSERT_TRUE(queue.try_pop(value));
        ASSERT_EQ(value, c);
    }

    queue.notify_stopping();
    ASSERT_FALSE(queue.try_pop(value));
}