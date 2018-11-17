/*
 *  This file is part of the "Coroutine" project and released under the MIT License.
 *
 *  Created by Samuel Williams on 10/5/2018.
 *  Copyright, 2018, by Samuel Williams. All rights reserved.
*/

#pragma once

#include <assert.h>
#include <string.h>

#if __cplusplus
extern "C" {
#endif

#define COROUTINE __attribute__((noreturn)) void

const size_t COROUTINE_REGISTERS = 9;

typedef struct
{
	void **stack_pointer;
} CoroutineContext;

typedef COROUTINE(* CoroutineStart)(CoroutineContext *from, CoroutineContext *self, void *argument);

void coroutine_trampoline();

static inline void coroutine_initialize(
	CoroutineContext *context,
	CoroutineStart start,
	void *argument,
	void *stack_pointer,
	size_t stack_size
) {
	context->stack_pointer = (void**)stack_pointer;
	
	*--context->stack_pointer = (void*)start;
	*--context->stack_pointer = (void*)argument;
	
	*--context->stack_pointer = (void*)coroutine_trampoline;
	
	context->stack_pointer -= COROUTINE_REGISTERS;
	memset(context->stack_pointer, 0, sizeof(void*) * COROUTINE_REGISTERS);
}

CoroutineContext * coroutine_transfer(CoroutineContext * current, CoroutineContext * target);

#if __cplusplus
}
#endif
