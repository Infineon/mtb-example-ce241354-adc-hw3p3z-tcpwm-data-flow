/*******************************************************************************
* File Name:   main.c
*
* Description: This is the main file for the PPCA CPU core 0. This file contains
* the main function for the core.
*
* Related Document: See README.md
*
*
********************************************************************************
* (c) 2026, Infineon Technologies AG, or an affiliate of Infineon
* Technologies AG. All rights reserved.
* This software, associated documentation and materials ("Software") is
* owned by Infineon Technologies AG or one of its affiliates ("Infineon")
* and is protected by and subject to worldwide patent protection, worldwide
* copyright laws, and international treaty provisions. Therefore, you may use
* this Software only as provided in the license agreement accompanying the
* software package from which you obtained this Software. If no license
* agreement applies, then any use, reproduction, modification, translation, or
* compilation of this Software is prohibited without the express written
* permission of Infineon.
*
* Disclaimer: UNLESS OTHERWISE EXPRESSLY AGREED WITH INFINEON, THIS SOFTWARE
* IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING, BUT NOT LIMITED TO, ALL WARRANTIES OF NON-INFRINGEMENT OF
* THIRD-PARTY RIGHTS AND IMPLIED WARRANTIES SUCH AS WARRANTIES OF FITNESS FOR A
* SPECIFIC USE/PURPOSE OR MERCHANTABILITY.
* Infineon reserves the right to make changes to the Software without notice.
* You are responsible for properly designing, programming, and testing the
* functionality and safety of your intended application of the Software, as
* well as complying with any legal requirements related to its use. Infineon
* does not guarantee that the Software will be free from intrusion, data theft
* or loss, or other breaches ("Security Breaches"), and Infineon shall have
* no liability arising out of any Security Breaches. Unless otherwise
* explicitly approved by Infineon, the Software may not be used in any
* application where a failure of the Product or any consequences of the use
* thereof can reasonably be expected to result in personal injury.
*******************************************************************************/

/*******************************************************************************
* Header Files
*******************************************************************************/
#include "cy_pdl.h"
#include "cycfg.h"
    
/******************************************************************************
* Macros
*******************************************************************************/
/* Shared memory addresses in M4 shared memory space (0x20040000-0x20043FFF) */
/* All cores can access M4 shared memory for inter-core communication */
#define PPCA_CPU0_M4_VAR_ADDRESS 0x20040400  /* Used by this core (CPU0) */

/*******************************************************************************
* Global Variables
*******************************************************************************/

/*******************************************************************************
* Function Prototypes
*******************************************************************************/

/*******************************************************************************
* Function Definitions
*******************************************************************************/

/*******************************************************************************
* Function Name: main
*********************************************************************************
* Summary:
* This is the main function for PPCA CPU 0. It performs the initialization of the
* variables used in the code. Also, it reads the ADC read data, ADC filter data
* and Hardware filter data in periodic intervals and copies the date to the
* shared memory for consuming by the main CPU.
*
* Parameters:
*  void
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    int32_t *adc_data           = (int32_t *)PPCA_CPU0_M4_VAR_ADDRESS;
    int32_t *adc_filter_out     = adc_data       + 1;
    int32_t *lp_filter_out      = adc_filter_out + 1;
    int32_t *core0_data_updated = lp_filter_out  + 1;

    for(;;)
    {
        Cy_SysLib_Delay(250);
        /* Reading ADC read data */
        *adc_data = Cy_PPCA_ADC_Read_ADC_Data(ADC_HW, 0);

        /* Reading ADC filter data */
        *adc_filter_out = Cy_PPCA_ADC_Filter_LPF_Output(ADC_FILTER_HW);

        /* Reading hardware filter data */
        *lp_filter_out = Cy_PPCA_HWFILT3P3Z_ReadFilterDataOutput(HWFILTER_HW);

        /* flag to indicate new data */
        *core0_data_updated = 1;
    }
}
