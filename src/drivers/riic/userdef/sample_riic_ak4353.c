/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
* Copyright (C) 2012 - 2014 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name   : drv_dac_ak4353.c
* $Rev: 474 $
* $Date:: 2013-10-03 17:13:02 +0900#$
* Description : DAC ak4353 control sample
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "iodefine.h"
#include "dev_drv.h"
#include "devdrv_riic.h"
#include "sample_riic_ak4353.h"
#include "reg_bit_access.h"

/******************************************************************************
Macro definitions
******************************************************************************/

/* ---- DAC Power Down Wait ---- */
#define POWER_DOWN_WAIT_COUNT   (100u)

/* ---- DAC setting table ID ---- */
#define DAC_SETTING_ID_0    (0)
#define DAC_SETTING_ID_1    (1)
#define DAC_SETTING_ID_2    (2)
#define DAC_SETTING_ID_3    (3)
#define DAC_SETTING_ID_4    (4)
#define DAC_SETTING_ID_5    (5)
#define DAC_SETTING_ID_6    (6)
#define NUM_DAC_SETTING_ID  (7)

/* ---- IIC Channel number ---- */
#define DAC_IIC_CH          (2)

#define DAC_IIC_CH_0        (0)
#define DAC_IIC_CH_1        (1)
#define DAC_IIC_CH_2        (2)
#define DAC_IIC_CH_3        (3)

/* ---- IIC transfer size ---- */
#define IIC_TRANSMIT_SIZE_2 (2)
#define IIC_TRANSMIT_SIZE_1 (1)

/* ---- IIC transfer offset ---- */
#define IIC_CHIP_ADDR_POSI  (0)
#define IIC_REG_ADDR_POSI   (1)

/* ---- IIC BRL, BRH ---- */
/* I Clock : 400MHz, B Clock : 133.33MHz */
/* P1 Clock : 66.67MHz, P0 Clock : 33.33MHz */
/* SCL line rising time : 1000ns, SCL line falling time : 300ns */
/* It is a setting example to operate SCL output bitrate at 50kHz under conditions of the above. */
#define CKS_VALUE           RIIC_CKS_DIVISION_16    /* Internal Reference Clock (IIC Clock) : P0 Clock / 16 */
#define BRL_VALUE           (23)                    /* SCL Low width :24 clock */
#define BRH_VALUE           (10)                    /* SCL High width:11 clock */

/* ---- GPIO Bit Data ---- */
#define AK4353_GPIO_CLEAR_VALUE (0)
#define AK4353_GPIO_SET_VALUE   (1)


/* Bit information for GPIO register */
enum gpio_ak4353_bit_inf
{
        GPIO_P4_BIT_14_PDN
    ,   GPIO_P1_BIT_4_SCL2
    ,   GPIO_P1_BIT_5_SDA2
    ,   NUM_GPIO_AK4353_BIT_INF
};

/******************************************************************************
Typedef definitions
******************************************************************************/
/* DAC Setting */
typedef struct dac_setting
{
    uint8_t reg_addr;
    uint8_t data;
} dac_setting_t;

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
/* ==== Prototype Declaration ==== */
static void DevDacAk4353RiicPortInit (void);
static void DevDacAk4353PowerDownPortInit (void);
static int32_t DevDacAk4353RiicInit (void);
static void DevDacAk4353Reset (void);
static void DevDacAk4353PowerDown (void);
static void DevDacAk4353PowerOn (void);
static void DevDacAk4353PowerDownWait(void);
static int32_t DevDacAk4353IicWrite (const ak4353_ch_t dac_ch, const uint8_t reg_addr, const uint8_t data);

/* ==== Global variables ==== */
/* Bit information for GPIO register */
static const uint16_reg_bit_inf_t gpio_ak4353_bit_inf_tbl[NUM_GPIO_AK4353_BIT_INF] =
{
        { 0x4000    ,   14  }   /* GPIO_P4_BIT_14_PDN */
    ,   { 0x0010    ,   4   }   /* GPIO_P1_BIT_4_SCL2 */
    ,   { 0x0020    ,   5   }   /* GPIO_P1_BIT_5_SDA2 */
};

