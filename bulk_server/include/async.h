#ifndef ASYNC_H
#define ASYNC_H

#include "blocking_queue.h"
#include "context.h"

#include <cstddef>
#include <string>

namespace async{

Context * connect();

/*!
 * Reads commands from the buffer and enqueues them for asynchronous processing.
 * Performs some amount of processing.
 * Does not guarantee all enqueued commands being processed upon function return.
 * The remaining commands are guaranteed be processed later;
 */
void receive(Context * context, const char * buffer, size_t chars_count);

void disconnect(Context *);

}

#endif