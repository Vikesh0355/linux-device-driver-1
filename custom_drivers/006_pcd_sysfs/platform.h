
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

ssize_t show_serial_num(struct device *dev, struct device_attribute *attr,char *buf);
ssize_t show_max_size(struct device *dev, struct device_attribute *attr,char *buf);
ssize_t store_max_size(struct device *dev, struct device_attribute *attr,const char *buf, size_t count);
int pcd_sysfs_create_files(struct device *pcd_dev);
//int pcd_platform_driver_remove(struct platform_device *pdev);
void pcd_platform_driver_remove(struct platform_device *pdev);
struct pcdev_platform_data* pcdev_get_platdata_from_dt(struct device *dev);
int pcd_platform_driver_probe(struct platform_device *pdev);