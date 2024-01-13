/*==========================================================================

    gpiopin.c

    A "class" for setting values of specific GPIO pins.

    Copyright (c)2020 Kevin Boone, GPL v3.0

============================================================================*/
#define _GNU_SOURCE
#include "gpiopin.h"
#include "defs.h"
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct _GPIOPin {
    int pin;
    int value_fd;
};

/*============================================================================
  gpiopin_create
============================================================================*/
GPIOPin *gpiopin_create(int pin) {
    GPIOPin *self = malloc(sizeof(GPIOPin));
    memset(self, 0, sizeof(GPIOPin));
    self->pin = pin;
    self->value_fd = -1;
    return self;
}

/*============================================================================
  gpiopin_write_to_file
============================================================================*/
static _Bool
gpiopin_write_to_file(const char *filename, const char *text, char **error) {
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
}

/*============================================================================
  gpiopin_destroy
============================================================================*/
void gpiopin_destroy(GPIOPin *self) {
    if (self) {
        gpiopin_uninit(self);
        free(self);
    }
}

/*============================================================================
  gpiopin_init
============================================================================*/
_Bool gpiopin_init(GPIOPin *self, char **error) {
    assert(self != NULL);
    char s[50];
    snprintf(s, sizeof(s), "%d", self->pin);
    _Bool ret = gpiopin_write_to_file("/sys/class/gpio/export", s, error);
    if (ret) {
        char s[50];
        snprintf(s, sizeof(s), "/sys/class/gpio/gpio%d/direction", self->pin);
        gpiopin_write_to_file(s, "out", NULL); // TODO: add input capability :)
        snprintf(s, sizeof(s), "/sys/class/gpio/gpio%d/value", self->pin);
        self->value_fd = open(s, O_WRONLY);
        if (self->value_fd >= 0) {
            ret = 1;
        } else {
            if (error)
                asprintf(error,
                         "Can't open %s for writing: %s",
                         s,
                         strerror(errno));
            ret = 0;
        }
    }
    return ret;
}

/*============================================================================
  gpiopin_uninit
============================================================================*/
void gpiopin_uninit(GPIOPin *self) {
    assert(self != NULL);
    if (self->value_fd >= 0)
        close(self->value_fd);
    self->value_fd = -1;
    char s[50];
    snprintf(s, sizeof(s), "%d", self->pin);
    gpiopin_write_to_file("/sys/class/gpio/unexport", s, NULL);
}

/*============================================================================
  gpiopin_set
============================================================================*/
void gpiopin_set(GPIOPin *self, _Bool val) {
    assert(self != NULL);
    assert(self->value_fd >= 0);
    char c = val ? '1' : '0';
    write(self->value_fd, &c, 1);
}
