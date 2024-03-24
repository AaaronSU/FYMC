	# Sample code for computing a dotprod of two arrays
data:
	f64 @a 0 	 # Memory address of array a
	f64 @b 200000	 # Memory address of array b
	
code:	
	movui u0, 0 	 # Array index
	movui u1, 10000	 # Number of array elements boundary
	movui u2, 8	 # sizeof(f64)
	mulu  u1, u1, u2 # Array size in bytes (loop boundary)
	movfi f0, 0.0	 # Accumulator

	movui u3, @a	
	movui u4, @b

	movfi f1, 1.1	 # Initializer for a
	movfi f2, 2.2	 # Initializer for b

	# Initialize arrays
_init_loop:

	storeu f1, (u3, u0)
	storeu f2, (u4, u0)

	addu u0, u0, u2
	cmpu u0, u1
	jl _init_loop

	movui u0, 0

	# Compute dotprod
_dotprod_loop:

	loadf f1, (u3, u0)	# Load 8-byte DP FP value from a's memory
	loadf f2, (u4, u0)	# Load 8-byte DP FP value from b's memory

	fmaf f0, f1, f2		# f0 += f1 * f2

	addu u0, u0, u2		# u0 += 8 bytes
	cmpu u0, u1
	jl _dotprod_loop

	outf f0

	hlt
