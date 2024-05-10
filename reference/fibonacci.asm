	# Sample program demonstrating how to calculate the Fibonacci series 
data:

code:
	movui u0, 0 		# Iteration index
	movui u1, 10		# Number of iterations

	# First two terms of the series
	movui u2, 1		
	movui u3, 1

_loop_entry:	
	addu u4, u2, u3		# Calculate the term's value
	outu u4			# Print the value

	# Swap and update the values for next iteration
	movu u2, u3		
	movu u3, u4

	incu u0
	cmpu u0, u1
	jl   _loop_entry
	hlt
