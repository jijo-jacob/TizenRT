/****************************************************************************
 * Copyright 2026 Samsung Electronics All Rights Reserved.
 * Licensed under the Apache License, Version 2.0 (the "License");
 ****************************************************************************/
/// @file   tash_pretty.c
/// @brief  Pretty Shell (Ctrl-P) implementation for TASH

#include <tinyara/config.h>

#ifdef CONFIG_PRETTY_SHELL

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>
#include "tash_internal.h"
#include <tinyara/fs/ioctl.h>
#include <tinyara/pretty_shell.h>

/* Pretty Shell state variables */
static enum tash_pretty_mode g_pretty_mode = TASH_PRETTY_PERMIT_ALL;
static unsigned int g_pretty_ms = 0;
static unsigned char g_pretty_en = 1;
static int g_console_fd = -1;

/* Keyboard shortcuts */
#define PRETTY_CHAR_START_TEST  0x11  /* Ctrl-Q: Start kernel test */
#define PRETTY_CHAR_STOP_TEST   0x17  /* Ctrl-W: Stop kernel test */

void tash_pretty_init(void)
{
	if (g_console_fd < 0) {
		g_console_fd = open("/dev/ttyS0", O_RDONLY);
		if (g_console_fd < 0) {
			g_console_fd = open("/dev/console", O_RDONLY);
		}
	}
	g_pretty_mode = TASH_PRETTY_PERMIT_ALL;
	g_pretty_ms = 0;
	g_pretty_en = 1;
}

static unsigned int tash_pretty_get_ms(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (unsigned int)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int tash_pretty_handle_char(char c)
{
	if (!g_pretty_en) {
		return 0;
	}

	/* Check for Ctrl-P (Pretty Shell toggle) */
	if (c == PRETTY_CHAR) {
		unsigned int now = tash_pretty_get_ms();

		printf("\n============ tty_pretty_mode ============\n");

		if (g_pretty_mode == TASH_PRETTY_PERMIT_ALL) {
			/* First press: PERMIT_ALL -> PERMIT_PRINTK */
			g_pretty_ms = now;
			g_pretty_mode = TASH_PRETTY_PERMIT_PRINTK;
			if (g_console_fd >= 0) {
				int mode = 1;
				ioctl(g_console_fd, PSIOC_SETMODE, (unsigned long)mode);
			}
			printf("TTY_PRETTY : Disable USER PRINT\n");
			printf("==========================================\n\n");
			
		} else if (g_pretty_mode == TASH_PRETTY_PERMIT_PRINTK &&
		           now - g_pretty_ms <= PRETTY_PRESS_MS) {
			/* Double press within 500ms: PERMIT_PRINTK -> PERMIT_NONE */
			g_pretty_ms = 0;
			g_pretty_mode = TASH_PRETTY_PERMIT_NONE;
			if (g_console_fd >= 0) {
				int mode = 2;
				ioctl(g_console_fd, PSIOC_SETMODE, (unsigned long)mode);
			}
			printf("TTY_PRETTY : Disable ALL PRINT\n");
			printf("==========================================\n\n");
			
		} else {
			/* Single press or timeout: re-enable all */
			g_pretty_ms = 0;
			g_pretty_mode = TASH_PRETTY_PERMIT_ALL;
			if (g_console_fd >= 0) {
				int mode = 0;
				ioctl(g_console_fd, PSIOC_SETMODE, (unsigned long)mode);
			}
			printf("TTY_PRETTY : Enable ALL PRINT\n");
			printf("==========================================\n\n");
		}
		return 1;
	}

	/* Check for Ctrl-Q (Start kernel test via ioctl) */
	if (c == PRETTY_CHAR_START_TEST) {
		if (g_console_fd >= 0) {
			ioctl(g_console_fd, PSIOC_START_TEST, 0);
		}
		printf("\n[KERNEL-TEST] Started (Ctrl-W to stop)\n\n");
		return 1;
	}

	/* Check for Ctrl-W (Stop kernel test via ioctl) */
	if (c == PRETTY_CHAR_STOP_TEST) {
		if (g_console_fd >= 0) {
			ioctl(g_console_fd, PSIOC_STOP_TEST, 0);
		}
		printf("\n[KERNEL-TEST] Stopped\n\n");
		return 1;
	}

	/* Check for Ctrl-F (Force disable pretty mode) */
	if (c == PRETTY_CHAR_DISABLE) {
		g_pretty_ms = 0;
		g_pretty_mode = TASH_PRETTY_PERMIT_ALL;
		if (g_console_fd >= 0) {
			int mode = 0;
			ioctl(g_console_fd, PSIOC_SETMODE, (unsigned long)mode);
		}
		printf("\n============ tty_pretty_mode ============\n");
		printf("TTY_PRETTY : Enable ALL PRINT\n");
		printf("==========================================\n\n");
		return 1;
	}

	return 0;
}

int tash_pretty_get_mode(void)
{
	return (int)g_pretty_mode;
}

void tash_pretty_reset(void)
{
	g_pretty_mode = TASH_PRETTY_PERMIT_ALL;
	g_pretty_ms = 0;
}

#endif /* CONFIG_PRETTY_SHELL */
