#ifndef OUTPUT_HANDLER_H
#define OUTPUT_HANDLER_H

#include "postprocessing_handler.h"

// Using green color to differentiate console output from input
#define GREEN   "\033[32m"
#define RESET   "\033[0m"

namespace postprocessing{

/*!
 * Outputs ready bulk to console. 
 */
class OutputHandler: public PostprocessingHandler
{
public:
    void Handle(const ProcessedBulk& bulk) override;

};

}

#endif