/****************************************************************************
 * apps/examples/hello/hello_main.c
 * Hello World Example
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <unistd.h>

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * hello_main
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int hello_main(int argc, char *argv[])
#endif
{
	printf("Hello, World!!\n");

#ifdef CONFIG_PRETTY_SHELL_TEST
	/* Pretty Shell Test - runs if CONFIG_PRETTY_SHELL_TEST=y */
	int i;

	printf("\n");
	printf("=== Pretty Shell Test ===\n");
	printf("This test prints messages every 500ms for 30 seconds.\n");
	printf("Keyboard Shortcuts:\n");
	printf("  Ctrl-P       : Toggle mode (0->1->2->0)\n");
	printf("  Ctrl-P+Ctrl-P: Direct to PERMIT_NONE (<500ms)\n");
	printf("  Ctrl-W       : Toggle kernel test\n");
	printf("Current pretty mode: check via Ctrl-P\n");
	printf("========================\n\n");

	/* Print noisy messages for 30 seconds (60 iterations * 500ms) */
	for (i = 0; i < 60; i++) {
		printf("[pretty_shell_test] noisy log message #%d\n", i);
		usleep(500 * 1000);  /* 500ms */
	}

	printf("\n[pretty_shell_test] Test complete\n");
	printf("[pretty_shell_test] Final pretty mode: check via Ctrl-P\n");
#endif

	return 0;
}
