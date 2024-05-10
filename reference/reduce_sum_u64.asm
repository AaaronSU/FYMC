data:
	u64 a 0			# Memory address of array a

code:	
	movui u0, 0 	 	# Array index
	movui u1, 1000		# Number of array elements boundary
	movui u2, 8	 	 	# sizeof(u64)
	mulu  u1, u1, u2 	# Array size in bytes (loop boundary)
	movui u24, 0	 	# Accumulator

	movui u3, $a
	movui u25, 1	 	# Initializer for a

# Initialize arrays
_init_loop:
	storeu (u3, u0), u25 	# memory(u3 + u0) = u25 

	addu u0, u0, u2			# u0 += u2 
	cmpu u0, u1

	jl _init_loop

	movui u0, 0

	# Compute dotprod
_dotprod_loop:

	loadu u25, (u3, u0)		# Load 8-byte DP FP value from a's memory

	addu u24, u24, u25		# u24 += u25

	addu u0, u0, u2			# u0 += 8 bytes
	cmpu u0, u1
	jl _dotprod_loop

	outu u24

	hlt
