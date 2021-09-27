
#ifndef _INIT_H_
#define	_INIT_H_



//定义常量



#ifdef _INIT_C_

void Init_Ports (void);
void Init_Timer0 (void);
void Init_Timer1 (void);
void Init_UART (void);

#endif
extern	void Init_Ports (void);
extern	void Init_Timer0 (void);
extern	void Init_Timer1 (void);
extern	void Init_UART (void);


#endif

