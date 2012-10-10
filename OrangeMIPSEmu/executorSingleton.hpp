#ifndef EXECUTOR_SINGLETON_HPP
#define EXECUTOR_SINGLETON_HPP

#include <stdio.h>
#include "memorySingleton.hpp"
#include "registerSingleton.hpp"
#include "instructionTypes.hpp"

///A singleton class for managing the execution of instructions
class ExecutorSingleton
{
    
private:
    ///Constructs an Executor singleton
    ExecutorSingleton()
    {
        mLoad = 0;
        mDone = false;
    }
    
    ///The copy constructor- shouldn't be called
    ExecutorSingleton(ExecutorSingleton const& );
    
    ///The setter operator - shouldn't be called
    void operator=(ExecutorSingleton const&);
    
    ///The program counter
    int mProgramCounter;
    
    ///The hi/lo bit as a long
    long mHiLo;
    
    ///The instructions in the program
    int* mInstructions;
    
    ///The load and skip flag
    int mLoad;
    
    ///The kill process flag
    bool mDone;
    
public:
    
    ///The singleton accessor
    static ExecutorSingleton& sharedExecutor()
    {
        static ExecutorSingleton instance;
        
        return instance;
    }
    
    ///Checks if execution should proceed
    bool shouldContinue()
    {
        return !mDone;
    }
    
    ///Load instructions
    void loadInstructions(int * pInstructions)
    {
        mInstructions = pInstructions;
        printf("\n\n");
    }
    
