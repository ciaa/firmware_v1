/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#if !defined(__FSL_DMA_REQUEST_H__)
#define __FSL_DMA_REQUEST_H__

/*!
 * @addtogroup edma_request
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief Structure for the DMA hardware request
 *
 * Defines the structure for the DMA hardware request collections. The user can configure the
 * hardware request into DMAMUX to trigger the DMA transfer accordingly. The index
 * of the hardware request varies according  to the to SoC.
 */
#if defined(CPU_MK70FN1M0VMJ12) || defined(CPU_MK70FN1M0VMJ15)
typedef enum _dma_request_source {
    kDmaRequestMux0Disable = 0|0x100,         /*!< Disable the DMA request. */
    kDmaRequestMux0Reserved1 = 1|0x100,       /*!< Reserved slot. */
    kDmaRequestMux0UART0Rx = 2|0x100,         /*!< UART 0 receiver. */
    kDmaRequestMux0UART0Tx = 3|0x100,         /*!< UART0 transmitter. */
    kDmaRequestMux0UART1Rx = 4|0x100,         /*!< UART1 receiver. */
    kDmaRequestMux0UART1Tx = 5|0x100,         /*!< UART1 transmitter.. */
    kDmaRequestMux0UART2Rx = 6|0x100,         /*!< UART2 receiver. */
    kDmaRequestMux0UART2Tx = 7|0x100,         /*!< UART2 transmitter. */
    kDmaRequestMux0UART3Rx = 8|0x100,         /*!< UART3 receiver. */
    kDmaRequestMux0UART3Tx = 9|0x100,         /*!< UART3 transmitter. */
    kDmaRequestMux0UART4Rx = 10|0x100,        /*!< UART4 receiver. */
    kDmaRequestMux0UART4Tx = 11|0x100,        /*!< UART4 transmitter. */
    kDmaRequestMux0UART5Rx = 12|0x100,        /*!< UART5 receiver. */
    kDmaRequestMux0UART5Tx = 13|0x100,        /*!< UART5 transmitter. */
    kDmaRequestMux0I2S0Rx = 14|0x100,         /*!< I2S0 receiver. */
    kDmaRequestMux0I2S0Tx = 15|0x100,         /*!< I2S0 transmitter. */
    kDmaRequestMux0SPI0Rx = 16|0x100,         /*!< SPI0 receiver. */
    kDmaRequestMux0SPI0Tx = 17|0x100,         /*!< SPI0 transmitter. */
    kDmaRequestMux0SPI1Rx = 18|0x100,         /*!< SPI1 receiver. */
    kDmaRequestMux0SPI1Tx = 19|0x100,         /*!< SPI1 transmitter. */
    kDmaRequestMux0SPI2Rx = 20|0x100,         /*!< SPI2 receiver. */
    kDmaRequestMux0SPI2Tx = 21|0x100,         /*!< SPI2 transmitter. */
    kDmaRequestMux0I2C0 = 22|0x100,           /*!< I2C0. */
    kDmaRequestMux0I2C1I2C2 = 23|0x100,       /*!< I2C1 or I2C2. */
    kDmaRequestMux0FTM0Channel0 = 24|0x100,   /*!< Channel 0 in FTM0. */
    kDmaRequestMux0FTM0Channel1 = 25|0x100,   /*!< Channel 1 in FTM0. */
    kDmaRequestMux0FTM0Channel2 = 26|0x100,   /*!< Channel 2 in FTM0. */
    kDmaRequestMux0FTM0Channel3 = 27|0x100,   /*!< Channel 3 in FTM0. */
    kDmaRequestMux0FTM0Channel4 = 28|0x100,   /*!< Channel 4 in FTM0. */
    kDmaRequestMux0FTM0Channel5 = 29|0x100,   /*!< Channel 5 in FTM0. */
    kDmaRequestMux0FTM0Channel6 = 30|0x100,   /*!< Channel 6 in FTM0. */
    kDmaRequestMux0FTM0Channel7 = 31|0x100,   /*!< Channel 7 in FTM0. */
    kDmaRequestMux0FTM1Channel0 = 32|0x100,   /*!< Channel 0 in FTM1. */
    kDmaRequestMux0FTM1Channel1 = 33|0x100,   /*!< Channel 1 in FTM1. */
    kDmaRequestMux0FTM2Channel0 = 34|0x100,   /*!< Channel 0 in FTM2. */
    kDmaRequestMux0FTM2Channel1 = 35|0x100,   /*!< Channel 1 in FTM2. */
    kDmaRequestMux0IEEE1588Timer0 = 36|0x100, /*!< IEEE 1588 timer0. */
    kDmaRequestMux0IEEE1588Timer1 = 37|0x100, /*!< IEEE 1588 timer1. */
    kDmaRequestMux0IEEE1588Timer2 = 38|0x100, /*!< IEEE 1588 timer2. */
    kDmaRequestMux0IEEE1588Timer3 = 39|0x100, /*!< IEEE 1588 timer3. */
    kDmaRequestMux0ADC0 = 40|0x100,           /*!< ADC0. */
    kDmaRequestMux0ADC1 = 41|0x100,           /*!< ADC1. */
    kDmaRequestMux0CMP0 = 42|0x100,           /*!< CMP0. */
    kDmaRequestMux0CMP1 = 43|0x100,           /*!< CMP1. */
    kDmaRequestMux0CMP2 = 44|0x100,           /*!< CMP2. */
    kDmaRequestMux0DAC0 = 45|0x100,           /*!< DAC0. */
    kDmaRequestMux0DAC1 = 46|0x100,           /*!< DAC1. */
    kDmaRequestMux0CMT = 47|0x100,            /*!< CMT. */
    kDmaRequestMux0PDB = 48|0x100,            /*!< PDB. */
    kDmaRequestMux0PortA = 49|0x100,          /*!< Port A. */
    kDmaRequestMux0PortB = 50|0x100,          /*!< Port B. */
    kDmaRequestMux0PortC = 51|0x100,          /*!< Port C. */
    kDmaRequestMux0PortD = 52|0x100,          /*!< Port D. */
    kDmaRequestMux0PortE = 53|0x100,          /*!< Port E. */
    kDmaRequestMux0AlwaysOn54 = 54|0x100,     /*!< Always on. */
    kDmaRequestMux0AlwaysOn55 = 55|0x100,     /*!< Always on. */
    kDmaRequestMux0AlwaysOn56 = 56|0x100,     /*!< Always on. */
    kDmaRequestMux0AlwaysOn57 = 57|0x100,     /*!< Always on. */
    kDmaRequestMux0AlwaysOn58 = 58|0x100,     /*!< Always on. */
    kDmaRequestMux0AlwaysOn59 = 59|0x100,     /*!< Always on. */
    kDmaRequestMux0AlwaysOn60 = 60|0x100,     /*!< Always on. */
    kDmaRequestMux0AlwaysOn61 = 61|0x100,     /*!< Always on. */
    kDmaRequestMux0AlwaysOn62 = 62|0x100,     /*!< Always on. */
    kDmaRequestMux0AlwaysOn63 = 63|0x100,     /*!< Always on. */
    kDmaRequestMux1Disable = 64|0x200,        /*!< Disable the DMA request. */
    kDmaRequestMux1Reserved1 = 65|0x200,      /*!< Reserved slot. */
    kDmaRequestMux1UART0Rx = 66|0x200,         /*!< UART 0 receiver. */
    kDmaRequestMux1UART0Tx = 67|0x200,         /*!< UART0 transmitter. */
    kDmaRequestMux1UART1Rx = 68|0x200,         /*!< UART1 receiver. */
    kDmaRequestMux1UART1Tx = 69|0x200,         /*!< UART1 transmitter.. */
    kDmaRequestMux1UART2Rx = 70|0x200,         /*!< UART2 receiver. */
    kDmaRequestMux1UART2Tx = 71|0x200,         /*!< UART2 transmitter. */
    kDmaRequestMux1UART3Rx = 72|0x200,         /*!< UART3 receiver. */
    kDmaRequestMux1UART3Tx = 73|0x200,         /*!< UART3 transmitter. */
    kDmaRequestMux1UART4Rx = 74|0x200,        /*!< UART4 receiver. */
    kDmaRequestMux1UART4Tx = 75|0x200,        /*!< UART4 transmitter. */
    kDmaRequestMux1UART5Rx = 76|0x200,        /*!< UART5 receiver. */
    kDmaRequestMux1UART5Tx = 77|0x200,        /*!< UART5 transmitter. */
    kDmaRequestMux1I2S1Rx = 78|0x200,         /*!< I2S1 receiver. */
    kDmaRequestMux1I2S1Tx = 79|0x200,         /*!< I2S1 transmitter. */
    kDmaRequestMux1SPI0Rx = 80|0x200,         /*!< SPI0 receiver. */
    kDmaRequestMux1SPI0Tx = 81|0x200,         /*!< SPI0 transmitter. */
    kDmaRequestMux1SPI1Rx = 82|0x200,         /*!< SPI1 receiver. */
    kDmaRequestMux1SPI1Tx = 83|0x200,         /*!< SPI1 transmitter. */
    kDmaRequestMux1SPI2Rx = 84|0x200,         /*!< SPI2 receiver. */
    kDmaRequestMux1SPI2Tx = 85|0x200,         /*!< SPI2 transmitter. */
    kDmaRequestMux1Reserved2 = 86|0x200,      /*!< Reserved slot. */
    kDmaRequestMux1Reserved3 = 87|0x200,      /*!< Reserved slot. */
    kDmaRequestMux1FTM3Channel0 = 88|0x200,   /*!< Channel 0 in FTM3. */
    kDmaRequestMux1FTM3Channel1 = 89|0x200,   /*!< Channel 1 in FTM3. */
    kDmaRequestMux1FTM3Channel2 = 90|0x200,   /*!< Channel 2 in FTM3. */
    kDmaRequestMux1FTM3Channel3 = 91|0x200,   /*!< Channel 3 in FTM3. */
    kDmaRequestMux1FTM3Channel4 = 92|0x200,   /*!< Channel 4 in FTM3. */
    kDmaRequestMux1FTM3Channel5 = 93|0x200,   /*!< Channel 5 in FTM3. */
    kDmaRequestMux1FTM3Channel6 = 94|0x200,   /*!< Channel 6 in FTM3. */
    kDmaRequestMux1FTM3Channel7 = 95|0x200,   /*!< Channel 7 in FTM3. */
    kDmaRequestMux1Reserved4 = 96|0x200,      /*!< Reserved slot. */
    kDmaRequestMux1Reserved5 = 97|0x200,      /*!< Reserved slot. */
    kDmaRequestMux1Reserved6 = 98|0x200,      /*!< Reserved slot. */
    kDmaRequestMux1Reserved7 = 99|0x200,      /*!< Reserved slot. */
    kDmaRequestMux1IEEE1588Timer0 = 100|0x200,/*!< IEEE 1588 timer0. */
    kDmaRequestMux1IEEE1588Timer1 = 101|0x200,/*!< IEEE 1588 timer1. */
    kDmaRequestMux1IEEE1588Timer2 = 102|0x200,/*!< IEEE 1588 timer2. */
    kDmaRequestMux1IEEE1588Timer3 = 103|0x200,/*!< IEEE 1588 timer3. */
    kDmaRequestMux1ADC0 = 104|0x200,          /*!< ADC0. */
    kDmaRequestMux1ADC1 = 105|0x200,          /*!< ADC1. */
    kDmaRequestMux1ADC2 = 106|0x200,          /*!< ADC2. */
    kDmaRequestMux1ADC3 = 107|0x200,          /*!< ADC3. */
    kDmaRequestMux1Reserved8 = 108|0x200,     /*!< Reserved slot. */
    kDmaRequestMux1DAC0 = 109|0x200,          /*!< DAC0. */
    kDmaRequestMux1DAC1 = 110|0x200,          /*!< DAC1. */
    kDmaRequestMux1CMP0 = 111|0x200,          /*!< CMP0. */
    kDmaRequestMux1CMP1 = 112|0x200,          /*!< CMP1. */
    kDmaRequestMux1CMP2 = 113|0x200,          /*!< CMP2. */
    kDmaRequestMux1CMP3 = 114|0x200,          /*!< CMP3. */
    kDmaRequestMux1Reserved9 = 115|0x200,     /*!< Reserved slot. */
    kDmaRequestMux1Reserved10 = 116|0x200,    /*!< Reserved slot. */
    kDmaRequestMux1PortF = 117|0x200,         /*!< Port F. */
    kDmaRequestMux1AlwaysOn54 = 118|0x200,    /*!< Always on. */
    kDmaRequestMux1AlwaysOn55 = 119|0x200,    /*!< Always on. */
    kDmaRequestMux1AlwaysOn56 = 120|0x200,    /*!< Always on. */
    kDmaRequestMux1AlwaysOn57 = 121|0x200,    /*!< Always on. */
    kDmaRequestMux1AlwaysOn58 = 122|0x200,    /*!< Always on. */
    kDmaRequestMux1AlwaysOn59 = 123|0x200,    /*!< Always on. */
    kDmaRequestMux1AlwaysOn60 = 124|0x200,    /*!< Always on. */
    kDmaRequestMux1AlwaysOn61 = 125|0x200,    /*!< Always on. */
    kDmaRequestMux1AlwaysOn62 = 126|0x200,    /*!< Always on. */
    kDmaRequestMux1AlwaysOn63 = 127|0x200,    /*!< Always on. */
} dma_request_source_t;