/******************************************************************************
* Function Name: R_DAC_AK4353PowerDown
* Description  : Power down Audio DAC ak4353
* Arguments    : dac_ch -
*                    DAC channel number
*                fs -
*                    sampling rate
* Return Value : DEVDRV_SUCCESS -
*                    Operation successful.
*                DEVDRV_ERROR -
*                    Operation error.
******************************************************************************/
void R_DAC_AK4353PowerDown (void)
{
    DevDacAk4353PowerDownPortInit();
    DevDacAk4353PowerDown();
}

/******************************************************************************
* Function Name: R_DAC_AK4353Init
* Description  : Audio DAC ak4353 setting
* Arguments    : dac_ch -
*                    DAC channel number
*                fs -
*                    sampling rate
* Return Value : DEVDRV_SUCCESS -
*                    Operation successful.
*                DEVDRV_ERROR -
*                    Operation error.
******************************************************************************/
int32_t R_DAC_AK4353Init (const ak4353_ch_t dac_ch, const ak4353_fs_t fs)
{
    dac_setting_t dac_setting_tbl[NUM_DAC_SETTING_ID] =
    {
            { DAC_REG_ADDR0, DAC_DIF_MODE0 }
        ,   { DAC_REG_ADDR1, 0 }
        ,   { DAC_REG_ADDR0, DAC_DIF_MODE0 | DAC_RSTN }
        ,   { DAC_REG_ADDR1, DAC_RSTN}
        ,   { DAC_REG_ADDR2, DAC_PL_STEREO | DAC_DEM_OFF | DAC_ATC_OFF }
        ,   { DAC_REG_ADDR3, DEFAULT_ATT }
        ,   { DAC_REG_ADDR4, DEFAULT_ATT }
    };
    uint32_t    dac_setting_tbl_id_loop;
    int32_t     ret;
    uint8_t     dac_fs_mask = DAC_FS_44_1KHZ;

    /* ==== Initialize the RIIC port for AK4353 control ==== */
    DevDacAk4353RiicPortInit();

    /* ==== Initialize RIIC ==== */
    ret = DevDacAk4353RiicInit();

    if (DEVDRV_ERROR != ret)
    {
        /* ==== Reset AK4353 ==== */
        DevDacAk4353Reset();

        /* ==== Select audio clock ==== */
        switch(fs)
        {
            case AUDIO_FS_44_1KHZ:
                dac_fs_mask = DAC_FS_44_1KHZ;
            break;

            case AUDIO_FS_22_05KHZ:
                dac_fs_mask = DAC_FS_22_05KHZ;
            break;

            case AUDIO_FS_16KHZ:
                dac_fs_mask = DAC_FS_16KHZ;
            break;

            case AUDIO_FS_32KHZ:
                dac_fs_mask = DAC_FS_32KHZ;
            break;

            case AUDIO_FS_24KHZ:
                dac_fs_mask = DAC_FS_24KHZ;
            break;

            case AUDIO_FS_48KHZ:
                dac_fs_mask = DAC_FS_48KHZ;
            break;

            default:
                /* unknown fs */
                ret = DEVDRV_ERROR;
            break;
        }
    }

    if (DEVDRV_ERROR != ret)
    {
        /* ==== Set audio clock ==== */
        COMMON_Set8BitMask(&dac_setting_tbl[DAC_SETTING_ID_1].data, dac_fs_mask);
        COMMON_Set8BitMask(&dac_setting_tbl[DAC_SETTING_ID_3].data, dac_fs_mask);

        /* ==== Write control data ==== */
        for (   dac_setting_tbl_id_loop = DAC_SETTING_ID_0;
                dac_setting_tbl_id_loop < (uint32_t)(sizeof(dac_setting_tbl) /
                                           sizeof(dac_setting_tbl[DAC_SETTING_ID_0]));
                dac_setting_tbl_id_loop++)
        {
            ret = DevDacAk4353IicWrite( dac_ch,
                                        dac_setting_tbl[dac_setting_tbl_id_loop].reg_addr,
                                        dac_setting_tbl[dac_setting_tbl_id_loop].data);
            if (DEVDRV_ERROR == ret)
            {
                break;
            }
        }
    }

    return ret;
}

