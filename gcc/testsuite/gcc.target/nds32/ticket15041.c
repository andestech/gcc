/* { dg-do compile } */
/* { dg-options "-mcpu=d1088 -mext-dsp -O2" } */

#include <stdint.h>

typedef long unsigned int size_t;
typedef int wchar_t;
typedef int8_t __s8;
typedef uint8_t __u8;
typedef int16_t __s16;
typedef uint16_t __u16;
typedef int32_t __s32;
typedef uint32_t __u32;
typedef uint64_t __u64;
typedef int64_t __s64;
typedef int8_t s8;
typedef uint8_t u8;
typedef int16_t s16;
typedef uint16_t u16;
typedef int32_t s32;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int64_t s64;
typedef char c8;
typedef char char_t;
typedef int8_t s8_t;
typedef uint8_t u8_t;
typedef int16_t s16_t;
typedef uint16_t u16_t;
typedef int32_t s32_t;
typedef uint32_t u32_t;

struct st_rate_gain
{
  uint8_t rate1;
  uint8_t rate2;
  uint8_t rate3;
  uint8_t rate4;
};

struct st_tempe_5g_table
{
  uint8_t bbscale_band0;
  uint8_t bbscale_band1;
  uint8_t bbscale_band2;
  uint8_t bbscale_band3;
  uint32_t bias1;
  uint32_t bias2;
};

static uint8_t rate_gain_value[14] = {0x72, 0x5B, 0x60, 0x66, 0x6C, 0x72, 0x79,
				      0x80, 0x88, 0x90, 0x98, 0xA1, 0xAB, 0xB5};

int check_rate_gain_value(struct st_rate_gain rate_gain, uint8_t flag)
{
  if( flag & 0x08 )
    {
      if(rate_gain.rate1 < 0 || rate_gain.rate1 > 13 )
        return -1;
    }
  if( flag & 0x04 )
    {
      if(rate_gain.rate2 < 0 || rate_gain.rate2 > 13 )
        return -1;
    }
  if( flag & 0x02 )
    {
      if(rate_gain.rate3 < 0 || rate_gain.rate3 > 13 )
        return -1;
    }
  if( flag & 0x01 )
    {
      if(rate_gain.rate4 < 0 || rate_gain.rate4 > 13 )
        return -1;
    }

  return 0;
}
uint32_t flag_to_mask(uint8_t flag)
{
  uint32_t reg_mask = 0;

  if( flag & 0x08 )
    reg_mask = reg_mask | 0xFF000000;
  if( flag & 0x04 )
    reg_mask = reg_mask | 0x00FF0000;
  if( flag & 0x02 )
    reg_mask = reg_mask | 0x0000FF00;
  if( flag & 0x01 )
    reg_mask = reg_mask | 0x000000FF;

  return reg_mask;
}


int write_reg_rate_gain_g(struct st_rate_gain rate_g, uint8_t wrk_flag)
{
  uint32_t new_value = 0;
  uint32_t reg_value = 0;
  uint32_t reg_mask = 0;

  if( check_rate_gain_value(rate_g, wrk_flag) != 0 )
    return -1;

  reg_mask = flag_to_mask(wrk_flag);
  new_value = new_value | ( rate_gain_value[rate_g.rate1]<<24 );
  new_value = new_value | ( rate_gain_value[rate_g.rate2]<<16 ) ;
  new_value = new_value | ( rate_gain_value[rate_g.rate3]<<8 );
  new_value = new_value | ( rate_gain_value[rate_g.rate4]<<0) ;
  reg_value = ( *( (volatile uint32_t *) ((0xccb0e000 +0x00000184)) ) );
  reg_value = (reg_value &~reg_mask ) | (new_value & reg_mask);
  ( *( (volatile uint32_t *) ((0xccb0e000 +0x00000184)) ) ) = reg_value;

  return 0;
}
