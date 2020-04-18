#include <stdio.h>	/* fprintf(), printf() *
#include <stdlib.h>	/* atoi() */
#include <stdint.h>	/* uint32_t */

#include "RegFile.h"
#include "Syscall.h"
#include "utils/heap.h"
#include "elf_reader/elf_reader.h"

int main(int argc, char * argv[]) {
	//hi there this is a  comment
	//MAKE A CHANGE
	
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
	uint8_t source_register;//should we use int instead of uint	
	uint8_t target_register;
	uint8_t destination_register;
	uint8_t shift_ammount;
	uint8_t function_bits;
	int16_t immediate;
	uint16_t unsinged_immediate;
	int32_t address;
	int16_t singed_address;
	int32_t rsValue;
	uint32_t unsigned_rsValue;
	int32_t rtValue;
	uint32_t unsigned_rtValue;
	int32_t word;
	uint32_t uword;
	int32_t data;
	uint32_t udata;
	int8_t byte;
	uint8_t ubyte;
	int8_t bflag=0;	
	uint32_t DelayCounter;
	

	int i;
	for(i = 0; i < MaxInstructions; i++) {
		//FETCH THE INSTRUCTION AT 'ProgramCounter'	
		int count =0; // A counter for LWL	
		if(bflag==1){
			ProgramCounter=DelayCounter;
			bflag=0;

		}
		CurrentInstruction = readWord(ProgramCounter,false);

		//PRINT CONTENTS OF THE REGISTER FILE	
		printRegFile();
		
		/********************************/
		/* ADD YOUR IMPLEMENTATION HERE */
		/********************************/
        
	    printf("currentInstruction: %08x\n", CurrentInstruction);
		opcode = (CurrentInstruction & 0xFC000000) >>26;

	   if(opcode == 0){
		source_register      =(CurrentInstruction & 0x03E00000) >> 21; 
		target_register      =(CurrentInstruction & 0x001F0000) >> 16;
		destination_register =(CurrentInstruction & 0x0000F800) >> 11;
		shift_ammount 		 =(CurrentInstruction & 0x000007C0) >> 6;
		function_bits 		 =(CurrentInstruction & 0x0000003F);
        rsValue              =RegFile[source_register];
        rtValue              =RegFile[target_register];
		}else{
		source_register      =(CurrentInstruction & 0x03E00000) >> 21;
		target_register      =(CurrentInstruction & 0x001F0000) >> 16;
	    immediate            =(CurrentInstruction & 0x0000FFFF);
	    unsinged_immediate   =(CurrentInstruction & 0x0000FFFF);//unsigned immidiate 
		address              =(CurrentInstruction & 0x03FFFFFF);
		rsValue              =RegFile[source_register];
        rtValue              =RegFile[target_register];
			}


		printf("opcode: %u\n", opcode);

		

		if(opcode == 0){
			switch (function_bits){

				case 32:{//ADD
					if(destination_register==0){
						break;
					}
					RegFile[destination_register]= rsValue + rtValue;
					printf("%d\n",RegFile[destination_register]);
					break;
				}
				case 33:{//addu
					if(destination_register==0){
						break;
					}
					unsigned_rtValue =(uint32_t)rtValue;//casting the rs and rt values to unsiged 
					unsigned_rsValue =(uint32_t)rsValue;
					RegFile[destination_register]= unsigned_rsValue + unsigned_rtValue;
					printf("%d\n",RegFile[destination_register]);
					break;
				}
				case 34:{//SUB
					if(destination_register==0){
						break;
					}
					RegFile[destination_register]= rsValue - rtValue;
					printf("%d\n",RegFile[destination_register]);
					break;
				}
				case 35:{//subu
					if(destination_register==0){
						break;
					}
					unsigned_rtValue =(uint32_t)rtValue;//casting the rs and rt values to unsiged 
					unsigned_rsValue =(uint32_t)rsValue;
					RegFile[destination_register]= unsigned_rsValue - unsigned_rtValue;
					printf("%d\n",RegFile[destination_register]);
					break;
				}
				case 36:{//AND
					if(destination_register==0){
						break;
					}
					RegFile[destination_register]= rsValue & rtValue;
					printf("%d\n",RegFile[destination_register]);
					break;
				}
				case 37:{//OR
					if(destination_register==0){
						break;
					}
					RegFile[destination_register]= rsValue | rtValue;
					printf("%d\n",RegFile[destination_register]);
					break;
				}
				case 38:{//XOR
					if(destination_register==0){
						break;
					}
					RegFile[destination_register]= rsValue ^ rtValue;
					printf("%d\n",RegFile[destination_register]);
					break;
				}
				case 39:{//NOR
					if(destination_register==0){
						break;
					}
					RegFile[destination_register]= !(rsValue | rtValue);
					printf("%d\n",RegFile[destination_register]);
					break;
				}
				case 42:{//SLT
					if(destination_register==0){
						break;
					}
					if(rsValue<rtValue){
						RegFile[destination_register]= true;
						printf("%d\n",RegFile[destination_register]);
					}
					else{
						RegFile[destination_register]= false;
						printf("%d\n",RegFile[destination_register]);
					}
					break;
				}
				case 43:{//SLTU
					if(destination_register==0){
						break;
					}
					unsigned_rtValue =(uint32_t)rtValue;//casting the rs and rt values to unsiged 
					unsigned_rsValue =(uint32_t)rsValue;
					if(unsigned_rsValue<unsigned_rtValue){
						RegFile[destination_register]= true;
						printf("%d\n",RegFile[destination_register]);
					}
					else{
						RegFile[destination_register]= false;
						printf("%d\n",RegFile[destination_register]);
					}
					break;
				}
				case 0:{//SLL
					if(destination_register==0){
						break;
					}
					RegFile[destination_register]= rtValue <<shift_ammount;
					printf("5d\n", RegFile[destination_register]);
					break;
					
				}
				case 2:{//SRL
					if(destination_register==0){
						break;
					}
					unsigned_rtValue =(uint32_t)rtValue;
					RegFile[destination_register]= unsigned_rtValue >> shift_ammount;
					printf("5d\n", RegFile[destination_register]);
					break;
				}
				case 3:{//SRA
					if(destination_register==0){
						break;
					}
					RegFile[destination_register]=(signed)rtValue>>shift_ammount;
					printf("%x\n",RegFile[destination_register]);
					break;
				}
				case 4:{//SLLV
					if(destination_register==0){
						break;
					}
					RegFile[destination_register]=rtValue<<rsValue;
					printf("%x\n",RegFile[destination_register]);
					break;
				}
				case 6:{//SRLV
					if(destination_register==0){
						break;
					}
					RegFile[destination_register]=rtValue>>rsValue;
					printf("%x\n",RegFile[destination_register]);
					break;
				}
				case 7:{//SRAV
					if(destination_register==0){
						break;
					}
					RegFile[destination_register]=(signed)rtValue>>rsValue;
					printf("%x\n",RegFile[destination_register]);
					break;
				}
				case 12:{
					printf("SYSCALL inst\n");
					SyscallExe(RegFile[2]);
				}
				case 16:{//MFHI
					RegFile[destination_register]=RegFile[32];
					printf("%d\n",RegFile[destination_register]);
					break;
				}
				case 17:{//MTHI
					RegFile[32]=rsValue;
					printf("%d\n",RegFile[32]);
					break;
				}
				case 18:{//MFLO
					RegFile[destination_register]=RegFile[33];
					printf("%d\n",RegFile[destination_register]);
					break;
				}
				case 19:{//MTLO
					RegFile[33]=rsValue;
					printf("%d\n",RegFile[33]);
					break;
				}
				case 24:{//MULT
					int64_t result = rsValue*rtValue;
					RegFile[32]= result >>32;
					RegFile[33]=(int32_t)result;
					printf("%d\n",RegFile[32]);
					printf("%d\n",RegFile[33]);
					break;
				}
				case 25:{//MULTU
					unsigned_rtValue =(uint32_t)rtValue;//casting the rs and rt values to unsiged 
					unsigned_rsValue =(uint32_t)rsValue;
					int64_t result = unsigned_rsValue*unsigned_rtValue;
					RegFile[32]= result >>32;
					RegFile[33]=(int32_t)result;
					printf("%d\n",RegFile[32]);
					printf("%d\n",RegFile[33]);
					break;				
				}
				case 26:{//DIV
					RegFile[32]=rsValue%rtValue;
					RegFile[33]=rsValue/rtValue;
					printf("%d\n",RegFile[32]);
					printf("%d\n",RegFile[33]);
					break;
				}
				case 27:{//DIVU
					unsigned_rtValue =(uint32_t)rtValue;//casting the rs and rt values to unsiged 
					unsigned_rsValue =(uint32_t)rsValue;
					RegFile[32]=unsigned_rsValue%unsigned_rtValue;
					RegFile[33]=unsigned_rsValue/unsigned_rtValue;
					printf("%d\n",RegFile[32]);
					printf("%d\n",RegFile[33]);
					break;
				}
				case 8:{
					//JR
					bflag =1;
					DelayCounter = rsValue;//DelayCounter is the PC that the Jumb reigster goes to.
					break;
				}
				case 9:{
					//JALR
					bflag =1;
					DelayCounter = RegFile[source_register];
					 //Place the return address link in GPR rd
					RegFile[31] = ProgramCounter +8;
					//ProgramCounter = RegFile[destination_register];
					
					break;
				}
			}
		}
			else if(opcode==8){//ADDI
			int32_t cas_immediate =(int32_t)immediate ; 
			RegFile[target_register]= rsValue + cas_immediate;
			printf("%d\n",RegFile[target_register]);
			}
			else if(opcode==9){//ADDUI
			unsigned_rsValue =(uint32_t)rsValue;
			int32_t cas_immediate =(int32_t)immediate; 
			RegFile[target_register]= rsValue + cas_immediate;
			printf("%d\n",RegFile[target_register]);
			}		
			else if (opcode==10){//SLTI
				int32_t cas_immediate =(int32_t)immediate ; 
				if(rsValue<cas_immediate){
						RegFile[destination_register]= true;
						printf("%d\n",RegFile[destination_register]);
					}
					else{
						RegFile[destination_register]= false;
						printf("%d\n",RegFile[destination_register]);
					}
					break;
			}
			else if (opcode==11){//SLTIU
				uint32_t cas_immediate =(uint32_t)unsinged_immediate; 
				if(rsValue<cas_immediate){
						RegFile[destination_register]= true;
						printf("%d\n",RegFile[destination_register]);
					}
					else{
						RegFile[destination_register]= false;
						printf("%d\n",RegFile[destination_register]);
					}
					break;
					
			}else if (opcode == 12){//ANDI
			int32_t cas_immediate =(int32_t)immediate;
			RegFile[target_register]= rsValue & cas_immediate;
			printf("%d\n",RegFile[target_register]); 
			}
			else if(opcode==13){//ORI
			int32_t cas_immediate =(int32_t)immediate;
			RegFile[target_register]= rsValue | cas_immediate;
			printf("%d\n",RegFile[target_register]); 
			}
			else if(opcode==14){//XORI
			int32_t cas_immediate =(int32_t)immediate;
			RegFile[target_register]= rsValue ^ cas_immediate;
			printf("%d\n",RegFile[target_register]); 
			}
			else if (opcode==15){//LUI
			int32_t cas_immediate =(int32_t)immediate;
			RegFile[target_register]=  cas_immediate <<16;
			}
			else if (opcode==1){

				switch(rtValue){
					case 0:{//BLTZ

					if((signed)rsValue<0){
						
						int32_t cas_immediate= immediate<<2;
						DelayCounter= ProgramCounter+(cas_immediate+4);
						bflag=1;
						break;
					}else{
						bflag=0;
						break;
					}

				}
				case 1:{//BGEZ
				if((signed)rsValue>=0){
						
						int32_t cas_immediate= immediate<<2;
						DelayCounter= ProgramCounter+(cas_immediate+4);
						bflag=1;
						break;
					}else{
						bflag=0;
						break;
					}		
				}
				case 16:{//BLTZAL
					if((signed)rsValue<0){
						
						int32_t cas_immediate= immediate<<2;//casting
						RegFile[31]= ProgramCounter+(cas_immediate+4);
						bflag=1;
						break;
					}else{
						bflag=0;
						break;
					}
				}
				case 17:{//BGEZAL
					if((signed)rsValue>=0){
						
						int32_t cas_immediate= immediate<<2;//casting
						RegFile[31]= ProgramCounter+(cas_immediate+4);
						bflag=1;
						break;
					}else{
						bflag=0;
						break;
					}
				}
			}	
		}	
		else if (opcode == 4){//BEQ
			if((signed)rsValue==(signed)rtValue){
						
						int32_t cas_immediate= immediate<<2;
						DelayCounter= ProgramCounter+(cas_immediate+4);
						bflag=1;
						break;
					}else{
						bflag=0;
						break;
					}	
		}
		else if (opcode == 5){//BNE
			if((signed)rsValue!=(signed)rtValue){
						
						int32_t cas_immediate= immediate<<2;
						DelayCounter= ProgramCounter+(cas_immediate+4);
						bflag=1;
						break;
					}else{
						bflag=0;
						break;
					}	
		}
		else if (opcode == 6){//BLEZ
			if((signed)rsValue<=0){
						
						int32_t cas_immediate= immediate<<2;
						DelayCounter= ProgramCounter+(cas_immediate+4);
						bflag=1;
						break;
					}else{
						bflag=0;
						break;
					}	
		}
		else if (opcode == 7){//BGTZ
			if((signed)rsValue>0){
						int32_t cas_immediate= immediate<<2;
						DelayCounter= ProgramCounter+(cas_immediate+4);
						bflag=1;
						break;
					}else{
						bflag=0;
						break;
					}	
		}
		else if(opcode==2){//J 
			bflag =1;
			uint32_t temp=(ProgramCounter)& 0xF0000000;
			int32_t cas_address= address<<2;
			DelayCounter= temp+(cas_address);
			break;
			printf("x\n", DelayCounter);
		}
		else if(opcode==3){//JAL
			bflag =1;
			uint32_t temp=(ProgramCounter)& 0xF0000000;
			RegFile[31] = ProgramCounter+4;
			int32_t cas_address= address<<2;
			DelayCounter= temp+(cas_address);
			break;

		}
		else if(opcode==32){//LB
			address= RegFile[source_register] + immediate;
			byte= readbyte(address, false);
			RegFile[target_register]=byte;
			byte=0;
			break;
		}
		else if(opcode==33){//LH
			address= RegFile[source_register] + immediate;
			byte= readbyte(address, false);
			RegFile[target_register]=byte << 8;
			byte= readbyte((address+1), false);
			RegFile[target_register]= RegFile[target_register] | byte; 
			byte=0;
			break;
		}
		else if(opcode==34){//LWL
			address = RegFile[source_register] + immediate;
			count = 4 - (address%4);
			if (count==4){
				word= readWord(address, false);
				RegFile[target_register]=byte;
			}
			else{
			int k;	
			for(k=0; k<count; k++){
				word= word | readbyte((address+k), false);
				word= word << 8;
			}
			word = word << (3-k)*8;
			RegFile[target_register]= (RegFile[target_register] & word) | word; 
			}
			word=0;
			break;
		}
		else if(opcode==35){//LW
			address= RegFile[source_register] + immediate;
			word= readword(address, false);
			RegFile[target_register]=word;
			word=0;
			break;
		}
		else if(opcode==36){//LBU
			address= RegFile[source_register] + immediate;
			ubyte= readbyte(address, false);
			RegFile[target_register]=ubyte;
			ubyte=0;
			break;
		}
		else if(opcode==37){//LHU
			address= RegFile[source_register] + immediate;
			ubyte= readbyte(address, false);
			RegFile[target_register]=ubyte << 8;
			ubyte= readbyte((address+1), false);
			RegFile[target_register]= RegFile[target_register] | ubyte; 
			ubyte=0;
			break;
		}
		else if(opcode==38){//LWR
			address = RegFile[source_register] + immediate;
			count = address%4; 
			if (count=!0){
			for(int k=4; k>count; k--){
				data= data | readbyte((address+k), false);
				data= data << 8;
			}
			data = data>>8;
			RegFile[target_register]= (RegFile[target_register] & data) | data; 
			}
			break;
		}
		else if(opcode==40){//SB
			address= RegFile[source_register] + immediate;
			ubyte= RegFile[target_register];
			writeByte(address,ubyte,false);
			ubyte=0;
			break;
		}
		else if(opcode==41){//SH
			address= RegFile[source_register] + immediate;
			ubyte= RegFile[target_register];
			writeByte(address+1,ubyte,false);
			ubyte = RegFile[target_register]>>8;
			writeByte(address,ubyte,false);
			ubyte=0;
		}
		else if(opcode==42){//SWL
			address= RegFile[source_register] + immediate;
			count=4- (address%4);
			uword=RegFile[target_register];
			if(count!=0){
			for(int j=0;j<count;j++){
				ubyte = uword;
				writeByte((address+j),ubyte,false);
				uword = uword >>8;
			}
		    }
		    writeByte(address,uword,false);

			uword =0;
			break;
		}
		else if(opcode == 43){//SW
			address= RegFile[source_register] + immediate;
			uword=RegFile[target_register];
			writeByte(address,uword,false);
			uword=0;
			break;
		}
		else if(opcode==46){//SWR
			address= RegFile[source_register] + immediate;
			count=address%4;
			uword=RegFile[target_register];
			if(count!=0){
				for(int k=4; k>count; k--){
					ubyte=uword;
					writeByte((address+k),ubyte,false);
					uword=uword<<8;
				}
			writeByte(address,uword,false);

			uword =0;
			}
		}
		ProgramCounter+=4;
		RegFile[0]=0;
	}   


	//Close file pointers & free allocated Memory
	closeFDT();
	CleanUp();

	return 0;
	//testing 
}