#pragma once

// this should only be included by .c files
// within the kek engine
int window_init(void);
int window_poll(void);
int window_swap_buffers(void);
