#ifndef _NDS32_INTRINSIC_H
#define _NDS32_INTRINSIC_H

#include <nds_intrinsic.h>

#if __riscv_xlen == 32
#define __nds32__msync_all __nds__msync_all

#define __nds32__cctl_l1d_wball_one_lvl __nds__cctl_l1d_wball_one_lvl
#define __nds32__cctl_l1d_wball_alvl __nds__cctl_l1d_wball_alvl
#define __nds32__cctl_l1d_invalall __nds__cctl_l1d_invalall

#define __nds32__mfsr __nds__mfsr
#define __nds32__mtsr __nds__mtsr

#define __nds32__rotr __nds__rotr
#define __nds32__wsbh __nds__wsbh
#endif

#endif
