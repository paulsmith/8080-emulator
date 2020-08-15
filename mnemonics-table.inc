// generated with gen-mnemonics.sh script - do not edit manually
static char *mnemonics_8080[] = {
    "NOP","LXI B,#","STAX B","INX B","INR B","DCR B","MVI B,#","RLC","NOP","DAD B","LDAX B","DCX B","INR C","DCR C","MVI C,#","RRC",
    "NOP","LXI D,#","STAX D","INX D","INR D","DCR D","MVI D,#","RAL","NOP","DAD D","LDAX D","DCX D","INR E","DCR E","MVI E,#","RAR",
    "NOP","LXI H,#","SHLD addr","INX H","INR H","DCR H","MVI H,#","DAA","NOP","DAD H","LHLD addr","DCX H","INR L","DCR L","MVI L,#","CMA",
    "NOP","LXI SP,#","STA addr","INX SP","INR M","DCR M","MVI M,#","STC","NOP","DAD SP","LDA addr","DCX SP","INR A","DCR A","MVI A,#","CMC",
    "MOV B,B","MOV B,C","MOV B,D","MOV B,E","MOV B,H","MOV B,L","MOV B,M","MOV B,A","MOV C,B","MOV C,C","MOV C,D","MOV C,E","MOV C,H","MOV C,L","MOV C,M","MOV C,A",
    "MOV D,B","MOV D,C","MOV D,D","MOV D,E","MOV D,H","MOV D,L","MOV D,M","MOV D,A","MOV E,B","MOV E,C","MOV E,D","MOV E,E","MOV E,H","MOV E,L","MOV E,M","MOV E,A",
    "MOV H,B","MOV H,C","MOV H,D","MOV H,E","MOV H,H","MOV H,L","MOV H,M","MOV H,A","MOV L,B","MOV L,C","MOV L,D","MOV L,E","MOV L,H","MOV L,L","MOV L,M","MOV L,A",
    "MOV M,B","MOV M,C","MOV M,D","MOV M,E","MOV M,H","MOV M,L","HLT","MOV M,A","MOV A,B","MOV A,C","MOV A,D","MOV A,E","MOV A,H","MOV A,L","MOV A,M","MOV A,A",
    "ADD B","ADD C","ADD D","ADD E","ADD H","ADD L","ADD M","ADD A","ADC B","ADC C","ADC D","ADC E","ADC H","ADC L","ADC M","ADC A",
    "SUB B","SUB C","SUB D","SUB E","SUB H","SUB L","SUB M","SUB A","SBB B","SBB C","SBB D","SBB E","SBB H","SBB L","SBB M","SBB A",
    "ANA B","ANA C","ANA D","ANA E","ANA H","ANA L","ANA M","ANA A","XRA B","XRA C","XRA D","XRA E","XRA H","XRA L","XRA M","XRA A",
    "ORA B","ORA C","ORA D","ORA E","ORA H","ORA L","ORA M","ORA A","CMP B","CMP C","CMP D","CMP E","CMP H","CMP L","CMP M","CMP A",
    "RNZ","POP B","JNZ addr","JMP addr","CNZ addr","PUSH B","ADI #","RST 0","RZ","RET","JZ addr","JMP addr","CZ  addr","CALL addr","ACI #","RST 1",
    "RNC","POP D","JNC addr","OUT #","CNC addr","PUSH D","SUI #","RST 2","RC","RET","JC addr","IN #","CC addr","CALL addr","SBI #","RST 3",
    "RPO","POP H","JPO addr","XTHL","CPO addr","PUSH H","ANI #","RST 4","RPE","PCHL","JPE addr","XCHG","CPE addr","CALL addr","XRI #","RST 5",
    "RP","POP PSW","JP addr","DI","CP addr","PUSH PSW","ORI #","RST 6","RM","SPHL","JM addr","EI","CM addr","CALL addr","CPI #","RST 7",
};