    ///Process the next instruction
    void process()
    {
        if (mProgramCounter == 0x8123456c)
        {
            mDone = true;
            return;
        }
        
        int instruction = mInstructions[mProgramCounter/4];
        //printf("%x\n",instruction);
        
        if(mLoad)
        {
            if(DEBUG)
            {
                printf("    Loading word %i\n",mLoad);
            }
            
            printf("-- [%i] .word %i\n", mProgramCounter, instruction);
            
            RegisterSingleton::sharedRegisters().setRegister(mLoad, instruction);
            mProgramCounter += 4;
            mLoad = 0;
            
            return;
        }
        
        int type 	= getType(instruction);
        mProgramCounter += 4;
        
        switch(type)
        {
            case eAdd:
            {
                int s = instruction >> 21;
                s = s & 0x0000001f;
                int t = instruction >> 16;
                t = t & 0x0000001f;
                int d = instruction >> 11;
                d = d & 0x0000001f;
                
                printf("-- [%i] add $%i, $%i, $%i\n", mProgramCounter-4, d, s, t);
                executeAdd(d,s,t);
                break;
            }
                
            case eSubtract:
            {
                int s = instruction >> 21;
                s = s & 0x0000001f;
                int t = instruction >> 16;
                t = t & 0x0000001f;
                int d = instruction >> 11;
                d = d & 0x0000001f;
                
                printf("-- [%i] sub $%i, $%i, $%i\n", mProgramCounter-4, d, s, t);
                executeSubtract(d,s,t);
                break;
            }
                
            case eMultiply:
            {
                int s = instruction >> 21;
                s = s & 0x0000001f;
                int t = instruction >> 16;
                t = t & 0x0000001f;

                printf("-- [%i] mult $%i, $%i\n", mProgramCounter-4, s, t);
                executeMultiply(s,t);
                break;
            }
                
            case eMultiplyU:
            {
                int s = instruction >> 21;
                s = s & 0x0000001f;
                int t = instruction >> 16;
                t = t & 0x0000001f;
                
                printf("-- [%i] multu $%i, $%i\n", mProgramCounter-4, s, t);
                executeMultiplyU(s,t);
                break;
            }
                
            case eDivide:
            {
                int s = instruction >> 21;
                s = s & 0x0000001f;
                int t = instruction >> 16;
                t = t & 0x0000001f;
                
                printf("-- [%i] div $%i, $%i\n", mProgramCounter-4, s, t);
                executeDivide(s,t);
                break;
            }
				
            case eDivideU:
            {
                int s = instruction >> 21;
                s = s & 0x0000001f;
                int t = instruction >> 16;
                t = t & 0x0000001f;
                
                printf("-- [%i] divu $%i, $%i\n", mProgramCounter-4, s, t);
                executeDivideU(s,t);
                break;
            }
                
            case eMoveFromHigh:
            {
                int d = instruction >> 11;
                
                printf("-- [%i] mfhi $%i\n", mProgramCounter-4, d);
                executeMoveFromHigh(d);
                break;
            }
                
            case eMoveFromLow:
            {
                int d = instruction >> 11;
                
                printf("-- [%i] mflo $%i\n", mProgramCounter-4, d);
                executeMoveFromLow(d);
                break;
            }
                
            case eLoadAndSkip:
            {
                int d = instruction >> 11;
                
                printf("-- [%i] lis $%i\n", mProgramCounter-4, d);
                executeLoadAndSkip(d);
                break;
            }
                
            case eLoadWord:
            {
                int s = instruction >> 21;
                s = s & 0x0000001f;
                int t = instruction >> 16;
                t = t & 0x0000001f;
                int i = instruction & 0x0000ffff;
                
                printf("-- [%i] lw $%i, %i($%i)\n", mProgramCounter-4, t, i, s);
                executeLoadWord(s, t, i);
                break;
            }
                
            case eStoreWord:
            {
                int s = instruction >> 21;
                s = s & 0x0000001f;
                int t = instruction >> 16;
                t = t & 0x0000001f;
                int i = instruction & 0x0000ffff;
                
                printf("-- [%i] sw $%i, %i($%i)\n", mProgramCounter-4, t, i, s);
                executeStoreWord(s, t, i);
                break;
            }
            
            case eSetLessThan:
            {
                int s = instruction >> 21;
                s = s & 0x0000001f;
                int t = instruction >> 16;
                t = t & 0x0000001f;
                int d = instruction >> 11;
                d = d & 0x0000001f;
                
                printf("-- [%i] slt $%i, $%i, $%i\n", mProgramCounter-4, d, s, t);
                
                executeSetLessThan(d, s, t);
                break;
            }
                
            case eSetLessThanU:
            {
                int s = instruction >> 21;
                s = s & 0x0000001f;
                int t = instruction >> 16;
                t = t & 0x0000001f;
                int d = instruction >> 11;
                d = d & 0x0000001f;
                
                 printf("-- [%i] sltu $%i, $%i, $%i\n", mProgramCounter-4, d, s, t);
                
                executeSetLessThanU(d, s, t);
                break;
            }
            
            case eBranchOnEqual:
            {
                int s = instruction >> 21;
                s = s & 0x0000001f;
                int t = instruction >> 16;
                t = t & 0x0000001f;
                int i = instruction & 0x0000ffff;
                if ((instruction & 0x00008000) == 0x00008000)
                {
                    i = i | 0xffff0000;
                }
                
                printf("-- [%i] beq $%i, $%i, %i\n", mProgramCounter-4, s, t, i);
                executeBranchOnEquals(s, t, i);
                break;
            }
                
            case eBranchOnNotEqual:
            {
                int s = instruction >> 21;
                s = s & 0x0000001f;
                int t = instruction >> 16;
                t = t & 0x0000001f;
                int i = instruction & 0x0000ffff;
                
                if ((instruction & 0x00008000) == 0x00008000)
                {
                    i = i | 0xffff0000;
                }
                
                printf("-- [%i] bne $%i, $%i, %i\n", mProgramCounter-4, s, t, i);
                executeBranchOnNotEquals(s, t, i);
                break;
            }
                
            case eJumpRegister:
            {
                int s = instruction >> 21;
                s = s & 0x0000001f;
                
                printf("-- [%i] jr $%i\n", mProgramCounter-4, s);
                executeJumpRegister(s);
                break;
            }
                
            case eJumpRegisterLink:
            {
                int s = instruction >> 21;
                s = s & 0x0000001f;
                
                printf("-- [%i] jalr $%i\n", mProgramCounter-4, s);
                executeJumpAndLink(s);
                break;
            }
                
            default:
            {
                break;
            }
                
        }
        
    }
    
