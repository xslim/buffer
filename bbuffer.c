//
// bbuffer.c
//
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <string.h>
#include "bbuffer.h"

/*
 * Allocate a new buffer with bbuffer_DEFAULT_SIZE.
 */

bbuffer_t *
bbuffer_new() {
  return bbuffer_new_with_size(bbuffer_DEFAULT_SIZE);
}

/*
 * Allocate a new buffer with `n` bytes.
 */

bbuffer_t *
bbuffer_new_with_size(size_t n) {
  bbuffer_t *self = malloc(sizeof(bbuffer_t));
  if (!self) return NULL;
  self->size = n;
  self->len = 0;

  self->data = calloc(n, 1);

  return self;
}


/*
 * Allocate a new buffer with `bytes` and `len`.
 */

bbuffer_t *
bbuffer_new_with_bytes_length(char *bytes, size_t len) {
  bbuffer_t *self = bbuffer_new_with_size(len);
  if (!self) return NULL;
  return (bbuffer_append(self, bytes, len) == 0) ? self : NULL;
}

/*
 * Free the buffer.
 */

void
bbuffer_free(bbuffer_t *self) {
  free(self->data);
  free(self);
}

/*
 * Return buffer size.
 */

size_t
bbuffer_size(bbuffer_t *self) {
  return self->size;
}

/*
 * Return data length.
 */

size_t
bbuffer_length(bbuffer_t *self) {
  return self->len;
}

/*
 * Resize to hold `n` bytes.
 */

int
bbuffer_resize(bbuffer_t *self, size_t n) {
  //n = nearest_multiple_of(1024, n);

  if (self->size >= n) {
    return 0;
  }

  char *new_data = realloc(self->data, n);
  if (!new_data) return -1;

  self->data = new_data;
  self->size = n;
  return 0;
}

/*
 * Append `len` `bytes` to `self` and return 0 on success, -1 on failure.
 */

int
bbuffer_append(bbuffer_t *self, char *bytes, size_t len) {
  size_t needed = len + self->len;

  // resize
  int ret = bbuffer_resize(self, needed);
  if (-1 == ret) return -1;
  memcpy(self->data + self->len, bytes, len);
  self->len += len;

  return 0;
}

/*
 * Return a new buffer based on the `from..to` slice of `buf`,
 * or NULL on error.
 */

bbuffer_t *
bbuffer_new_slice(bbuffer_t *buf, size_t from, ssize_t to) {
  size_t len = buf->len;

  // bad range
  if (to < from) return NULL;

  // relative to end
  if (to < 0) to = len - ~to;

  // cap end
  if (to > len) to = len;

  size_t n = to - from;

  bbuffer_t *self = bbuffer_new_with_size(n);

  memcpy(self->data, buf->data + from, n);
  self->len = n;

  return self;
}

#define MIN(a,b) (((a)<(b))?(a):(b))

int
bbuffer_equals(bbuffer_t *self, bbuffer_t *other) {
  size_t n = MIN(self->len, other->len);
  return memcmp(self, other, n) == 0 ? 1 : 0;
}

/*
 * Clear the buffer
 */

void
bbuffer_clear(bbuffer_t *self) {
  memset(self->data, 0, self->len);
  self->len = 0;
}

/*
 * Print a hex dump of the buffer.
 */

void
bbuffer_print_hex(bbuffer_t *self) {
  size_t len = self->len;

  // hex
  for (int i = 0; i < len; ++i) {
    printf(" %02x", self->data[i]);
    if ((i + 1) % 8 == 0) printf("\n ");
  }

  printf("\n");
}
