#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
# include <linux/sched.h>
# include <linux/netlink.h>
# include <net/sock.h>
# include <net/net_namespace.h>

/*
  refer to https://elixir.bootlin.com/linux/v5.15.13/source/include/linux/netlink.h
*/

#define MY_NETLINK 30
#define SIZE 2 * sizeof(int)
#define BCM2837_GPIO_ADDRESS 0x3F200000

static struct sock *nl_sk = NULL;
static unsigned int* gpio_registers = NULL;
int* msg;

static void gpio_pin_on(unsigned int pin)
{
	unsigned int fsel_index = pin/10;
	unsigned int fsel_bitpos = pin%10;
	unsigned int* gpio_fsel = gpio_registers + fsel_index;
	unsigned int* gpio_on_register = (unsigned int*)((char*)gpio_registers + 0x1c);

	*gpio_fsel &= ~(7 << (fsel_bitpos*3));
	*gpio_fsel |= (1 << (fsel_bitpos*3));
	*gpio_on_register |= (1 << pin);

	return;
}

static void gpio_pin_off(unsigned int pin)
{
	unsigned int *gpio_off_register = (unsigned int*)((char*)gpio_registers + 0x28);

	*gpio_off_register |= (1<<pin);
	return;
}

static void nl_recv_msg (struct sk_buff *skb) {
    int pid;
    struct nlmsghdr *nlh = NULL;
    struct sk_buff *skb_out;

    printk(KERN_INFO "enterd callback function");

    if (skb == NULL) {
        printk(KERN_ALERT "skb is null\n");
        return;
    }

    nlh = (struct nlmsghdr*)skb->data;
    int* recv = (int*)NLMSG_DATA(nlh);

    int pin = *recv;
    int mode = *(recv + 1);

    if (pin > 21 || pin < 0) {
      printk("invalid pin number: %d", pin);
      return;
    }

    if (mode > 1 || mode < 0) {
      printk("invalid mode: %d", mode);
      return;
    }

    printk("setting pin %d to %d", pin, mode);

    if (mode == 1) {
      gpio_pin_on(pin);
    } else {
      gpio_pin_off(pin);
    }

    pid = nlh->nlmsg_pid;

    skb_out = nlmsg_new(SIZE, 0);

    if (!skb_out) {
        printk(KERN_ERR "failed to allocate skb\n");
        return;
    }

    nlh = nlmsg_put(skb_out, 0, 0, NLMSG_DONE, SIZE, 0);
    NETLINK_CB(skb_out).dst_group = 0;

    printk(KERN_INFO "sending msg with payload: first: %d, second: %d", *recv, *(recv+1));
    int ret = memcpy(NLMSG_DATA(nlh), recv, SIZE);

    if (ret < 0) {
      printk(KERN_INFO "error on memcpy: %d", ret);
    }

    if (nlmsg_unicast(nl_sk, skb_out, pid) < 0) {
        printk(KERN_ERR "error sending mesg");
    }

}


static int __init nl_init(void) {           

    gpio_registers = (int*)ioremap(BCM2837_GPIO_ADDRESS, PAGE_SIZE);

    if (gpio_registers == NULL) {
      printk(KERN_ALERT "error mapping gpio addr");      
      return -1;
    }

    struct netlink_kernel_cfg cfg = {
      .input = nl_recv_msg,
    };
    
    nl_sk = netlink_kernel_create(&init_net, MY_NETLINK, &cfg);
    
    printk(KERN_INFO "Entering: %s, protocol family = %d \n",__FUNCTION__, MY_NETLINK);

    if (!nl_sk) 
    {
      printk(KERN_ALERT "error creating netlink socket");
      return -1;
    }

    printk(KERN_INFO "MyNetLink Init OK!\n");
    return 0;
}

static void __exit nl_exit(void)
{   
    sock_release(nl_sk->sk_socket);
    iounmap(gpio_registers);
    printk(KERN_INFO "exiting myNetLink module\n");    
}

module_init(nl_init);
module_exit(nl_exit);
MODULE_LICENSE("GPL");