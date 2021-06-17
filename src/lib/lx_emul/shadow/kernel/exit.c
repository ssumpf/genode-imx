
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/rcuwait.h>

#include <lx_emul/debug.h>


int rcuwait_wake_up(struct rcuwait * w)
{
	if (w && w->task)
		return  wake_up_process(w->task);

	return 0;
}



void __noreturn do_exit(long code)
{
	struct task_struct *tsk = current;
	tsk->exit_code = code;
	set_special_state(TASK_DEAD);

	if (tsk->vfork_done)
		complete(tsk->vfork_done);

	current->flags |= PF_NOFREEZE;
	schedule();
	BUG();
}
