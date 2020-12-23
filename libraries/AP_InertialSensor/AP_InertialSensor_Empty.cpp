/*
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/*
  driver for all supported Invensense IMUs, including MPU6000, MPU9250
  ICM-20608 and ICM-20602
 */

#include <assert.h>
#include <utility>
#include <stdio.h>

#include <AP_HAL/AP_HAL.h>

#include "AP_InertialSensor_Empty.h"

extern const AP_HAL::HAL& hal;

#if CONFIG_HAL_BOARD == HAL_BOARD_LINUX
#include <AP_HAL_Linux/GPIO.h>
#if CONFIG_HAL_BOARD_SUBTYPE == HAL_BOARD_SUBTYPE_LINUX_ERLEBOARD || CONFIG_HAL_BOARD_SUBTYPE == HAL_BOARD_SUBTYPE_LINUX_PXF
#define INVENSENSE_DRDY_PIN BBB_P8_14
#elif CONFIG_HAL_BOARD_SUBTYPE == HAL_BOARD_SUBTYPE_LINUX_DISCO || CONFIG_HAL_BOARD_SUBTYPE == HAL_BOARD_SUBTYPE_LINUX_BEBOP
#define INVENSENSE_EXT_SYNC_ENABLE 1
#endif
#endif

#if CONFIG_HAL_BOARD == HAL_BOARD_CHIBIOS
// hal.console can be accessed from bus threads on ChibiOS
#define debug(fmt, args ...)  do {hal.console->printf("MPU: " fmt "\n", ## args); } while(0)
#else
#define debug(fmt, args ...)  do {printf("MPU: " fmt "\n", ## args); } while(0)
#endif

/*
  EXT_SYNC allows for frame synchronisation with an external device
  such as a camera. When enabled the LSB of AccelZ holds the FSYNC bit
 */
#ifndef INVENSENSE_EXT_SYNC_ENABLE
#define INVENSENSE_EXT_SYNC_ENABLE 0
#endif

#define MPU_SAMPLE_SIZE 14
#define MPU_FIFO_BUFFER_LEN 16

#define int16_val(v, idx) ((int16_t)(((uint16_t)v[2*idx] << 8) | v[2*idx+1]))
#define uint16_val(v, idx)(((uint16_t)v[2*idx] << 8) | v[2*idx+1])

/*
 *  RM-MPU-6000A-00.pdf, page 31, section 4.23 lists LSB sensitivity of
 *  accel as 4096 LSB/mg at scale factor of +/- 8g (AFS_SEL==2)
 *
 *  See note below about accel scaling of engineering sample MPU6k
 *  variants however
 */

AP_InertialSensor_Empty::AP_InertialSensor_Empty(AP_InertialSensor &imu,
                                                           AP_HAL::OwnPtr<AP_HAL::Device> dev,
                                                           enum Rotation rotation)
    : AP_InertialSensor_Backend(imu)    
{
}

AP_InertialSensor_Empty::~AP_InertialSensor_Empty()
{
        
}

AP_InertialSensor_Backend *AP_InertialSensor_Empty::probe(AP_InertialSensor &imu,
                                                               AP_HAL::OwnPtr<AP_HAL::I2CDevice> dev,
                                                               enum Rotation rotation)
{
    if (!dev) {
        return nullptr;
    }
    AP_InertialSensor_Empty *sensor =
        new AP_InertialSensor_Empty(imu, std::move(dev), rotation);
    return sensor;
}


AP_InertialSensor_Backend *AP_InertialSensor_Empty::probe(AP_InertialSensor &imu,
                                                               AP_HAL::OwnPtr<AP_HAL::SPIDevice> dev,
                                                               enum Rotation rotation)
{
    if (!dev) {
        return nullptr;
    }
    AP_InertialSensor_Empty *sensor;
    sensor = new AP_InertialSensor_Empty(imu, std::move(dev), rotation);
    
    return sensor;
}

// void AP_InertialSensor_Empty::start()
// {
//  return ;
// }


/*
  publish any pending data
 */
bool AP_InertialSensor_Empty::update()
{
   
    update_accel(0);
    update_gyro(0);
    return true;
}

/*
  accumulate new samples
 */
// void AP_InertialSensor_Empty::accumulate()
// {
//     // nothing to do
// }