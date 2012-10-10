#ifndef MEMORY_SINGLETON_HPP
#define MEMORY_SINGLETON_HPP

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "config.hpp"

///A singleton for managing the main RAM of the MIPS cpu
class MemorySingleton
{
    
private:
    ///Constructs a memory singleton. Allocates 64 words of memory
    ///right off the top
    MemorySingleton()
    {
        mTopHalfSize 	= 32;
        mBottomHalfSize = 32;
        
        mTopHalf	= (int*)malloc(sizeof(int) * mTopHalfSize);
        mBottomHalf 	= (int*)malloc(sizeof(int) * mBottomHalfSize);
    }
    
    ///The copy constructor- shouldn't be called
    MemorySingleton(MemorySingleton const& );
    
    ///The setter operator - shouldn't be called
    void operator=(MemorySingleton const&);
    
    
    ///The top half - addresses from 0...8388607
    int *mTopHalf;
    
    ///The amount of space allocated on the top half
    int mTopHalfSize;
    
    
    ///The bottom half - addresses from 8388608...16777215
    int *mBottomHalf;
    
    ///The amount of space allocated on the bottom half
    int mBottomHalfSize;
    
public:
    
    ///The singleton accessor
    static MemorySingleton& sharedMemory()
    {
        static MemorySingleton instance;
        
        return instance;
    }
    
    void setMemory(int pIndex, int pValue)
    {
        if(DEBUG)
        {
            printf("        Setting memory address %i to %i\n", pIndex, pValue);
            assert(pIndex >= 0 && pIndex < 16777216);
        }
        
        
        if(pIndex < 8388608)
        {
            if(pIndex >= mTopHalfSize)
            {
                
                while( mTopHalfSize < pIndex)
                    mTopHalfSize *= 2;
				
                mTopHalf = (int*)realloc(mTopHalf, sizeof(int) * mTopHalfSize);
            }
            
            mTopHalf[pIndex] = pValue;
        }
        else
        {
            pIndex = 16777215 - pIndex;
            
            if(pIndex >= mBottomHalfSize)
            {
                while( mBottomHalfSize < pIndex)
                    mBottomHalfSize *= 2;
				
                mBottomHalf = (int*)realloc(mBottomHalf, sizeof(int) * mBottomHalfSize);
            }
            
            mBottomHalf[pIndex] = pValue;
        }
    }
    
    int getMemory(int pIndex)
    {
        if(DEBUG)
        {
            printf("        Getting value of memory address %i.\n", pIndex);
            assert(pIndex >= 0 && pIndex < 16777216);
        }
        
        if(pIndex < 8388608)
        {
            if (DEBUG)
            {
                printf("        Accessing from the top half of memory\n");
                assert(pIndex < mTopHalfSize);
            }
            
            return mTopHalf[pIndex];
        }
        else
        {
            pIndex = 16777215 - pIndex;
            
            if(DEBUG)
            {
                printf("        Access from the bottom half of memory (real index %i\n).", pIndex);
                assert(pIndex < mBottomHalfSize);
            }
			
            return mBottomHalf[pIndex];
        }
    }
    
};

#endif
