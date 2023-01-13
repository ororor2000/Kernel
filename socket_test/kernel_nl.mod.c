#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xbeefae88, "module_layout" },
	{ 0x4db5959a, "sock_release" },
	{ 0x8da6585d, "__stack_chk_fail" },
	{ 0xd1a1d7d, "__netlink_kernel_create" },
	{ 0x513b1139, "init_net" },
	{ 0x4baaff7c, "netlink_unicast" },
	{ 0x9166fada, "strncpy" },
	{ 0x14a34fe3, "__nlmsg_put" },
	{ 0x422c4641, "__alloc_skb" },
	{ 0xdcb764ad, "memset" },
	{ 0x92997ed8, "_printk" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "B8DFD19BCFA1D4B52E1E546");
