#ifndef REGISTER_SINGLETON_HPP
#define REGISTER_SINGLETON_HPP

#include <assert.h>
#include <stdio.h>
#include "config.hpp"

/// A singleton for maintain the 32 registers available on a MIPS cpu
class RegisterSingleton
{
	private:
		///Constructs a register singleton
		RegisterSingleton()
		{
			mRegisters[0] = 0;
			mRegisters[1] = 0x01000000;
            mRegisters[31] = 0x8123456c;
		}

		///The copy constructor- shouldn't be called
		RegisterSingleton(RegisterSingleton const& );

		///The setter operator - shouldn't be called
		void operator=(RegisterSingleton const&);

		///The 32 registers - each a 32 bit value
		int mRegisters[32];

	public:
	
		///The singleton accessor
		static RegisterSingleton& sharedRegisters()
		{
			static RegisterSingleton instance;	

			return instance;
		}

		///Sets the value of the register pRegister to pWord
		void setRegister(int pRegister, int pWord)
		{
			if(DEBUG)
			{
				printf("        Setting Register $%i to value %i\n",pRegister,pWord);
				assert(pRegister >= 0 && pRegister <32);
			}

			mRegisters[pRegister] = pWord;	
		}

		///Returns the value of the register pRegister
		int getRegister(int pRegister)
		{
			if(DEBUG)
			{
				printf("        Getting Register $%2i",pRegister);
				assert(pRegister >=0 && pRegister<32);
			}
			
			int reg = mRegisters[pRegister];
            
            if(DEBUG)
            {
                printf(":   0x%08x\n",reg);
            }
            
            return reg;
		}
};

#endif 
