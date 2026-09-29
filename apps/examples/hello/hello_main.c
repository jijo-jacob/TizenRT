/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * examples/hello/hello_main.c
 *
 *   Copyright (C) 2008, 2011-2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <unistd.h>

#ifdef CONFIG_PRETTY_SHELL
extern int tash_pretty_get_mode(void);
#endif


/****************************************************************************
 * hello_main
 *
 * Description:
 *   This is the entry point for the hello test application. It serves as
 *   a test case for the Pretty Shell (Ctrl-P) feature.
 *
 * Test Scenario:
 *   The hello_main application spawns a background thread that continuously
 *   prints log messages to the console, simulating a noisy application.
 *   This allows the user to test the Pretty Shell feature:
 *
 *   1. Run "hello" from TASH - the background thread starts printing messages
 *   2. Press Ctrl-P once  -> Application output is suppressed (PERMIT_PRINTK)
 *      - The flood of "Hello, World!!" messages stops
 *      - TASH prompt and commands are still visible
 *      - Kernel debug messages are still visible
 *   3. Press Ctrl-P twice (within 500ms) -> ALL output suppressed (PERMIT_NONE)
 *      - Completely clean console
 *      - Only TASH prompt and echo are visible
 *   4. Press Ctrl-P again -> All output re-enabled (PERMIT_ALL)
 *      - "Hello, World!!" messages resume
 *   5. Press Ctrl-F -> Force re-enable all output (PERMIT_ALL)
 *      - Same as step 4, but works from any state
 *
 *   The background thread runs for 30 seconds and then exits automatically.
 *
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int hello_main(int argc, char *argv[])
#endif
{
	int i;

	printf("Hello, World!!\n");

#ifdef CONFIG_PRETTY_SHELL
	printf("\n");
	printf("=== Pretty Shell Test ===\n");
	printf("This test prints messages every 500ms for 30 seconds.\n");
	printf("Press Ctrl-P to suppress this output (Pretty Shell).\n");
	printf("  Ctrl-P once  : Suppress userland output (PERMIT_PRINTK)\n");
	printf("  Ctrl-P twice : Suppress ALL output (PERMIT_NONE)\n");
	printf("  Ctrl-P again : Re-enable all output (PERMIT_ALL)\n");
	printf("  Ctrl-F       : Force re-enable all output\n");
	printf("Current pretty mode: %d\n", tash_pretty_get_mode());
	printf("=========================\n\n");

	/* Print messages in a loop to simulate noisy application output */
	for (i = 0; i < 60; i++) {
		printf("[hello] noisy log message #%d (press Ctrl-P to suppress)\n", i);
		usleep(500 * 1000);  /* 500ms */
	}

	printf("\n[hello] Pretty Shell test complete.\n");
	printf("[hello] Final pretty mode: %d\n", tash_pretty_get_mode());
#endif

	return 0;
}

