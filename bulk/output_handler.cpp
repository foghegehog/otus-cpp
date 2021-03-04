#include <iostream>

#include "output_handler.h"

using namespace std;

OutputHandler::OutputHandler(shared_ptr<ControlUnit> controlUnit, shared_ptr<Accumulator> accumulator)
    :m_control_unit(controlUnit), m_accumulator(accumulator)
{
}

void OutputHandler::ExecuteCommand(const ExecutableCommand& command)
{
    UNUSED(command);
    //cout << m_control_unit->GetState() << endl;
    OutputBulkIfReady();
}

void OutputHandler::HandleControlFlow(const ControlCommand& command)
{
    UNUSED(command);
    //cout << m_control_unit->GetState() << endl;
    OutputBulkIfReady();
}

void OutputHandler::OutputBulkIfReady() const
{
    if ((m_control_unit->GetState() == ControlUnit::BulkReady)
        || (m_control_unit->GetState() == ControlUnit::ProcessUnfinished))
    {
        auto bulk = m_accumulator->GetBulk();
        string separator = "";
       
        cout << GREEN << "bulk: ";
        for(const auto& command: bulk)
        {
            cout << separator << command.Text;
            separator = ", ";
        }
        cout << RESET << endl;
    }
}