/******************************************************************************
* Function Name: DevDacAk4353RiicPortInit
* Description  : Initialize the RIIC Port for AK4353 control
* Arguments    : none
* Return Value : none
******************************************************************************/
static void DevDacAk4353RiicPortInit (void)
{
    const uint16_reg_bit_inf_t * gpio_bit_inf;

    /* ==== RIIC Port ==== */
    /* ---- SCL2 ---- */
    gpio_bit_inf = &gpio_ak4353_bit_inf_tbl[GPIO_P1_BIT_4_SCL2];
    COMMON_Set16BitMaskLShift(&GPIO.PIBC1,  gpio_bit_inf, AK4353_GPIO_CLEAR_VALUE); /* Input Buffer:disable */
    COMMON_Set16BitMaskLShift(&GPIO.PBDC1,  gpio_bit_inf, AK4353_GPIO_CLEAR_VALUE); /* Bidirectional mode:disable */
    COMMON_Set16BitMaskLShift(&GPIO.PM1,    gpio_bit_inf, AK4353_GPIO_SET_VALUE);   /* Input mode */
    COMMON_Set16BitMaskLShift(&GPIO.PMC1,   gpio_bit_inf, AK4353_GPIO_CLEAR_VALUE); /* Port mode */
    COMMON_Set16BitMaskLShift(&GPIO.PIPC1,  gpio_bit_inf, AK4353_GPIO_CLEAR_VALUE); /* software I/O control */
    COMMON_Set16BitMaskLShift(&GPIO.PBDC1,  gpio_bit_inf, AK4353_GPIO_SET_VALUE);   /* Bidirectional mode:enabled */
    COMMON_Set16BitMaskLShift(&GPIO.PFC1,   gpio_bit_inf, AK4353_GPIO_CLEAR_VALUE); /* 1st Alternative:SCL2 */
    COMMON_Set16BitMaskLShift(&GPIO.PFCE1,  gpio_bit_inf, AK4353_GPIO_CLEAR_VALUE); /* 1st Alternative:SCL2 */
    COMMON_Set16BitMaskLShift(&GPIO.PFCAE1, gpio_bit_inf, AK4353_GPIO_CLEAR_VALUE); /* 1st Alternative:SCL2 */
    COMMON_Set16BitMaskLShift(&GPIO.PIPC1,  gpio_bit_inf, AK4353_GPIO_SET_VALUE);   /* direct I/O control */
    COMMON_Set16BitMaskLShift(&GPIO.PMC1,   gpio_bit_inf, AK4353_GPIO_SET_VALUE);   /* Alternative mode */

    /* ---- SDA2 ---- */
    gpio_bit_inf = &gpio_ak4353_bit_inf_tbl[GPIO_P1_BIT_5_SDA2];
    COMMON_Set16BitMaskLShift(&GPIO.PIBC1,  gpio_bit_inf, AK4353_GPIO_CLEAR_VALUE); /* Input Buffer:disable */
    COMMON_Set16BitMaskLShift(&GPIO.PBDC1,  gpio_bit_inf, AK4353_GPIO_CLEAR_VALUE); /* Bidirectional mode:disable */
    COMMON_Set16BitMaskLShift(&GPIO.PM1,    gpio_bit_inf, AK4353_GPIO_SET_VALUE);   /* Input mode */
    COMMON_Set16BitMaskLShift(&GPIO.PMC1,   gpio_bit_inf, AK4353_GPIO_CLEAR_VALUE); /* Port mode */
    COMMON_Set16BitMaskLShift(&GPIO.PIPC1,  gpio_bit_inf, AK4353_GPIO_CLEAR_VALUE); /* software I/O control */
    COMMON_Set16BitMaskLShift(&GPIO.PBDC1,  gpio_bit_inf, AK4353_GPIO_SET_VALUE);   /* Bidirectional mode enabled */
    COMMON_Set16BitMaskLShift(&GPIO.PFC1,   gpio_bit_inf, AK4353_GPIO_CLEAR_VALUE); /* 1st Alternative:SDA2 */
    COMMON_Set16BitMaskLShift(&GPIO.PFCE1,  gpio_bit_inf, AK4353_GPIO_CLEAR_VALUE); /* 1st Alternative:SDA2 */
    COMMON_Set16BitMaskLShift(&GPIO.PFCAE1, gpio_bit_inf, AK4353_GPIO_CLEAR_VALUE); /* 1st Alternative:SDA2 */
    COMMON_Set16BitMaskLShift(&GPIO.PIPC1,  gpio_bit_inf, AK4353_GPIO_SET_VALUE);   /* direct I/O control */
    COMMON_Set16BitMaskLShift(&GPIO.PMC1,   gpio_bit_inf, AK4353_GPIO_SET_VALUE);   /* Alternative mode */
}

