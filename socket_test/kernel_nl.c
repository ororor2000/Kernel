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
static struct sock *nl_sk = NULL;

#define MY_NETLINK 30
#define MAX_PAYLOAD 1024
char* msg[MAX_PAYLOAD];

static void nl_recv_msg (struct sk_buff *skb) {
  int pid;
  struct nlmsghdr *nlh = NULL;
  struct sk_buff *skb_out;
  char* data = NULL;

  printk(KERN_INFO "enterd callback function");

  memset(&msg, 0, sizeof(msg));

  if (skb == NULL) {
    printk(KERN_ALERT "skb is null\n");
    return;
  }

  nlh = (struct nlmsghdr*)skb->data;  

  printk(KERN_INFO "got message: %s", NLMSG_DATA(nlh));  
  data = (char*)NLMSG_DATA(nlh);

  pid = nlh->nlmsg_pid;

  skb_out = nlmsg_new(MAX_PAYLOAD, 0);

  if (!skb_out) {
    printk(KERN_ERR "failed to allocate skb\n");
    return;
  }

  nlh = nlmsg_put(skb_out, 0, 0, NLMSG_DONE, MAX_PAYLOAD, 0);
  NETLINK_CB(skb_out).dst_group = 0;
  strncpy(NLMSG_DATA(nlh), data, MAX_PAYLOAD);
  
  if (nlmsg_unicast(nl_sk, skb_out, pid) < 0) {
    printk(KERN_ERR "error sending mesg");
  }

  printk(KERN_INFO "sending msg with payload: %s", data);
}


static int __init nl_init(void)
{           
    struct netlink_kernel_cfg cfg = {
      .input = nl_recv_msg,
    };
    
    nl_sk = netlink_kernel_create(&init_net, MY_NETLINK, &cfg);
    
    printk(KERN_INFO "Entering: %s, protocol family = %d \n",__FUNCTION__, MY_NETLINK);

    if (!nl_sk) 
    {
      printk(KERN_ALERT "error");
      return -1;
    }

    printk(KERN_INFO "MyNetLink Init OK!\n");
    return 0;
}

static void __exit nl_exit(void)
{   
    sock_release(nl_sk->sk_socket);
    printk(KERN_INFO "exiting myNetLink module\n");    
}

module_init(nl_init);
module_exit(nl_exit);
MODULE_LICENSE("GPL");