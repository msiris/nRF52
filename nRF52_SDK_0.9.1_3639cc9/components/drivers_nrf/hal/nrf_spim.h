/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/**
 * @defgroup nrf_spim_hal SPIM HAL
 * @{
 * @ingroup nrf_spi_master
 *
 * @brief Hardware abstraction layer for accessing the SPIM peripheral.
 */

#ifndef NRF_SPIM_H__
#define NRF_SPIM_H__

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "nrf.h"


/**
 * @brief Value that may be used as a parameter in the nrf_spim_pins_set()
 *        function call to specify that a given SPI signal (SCK, MOSI or MISO)
 *        shall not be connected to any physical pin.
 */
#define NRF_SPIM_PIN_NOT_CONNECTED  0xFFFFFFFF


/**
 * @brief SPIM tasks.
 */
typedef enum
{
    /*lint -save -e30*/
    NRF_SPIM_TASK_START   = offsetof(NRF_SPIM_Type, TASKS_START),   ///< Start SPI transaction.
    NRF_SPIM_TASK_STOP    = offsetof(NRF_SPIM_Type, TASKS_STOP),    ///< Stop SPI transaction.
    NRF_SPIM_TASK_SUSPEND = offsetof(NRF_SPIM_Type, TASKS_SUSPEND), ///< Suspend SPI transaction.
    NRF_SPIM_TASK_RESUME  = offsetof(NRF_SPIM_Type, TASKS_RESUME)   ///< Resume SPI transaction.
    /*lint -restore*/
} nrf_spim_task_t;

/**
 * @brief SPIM events.
 */
typedef enum
{
    /*lint -save -e30*/
    NRF_SPIM_EVENT_STOPPED = offsetof(NRF_SPIM_Type, EVENTS_STOPPED), ///< SPI transaction has stopped.
    NRF_SPIM_EVENT_ENDRX   = offsetof(NRF_SPIM_Type, EVENTS_ENDRX),   ///< End of RXD buffer reached.
#ifdef NRF52
    NRF_SPIM_EVENT_END     = offsetof(NRF_SPIM_Type, EVENTS_END),     ///< End of RXD buffer and TXD buffer reached.
#endif
    NRF_SPIM_EVENT_ENDTX   = offsetof(NRF_SPIM_Type, EVENTS_ENDTX),   ///< End of TXD buffer reached.
    NRF_SPIM_EVENT_STARTED = offsetof(NRF_SPIM_Type, EVENTS_STARTED)  ///< Transaction started.
    /*lint -restore*/
} nrf_spim_event_t;

#ifdef NRF52
/**
 * @brief SPIM shortcuts.
 */
typedef enum
{
    NRF_SPIM_SHORT_END_START_MASK = SPIM_SHORTS_END_START_Msk ///< Shortcut between END event and START task.
} nrf_spim_short_mask_t;
#endif

/**
 * @brief SPIM interrupts.
 */
typedef enum
{
    NRF_SPIM_INT_STOPPED_MASK = SPIM_INTENSET_STOPPED_Msk, ///< Interrupt on STOPPED event.
    NRF_SPIM_INT_ENDRX_MASK   = SPIM_INTENSET_ENDRX_Msk,   ///< Interrupt on ENDRX event.
#ifdef NRF52
    NRF_SPIM_INT_END_MASK     = SPIM_INTENSET_END_Msk,     ///< Interrupt on END event.
#endif
    NRF_SPIM_INT_ENDTX_MASK   = SPIM_INTENSET_ENDTX_Msk,   ///< Interrupt on ENDTX event.
    NRF_SPIM_INT_STARTED_MASK = SPIM_INTENSET_STARTED_Msk  ///< Interrupt on STARTED event.
} nrf_spim_int_mask_t;

/**
 * @brief SPI master data rates.
 */
typedef enum
{
    NRF_SPIM_FREQ_125K = SPIM_FREQUENCY_FREQUENCY_K125,   ///< 125 kbps.
    NRF_SPIM_FREQ_250K = SPIM_FREQUENCY_FREQUENCY_K250,   ///< 250 kbps.
    NRF_SPIM_FREQ_500K = SPIM_FREQUENCY_FREQUENCY_K500,   ///< 500 kbps.
    NRF_SPIM_FREQ_1M   = SPIM_FREQUENCY_FREQUENCY_M1,     ///< 1 Mbps.
    NRF_SPIM_FREQ_2M   = SPIM_FREQUENCY_FREQUENCY_M2,     ///< 2 Mbps.
    NRF_SPIM_FREQ_4M   = SPIM_FREQUENCY_FREQUENCY_M4,     ///< 4 Mbps.
    // [conversion to 'int' needed to prevent compilers from complaining
    //  that the provided value (0x80000000UL) is out of range of "int"]
    NRF_SPIM_FREQ_8M   = (int)SPIM_FREQUENCY_FREQUENCY_M8 ///< 8 Mbps.
} nrf_spim_frequency_t;

