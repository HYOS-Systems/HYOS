/*
 * BMI088.c
 *
 *  Created on: Jan 16, 2021
 *      Author: Julius
 */

#include "BMI088.h"

double convert_list[] = {2000., 1000., 500., 250., 125.};
double convert;

void write_to_acc(const bmi088_type * bmi088, uint16_t regAddress, uint8_t * msg, const uint16_t size){
	bmi088->Write((bmi088->AccAddress << 1), regAddress, msg, size);
}

void read_from_acc(const bmi088_type * bmi088, uint16_t regAddress, uint8_t * msg, const uint16_t size){
	bmi088->Read((bmi088->AccAddress << 1), regAddress, msg, size);
}

void write_to_gyro(const bmi088_type * bmi088, uint16_t regAddress, uint8_t * msg, const uint16_t size){
	bmi088->Write((bmi088->GyroAddress << 1), regAddress, msg, size);
}

void read_from_gyro(const bmi088_type * bmi088, uint16_t regAddress, uint8_t * msg, const uint16_t size){
	bmi088->Read((bmi088->GyroAddress << 1), regAddress, msg, size);
}

double convert_acceleration_to_mg(const bmi088_type * bmi088, int16_t acc){
	double acc_mg;
	acc_mg = 1000. * pow(2., (double) (bmi088->AccRange + 1)) * 1.5 * (double) acc / 32768.;
	return (acc_mg);
}

int16_t get_acceleration(const bmi088_type * bmi088, coord xyz){
	uint8_t msb = 0, lsb = 0;
	uint16_t addr1, addr2;
	int16_t acceleration;

	switch (xyz){
		case 1:
			addr1 = BMI088_ACC_X_LSB;
			addr2 = BMI088_ACC_X_MSB;
			break;
		case 2:
			addr1 = BMI088_ACC_Y_LSB;
			addr2 = BMI088_ACC_Y_MSB;
			break;
		case 3:
			addr1 = BMI088_ACC_Z_LSB;
			addr2 = BMI088_ACC_Z_MSB;
			break;
	}

	read_from_acc(bmi088, addr1, &lsb, 1);
	read_from_acc(bmi088, addr2, &msb, 1);
	acceleration = msb * 256 + lsb;
	return (acceleration);
}

int16_t get_angular_rate(const bmi088_type * bmi088, coord xyz){
	uint8_t msb = 0, lsb = 0;
	uint16_t addr1, addr2;
	int16_t angular_rate;

	switch (xyz){
		case 1:
			addr1 = BMI088_GYRO_RATE_X_LSB;
			addr2 = BMI088_GYRO_RATE_X_MSB;
			break;
		case 2:
			addr1 = BMI088_GYRO_RATE_Y_LSB;
			addr2 = BMI088_GYRO_RATE_Y_MSB;
			break;
		case 3:
			addr1 = BMI088_GYRO_RATE_Z_LSB;
			addr2 = BMI088_GYRO_RATE_Z_MSB;
			break;
	}

	read_from_gyro(bmi088, addr1, &lsb, 1);
	read_from_gyro(bmi088, addr2, &msb, 1);
	angular_rate = msb * 256 + lsb;
	return (angular_rate);
}

void bmi088_get_acc_range(bmi088_type * bmi088){
	uint8_t range;
	read_from_acc(bmi088, BMI088_ACC_RANGE, &range, 1);
	bmi088->AccRange = (acc_range_type) (range & 0x03);
}

void bmi088_set_acc_range(bmi088_type * bmi088, acc_range_type range){
	range = ((bmi088->AccRange & 0xFC) | range);
	write_to_acc(bmi088, BMI088_ACC_RANGE, &range, 1);
	bmi088->AccRange = (range & 0x03);
}

void bmi088_set_gyro_range(bmi088_type * bmi088, gyro_range_type range){
	write_to_acc(bmi088, BMI088_GYRO_RANGE, &range, 1);
	bmi088->GyroRange = range;
	convert = convert_list[bmi088->GyroRange] / 32767.;
}

