#include <stdlib.h>

#include "executorSingleton.hpp"
#include "registerSingleton.hpp"
#include "memorySingleton.hpp"
#include "fileSingleton.hpp"

int main(int argc, char** argv)
{
    int* instructions = FileSingleton::sharedFileSystem().loadMipsFile();
        
    RegisterSingleton::sharedRegisters().setRegister(1, 0x00000000);
    RegisterSingleton::sharedRegisters().setRegister(2, 0x00000009);
    
    //-9 1 4 6 8 9 18 43 100
    MemorySingleton::sharedMemory().setMemory(0x00000000, 1);
    MemorySingleton::sharedMemory().setMemory(0x00000001, 9);
    MemorySingleton::sharedMemory().setMemory(0x00000002, 6);
    MemorySingleton::sharedMemory().setMemory(0x00000003, 4);
    MemorySingleton::sharedMemory().setMemory(0x00000004, 100);
    MemorySingleton::sharedMemory().setMemory(0x00000005, 43);
    MemorySingleton::sharedMemory().setMemory(0x00000006, 18);
    MemorySingleton::sharedMemory().setMemory(0x00000007, -9);
    MemorySingleton::sharedMemory().setMemory(0x00000008, 8);
    

    ExecutorSingleton::sharedExecutor().loadInstructions(instructions);
    
    while (ExecutorSingleton::sharedExecutor().shouldContinue())
    {
        ExecutorSingleton::sharedExecutor().process();
    }
    
    printf("%i",RegisterSingleton::sharedRegisters().getRegister(3));
    
    
	return 0;
}
