	# Sample code for computing a dotprod of two arrays
data:
	u64 a 0 	 # Memory address of array a
	u64 b 200000	 # Memory address of array b
	
code:	
	movui u0, 0 	 # Array index
	movui u1, 1000	 # Number of array elements boundary
    movui u5, 64
	mulu  u1, u1, u2 # Array size in bytes (loop boundary)
	movgi g0 { 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 }	 # Accumulator

	movui u3, $a	
	movui u4, $b

	movgi g1 { 1.1 1.1 1.1 1.1 1.1 1.1 1.1 1.1 }
	movgi g2 { 2.2 2.2 2.2 2.2 2.2 2.2 2.2 2.2 }

	# Initialize arrays
_init_loop:

	storeg (u3, u0), g1
	storeg (u4, u0), g2

	addu u0, u0, u5
	cmpu u0, u1
	jl _init_loop

	movui u0, 0

	# Compute dotprod
_dotprod_loop:

	loadg g1, (u3, u0)	# Load 8-byte DP FP value from a's memory
	loadg g2, (u4, u0)	# Load 8-byte DP FP value from b's memory

	fmag g0, g1, g2 	# g0 += g1 * g2
	addu u0, u0, u5		# u0 += 64 bytes

	cmpu u0, u1
	jl _dotprod_loop

	sumg g0
	outg g0

	hlt