#elif defined(K64F12_SERIES) || defined(K63F12_SERIES)
typedef enum _dma_request_source {
    kDmaRequestMux0Disable = 0|0x100,         /*!< Disable the DMA request. */
    kDmaRequestMux0Reserved1 = 1|0x100,       /*!< Reserved slot. */
    kDmaRequestMux0UART0Rx = 2|0x100,         /*!< UART 0 receiver. */
    kDmaRequestMux0UART0Tx = 3|0x100,         /*!< UART0 transmitter. */
    kDmaRequestMux0UART1Rx = 4|0x100,         /*!< UART1 receiver. */
    kDmaRequestMux0UART1Tx = 5|0x100,         /*!< UART1 transmitter.. */
    kDmaRequestMux0UART2Rx = 6|0x100,         /*!< UART2 receiver. */
    kDmaRequestMux0UART2Tx = 7|0x100,         /*!< UART2 transmitter. */
    kDmaRequestMux0UART3Rx = 8|0x100,         /*!< UART3 receiver. */
    kDmaRequestMux0UART3Tx = 9|0x100,         /*!< UART3 transmitter. */
    kDmaRequestMux0UART4 = 10|0x100,          /*!< UART4 receiver or transmitter. */
    kDmaRequestMux0UART5 = 11|0x100,          /*!< UART5 transmitter or receiver. */
    kDmaRequestMux0I2S0Rx = 12|0x100,         /*!< I2S0 receiver. */
    kDmaRequestMux0I2S0Tx = 13|0x100,         /*!< I2S0 transmitter. */
    kDmaRequestMux0SPI0Rx = 14|0x100,         /*!< SPI0 receiver. */
    kDmaRequestMux0SPI0Tx = 15|0x100,         /*!< SPI0 transmitter. */
    kDmaRequestMux0SPI1   = 16|0x100,         /*!< SPI1 transmitter or receiver. */
    kDmaRequestMux0SPI2   = 17|0x100,         /*!< SPI2 transmitter or receiver. */
    kDmaRequestMux0I2C0 = 18|0x100,           /*!< I2C0. */
    kDmaRequestMux0I2C1I2C2 = 19|0x100,       /*!< I2C1 or I2C2. */
    kDmaRequestMux0FTM0Channel0 = 20|0x100,   /*!< Channel 0 in FTM0. */
    kDmaRequestMux0FTM0Channel1 = 21|0x100,   /*!< Channel 1 in FTM0. */
    kDmaRequestMux0FTM0Channel2 = 22|0x100,   /*!< Channel 2 in FTM0. */
    kDmaRequestMux0FTM0Channel3 = 23|0x100,   /*!< Channel 3 in FTM0. */
    kDmaRequestMux0FTM0Channel4 = 24|0x100,   /*!< Channel 4 in FTM0. */
    kDmaRequestMux0FTM0Channel5 = 25|0x100,   /*!< Channel 5 in FTM0. */
    kDmaRequestMux0FTM0Channel6 = 26|0x100,   /*!< Channel 6 in FTM0. */
    kDmaRequestMux0FTM0Channel7 = 27|0x100,   /*!< Channel 7 in FTM0. */
    kDmaRequestMux0FTM1Channel0 = 28|0x100,   /*!< Channel 0 in FTM1. */
    kDmaRequestMux0FTM1Channel1 = 29|0x100,   /*!< Channel 1 in FTM1. */
    kDmaRequestMux0FTM2Channel0 = 30|0x100,   /*!< Channel 0 in FTM2. */
    kDmaRequestMux0FTM2Channel1  = 31|0x100,  /*!< Channel 1 in FTM2. */
    kDmaRequestMux0FTM3Channel0 = 32|0x100,   /*!< Channel 0 in FTM3. */
    kDmaRequestMux0FTM3Channel1 = 33|0x100,   /*!< Channel 1 in FTM3. */
    kDmaRequestMux0FTM3Channel2 = 34|0x100,   /*!< Channel 2 in FTM3. */
    kDmaRequestMux0FTM3Channel3 = 35|0x100,   /*!< Channel 3 in FTM3. */
    kDmaRequestMux0FTM3Channel4 = 36|0x100,   /*!< Channel 4 in FTM3. */
    kDmaRequestMux0FTM3Channel5 = 37|0x100,   /*!< Channel 5 in FTM3. */
    kDmaRequestMux0FTM3Channel6 = 38|0x100,   /*!< Channel 6 in FTM3. */
    kDmaRequestMux0FTM3Channel7 = 39|0x100,   /*!< Channel 7 in FTM3. */
    kDmaRequestMux0ADC0 = 40|0x100,           /*!< ADC0. */
    kDmaRequestMux0ADC1 = 41|0x100,           /*!< ADC1. */
    kDmaRequestMux0CMP0 = 42|0x100,           /*!< CMP0. */
    kDmaRequestMux0CMP1 = 43|0x100,           /*!< CMP1. */
    kDmaRequestMux0CMP2 = 44|0x100,           /*!< CMP2. */
    kDmaRequestMux0DAC0 = 45|0x100,           /*!< DAC0. */
    kDmaRequestMux0DAC1 = 46|0x100,           /*!< DAC1. */
    kDmaRequestMux0CMT = 47|0x100,            /*!< CMT. */
    kDmaRequestMux0PDB = 48|0x100,            /*!< PDB. */
    kDmaRequestMux0PortA = 49|0x100,          /*!< Port A. */
    kDmaRequestMux0PortB = 50|0x100,          /*!< Port B. */
    kDmaRequestMux0PortC = 51|0x100,          /*!< Port C. */
    kDmaRequestMux0PortD = 52|0x100,          /*!< Port D. */
    kDmaRequestMux0PortE = 53|0x100,          /*!< Port E. */
    kDmaRequestMux0IEEE1588Timer0 = 54|0x100,/*!< IEEE 1588 timer0. */
    kDmaRequestMux0IEEE1588Timer1 = 55|0x100,/*!< IEEE 1588 timer1. */
    kDmaRequestMux0IEEE1588Timer2 = 56|0x100,/*!< IEEE 1588 timer2. */
    kDmaRequestMux0IEEE1588Timer3 = 57|0x100,/*!< IEEE 1588 timer3. */
    kDmaRequestMux0AlwaysOn58 = 58|0x100,     /*!< Always on. */
    kDmaRequestMux0AlwaysOn59 = 59|0x100,     /*!< Always on. */
    kDmaRequestMux0AlwaysOn60 = 60|0x100,     /*!< Always on. */
    kDmaRequestMux0AlwaysOn61 = 61|0x100,     /*!< Always on. */
    kDmaRequestMux0AlwaysOn62 = 62|0x100,     /*!< Always on. */
    kDmaRequestMux0AlwaysOn63 = 63|0x100,     /*!< Always on. */
} dma_request_source_t;

