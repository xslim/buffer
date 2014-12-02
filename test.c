
//
// test.c
//
// Copyright (c) 2012 TJ Holowaychuk <tj@vision-media.ca>
//

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bbuffer.h"

void
equal(char *a, char *b) {
  if (strcmp(a, b)) {
    printf("\n");
    printf("  expected: '%s'\n", a);
    printf("    actual: '%s'\n", b);
    printf("\n");
    exit(1);
  }
}

char *bbuffer_data(bbuffer_t *buf) {
  return buf->data;
}

int
bbuffer_append_str(bbuffer_t *buf, char *str) {
  return bbuffer_append(buf, str, strlen(str));
}

bbuffer_t *
bbuffer_new_with_str(char *str) {
  return bbuffer_new_with_bytes_length(str, strlen(str));
}

// Tests

void
test_bbuffer_new() {
  bbuffer_t *buf = bbuffer_new();
  assert(bbuffer_DEFAULT_SIZE == bbuffer_size(buf));
  assert(0 == bbuffer_length(buf));

}

void
test_bbuffer_new_with_size() {
  bbuffer_t *buf = bbuffer_new_with_size(1024);
  assert(1024 == bbuffer_size(buf));
  assert(0 == bbuffer_length(buf));

}

void
test_bbuffer_append_str() {
  bbuffer_t *buf = bbuffer_new();
  assert(0 == bbuffer_append_str(buf, "Hello"));
  assert(0 == bbuffer_append_str(buf, " World"));
  assert(strlen("Hello World") == bbuffer_length(buf));
  equal("Hello World", bbuffer_data(buf));
}

void
test_bbuffer_append_str__grow() {
  bbuffer_t *buf = bbuffer_new_with_size(10);
  assert(0 == bbuffer_append_str(buf, "Hello"));
  assert(0 == bbuffer_append_str(buf, " tobi"));
  assert(0 == bbuffer_append_str(buf, " was"));
  assert(0 == bbuffer_append_str(buf, " here"));

  char *str = "Hello tobi was here";
  equal(str, bbuffer_data(buf));
  assert(strlen(str) == bbuffer_size(buf));
  assert(strlen(str) == bbuffer_length(buf));
}

void
test_bbuffer_slice() {
  bbuffer_t *buf = bbuffer_new();
  bbuffer_append_str(buf, "Tobi Ferret");

  bbuffer_t *a = bbuffer_new_slice(buf, 2, 8);
  equal("Tobi Ferret", bbuffer_data(buf));

  assert(strlen("bi Fer") == bbuffer_length(a));
  equal("bi Fer", bbuffer_data(a));
}

void
test_bbuffer_slice__range_error() {
  bbuffer_t *buf = bbuffer_new_with_str("Tobi Ferret");
  bbuffer_t *a = bbuffer_new_slice(buf, 10, 2);
  assert(NULL == a);
}

void
test_bbuffer_slice__end() {
  bbuffer_t *buf = bbuffer_new_with_str("Tobi Ferret");

  bbuffer_t *a = bbuffer_new_slice(buf, 5, -1);
  equal("Tobi Ferret", bbuffer_data(buf));
  equal("Ferret", bbuffer_data(a));

  bbuffer_t *b = bbuffer_new_slice(buf, 5, -3);
  equal("Ferr", bbuffer_data(b));

  bbuffer_t *c = bbuffer_new_slice(buf, 8, -1);
  equal("ret", bbuffer_data(c));
}

void
test_bbuffer_slice__end_overflow() {
  bbuffer_t *buf = bbuffer_new_with_str("Tobi Ferret");
  bbuffer_t *a = bbuffer_new_slice(buf, 5, 1000);
  equal("Tobi Ferret", bbuffer_data(buf));
  equal("Ferret", bbuffer_data(a));
}

void
test_bbuffer_equals() {
  bbuffer_t *a = bbuffer_new_with_str("Hello");
  bbuffer_t *b = bbuffer_new_with_str("Hello");

  assert(1 == bbuffer_equals(a, b));

  bbuffer_append_str(b, " World");
  assert(0 == bbuffer_equals(a, b));
}

void
test_bbuffer_clear() {
  bbuffer_t *buf = bbuffer_new_with_str("Hello");
  assert(5 == bbuffer_length(buf));

  bbuffer_clear(buf);
  assert(0 == bbuffer_length(buf));
}

int
main(){
  test_bbuffer_new();
  test_bbuffer_new_with_size();
  test_bbuffer_append_str();
  test_bbuffer_append_str__grow();
  test_bbuffer_slice();
  test_bbuffer_slice__range_error();
  test_bbuffer_slice__end();
  test_bbuffer_slice__end_overflow();
  test_bbuffer_equals();
  test_bbuffer_clear();
  printf("\n  \e[32m\u2713 \e[90mok\e[0m\n\n");
  return 0;
}
