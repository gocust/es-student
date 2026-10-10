#include <stdio.h>
#include <stdint.h>
#include "hardware/regs/addressmap.h"
#include "pico/stdlib.h"
#include "command.h"
#include <stdlib.h>
#include "device.h"
#include "hardware/regs/addressmap.h"
#include "hardware/regs/sio.h"

#define VECTOR_TABLE 0x10000100

extern char __flash_binary_start;
extern char __flash_binary_end;
extern char __boot2_start__;
extern char __boot2_end__;
extern char __etext;
extern char __data_start__;
extern char __data_end__;
extern char __bss_start__;
extern char __bss_end__;
extern char __HeapLimit;
extern char __StackBottom;
extern char __StackTop;

extern int main(void);
static int led_pin(){return 25;}

uint32_t data_variable = 100;
uint32_t bss_variable;




static void row(const char *name, uintptr_t start, uintptr_t end)
{
    printf("%-10s 0x%08x 0x%08x %8u\n",
           name, (unsigned)start, (unsigned)end, (unsigned)(end - start));
}


void mem_info(void)
{
    printf("area       start      end        size\n");

    row("flash", XIP_BASE, XIP_BASE + PICO_FLASH_SIZE_BYTES);
    row("sram", SRAM_BASE, SRAM_BASE+0x42000);
    row("rom", ROM_BASE, ROM_BASE+0x4000);

    row("image", (uintptr_t)&__flash_binary_start, (uintptr_t)&__flash_binary_end);
    row("free", (uintptr_t)&__flash_binary_end, XIP_BASE + PICO_FLASH_SIZE_BYTES);
    row("boot2", (uintptr_t)&__boot2_start__, (uintptr_t)&__boot2_end__);
    row("text", (uintptr_t)&__boot2_end__, (uintptr_t)&__etext);

    row("data flash", (uintptr_t)&__etext, (uintptr_t)(&__etext+(&__data_end__ - &__data_start__)));
    row("data ram", (uintptr_t)&__data_start__, (uintptr_t)&__data_end__);
    row("bss", (uintptr_t)&__bss_start__, (uintptr_t)&__bss_end__);
    row("heap", (uintptr_t)&__bss_end__, (uintptr_t)&__HeapLimit);
    row("stack", (uintptr_t)&__StackBottom, (uintptr_t)&__StackTop);

    printf("\ntotal\n");

    printf("  flash image %8u = boot2 256 + text %u + data %u\n", 
        (unsigned)((uintptr_t)&__flash_binary_end - (uintptr_t)&__flash_binary_start), 
        (unsigned)((uintptr_t)&__etext - (uintptr_t)&__boot2_end__),
        (unsigned)((uintptr_t)&__data_end__ - (uintptr_t)&__data_start__)
    );

    printf("  flash free  %8u of %u\n", 
    (unsigned)(XIP_BASE + PICO_FLASH_SIZE_BYTES - (uintptr_t)&__flash_binary_end),
    PICO_FLASH_SIZE_BYTES);

    printf("  ram used    %8u = data %u + bss %u\n",
    (unsigned)(((uintptr_t)&__data_end__ - (uintptr_t)&__data_start__) + ((uintptr_t)&__bss_end__ - (uintptr_t)&__bss_start__)),
    (unsigned)((uintptr_t)&__data_end__ - (uintptr_t)&__data_start__),
    (unsigned)((uintptr_t)&__bss_end__ - (uintptr_t)&__bss_start__));

    printf("  ram free    %8u for heap and %u for stack\n",
    (unsigned)((uintptr_t)&__HeapLimit - (uintptr_t)&__bss_end__),
    (unsigned)((uintptr_t)&__StackTop - (uintptr_t)&__StackBottom));
}

void fw_info(void)
{
    data_variable++; bss_variable++;
    
    uint32_t stack_variable = 1946;
    uint32_t *heap_variable = malloc(sizeof(uint32_t));

    uint16_t *main_code = (uint16_t *)((uintptr_t)main & ~1u);
    uint16_t *fw_info_addr = (uint16_t *)((uintptr_t)fw_info & ~1u);

    if (heap_variable != NULL)
    {
        *heap_variable = 1951;
    }
    printf("object         address     value\n");

    printf("main           0x%08x 0x%04x\n", (unsigned)main_code | 1u, (uint16_t)(*main_code));
    printf("fw_info        0x%08x 0x%04x\n",(unsigned)fw_info_addr | 1u, (uint16_t)(*fw_info_addr));

    printf("commands       0x%08x\n", (unsigned)(&commands));

    for(int i = 0; i < command_count; i++){
        printf("- %-08s     0x%08x\n", commands[i].name, (unsigned)((uintptr_t)commands[i].handler & ~1u));
    }

    printf("DEVICE_PROJECT 0x%08x %-10s\n", (unsigned)(uintptr_t)&DEVICE_PROJECT, DEVICE_PROJECT);
    printf("DEVICE_BOARD   0x%08x %-10s\n", (unsigned)(uintptr_t)&DEVICE_BOARD, DEVICE_BOARD);
    
    printf("data_variable  0x%08x %u\n", (unsigned)(uintptr_t)&data_variable, data_variable);
    printf("bss_variable   0x%08x %u\n", (unsigned)(uintptr_t)&bss_variable, bss_variable);
    printf("stack_variable 0x%08x %u\n", (unsigned)(uintptr_t)&stack_variable, stack_variable);
    printf("heap_variable  0x%08x %u\n", (unsigned)(uintptr_t)heap_variable, *heap_variable);


    free(heap_variable);
}

void boot_info(void){
    const uint32_t *vectors = (const uint32_t *)VECTOR_TABLE;

    uint32_t stack_top = vectors[0];
    uint32_t reset_handler = vectors[1];
    volatile uint32_t *gpio_in = (uint32_t *)(SIO_BASE + SIO_GPIO_IN_OFFSET);
    uint32_t level = (*gpio_in >> led_pin()) & 1u;

    printf("vector table   0x%08x\n", vectors);
    printf("  stack top    0x%08x\n", vectors[0]);
    printf("  reset        0x%08x\n", vectors[1]);
    printf("  reset (even) 0x%08x\n", vectors[1] & ~1u);
    printf("gpio in        0x%08x\n", gpio_in);
    printf("  led bit      0x%01x\n", level);
    printf("  gpio_get     0x%01x\n", gpio_get(25));
}