#elif defined(K22F12810_SERIES) || defined(K22F25612_SERIES) || defined(K22F51212_SERIES) || \
    defined(KV31F12810_SERIES) || defined(KV31F25612_SERIES) || defined(KV31F51212_SERIES) 
typedef enum _dma_request_source {
    kDmaRequestMux0Disable = 0|0x100,         /*!< Disable the DMA request. */
    kDmaRequestMux0Reserved1 = 1|0x100,       /*!< Reserved slot. */
    kDmaRequestMux0UART0Rx = 2|0x100,         /*!< UART 0 receiver. */
    kDmaRequestMux0UART0Tx = 3|0x100,         /*!< UART0 transmitter. */
    kDmaRequestMux0UART1Rx = 4|0x100,         /*!< UART1 receiver. */
    kDmaRequestMux0UART1Tx = 5|0x100,         /*!< UART1 transmitter.. */
    kDmaRequestMux0UART2Rx = 6|0x100,         /*!< UART2 receiver. */
    kDmaRequestMux0UART2Tx = 7|0x100,         /*!< UART2 transmitter. */
    kDmaRequestMux0UART3Rx = 8|0x100,         /*!< UART3 receiver. */
    kDmaRequestMux0UART3Tx = 9|0x100,         /*!< UART3 transmitter. */
    kDmaRequestMux0Reserved10 = 10|0x100,     /*!< Reserved. */
    kDmaRequestMux0Reserved11 = 11|0x100,     /*!< Reserved. */
    kDmaRequestMux0I2S0Rx = 12|0x100,         /*!< I2S0 receiver. */
    kDmaRequestMux0I2S0Tx = 13|0x100,         /*!< I2S0 transmitter. */
    kDmaRequestMux0SPI0Rx = 14|0x100,         /*!< SPI0 receiver. */
    kDmaRequestMux0SPI0Tx = 15|0x100,         /*!< SPI0 transmitter. */
    kDmaRequestMux0SPI1   = 16|0x100,         /*!< SPI1 transmitter or receiver. */
    kDmaRequestMux0Reserved17   = 17|0x100,   /*!< Reserved. */
    kDmaRequestMux0I2C0 = 18|0x100,           /*!< I2C0. */
    kDmaRequestMux0I2C1 = 19|0x100,           /*!< I2C1. */
    kDmaRequestMux0FTM0Channel0 = 20|0x100,   /*!< Channel 0 in FTM0. */
    kDmaRequestMux0FTM0Channel1 = 21|0x100,   /*!< Channel 1 in FTM0. */
    kDmaRequestMux0FTM0Channel2 = 22|0x100,   /*!< Channel 2 in FTM0. */
    kDmaRequestMux0FTM0Channel3 = 23|0x100,   /*!< Channel 3 in FTM0. */
    kDmaRequestMux0FTM0Channel4 = 24|0x100,   /*!< Channel 4 in FTM0. */
    kDmaRequestMux0FTM0Channel5 = 25|0x100,   /*!< Channel 5 in FTM0. */
    kDmaRequestMux0FTM0Channel6 = 26|0x100,   /*!< Channel 6 in FTM0. */
    kDmaRequestMux0FTM0Channel7 = 27|0x100,   /*!< Channel 7 in FTM0. */
    kDmaRequestMux0FTM1Channel0 = 28|0x100,   /*!< Channel 0 in FTM1. */
    kDmaRequestMux0FTM1Channel1 = 29|0x100,   /*!< Channel 1 in FTM1. */
    kDmaRequestMux0FTM2Channel0 = 30|0x100,   /*!< Channel 0 in FTM2. */
    kDmaRequestMux0FTM2Channel1  = 31|0x100,   /*!< Channel 1 in FTM2. */
    kDmaRequestMux0FTM3Channel0 = 32|0x100,   /*!< Channel 0 in FTM3. */
    kDmaRequestMux0FTM3Channel1 = 33|0x100,   /*!< Channel 1 in FTM3. */
    kDmaRequestMux0FTM3Channel2 = 34|0x100,   /*!< Channel 2 in FTM3. */
    kDmaRequestMux0FTM3Channel3 = 35|0x100,   /*!< Channel 3 in FTM3. */
    kDmaRequestMux0FTM3Channel4 = 36|0x100,   /*!< Channel 4 in FTM3. */
    kDmaRequestMux0FTM3Channel5 = 37|0x100,   /*!< Channel 5 in FTM3. */
    kDmaRequestMux0FTM3Channel6 = 38|0x100,   /*!< Channel 6 in FTM3. */
    kDmaRequestMux0FTM3Channel7 = 39|0x100,   /*!< Channel 7 in FTM3. */
    kDmaRequestMux0ADC0 = 40|0x100,           /*!< ADC0. */
    kDmaRequestMux0ADC1 = 41|0x100,           /*!< ADC1. */
    kDmaRequestMux0CMP0 = 42|0x100,           /*!< CMP0. */
    kDmaRequestMux0CMP1 = 43|0x100,           /*!< CMP1. */
    kDmaRequestMux0Reserved44 = 44|0x100,           /*!< Reserved Slot 44 */
    kDmaRequestMux0DAC0 = 45|0x100,           /*!< DAC0. */
    kDmaRequestMux0DAC1 = 46|0x100,           /*!< DAC1. */
    kDmaRequestMux0Reserved47 = 47|0x100,            /*!< Reserved Slot 47. */
    kDmaRequestMux0PDB = 48|0x100,            /*!< PDB. */
    kDmaRequestMux0PortA = 49|0x100,          /*!< Port A. */
    kDmaRequestMux0PortB = 50|0x100,          /*!< Port B. */
    kDmaRequestMux0PortC = 51|0x100,          /*!< Port C. */
    kDmaRequestMux0PortD = 52|0x100,          /*!< Port D. */
    kDmaRequestMux0PortE = 53|0x100,          /*!< Port E. */
    kDmaRequestMux0Reserved54 = 54|0x100,     /*!< Always on. */
    kDmaRequestMux0Reserved55 = 55|0x100,     /*!< Always on. */
    kDmaRequestMux0Reserved56 = 56|0x100,     /*!< Always on. */
    kDmaRequestMux0Reserved57 = 57|0x100,     /*!< Always on. */
    kDmaRequestMux0LPUART0Rx = 58|0x100,     /*!< Always on. */
    kDmaRequestMux0LPUART0Tx = 59|0x100,     /*!< Always on. */
    kDmaRequestMux0AlwaysOn60 = 60|0x100,     /*!< Always on. */
    kDmaRequestMux0AlwaysOn61 = 61|0x100,     /*!< Always on. */
    kDmaRequestMux0AlwaysOn62 = 62|0x100,     /*!< Always on. */
    kDmaRequestMux0AlwaysOn63 = 63|0x100,     /*!< Always on. */
} dma_request_source_t;

