/**
 * @file hardware.h
 * @brief Hardware register definitions and memory mappings for STM32.
 *
 * Contains:
 * - Base addresses for peripherals (GPIO, RCC, SYSCFG, EXTI, NVIC)
 * - Register offsets and memory-mapped register macros
 * - GPIO register structure definition
 * - Enums for GPIO modes, ports, EXTI lines, and interrupt configuration
 * 
 * Written by Edwin J Martinez
 */

#pragma once

#include <stdint.h>

//* Base Addresses
#define PERIPH_BASE         0x40000000UL
#define APB2_BASE           (PERIPH_BASE + 0x00010000UL)
#define AHB1_BASE           (PERIPH_BASE + 0x00020000UL)
#define EXTI_BASE           (APB2_BASE   + 0x00003C00UL)
#define SYSCFG_BASE         (APB2_BASE   + 0x00003800UL)
#define RCC_BASE            (AHB1_BASE   + 0x00003800UL)
#define GPIOA_BASE          (AHB1_BASE   + 0x00000000UL)
// From PM0214 Manual - M-Cortex
#define NVIC_ISER0_BASE     0xE000E100UL

//* Offsets
#define RCC_AHB1ENR_OFFSET      0x30UL
#define RCC_APB2ENR_OFFSET      0x44UL
#define SYSCFG_EXTICR1_OFFSET   0x08UL
#define EXTI_IMR_OFFSET         0x00UL
#define EXTI_RTSR_OFFSET        0x08UL
#define EXTI_FTSR_OFFSET        0x0CUL
#define EXTI_PR_OFFSET          0x14UL     

//* RCC clock bits
#define GPIOAEN                 0x00U
#define SYSCFGEN                0x0EU

//* RCC register
#define RCC_AHB1ENR         (*(volatile uint32_t*)(RCC_BASE    + RCC_AHB1ENR_OFFSET))
#define RCC_APB2ENR         (*(volatile uint32_t*)(RCC_BASE    + RCC_APB2ENR_OFFSET))
#define SYSCFG_EXTICR1      (*(volatile uint32_t*)(SYSCFG_BASE + SYSCFG_EXTICR1_OFFSET))
#define SYSCFG_EXTICR1      (*(volatile uint32_t*)(SYSCFG_BASE + SYSCFG_EXTICR1_OFFSET))
#define EXTI_IMR            (*(volatile uint32_t*)(EXTI_BASE   + EXTI_IMR_OFFSET))
#define EXTI_RTSR           (*(volatile uint32_t*)(EXTI_BASE   + EXTI_RTSR_OFFSET))
#define EXTI_FTSR           (*(volatile uint32_t*)(EXTI_BASE   + EXTI_FTSR_OFFSET))
#define EXTI_PR             (*(volatile uint32_t*)(EXTI_BASE   + EXTI_PR_OFFSET))

//* M-Cortex register 
#define NVIC_ISER0          (*(volatile uint32_t*) NVIC_ISER0_BASE)

//* GPIO Register Layout
typedef struct
{
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFR[2];
} GPIORegisters;

typedef enum
{
    GPIOMODE_INPUT              = 0x0U,
    GPIOMODE_OUTPUT             = 0x1U,
    GPIOMODE_ALTERNATE_FUNCTION = 0x2U,
    GPIOMODE_ANALOG             = 0x3U
} GPIOMode;

typedef enum
{
    NONE        = 0x0U,
    PULL_UP     = 0x1U,
    PULL_DOWN   = 0x2U,
    REVERSE     = 0x3U
} PUPDMode;

typedef enum
{
    GPIO_PA          = 0x0U,
    GPIO_PB          = 0x1U,
    GPIO_PC          = 0x2U,
    GPIO_PD          = 0x3U,
    GPIO_PE          = 0x4U,
    GPIO_PF          = 0x5U,
    GPIO_PG          = 0x6U,
    GPIO_PH          = 0x7U,
} GPIOPort;

typedef enum
{
    EXTI0       = 0x0U,
    EXTI1       = 0x1U,
    EXTI2       = 0x2U,
    EXTI3       = 0x3U,
    EXTI4       = 0x4U,
} EXTIline;

typedef enum
{
    TRIG_RISING = 0,
    TRIG_FALLING,
    TRIG_BOTH
} InterruptTriggerMode;

typedef enum
{
    EXTI_MASKED      = 0x0U,
    EXTI_UNMASKED    = 0x1U

} EXTImask;

//* Peripheral instances
#define GPIOA_REGS          ((GPIORegisters*)GPIOA_BASE)
