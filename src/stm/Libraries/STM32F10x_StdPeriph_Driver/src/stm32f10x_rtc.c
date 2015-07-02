/*
 * Author: Copyright (C) STMicroelectronics
 *                                              MCD Application Team            Date:   04/06/2009
 *
 * This file is part of Nitrokey.
 *
 * Nitrokey is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * Nitrokey is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Nitrokey. If not, see <http://www.gnu.org/licenses/>.
 */

/* Includes
   ------------------------------------------------------------------ */
#include "stm32f10x_rtc.h"

/** @addtogroup StdPeriph_Driver
  * @{
  */

/** @defgroup RTC
  * @brief RTC driver modules
  * @{
  */

/** @defgroup RTC_Private_TypesDefinitions
  * @{
  */
/**
  * @}
  */

/** @defgroup RTC_Private_Defines
  * @{
  */

#define CRL_CNF_Set      ((uint16_t)0x0010) /* Configuration Flag Enable Mask
                                             */
#define CRL_CNF_Reset    ((uint16_t)0xFFEF) /* Configuration Flag Disable
                                               Mask */
#define RTC_LSB_Mask     ((uint32_t)0x0000FFFF) /* RTC LSB Mask */
#define PRLH_MSB_Mask    ((uint32_t)0x000F0000) /* RTC Prescaler MSB Mask */

/**
  * @}
  */

/** @defgroup RTC_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup RTC_Private_Variables
  * @{
  */

/**
  * @}
  */

/** @defgroup RTC_Private_FunctionPrototypes
  * @{
  */

/**
  * @}
  */

/** @defgroup RTC_Private_Functions
  * @{
  */

/**
  * @brief  Enables or disables the specified RTC interrupts.
  * @param RTC_IT: specifies the RTC interrupts sources to be enabled or
  *                disabled.
  *   This parameter can be any combination of the following values:
  * @arg RTC_IT_OW: Overflow interrupt
  * @arg RTC_IT_ALR: Alarm interrupt
  * @arg RTC_IT_SEC: Second interrupt
  * @param NewState: new state of the specified RTC interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval : None
  */
void RTC_ITConfig (uint16_t RTC_IT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param (IS_RTC_IT (RTC_IT));
    assert_param (IS_FUNCTIONAL_STATE (NewState));

    if (NewState != DISABLE)
    {
        RTC->CRH |= RTC_IT;
    }
    else
    {
        RTC->CRH &= (uint16_t) ~ RTC_IT;
    }
}

/**
  * @brief  Enters the RTC configuration mode.
  * @param  None
  * @retval : None
  */
void RTC_EnterConfigMode (void)
{
    /* Set the CNF flag to enter in the Configuration Mode */
    RTC->CRL |= CRL_CNF_Set;
}

/**
  * @brief  Exits from the RTC configuration mode.
  * @param  None
  * @retval : None
  */
void RTC_ExitConfigMode (void)
{
    /* Reset the CNF flag to exit from the Configuration Mode */
    RTC->CRL &= CRL_CNF_Reset;
}

/**
  * @brief  Gets the RTC counter value.
  * @param  None
  * @retval : RTC counter value.
  */
uint32_t RTC_GetCounter (void)
{
    uint16_t tmp = 0;

    tmp = RTC->CNTL;
    return (((uint32_t) RTC->CNTH << 16) | tmp);
}

/**
  * @brief  Sets the RTC counter value.
  * @param CounterValue: RTC counter new value.
  * @retval : None
  */
void RTC_SetCounter (uint32_t CounterValue)
{
    RTC_EnterConfigMode ();
    /* Set RTC COUNTER MSB word */
    RTC->CNTH = CounterValue >> 16;
    /* Set RTC COUNTER LSB word */
    RTC->CNTL = (CounterValue & RTC_LSB_Mask);
    RTC_ExitConfigMode ();
}

/**
  * @brief  Sets the RTC prescaler value.
  * @param PrescalerValue: RTC prescaler new value.
  * @retval : None
  */
void RTC_SetPrescaler (uint32_t PrescalerValue)
{
    /* Check the parameters */
    assert_param (IS_RTC_PRESCALER (PrescalerValue));

    RTC_EnterConfigMode ();
    /* Set RTC PRESCALER MSB word */
    RTC->PRLH = (PrescalerValue & PRLH_MSB_Mask) >> 16;
    /* Set RTC PRESCALER LSB word */
    RTC->PRLL = (PrescalerValue & RTC_LSB_Mask);
    RTC_ExitConfigMode ();
}

/**
  * @brief  Sets the RTC alarm value.
  * @param AlarmValue: RTC alarm new value.
  * @retval : None
  */
void RTC_SetAlarm (uint32_t AlarmValue)
{
    RTC_EnterConfigMode ();
    /* Set the ALARM MSB word */
    RTC->ALRH = AlarmValue >> 16;
    /* Set the ALARM LSB word */
    RTC->ALRL = (AlarmValue & RTC_LSB_Mask);
    RTC_ExitConfigMode ();
}

