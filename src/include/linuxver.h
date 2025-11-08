/*
 * Linux-specific abstractions to gain some independence from linux kernel versions.
 * Pave over some 2.2 versus 2.4 versus 2.6 kernel differences.
 *
 * Copyright (C) 2015, Broadcom Corporation. All Rights Reserved.
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * $Id: linuxver.h 372519 2012-12-04 01:21:16Z $
 */

#ifndef _linuxver_h_
#define _linuxver_h_

#include <linux/version.h>
#include <generated/autoconf.h>

#include <linux/kconfig.h>

#include <linux/module.h>

#include <linux/slab.h>

#include <linux/types.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/string.h>
#include <linux/pci.h>
#include <linux/interrupt.h>
#include <linux/netdevice.h>
#include <linux/semaphore.h>
#undef IP_TOS
#include <asm/io.h>

#include <linux/workqueue.h>

#define DAEMONIZE(a) daemonize(a); \
	allow_signal(SIGKILL); \
	allow_signal(SIGTERM);

#define	MY_INIT_WORK(_work, _func)	INIT_WORK(_work, _func)

typedef irqreturn_t(*FN_ISR) (int irq, void *dev_id, struct pt_regs *ptregs);

#define MOD_INC_USE_COUNT
#define MOD_DEC_USE_COUNT

#include <linux/sched.h>

#include "lib80211.h"

#include <linux/ieee80211.h>

#ifdef CUSTOMER_HW4
#include <linux/kthread.h>
#endif

#ifndef __exit
#define __exit
#endif
#ifndef __devexit
#define __devexit
#endif
#ifndef __devinit
#define __devinit	__init
#endif
#ifndef __devinitdata
#define __devinitdata
#endif
#ifndef __devexit_p
#define __devexit_p(x)	x
#endif

#define pci_module_init pci_register_driver

#define WL_USE_NETDEV_OPS

#if defined(CONFIG_RFKILL)
#define WL_CONFIG_RFKILL
#else
#undef WL_CONFIG_RFKILL
#endif

#define netif_down(dev)

#define	PCI_SAVE_STATE(a, b)	pci_save_state(a)
#define	PCI_RESTORE_STATE(a, b)	pci_restore_state(a)

#ifndef SET_MODULE_OWNER
#define SET_MODULE_OWNER(dev)		do {} while (0)
#endif
#ifndef MOD_INC_USE_COUNT
#define MOD_INC_USE_COUNT			do {} while (0)
#endif
#ifndef MOD_DEC_USE_COUNT
#define MOD_DEC_USE_COUNT			do {} while (0)
#endif
#define OLD_MOD_INC_USE_COUNT		MOD_INC_USE_COUNT
#define OLD_MOD_DEC_USE_COUNT		MOD_DEC_USE_COUNT

#ifndef SET_NETDEV_DEV
#define SET_NETDEV_DEV(net, pdev)	do {} while (0)
#endif

#ifndef HAVE_FREE_NETDEV
#define free_netdev(dev)		kfree(dev)
#endif

#define DRV_SUSPEND_STATE_TYPE pm_message_t

#define CHECKSUM_HW	CHECKSUM_PARTIAL

typedef struct {
	void 	*parent;  
	struct	task_struct *p_task;
	long 	thr_pid;
	int 	prio; 
	struct	semaphore sema;
	int	terminated;
	struct	completion completed;
} tsk_ctl_t;

#define DBG_THR(x)

#define SMP_RD_BARRIER_DEPENDS(x) smp_read_barrier_depends(x)

#define PROC_START(thread_func, owner, tsk_ctl, flags) \
{ \
	sema_init(&((tsk_ctl)->sema), 0); \
	init_completion(&((tsk_ctl)->completed)); \
	(tsk_ctl)->parent = owner; \
	(tsk_ctl)->terminated = FALSE; \
	(tsk_ctl)->thr_pid = kernel_thread(thread_func, tsk_ctl, flags); \
	if ((tsk_ctl)->thr_pid > 0) \
		wait_for_completion(&((tsk_ctl)->completed)); \
	DBG_THR(("%s thr:%lx started\n", __FUNCTION__, (tsk_ctl)->thr_pid)); \
}

#ifdef USE_KTHREAD_API
#define PROC_START2(thread_func, owner, tsk_ctl, flags, name) \
{ \
	sema_init(&((tsk_ctl)->sema), 0); \
	init_completion(&((tsk_ctl)->completed)); \
	(tsk_ctl)->parent = owner; \
	(tsk_ctl)->terminated = FALSE; \
	(tsk_ctl)->p_task  = kthread_run(thread_func, tsk_ctl, (char*)name); \
	(tsk_ctl)->thr_pid = (tsk_ctl)->p_task->pid; \
	DBG_THR(("%s thr:%lx created\n", __FUNCTION__, (tsk_ctl)->thr_pid)); \
}
#endif

#define PROC_STOP(tsk_ctl) \
{ \
	(tsk_ctl)->terminated = TRUE; \
	smp_wmb(); \
	up(&((tsk_ctl)->sema));	\
	wait_for_completion(&((tsk_ctl)->completed)); \
	DBG_THR(("%s thr:%lx terminated OK\n", __FUNCTION__, (tsk_ctl)->thr_pid)); \
	(tsk_ctl)->thr_pid = -1; \
}

#define KILL_PROC(nr, sig) \
{ \
struct task_struct *tsk; \
struct pid *pid;    \
pid = find_get_pid((pid_t)nr);    \
tsk = pid_task(pid, PIDTYPE_PID);    \
if (tsk) send_sig(sig, tsk, 1); \
}

#include <linux/time.h>
#include <linux/wait.h>

#define DEV_PRIV(dev)	netdev_priv(dev)

#define WL_ISR(i, d, p)         wl_isr((i), (d))

#endif 
