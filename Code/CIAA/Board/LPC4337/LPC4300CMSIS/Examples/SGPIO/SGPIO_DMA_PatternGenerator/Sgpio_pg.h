

typedef struct {
//uint32_t REGdata[16];
uint32_t REG_SSdata[16];
uint32_t REG_SSdata_1[16];
uint32_t REG_SSdata_2[16];
uint32_t REG_SSdata_3[16];
uint32_t EN_Slices;
}DMA_MEM_Type;


typedef struct {
__IO uint32_t CONTRSTAT;			 /*!< (@ E000E010) M3 SYSTICK Control and Status Register */ 
__IO uint32_t RELOAD;				 /*!< (@ E000E014) M3 SYSTICK Reload Value Register */
__IO uint32_t CURRENT;				 /*!< (@ E000E018) M3 SYSTICK Current Value Register */
__IO uint32_t CALIBRATION;			 /*!< (@ E000E01C) M3 SYSTICK Calibration Value Register */
}M3_SYSTICK_Type;


#define MEM_BASE 0x20000000
#define M3_SYSTICK_BASE 0xE000E010

#define DMA_MEM ((DMA_MEM_Type *) MEM_BASE)
#define M3_SYSTICK ((M3_SYSTICK_Type *) M3_SYSTICK_BASE)

#define TENMS_Mask 0xFFFFFF

void setupSGPIO(void);

void Setup_DMAChl(void);

//void DMA_Chl1(void);

//void putdata(uint32_t data);

void SGPIO_IRQHandler(void);

void CGU_Init_M4SGPIO(void);

void WaitMS(int ms);

//void Wait10MS(void);
