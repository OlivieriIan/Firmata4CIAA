/* Copyright 2016, Ian Olivieri <ianolivieri93@gmail.com>
 * All rights reserved.
 *
 * This file is part of the Firmata4CIAA program.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */
 
/*
 * For more information about the ADC and DAC peripherals, 
 * refer to the Chapter 48 and 49, repectively,  of the LPC43xx user manual
 */


/*==================[inclusions]=============================================*/
#include "chip.h"
/* Specific modules used:
   #include "adc_18xx_43xx.h" for Chip_ADC funtions
   #include "dac_18xx_43xx.h" for Chip_DAC funtions
*/

#include "sAPI_DataTypes.h"
#include "sAPI_PeripheralMap.h"

#include "sAPI_AnalogIO.h"


/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/


/*
 * @brief:   enable/disable the ADC and DAC peripheral
 * @param:   enable: 1 to enable, 0 to disable.
 * @return:   none
*/
void analogConfig(bool_t enable)
{
   if(enable)
   {
         /* Enable ADC peripheral */
         ADC_CLOCK_SETUP_T ADCSetup;

         /* Initialized to default values:
		  *   - Sample rate:ADC_MAX_SAMPLE_RATE=400KHz
		  *   - resolution: ADC_10BITS
		  *   - burst mode: DISABLE */
         Chip_ADC_Init(LPC_ADC0,&ADCSetup);
         /* Disable burst mode */
         Chip_ADC_SetBurstCmd(LPC_ADC0, DISABLE);
         /* Set sample rate to 200KHz */
         Chip_ADC_SetSampleRate(LPC_ADC0, &ADCSetup,ADC_MAX_SAMPLE_RATE/2 );
         /* Disable all channels */
         Chip_ADC_EnableChannel(LPC_ADC0,ADC_CH1,DISABLE);
         Chip_ADC_Int_SetChannelCmd(LPC_ADC0,ADC_CH1,DISABLE);

         Chip_ADC_EnableChannel(LPC_ADC0,ADC_CH2,DISABLE);
         Chip_ADC_Int_SetChannelCmd(LPC_ADC0,ADC_CH2,DISABLE);

         Chip_ADC_EnableChannel(LPC_ADC0,ADC_CH3,DISABLE);
         Chip_ADC_Int_SetChannelCmd(LPC_ADC0,ADC_CH3,DISABLE);

         /* Source: http://docs.lpcware.com/lpcopen/v1.03/lpc18xx__43xx_2examples_2periph_2periph__adc_2adc_8c_source.html
          Line 221 */

         /* Enable DAC peripheral */
		 
         /* Initialize the DAC peripheral */
         Chip_DAC_Init(LPC_DAC);
		 
		 /* Set the update rate to the lowest */
		 Chip_DAC_SetBias(LPC_DAC, DAC_MAX_UPDATE_RATE_400kHz);

         /* Enables the DMA operation and controls DMA timer */
         Chip_DAC_ConfigDAConverterControl(LPC_DAC, DAC_DMA_ENA);
		
         /* Reset DAC value */
         Chip_DAC_UpdateValue(LPC_DAC, 0);
         
   }
   else
   {
      /* Disable ADC peripheral */
      Chip_ADC_DeInit(LPC_ADC0);

      /* Disable DAC peripheral */
	  Chip_DAC_DeInit(LPC_DAC);
   }
}

/*
 * @brief   Write a value in the DAC.
 * @param   value: value to be writen in the DAC, from 0 to 1023
 * @return   none
 */
void analogWrite(uint32_t value)
{
   if(value>1023){value=1023;} 
   Chip_DAC_UpdateValue(LPC_DAC, value);
}


/*
 * @brief   Get the value of one ADC channel. Mode: BLOCKING
 * @param   channel: number of channel, from ACH1 to ACH3 (1 to 3)
 * @return   value of the channel
 */
uint16_t analogRead(uint8_t channel)
{
   uint16_t channelValue = 0;

   Chip_ADC_EnableChannel(LPC_ADC0,channel,ENABLE);
   Chip_ADC_SetStartMode(LPC_ADC0, ADC_START_NOW, ADC_TRIGGERMODE_RISING);

   while((Chip_ADC_ReadStatus(LPC_ADC0 , channel , ADC_DR_DONE_STAT) != SET));
   Chip_ADC_ReadValue(LPC_ADC0 , channel, &channelValue);

   Chip_ADC_EnableChannel(LPC_ADC0,channel,DISABLE);

   return(channelValue);
}

/*==================[end of file]============================================*/