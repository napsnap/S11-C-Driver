/**
 * BSD 3-Clause License
 * 
 * Copyright (c) 2021, Jhonatan Napadow
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **/

/**
 * @brief Function headers for Senseair Sunrise CO2 sensor (S11) driver
 * @file s11.h
 * @date 2021-03-11
 * @author Jhonatan Napadow
 */
 
  /** @defgroup core Driver core
 *  API functions for interacting with the driver
 */
 
  /** @defgroup info_status Sensor information and status
 *  API for reading sensor information and status
 */
  
 /** @defgroup calibration Calibration
 *  API for (re-)calibration of sensor
 */
 
 /** @defgroup measurement Measurement
 *  API for measuring CO2
 */
 
 /** @defgroup settings Settings
 *  API for changing sensors internal settings
 */

#ifndef S11_H_
#define S11_H_

#include "s11_defs.h"

// C++
#ifdef __cplusplus
extern "C" {
#endif


// Function prototype declarations


/**
 *  @brief Initialize the device,
 *  reads configuration and device id from the sensor.
 *  @param[in,out] dev : Instance of S11 structure
 *  @return Execution status, defined in s11_defs.h, S11_E_...
 */
int8_t s11_init(struct s11_dev *dev);

/**
 *  @brief Reads error status from the sensor.
 *  Stores it in the device cluster.
 *  @param[in,out] dev : Instance of S11 structure
 */
int8_t s11_get_status(struct s11_dev *dev);

/**
 * @brief Reads firmware rev and Sensor ID from the sensor.
 * Stores it in the device cluster.
 * @param[in,out] dev : Instance of S11 structure
 */
int8_t s11_get_sensor_info(struct s11_dev *dev);

/** 
 * @brief Read the sensors calibration status
 * @param[in,out] dev : Instance of S11 structure
 * @ingroup calibration
 */
int8_t s11_get_cal_status(struct s11_dev *dev);

/** 
 * @brief Perform the sensors built-in calibration routine
 * and retrieve calibration status afterwards
 * @param[in,out] dev : Instance of S11 structure
 * @ingroup calibration
 */
int8_t s11_calibrate(struct s11_dev *dev);

/**
 *  @brief Start a measurement cycle,
 *  if applicable also write ABC+IIR filter data
 *  @param[in,out] dev : Instance of S11 structure
  * @ingroup measurement
 */
int8_t s11_start_meas(struct s11_dev *dev);

/**
 * @brief Read back measurement data,
 * @param[in,out] dev : Instance of S11 structure
 * @ingroup measurement
*/
int8_t s11_get_meas_data(struct s11_dev *dev);

/**
 * @brief Read back ABC+IIR data,
 * @param[in,out] dev : Instance of S11 structure
 * @ingroup measurement
*/
int8_t s11_get_state_data(struct s11_dev *dev);

/** 
 * @brief read sensor settings from EEPROM
 * @param[in,out] dev : Instance of S11 structure
 * @ingroup settings
 */
int8_t s11_get_dev_settings(struct s11_dev *dev);

/** 
 * @brief Apply sensor settings to EEPROM and restart sensor
 * @param[in,out] dev : Instance of S11 structure
 * @ingroup settings
 */
int8_t s11_set_dev_settings(struct s11_dev *dev);

/** 
 * @brief Restart sensor
 * @param[in,out] dev : Instance of S11 structure
 */
int8_t s11_restart(struct s11_dev *dev);

#ifdef __cplusplus
}
#endif

#endif // S11_H_
