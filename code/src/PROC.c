#include <stdio.h>	/* fprintf(), printf() */
#include <stdlib.h>	/* atoi() */
#include <stdint.h>	/* uint32_t */

#include "RegFile.h"
#include "Syscall.h"
#include "utils/heap.h"
#include "elf_reader/elf_reader.h"

int main(int argc, char * argv[]) {
	//hi there this is a  comment
	//MAKE A CHANGE
	printf("omar/n")
	
	/*
	 * This variable will store the maximum
	 * number of instructions to run before
	 * forcibly terminating the program. It
	 * is set via a command line argument.
	 */
	uint32_t MaxInstructions;

	/*
	 * This variable will store the address
	 * of the next instruction to be fetched
	 * from the instruction memory.
	 */
	uint32_t ProgramCounter;

	/*
	 * This variable will store the instruction
	 * once it is fetched from instruction memory.
	 */
	uint32_t CurrentInstruction;

	//IF THE USER HAS NOT SPECIFIED ENOUGH COMMAND LINE ARUGMENTS
	if(argc < 3){

		//PRINT ERROR AND TERMINATE
		fprintf(stderr, "ERROR: Input argument missing!\n");
		fprintf(stderr, "Expected: file-name, max-instructions\n");
		return -1;

	}

     	//CONVERT MAX INSTRUCTIONS FROM STRING TO INTEGER	
	MaxInstructions = atoi(argv[2]);	

	//Open file pointers & initialize Heap & Regsiters
	initHeap();
	initFDT();
	initRegFile(0);

	//LOAD ELF FILE INTO MEMORY AND STORE EXIT STATUS
	int status = LoadOSMemory(argv[1]);

	//IF LOADING FILE RETURNED NEGATIVE EXIT STATUS
	if(status < 0){ 
		
		//PRINT ERROR AND TERMINATE
		fprintf(stderr, "ERROR: Unable to open file at %s!\n", argv[1]);
		return status; 
	
	}

	printf("\n ----- BOOT Sequence ----- \n");
	printf("Initializing sp=0x%08x; gp=0x%08x; start=0x%08x\n", exec.GSP, exec.GP, exec.GPC_START);

	RegFile[28] = exec.GP;
	RegFile[29] = exec.GSP;
	RegFile[31] = exec.GPC_START;

	printRegFile();

	printf("\n ----- Execute Program ----- \n");
	printf("Max Instruction to run = %d \n",MaxInstructions);
	fflush(stdout);
	ProgramCounter = exec.GPC_START;
	
	/***************************/
	/* ADD YOUR VARIABLES HERE */
	/***************************/
	uint8_t opcode;
	uint8_t source_register;	
	uint8_t target_register;
	uint8_t destination_register;
	uint8_t shift_ammount;
	uint8_t function_bits;
	uint16_t target_register;
	uint16_t immediate;
	uint32_t address;
	int i;
	for(i = 0; i < MaxInstructions; i++) {

		//FETCH THE INSTRUCTION AT 'ProgramCounter'		
		CurrentInstruction = readWord(ProgramCounter,false);

		//PRINT CONTENTS OF THE REGISTER FILE	
		printRegFile();
		
		/********************************/
		/* ADD YOUR IMPLEMENTATION HERE */
		/********************************/
        
	        printf("currentInstruction: %08x\n", CurrentInstruction);
		opcode = (CurrentInsturction & 0xFC000000) >>26;

	   if(opcode == 0){
		source_register      =(CurrentInsturction & 0x03E00000) >> 21; 
		target_register      =(CurrentInsturction & 0x001F0000) >> 16;
		destination_register =(CurrentInsturction & 0x0000F800) >> 11;
		shift_ammount 		 =(CurrentInsturction & 0x000007C0) >> 6;
		function_bits 		 =(CurrentInsturction & 0x0000003F);
        rsValue              =RegFile[source_register];
        rtValue              =RegFile[target_register];
		}else{
		source_register      =(CurrentInsturction & 0x03E00000) >> 21;
		target_register      =(CurrentInsturction & 0x001F0000) >> 16;
	    immediate            =(CurrentInsturction & 0x0000FFFF);
		address              =(CurrentInsturction &0x03FFFFFF);
		rsValue              =RegFile[source_register];
        rtValue              =RegFile[target_register];
			}


		printf("opcode: %u\n", opcode);

		if(opcode == 0){
			switch (function_bits){
				case 32:{
					RegFile[destination_register]= rsValue + rtValue;
					printf("%d\n",RegFile[destination_register]);
					break;
				}
				case 33;{
					RegFile[destination_register]= rsValue + rtValue;
					printf("%d\n",RegFile[destination_register]);
					break;
				}
				case 34;{
					RegFile[destination_register]= rsValue - rtValue;
					printf("%d\n",RegFile[destination_register]);
					break;
				}
				case 35;{
					RegFile[destination_register]= rsValue - rtValue;
					printf("%d\n",RegFile[destination_register]);
					break;
				}
				case 36;{
					RegFile[destination_register]= source_register & target_register;
					printf("%d\n",RegFile[destination_register]);
					break;
				}
				case 37;{
					RegFile[destination_register]= source_register | target_register;
					printf("%d\n",RegFile[destination_register]);
					break;
				}
				case 38;{//XOR
					RegFile[destination_register]= source_register ^ target_register;
					printf("%d\n",RegFile[destination_register]);
					break;
				}
				case 39;{//NOR
					RegFile[destination_register]= !(source_register | target_register);
					printf("%d\n",RegFile[destination_register]);
					break;
				case 42;{//SLT
					if(rsValue<rtValue){
						RegFile[destination_register]= true;
						printf("%d\n",RegFile[destination_register]);
					}
					else{
						RegFile[destination_register]= false;
						printf("%d\n",RegFile[destination_register]);
					}
					break;
				case 43;{//SLTU
					if(rsValue<rtValue)
						RegFile[destination_register]= true;
						printf("%d\n",RegFile[destination_register]);
					else
						RegFile[destination_register]= false;
						printf("%d\n",RegFile[destination_register]);
					break;
				}
				
			}

			pintf("ADDIU\n");
			
			RegFile[target_register] = RegFile[source_register] + ......

			ProgramCounter = ProgramCounter +4;
		}else if(opcode== ....){
				
		//xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx
		//0000 0011 1111 1111 1111 1111 1111 1111
		//0x03FFFFFF 

		//0000 0000 0000 0000 0000 0111 1100 0000 - shift_amount

		//1111 1100 0000 0000 0000 00000 0000 0000
		//F    C    0    0    0    0     0    0
		//0    3    E (mask 0x03E00000)
		//0    0    1	 F (mask 0x001F0000)
		//0	   0    0    0    F    8     0    0 ( mask 0x0000F800)
		//Mask; 0xFC000000
		//xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx
		//1111 0101 0000 1111 0000 1101 1010 00111

	}   

	//Close file pointers & free allocated Memory
	closeFDT();
	CleanUp();

	return 0;
	//testing 
