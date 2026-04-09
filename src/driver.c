/**
 * @file driver.c
 * @brief Implementation of low-level GPIO and EXTI interrupt driver functions.
 *
 * Contains register-level functions for:
 * - Enabling peripheral clocks
 * - Configuring GPIO modes and pull settings
 * - Configuring EXTI interrupts (routing, masking, trigger, NVIC)
 * - Basic GPIO operations (set, clear, toggle, read)
 * 
 *  Written by Edwin J Martinez
 */

#include "driver.h"
#include "hardware.h"


void enable_clock(volatile uint32_t* reg, uint32_t port)
{
    *reg |= BIT(port);
    (void)*reg;
}

void setup_gpio_interrupt(GPIOPort mode, EXTIline line, EXTImask mask, InterruptTriggerMode trig)
{
    // Map GPIO PA to EXTI
    SYSCFG_EXTICR1 &= ~(FOUR_BIT_WIDTH << SHIFT_BY_FOUR(line));
    SYSCFG_EXTICR1 |= ((mode & FOUR_BIT_WIDTH) << SHIFT_BY_FOUR(line));

    // Set mask for EXTI - Modes: Masked = 0, Unmasked = 1
    switch (mask)
    {
    case EXTI_UNMASKED:
        EXTI_IMR |= BIT(line);
        break;
    
    case EXTI_MASKED:
        EXTI_IMR |= BIT(line);
        break;
    }

    switch (trig)
    {
    case TRIG_RISING:
        // Enable Rising Trigger
        EXTI_RTSR |= BIT(line);
        // Clear Falling Trigger
        EXTI_FTSR &= ~BIT(line);
        break;
    case TRIG_FALLING:
        // Enable Falling Trigger
        EXTI_FTSR |= BIT(line);
        // Clear Rising Trigger
        EXTI_RTSR &= ~BIT(line);
        break;
    case TRIG_BOTH:
        // Enable Falling Trigger
        EXTI_FTSR |= BIT(line);
        // Enable Rising Trigger
        EXTI_RTSR |= BIT(line);
        break;
    }

    // Enable NVIC-IRQ(position) - note, the chart is found in RM0390 page-236
    switch (line)
    {
    case EXTI0:
        NVIC_ISER0 |= BIT(6);
        break;
    case EXTI1:
        NVIC_ISER0 |= BIT(7);
        break;
    case EXTI2:
        NVIC_ISER0 |= BIT(8);
        break;
    case EXTI3:
        NVIC_ISER0 |= BIT(9);
        break;
    case EXTI4:
        NVIC_ISER0 |= BIT(10);
        break;
    }
    
}

void set_gpio_mode(GPIORegisters* port, uint32_t pin, GPIOMode mode)
{
    port->MODER &= ~(TWO_BIT_WIDTH << SHIFT_BY_TWO(pin));
    port->MODER |= ((mode & TWO_BIT_WIDTH) << SHIFT_BY_TWO(pin));
}

void set_pupd_mode(GPIORegisters* port, uint32_t pin, PUPDMode mode)
{
    port->PUPDR &= ~(TWO_BIT_WIDTH << SHIFT_BY_TWO(pin));
    port->PUPDR |= ((mode & TWO_BIT_WIDTH) << SHIFT_BY_TWO(pin));
}

void set_pin(GPIORegisters* port, uint32_t pin)
{
    port->ODR |= BIT(pin);
}

void set_pin_by_bsrr(GPIORegisters* port, uint32_t pin)
{
    port->BSRR = BIT(pin);
}

void clear_pin(GPIORegisters* port, uint32_t pin)
{
    port->ODR &= ~BIT(pin);
}

void clear_pin_by_bsrr(GPIORegisters* port, uint32_t pin)
{
    port->BSRR = BIT(pin + 16U);
}

void toggle_pin(GPIORegisters* port, uint32_t pin)
{
    port->ODR ^= BIT(pin);
}

uint8_t read_pin(GPIORegisters* port, uint32_t pin)
{
    return (port->IDR & BIT(pin)) ? 1U : 0U;
}

bool read_pin_as_boolean(GPIORegisters* port, uint32_t pin)
{
    return (port->IDR & BIT(pin)) != 0U;
}
