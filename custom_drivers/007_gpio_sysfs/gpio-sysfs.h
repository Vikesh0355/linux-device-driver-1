ssize_t direction_show(struct device *dev, struct device_attribute *attr,char *buf);
ssize_t direction_store(struct device *dev, struct device_attribute *attr,const char *buf, size_t count);
ssize_t value_show(struct device *dev, struct device_attribute *attr,char *buf);
ssize_t value_store(struct device *dev, struct device_attribute *attr,const char *buf, size_t count);
ssize_t label_show(struct device *dev, struct device_attribute *attr,char *buf);
void  gpio_sysfs_remove(struct platform_device *pdev);
int gpio_sysfs_probe(struct platform_device *pdev);
void __exit gpio_sysfs_exit(void);
int __init gpio_sysfs_init(void);
struct gpio_desc *devm_fwnode_get_gpiod_from_child(struct device *dev,
						   const char *con_id,
						   struct fwnode_handle *child,
						   enum gpiod_flags flags,
						   const char *label);