#pragma once
/*

 */

#include <stdint.h>

#include <AP_HAL/AP_HAL.h>
#include <AP_HAL/I2CDevice.h>
#include <AP_HAL/SPIDevice.h>
#include <AP_HAL/utility/OwnPtr.h>
#include <AP_Math/AP_Math.h>
#include <Filter/Filter.h>
#include <Filter/LowPassFilter.h>
#include <Filter/LowPassFilter2p.h>

#include "AP_InertialSensor.h"
#include "AP_InertialSensor_Backend.h"


class AP_InertialSensor_Empty : public AP_InertialSensor_Backend
{    

public:
    virtual ~AP_InertialSensor_Empty();

    static AP_InertialSensor_Backend *probe(AP_InertialSensor &imu,
                                            AP_HAL::OwnPtr<AP_HAL::I2CDevice> dev,
                                            enum Rotation rotation);

    static AP_InertialSensor_Backend *probe(AP_InertialSensor &imu,
                                            AP_HAL::OwnPtr<AP_HAL::SPIDevice> dev,
                                            enum Rotation rotation);

    /* update accel and gyro state */
    bool update() override;
    //void accumulate() override;

    
    //void start() override;

 
private:
    AP_InertialSensor_Empty(AP_InertialSensor &imu,
                              AP_HAL::OwnPtr<AP_HAL::Device> dev,
                              enum Rotation rotation);

    
};
