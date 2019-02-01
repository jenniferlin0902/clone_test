/* File: gpio.c
 * ------------
 *
 * Pat Hanrahn, Phil Levis, original authors
 * Julie Zelenski edited Wed Jan 24 11:41:57 PST 2018
 *
 * The basic gpio module supports set/get pin function and pin read/write.
 * See gpioextra for event detection and pull-up/pull-down.
 */


/*
   GPIO (general purpose input/output) pins can be configured in many
   ways, so there are a lot of configuration data registers for them.

   The BCM2835 supports 54 GPIO pins. Each pin can be configured to be
   in one of up to 8 states: input, output, then one of 6 alternate functions
   which is pin-specific. Consult the BCM2835 ARM peripheral manual,
   section 6.2 (pages 102-103) for details on these 6 functions. Some
   pins support 6, some support 3, some actually support none (pins
   46-54).
*/

#include "gpio.h"
#include "_gpio_private.h"
// Reminder of declaration from private: volatile struct gpio * const _gpio;

/*
 * all libpi peripherals libraries require an init
 * even if it doesn't do anything
 */
void gpio_init(void) {}

void gpio_set_function(unsigned int pin, unsigned int func)
{
    if (pin < GPIO_PIN_FIRST || pin > GPIO_PIN_LAST) {
        return;
    }
    if (func < 0 || func > 7) {
        return;
    }

    unsigned int bank = pin / 10;
    unsigned int shift = (pin % 10) * 3;

    _gpio->fsel[bank] = (_gpio->fsel[bank] & ~(7 << shift)) | ((func & 7) << shift);
}

unsigned int gpio_get_function(unsigned int pin)
{
    if (pin < GPIO_PIN_FIRST || pin > GPIO_PIN_LAST) {
        return GPIO_INVALID_REQUEST;
    }

    unsigned int bank = pin / 10;
    unsigned int shift = (pin % 10) * 3;

    return (_gpio->fsel[bank] >> shift) & 7;
}

void gpio_set_input(unsigned int pin)
{
    gpio_set_function(pin, GPIO_FUNC_INPUT);
}

void gpio_set_output(unsigned int pin)
{
    gpio_set_function(pin, GPIO_FUNC_OUTPUT);
}

void gpio_write(unsigned int pin, unsigned int val)
{
    if (pin < GPIO_PIN_FIRST || pin > GPIO_PIN_LAST) {
        return;
    }

    unsigned int bank = pin / 32;
    unsigned int shift = pin % 32;

    if (val) {
        _gpio->set[bank] = 1 << shift;
    } else {
        _gpio->clr[bank] = 1 << shift;
    }
}

unsigned int gpio_read(unsigned int pin)
{
    if (pin < GPIO_PIN_FIRST || pin > GPIO_PIN_LAST) {
        return GPIO_INVALID_REQUEST;
    }

    unsigned int bank = pin / 32;
    unsigned int shift = pin % 32;

    return (_gpio->lev[bank] & (1 << shift)) != 0;
}
