/****************************************************************************
 * os/include/tinyara/pretty_shell.h
 *
 * Pretty Shell - Output Suppression Feature
 ****************************************************************************/

#ifndef __ARCH_INCLUDE_PRETTY_SHELL_H
#define __ARCH_INCLUDE_PRETTY_SHELL_H

#ifdef CONFIG_PRETTY_SHELL

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Pretty Shell modes */
#define PRETTY_PERMIT_ALL       0  /* Allow all output */
#define PRETTY_PERMIT_PRINTK    1  /* Allow only kernel output */
#define PRETTY_PERMIT_NONE      2  /* Suppress all output */

/* Keyboard shortcuts */
#define PRETTY_CHAR             0x10  /* Ctrl-P: Toggle pretty mode */
#define PRETTY_CHAR_STOP        0x17  /* Ctrl-W: Toggle kernel test */
#define PRETTY_PRESS_MS         500   /* Double-press interval in ms */

/****************************************************************************
 * Public Data
 ****************************************************************************/

/* Global variable for Pretty Shell state */
extern int g_pretty_shell_blocked;

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

void pretty_shell_init(void);
void pretty_shell_reset(void);

/* Keyboard handler - called by serial driver */
void pretty_shell_handle_char(char c);

/* Kernel test functions - always available for ioctl */
void pretty_shell_kernel_test_start(void);
void pretty_shell_kernel_test_stop(void);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* CONFIG_PRETTY_SHELL */
#endif /* __ARCH_INCLUDE_PRETTY_SHELL_H */
