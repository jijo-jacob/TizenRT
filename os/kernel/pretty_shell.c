/***********************************************************
 * os/kernel/pretty_shell.c
 * Pretty Shell - Core Implementation
 ***********************************************************/

#include <tinyara/config.h>

#ifdef CONFIG_PRETTY_SHELL

#include <stdio.h>

#ifdef CONFIG_PRETTY_SHELL_TEST
extern void pretty_shell_kernel_test_start(void);
extern void pretty_shell_kernel_test_stop(void);
#endif

/* Global variable for Pretty Shell state */
int g_pretty_shell_blocked = 0;  /* 0=PERMIT_ALL, 1=PERMIT_PRINTK, 2=PERMIT_NONE */

void pretty_shell_init(void)
{
	g_pretty_shell_blocked = 0;
	/* Auto-start removed - causes boot hang before syslog/work_queue ready */
	/* User must start test manually via TASH command: pretty_kernel_start */
}

void pretty_shell_reset(void)
{
	g_pretty_shell_blocked = 0;
}

int pretty_shell_get_mode(void)
{
	return g_pretty_shell_blocked;
}

#endif /* CONFIG_PRETTY_SHELL */
