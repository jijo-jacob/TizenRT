/***********************************************************
 * os/kernel/pretty_shell.c
 * Pretty Shell - Core Implementation
 ***********************************************************/

#include <tinyara/config.h>

#ifdef CONFIG_PRETTY_SHELL

#include <stdio.h>
#include <time.h>
#include <tinyara/sched.h>
#include <tinyara/arch.h>
#include <tinyara/clock.h>
#include <tinyara/pretty_shell.h>

#ifdef CONFIG_PRETTY_SHELL_TEST
extern void pretty_shell_kernel_test_start(void);
extern void pretty_shell_kernel_test_stop(void);
#endif

/* Global variable for Pretty Shell state */
int g_pretty_shell_blocked = 0;  /* 0=PERMIT_ALL, 1=PERMIT_PRINTK, 2=PERMIT_NONE */

/* Keyboard handler state */
static unsigned int g_pretty_last_press_ms = 0;

/* Helper: Get current time in milliseconds */
static inline uint32_t pretty_get_ms(void)
{
	/* Use clock_systimer() which returns system ticks, convert to ms */
	/* TICK_PER_SEC = ticks per second, so ticks / TICK_PER_SEC * 1000 = ms */
	return (uint32_t)(clock_systimer() * MSEC_PER_SEC / TICK_PER_SEC);
}

/* Helper: Print status message (bypasses suppression) */
static void pretty_print_status(FAR const char *msg)
{
	/* Use low-level putc to bypass suppression */
	FAR const char *p;
	for (p = msg; *p; p++) {
		up_putc(*p);
	}
}

void pretty_shell_init(void)
{
	g_pretty_shell_blocked = 0;
	g_pretty_last_press_ms = 0;
}

void pretty_shell_reset(void)
{
	g_pretty_shell_blocked = 0;
	g_pretty_last_press_ms = 0;
}

/* Keyboard handler for Ctrl-P and Ctrl-W */
void pretty_shell_handle_char(char c)
{
	unsigned int now;
	
	if (c == PRETTY_CHAR) {
		/* Ctrl-P pressed */
		now = pretty_get_ms();
		
		pretty_print_status("\n============ tty_pretty_mode ============\n");
		
		if (g_pretty_shell_blocked == PRETTY_PERMIT_ALL) {
			/* First press: PERMIT_ALL -> PERMIT_PRINTK */
			g_pretty_last_press_ms = now;
			g_pretty_shell_blocked = PRETTY_PERMIT_PRINTK;
			pretty_print_status("TTY_PRETTY : Disable USER PRINT\n");
			pretty_print_status("==========================================\n\n");
			
		} else if (g_pretty_shell_blocked == PRETTY_PERMIT_PRINTK &&
		           now - g_pretty_last_press_ms <= PRETTY_PRESS_MS) {
			/* Double press within 500ms: PERMIT_PRINTK -> PERMIT_NONE */
			g_pretty_last_press_ms = 0;
			g_pretty_shell_blocked = PRETTY_PERMIT_NONE;
			pretty_print_status("TTY_PRETTY : Disable ALL PRINT\n");
			pretty_print_status("==========================================\n\n");
			
		} else {
			/* Single press or timeout: re-enable all */
			g_pretty_last_press_ms = 0;
			g_pretty_shell_blocked = PRETTY_PERMIT_ALL;
			pretty_print_status("TTY_PRETTY : Enable ALL PRINT\n");
			pretty_print_status("==========================================\n\n");
		}
		
	} else if (c == PRETTY_CHAR_STOP) {
		/* Ctrl-W pressed - Toggle kernel test */
		pretty_print_status("\n[DEBUG] Ctrl-W detected\n");
#ifdef CONFIG_PRETTY_SHELL_TEST
		static int g_test_running = 0;
		
		if (g_test_running) {
			pretty_shell_kernel_test_stop();
			pretty_print_status("\n[KERNEL-TEST] Stopped\n\n");
			g_test_running = 0;
		} else {
			pretty_shell_kernel_test_start();
			pretty_print_status("\n[KERNEL-TEST] Started (Ctrl-W to stop)\n\n");
			g_test_running = 1;
		}
#else
		pretty_print_status("\n[KERNEL-TEST] Not enabled (CONFIG_PRETTY_SHELL_TEST not set)\n\n");
#endif
	}
}

#endif /* CONFIG_PRETTY_SHELL */
