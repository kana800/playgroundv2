// Given two numbers stored in the registers R0 and R1,
// compute the maximum between them and store it in the R2 register.
  @R0
  @R1
  @OUTPUT_FIRST
  @R1
  @OUTPUT_D
  @21
  @32
  @111
  @423
  @11
  @9999
  @65532

(OUTPUT_FIRST)
  @R0
  D=M              // D = first number

(OUTPUT_D)
  @R2
  M=D              // M[2] = D (greatest number)

(INFINITE_LOOP)
  @INFINITE_LOOP
  0;JMP            // infinite loop
