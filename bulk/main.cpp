#include "accumulate_handler.h"
#include "control_unit_handler.h"
#include "handlers_chain.h"
#include "logging_handler.h"
#include "output_handler.h"
#include "processing_handler.h"

#include <iostream>

using namespace std;

int main (int argc, char ** argv)
{
    using namespace std;
    
    if (argc != 2)
    {
        cout << "Bulk size N should be specified as command-line argument" << endl;
        return 0;
    }

    int N = stoi(argv[1]);

    auto accumulator = make_shared<Accumulator>(); 
    auto control_unit = make_shared<ControlUnit>(N);

    auto chain = HandlersChain(make_shared<ProcessingHandler>(control_unit, accumulator));
    chain.AddFront(make_shared<LoggingHandler>(control_unit, accumulator));
    chain.AddFront(make_shared<OutputHandler>(control_unit, accumulator));
    chain.AddFront(make_shared<ControlUnitHandler>(control_unit));
    chain.AddFront(make_shared<AccumulateHandler>(accumulator));

    do
    {
        string line = "";
        if (getline(cin, line))
        {
            chain.PassThrough(line);
            if (line == "EOF")
            {
                break;
            }
        }
        else
        {
            chain.PassThrough("EOF");
            break;
        }
    } while (true);

    return 0;
}
