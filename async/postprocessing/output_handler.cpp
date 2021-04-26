#include "bulk_writer.h"
#include "output_handler.h"

#include <iostream>

namespace postprocessing{

void Handle(const ProcessedBulk& bulk) 
{
    using namespace std;
       
    cout << GREEN;
    BulkWriter bulk_writer(cout);
    bulk_writer.WriteBulk(bulk.m_commands);
    cout << RESET << endl;
}

}