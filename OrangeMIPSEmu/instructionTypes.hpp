#ifndef INSTRUCTION_TYPES_HPP
#define INSTRUCTION_TYPES_HPP

enum InstructionTypes
{
	eWord               = 0,
	eAdd                = 1,
	eSubtract           = 2,
	eMultiply           = 3,
	eMultiplyU          = 4,
	eDivide             = 5,
	eDivideU            = 6,
	eMoveFromHigh       = 7,
	eMoveFromLow        = 8,
	eLoadAndSkip        = 9,
	eLoadWord           = 11,
	eStoreWord          = 12,
	eSetLessThan        = 13,
	eSetLessThanU       = 14,
	eBranchOnEqual      = 15,
	eBranchOnNotEqual   = 16,
	eJumpRegister       = 17,
	eJumpRegisterLink   = 18
};

#endif
