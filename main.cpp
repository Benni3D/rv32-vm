#include <fmt/core.h>
#include "cpu.hpp"
#include "devices.hpp"

int main() {
	using namespace rv32;
	Processor proc{};
	proc.mem.add(std::make_unique<RAM>(256), 0);
	proc.mem.load_file("/home/benni/src/rv32i-emulator/test/prog");
	
	while (proc.clk());
	proc.dump();
	return int(proc.regs[1].read());
}