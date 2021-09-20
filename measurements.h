/* 
 * File:   measurements.h
 * Author: ilya_000
 *
 * Created on September 20, 2021, 1:12 PM
 */

#ifndef MEASUREMENTS_H
#define	MEASUREMENTS_H

#ifdef	__cplusplus
extern "C" {
#endif

uint8_t getBatteryLevel(void);
void getTableValues(void);
int16_t getTemperature(void);


#ifdef	__cplusplus
}
#endif

#endif	/* MEASUREMENTS_H */

