#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <asm/types.h>
#include <linux/netlink.h>
#include <linux/socket.h>
#include <errno.h>

#define NETLINK_USER 30 // same customized protocol as in my kernel module
#define MAX_PAYLOAD 1024 // maximum payload size

struct sockaddr_nl src, dst;
struct nlmsghdr *nlh = NULL;
struct msghdr msg;
struct iovec iov;
int sock_fd;


int main(int args, char *argv[]) {
  // creating socket and getting socket fd
  sock_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_USER);

  if (sock_fd < 0) {
    printf("error creating socket\n");

    return -1;
  }

  // init src and dst adr sturcts
  memset(&src, 0, sizeof(src));
  memset(&dst, 0, sizeof(dst));

  src.nl_family = AF_NETLINK;
  src.nl_pid = getpid();

  if (bind(sock_fd, &src, sizeof(src))) {
    printf("error on bind\n");
    close(sock_fd);

    return -1;
  }

  dst.nl_family = AF_NETLINK;
  dst.nl_pid = 0; // 0 is the kernel
  dst.nl_groups = 0; // 0 is for unicast

  nlh = (struct nlmsghdr*)malloc(NLMSG_SPACE(MAX_PAYLOAD));  

  memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));
  
  nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);

  nlh->nlmsg_pid = getpid();    
  nlh->nlmsg_flags = 0;

  char user_msg[MAX_PAYLOAD] = {0};

  printf("enter message to kernel:\n");
  fgets(&user_msg, MAX_PAYLOAD, stdin);

  printf("len is %llu\n", strlen(user_msg));

  strcpy(NLMSG_DATA(nlh), user_msg);

  iov.iov_base = (void*)nlh;  
  iov.iov_len = nlh->nlmsg_len;  
  msg.msg_name = (void*)&dst;
  msg.msg_namelen = sizeof(dst);  
  msg.msg_iov = &iov;  
  msg.msg_iovlen = 1;


  printf("sending msg '%s' to the kernel\n", user_msg);

  int ret = sendmsg(sock_fd, &msg, 0);
  printf("sending msg - ret: %d\n", ret);

  printf("waiting for response...\n");

  recvmsg(sock_fd, &msg, 0);

  //char* kernel_msg[MAX_PAYLOAD] = (char* )NLMSG_DATA(nl_res_header);

  printf("kernel msg: %s\n", (char*)NLMSG_DATA(nlh));

  close(sock_fd);
  return 0;
}