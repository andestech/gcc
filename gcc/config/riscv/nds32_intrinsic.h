#ifndef _NDS32_INTRINSIC_H
#define _NDS32_INTRINSIC_H

register long __sp asm ("sp");

#define __nds32__get_current_sp() __sp
#define __nds32__set_current_sp(new_sp) __sp = new_sp

#define __nds32__msync_all() asm volatile ("fence" : : : "memory")

#define __nds32__cctl_l1d_wball_one_lvl()
#define __nds32__cctl_l1d_wball_alvl()
#define __nds32__cctl_l1d_invalall()

static unsigned int __nds32__rotr(unsigned int val, unsigned int ror) __attribute__((unused));
static unsigned int __nds32__wsbh(unsigned int a) __attribute__((unused));

static unsigned int __nds32__rotr(unsigned int val, unsigned int ror)
{
  unsigned int rotr = ror & 0x1f;
  return (val >> rotr) | (val << (sizeof(val)*8 - rotr));
}

static unsigned int __nds32__wsbh(unsigned int a)
{
  unsigned int b0 = (a >>  0) & 0xff;
  unsigned int b1 = (a >>  8) & 0xff;
  unsigned int b2 = (a >> 16) & 0xff;
  unsigned int b3 = (a >> 24) & 0xff;
  return (b0 << 8) | (b1 << 0) | (b2 << 24) | (b3 << 16);
}

#endif
