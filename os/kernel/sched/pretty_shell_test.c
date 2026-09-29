/***********************************************************
 * os/kernel/sched/pretty_shell_test.c
 * Pretty Shell Test - Kernel Print Suppression Verification
 ***********************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <syslog.h>
#include <tinyara/wqueue.h>

/* Test state */
static int g_pretty_test_running = 0;
static int g_pretty_test_count = 0;
static struct work_s g_pretty_test_work;

/* Work handler - runs in high-priority worker thread */
static void pretty_shell_test_worker(FAR void *arg)
{
	if (!g_pretty_test_running) return;
	
	/* Print kernel message */
	syslog(LOG_INFO, "[KERNEL] noisy message #%d (press Ctrl-PP to suppress)\n", 
	       g_pretty_test_count++);
	
	/* Re-schedule every 500ms (100Hz tick = 50 ticks) */
	if (g_pretty_test_running) {
		work_queue(HPWORK, &g_pretty_test_work, pretty_shell_test_worker, NULL, 300); /* 300 ticks = 3 seconds */
	}
}

void pretty_shell_kernel_test_start(void)
{
	if (g_pretty_test_running) {
		syslog(LOG_INFO, "[KERNEL-TEST] Already running\n");
		return;
	}
	
	g_pretty_test_running = 1;
	g_pretty_test_count = 0;
	
	syslog(LOG_INFO, "[KERNEL-TEST] Started (Ctrl-W to stop)\n");
	
	/* Start periodic work - no thread creation, uses existing HPWORK queue */
	work_queue(HPWORK, &g_pretty_test_work, pretty_shell_test_worker, NULL, 0);
}

void pretty_shell_kernel_test_stop(void)
{
	g_pretty_test_running = 0;
	syslog(LOG_INFO, "[KERNEL-TEST] Stopping...\n");
}