/**
  * @brief  Gets the RTC divider value.
  * @param  None
  * @retval : RTC Divider value.
  */
uint32_t RTC_GetDivider (void)
{
    uint32_t tmp = 0x00;

    tmp = ((uint32_t) RTC->DIVH & (uint32_t) 0x000F) << 16;
    tmp |= RTC->DIVL;
    return tmp;
}

/**
  * @brief  Waits until last write operation on RTC registers has finished.
  *   This function must be called before any write to RTC registers.
  * @param  None
  * @retval : None
  */
void RTC_WaitForLastTask (void)
{
    /* Loop until RTOFF flag is set */
    while ((RTC->CRL & RTC_FLAG_RTOFF) == (uint16_t) RESET)
    {
    }
}

/**
  * @brief  Waits until the RTC registers (RTC_CNT, RTC_ALR and RTC_PRL)
  *   are synchronized with RTC APB clock.
  *   This function must be called before any read operation after
  *   an APB reset or an APB clock stop.
  * @param  None
  * @retval : None
  */
void RTC_WaitForSynchro (void)
{
    /* Clear RSF flag */
    RTC->CRL &= (uint16_t) ~ RTC_FLAG_RSF;
    /* Loop until RSF flag is set */
    while ((RTC->CRL & RTC_FLAG_RSF) == (uint16_t) RESET)
    {
    }
}

/**
  * @brief  Checks whether the specified RTC flag is set or not.
  * @param RTC_FLAG: specifies the flag to check.
  *   This parameter can be one the following values:
  * @arg RTC_FLAG_RTOFF: RTC Operation OFF flag
  * @arg RTC_FLAG_RSF: Registers Synchronized flag
  * @arg RTC_FLAG_OW: Overflow flag
  * @arg RTC_FLAG_ALR: Alarm flag
  * @arg RTC_FLAG_SEC: Second flag
  * @retval : The new state of RTC_FLAG (SET or RESET).
  */
FlagStatus RTC_GetFlagStatus (uint16_t RTC_FLAG)
{
FlagStatus bitstatus = RESET;

    /* Check the parameters */
    assert_param (IS_RTC_GET_FLAG (RTC_FLAG));

    if ((RTC->CRL & RTC_FLAG) != (uint16_t) RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }
    return bitstatus;
}

/**
  * @brief  Clears the RTC�s pending flags.
  * @param RTC_FLAG: specifies the flag to clear.
  *   This parameter can be any combination of the following values:
  * @arg RTC_FLAG_RSF: Registers Synchronized flag. This flag
  *   is cleared only after an APB reset or an APB Clock stop.
  * @arg RTC_FLAG_OW: Overflow flag
  * @arg RTC_FLAG_ALR: Alarm flag
  * @arg RTC_FLAG_SEC: Second flag
  * @retval : None
  */
void RTC_ClearFlag (uint16_t RTC_FLAG)
{
    /* Check the parameters */
    assert_param (IS_RTC_CLEAR_FLAG (RTC_FLAG));

    /* Clear the coressponding RTC flag */
    RTC->CRL &= (uint16_t) ~ RTC_FLAG;
}

/**
  * @brief  Checks whether the specified RTC interrupt has occured or not.
  * @param RTC_IT: specifies the RTC interrupts sources to check.
  *   This parameter can be one of the following values:
  * @arg RTC_IT_OW: Overflow interrupt
  * @arg RTC_IT_ALR: Alarm interrupt
  * @arg RTC_IT_SEC: Second interrupt
  * @retval : The new state of the RTC_IT (SET or RESET).
  */
ITStatus RTC_GetITStatus (uint16_t RTC_IT)
{
ITStatus bitstatus = RESET;

    /* Check the parameters */
    assert_param (IS_RTC_GET_IT (RTC_IT));

    bitstatus = (ITStatus) (RTC->CRL & RTC_IT);
    if (((RTC->CRH & RTC_IT) != (uint16_t) RESET)
        && (bitstatus != (uint16_t) RESET))
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }
    return bitstatus;
}

/**
  * @brief  Clears the RTC�s interrupt pending bits.
  * @param RTC_IT: specifies the interrupt pending bit to clear.
  *   This parameter can be any combination of the following values:
  * @arg RTC_IT_OW: Overflow interrupt
  * @arg RTC_IT_ALR: Alarm interrupt
  * @arg RTC_IT_SEC: Second interrupt
  * @retval : None
  */
void RTC_ClearITPendingBit (uint16_t RTC_IT)
{
    /* Check the parameters */
    assert_param (IS_RTC_IT (RTC_IT));

    /* Clear the coressponding RTC pending bit */
    RTC->CRL &= (uint16_t) ~ RTC_IT;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
