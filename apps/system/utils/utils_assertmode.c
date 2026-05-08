/****************************************************************************
 * apps/system/utils/utils_assertmode.c
 *
 *   Copyright (C) 2026 Samsung Electronics. All rights reserved.
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
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <tinyara/assertmode.h>

#if defined(CONFIG_ENABLE_ASSERTMODE)

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int utils_assertmode(int argc, char **args)
{
	int fd;
	int ret;
	struct assertmode_info_s info;

	fd = open(ASSERTMODE_DRVPATH, O_RDWR);
	if (fd < 0) {
		printf("ERROR: Failed to open %s, errno: %d\n",
		       ASSERTMODE_DRVPATH, get_errno());
		return ERROR;
	}

	printf("\n");

	if (argc == 1) {
		/* No argument: read and display current mode */
		ret = ioctl(fd, ASSERTMODEIOC_GET_MODE, (unsigned long)&info);
		if (ret < 0) {
			printf("ERROR: ioctl GET_MODE failed, errno: %d\n", get_errno());
			close(fd);
			return ERROR;
		}

		if (info.mode == ASSERT_BEHAVIOR_HALT) {
			printf("Current: halt (watchdog kick)\n");
		} else {
			printf("Current: auto-reset\n");
		}
	} else if (argc == 2) {
		/* Set mode */
		int mode = atoi(args[1]);

		if (mode != ASSERT_BEHAVIOR_AUTORESET && mode != ASSERT_BEHAVIOR_HALT) {
			printf("Invalid mode. Use 0 (auto-reset) or 1 (halt)\n");
			close(fd);
			return ERROR;
		}

		info.mode = mode;
		ret = ioctl(fd, ASSERTMODEIOC_SET_MODE, (unsigned long)&info);
		if (ret < 0) {
			printf("ERROR: ioctl SET_MODE failed, errno: %d\n", get_errno());
			printf("\n");
			close(fd);
			return ERROR;
		}

		printf("Assert behavior set to %s\n",
		       mode == ASSERT_BEHAVIOR_HALT ? "halt" : "auto-reset");
	}

	printf("Usage: assertmode [0|1]\n");
        printf("  assertmode (for assertmode status)\n");
        printf("  0 = auto-reset on assert (default)\n");
        printf("  1 = halt with watchdog kick on assert\n");

	close(fd);
	return OK;
}

#endif /* CONFIG_ENABLE_ASSERTMODE */
