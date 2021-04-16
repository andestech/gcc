	.text
	.align	2
	.global	__clzsi2
	.type	__clzsi2, @function
/*
#  int n = 31;
#  int shift = 16;
#
#  if (x != 0)
#  {
#  do
#    {
#      if ((x >> shift))
#	{
#	  n -= shift;
#	  x >>= shift;
#	}
#      shift >>= 1;
#    }
#  while (shift > 0);
#
#  return n;
#  }
#  else
#    return (32);
*/
__clzsi2:
	beqz38	$r0, .Lzero
	/* Handel general case.  */
	movi	$r1, #16
	movi	$r3, #31
.Lloop:
	sub333	$r4, $r3, $r1
	srl	$r2, $r0, $r1
	cmovn	$r3, $r4, $r2
	cmovn	$r0, $r2, $r2
	srli45	$r1, #1
	bnez38	$r1, .Lloop

	move	$r0, $r3
	ret5

.Lzero:
	/* Handel corner case. (Input value is zero)  */
	movi	$r0, #32
	ret5
	.size __clzsi2, .-__clzsi2
