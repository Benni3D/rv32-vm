#ifndef FILE_DEVICES_HPP
#define FILE_DEVICES_HPP
#include "bus.hpp"

namespace rv32 {
	class RAM : public BusDevice {
	private:
		std::uint8_t* mem;
	public:
		explicit RAM(std::uint32_t sz) : BusDevice(sz), mem(new std::uint8_t[sz]) {}
		~RAM() noexcept override { delete[] mem; mem = nullptr; }
		
		void write8(std::uint32_t addr, std::uint8_t data) override {
			mem[addr] = data;
		}
		void write16(std::uint32_t addr, std::uint16_t data) override {
			mem[addr + 0] = data & 0xff;
			mem[addr + 1] = (data >> 8) & 0xff;
		}
		void write32(std::uint32_t addr, std::uint32_t data) override {
			mem[addr + 0] = data & 0xff;
			mem[addr + 1] = (data >> 8) & 0xff;
			mem[addr + 2] = (data >> 16) & 0xff;
			mem[addr + 3] = (data >> 24) & 0xff;
		}
		std::uint8_t read8(std::uint32_t addr) override {
			return mem[addr];
		}
		std::uint16_t read16(std::uint32_t addr) override {
			return mem[addr] | (mem[addr + 1] << 8);
		}
		std::uint32_t read32(std::uint32_t addr) override {
			return mem[addr] | (mem[addr + 1] << 8) | (mem[addr + 2] << 16) | (mem[addr + 3] << 24);
		}
	};
}

#endif /* FILE_DEVICES_HPP */