    ///Determine an instruction type
    int getType(int pInstruction)
    {
        int mask;
        
        mask = 0xac000000;
        if((pInstruction & mask) == mask)
            return eStoreWord;
        
        mask = 0x8c000000;
        if((pInstruction & mask) == mask)
            return eLoadWord;
        
        
        mask = 0x14000000;
        if((pInstruction & mask) == mask)
            return eBranchOnNotEqual;
        
        mask = 0x10000000;
        if((pInstruction & mask) == mask)
            return eBranchOnEqual;
        
        
        mask = 0x0000002d;
        if((pInstruction & mask) == mask)
            return eSetLessThanU;
        
        mask = 0x0000002a;
        if((pInstruction & mask) == mask)
            return eSetLessThan;
        
        mask = 0x00000022;
        if((pInstruction & mask) == mask)
            return eSubtract;
        
        mask = 0x00000020;
        if((pInstruction & mask) == mask)
            return eAdd;
        
        
    
        mask = 0x0000001b;
        if((pInstruction & mask) == mask)
            return eDivideU;
        
        mask = 0x0000001a;
        if((pInstruction & mask) == mask)
            return eDivide;
        
        mask = 0x00000019;
        if((pInstruction & mask) == mask)
            return eMultiplyU;
        
        mask = 0x00000018;
        if((pInstruction & mask) == mask)
            return eMultiply;
        
        mask = 0x00000014;
        if((pInstruction & mask) == mask)
            return eLoadAndSkip;
        
        mask = 0x00000012;
        if((pInstruction & mask) == mask)
            return eMoveFromLow;
        
        mask = 0x00000010;
        if((pInstruction & mask) == mask)
            return eMoveFromHigh;
        
        
        mask = 0x00000009;
        if((pInstruction & mask) == mask)
            return eJumpRegisterLink;
        
        mask = 0x00000008;
        if((pInstruction & mask) == mask)
            return eJumpRegister;
        
        
        return -1;
    }
    
    ///Execute an Add operator
    void executeAdd(int regD, int regS, int regT)
    {
        if(DEBUG)
        {
            assert(regD >= 0 && regD < 32);
            assert(regS >= 0 && regS < 32);
            assert(regT >= 0 && regT < 32);
            
            printf("    Adding register $%i to $%i and storing in $%i\n",regS, regT, regD);
        }
        
        
        int s = RegisterSingleton::sharedRegisters().getRegister(regS);
        int t = RegisterSingleton::sharedRegisters().getRegister(regT);
        
        int d = s + t;
        
        if(DEBUG)
        {
            printf("    Add result was %i.\n",d);
        }
        
        RegisterSingleton::sharedRegisters().setRegister(regD, d);
    }
    
    ///Execute an Subtract operator
    void executeSubtract(int regD, int regS, int regT)
    {
        if(DEBUG)
        {
            assert(regD >= 0 && regD < 32);
            assert(regS >= 0 && regS < 32);
            assert(regT >= 0 && regT < 32);
            
            printf("    Subtracting register $%i from $%i and storing in $%i\n",regT, regS, regD);
        }
        
        int s = RegisterSingleton::sharedRegisters().getRegister(regS);
        int t = RegisterSingleton::sharedRegisters().getRegister(regT);
        
        int d = s -  t;
        
        if(DEBUG)
        {
            printf("    Subtract result was %i.\n",d);
        }
        
        RegisterSingleton::sharedRegisters().setRegister(regD, d);
    }
    
    ///Execute a Multply operator
    void executeMultiply(int regS, int regT)
    {
        if(DEBUG)
        {
            assert(regS >= 0 && regS < 32);
            assert(regT >= 0 && regT < 32);
            
            printf("    Multiplying $%i by $%i.\n",regS, regT);
        }
        
        
        int s = RegisterSingleton::sharedRegisters().getRegister(regS);
        int t = RegisterSingleton::sharedRegisters().getRegister(regT);
        
        mHiLo = s * t;
        
        if (DEBUG)
        {
            printf("    Multiply result was %li",mHiLo);
        }
    }
    
