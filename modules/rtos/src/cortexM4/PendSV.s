	.syntax unified

	.text
	.thumb_func
	.global PendSV_Handler
	.extern Osek_OldTaskPtr_Arch,Osek_NewTaskPtr_Arch,CheckTerminatingTask_Arch

/* Pendable Service Call, used for context-switching in all Cortex-M processors */
PendSV_Handler:
	/* uso el sp correspondiente, segun si vengo de user o kernel */
	tst lr,4
	ite eq
	mrseq r0,msp
	mrsne r0,psp

	/* FPU context saving */
	tst lr,0x10
	it eq
	vstmdbeq r0!,{s16-s31}

	/* Integer context saving */
	stmdb r0!,{r4-r11,lr}

	/* restituyo MSP, por si existen irqs anidadas */
	tst lr,4
	it eq
	msreq msp,r0

	/* guardo stack actual si corresponde */
	ldr r1,=Osek_OldTaskPtr_Arch
	ldr r1,[r1]
	cmp r1,0
	it ne
	strne r0,[r1]

	/* cargo stack siguiente */
	ldr r1,=Osek_NewTaskPtr_Arch
	ldr r1,[r1]
	ldr r0,[r1]

	/* recupero contexto actual */
	ldmia r0!,{r4-r11,lr}

	/* recupero contexto FPU si es necesario */
	tst lr,0x10
	it eq
	vldmiaeq r0!,{s16-s31}

	/* Me fijo si tengo que volver a modo privilegiado.
	   Actualizo el registro CONTROL */
	mrs r1,control
	tst lr,4
	ittee eq
	/* modo thread -> privilegiado, usar MSP */
	biceq r1,3
	msreq msp,r0
	/* modo thread -> no privilegiado, usar PSP */
	orrne r1,3
	msrne psp,r0

	msr control,r1

	/* reinicio el stack de la tarea que termino */
	push {lr}
	bl CheckTerminatingTask_Arch
	pop {lr}

	bx lr
