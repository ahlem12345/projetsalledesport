#include "api_client.h"
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct buffer {
    char *data;
    size_t size;
};

static size_t write_cb(void *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t total = size * nmemb;
    struct buffer *buf = userdata;

    buf->data = realloc(buf->data, buf->size + total + 1);
    memcpy(buf->data + buf->size, ptr, total);
    buf->size += total;
    buf->data[buf->size] = '\0';

    return total;
}

char *appel_api_stat(int id) {
    CURL *curl = curl_easy_init();
    struct buffer buf = { malloc(1), 0 };
    char url[128];

    sprintf(url, "http://localhost:5000/stats?id=%d", id);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buf);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    return buf.data;
}

