// proc_desc.c —— 演示进程描述符（task_struct）、current、进程状态、进程家族树与内核线程
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/sched/task.h>
#include <linux/kthread.h>
#include <linux/list.h>

#define OURMODNAME "proc_desc"

static struct task_struct *kthrd;

static int kthrd_fn(void *data)
{
	pr_info("%s: kernel thread [%s] running, pid=%d\n",
		OURMODNAME, current->comm, current->pid);
	while (!kthread_should_stop())
		schedule();
	pr_info("%s: kernel thread [%s] exiting\n", OURMODNAME, current->comm);
	return 0;
}

static void show_state_consts(void)
{
	pr_info("%s: process state constants (include/linux/sched.h):\n", OURMODNAME);
	pr_info("  TASK_RUNNING         = 0x%04x\n", TASK_RUNNING);
	pr_info("  TASK_INTERRUPTIBLE   = 0x%04x\n", TASK_INTERRUPTIBLE);
	pr_info("  TASK_UNINTERRUPTIBLE = 0x%04x\n", TASK_UNINTERRUPTIBLE);
	pr_info("  __TASK_STOPPED       = 0x%04x\n", __TASK_STOPPED);
	pr_info("  __TASK_TRACED        = 0x%04x\n", __TASK_TRACED);
	pr_info("  TASK_WAKEKILL        = 0x%04x\n", TASK_WAKEKILL);
	pr_info("  TASK_KILLABLE        = 0x%04x\n", TASK_KILLABLE);
	pr_info("  TASK_IDLE            = 0x%04x\n", TASK_IDLE);
	pr_info("  TASK_NEW             = 0x%04x\n", TASK_NEW);
}

static void show_current(void)
{
	pr_info("%s: sizeof(struct task_struct) = %zu\n",
		OURMODNAME, sizeof(struct task_struct));
	pr_info("%s: current (our task_struct) pointer = 0x%px\n",
		OURMODNAME, current);
	pr_info("%s:   pid   = %d\n", OURMODNAME, current->pid);
	pr_info("%s:   tgid  = %d\n", OURMODNAME, current->tgid);
	pr_info("%s:   comm  = %s\n", OURMODNAME, current->comm);
	pr_info("%s:   __state = 0x%x (TASK_RUNNING=0 means runnable)\n",
		OURMODNAME, current->__state);
}

static void show_family_tree(void)
{
	struct task_struct *t;

	pr_info("%s: walking UP the parent chain from current:\n", OURMODNAME);
	for (t = current; t != &init_task; t = t->parent)
		pr_info("    %s (pid=%d) -> parent %s (pid=%d)\n",
			t->comm, t->pid, t->parent->comm, t->parent->pid);
	pr_info("    reached init_task: %s (pid=%d)\n",
		init_task.comm, init_task.pid);

	pr_info("%s: children of current (%s):\n", OURMODNAME, current->comm);
	if (list_empty(&current->children)) {
		pr_info("    <none — current has no children>\n");
	} else {
		list_for_each_entry(t, &current->children, sibling)
			pr_info("    child %s (pid=%d)\n", t->comm, t->pid);
	}
}

static int __init proc_desc_init(void)
{
	pr_info("%s: inserted\n", OURMODNAME);
	show_state_consts();
	show_current();
	show_family_tree();

	kthrd = kthread_run(kthrd_fn, NULL, "proc_desc_kthrd");
	if (IS_ERR(kthrd)) {
		pr_err("%s: kthread_run failed\n", OURMODNAME);
		return PTR_ERR(kthrd);
	}
	pr_info("%s: created kernel thread, kthrd=%s (pid=%d)\n",
		OURMODNAME, kthrd->comm, kthrd->pid);
	return 0;
}

static void __exit proc_desc_exit(void)
{
	kthread_stop(kthrd);
	pr_info("%s: kernel thread stopped\n", OURMODNAME);
	pr_info("%s: removed\n", OURMODNAME);
}

module_init(proc_desc_init);
module_exit(proc_desc_exit);

MODULE_AUTHOR("linxi");
MODULE_DESCRIPTION("Demo: process descriptor, current, states, family tree, kthread (LKD ch3, 5.15)");
MODULE_LICENSE("GPL");