void bmi088_set_acc_bw(bmi088_type * bmi088, acc_odr_type odr, acc_os_type os){
    uint8_t acc_conf;
    acc_conf = os;
    acc_conf = acc_conf << 4;
    acc_conf = acc_conf | odr;
    write_to_acc(bmi088, BMI088_ACC_CONF, &acc_conf, 1);
}

void bmi088_set_gyro_bw(bmi088_type * bmi088, gyro_odr_type odr){
	write_to_acc(bmi088, BMI088_GYRO_BAND_WIDTH, &odr, 1);
}


void bmi088_init(bmi088_type * bmi088){
	uint8_t msg;
	uint8_t chip_id;

	bmi088->DelayMs(1);
	msg = BMI088_ON;
	write_to_acc(bmi088, BMI088_ACC_PWR_CTRl, &msg, 1);
	bmi088->DelayMs(50);

	msg = BMI088_GYRO_SUSPEND;
	write_to_gyro(bmi088, BMI088_GYRO_LPM_1, &msg, 1);

	read_from_acc(bmi088, BMI088_ACC_CHIP_ID, &chip_id, 1);
	if (chip_id != 0x1E){
		bmi088->errors = NO_ACC_COMMUNICATION;
	}
	read_from_gyro(bmi088, BMI088_GYRO_CHIP_ID, &chip_id, 1);
	if (chip_id != 0x0F){
		bmi088->errors = NO_GYRO_COMMUNICATION;
	}
}

void bmi088_power_up(bmi088_type * bmi088){
	uint8_t msg;
	uint8_t chip_id;

	msg = BMI088_ACTIVE;
	write_to_acc(bmi088, BMI088_ACC_PWR_CONF, &msg, 1);
	bmi088->DelayMs(50);

	msg = BMI088_GYRO_ACTIVE;
	write_to_gyro(bmi088, BMI088_GYRO_LPM_1, &msg, 1);
	bmi088->DelayMs(50);

	read_from_acc(bmi088, BMI088_ACC_CHIP_ID, &chip_id, 1);
	if (chip_id != 0x1E){
		bmi088->errors = NO_ACC_COMMUNICATION;
	}
	read_from_gyro(bmi088, BMI088_GYRO_CHIP_ID, &chip_id, 1);
	if (chip_id != 0x0F){
		bmi088->errors = NO_GYRO_COMMUNICATION;
	}
}


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
{
    bmi088_type bmi088;
    bmi088.DelayMs = delay;
    bmi088.Write = write;
    bmi088.Read = read;
    bmi088.AccAddress = AccAddress;
    bmi088.GyroAddress = GyroAddress;
    bmi088_init(&bmi088);
    bmi088_set_acc_range(&bmi088, acc_range);
    bmi088_set_acc_bw(&bmi088, acc_odr, acc_os);
    bmi088_set_gyro_range(&bmi088, gyro_range);
    bmi088_set_gyro_bw(&bmi088, gyro_odr);
    return bmi088;
}

void bmi088_get_acceleration_xyz(const bmi088_type * bmi088, double * msg){
	int16_t x, y, z;
	x = get_acceleration(bmi088, X);
	y = get_acceleration(bmi088, Y);
	z = get_acceleration(bmi088, Z);
	msg[0] = convert_acceleration_to_mg(bmi088, x);
	msg[1] = convert_acceleration_to_mg(bmi088, y);
	msg[2] = convert_acceleration_to_mg(bmi088, z);
}

void bmi088_get_angular_rate_xyz(const bmi088_type * bmi088, double * msg){
	int16_t x, y, z;
	x = get_angular_rate(bmi088, X);
	y = get_angular_rate(bmi088, Y);
	z = get_angular_rate(bmi088, Z);
	msg[0] = convert * (double) x;
	msg[1] = convert * (double) y;
	msg[2] = convert * (double) z;

}
