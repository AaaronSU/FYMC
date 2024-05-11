verify_as:
	cd asm_to_binary && make && cd ..

	cp ./asm_to_binary/op_codes ./asm_to_binary/register_list .

	./asm_to_binary/fymcc reference/dotprod_u64.asm && python3 script/compare.py a.archy reference/dotprod_u64.archyb

	# ./asm_to_binary/fymcc reference/dotprod.asm && python3 script/compare.py a.archy reference/dotprod.archyb

	./asm_to_binary/fymcc reference/hello_world.asm && python3 script/compare.py a.archy reference/hello_world.archyb

	./asm_to_binary/fymcc reference/reduce_sum_u64.asm && python3 script/compare.py a.archy reference/reduce_sum_u64.archyb

	./asm_to_binary/fymcc reference/fibonacci.asm && python3 script/compare.py a.archy reference/fibonacci.archyb

	./asm_to_binary/fymcc reference/dotprod.asm

	rm ./op_codes ./register_list ./a.archy
	cd asm_to_binary && make clean && cd ..