#elif defined(K24F12_SERIES)
typedef enum _dma_request_source {
    kDmaRequestMux0Disable = 0|0x100,         /*!< Disable the DMA request. */
    kDmaRequestMux0Reserved1 = 1|0x100,       /*!< Reserved slot. */
    kDmaRequestMux0UART0Rx = 2|0x100,         /*!< UART 0 receiver. */
    kDmaRequestMux0UART0Tx = 3|0x100,         /*!< UART0 transmitter. */
    kDmaRequestMux0UART1Rx = 4|0x100,         /*!< UART1 receiver. */
    kDmaRequestMux0UART1Tx = 5|0x100,         /*!< UART1 transmitter.. */
    kDmaRequestMux0UART2Rx = 6|0x100,         /*!< UART2 receiver. */
    kDmaRequestMux0UART2Tx = 7|0x100,         /*!< UART2 transmitter. */
    kDmaRequestMux0UART3Rx = 8|0x100,         /*!< UART3 receiver. */
    kDmaRequestMux0UART3Tx = 9|0x100,         /*!< UART3 transmitter. */
    kDmaRequestMux0UART4 = 10|0x100,          /*!< UART4 receiver or transmitter. */
    kDmaRequestMux0UART5 = 11|0x100,          /*!< UART5 transmitter or receiver. */
    kDmaRequestMux0I2S0Rx = 12|0x100,         /*!< I2S0 receiver. */
    kDmaRequestMux0I2S0Tx = 13|0x100,         /*!< I2S0 transmitter. */
    kDmaRequestMux0SPI0Rx = 14|0x100,         /*!< SPI0 receiver. */
    kDmaRequestMux0SPI0Tx = 15|0x100,         /*!< SPI0 transmitter. */
    kDmaRequestMux0SPI1   = 16|0x100,         /*!< SPI1 transmitter or receiver. */
    kDmaRequestMux0SPI2   = 17|0x100,         /*!< SPI2 transmitter or receiver. */
    kDmaRequestMux0I2C0 = 18|0x100,           /*!< I2C0. */
    kDmaRequestMux0I2C1I2C2 = 19|0x100,       /*!< I2C1 or I2C2. */
    kDmaRequestMux0FTM0Channel0 = 20|0x100,   /*!< Channel 0 in FTM0. */
    kDmaRequestMux0FTM0Channel1 = 21|0x100,   /*!< Channel 1 in FTM0. */
    kDmaRequestMux0FTM0Channel2 = 22|0x100,   /*!< Channel 2 in FTM0. */
    kDmaRequestMux0FTM0Channel3 = 23|0x100,   /*!< Channel 3 in FTM0. */
    kDmaRequestMux0FTM0Channel4 = 24|0x100,   /*!< Channel 4 in FTM0. */
    kDmaRequestMux0FTM0Channel5 = 25|0x100,   /*!< Channel 5 in FTM0. */
    kDmaRequestMux0FTM0Channel6 = 26|0x100,   /*!< Channel 6 in FTM0. */
    kDmaRequestMux0FTM0Channel7 = 27|0x100,   /*!< Channel 7 in FTM0. */
    kDmaRequestMux0FTM1Channel0 = 28|0x100,   /*!< Channel 0 in FTM1. */
    kDmaRequestMux0FTM1Channel1 = 29|0x100,   /*!< Channel 1 in FTM1. */
    kDmaRequestMux0FTM2Channel0 = 30|0x100,   /*!< Channel 0 in FTM2. */
    kDmaRequestMux0FTM2Channel1  = 31|0x100,  /*!< Channel 1 in FTM2. */
    kDmaRequestMux0FTM3Channel0 = 32|0x100,   /*!< Channel 0 in FTM3. */
    kDmaRequestMux0FTM3Channel1 = 33|0x100,   /*!< Channel 1 in FTM3. */
    kDmaRequestMux0FTM3Channel2 = 34|0x100,   /*!< Channel 2 in FTM3. */
    kDmaRequestMux0FTM3Channel3 = 35|0x100,   /*!< Channel 3 in FTM3. */
    kDmaRequestMux0FTM3Channel4 = 36|0x100,   /*!< Channel 4 in FTM3. */
    kDmaRequestMux0FTM3Channel5 = 37|0x100,   /*!< Channel 5 in FTM3. */
    kDmaRequestMux0FTM3Channel6 = 38|0x100,   /*!< Channel 6 in FTM3. */
    kDmaRequestMux0FTM3Channel7 = 39|0x100,   /*!< Channel 7 in FTM3. */
    kDmaRequestMux0ADC0 = 40|0x100,           /*!< ADC0. */
    kDmaRequestMux0ADC1 = 41|0x100,           /*!< ADC1. */
    kDmaRequestMux0CMP0 = 42|0x100,           /*!< CMP0. */
    kDmaRequestMux0CMP1 = 43|0x100,           /*!< CMP1. */
    kDmaRequestMux0CMP2 = 44|0x100,           /*!< CMP2. */
    kDmaRequestMux0DAC0 = 45|0x100,           /*!< DAC0. */
    kDmaRequestMux0DAC1 = 46|0x100,           /*!< DAC1. */
    kDmaRequestMux0CMT = 47|0x100,            /*!< CMT. */
    kDmaRequestMux0PDB = 48|0x100,            /*!< PDB. */
    kDmaRequestMux0PortA = 49|0x100,          /*!< Port A. */
    kDmaRequestMux0PortB = 50|0x100,          /*!< Port B. */
    kDmaRequestMux0PortC = 51|0x100,          /*!< Port C. */
    kDmaRequestMux0PortD = 52|0x100,          /*!< Port D. */
    kDmaRequestMux0PortE = 53|0x100,          /*!< Port E. */
    kDmaRequestMux0Reserved54 = 54|0x100,/*!< Reserved slot. */
    kDmaRequestMux0Reserved55 = 55|0x100,/*!< Reserved slot. */
    kDmaRequestMux0Reserved56 = 56|0x100,/*!< Reserved slot. */
    kDmaRequestMux0Reserved57 = 57|0x100,/*!< Reserved slot. */
    kDmaRequestMux0AlwaysOn58 = 58|0x100,     /*!< Always on. */
    kDmaRequestMux0AlwaysOn59 = 59|0x100,     /*!< Always on. */
    kDmaRequestMux0AlwaysOn60 = 60|0x100,     /*!< Always on. */
    kDmaRequestMux0AlwaysOn61 = 61|0x100,     /*!< Always on. */
    kDmaRequestMux0AlwaysOn62 = 62|0x100,     /*!< Always on. */
    kDmaRequestMux0AlwaysOn63 = 63|0x100,     /*!< Always on. */
} dma_request_source_t;

#else
	#error "No valid CPU defined"
#endif

/* @} */

#endif /* __FSL_DMA_REQUEST_H__ */
/*******************************************************************************
 * EOF
 ******************************************************************************/

