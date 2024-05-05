#include <linux/jiffies.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#define BUFFER_SIZE 128

static struct proc_dir_entry *seconds_entry;
static unsigned long start_jiffies;

static ssize_t seconds_read(struct file *file, char __user *user_buf,
                            size_t count, loff_t *ppos) {
  char buffer[BUFFER_SIZE];
  int len;
  unsigned long elapsed_seconds;

  elapsed_seconds = (jiffies - start_jiffies) / HZ;
  len = snprintf(buffer, BUFFER_SIZE, "%lu\n", elapsed_seconds);
  return simple_read_from_buffer(user_buf, count, ppos, buffer, len);
}

static const struct proc_ops seconds_proc_ops = {
    .proc_read = seconds_read,
};

static int __init seconds_module_init(void) {
  seconds_entry = proc_create("seconds", 0, NULL, &seconds_proc_ops);
  if (!seconds_entry) {
    return -ENOMEM;
  }

  start_jiffies = jiffies;

  return 0;
}

static void __exit seconds_module_exit(void) { proc_remove(seconds_entry); }

module_init(seconds_module_init);
module_exit(seconds_module_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Seconds Module");
MODULE_AUTHOR("Your Name");