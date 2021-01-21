#ifndef FILE_BUS_HPP
#define FILE_BUS_HPP
#include <stdexcept>
#include <utility>
#include <cstdint>
#include <vector>
#include <memory>

namespace rv32 {
	class BusDevice {
	public:
		const std::uint32_t size;
		constexpr explicit BusDevice(std::uint32_t size) : size(size) {
			if (size & 3) throw std::invalid_argument("(size & 3) != 0");
		}
		constexpr virtual ~BusDevice() = default;
		
		constexpr virtual void write8(std::uint32_t addr, std::uint8_t data) = 0;
		constexpr virtual void write16(std::uint32_t addr, std::uint16_t data) = 0;
		constexpr virtual void write32(std::uint32_t addr, std::uint32_t data) = 0;
		constexpr virtual std::uint8_t read8(std::uint32_t addr) = 0;
		constexpr virtual std::uint16_t read16(std::uint32_t addr) = 0;
		constexpr virtual std::uint32_t read32(std::uint32_t addr) = 0;
	};
	class Bus {
	private:
		struct Connection {
			std::unique_ptr<BusDevice> device;
			std::uint32_t addr;
			
			Connection(std::unique_ptr<BusDevice>&& dev, std::uint32_t addr)
				: device(std::move(dev)), addr(addr) {
				if (!device || (addr & 3)) throw std::invalid_argument("((device == nullptr) || ((addr & 3) != 0))");
			}
			
			[[nodiscard]]
			bool isMapped(const std::uint32_t a) const {
				if ((a & 3)) throw std::invalid_argument("(((a & 3) != 0) || ((n & 3) != 0))");
				return (a >= addr) && (a < (addr + device->size));
			}
			auto operator->() const { return device.get(); }
		};
		// Waiting for constexpr std::vector support.
		std::vector<Connection> devs{};
	public:
		void add(std::unique_ptr<BusDevice>&& dev, std::uint32_t addr) {
			devs.emplace_back(std::move(dev), addr);
		}
		void write8(std::uint32_t addr, std::uint8_t data) {
			for (const auto& c : devs) {
				if (c.isMapped(addr)) {
					c->write8(addr - c.addr, data);
					return;
				}
			}
		}
		void write16(std::uint32_t addr, std::uint16_t data) {
			if (addr & 1) return;
			for (const auto& c : devs) {
				if (c.isMapped(addr)) {
					c->write16(addr - c.addr, data);
					return;
				}
			}
		}
		void write32(std::uint32_t addr, std::uint32_t data) {
			if (addr & 3) return;
			for (const auto& c : devs) {
				if (c.isMapped(addr)) {
					c->write32(addr - c.addr, data);
					return;
				}
			}
		}
		std::uint8_t read8(std::uint32_t addr) {
			for (const auto& c : devs) {
				if (c.isMapped(addr)) return c->read8(addr - c.addr);
			}
			return 0;
		}
		std::uint16_t read16(std::uint32_t addr) {
			if (addr & 1) return 0;
			for (const auto& c : devs) {
				if (c.isMapped(addr)) return c->read16(addr - c.addr);
			}
			return 0;
		}
		std::uint32_t read32(std::uint32_t addr) {
			if (addr & 3) return 0;
			for (const auto& c : devs) {
				if (c.isMapped(addr)) return c->read32(addr - c.addr);
			}
			return 0;
		}
		bool load_file(const char* filename) {
			FILE* file = std::fopen(filename, "rb");
			if (!file) return false;
			std::fseek(file, 0, SEEK_END);
			const std::uint32_t sz = std::ftell(file);
			std::fseek(file, 0, SEEK_SET);
			std::uint8_t data;
			for (std::uint32_t i = 0; i < sz; ++i) {
				std::fread(&data, 1, sizeof(data), file);
				write8(i * sizeof(data), data);
			}
			std::fclose(file);
			return true;
		}
	};
}

#endif /* FILE_BUS_HPP */
