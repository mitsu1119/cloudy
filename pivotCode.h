/* pivotCode.h */

#define LOADI 	1	/* LOADI r,i */ /* load int */
#define LOADA 	2	/* LOADA r,n *//* load arg */
#define LOADL	3	/* LOADL r,n *//* load local var */
#define STOREA	4	/* STOREA r,n *//* store arg */
#define STOREL	5	/* STOREL r,n *//* store local var */
#define ADD 	6	/* ADD t,r1,r2 */
#define SUB 	7	/* SUB t,r1,r2 */
#define MUL 	8	/* MUL t,r1,r2 */
#define DIV     9   /* DIV t,r1,r2 */
#define GT	    10 	/* GT  t,r1,r2 */
#define LT	    11	/* LT  r,r1,r2 */
#define BEQ0	12	/* BQ  r,L *//* branch if eq 0 */
#define JUMP	13	/* JUMP L */
#define ARG     14	/* ARG r,n *//* set Argument */
#define CALL	15	/* CALL r,func */
#define RET	    16	/* RET r *//* return */
#define PRINTLN	17	/* PRINTLN r,L *//* println function */
#define LABEL 	18	/* LABEL L *//* label */

#define LOADS   19	/* load string label */

#define LOADADDR    20
#define LOAD        21
#define STORE	    22

char *getPivotName(int codenum);
int getPivotNum(char *codename);