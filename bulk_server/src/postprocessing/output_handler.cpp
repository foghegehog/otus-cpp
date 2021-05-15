#include "../../include/postprocessing/bulk_writer.h"
#include "../../include/handlers/output_handler.h"

#include <iostream>

namespace postprocessing{

void OutputHandler::Handle(std::shared_ptr<ProcessedBulk> bulk) 
{
    using namespace std;
       
    cout << GREEN;
    BulkWriter bulk_writer(cout);
    bulk_writer.WriteBulk(bulk->m_commands);
    cout << RESET << endl;
}

}