
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
 * @brief Source file for Senseair Sunrise CO2 sensor (S11) driver
 * @file uno_continuous_meas.ino
 * @date 2021-03-11
 * @author Jhonatan Napadow
 **/

/**
 * Hook-up diagram
 * S11 pin    | UNO pin
 * -----------+--------
 * GND     (1): GND  
 * VBB     (2): 3.3V
 * VDDIO   (3): D8
 * RxD/SDA (4): D18 (SDA)
 * TxD/SCL (5): D19 (SCL)
 * COMSEL  (6): GND
 * nRDY    (7): D2
 * DVCC    (8): N/C
 * EN      (9): D9
 **/


#include "s11.h"
#include "Wire.h"

/** @brief callback for I2C read function */
int8_t sensor_read(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *data, uint8_t len) {
  // Internal variables
  uint8_t i;
  uint8_t res;

  // Wake up sensor 
  Wire.beginTransmission(i2c_addr);
  Wire.endTransmission(); // ignore result
      
  // Write register address to sensor
  Wire.beginTransmission(i2c_addr);
  res = Wire.write(reg_addr);

  // Error handling
  if (res=1) {
    res = Wire.endTransmission();
    if (res!=0) return S11_E_I2C;
  }
  else {
    Wire.endTransmission(); // ignore result
    return S11_E_I2C;
  }
  
  // Read back data beginning at selected register address
  Wire.requestFrom(i2c_addr,len); // ignore result;

  i = 0;
  while(Wire.available()) {
    data[i] = Wire.read();
    i++;
  } 
  delay(1); // wait before allowing more com on I2C, not so nice..
  if (i!=len) return S11_E_I2C;
      
  return S11_E_OK;
}

/** @brief callback for I2C write function */
int8_t sensor_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *data, uint8_t len) {
  uint8_t res; 

  // Wake up sensor 
  Wire.beginTransmission(i2c_addr);
  Wire.endTransmission(); // ignore result

  // Write register address to sensor
  Wire.beginTransmission(i2c_addr);
  res = Wire.write(reg_addr);
  if (res!=1) {  // nb of written bytes must be 1
    Wire.endTransmission(); // ignore result
    return S11_E_I2C;
  }
  
  // Write register address and data
  res = Wire.write(data,len);
  Wire.endTransmission();
  if (res!=len) return S11_E_I2C;
  
  return S11_E_OK;
}

/** @brief callback for delay function */
int8_t sensor_delay_ms(uint32_t period) {
  delay(period);
  return S11_E_OK;
}

struct s11_dev co2_sensor;

void setup() {

  Serial.begin(9600);
  Serial.println("*** Setup begin ***");

  // Init io
  pinMode(2, INPUT);           // sensor pin: nReady
  pinMode(8, OUTPUT);          // sensor pin: VDDIO
  pinMode(9, OUTPUT);          // sensor pin: EN
  digitalWrite(8, HIGH);       // enable sensor
  digitalWrite(9, HIGH);       // power EN 
  delay(S11_RESTART_DELAY_MS); // wait for sensor to power up
  
  // Init i2c
  Wire.begin();
  Wire.setClock(100000);

  // Init sensor
  int8_t res =0;
  co2_sensor.i2c_address = S11_DEF_I2C_ADDR;  // set address
  co2_sensor.write = sensor_write;            // add write callback
  co2_sensor.read  = sensor_read;             // add read callback
  co2_sensor.delay = sensor_delay_ms;         // add delay callback
  res = s11_init(&co2_sensor);                // initialize sensor
  Serial.print("Sensor init res: 0x");
  Serial.println(res,HEX);

  // Write sensor settings
  co2_sensor.dev_sett.meas_mode = S11_MM_STATE_CONT_MEAS; // continuous measurement
  co2_sensor.dev_sett.meas_period = 2;                    // period in [s]
  co2_sensor.dev_sett.meas_nb_samples = 10;               // samples/measurement
  res = s11_set_dev_settings(&co2_sensor);                // write settings to sensor
  Serial.print("Set sensor settings res: 0x");
  Serial.println(res,HEX);
  Serial.print("Number of settings written to sensor EEPROM: ");
  Serial.println(co2_sensor.dev_sett.write_count,DEC);

  Serial.println("*** Setup end ***");

}

void loop() {

  // Read sensor data periodically
  s11_get_meas_data(&co2_sensor);
  Serial.print("T: ");
  Serial.print(co2_sensor.meas_res.temp*0.01);
  Serial.print(" deg. C, ");
  Serial.print("CO2(fp): ");
  Serial.print(co2_sensor.meas_res.co2_fp);
  Serial.println(" ppm");
  delay(1000);





  

}
