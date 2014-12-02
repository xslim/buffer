
//
// bbuffer.h
//
//

#ifndef BBUFFER
#define BBUFFER

/*
 * Default buffer size.
 */

#ifndef bbuffer_DEFAULT_SIZE
#define bbuffer_DEFAULT_SIZE 64
#endif

/*
 * Buffer struct.
 */

typedef struct {
  size_t len;  // length of data
  size_t size; // size of current buffer
  char *data;  // actual data of `len` length
} bbuffer_t;

// prototypes

bbuffer_t *
bbuffer_new();

bbuffer_t *
bbuffer_new_with_size(size_t n);

bbuffer_t *
bbuffer_new_with_bytes_length(char *bytes, size_t len);


size_t
bbuffer_size(bbuffer_t *self);

size_t
bbuffer_length(bbuffer_t *self);

void
bbuffer_free(bbuffer_t *self);

int
bbuffer_append(bbuffer_t *self, char *bytes, size_t len);

int
bbuffer_equals(bbuffer_t *self, bbuffer_t *other);

bbuffer_t *
bbuffer_new_slice(bbuffer_t *self, size_t from, ssize_t to);

void
bbuffer_clear(bbuffer_t *self);

void
bbuffer_print_hex(bbuffer_t *self);

#endif
