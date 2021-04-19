#ifndef ASYNC_H
#define ASYNC_H

#include "blocking_queue.h"
#include "context.h"

#include <cstddef>
#include <string>

namespace async{

Context * connect(size_t bulk_size);
void receive(Context * context, const char * buffer, size_t chars_count);
void disconnect(Context *);

}

#endif