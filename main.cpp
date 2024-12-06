#include <cstdio>
#include <cstdint>

#include <frt.h>

#include "uart.h"

namespace
{

	class ExampleTask final :
		public frt::Task<ExampleTask>
	{
	public:
		bool run()
		{
			msleep(1000);
			std::puts("Example FreeRTOS Task");

			return true;
		}
	};

	class BlinkTask final :
		public frt::Task<BlinkTask>
	{
	public:
		BlinkTask()
		{
			DDRB |= (1 << DDB5);
		}

		bool run()
		{
			PORTB ^= (1 << PB5);
			msleep(100);

			return true;
		}
	};

	ExampleTask example_task;
	BlinkTask blink_task;

}

int main()
{
	uart_init();

    std::puts("Example FreeRTOS Project");

	example_task.start(1);
	blink_task.start(2);

    /* Start the scheduler. */
    vTaskStartScheduler();

    for (;;);

    return 0;
}

volatile std::uint8_t counter = 0;

void vApplicationIdleHook()
{
	static bool stopped = false;
	static std::uint8_t seconds = 0;

	if (counter > 62) {
		counter = 0;
		++seconds;
	}

	if (!stopped && seconds == 10) {
		std::printf("ExampleTask used %u bytes of stack this time.\n", example_task.getUsedStackSize());
		std::printf("BlinkTask used %u bytes of stack this time.\n", blink_task.getUsedStackSize());

		std::puts("Stopping...");
		example_task.stopFromIdleTask();
		std::puts("Stopped.");

		stopped = true;
	}
}

void vApplicationTickHook()
{
	++counter;
}

#if (configCHECK_FOR_STACK_OVERFLOW > 0)
void vApplicationStackOverflowHook(TaskHandle_t xTask, char* pcTaskName)
{
	for (;;) {
		std::printf("'%s' stack overflow!\n", pcTaskName);
	}
}
#endif