/******************************************************************************
* Function Name: DevDacAk4353PowerDownPortInit
* Description  : Initialize the Powerdown Port for AK4353 control
* Arguments    : none
* Return Value : none
******************************************************************************/
static void DevDacAk4353PowerDownPortInit (void)
{
    const uint16_reg_bit_inf_t * gpio_bit_inf;

    /* ==== Powerdown Port ==== */
    gpio_bit_inf = &gpio_ak4353_bit_inf_tbl[GPIO_P4_BIT_14_PDN];
    COMMON_Set16BitMaskLShift(&GPIO.PIBC4, gpio_bit_inf, AK4353_GPIO_CLEAR_VALUE);  /* Input Buffer:disable */
    COMMON_Set16BitMaskLShift(&GPIO.PBDC4, gpio_bit_inf, AK4353_GPIO_CLEAR_VALUE);  /* Bidirectional mode:disable */
    COMMON_Set16BitMaskLShift(&GPIO.PM4,   gpio_bit_inf, AK4353_GPIO_SET_VALUE);    /* Input mode */
    COMMON_Set16BitMaskLShift(&GPIO.PMC4,  gpio_bit_inf, AK4353_GPIO_CLEAR_VALUE);  /* Port mode */
    COMMON_Set16BitMaskLShift(&GPIO.PIPC4, gpio_bit_inf, AK4353_GPIO_CLEAR_VALUE);  /* software I/O control */
    COMMON_Set16BitMaskLShift(&GPIO.P4,    gpio_bit_inf, AK4353_GPIO_SET_VALUE);    /* Outputs high level */
    COMMON_Set16BitMaskLShift(&GPIO.PM4,   gpio_bit_inf, AK4353_GPIO_CLEAR_VALUE);  /* Output mode */
}

/******************************************************************************
* Function Name: DevDacAk4353RiicInit
* Description  : Initialize RIIC
* Arguments    : none
* Return Value : DEVDRV_SUCCESS -
*                    Operation successful.
*                DEVDRV_ERROR -
*                    Operation error.
******************************************************************************/
static int32_t DevDacAk4353RiicInit (void)
{
    int32_t ret;

    ret = R_RIIC_Init(AK4353_CHANNEL2);

    return ret;
}

