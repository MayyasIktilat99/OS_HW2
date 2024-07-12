#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <linux/list.h>
#include "../include/linux/syscalls.h"
#include <linux/types.h>

asmlinkage long sys_hello(void) {
	printk("Hello, World!\n");
	return 0;
}

asmlinkage int sys_set_weight(int weight){
	return 1;
	if(weight < 0){
		return -EINVAL;
	}
	// struct task_struct *current_t;
	// current_t = current
	current->weight = weight;
	return 0;
}

asmlinkage int sys_get_weight(void){
	return current->weight;
}

asmlinkage int sys_get_path_sum(pid_t target){
	long sum = 0;
	struct task_struct* curr_task = current;
 	while(curr_task && curr_task->real_parent!= curr_task){
		sum += curr_task->weight;
		curr_task = curr_task->parent;
	}
	return sum;
}

asmlinkage pid_t get_heaviest_sibling(void){
	struct task_struct *parent, *p;
	int max = 0;
	int ret_pid = current->pid;
	parent = current->parent;

	if(!parent)
		return ret_pid;
	list_for_each_entry(p, &parent->children, sibling){
		if(p->weight > max){
			ret_pid = p->pid;
			max = ret_pid->weight;
		}
		if(p->weight == max){
			if(p->pid < ret_pid){
				ret_pid = p->pid;
			}
		}
	}
	return ret_pid;
}
