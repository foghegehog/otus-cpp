#ifndef ASYNC_H
#define ASYNC_H

#include "blocking_queue.h"
#include "context.h"

#include <cstddef>
#include <string>

namespace async{

Context * connect(size_t bulk_size);

/*!
 * Reads commands from the buffer and enqueues them for asynchronous processing.
 * Performs some amount of processing.
 * Does not guarantee all enqueued commands being processed upon function return.
 * The remaining commands shall be processed in the background, if only disconnect wouldn't be called before.
 */
void receive(Context * context, const char * buffer, size_t chars_count);
void disconnect(Context *);

}

#endif