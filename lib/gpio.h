/*============================================================================

  gpio.h

  Functions to control a specific GPIO pin.

  Copyright (c)1990-2020 Kevin Boone. Distributed under the terms of the
  GNU Public Licence, v3.0

  ==========================================================================*/
#ifndef __GPIO_H__
#define __GPIO_H__

// GPIO struct
typedef struct GPIO {
    int pin;
    int value_fd;
} GPIO;

void gpio_err_msg(const char *filename, char **error);

/** Initialize the GPIO object with pin number.
    Note that this method only stores values,
    and will always succeed. */
GPIO *gpio_create(int pin);

/** Clean up the object. This method implicitly calls _terminate(). */
void gpio_destroy(GPIO *self);

/** Initialize the object. This opens a file handles for the
    sysfs file for the GPIO pin. Consequently, the method
    can fail. If it does, and *error is not NULL, then it is written with
    and error message that the caller should free. If this method
    succeeds, _terminate() should be called in due course to clean up. */
_Bool gpio_init(GPIO *self, char **error);

/** Clean up. In principle, this operation can fail, as it involves sysfs
    operations. But what can we do if this happens? Probably nothing, so no
    errors are reported. */
void gpio_terminate(GPIO *self);

/** Set this pin HIGH or LOW. */
void gpio_set(GPIO *self, _Bool val);

#endif
