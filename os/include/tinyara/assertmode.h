/****************************************************************************
 * include/tinyara/assertmode.h
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

#ifndef __INCLUDE_TINYARA_ASSERTMODE_H
#define __INCLUDE_TINYARA_ASSERTMODE_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Driver path */
#define ASSERTMODE_DRVPATH "/dev/assertmode"

/* Assert behavior modes */
#define ASSERT_BEHAVIOR_AUTORESET  0  /* Auto reset on assert (default) */
#define ASSERT_BEHAVIOR_HALT       1  /* Halt with watchdog kick */

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* Structure used for ASSERTMODEIOC_GET_MODE and ASSERTMODEIOC_SET_MODE */
struct assertmode_info_s {
	int mode;  /* ASSERT_BEHAVIOR_AUTORESET or ASSERT_BEHAVIOR_HALT */
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: assertmode_register
 *
 * Description:
 *   Register the /dev/assertmode driver.
 *
 ****************************************************************************/

void assertmode_register(void);

/****************************************************************************
 * Name: assert_behavior_get
 *
 * Description:
 *   Get the current runtime assert behavior mode.
 *
 * Returned Value:
 *   0 = auto reset, 1 = halt with watchdog kick
 *
 ****************************************************************************/

int assert_behavior_get(void);

/****************************************************************************
 * Name: assert_behavior_set
 *
 * Description:
 *   Set the runtime assert behavior mode.
 *
 * Input Parameters:
 *   mode - 0 for auto-reset, 1 for halt with watchdog kick
 *
 * Returned Value:
 *   OK on success, -EINVAL for invalid mode value
 *
 ****************************************************************************/

int assert_behavior_set(int mode);

#endif /* __INCLUDE_TINYARA_ASSERTMODE_H */