    ///Execute a Multply operator
    void executeMultiplyU(int regS, int regT)
    {
        if(DEBUG)
        {
            assert(regS >= 0 && regS < 32);
            assert(regT >= 0 && regT < 32);
            
            printf("    Multiplying Unsigned $%i by $%i.\n",regS, regT);
        }
        
        
        unsigned int s = RegisterSingleton::sharedRegisters().getRegister(regS);
        unsigned int t = RegisterSingleton::sharedRegisters().getRegister(regT);
        
        mHiLo = s * t;
        
        if (DEBUG)
        {
            printf("    Multiply result was %liu\n",mHiLo);
        }
    }
    
    ///Execute a Divide operator
    void executeDivide(int regS, int regT)
    {
        if(DEBUG)
        {
            assert(regS >= 0 && regS < 32);
            assert(regT >= 0 && regT < 32);
            
            printf("    Dividing register $%i by $%i.\n", regS, regT);
        }
        
        
        int s = RegisterSingleton::sharedRegisters().getRegister(regS);
        int t = RegisterSingleton::sharedRegisters().getRegister(regT);
        
        long lo = s / t;
        long hi = s % t;
        hi = hi << 32;
        
        mHiLo = hi | lo;
        
        if (DEBUG)
        {
            printf("    Lo bit : %li\n",lo);
            printf("    Hi bit : %li\n", hi);
        }
    }
    
    ///Execute a Divide Unsigned operator
    void executeDivideU(int regS, int regT)
    {
        if(DEBUG)
        {
            assert(regS >= 0 && regS < 32);
            assert(regT >= 0 && regT < 32);
            
            printf("    Diving register $%i by $%i.\n", regS, regT);
        }
        
        
        unsigned int s = RegisterSingleton::sharedRegisters().getRegister(regS);
        unsigned int t = RegisterSingleton::sharedRegisters().getRegister(regT);
        
        long lo = s / t;
        long hi = s % t;
        hi = hi << 32;
        
        mHiLo = hi | lo;
        
        if (DEBUG)
        {
            printf("    Lo bit : %li\n",lo);
            printf("    Hi bit : %li\n", hi);
        }
    }
    
    ///Moves from the hi word
    void executeMoveFromHigh(int regD)
    {
        if(DEBUG)
        {
            assert(regD >= 0 && regD < 32);
            
            printf("    Moving from the hi bit to $%i\n",regD);
        }
        
        int d = (int)(mHiLo >> 32);
        
        if (DEBUG)
        {
            printf("    Hi bit is %i\n",d);
        }
        
        RegisterSingleton::sharedRegisters().setRegister(regD, d);
    }
    
    ///Moves from the lo word
    void executeMoveFromLow(int regD)
    {
        
        if(DEBUG)
        {
            assert(regD >= 0 && regD < 32);
            
            printf("    Moving from the lo bit to $%i\n",regD);
        }
        
        int d = (int)(mHiLo);
        
        if (DEBUG)
        {
            printf("    Lo bit is %i\n",d);
        }
        
        RegisterSingleton::sharedRegisters().setRegister(regD, d);
    }
    
    ///Execute a load and skip
    void executeLoadAndSkip(int regD)
    {
        if(DEBUG)
        {
            assert(regD >= 0 && regD < 32);
            
            printf("    Load and skip to register $%i\n",regD);
        }
        
        mLoad = regD;
    }
    
    ///Execute a load word
    void executeLoadWord(int regS, int regT, int immI)
    {
        if(DEBUG)
        {
            assert(regS >= 0 && regS < 32);
            assert(regT >= 0 && regT < 32);
            assert(immI % 4 == 0);
            
            printf("    Load word from $%i with offset %i to $%i\n",regS, immI, regT);
        }
        
        int address = (RegisterSingleton::sharedRegisters().getRegister(regS) + immI)/4;
        int t = MemorySingleton::sharedMemory().getMemory(address);
		
        RegisterSingleton::sharedRegisters().setRegister(regT, t);
    }
    
	
    ///Execut a store word
    void executeStoreWord(int regS, int regT, int immI)
    {
        if(DEBUG)
        {
            assert(regS >= 0 && regS < 32);
            assert(regT >= 0 && regT < 32);
            assert(immI % 4 == 0);
            
            printf("    Storing word from $%i with offset %i to $%i\n",regS, immI, regT);
        }
        
        int address = (RegisterSingleton::sharedRegisters().getRegister(regS) + immI)/4;
        int t	= RegisterSingleton::sharedRegisters().getRegister(regT);
        
        if (immI == 0xffff000c)
        {
            printf("OUTPUT: %c", (char)t);
        }
        
        if (DEBUG) {
            printf("    Word was %i\n",t);
        }
        
        MemorySingleton::sharedMemory().setMemory(address, t);
    }
    
    
    ///Execute a set less than
    void executeSetLessThan(int regD, int regS, int regT)
    {
        if(DEBUG)
        {
            assert(regS >= 0 && regS < 32);
            assert(regT >= 0 && regT < 32);
            assert(regD >= 0 && regD < 32);
            
            printf("    Setting $%i to 1 if $%i is less than $%i\n",regD, regS, regT);
        }
        
        int s = RegisterSingleton::sharedRegisters().getRegister(regS);
        int t = RegisterSingleton::sharedRegisters().getRegister(regT);
        
        int d = s < t ? 1 : 0;
        if(DEBUG)
        {
            printf("    Set $%i to %i\n",regD, d);
        }
        
        RegisterSingleton::sharedRegisters().setRegister(regD, d);
    }
    
