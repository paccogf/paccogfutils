#include "MyLDElib.h"

//
//
//	TO DO:  INCLUIR CHEQUEOS SOBRE CODIFICACIONES INVALIDAS DEL BYTE ModR/M DE 
//			ALGUNOS OPCODES
//
//	DONE:	GRUPO 3-1A DEL MANUAL DE INTEL
//
//


#define SIB_BYTE 0x04    

#define SET_PREFIX(prefix) *status|=(prefix);\
	finished = FALSE;

#define CHECK_PREFIX(prefix) ((*status) & (prefix))



// DEVUELVE 0 SI ESE PREFIJO ESTABA YA FIJADO
/*
#define SET_PREFIX(prefix) if(prefix) return 0;\
                             prefix=TRUE;\
                             finished=FALSE;\
                             opcode++;*/


// SI SE AÑADE ALGUN OTRO PREFIJO TRAS LOS DE TAMAÑO, FORMAN PARTE DE LA INSTRUCCION
/*#define SET_PREFIX(prefix) prefix=TRUE;\
                             finished=(b66Prefix || b67Prefix)? FALSE: TRUE;\
                             opcode++; */
#ifdef __cplusplus
extern "C"
#endif
//int LDE_Size(BYTE *opcode)
int LDE_Size(BYTE *opcode, DWORD *status)  // AÑADIDO PARAMETRO status -> MULTITHREAD-SAFE
{


register int opsize=0;

BOOL finished=TRUE;
BOOL bModRegRM=FALSE;


opsize++;
switch(*opcode)
{
        ///////////////////// OTROS PREFIJOS ////////////////////////////
    case 0x26:  // PREFIJO DE SEGMENTO ES:
                SET_PREFIX(LDE_ES);
                break;
    case 0x2E:  // PREFIJO DE SEGMENTO CS:
                SET_PREFIX(LDE_CS);
                break;
    case 0x36:  // PREFIJO DE SEGMENTO SS:
                SET_PREFIX(LDE_SS);
                break;
    case 0x3E:  // PREFIJO DE SEGMENTO DS:
                SET_PREFIX(LDE_DS);
                break;
    case 0x64:  // PREFIJO DE SEGMENTO FS:
                SET_PREFIX(LDE_FS);
                break;
    case 0x65:  // PREFIJO DE SEGMENTO GS:
                SET_PREFIX(LDE_GS);
                break;

    case 0xF0:  // LOCK (PREFIJO)
                SET_PREFIX(LDE_LOCK);
                break;

    case 0xF2:  // REPNE (PREFIJO)
                SET_PREFIX(LDE_REPNE);
                break;

    case 0xF3:  // REP/REPE (PREFIJO)
                SET_PREFIX(LDE_REPE);
                break;

                //////////////////////////////////////////////////////////////////


        ////////////////// ESTOS PREFIJOS INFLUYEN EN EL TAMAÑO DE LAS INSTRUCCIONES!!!!!
    case 0x66: /////////////////////////// TAMAÑO DE OPERANDO (SIZE OVERRIDE)///////////////////////////////// 
                SET_PREFIX(LDE_66);
                break;
    case 0x67: /////////////////////////// TAMAÑO DE DIRECCION (ADDRESS OVERRIDE)////////////////////////////
        ////////////////////////////////////////////////////////////////////////////
                SET_PREFIX(LDE_67);
                break;

    /*******************************************************************************/
    /*                         OPCODES DE 1 BYTE                                   */
    /*******************************************************************************/
    
    /*******************************************************************************/
    /*            INSTRUCCIONES DE 0 BYTES DE OPERANDO                             */

    case 0x06:  // PUSH ES
    case 0x07:  // POP ES
    
    case 0x0E:  // PUSH CS

    // case 0x0F: POP CS ----> PERO COINCIDE CON EL 0X0F DEL CODIGO DE ESCAPE PARA OPCODES DE 2 BYTES !!!! NO PERMITIDO EN IA32
    case 0x16:  // PUSH SS
    case 0x17:  // POP SS
    case 0x1E:  // PUSH DS
    case 0x1F:  // POP DS

    case 0x27:  // DAA
    case 0x2F:  // DAS
    case 0x37:  // AAA
    case 0x3F:  // AAS

    case 0x40:  // INC reg
    case 0x41:
    case 0x42:
    case 0x43:
    case 0x44:
    case 0x45:
    case 0x46:
    case 0x47:

    case 0x48:  // DEC reg
    case 0x49:
    case 0x4A:
    case 0x4B:
    case 0x4C:
    case 0x4D:
    case 0x4E:
    case 0x4F:

    case 0x50:  // PUSH reg32
    case 0x51:
    case 0x52:
    case 0x53:
    case 0x54:
    case 0x55:
    case 0x56:
    case 0x57: //

    case 0x58:  // POP reg32
    case 0x59:
    case 0x5A:
    case 0x5B:
    case 0x5C:
    case 0x5D:
    case 0x5E:
    case 0x5F: //

    case 0x60:  // PUSHAD
    case 0x61:  // POPAD
   
    case 0x6C:  // INSB
    case 0x6D:  // INSW
    case 0x6E:  // OUTSB
    case 0x6F:  // OUTSW
    
    case 0x90:  // NOP == XCHG EAX, EAX
    case 0x91:  // XCHG
    case 0x92:
    case 0x93:
    case 0x94:
    case 0x95:
    case 0x96:
    case 0x97:

    case 0x98:  // CBW/CWDE/CDQE
    case 0x99:  // CDQ/CWD/CQO
    case 0x9B:  // FWAIT / WAIT
    case 0x9C:  // PUSHF/D/Q
    case 0x9D:  // POPF/D/Q
    case 0x9E:  // SAHF
    case 0x9F:  // LAHF

    case 0xA4:  //MOVS MOVSB MOVSW MOVSD
    case 0xA5:
    
    case 0xA6:  // CMPS/CMPSB/CMPSW/CMPSD
    case 0xA7:

    case 0xAA:  // STOS/B/W/D
    case 0xAB:  // STOSD
    case 0xAC:  // LODS/B/W/D
    case 0xAD:  // LODSD
    case 0xAE:  // SCASB
    case 0xAF:  // SCASD

    case 0xC3:  // RETN 
    case 0xC9:  // LEAVE
    case 0xCB:  // RETF
    case 0xCC:  // INT 3H
    case 0xCE:  // INTO
    case 0xCF:  // IRETD/D/Q

    case 0xD6:  // SETALC    --> NO ESTA EN LA LISTA DE OPCODES DE INTEL!!!!!!!!!!!!!!!!
    case 0xD7:  // XLAT/XLATB
    
     
    case 0xEC:  // IN al, dx
    case 0xED:  // IN eAX, dx
    case 0xEE:  // OUT dx, al
    case 0xEF:  // OUT dx, eAX


    case 0xF4:  // HLT
    case 0xF5:  // CMC
    case 0xF8:  // CLC
    case 0xF9:  // STC
    case 0xFA:  // CLI
    case 0xFB:  // STI
    case 0xFC:  // CLD
    case 0xFD:  // STD
                //if(*opcode == 0x6C) __asm int 3h;
                break;
    /*******************************************************************************/


    /*******************************************************************************/
    /*            INSTRUCCIONES DE 2 BYTES (OPERANDO CONSTANTE DE 1 BYTE)          */
    
    case 0x04:  // ADD AL, imm8
    case 0x0C:  // OR AL, imm8
    case 0x14:  // ADC AL, imm8
    case 0x1C:  // SBB AL, imm8
    case 0x24:  // AND AL, imm8
    case 0x2C:  // SUB AL, imm8
    case 0x34:  // XOR AL, imm8
    case 0x3C:  // CMP AL, imm8

    case 0x6A:  // PUSH byte
 

    case 0x70:  // JO
    case 0x71:  // JNO
    case 0x72:  // JB/JNAE/JC
    case 0x73:  // JNB/AE/NC
    case 0x74:  // JZ/JE
    case 0x75:  // JNZ/JNE
    case 0x76:  // JBE/JNA
    case 0x77:  // JNBE/JA

    case 0x78:  // JS
    case 0x79:  // JNS
    case 0x7A:  // JP/JPE
    case 0x7B:  // JNP/JPO
    case 0x7C:  // JL/JNGE
    case 0x7D:  // JNL/JGE
    case 0x7E:  // JLE/JNG
    case 0x7F:  // JNLE/JG

    case 0xA8:  // TEST AL, IMM8

    case 0xB0:  // MOV reg, imm8   // ALTERNATE ENCODING PARA MOV
    case 0xB1:  // MOV reg, imm8
    case 0xB2:  // MOV reg, imm8
    case 0xB3:  // MOV reg, imm8
    case 0xB4:  // MOV reg, imm8
    case 0xB5:  // MOV reg, imm8
    case 0xB6:  // MOV reg, imm8
    case 0xB7:  // MOV reg, imm8

    

    case 0xCD:  // INT byte

    

    case 0xD4:  // AAM byte  
    case 0xD5:  // AAD byte

    case 0xE0:  // LOOPNE/LOOPNZ byte
    case 0xE1:  // LOOPE/LOOPZ byte
    case 0xE2:  // LOOP byte
    case 0xE3:  // JCXZ / JECXZ
    case 0xE4:  // IN al, byte
    case 0xE5:  // IN eAX, byte
    case 0xE6:  // OUT al, byte
    case 0xE7:  // OUT eAX, byte

    case 0xEB:  // JMP SHORT XX
                opsize++;
                break;
    /*******************************************************************************/
                

    /*******************************************************************************/
    /*          INSTRUCCIONES DE 3 BYTES (OPERANDO CONSTANTE DE 2 BYTES)           */
    case 0xC2:  // RETN XXXXH
    case 0xCA:  // RETF nnnn
                opsize+=2;
                break;
    /*******************************************************************************/
                
    
    /*******************************************************************************/
    /*        INSTRUCCIONES DE 4 BYTES (OPERANDO CONSTANTE DE 3 BYTES)             */
    //
    case 0xC8:  // ENTER
                opsize+=3;
                break;
    /*******************************************************************************/
        

    /*******************************************************************************/
    /* CODIFICACION COMUN PARA EXTENSION DE OPERACIONES ARITMETICO-LOGICAS CON    */
    /* BYTE MOD-REG-R/M E INMEDIATO                                               */
 
    // TEST, MUL, NOT, NEG, DIV, IDIV, IMUL.. DEPENDIENDO DEL VALOR DEL BYTE 
   
                 // INMEDIATO DE TAMAÑO BYTE
    case 0x80:  // XOR, AND, SUB...
    case 0x82:  // XOR, AND, SUB...
    case 0x83:  // XOR, AND, SUB...
    case 0xC6:  // MOV

                opsize++;
                bModRegRM=TRUE;
                break;
    
    
    // TEST, MUL, NOT, NEG, DIV, IDIV, IMUL.. DEPENDIENDO DEL VALOR DEL BYTE 
    case 0x69:  // OPERACIONES DE TAMAÑO 32/16 BITS
    case 0x6B:
    case 0x81:  // XOR, AND, SUB...
    
    case 0xC7:  // MOV
                opsize += CHECK_PREFIX(LDE_66)? 2: 4;

                bModRegRM=TRUE;
                break;
    /*******************************************************************************/


    /**********************************************************************************/
    /* INSTRUCCI ONES OPERANDO INMEDIATO DE 32 BITS SU TAMAÑO DEPENDE DEL PREFIJO 0x66 */
    /* SIN BYTE MODREGRM                                                              */

    case 0x9A:  // CALL OFFSET, SELECTOR - OFFSET PUEDE SER DE 32 Ó 16 BITS, SELECTOR SIEMPRE DE 16
    case 0xEA:  // JMP OFFSET, SELECTOR - OFFSET PUEDE SER DE 32 Ó 16 BITS, SELECTOR SIEMPRE DE 16
                opsize+=2;
    case 0x05:
    case 0x15:  // ADC EAX, imm32
    case 0x1D:  // SBB EAX, imm32
    case 0x25:  // AND EAX
    case 0x2D:  // SUB EAX, imm32/16
    case 0x35:  // XOR EAX, ...
    case 0x3D:  // CMP

    case 0x68:  // PUSH IMM32/16
    case 0xA9:  // TEST (E)AX, IMM132/16

    case 0xB8: // MOV reg, dis32/dis16
    case 0xB9:
    case 0xBA:
    case 0xBB:
    case 0xBC:
    case 0xBD:
    case 0xBE:
    case 0xBF:
    case 0x0D: // OR (E)AX, IMM16(32)
    
    case 0xE8:  // CALL XXXX
    case 0xE9:  // JMP XXXX
                opsize += CHECK_PREFIX(LDE_66)? 2: 4;
                break;
    /*******************************************************************************/
     

    // INSTRUCCIONES OPERANDO ES DESPLAZAMIENTO DE 32 BITS (Ó 16 BITS SI LLEVA PREFIJO 0x67)
    //  SU TAMAÑO DEPENDE DEL PREFIJO 0x67
    case 0xA0:  // MOV (E)AX/AL, dis32/dis16
    case 0xA1:
    case 0xA2:
    case 0xA3:
                opsize += CHECK_PREFIX(LDE_67)? 2: 4;
                break;
    /*******************************************************************************/
    

    /********************************************************************/
    /* INSTRUCCIONES DE COPROCESADOR, NECESITAN DECODIFICACION DISTINTA */

    case 0xD8:
    case 0xD9:
    case 0xDA:
    case 0xDB:  
    case 0xDC:
    case 0xDD:
    case 0xDE:
    case 0xDF:
/*                opcode++;
                if(*opcode <= 3 || *opcode >= 0xC0)  // UN OPERANDO EN MEMORIA DIRECCIONADO MEDIANTE REGISTRO
                {
                    opsize++;
                    break;
                }
*/
                bModRegRM=TRUE;
                break;
    /*******************************************************************************/




    
    /***************************************************************************************************/
    /* INSTRUCCIONES CON DESPLAZAMIENTO DE TAMAÑO VBLE Y SIB (TIENEN BYTE MOD REG R/M - CASO GENERAL)  */
    case 0xC0:  // SAR, ROR, ROL, SHLSHR
    case 0xC1:
                opsize++;  // ESTOS 2 OPCODES ANTERIORES IMPLICAN UN INMEDIATO DE 8 BITS
    case 0x00: // ADD
    case 0x01:
    case 0x02:          
    case 0x03:

    case 0x08: // OR
    case 0x09:
    case 0x0A:
    case 0x0B:

    case 0x10: // ADC
    case 0x11:
    case 0x12:
    case 0x13:

    case 0x18: // SBB
    case 0x19:
    case 0x1A:
    case 0x1B:

    case 0x20: // AND 
    case 0x21:
    case 0x22:
    case 0x23:

    case 0x28:  // SUB
    case 0x29:
    case 0x2A:
    case 0x2B:

    case 0x30: // XOR 
    case 0x31:
    case 0x32:
    case 0x33:

    case 0x38:  // CMP
    case 0x39:  
    case 0x3A:
    case 0x3B:

    case 0x62:  // BOUND
    case 0x63:  // ARPL

    case 0x84:  // TEST
    case 0x85:
    case 0x86:  // XCHG
    case 0x87:

    case 0x88:  // MOV
    case 0x89:  
    case 0x8A:          
    case 0x8B:  
    case 0x8C:
    case 0x8D: // LEA
    case 0x8E:
    
    case 0x8F:  // POP WORD

    case 0xC4:  // LES
    case 0xC5:  // LDS
 
    case 0xD0:  // SAR, ROR, ROL, SHLSHR, RCR, RCL
    case 0xD1: 
    case 0xD2:
    case 0xD3:
                bModRegRM=TRUE;
                break;
   
    case 0xFE:  // DEC
                // CODIFICACIONES INVALIDAS
                if(opcode[1] >= 0xD0)
                {
                    opsize=0;
                    *status=0;
                    finished=TRUE;
                }
                else
                    bModRegRM=TRUE;
                break;
                
                
    case 0xFF:  
                // CALL INDIRECTO ... !!!!!!!!!!???????????????????
                // DEC
                // CODIFICACIONES INVALIDAS DEL BYTE MOD/REG PARA ESTE OPCODE
                if((opcode[1] >= 0xD8) && (opcode[1] & 0x08))
                {
                    opsize=0;
                    *status=0;
                    finished=TRUE;
                }
                else
                    bModRegRM=TRUE;
                break;
                
    /*******************************************************************************/


    // CODIFICACION ESPECIAL PARA NOT, NEG, TEST, MUL, DIV, IMUL, IDIV
    case 0xF6:  // ESTE CASO NO TIENE INMEDIATO
					/*
					ESTA CODIFICACION DEL BYTE ModR/M ES INVALIDA PARA ESTE OPCODE
					*/
					if((opcode[1] & 0x38) == 0x08)	
					{								
						opsize=0;
						*status=0;
						finished=TRUE;
						break;
					}
                if((*(opcode+1) & 0x38) == 0) opsize++; // TEST CON INMEDIATO DE 1 BYTE
                bModRegRM=TRUE;
                break;
                

    case 0xF7:  // IDIV EAX, REG
					/*
					ESTA CODIFICACION DEL BYTE ModR/M ES INVALIDA PARA ESTE OPCODE
					*/
					if((opcode[1] & 0x38) == 0x08)	
					{								
						opsize=0;
						*status=0;
						finished=TRUE;
						break;
					}

					if((opcode[1] & 0x38) == 0) opsize += CHECK_PREFIX(LDE_66)? 2: 4; // TEST CON INMEDIATO DE 1 BYTE
                bModRegRM=TRUE;
                break;




    /********************************************/
    /* CODIGO DE ESCAPE PARA OPCODES DE 2 BYTES */
    /********************************************/
    case 0x0F:  opsize++;
                opcode++;
                switch(*opcode)
                {
                    // INSTRUCCIONES DE 1 SOLO BYTE CONSTANTE SIN OPERANDO
                    //
                    case 0x06:  // CLTS
                    case 0x08:  // INVD
                    case 0x09:  // WBINVD
                    case 0x30:  // WRMSR
                    case 0x31:  // RDTSC
                    case 0x32:  // RDMSR
                    case 0x33:  // RDPMC

             

                    case 0xA0:  // PUSH FS
                    case 0xA1:  // POP FS
                    case 0xA8:  // PUSH GS
                    case 0xA9:  // POP GS


                    case 0xC8:  // BSWAP
                    case 0xC9:  // BSWAP
                    case 0xCA:  // BSWAP
                    case 0xCB:  // BSWAP
                    case 0xCC:  // BSWAP
                    case 0xCD:  // BSWAP
                    case 0xCE:  // BSWAP
                    case 0xCF:  // BSWAP
                                break;


                    // INSTRUCCIONES DE OPERANDOS DE 1 BYTE CONSTANTE
                    // 
                    case 0x20:  // MOV REG, CR0-7
                    case 0x21:  // MOV REG, DR0-DR7
                    case 0x22:  // MOV CR0-7, REG
                    case 0x23:  // MOV DR0-DR7, REG
                                break;

                    //////////// ESTA ES IGUAL QUE LAS DEL GRUPO DE ABAJO PERO CON 1 BYTE DE DATOS DE INMEDIATOS ///////
                    case 0xA4: // SHLD
                    case 0xAC: // SHRD
                    case 0xBA:  opsize++;


                    // INSTRUCCIONES DESPLAZAMIENTO VBLE - NECESARIO PROCESAMIENTO POSTERIOR DE BYTE MOD REG R/M
                    //
                    case 0x00:  // LTR / STR / VERR / VERW / LIDT / LLDT / SIDT /SLDT
                    case 0x01:  // SMSW / LMSW / INVLPG / LGDT
                    case 0x02:  // LAR
                    case 0x03:  // LSL
                    case 0x0B:  // UD2 (UNDEFINED INSTRUCTION 2)

                    case 0x40:  // CMOV(COND)
                    case 0x41:
                    case 0x42:
                    case 0x43:
                    case 0x44:
                    case 0x45:
                    case 0x46:
                    case 0x47:

                    case 0x90: // SETcc - BYTE SET ON CONDITION
                    case 0x91:
                    case 0x92:
                    case 0x93:
                    case 0x94:
                    case 0x95:
                    case 0x96:
                    case 0x97:
                    case 0x98:
                    case 0x99:
                    case 0x9A:
                    case 0x9B:
                    case 0x9C:
                    case 0x9D:
                    case 0x9E:
                    case 0x9F:
                    
                    case 0xA3:  // BT
                    case 0xA5:  // SHLD

                    case 0xAA:  // RSM
                    case 0xAB:  // BTS



                    case 0xAD:  // SHRD
                    case 0xAF:  // IMUL
                    
                    case 0xB0:  // CMPXCHG
                    case 0xB1:


                    case 0xB2:  // LSS
                    case 0xB3:  // BTR
                    case 0xB4:  // LFS  
                    case 0xB5:  // LGS

                    case 0xB6:  // MOVZX
                    case 0xB7:

                    case 0xC0:  // XADD
                    case 0xC1:


                    case 0xC7:  // CMPXCHG8B

                    case 0xBB:  // BTC
                    case 0xBC:  // BSF
                    case 0xBD:  // BSR

                    case 0xBE:  // MOVSX
                    case 0xBF:
		    case 0x1F:
                                //opcode++;
                                bModRegRM=TRUE;
                                break;



                    // OPERANDO INMEDIATO DE 32/16
                    case 0x80: // Jcc
                    case 0x81:
                    case 0x82:
                    case 0x83:
                    case 0x84:
                    case 0x85:
                    case 0x86:
                    case 0x87:
                    case 0x88:
                    case 0x89:
                    case 0x8A:
                    case 0x8B:
                    case 0x8C:
                    case 0x8D:
                    case 0x8E:
                    case 0x8F:  
                                opsize+=CHECK_PREFIX(LDE_66)? 2: 4;
                                break;
                    

                    // CASO POR DEFECTO - NO TENEMOS NI IDEA DE QUE OPCODE ES!!!!!!!!!!!!!!!!! -> DEVOLVEMOS 0    
                    //
                    default:    opsize=0; 
                                break;


                }
                break;
                


    // CASO POR DEFECTO - NO TENEMOS NI IDEA DE QUE OPCODE ES!!!!!!!!!!!!!!!!! -> DEVOLVEMOS 0
    default:    opsize=0; // 
                break;
}  // switch


if(finished)
{
    // POST-PROCESO DEL BYTE MOD REG R/M
    // SUMA A opsize EL TAMAÑO DE LOS DESPLAZAMIENTOS ENCONTRADOS + TAMAÑO SIB + TAMAÑO BYTE MOD REG R/M
    //
    if(bModRegRM)
    {
        opcode++;  // ACTUALIZAMOS OPSIZE PARA APUNTAR AL BYTE MOD-REG-RM
        opsize++;  // ACTUALIZAMOS OPSIZE PARA INCLUIR TAMAÑO CON EL BYTE MOD-REG-RM

        if(*opcode < 0xC0)   // MOD REG R/M (MOD BITS <3)
        {
            if(((*opcode & 0x07) == SIB_BYTE) && !CHECK_PREFIX(LDE_67)) // SIB BYTE PRESENTE
            {
                opsize++;
                if((*(opcode+1) & 0x07) == 5)  // EXCEPTO ESTOS CASOS  
                {
                    //switch(*(opcode+1) & 0xC0)
                    switch(*opcode & 0xC0)
                    {
                        case 0x00:
                        case 0x80:  //opsize+=4;
                                    break;
                        
                        case 0x40:  //opsize++;
                                    break;

                        default:    return 0;  // AL CARAJO
                    }
                    
                }    
            }

            switch(*opcode >> 6) // NOS QUEDAMOS CON LOS 2 BYTES MAS ALTOS
            {
                case 0: // SOLO HAY DESPLAZAMIENTO DE / A REGISTRO
                        if(!CHECK_PREFIX(LDE_67))
                        {
                            if((*opcode & 0x07) == 5)  // EXCEPTO ESTOS CASOS  
                            {
                                opsize += 4;

                            }    
                        }
                        else
                        {
                            if((*opcode & 0x07) == 6)
                            {
                                opsize += 2;
                            }
                        }
                        break;

                case 1: opsize++;  // DESPLAZAMIENTO DE 8 BITS
                        break;

                case 2: opsize += CHECK_PREFIX(LDE_67)? 2: 4; // DESPLAZAMIENTO 32 Ó 16 BITS
                        break;

                case 3:  opsize=0; // ERROR
                default: opsize=0; 
            }

        }

    }

    *status=0;

}
return opsize;
}


