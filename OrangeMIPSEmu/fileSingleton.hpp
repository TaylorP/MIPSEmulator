#ifndef FILE_SINGLETON_HPP
#define FILE_SINGLETON_HPP

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <fstream>
#include "config.hpp"

using namespace std;

///A singleton for accessing and loading mips files
class FileSingleton
{
    
private:
    
    ///Constructs a file singleton
    FileSingleton()
    {
    }
    
    ///The copy constructor- shouldn't be called
    FileSingleton(FileSingleton const& );
    
    ///The setter operator - shouldn't be called
    void operator=(FileSingleton const&);
    
public:
    
    ///The singleton accessor
    static FileSingleton& sharedFileSystem()
    {
        static FileSingleton instance;
        
        return instance;
    }
    
    int* loadMipsFile()
    {
        ifstream file ("file.mips", ios::in|ios::binary|ios::ate);
        
        ifstream::pos_type size;
        char  * memblock;
        int   * retBlock;
        
        if (file.is_open())
        {
            size = file.tellg();
            memblock = new char [size];
            file.seekg (0, ios::beg);
            file.read (memblock, size);
            file.close();
            
            retBlock = (int*)malloc(sizeof(int) * size/4);
            
            for (int i = 0; i<size; i+=4)
            {
                retBlock[i/4] = (((unsigned char)memblock[i]) * 16777216u) + (((unsigned char)memblock[i+1]) * 65536) + (((unsigned char)memblock[i+2]) * 256u) + ((unsigned char)memblock[i+3]);
            }
            
            
            
            delete[] memblock;
        }
        
        return retBlock;
    }
    
};

#endif