/**
 * @brief SPI modes.
 */
typedef enum
{
    NRF_SPIM_MODE_0, ///< SCK active high, sample on leading edge of clock.
    NRF_SPIM_MODE_1, ///< SCK active high, sample on trailing edge of clock.
    NRF_SPIM_MODE_2, ///< SCK active low, sample on leading edge of clock.
    NRF_SPIM_MODE_3  ///< SCK active low, sample on trailing edge of clock.
} nrf_spim_mode_t;

/**
 * @brief SPI bit orders.
 */
typedef enum
{
    NRF_SPIM_BIT_ORDER_MSB_FIRST = SPIM_CONFIG_ORDER_MsbFirst, ///< Most significant bit shifted out first.
    NRF_SPIM_BIT_ORDER_LSB_FIRST = SPIM_CONFIG_ORDER_LsbFirst  ///< Least significant bit shifted out first.
} nrf_spim_bit_order_t;


/**
 * @brief Function for activating a specific SPIM task.
 *
 * @param[in] p_spim    SPIM instance.
 * @param     spim_task Task to activate.
 */
__STATIC_INLINE void nrf_spim_task_trigger(NRF_SPIM_Type * p_spim,
                                           nrf_spim_task_t spim_task);

/**
 * @brief Function for getting the address of a specific SPIM task register.
 *
 * @param[in] p_spim    SPIM instance.
 * @param     spim_task Desired task.
 *
 * @return Address of specified task register.
 */
__STATIC_INLINE uint32_t * nrf_spim_task_address_get(NRF_SPIM_Type * p_spim,
                                                     nrf_spim_task_t spim_task);

/**
 * @brief Function for clearing a specific SPIM event.
 *
 * @param[in] p_spim     SPIM instance.
 * @param     spim_event Event to clear.
 */
__STATIC_INLINE void nrf_spim_event_clear(NRF_SPIM_Type * p_spim,
                                          nrf_spim_event_t spim_event);

/**
 * @brief Function for checking the state of a specific SPIM event.
 *
 * @param[in] p_spim     SPIM instance.
 * @param     spim_event Event to check.
 *
 * @retval true  if event is set,
 * @retval false otherwise.
 */
__STATIC_INLINE bool nrf_spim_event_check(NRF_SPIM_Type * p_spim,
                                          nrf_spim_event_t spim_event);

/**
 * @brief Function for returning the address of a specific SPIM event register.
 *
 * @param[in] p_spim     SPIM instance.
 * @param     spim_event Desired event.
 *
 * @return Address of specified event register.
 */
__STATIC_INLINE uint32_t * nrf_spim_event_address_get(NRF_SPIM_Type  * p_spim,
                                                      nrf_spim_event_t spim_event);
#ifdef NRF52
/**
 * @brief Function for enabling specified shortcuts.
 *
 * @param[in] p_spim           SPIM instance.
 * @param     spim_shorts_mask Shortcuts to enable.
 */
__STATIC_INLINE void nrf_spim_shorts_enable(NRF_SPIM_Type * p_spim,
                                            uint32_t spim_shorts_mask);

/**
 * @brief Function for disabling specified shortcuts.
 *
 * @param[in] p_spim           SPIM instance.
 * @param     spim_shorts_mask Shortcuts to disable.
 */
__STATIC_INLINE void nrf_spim_shorts_disable(NRF_SPIM_Type * p_spim,
                                             uint32_t spim_shorts_mask);
#endif
/**
 * @brief Function for enabling specified interrupts.
 *
 * @param[in] p_spim        SPIM instance.
 * @param     spim_int_mask Interrupts to enable.
 */
__STATIC_INLINE void nrf_spim_int_enable(NRF_SPIM_Type * p_spim,
                                         uint32_t spim_int_mask);

/**
 * @brief Function for disabling specified interrupts.
 *
 * @param[in] p_spim        SPIM instance.
 * @param     spim_int_mask Interrupts to disable.
 */
__STATIC_INLINE void nrf_spim_int_disable(NRF_SPIM_Type * p_spim,
                                          uint32_t spim_int_mask);

/**
 * @brief Function for retrieving the state of a given interrupt.
 *
 * @param[in] p_spim   SPIM instance.
 * @param     spim_int Interrupt to check.
 *
 * @retval true  if the interrupt is enabled,
 * @retval false otherwise.
 */
__STATIC_INLINE bool nrf_spim_int_enable_check(NRF_SPIM_Type * p_spim,
                                               nrf_spim_int_mask_t spim_int);

