#include <curl/curl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define trace(X) do { } while(0)

int dummy_glue(void) {
  return 0;
}

int dummy_valueLength(void) {
  trace(__PRETTY_FUNCTION__);
  return dummy_glue();
}

int dummy_valueCall(void) {
  trace(__PRETTY_FUNCTION__);
  return dummy_glue();
}

int dummy_valueIndex(void) {
  trace(__PRETTY_FUNCTION__);
  return dummy_glue();
}

#define NAN_HEAD 0x7FF80000
#define TYPE_FUNCTION 3
#define TYPE_STRING 1
#define TYPE_SYMBOL 2

int64_t dummy_valueGet(uint8_t *const heap, int32_t v_addr, int32_t p_ptr,
                       int32_t p_len) {
  int i = 0;
  uint32_t value_idx;
  memcpy(&value_idx, &heap[v_addr], 4);
  uint32_t retval_v = NAN_HEAD | TYPE_FUNCTION;
  if (heap[p_ptr + 0] == 'O' && heap[p_ptr + 1] == '_') {
    retval_v = 42;
  }
  return ((uint64_t)retval_v) << 32;
}

int dummy_valueNew(void) {
  trace(__PRETTY_FUNCTION__);
  return dummy_glue();
}

int dummy_valueSet(void) {
  trace(__PRETTY_FUNCTION__);
  return dummy_glue();
}

int dummy_valueSetIndex(void) {
  trace(__PRETTY_FUNCTION__);
  return dummy_glue();
}

int dummy_stringVal(void) {
  trace(__PRETTY_FUNCTION__);
  return dummy_glue();
}

int dummy_valuePrepareString(void) {
  trace(__PRETTY_FUNCTION__);
  return dummy_glue();
}

int dummy_valueLoadString(void) {
  trace(__PRETTY_FUNCTION__);
  return dummy_glue();
}

typedef struct dynbuf {
  char *ptr;
  size_t len;
  size_t cursor;
} dynbuf;

static size_t writefunc(void *ptr, size_t size, size_t nmemb, dynbuf *db) {
  size_t new_len;

  new_len = db->len + size * nmemb;
  if (new_len < db->len) {
    abort();
  }
  db->ptr = (char *)realloc(db->ptr, new_len + 1);
  if (db->ptr == NULL) {
    abort();
  }
  memcpy(db->ptr + db->len, ptr, size * nmemb);
  db->ptr[new_len] = 0;
  db->len = new_len;

  return size * nmemb;
}

int64_t http_get(uint8_t *const heap, const int32_t uri_ptr,
                 const int32_t uri_len) {
  CURL *curl;
  CURLcode res;
  dynbuf *db;
  char *uri;

  curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();
  db = calloc(1, sizeof *db);
  db->ptr = malloc(1);
  uri = malloc(uri_len + 1);
  memcpy(uri, &heap[uri_ptr], uri_len);
  uri[uri_len] = 0;
  curl_easy_setopt(curl, CURLOPT_URL, uri);
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, db);
  res = curl_easy_perform(curl);
  free(uri);
  curl_easy_cleanup(curl);
  curl_global_cleanup();
  if (res != CURLE_OK) {
    free(db->ptr);
    free(db);
    return -1;
  }
  return (int64_t)(uintptr_t)db;
};

int64_t http_body_chunk(uint8_t *const heap, int64_t handle,
                        const int32_t data_ptr, int32_t data_len) {
  dynbuf *db;
  size_t left;

  memset(&heap[data_ptr], 0, data_len);
  db = (dynbuf *)(uintptr_t)handle;
  left = db->len - db->cursor;
  if (left <= 0) {
    free(db->ptr);
    db->ptr = NULL;
    free(db);
    return 0;
  }
  if (data_len > left) {
    data_len = left;
  }
  memcpy(&heap[data_ptr], &db->ptr[db->cursor], data_len);
  db->cursor += data_len;

  return (int64_t)data_len;
}
