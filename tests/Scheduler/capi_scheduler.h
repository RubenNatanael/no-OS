#ifndef CAPI_SCHEDULER_H
#define CAPI_SCHEDULER_H

#include "Scheduler.h"

/**
 * @brief Enqueue a function call to be executed at a specified trigger time.
 *
 * Copies the function pointer and its arguments into the scheduler queue.
 * The arguments are deep-copied based on the function type.
 *
 * @param scheduler Pointer to the scheduler instance.
 * @param trigger   Time at which the function should be executed.
 * @param fun       Function pointer to be called (cast to void(*)(void)).
 * @param ...       Variadic arguments matching the function signature.
 * @return          void
 */
void capi_enqueue(Scheduler *scheduler, uint64_t trigger, void (*fun)(void), ...);

/**
 * @brief Remove and return the next task from the scheduler queue.
 *
 * @note Not implemented yet.
 *
 * @param scheduler Pointer to the scheduler instance.
 * @return          The dequeued task.
 */
DecodedRPC capi_dequeue(Scheduler *scheduler);

#endif