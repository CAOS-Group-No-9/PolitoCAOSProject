#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

#include <system.h> // Assicurati di includere gli header necessari per SysTick e SystemCoreClock

#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() \
    { \
        /* Configura SysTick per interrompere a una frequenza specifica */ \
        SysTick_Config(SystemCoreClock / 1000); /* Esempio: SysTick a 1 ms */ \
    }

#define portGET_RUN_TIME_COUNTER_VALUE() (SysTick->VAL)




/* Ensure these are defined in FreeRTOSConfig.h */
#ifndef configGENERATE_RUN_TIME_STATS
#error "configGENERATE_RUN_TIME_STATS should be defined in FreeRTOSConfig.h"
#endif

#ifndef configUSE_STATS_FORMATTING_FUNCTIONS
#error "configUSE_STATS_FORMATTING_FUNCTIONS should be defined in FreeRTOSConfig.h"
#endif

/* Buffer size for the runtime stats */
#define RUNTIME_STATS_BUFFER_SIZE  1024

/* Task handles */
TaskHandle_t Task1Handle, Task2Handle;

/* Task function prototypes */
void SimpleCounter();
void Task2();

int demo2(void) 
{
    /* Create tasks */
    xTaskCreate(SimpleCounter, "Task1", configMINIMAL_STACK_SIZE, NULL, 1, &Task1Handle);
    xTaskCreate(Task2, "Task2", configMINIMAL_STACK_SIZE, NULL, 1, &Task2Handle);

    /* Start the scheduler */
    vTaskStartScheduler();

    /* Infinite loop */
    for(;;);

    return 0;
}

void SimpleCounter() 
{
        uint32_t i = 0;
        while(1){
            printf("Task1 Counter: %lu\n", i);
            i++;
            vTaskDelay(pdMS_TO_TICKS(3000));
    }
}

void Task2() 
{
    char statsBuffer[RUNTIME_STATS_BUFFER_SIZE];

    for (;;) 
    {
        /* Wait for a period to gather sufficient runtime statistics */
        vTaskDelay(pdMS_TO_TICKS(1000));

        /* Generate runtime statistics */
        vTaskGetRunTimeStats(statsBuffer);

        /* Print the runtime statistics */
        printf("%s", statsBuffer);
    }
}
