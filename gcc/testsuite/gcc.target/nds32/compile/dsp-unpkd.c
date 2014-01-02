/* { dg-do compile } */
/* { dg-options "-O2 -mext-dsp" } */
/* { dg-final { scan-assembler "sunpkd810" } } */
/* { dg-final { scan-assembler "sunpkd820" } } */
/* { dg-final { scan-assembler "sunpkd830" } } */
/* { dg-final { scan-assembler "sunpkd831" } } */
/* { dg-final { scan-assembler "zunpkd810" } } */
/* { dg-final { scan-assembler "zunpkd820" } } */
/* { dg-final { scan-assembler "zunpkd830" } } */
/* { dg-final { scan-assembler "zunpkd831" } } */

typedef signed char v4qi __attribute__ ((vector_size (4)));
typedef short v2hi __attribute__ ((vector_size (4)));
typedef unsigned char uv4qi __attribute__ ((vector_size (4)));
typedef unsigned short uv2hi __attribute__ ((vector_size (4)));

v2hi sunpkd810(v4qi v)
{
  v2hi ret;
  ret[0] = v[0];
  ret[1] = v[1];
  return ret;
}

v2hi sunpkd820(v4qi v)
{
  v2hi ret;
  ret[0] = v[0];
  ret[1] = v[2];
  return ret;
}

v2hi sunpkd830(v4qi v)
{
  v2hi ret;
  ret[0] = v[0];
  ret[1] = v[3];
  return ret;
}

v2hi sunpkd831(v4qi v)
{
  v2hi ret;
  ret[0] = v[1];
  ret[1] = v[3];
  return ret;
}

uv2hi zunpkd810(uv4qi v)
{
  uv2hi ret;
  ret[0] = v[0];
  ret[1] = v[1];
  return ret;
}

uv2hi zunpkd820(uv4qi v)
{
  uv2hi ret;
  ret[0] = v[0];
  ret[1] = v[2];
  return ret;
}

uv2hi zunpkd830(uv4qi v)
{
  uv2hi ret;
  ret[0] = v[0];
  ret[1] = v[3];
  return ret;
}

uv2hi zunpkd831(uv4qi v)
{
  uv2hi ret;
  ret[0] = v[1];
  ret[1] = v[3];
  return ret;
}