/**
 * @brief Function for enabling the SPIM peripheral.
 *
 * @param[in] p_spim SPIM instance.
 */
__STATIC_INLINE void nrf_spim_enable(NRF_SPIM_Type * p_spim);

/**
 * @brief Function for disabling the SPIM peripheral.
 *
 * @param[in] p_spim SPIM instance.
 */
__STATIC_INLINE void nrf_spim_disable(NRF_SPIM_Type * p_spim);

/**
 * @brief Function for configuring SPIM pins.
 *
 * If a given signal is not needed, pass the @ref NRF_SPIM_PIN_NOT_CONNECTED
 * value instead of its pin number.
 *
 * @param[in] p_spim    SPIM instance.
 * @param     sck_pin   SCK pin number.
 * @param     mosi_pin  MOSI pin number.
 * @param     miso_pin  MISO pin number.
 */
__STATIC_INLINE void nrf_spim_pins_set(NRF_SPIM_Type * p_spim,
                                       uint32_t sck_pin,
                                       uint32_t mosi_pin,
                                       uint32_t miso_pin);

/**
 * @brief Function for setting the SPI master data rate.
 *
 * @param[in] p_spim    SPIM instance.
 * @param     frequency SPI frequency.
 */
__STATIC_INLINE void nrf_spim_frequency_set(NRF_SPIM_Type * p_spim,
                                            nrf_spim_frequency_t frequency);

/**
 * @brief Function for setting the transmit buffer.
 *
 * @param[in]  p_spim   SPIM instance.
 * @param[in]  p_buffer Pointer to the buffer with data to send.
 * @param      length   Maximum number of data bytes to transmit.
 */
__STATIC_INLINE void nrf_spim_tx_buffer_set(NRF_SPIM_Type * p_spim,
                                            uint8_t const * p_buffer,
                                            uint8_t         length);

/**
 * @brief Function for setting the receive buffer.
 *
 * @param[in] p_spim   SPIM instance.
 * @param[in] p_buffer Pointer to the buffer for received data.
 * @param     length   Maximum number of data bytes to receive.
 */
__STATIC_INLINE void nrf_spim_rx_buffer_set(NRF_SPIM_Type * p_spim,
                                            uint8_t * p_buffer,
                                            uint8_t   length);

/**
 * @brief Function for setting the SPI configuration.
 *
 * @param[in] p_spim        SPIM instance.
 * @param     spi_mode      SPI mode.
 * @param     spi_bit_order SPI bit order.
 */
__STATIC_INLINE void nrf_spim_configure(NRF_SPIM_Type * p_spim,
                                        nrf_spim_mode_t spi_mode,
                                        nrf_spim_bit_order_t spi_bit_order);

/**
 * @brief Function for setting the over-read character.
 *
 * @param[in] p_spim SPIM instance.
 * @param     orc    Over-read character. Character clocked out in case of
 *                   over-read of the TXD buffer.
 */
__STATIC_INLINE void nrf_spim_orc_set(NRF_SPIM_Type * p_spim,
                                      uint8_t orc);


#ifndef SUPPRESS_INLINE_IMPLEMENTATION

__STATIC_INLINE void nrf_spim_task_trigger(NRF_SPIM_Type * p_spim,
                                           nrf_spim_task_t spim_task)
{
    *((volatile uint32_t *)((uint8_t *)p_spim + (uint32_t)spim_task)) = 0x1UL;
}

__STATIC_INLINE uint32_t * nrf_spim_task_address_get(NRF_SPIM_Type * p_spim,
                                                     nrf_spim_task_t spim_task)
{
    return (uint32_t *)((uint8_t *)p_spim + (uint32_t)spim_task);
}

__STATIC_INLINE void nrf_spim_event_clear(NRF_SPIM_Type * p_spim,
                                          nrf_spim_event_t spim_event)
{
    *((volatile uint32_t *)((uint8_t *)p_spim + (uint32_t)spim_event)) = 0x0UL;
}

__STATIC_INLINE bool nrf_spim_event_check(NRF_SPIM_Type * p_spim,
                                          nrf_spim_event_t spim_event)
{
    return (bool)*(volatile uint32_t *)((uint8_t *)p_spim + (uint32_t)spim_event);
}

__STATIC_INLINE uint32_t * nrf_spim_event_address_get(NRF_SPIM_Type * p_spim,
                                                      nrf_spim_event_t spim_event)
{
    return (uint32_t *)((uint8_t *)p_spim + (uint32_t)spim_event);
}

#ifdef NRF52
__STATIC_INLINE void nrf_spim_shorts_enable(NRF_SPIM_Type * p_spim,
                                            uint32_t spim_shorts_mask)
{
    p_spim->SHORTS |= spim_shorts_mask;
}

