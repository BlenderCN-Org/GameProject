#ifndef OCOPERATIONS_HPP
#define OCOPERATIONS_HPP

typedef char OPCode;

const short defaultStackSize = 4*1024;
const short defaultHeapSize = 16*1024;

const OPCode NOP            = 0x00;
const OPCode ADD_REG        = 0x01;
const OPCode ADD_CONST      = 0x02;
const OPCode SUB_REG        = 0x03;
const OPCode SUB_CONST      = 0x04;
const OPCode MUL_REG        = 0x05;
const OPCode MUL_CONST      = 0x06;
const OPCode DIV_REG        = 0x07;
const OPCode DIV_CONST      = 0x08;
const OPCode MOV_REG_CONST  = 0x09;
const OPCode MOV_REG_REG    = 0x0A;
const OPCode MOV_REG_PCONST = 0x0B;
const OPCode MOV_PCONST_REG = 0x0C;
const OPCode MOV_REG_PREG   = 0x0D;
const OPCode MOV_PREG_REG   = 0x0E;
const OPCode JMP            = 0x0F;


#endif