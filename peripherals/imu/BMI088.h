/*
 * BMI088.h
 *
 *  Created on: Jan 16, 2021
 *      Author: Julius
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#ifndef __BOSCH_BMI088_H__
#define __BOSCH_BMI088_H__


typedef enum
{
	NO_ACC_COMMUNICATION = 1,
	NO_GYRO_COMMUNICATION = 2,
} bmi088_error_type;

typedef enum  {
    RANGE_3G = 0x00,
    RANGE_6G = 0x01,
    RANGE_12G = 0x02,
    RANGE_24G = 0x03,
}acc_range_type;

typedef enum  {
    RANGE_2000 = 0x00,
    RANGE_1000 = 0x01,
    RANGE_500 = 0x02,
    RANGE_250 = 0x03,
    RANGE_125 = 0x04,
}gyro_range_type;

typedef enum { // output data rate
    ODR_12 = 0x05,
    ODR_25 = 0x06,
    ODR_50 = 0x07,
    ODR_100 = 0x08,
    ODR_200 = 0x09,
    ODR_400 = 0x0A,
    ODR_800 = 0x0B,
    ODR_1600 = 0x0C,
} acc_odr_type;

typedef enum { //oversampling
    OS_4 = 0x08,
    OS_2 = 0x09,
    OS_0 = 0x0A,
} acc_os_type;

typedef enum { // output data rate and bandwith
    ODR_2000_BW_532 = 0x00,
    ODR_2000_BW_230 = 0x01,
    ODR_1000_BW_116 = 0x02,
    ODR_400_BW_47 = 0x03,
    ODR_200_BW_23 = 0x04,
    ODR_100_BW_12 = 0x05,
    ODR_200_BW_64 = 0x06,
    ODR_100_BW_32 = 0x07,
} gyro_odr_type;

typedef struct
{
	uint8_t AccAddress;
	uint8_t GyroAddress;
	acc_range_type AccRange;
	gyro_range_type GyroRange;
	void (*Write)(uint16_t devAddress, uint16_t regAddress, uint8_t * msg, uint16_t size);
	void (*Read)(uint16_t devAddress, uint16_t regAddress, uint8_t * msg, uint16_t size);
	void (*DelayMs)(uint32_t delayMs);
	bmi088_error_type errors;

} bmi088_type;

typedef enum
{
	X = 1,
	Y = 2,
	Z = 3,
} coord;

#define BMI088_ACC1_ADDRESS         0x18
#define BMI088_ACC2_ADDRESS         0x19
#define BMI088_GYRO1_ADDRESS        0x68
#define BMI088_GYRO2_ADDRESS        0x69

#define BMI088_ON		    0x04
#define BMI088_OFF	   	    0x00
#define BMI088_ACTIVE		    0x00
#define BMI088_SUSPEND		    0x03
#define BMI088_GYRO_SUSPEND	    0x80
#define BMI088_GYRO_ACTIVE	    0x00


#define BMI088_ACC_CHIP_ID          0x00 // should return 0x1E
#define BMI088_ACC_ERR_REG          0x02
#define BMI088_ACC_STATUS           0x03

#define BMI088_ACC_X_LSB            0x12
#define BMI088_ACC_X_MSB            0x13
#define BMI088_ACC_Y_LSB            0x14
#define BMI088_ACC_Y_MSB            0x15
#define BMI088_ACC_Z_LSB            0x16
#define BMI088_ACC_Z_MSB            0x17

#define BMI088_ACC_SENSOR_TIME_0    0x18
#define BMI088_ACC_SENSOR_TIME_1    0x19
#define BMI088_ACC_SENSOR_TIME_2    0x1A

#define BMI088_ACC_INT_STAT_1       0x1D

#define BMI088_ACC_TEMP_MSB         0x22
#define BMI088_ACC_TEMP_LSB         0x23

#define BMI088_ACC_CONF             0x40
#define BMI088_ACC_RANGE            0x41

#define BMI088_ACC_INT1_IO_CTRL     0x53
#define BMI088_ACC_INT2_IO_CTRL     0x54
#define BMI088_ACC_INT_MAP_DATA     0x58

#define BMI088_ACC_SELF_TEST        0x6D

#define BMI088_ACC_PWR_CONF         0x7C
#define BMI088_ACC_PWR_CTRl         0x7D

#define BMI088_ACC_SOFT_RESET       0x7E

#define BMI088_GYRO_ADDRESS             0x69

#define BMI088_GYRO_CHIP_ID             0x00 // Default value 0x0F

#define BMI088_GYRO_RATE_X_LSB          0x02
#define BMI088_GYRO_RATE_X_MSB          0x03
#define BMI088_GYRO_RATE_Y_LSB          0x04
#define BMI088_GYRO_RATE_Y_MSB          0x05
#define BMI088_GYRO_RATE_Z_LSB          0x06
#define BMI088_GYRO_RATE_Z_MSB          0x07

#define BMI088_GYRO_INT_STAT_1          0x0A

#define BMI088_GYRO_RANGE               0x0F
#define BMI088_GYRO_BAND_WIDTH          0x10

#define BMI088_GYRO_LPM_1               0x11

#define BMI088_GYRO_SOFT_RESET          0x14

#define BMI088_GYRO_INT_CTRL            0x15
#define BMI088_GYRO_INT3_INT4_IO_CONF   0x16
#define BMI088_GYRO_INT3_INT4_IO_MAP    0x18

#define BMI088_GYRO_SELF_TEST           0x3C

enum device_type_t { // device type
    ACC = 0x00, //
    GYRO = 0x01, //
};

enum acc_power_type_t { // power mode
    ACC_ACTIVE = 0x00, //
    ACC_SUSPEND = 0x03, //
};


enum gyro_power_type_t { // power mode
    GYRO_NORMAL = 0x00, //
    GYRO_SUSPEND = 0x80, //
    GYRO_DEEP_SUSPEND = 0x20, //
};

void bmi088_init(bmi088_type * bmi088);
void bmi088_power_up(bmi088_type * bmi088);
int16_t get_acceleration(const bmi088_type * bmi088, coord xyz);
void bmi088_get_acceleration_xyz(const bmi088_type * bmi088, double * msg);
void bmi088_get_angular_rate_xyz(const bmi088_type * bmi088, double * msg);
void bmi088_set_acc_range(bmi088_type * bmi088, acc_range_type range);
bmi088_type constructBMI088(
	void (*delay)(uint32_t),
	void (*write)(uint16_t, uint16_t, uint8_t*, uint16_t),
	void (*read)(uint16_t, uint16_t, uint8_t*, uint16_t),
	uint8_t AccAddress,
	uint8_t GyroAddress,
	acc_range_type acc_range,
	acc_odr_type acc_odr,
	acc_os_type acc_os,
	gyro_range_type gyro_range,
	gyro_odr_type gyro_odr)
;

#endif
