#ifndef SYS_CALL
#define SYS_CALL


void setUpSyscalls();

void _irq80Handler(void);

void setup_IDT_entry (int , byte , qword , byte );

typedef enum {LOCK, TRY_LOCK, UNLOCK} lockAction_type;

#endif
