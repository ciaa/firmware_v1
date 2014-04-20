
typedef struct {
//uint32_t REGdata[16];
uint32_t REG_SSdata[16];
uint32_t REG_SSdata_1[16];
uint32_t REG_SSdata_2[16];
uint32_t REG_SSdata_3[16];
uint32_t EN_Slices;
}DMA_MEM_Type;

#define DMA_MEM_BASE 0x20000000

#define DMA_MEM ((DMA_MEM_Type *) DMA_MEM_BASE)

void putdata(uint32_t data);
void CGU_Init_M4SGPIO(void);
void WaitMS(int ms);
void copy_VectorTable(void);
