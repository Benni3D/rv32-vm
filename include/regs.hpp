#ifndef FILE_REGS_HPP
#define FILE_REGS_HPP
#include <cstdint>
#include <memory>
#include <array>

namespace rv32::detail {
	class RegisterBase {
	public:
		virtual ~RegisterBase() = default;
		
		constexpr RegisterBase& operator=(const std::uint32_t val) noexcept {
			write(val);
			return *this;
		}
		[[nodiscard]]
		constexpr operator std::uint32_t() const noexcept { return read(); }
	
		constexpr virtual void write(std::uint32_t) noexcept = 0;
		[[nodiscard]] constexpr virtual std::uint32_t read() const noexcept = 0;
	};
	class ZeroRegister : public RegisterBase {
	public:
		constexpr void write(const std::uint32_t) noexcept override {}
		constexpr std::uint32_t read() const noexcept override { return 0; }
	};
	class Register : public RegisterBase {
	private:
		std::uint32_t value{};
	public:
		constexpr void write(const std::uint32_t v) noexcept override { value = v; }
		constexpr std::uint32_t read() const noexcept override { return value; }
	};
}

namespace rv32 {
	constexpr std::array<std::string_view, 32> reg_names = {
		"x0", "ra",
		"sp", "gp",
		"tp", "t0",
		"t1", "t2",
		"fp", "s1",
		"a0", "a1",
		"a2", "a3",
		"a4", "a5",
		"a6", "a7",
		"s2", "s3",
		"s4", "s5",
		"s6", "s7",
		"s8", "s9",
		"s10", "s11",
		"t3", "t4",
		"t5", "t6"
	};
	
	class RegisterFile {
	private:
		std::array<std::unique_ptr<detail::RegisterBase>, 32> regs{};
	public:
		RegisterFile() {
			regs[0] = std::make_unique<detail::ZeroRegister>();
			for (std::size_t i = 1; i < regs.size(); ++i)
				regs[i] = std::make_unique<detail::Register>();
		}
		RegisterFile(const RegisterFile&) = delete;
		RegisterFile(RegisterFile&&) = default;
		~RegisterFile() = default;
		
		RegisterFile& operator=(const RegisterFile&) = delete;
		RegisterFile& operator=(RegisterFile&&) = default;
		
		[[nodiscard]]
		auto& operator[](const std::size_t i) { return *(regs[i]); }
		[[nodiscard]]
		const auto& operator[](const std::size_t i) const { return *regs[i]; }
		
		void reset() noexcept {
			for (std::size_t i = 1; i < regs.size(); ++i)
				*regs[i] = 0;
		}
	};
}

#endif /* FILE_REGS_HPP */
