/****************************************************************************
 * drivers/assertmode/assertmode.c
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

#include <sys/types.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/assertmode.h>

/****************************************************************************
 * Global Assert Behavior Variable (Always defined)
 ****************************************************************************/

/* Default initialization based on config */
#if CONFIG_BOARD_ASSERT_AUTORESET
volatile int g_assert_behavior = ASSERT_BEHAVIOR_AUTORESET;
#elif CONFIG_BOARD_ASSERT_SYSTEM_HALT
volatile int g_assert_behavior = ASSERT_BEHAVIOR_HALT;
#else
volatile int g_assert_behavior = ASSERT_BEHAVIOR_NOT_SELECTED;
#endif

#ifdef CONFIG_ENABLE_ASSERTMODE

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int assertmode_open(FAR struct file *filep);
static int assertmode_close(FAR struct file *filep);
static ssize_t assertmode_read(FAR struct file *filep,
                               FAR char *buffer, size_t buflen);
static ssize_t assertmode_write(FAR struct file *filep,
                                FAR const char *buffer, size_t buflen);
static int assertmode_ioctl(FAR struct file *filep,
                             int cmd, unsigned long arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct file_operations assertmode_fops = {
	assertmode_open,      /* open */
	assertmode_close,     /* close */
	assertmode_read,      /* read */
	assertmode_write,     /* write */
	NULL,                 /* seek */
	assertmode_ioctl,     /* ioctl */
	NULL                  /* poll */
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int assertmode_open(FAR struct file *filep)
{
	return OK;
}

static int assertmode_close(FAR struct file *filep)
{
	return OK;
}

static ssize_t assertmode_read(FAR struct file *filep,
                               FAR char *buffer, size_t buflen)
{
	return 0;
}

static ssize_t assertmode_write(FAR struct file *filep,
                                FAR const char *buffer, size_t buflen)
{
	return buflen;
}

/****************************************************************************
 * Name: assertmode_ioctl
 *
 * Description:
 *   Handle ioctl commands for the assertmode driver.
 *
 *   ASSERTMODEIOC_GET_MODE - Get current assert behavior mode
 *     arg: pointer to struct assertmode_info_s
 *
 *   ASSERTMODEIOC_SET_MODE - Set assert behavior mode
 *     arg: pointer to struct assertmode_info_s with mode field set
 *
 ****************************************************************************/

static int assertmode_ioctl(FAR struct file *filep,
                             int cmd, unsigned long arg)
{
	int ret = OK;
	FAR struct assertmode_info_s *info;

	switch (cmd) {
	case ASSERTMODEIOC_GET_MODE:
		info = (FAR struct assertmode_info_s *)arg;
		if (!info) {
			ret = -EINVAL;
		} else {
			info->mode = assert_behavior_get();
		}
		break;

	case ASSERTMODEIOC_SET_MODE:
		info = (FAR struct assertmode_info_s *)arg;
		if (!info) {
			ret = -EINVAL;
		} else {
			ret = assert_behavior_set(info->mode);
		}
		break;

	default:
		ret = -ENOTTY;
		break;
	}

	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: assertmode_register
 *
 * Description:
 *   Register the assertmode character driver at /dev/assertmode.
 *
 ****************************************************************************/

void assertmode_register(void)
{
	int ret;

	ret = register_driver(ASSERTMODE_DRVPATH, &assertmode_fops, 0666, NULL);
	if (ret < 0) {
		lldbg("Failed to register assertmode driver: %d\n", ret);
	}
}

#endif /* CONFIG_ENABLE_ASSERTMODE */
