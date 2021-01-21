#ifndef FILE_INSTR_HPP
#define FILE_INSTR_HPP
#include <cstdint>

namespace rv32 {
	union Instruction {
		std::uint32_t raw           : 32;
		std::uint32_t opcode        :  7;
		struct {
			std::uint32_t opcode    : 7;
			std::uint32_t rd        : 5;
			std::uint32_t funct3    : 3;
			std::uint32_t rs1       : 5;
			std::uint32_t rs2       : 5;
			std::uint32_t funct7    : 7;
		} reg;
		struct {
			std::uint32_t opcode    : 7;
			std::uint32_t rd        : 5;
			std::uint32_t funct3    : 3;
			std::uint32_t rs1       : 5;
			std::uint32_t imm11_0   : 12;
			[[nodiscard]]
			constexpr std::uint32_t imm() const noexcept {
				const bool sign = imm11_0 >> 11;
				return (sign ? 0xffff'f000 : 0) | imm11_0;
			}
		} imm;
		struct {
			std::uint32_t opcode    : 7;
			std::uint32_t rd        : 5;
			std::uint32_t funct3    : 3;
			std::uint32_t rs1       : 5;
			std::uint32_t shamt     : 5;
			std::uint32_t funct7    : 7;
		} imm_shift;
		struct {
			std::uint32_t opcode    : 7;
			std::uint32_t rd        : 5;
			std::uint32_t imm31_12  : 20;
			[[nodiscard]]
			constexpr std::uint32_t imm() const noexcept {
				return imm31_12 << 12;
			}
		} upper_imm;
		struct {
			std::uint32_t opcode    : 7;
			std::uint32_t imm4_0    : 5;
			std::uint32_t funct3    : 3;
			std::uint32_t rs1       : 5;
			std::uint32_t rs2       : 5;
			std::uint32_t imm11_5   : 7;
			[[nodiscard]]
			constexpr std::uint32_t imm() const noexcept {
				const bool sign = imm11_5 >> 6;
				return (sign ? 0xffff'f000 : 0) | (imm11_5 << 5) | imm4_0;
			}
		} store;
		struct {
			std::uint32_t opcode    : 7;
			std::uint32_t imm11     : 1;
			std::uint32_t imm4_1    : 4;
			std::uint32_t funct3    : 3;
			std::uint32_t rs1       : 5;
			std::uint32_t rs2       : 5;
			std::uint32_t imm10_5   : 6;
			std::uint32_t imm12     : 1;
			[[nodiscard]]
			constexpr std::uint32_t imm() const noexcept {
				return (imm12 ? 0xffff'e000 : 0) | (imm12 << 12) | (imm11 << 11) | (imm10_5 << 5) | (imm4_1 << 1);
			}
		} branch;
		struct {
			std::uint32_t opcode    : 7;
			std::uint32_t rd        : 5;
			std::uint32_t imm19_12  : 8;
			std::uint32_t imm11     : 1;
			std::uint32_t imm10_1   : 10;
			std::uint32_t imm20     : 1;
			[[nodiscard]]
			constexpr std::uint32_t imm() const noexcept {
				return (imm20 ? 0xfff0'0000 : 0) | (imm20 << 20) | (imm19_12 << 12) | (imm11 << 11) | (imm10_1 << 1);
			}
		} jump;
	};
	static_assert(sizeof(Instruction) == 4);
}
#endif /* FILE_INSTR_HPP */
