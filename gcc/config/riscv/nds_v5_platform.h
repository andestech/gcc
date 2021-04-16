#ifndef __NDS_V5_PLATFORM_H__
#define __NDS_V5_PLATFORM_H__

#include "nds_intrinsic.h"

#ifdef __cplusplus
extern "C" {
#endif

/* PLIC Feature Flags */
#define NDS_PLIC_FEATURE_PREEMPT        (1 << 0)
#define NDS_PLIC_FEATURE_VECTORED       (1 << 1)

/* Priority Register - 32 bits per source */
#define PLIC_PRIORITY_OFFSET            (0x00000000UL)
#define PLIC_PRIORITY_SHIFT_PER_SOURCE  2

/* Pending Register - 1 bit per soirce */
#define PLIC_PENDING_OFFSET             (0x00001000UL)
#define PLIC_PENDING_SHIFT_PER_SOURCE   0

/* Enable Register - 0x80 per target */
#define PLIC_ENABLE_OFFSET              (0x00002000UL)
#define PLIC_ENABLE_SHIFT_PER_TARGET    7

/* Priority Threshold Register - 0x1000 per target */
#define PLIC_THRESHOLD_OFFSET           (0x00200000UL)
#define PLIC_THRESHOLD_SHIFT_PER_TARGET 12

/* Claim Register - 0x1000 per target */
#define PLIC_CLAIM_OFFSET               (0x00200004UL)
#define PLIC_CLAIM_SHIFT_PER_TARGET     12


#ifdef NDS_PLIC_BASE

__attribute__((always_inline)) static inline void __nds__plic_set_feature (unsigned int feature)
{
  volatile unsigned int *feature_ptr = (volatile unsigned int *)NDS_PLIC_BASE;
  *feature_ptr = feature;
}

__attribute__((always_inline)) static inline void __nds__plic_set_threshold (unsigned int threshold)
{
  unsigned int hart_id = __nds__mfsr(NDS_MHARTID);
  volatile unsigned int *threshold_ptr = (volatile unsigned int *)(NDS_PLIC_BASE +
                                                                   PLIC_THRESHOLD_OFFSET +
                                                                   (hart_id << PLIC_THRESHOLD_SHIFT_PER_TARGET));
  *threshold_ptr = threshold;
}

__attribute__((always_inline)) static inline void __nds__plic_set_priority (unsigned int source, unsigned int priority)
{
  volatile unsigned int *priority_ptr = (volatile unsigned int *)(NDS_PLIC_BASE +
                                                                  PLIC_PRIORITY_OFFSET +
                                                                  (source << PLIC_PRIORITY_SHIFT_PER_SOURCE));
  *priority_ptr = priority;
}

__attribute__((always_inline)) static inline void __nds__plic_set_pending (unsigned int source)
{
  volatile unsigned int *current_ptr = (volatile unsigned int *)(NDS_PLIC_BASE +
                                                                 PLIC_PENDING_OFFSET +
                                                                 ((source >> 5) << 2));
  *current_ptr = (1 << (source & 0x1F));
}

__attribute__((always_inline)) static inline void __nds__plic_enable_interrupt (unsigned int source)
{
  unsigned int hart_id = __nds__mfsr(NDS_MHARTID);
  volatile unsigned int *current_ptr = (volatile unsigned int *)(NDS_PLIC_BASE +
                                                                 PLIC_ENABLE_OFFSET +
                                                                 (hart_id << PLIC_ENABLE_SHIFT_PER_TARGET) +
                                                                 ((source >> 5) << 2));
  unsigned int current = *current_ptr;
  current = current | (1 << (source & 0x1F));
  *current_ptr = current;
}

__attribute__((always_inline)) static inline void __nds__plic_disable_interrupt (unsigned int source)
{
  unsigned int hart_id = __nds__mfsr(NDS_MHARTID);
  volatile unsigned int *current_ptr = (volatile unsigned int *)(NDS_PLIC_BASE +
                                                                 PLIC_ENABLE_OFFSET +
                                                                 (hart_id << PLIC_ENABLE_SHIFT_PER_TARGET) +
                                                                 ((source >> 5) << 2));
  unsigned int current = *current_ptr;
  current = current & ~((1 << (source & 0x1F)));
  *current_ptr = current;
}

__attribute__((always_inline)) static inline unsigned int __nds__plic_claim_interrupt(void)
{
  unsigned int hart_id = __nds__mfsr(NDS_MHARTID);
  volatile unsigned int *claim_addr = (volatile unsigned int *)(NDS_PLIC_BASE +
                                                                PLIC_CLAIM_OFFSET +
                                                                (hart_id << PLIC_CLAIM_SHIFT_PER_TARGET));
  return  *claim_addr;
}

__attribute__((always_inline)) static inline void __nds__plic_complete_interrupt(unsigned int source)
{
  unsigned int hart_id = __nds__mfsr(NDS_MHARTID);
  volatile unsigned int *claim_addr = (volatile unsigned int *)(NDS_PLIC_BASE +
                                                                PLIC_CLAIM_OFFSET +
                                                                (hart_id << PLIC_CLAIM_SHIFT_PER_TARGET));
  *claim_addr = source;
}

#endif	// NDS_PLIC_BASE

#ifdef NDS_PLIC_SW_BASE

__attribute__((always_inline)) static inline void __nds__plic_sw_set_threshold (unsigned int threshold)
{
  unsigned int hart_id = __nds__mfsr(NDS_MHARTID);
  volatile unsigned int *threshold_ptr = (volatile unsigned int *)(NDS_PLIC_SW_BASE +
                                                                   PLIC_THRESHOLD_OFFSET +
                                                                   (hart_id << PLIC_THRESHOLD_SHIFT_PER_TARGET));
  *threshold_ptr = threshold;
}

__attribute__((always_inline)) static inline void __nds__plic_sw_set_priority (unsigned int source, unsigned int priority)
{
  volatile unsigned int *priority_ptr = (volatile unsigned int *)(NDS_PLIC_SW_BASE +
                                                                  PLIC_PRIORITY_OFFSET +
                                                                  (source << PLIC_PRIORITY_SHIFT_PER_SOURCE));
  *priority_ptr = priority;
}

__attribute__((always_inline)) static inline void __nds__plic_sw_set_pending (unsigned int source)
{
  volatile unsigned int *current_ptr = (volatile unsigned int *)(NDS_PLIC_SW_BASE +
                                                                 PLIC_PENDING_OFFSET +
                                                                 ((source >> 5) << 2));
  *current_ptr = (1 << (source & 0x1F));
}

__attribute__((always_inline)) static inline void __nds__plic_sw_enable_interrupt (unsigned int source)
{
  unsigned int hart_id = __nds__mfsr(NDS_MHARTID);
  volatile unsigned int *current_ptr = (volatile unsigned int *)(NDS_PLIC_SW_BASE +
                                                                 PLIC_ENABLE_OFFSET +
                                                                 (hart_id << PLIC_ENABLE_SHIFT_PER_TARGET) +
                                                                 ((source >> 5) << 2));
  unsigned int current = *current_ptr;
  current = current | (1 << (source & 0x1F));
  *current_ptr = current;
}

__attribute__((always_inline)) static inline void __nds__plic_sw_disable_interrupt (unsigned int source)
{
  unsigned int hart_id = __nds__mfsr(NDS_MHARTID);
  volatile unsigned int *current_ptr = (volatile unsigned int *)(NDS_PLIC_SW_BASE +
                                                                 PLIC_ENABLE_OFFSET +
                                                                 (hart_id << PLIC_ENABLE_SHIFT_PER_TARGET) +
                                                                 ((source >> 5) << 2));
  unsigned int current = *current_ptr;
  current = current & ~((1 << (source & 0x1F)));
  *current_ptr = current;
}

__attribute__((always_inline)) static inline unsigned int __nds__plic_sw_claim_interrupt(void)
{
  unsigned int hart_id = __nds__mfsr(NDS_MHARTID);
  volatile unsigned int *claim_addr = (volatile unsigned int *)(NDS_PLIC_SW_BASE +
                                                                PLIC_CLAIM_OFFSET +
                                                                (hart_id << PLIC_CLAIM_SHIFT_PER_TARGET));
  return  *claim_addr;
}

__attribute__((always_inline)) static inline void __nds__plic_sw_complete_interrupt(unsigned int source)
{
  unsigned int hart_id = __nds__mfsr(NDS_MHARTID);
  volatile unsigned int *claim_addr = (volatile unsigned int *)(NDS_PLIC_SW_BASE +
                                                                PLIC_CLAIM_OFFSET +
                                                                (hart_id << PLIC_CLAIM_SHIFT_PER_TARGET));
  *claim_addr = source;
}

#endif	// NDS_PLIC_SW_BASE

#ifdef __cplusplus
}
#endif

#endif	// __NDS_V5_PLATFORM_H__
