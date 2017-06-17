		
#include <stdio.h>
#include <stdlib.h>
#define KEY_LENGTH 2 //numbers of bits
#define MAX 256
#define byte unsigned char

int setZeroACC ( char *STAT, int bit){ //Zero Acumulador
	if(bit == 1)
		*STAT = *STAT | 1;
	else
		*STAT = *STAT & (255-1);
}

int setCarry ( char *STAT, int bit){ //Carry
	if(bit == 1)
		*STAT = *STAT | 2;
	else
		*STAT = *STAT & (255-2);
}

int setOver ( char *STAT, int bit){ //Overflow
	if(bit == 1)
		*STAT = *STAT | 4;
	else
		*STAT = *STAT & (255-4);
}

int testACC(byte acc, char *STAT){
	byte bit;
	if (acc==0)
		bit=1;
	else
		bit=0;	
	setZeroACC(STAT, bit);
}

typedef struct format{
	byte opcode;
	byte operand;
}instruction;

int main(int argc, char *argv[]){

	instruction number[MAX];
	FILE *fin, *fout;

	unsigned short tmp;

	//****Registradores****
	byte acc=0;
	byte pc=0;
	byte STAT=0;

	//*********************
	byte ch[KEY_LENGTH];
	byte mem[MAX];
	byte bit;
	
	fin = fopen(argv[1], "rb");

	if(fin == NULL){
		printf("Error while opening the file %s \n", argv[1]);
		exit(2);
	}

	while(fread(ch, 1, KEY_LENGTH, fin)){
		number[pc].opcode = ch[0];
		number[pc].operand = ch[1];
		pc++;

		//printf("%d %d\n", number[pc].opcode, number[pc].operand);
	}

	pc = 0;

	while(number[pc].opcode){

		switch(number[pc].opcode){

			case 0://0 LOAD MEMORY
				acc = mem[number[pc].operand];
				testACC(acc, &STAT);
				pc++;
				break;

			case 1://1 LOAD
				acc = number[pc].operand;
				testACC(acc, &STAT);
				pc++;
				break;

			case 2://2 STORE
				mem[number[pc].operand]=acc;
				pc++;
				break;

			case 3://3 ADD
			  	tmp = (acc + mem[number[pc].operand]);
				if(tmp>255){
					bit = 1;
					setCarry(&STAT, bit);
					acc = tmp & 0xFF;
					testACC(acc, &STAT);
				}
				else{
					bit =0;
					setCarry(&STAT, bit);
					acc = tmp;
					testACC(acc, &STAT);
				}
				pc++;
				break;

			case 4://4 SUB
				acc = acc - mem[number[pc].operand];
				testACC(acc, &STAT);
				if(acc>255){
					bit = 1;
					setCarry(&STAT, bit);
				}
				else{
					bit =0;
					setCarry(&STAT, bit);
				}
				pc++;
				break;

			case 5://5 MUL
				tmp = (acc * mem[number[pc].operand]);
				if(tmp>255){
					bit = 1;
					setOver(&STAT, bit);
					acc = tmp & 0xFF;
					testACC(acc, &STAT);
					pc++;
				}
				else{
			    	bit =0;
					setOver(&STAT, bit);
					acc = tmp;
					testACC(acc, &STAT);
					pc++;
				}
				break;

			case 6://6 DIV
				bit = 0;
				if(mem[number[pc].operand]==0){
					printf("ERROR, you couldn't divided by zero\n");
					pc++;
					break;
				}
				acc = acc/mem[number[pc].operand];
				setCarry(&STAT, 0);
				setOver(&STAT, 0);
				testACC(acc, &STAT);
				pc++;
				break;

			case 7://7 INC
				tmp = acc + 1;
				if(tmp>255){
					bit = 1;
					setCarry(&STAT, bit);
					acc = tmp & 0xFF;
					testACC(acc, &STAT);
				}
				else{
					bit =0;
					setCarry(&STAT, bit);
					acc = tmp;
					testACC(acc, &STAT);
				}
				pc++;
				break;

			case 8://8 DEC
				acc = acc - 1;
				setCarry(&STAT, 0);
				setOver(&STAT, 0);
				testACC(acc, &STAT);
				pc++;
				break;

			case 9://9 AND
				acc = acc & mem[number[pc].operand];
				testACC(acc, &STAT);
				pc++;
				break;

			case 10://10 OR
				acc = acc | mem[number[pc].operand];
				testACC(acc, &STAT);
				pc++;
				break;

			case 11://11 NOT
				acc = ~acc;
				testACC(acc, &STAT);
				pc++;
				break;

			case 12://12 JMP
			    pc = number[pc].operand/2;
				break;

			case 13://13 JZ
				if(acc==0){
					pc = number[pc].operand/2;
					printf("%c\n", pc);
				}
				else
					pc++;
				break;

			case 14://14 JNZ

				printf("%c\n", pc);	

				if(acc!=0){

					pc = number[pc].operand/2;
					printf("%c\n", pc);
				}

				else
					pc++;
				break;

			case 15://15
				printf("ACC: %d\nSTAT: %.2X\n", acc, STAT);	
				exit(1);
				break;
		}
	}

	printf("ACC: %d\nSTAT: %.2X\n", acc, STAT);	
	
}
