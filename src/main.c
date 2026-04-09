/**
 * @file main.c
 * @brief Application entry point demonstrating interrupt-driven GPIO handling.
 *
 * Configures:
 * - GPIO for LED output and button input
 * - EXTI interrupt for button press
 *
 * Behavior:
 * - Button press triggers an interrupt
 * - ISR clears the interrupt flag and toggles the LED
 *
 * Demonstrates event-driven design without polling.
 * 
 * Written by Edwin J Martinez
 */

#include "driver.h"

//* Pins
#define LED_PIN             0x00U
#define BTN_PIN             0x01U // A1->PA1 EXTI1


//* ISR Function
void Button_Handler(void)
{
    // Clear the pending bit by writing 1 to it. 
    // Note: this register is designed with W1C(write-1-to-clear) meaning it is not a logical operation, it is a command operation
    // so it is ok to use the equal sign "EXTI_PR =" as this will not overwrite the other bits as you would think.
    // Whatever position your mask pushes 1 to using "BIT" is the bit that will be cleared, so long as that bit is in fact a 1 as well:
    //  EXTI_PR = 1010 // our register before operation - we want to clear bit 1
    //  MASK    = 0010 // our mask - shifted to bit 1
    //  EXTI_PR = 1000 // our register after setting with the equal sign
    EXTI_PR = BIT(EXTI1);

    // Handle the LED via gpio driver function - this will not work as expected most of the time due to no debouncer.
    toggle_pin(GPIOA_REGS, LED_PIN);
}


void init()
{
    // Enable the clock for Port A
    enable_clock(&RCC_AHB1ENR, GPIOAEN);

    // Enable clock for System Configuration Controller - will be used for interrupts to connect the subs-sytems GPIO pin to EXTI(External Interrupt)
    enable_clock(&RCC_APB2ENR, SYSCFGEN);

    // Setup Interrupt registers - see function definition
    setup_gpio_interrupt(GPIO_PA, EXTI1, EXTI_UNMASKED, TRIG_FALLING);

    // Set LED pin, MODER, to Output Mode
    set_gpio_mode(GPIOA_REGS, LED_PIN, GPIOMODE_OUTPUT);

    // Set Button Pin, MODER, to Input Mode
    set_gpio_mode(GPIOA_REGS, BTN_PIN, GPIOMODE_INPUT);

    // Set Button Pin, PUPDR, to Pull-Up Resistor
    set_pupd_mode(GPIOA_REGS, BTN_PIN, PULL_UP);

    // Clear LED pin by default
    clear_pin_by_bsrr(GPIOA_REGS, LED_PIN);

}

int main()
{
    init();

    while (1){} // Notice, we do not need to poll the button press. This is handled in the ISR, Button_Handler, above.
    
}
