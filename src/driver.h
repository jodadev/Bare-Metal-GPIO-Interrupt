/**
 * @file driver.h
 * @brief Public API for GPIO and EXTI interrupt driver.
 *
 * Defines:
 * - Helper macros for bit manipulation
 * - Function prototypes for GPIO and interrupt configuration
 *
 * Acts as the interface between application code and low-level driver logic.
 * 
 * Written by Edwin J Martinez
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "hardware.h"

//* Helpers
#define BIT(p)              (0x01UL << (p))
#define SHIFT_BY_TWO(p)     ((p) * 0x02U)
#define SHIFT_BY_FOUR(p)    ((p) * 0x04U)
#define TWO_BIT_WIDTH       0x3UL
#define FOUR_BIT_WIDTH      0x0FUL



/// @brief Enables the peripheral clock for a given GPIO port. 
/// @param port Bit position corresponding to the GPIO port in RCC_AHB1ENR.
void enable_clock(volatile uint32_t* reg, uint32_t port);

/// @brief Configures a GPIO pin as an EXTI interrupt source with selectable mask and trigger mode, and enables the corresponding NVIC interrupt.
/// 
/// This function:
/// - Routes the selected GPIO port to the specified EXTI line via SYSCFG
/// - Masks or unmasks the EXTI line to control whether interrupts are generated
/// - Configures the interrupt trigger type (rising, falling, or both edges)
/// - Enables the appropriate NVIC IRQ based on the EXTI line number
/// 
/// @param mode GPIO port to route to the EXTI line (e.g., PA, PB, PC)
/// @param line EXTI line corresponding to the pin number (e.g., EXTI0–EXTI4)
/// @param mask Determines whether the EXTI line is masked or unmasked (EXTI_MASKED or EXTI_UNMASKED)
/// @param trig Interrupt trigger mode (TRIG_RISING, TRIG_FALLING, or TRIG_BOTH)
void setup_gpio_interrupt(GPIOPort mode, EXTIline line, EXTImask mask, InterruptTriggerMode trig);

/// @brief Configures the mode of a GPIO pin (input, output, alternate, analog).
/// @param port Pointer to GPIO register structure.
/// @param pin Pin number within the GPIO port.
/// @param mode Desired mode from GPIOMode enum.
void set_gpio_mode(GPIORegisters* port, uint32_t pin, GPIOMode mode);

/// @brief Configures the pull-up or pull-down resistor for a GPIO pin. 
/// @param port Pointer to GPIO register structure.
/// @param pin Pin number within the GPIO port.
/// @param mode Pull configuration from PUPDMode enum.
void set_pupd_mode(GPIORegisters* port, uint32_t pin, PUPDMode mode);

/// @brief Sets a GPIO pin high using ODR (read-modify-write, non-atomic).
/// @param port Pointer to GPIO register structure.
/// @param pin Pin number within the GPIO port.
void set_pin(GPIORegisters* port, uint32_t pin);

/// @brief Sets a GPIO pin high using BSRR (atomic write).
/// @param port Pointer to GPIO register structure.
/// @param pin Pin number within the GPIO port.
void set_pin_by_bsrr(GPIORegisters* port, uint32_t pin);

/// @brief Clears a GPIO pin (sets it low) using ODR (read-modify-write, non-atomic).
/// @param port Pointer to GPIO register structure.
/// @param pin Pin number within the GPIO port.
void clear_pin(GPIORegisters* port, uint32_t pin);

/// @brief Clears a GPIO pin (sets it low) using BSRR (atomic write). 
/// @param port Pointer to GPIO register structure.
/// @param pin Pin number within the GPIO port.
void clear_pin_by_bsrr(GPIORegisters* port, uint32_t pin);

/// @brief Toggles the output state of a GPIO pin using ODR.
/// @param port Pointer to GPIO register structure.
/// @param pin Pin number within the GPIO port.
void toggle_pin(GPIORegisters* port, uint32_t pin);

/// @brief Reads the current logic level of a GPIO pin as a numeric value (0 or 1). 
/// @param port Pointer to GPIO register structure.
/// @param pin Pin number within the GPIO port.
/// @return uint8_t 0 if low, 1 if high.
uint8_t read_pin(GPIORegisters* port, uint32_t pin);

/// @brief Reads the current logic level of a GPIO pin as a boolean value.
/// @param port Pointer to GPIO register structure.
/// @param pin Pin number within the GPIO port.
/// @return true if high, false if low.
bool read_pin_as_boolean(GPIORegisters* port, uint32_t pin);