/******************************************************************************
* Function Name: DevDacAk4353Reset
* Description  : Reset ak4353
* Arguments    : none
* Return Value : none
******************************************************************************/
static void DevDacAk4353Reset (void)
{
    /* ---- Set DAC to powerdown mode ---- */
    DevDacAk4353PowerDown();

    /* ---- wait(More than 150ns) ---- */
    DevDacAk4353PowerDownWait();

    /* ---- Set DAC to power on mode ---- */
    DevDacAk4353PowerOn();
}

/******************************************************************************
* Function Name: DevDacPowerDown
* Description  : Set powerdown mode
* Arguments    : none
* Return Value : none
******************************************************************************/
static void DevDacAk4353PowerDown (void)
{
    COMMON_Set16BitMaskLShift(&GPIO.P4, &gpio_ak4353_bit_inf_tbl[GPIO_P4_BIT_14_PDN], AK4353_GPIO_CLEAR_VALUE);
}

/******************************************************************************
* Function Name: DevDacPowerOn
* Description  : Set poweron mode
* Arguments    : none
* Return Value : none
******************************************************************************/
static void DevDacAk4353PowerOn (void)
{
    COMMON_Set16BitMaskLShift(&GPIO.P4, &gpio_ak4353_bit_inf_tbl[GPIO_P4_BIT_14_PDN], AK4353_GPIO_SET_VALUE);
}

/******************************************************************************
* Function Name: DevDacAk4353PowerDownWait
* Description  : Wait for Power Down
*                Waiting time more than 150ns is necessary.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void DevDacAk4353PowerDownWait(void)
{
    volatile uint32_t cnt;

    for (cnt = 0; cnt < POWER_DOWN_WAIT_COUNT; cnt++)
    {
        /* Do Nothing */
    }
}

/******************************************************************************
* Function Name: dev_dac_iic_write
* Description  : IIC write for AK4353
* Arguments    : dac_ch -
*                    DAC channel number
*                reg_addr -
*                    register address
*                data -
*                    control data
*                    flase:Pause OFF
* Return Value : DEVDRV_SUCCESS -
*                    Operation successful.
*                DEVDRV_ERROR -
*                    Operation error.
******************************************************************************/
static int32_t DevDacAk4353IicWrite (const ak4353_ch_t dac_ch, const uint8_t reg_addr, const uint8_t data)
{
    int32_t ret;
    uint8_t buf[IIC_TRANSMIT_SIZE_2];

    /* ==== Build data to 1st transfer data ==== */
    /* ---- Set a chip address ---- */
    if (AK4353_CHANNEL1 == dac_ch)
    {
        buf[IIC_CHIP_ADDR_POSI] = DAC1_CHIP_ADDR;
    }
    else
    {
        buf[IIC_CHIP_ADDR_POSI] = DAC2_CHIP_ADDR;
    }

    /* ---- Set a register address ---- */
    buf[IIC_REG_ADDR_POSI] = reg_addr;

    /* ==== Issue a start condition ==== */
    ret = R_RIIC_SendCond(DAC_IIC_CH, RIIC_TX_MODE_START);

    /* ==== Issue the 1st transfer data ==== */
    if (DEVDRV_SUCCESS == ret)
    {
        ret = R_RIIC_Write(DAC_IIC_CH, buf, IIC_TRANSMIT_SIZE_2);
    }

    /* ==== Issue the 2st transfer data ==== */
    if (DEVDRV_SUCCESS == ret)
    {
        ret = R_RIIC_Write(DAC_IIC_CH, &data, IIC_TRANSMIT_SIZE_1);
    }

    /* ==== Issue a stop condition ==== */
    if (DEVDRV_SUCCESS == ret)
    {
        ret = R_RIIC_SendCond(DAC_IIC_CH, RIIC_TX_MODE_STOP);
    }

    if (DEVDRV_SUCCESS == ret)
    {
        ret = R_RIIC_DetectStop(DAC_IIC_CH);
    }

    return ret;
}

