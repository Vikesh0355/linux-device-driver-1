
/*platform data of the pcdev */
struct pcdev_platform_data
{
	int size;
	int perm;
	const char *serial_number;

};

/*Permission codes */

#define RDWR 0x11
#define RDONLY 0x01
#define WRONLY 0x10

int check_permission(int dev_perm, int acc_mode);
loff_t pcd_lseek(struct file *filp, loff_t offset, int whence);
ssize_t pcd_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos);
ssize_t pcd_write(struct file *filp, const char __user *buff, size_t count, loff_t *f_pos);
int pcd_open(struct inode *inode, struct file *filp);
int pcd_release(struct inode *inode, struct file *flip);
//int pcd_platform_driver_remove(struct platform_device *pdev); /*TODO:MISH: coomnedt as per new kernel 6.xx */
void  pcd_platform_driver_remove(struct platform_device *pdev);
struct pcdev_platform_data* pcdev_get_platdata_from_dt(struct device *dev);
int pcd_platform_driver_probe(struct platform_device *pdev);








