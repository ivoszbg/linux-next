/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_PARISC_THREAD_INFO_H
#define _ASM_PARISC_THREAD_INFO_H

#ifndef __ASSEMBLER__
#include <asm/processor.h>
#include <asm/special_insns.h>

struct thread_info {
	unsigned long flags;		/* thread_info flags (see TIF_*) */
	int preempt_count;		/* 0=premptable, <0=BUG; will also serve as bh-counter */
#ifdef CONFIG_SMP
	unsigned int cpu;
#endif
};

#define INIT_THREAD_INFO(tsk)			\
{						\
	.flags		= 0,			\
	.preempt_count	= INIT_PREEMPT_COUNT,	\
}

#endif /* !__ASSEMBLER__ */

/* thread information allocation */

#ifdef CONFIG_IRQSTACKS
#define THREAD_SIZE_ORDER	2 /* PA-RISC requires at least 16k stack */
#else
#define THREAD_SIZE_ORDER	3 /* PA-RISC requires at least 32k stack */
#endif

/* Be sure to hunt all references to this down when you change the size of
 * the kernel stack */
#define THREAD_SIZE             (PAGE_SIZE << THREAD_SIZE_ORDER)
#define THREAD_SHIFT            (PAGE_SHIFT + THREAD_SIZE_ORDER)

/*
 * thread information flags
 */
#define TIF_SYSCALL_TRACE	0	/* syscall trace active */
#define TIF_SIGPENDING		1	/* signal pending */
#define TIF_NEED_RESCHED	2	/* rescheduling necessary */
#define TIF_POLLING_NRFLAG	3	/* true if poll_idle() is polling TIF_NEED_RESCHED */
#define TIF_32BIT               4       /* 32 bit binary */
#define TIF_MEMDIE		5	/* is terminating due to OOM killer */
#define TIF_NOTIFY_SIGNAL	6	/* signal notifications exist */
#define TIF_SYSCALL_AUDIT	7	/* syscall auditing active */
#define TIF_NOTIFY_RESUME	8	/* callback before returning to user */
#define TIF_SINGLESTEP		9	/* single stepping? */
#define TIF_BLOCKSTEP		10	/* branch stepping? */
#define TIF_SECCOMP		11	/* secure computing */
#define TIF_SYSCALL_TRACEPOINT	12	/* syscall tracepoint instrumentation */
#define TIF_NONBLOCK_WARNING	13	/* warned about wrong O_NONBLOCK usage */

#define _TIF_SYSCALL_TRACE	(1 << TIF_SYSCALL_TRACE)
#define _TIF_SIGPENDING		(1 << TIF_SIGPENDING)
#define _TIF_NOTIFY_SIGNAL	(1 << TIF_NOTIFY_SIGNAL)
#define _TIF_NEED_RESCHED	(1 << TIF_NEED_RESCHED)
#define _TIF_POLLING_NRFLAG	(1 << TIF_POLLING_NRFLAG)
#define _TIF_32BIT		(1 << TIF_32BIT)
#define _TIF_SYSCALL_AUDIT	(1 << TIF_SYSCALL_AUDIT)
#define _TIF_NOTIFY_RESUME	(1 << TIF_NOTIFY_RESUME)
#define _TIF_SINGLESTEP		(1 << TIF_SINGLESTEP)
#define _TIF_BLOCKSTEP		(1 << TIF_BLOCKSTEP)
#define _TIF_SECCOMP		(1 << TIF_SECCOMP)
#define _TIF_SYSCALL_TRACEPOINT	(1 << TIF_SYSCALL_TRACEPOINT)

#define _TIF_USER_WORK_MASK     (_TIF_SIGPENDING | _TIF_NOTIFY_RESUME | \
                                 _TIF_NEED_RESCHED | _TIF_NOTIFY_SIGNAL)
#define _TIF_SYSCALL_TRACE_MASK (_TIF_SYSCALL_TRACE | _TIF_SINGLESTEP |	\
				 _TIF_BLOCKSTEP | _TIF_SYSCALL_AUDIT | \
				 _TIF_SECCOMP | _TIF_SYSCALL_TRACEPOINT)

#ifdef CONFIG_64BIT
# ifdef CONFIG_COMPAT
#  define is_32bit_task()	(test_thread_flag(TIF_32BIT))
# else
#  define is_32bit_task()	(0)
# endif
#else
# define is_32bit_task()	(1)
#endif

#endif /* _ASM_PARISC_THREAD_INFO_H */
