#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

MODULE_LICENSE("GPL");

#define PROC_AUTH_DIR "auth"
#define PROC_AUTH_FILE "key"

static struct proc_dir_entry *proc_key_dir;
static struct proc_dir_entry *proc_key_entry;

static int auth_key_file_show(struct seq_file *seq, void *v)
{
    seq_puts(seq, "This an auth key.\n");

    return 0;
}

static int auth_key_open(struct inode *inode, struct file *filp)
{
    return single_open(filp, auth_key_file_show, inode->i_private);
}

static const struct file_operations auth_key_ops = {
    .open = auth_key_open,
    .read = seq_read,
};

static __init int auth_key_init(void)
{
    proc_key_dir = proc_mkdir(PROC_AUTH_DIR, NULL);
    if (proc_key_dir == NULL)
        return -ENOMEM;

    proc_key_entry = proc_create(PROC_AUTH_FILE, 0400, proc_key_dir, &auth_key_ops);
    if (proc_key_entry == NULL)
        return -ENOMEM;

    return 0;
}

static __exit void auth_key_exit(void)
{
    if (proc_key_entry)
        remove_proc_entry(PROC_AUTH_DIR"/"PROC_AUTH_FILE, NULL);

    if (proc_key_dir)
        remove_proc_entry(PROC_AUTH_DIR, NULL);
}

module_init(auth_key_init);
module_exit(auth_key_exit);