    ///Execute a set less than unsigned
    void executeSetLessThanU(int regD, int regS, int regT)
    {
        if(DEBUG)
        {
            assert(regS >= 0 && regS < 32);
            assert(regT >= 0 && regT < 32);
            assert(regD >= 0 && regD < 32);
            
            printf("    Unsigned setting $%i to 1 if $%i is less than $%i\n",regD, regS, regT);
        }
        
        unsigned int s = RegisterSingleton::sharedRegisters().getRegister(regS);
        unsigned int t = RegisterSingleton::sharedRegisters().getRegister(regT);
        
        int d = s < t ? 1 : 0;
        if(DEBUG)
        {
            printf("    Set $%i to %i\n",regD, d);
        }
        
        
        RegisterSingleton::sharedRegisters().setRegister(regD, d);
    }
    
    
    ///Executes a branch on equal
    void executeBranchOnEquals(int regS, int regT, int immI)
    {
        if(DEBUG)
        {
            assert(regS >= 0 && regS < 32);
            assert(regT >= 0 && regT < 32);
                
            printf("    Branching by %i if $%i is equal to $%i\n",immI, regS, regT);
        }
        
        int s = RegisterSingleton::sharedRegisters().getRegister(regS);
        int t = RegisterSingleton::sharedRegisters().getRegister(regT);
        
        if(s == t)
        {
            if(DEBUG)
            {
                printf("    Branching\n");
            }
            
            mProgramCounter += immI * 4;
            
        }
        else if(DEBUG)
        {
            printf("    Not branching\n");
        }
    }
    
    ///Executes a branch on not equal
    void executeBranchOnNotEquals(int regS, int regT, int immI)
    {
        if(DEBUG)
        {
            assert(regS >= 0 && regS < 32);
            assert(regT >= 0 && regT < 32);
            
            printf("    Branching by %i if $%i is not equal to $%i\n",immI, regS, regT);
        }
        
        int s = RegisterSingleton::sharedRegisters().getRegister(regS);
        int t = RegisterSingleton::sharedRegisters().getRegister(regT);
        
        if(s != t)
        {
            if(DEBUG)
            {
                printf("    Branching\n");
            }
            
            mProgramCounter += immI * 4;
            
        }
        else if(DEBUG)
        {
            printf("    Not branching\n");
        }
    }
    
    ///Execute a jump register
    void executeJumpRegister(int regS)
    {
        if(DEBUG)
        {
            assert(regS >= 0 && regS < 32);
            
            printf("    Jumping to $%i\n",regS);
        }
        
        mProgramCounter = RegisterSingleton::sharedRegisters().getRegister(regS);
    }
    
    ///Execute a jump and link
    void executeJumpAndLink(int regS)
    {
        if(DEBUG)
        {
            assert(regS >= 0 && regS < 32);
            
            printf("    Jumping to and linking $%i\n",regS);
        }
        
        int s  = RegisterSingleton::sharedRegisters().getRegister(regS);
        RegisterSingleton::sharedRegisters().setRegister(31, s);
        mProgramCounter = s;
    }
};

#endif
