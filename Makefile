verify_as:
	cp ./asm_to_binary/op_codes ./asm_to_binary/register_list .

	./asm_to_binary/fymcc reference/dotprod_u64.asm && python3 script/compare.py a.archy reference/dotprod_u64.archyb

	# ./asm_to_binary/fymcc reference/dotprod.asm && python3 script/compare.py a.archy reference/dotprod.archyb

	./asm_to_binary/fymcc reference/hello_world.asm && python3 script/compare.py a.archy reference/hello_world.archyb

	./asm_to_binary/fymcc reference/reduce_sum_u64.asm && python3 script/compare.py a.archy reference/reduce_sum_u64.archyb

	./asm_to_binary/fymcc reference/fibonacci.asm && python3 script/compare.py a.archy reference/fibonacci.archyb

	rm ./op_codes ./register_list