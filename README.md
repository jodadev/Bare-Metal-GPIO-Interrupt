# STM32 EXTI Interrupt Driver (Bare-Metal)

This project demonstrates interrupt-based input handling using EXTI (External Interrupts) and NVIC on an STM32 microcontroller, implemented entirely at the register level in C.  

The goal is to showcase an understanding of hardware-driven events, interrupt routing, and CPU-level interrupt handling without relying on vendor libraries such as HAL or LL.

---

## Features

- Direct register-level configuration (no HAL or LL)
- External interrupt (EXTI) configuration
- SYSCFG routing of GPIO pins to EXTI lines
- NVIC interrupt enabling using Cortex-M core registers
- Custom vector table with manually mapped interrupt handler
- Interrupt Service Routine (ISR) implementation
- Event-driven input handling (no polling)
- LED control triggered by hardware interrupt

---

## Concepts Demonstrated

- Memory-mapped peripheral access
- Interrupt-driven programming vs polling
- EXTI line configuration and masking
- Edge-triggered interrupts (falling edge)
- NVIC interrupt enable registers (`ISER`)
- Vector table structure and IRQ mapping
- Write-1-to-clear (W1C) interrupt flags
- Peripheral vs CPU-level interrupt separation
- Hardware event handling

---

## Project Structure

```text
├── linker.ld    # Linker script defining memory layout (Flash, RAM) and section placement
├── startup.s    # Startup code: vector table, reset handler, and system initialization
├── hardware.h   # Base addresses, EXTI/NVIC/RCC register definitions
├── driver.h     # Driver API, enums, macros
├── driver.c     # Driver implementation
└── main.c       # Application logic and ISR
```

---

## Example Usage

```c
int main()
{
    init();

    while (1)
    {
        // Interrupt-driven system
        // No polling required
    }
}
```

---

## Behavior

- Pressing the button triggers a hardware interrupt  
- EXTI detects the falling edge on the input pin  
- NVIC dispatches the interrupt to the CPU  
- The CPU jumps to the interrupt handler  
- The handler clears the interrupt flag  
- The LED toggles on each button press  

**Note:** The button is configured as active-low using an internal pull-up resistor.

---

## Requirements

- ARM GCC Toolchain (`arm-none-eabi-gcc`)
- Make
- OpenOCD or ST-Link (for flashing)
- STM32F446RE board (or compatible)

---

## Hardware Setup
<img src="./Simple-Buttons-LED.png"  />

- STM32 microcontroller (tested on STM32F446RE)  
- LED connected to PA0  
- Button connected to PA1  
- Internal pull-up enabled on button input 
- 220 Ohm resistor for LED 

---

## Build & Flash Instructions

```bash
make
```
This will generate the .elf and .bin files.

```bash
make flash
```
This will flash the program to your STM32 (ensure it is plugged in to your computer via USB).

```bash
make clean
```
This will remove all build files if you need a clean sweep.

---

## Interrupt Flow

```text
GPIO (PA1 button)
   |_
      SYSCFG (routes PA1 → EXTI1)
         |_
            EXTI (detects falling edge)
               |_
                  NVIC (enables IRQ7)
                     |_
                        Vector Table (maps IRQ7 → handler)
                           |_
                              ISR (clears flag + toggles LED)
```

---

## Notes

- EXTI pending flags must be cleared by writing **1** (W1C behavior)  
- NVIC registers are part of the Cortex-M core, not STM32 peripherals  
- Vector table must correctly map IRQs to handlers when using custom startup code  
- This implementation does not include debounce logic  
- Interrupts are configured for falling-edge only (button press)  

---

## Future Improvements

- Add software debounce for button input  
- Support rising and dual-edge interrupts  
- Handle grouped EXTI lines (EXTI9_5, EXTI15_10)  
- Add interrupt priority configuration  
- Extend driver for multiple GPIO ports  
- Integrate low-power wake-up using EXTI  
- Add abstraction layer for event callbacks  

---

## Purpose

This project is intended as a portfolio piece to demonstrate low-level embedded systems knowledge, including:

- register-level programming  
- interrupt handling  
- hardware-to-CPU event flow  
- vector table manipulation  
- real-time embedded system behavior  

---

## Contact

Edwin Martinez  
**Software Engineer | Embedded Systems**  

GitHub: https://github.com/jodadev  
LinkedIn: https://linkedin.com/in/jodadev  
