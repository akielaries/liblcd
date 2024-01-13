/*==========================================================================

    gpio.c

    A "class" for setting values of specific GPIO pins.

    Copyright (c)2020 Kevin Boone, GPL v3.0

============================================================================*/
#include "../lib/gpio.h"
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*============================================================================
  gpio_create
============================================================================*/
GPIO *gpio_create(int pin) {
    GPIO *self = malloc(sizeof(GPIO));
    memset(self, 0, sizeof(GPIO));
    self->pin = pin;
    self->value_fd = -1;
    return self;
}

/*============================================================================
  gpio_write_to_file
============================================================================*/
/*static _Bool
gpio_write_to_file(const char *filename, const char *text, char **error) {
    assert(filename != NULL);
    assert(text != NULL);
    _Bool ret = 0;
    FILE *f = fopen(filename, "w");
    if (f) {
        fprintf(f, text);
        fclose(f);
        ret = 1;
    } else {
        if (error)
            asprintf(error,
                     "Can't open %s for writing: %s",
                     filename,
                     strerror(errno));
        ret = 0;
    }
    return ret;
}*/

void gpio_err_msg(const char *filename, char **error) {
    // get size of error message
    size_t error_size = snprintf(NULL,
                                 0,
                                 "Can't open %s for writing: %s",
                                 filename,
                                 strerror(errno));

    // allocate memory for err msg
    *error = (char *)malloc(error_size);

    // create err msg
    snprintf(*error,
             error_size,
             "Can't open %s for writing: %s",
             filename,
             strerror(errno));
}

static _Bool
gpio_write_to_file(const char *filename, const char *text, char **error) {
    assert(filename != NULL);
    assert(text != NULL);
    _Bool ret;
    FILE *f = fopen(filename, "w");
    if (f) {
        fprintf(f, "%s", text);
        fclose(f);
        ret = 1;
    } else {
        if (error) {
            // create err message
            gpio_err_msg(filename, error);
        }
        ret = 0;
    }
    return ret;
}

/*============================================================================
  gpio_destroy
============================================================================*/
void gpio_destroy(GPIO *self) {
    if (self) {
        gpio_uninit(self);
        free(self);
    }
}

/*============================================================================
  gpio_init
============================================================================*/
_Bool gpio_init(GPIO *self, char **error) {
    assert(self != NULL);
    char s[50];
    snprintf(s, sizeof(s), "%d", self->pin);
    _Bool ret = gpio_write_to_file("/sys/class/gpio/export", s, error);
    if (ret) {
        snprintf(s, sizeof(s), "/sys/class/gpio/gpio%d/direction", self->pin);
        ret =
            gpio_write_to_file(s, "out", NULL); // TODO: add input capability :)
        if (ret) {
            snprintf(s, sizeof(s), "/sys/class/gpio/gpio%d/value", self->pin);
            self->value_fd = open(s, O_WRONLY);
            if (self->value_fd >= 0) {
                ret = 1;
            } else {
                if (error) {
                    // create err msg
                    gpio_err_msg(s, error);
                }
                ret = 0;
            }
        }
    }
    return ret;
}

/*============================================================================
  gpio_uninit
============================================================================*/
void gpio_uninit(GPIO *self) {
    assert(self != NULL);
    if (self->value_fd >= 0)
        close(self->value_fd);
    self->value_fd = -1;
    char s[50];
    snprintf(s, sizeof(s), "%d", self->pin);
    gpio_write_to_file("/sys/class/gpio/unexport", s, NULL);
}

/*============================================================================
  gpio_set
============================================================================*/
void gpio_set(GPIO *self, _Bool val) {
    assert(self != NULL);
    assert(self->value_fd >= 0);
    char c = val ? '1' : '0';
    write(self->value_fd, &c, 1);
}
