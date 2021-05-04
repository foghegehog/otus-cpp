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