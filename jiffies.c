#include <linux/jiffies.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#define BUFFER_SIZE 128

static struct proc_dir_entry *jiffies_entry;

static ssize_t jiffies_read(struct file *file, char __user *user_buf,
                            size_t count, loff_t *ppos) {
  char buffer[BUFFER_SIZE];
  int len;

  len = snprintf(buffer, BUFFER_SIZE, "%lu\n", jiffies);
  return simple_read_from_buffer(user_buf, count, ppos, buffer, len);
}

static const struct proc_ops jiffies_proc_ops = {
    .proc_read = jiffies_read,
};

static int __init jiffies_module_init(void) {
  jiffies_entry = proc_create("jiffies", 0, NULL, &jiffies_proc_ops);
  if (!jiffies_entry) {
    return -ENOMEM;
  }

  return 0;
}

static void __exit jiffies_module_exit(void) { proc_remove(jiffies_entry); }

module_init(jiffies_module_init);
module_exit(jiffies_module_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Jiffies Module");
MODULE_AUTHOR("Your Name");