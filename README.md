# SP1-Assembly-Emulator

This is a SP1 Processor Emulator for Load-Store Assembler. 

# Instructions:

1 - Extract the directory SP1Assembler.

2 - Compile the Assembler using the command:
      
      $make .

3 - Mount the test file (test.asm) 

      $./sp1a test.asm -o test.bin
      
4 - Use the emulator to run the program test.bin

      $./emulator test.bin