/******************************************************************************
* Function Name: Sample_RIIC_Ri0_Interrupt
* Description  : INTIICRI0 Interrupt function
* Arguments    : int_sense -
*                    Interrupt detection - Not Used
* Return Value : none
******************************************************************************/

#if 1 	//Generallai
void Sample_RIIC_Ri0_Interrupt(uint32_t int_sense)
{
    (void)R_RIIC_RiInterrupt(DAC_IIC_CH_0);
}


/******************************************************************************
* Function Name: Sample_RIIC_Ti0_Interrupt
* Description  : INTIICTI0 Interrupt function
* Arguments    : int_sense -
*                    Interrupt detection - Not Used
* Return Value : none
******************************************************************************/
void Sample_RIIC_Ti0_Interrupt(uint32_t int_sense)
{
    (void)R_RIIC_TiInterrupt(DAC_IIC_CH_0);
}

/******************************************************************************
* Function Name: Sample_RIIC_Tei0_Interrupt
* Description  : INTIICTEI0 Interrupt function
* Arguments    : int_sense -
*                    Interrupt detection - Not Used
* Return Value : none
******************************************************************************/
void Sample_RIIC_Tei0_Interrupt(uint32_t int_sense)
{
    (void)R_RIIC_TeiInterrupt(DAC_IIC_CH_0);
}
#endif
/******************************************************************************
* Function Name: Sample_RIIC_Ri2_Interrupt
* Description  : INTIICRI2 Interrupt function
* Arguments    : int_sense -
*                    Interrupt detection - Not Used
* Return Value : none
******************************************************************************/
void Sample_RIIC_Ri2_Interrupt(uint32_t int_sense)
{
    (void)R_RIIC_RiInterrupt(DAC_IIC_CH_2);
}

/******************************************************************************
* Function Name: Sample_RIIC_Ti2_Interrupt
* Description  : INTIICTI2 Interrupt function
* Arguments    : int_sense -
*                    Interrupt detection - Not Used
* Return Value : none
******************************************************************************/
void Sample_RIIC_Ti2_Interrupt(uint32_t int_sense)
{
    (void)R_RIIC_TiInterrupt(DAC_IIC_CH_2);
}

/******************************************************************************
* Function Name: Sample_RIIC_Tei2_Interrupt
* Description  : INTIICTEI2 Interrupt function
* Arguments    : int_sense -
*                    Interrupt detection - Not Used
* Return Value : none
******************************************************************************/
void Sample_RIIC_Tei2_Interrupt(uint32_t int_sense)
{
    (void)R_RIIC_TeiInterrupt(DAC_IIC_CH_2);
}

/******************************************************************************
* Function Name: Sample_RIIC_Ri3_Interrupt
* Description  : INTIICRI3 Interrupt function
* Arguments    : int_sense -
*                    Interrupt detection - Not Used
* Return Value : none
******************************************************************************/
#if 1				//Generallai

void Sample_RIIC_Ri3_Interrupt(uint32_t int_sense)
{
    (void)R_RIIC_RiInterrupt(DAC_IIC_CH_3);
}

/******************************************************************************
* Function Name: Sample_RIIC_Ti3_Interrupt
* Description  : INTIICTI3 Interrupt function
* Arguments    : int_sense -
*                    Interrupt detection - Not Used
* Return Value : none
******************************************************************************/
void Sample_RIIC_Ti3_Interrupt(uint32_t int_sense)
{
    (void)R_RIIC_TiInterrupt(DAC_IIC_CH_3);
}

/******************************************************************************
* Function Name: Sample_RIIC_Tei3_Interrupt
* Description  : INTIICTEI3 Interrupt function
* Arguments    : int_sense -
*                    Interrupt detection - Not Used
* Return Value : none
******************************************************************************/
void Sample_RIIC_Tei3_Interrupt(uint32_t int_sense)
{
    (void)R_RIIC_TeiInterrupt(DAC_IIC_CH_3);
}
#endif

/* End of File */
