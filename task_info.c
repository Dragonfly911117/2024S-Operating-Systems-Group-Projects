#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/uaccess.h>

#define BUFFER_SIZE 256

static struct proc_dir_entry *task_info_entry;

static ssize_t task_info_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos) {
  char buffer[BUFFER_SIZE];
  int len;
  struct task_struct *task;

  if (*ppos > 0) {
    return 0;
  }

  task = get_current();

  len = snprintf(buffer, BUFFER_SIZE,
                 "Task Name: %s\nPID: %d\nState: %ld\nPriority: %d\nStatic Priority: %d\nNormal Priority: %d\n",
                 task->comm, task->pid, task->__state, task->prio, task->static_prio, task->normal_prio);

  if (len > BUFFER_SIZE) {
    len = BUFFER_SIZE;
  }

  if (copy_to_user(user_buf, buffer, len)) {
    return -EFAULT;
  }

  *ppos = len;
  return len;
}

static ssize_t task_info_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos) {
  char buffer[BUFFER_SIZE];
  memset(buffer, 0, BUFFER_SIZE);
  size_t len = count;

  if (len > BUFFER_SIZE) {
    len = BUFFER_SIZE;
  }

  if (copy_from_user(buffer, user_buf, len)) {
    return -EFAULT;
  }

  // Process the written data (if needed)
  printk(KERN_INFO "task_info: Received %s\n", buffer);

  return len;
}

static const struct proc_ops task_info_proc_ops = {
    .proc_read = task_info_read,
    .proc_write = task_info_write,
};

static int __init task_info_init(void) {
  task_info_entry = proc_create("task_info", 0666, NULL, &task_info_proc_ops);
  if (!task_info_entry) {
    return -ENOMEM;
  }

  return 0;
}

static void __exit task_info_exit(void) {
  proc_remove(task_info_entry);
}

module_init(task_info_init);
module_exit(task_info_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Task Information Module");
MODULE_AUTHOR("Your Name");