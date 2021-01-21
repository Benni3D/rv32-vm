#ifndef FILE_MEM_HPP
#define FILE_MEM_HPP
#include <cstdint>
#include <cstdio>
#include <array>
#include "bus.hpp"

namespace rv32 {
	class Memory {
	public:
		Bus bus;
		[[nodiscard]]
		std::uint32_t read32(const std::uint32_t addr) noexcept {
			if (addr & 3) {
				// TODO: Implement: unaligned memory read
				return 0;
			}
			else return get32(addr);
		}
		[[nodiscard]]
		std::uint16_t read16(const std::uint16_t addr) noexcept {
			if (addr & 1) {
				// TODO: Implement: unaligned memory read
				return 0;
			}
			else return (get32(addr) >> (addr & 2 ? 16 : 0)) & 0xffff;
		}
		[[nodiscard]]
		std::uint8_t read8(const std::uint32_t addr) noexcept {
			return (get32(addr) >> ((addr & 3) * 8)) & 0xff;
		}
		
		void write32(const std::uint32_t addr, const std::uint32_t data) noexcept {
			if (addr & 3) {
				// TODO: Implement: unaligned memory write
			}
			else set32(addr, data);
		}
		void write16(const std::uint32_t addr, const std::uint16_t data) noexcept {
			if (addr & 1) {
				// TODO: Implement: unaligned memory write
			}
			else {
				std::uint32_t tmp = get32(addr);
				tmp &= 0xffff << ((addr & 1) * 16);
				tmp |= std::uint32_t{data} << ((addr & 1) * 16);
				set32(addr, tmp);
			}
		}
		void write8(const std::uint32_t addr, const std::uint8_t data) noexcept {
			std::uint32_t tmp = get32(addr);
			tmp &= 0xff << ((addr & 3) * 8);
			tmp |= std::uint32_t{data} << ((addr & 3) * 8);
			set32(addr, tmp);
		}
		
		[[nodiscard]]
		std::uint32_t get32(const std::uint32_t addr) noexcept {
			return bus.read(addr);
		}
		void set32(const std::uint32_t addr, const std::uint32_t data) noexcept {
			bus.write(addr, data);
		}
		
		void load(const std::initializer_list<std::uint32_t>& l) {
			for (std::uint32_t i = 0; i < l.size(); ++i)
				set32(i << 2, l.begin()[i]);
		}
		void load(const std::initializer_list<std::uint8_t>& l) {
			for (std::uint32_t i = 0; i < l.size(); ++i)
				write8(i, l.begin()[i]);
		}
		bool load_file(const char* filename) {
			FILE* file = std::fopen(filename, "rb");
			if (!file) return false;
			std::fseek(file, 0, SEEK_END);
			const std::uint32_t sz = std::ftell(file);
			std::fseek(file, 0, SEEK_SET);
			std::uint32_t data;
			for (std::uint32_t i = 0; i < sz; ++i) {
				std::fread(&data, 1, sizeof(data), file);
				bus.write(i * sizeof(data), data);
			}
			std::fclose(file);
			return true;
		}
	};
}

#endif /* FILE_MEM_HPP */
