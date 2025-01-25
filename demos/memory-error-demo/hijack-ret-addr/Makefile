GCC := gcc-11

help:
	@echo "prep demo shellcode restore clean"

prep:
	cat /proc/sys/kernel/randomize_va_space
	sudo sysctl -w kernel.randomize_va_space=0
	cat /proc/sys/kernel/randomize_va_space

demo:
	$(GCC) -fno-stack-protector -z execstack -g -o demo demo.c

shellcode:
	$(GCC) -z execstack -o shellcode shellcode.c

restore:
	sudo sysctl -w kernel.randomize_va_space=2
	cat /proc/sys/kernel/randomize_va_space

clean:
	rm -rf demo shellcode input