__STATIC_INLINE void nrf_spim_shorts_disable(NRF_SPIM_Type * p_spim,
                                             uint32_t spim_shorts_mask)
{
    p_spim->SHORTS &= ~(spim_shorts_mask);
}
#endif
__STATIC_INLINE void nrf_spim_int_enable(NRF_SPIM_Type * p_spim,
                                         uint32_t spim_int_mask)
{
    p_spim->INTENSET = spim_int_mask;
}

__STATIC_INLINE void nrf_spim_int_disable(NRF_SPIM_Type * p_spim,
                                          uint32_t spim_int_mask)
{
    p_spim->INTENCLR = spim_int_mask;
}

__STATIC_INLINE bool nrf_spim_int_enable_check(NRF_SPIM_Type * p_spim,
                                               nrf_spim_int_mask_t spim_int)
{
    return (bool)(p_spim->INTENSET & spim_int);
}

__STATIC_INLINE void nrf_spim_enable(NRF_SPIM_Type * p_spim)
{
    p_spim->ENABLE = (SPIM_ENABLE_ENABLE_Enabled << SPIM_ENABLE_ENABLE_Pos);
}

__STATIC_INLINE void nrf_spim_disable(NRF_SPIM_Type * p_spim)
{
    p_spim->ENABLE = (SPIM_ENABLE_ENABLE_Disabled << SPIM_ENABLE_ENABLE_Pos);
}

__STATIC_INLINE void nrf_spim_pins_set(NRF_SPIM_Type * p_spim,
                                       uint32_t sck_pin,
                                       uint32_t mosi_pin,
                                       uint32_t miso_pin)
{
    p_spim->PSEL.SCK  = sck_pin;
    p_spim->PSEL.MOSI = mosi_pin;
    p_spim->PSEL.MISO = miso_pin;
}

__STATIC_INLINE void nrf_spim_frequency_set(NRF_SPIM_Type * p_spim,
                                            nrf_spim_frequency_t frequency)
{
    p_spim->FREQUENCY = frequency;
}

__STATIC_INLINE void nrf_spim_tx_buffer_set(NRF_SPIM_Type * p_spim,
                                            uint8_t const * p_buffer,
                                            uint8_t         length)
{
    p_spim->TXD.PTR    = (uint32_t)p_buffer;
    p_spim->TXD.MAXCNT = length;
}

__STATIC_INLINE void nrf_spim_rx_buffer_set(NRF_SPIM_Type * p_spim,
                                            uint8_t * p_buffer,
                                            uint8_t   length)
{
    p_spim->RXD.PTR    = (uint32_t)p_buffer;
    p_spim->RXD.MAXCNT = length;
}

__STATIC_INLINE void nrf_spim_configure(NRF_SPIM_Type * p_spim,
                                        nrf_spim_mode_t spi_mode,
                                        nrf_spim_bit_order_t spi_bit_order)
{
    uint32_t config = (spi_bit_order == NRF_SPIM_BIT_ORDER_MSB_FIRST ?
        SPIM_CONFIG_ORDER_MsbFirst : SPIM_CONFIG_ORDER_LsbFirst);
    switch (spi_mode)
    {
    default:
    case NRF_SPIM_MODE_0:
        config |= (SPIM_CONFIG_CPOL_ActiveHigh << SPIM_CONFIG_CPOL_Pos) |
                  (SPIM_CONFIG_CPHA_Leading    << SPIM_CONFIG_CPHA_Pos);
        break;

    case NRF_SPIM_MODE_1:
        config |= (SPIM_CONFIG_CPOL_ActiveHigh << SPIM_CONFIG_CPOL_Pos) |
                  (SPIM_CONFIG_CPHA_Trailing   << SPIM_CONFIG_CPHA_Pos);
        break;

    case NRF_SPIM_MODE_2:
        config |= (SPIM_CONFIG_CPOL_ActiveLow  << SPIM_CONFIG_CPOL_Pos) |
                  (SPIM_CONFIG_CPHA_Leading    << SPIM_CONFIG_CPHA_Pos);
        break;

    case NRF_SPIM_MODE_3:
        config |= (SPIM_CONFIG_CPOL_ActiveLow  << SPIM_CONFIG_CPOL_Pos) |
                  (SPIM_CONFIG_CPHA_Trailing   << SPIM_CONFIG_CPHA_Pos);
        break;
    }
    p_spim->CONFIG = config;
}

__STATIC_INLINE void nrf_spim_orc_set(NRF_SPIM_Type * p_spim,
                                      uint8_t orc)
{
    p_spim->ORC = orc;
}

#endif // SUPPRESS_INLINE_IMPLEMENTATION

#endif // NRF_SPIM_H__

/** @} */
