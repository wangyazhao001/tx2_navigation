#include <stdio.h> 
#include <stdint.h> 
#include <fcntl.h> 
#include <sys/ioctl.h> 
#include "i2c.h" 
#include <linux/i2c-dev.h>

#define BMP280_ADD 0x77     //BMP280 address
#define DIG_START 0x88      //temperament compensate register address
#define TEMP_START 0xFA     //temperament measurement register address
#define CTRL_MEAS 0xF4      //control mode register address

#define TEMP_ONLY_NORMAL_MODE 0xE3  // 111 000 11

int writeReg(int pt, unsigned char add, char value){ 
        unsigned char w_buff[2]; 
        w_buff[0] = add; 
        w_buff[1] = value; 
        if (write(pt, w_buff, 2) != 2){ 
                perror("Failed write to device"); 
                return 1; 
        } 
        return 0; 
} 
int readReg(int pt, unsigned char s_add, char buf[], int size){ 
        char writeBuff[1] = {s_add}; 
        if(write(pt, writeBuff, 1)!=1) { 
                perror("Failed to reset the read address\n"); 
                return 1; 
        } 
        if(read(pt, buf, size)!=size){ 
                perror("Failed to read in the buffer\n"); 
                return 1; 
        } 
        return 0; 
}

float myFunc(uint32_t adc_T, unsigned short dig_T1, short dig_T2, short dig_T3){ 
        uint32_t var1, var2; 
        float T; 
        var1 = (((double)adc_T)/16384.0-((double)dig_T1)/1024.0)*((double)dig_T2); 
        var2 = ((((double)adc_T)/131072.0-((double)dig_T1)/8192.0)*(((double)adc_T)/131072.0-((double)dig_T1)/8192.0))*((double)dig_T2); 
        T = (var1+var2)/5120.0; 
        return T; 
}

int main(){ 
        int file; 
        if((file=open("/dev/i2c-1", O_RDWR)) < 0) { 
                perror("failed to open the bus\n"); 
                return 1; 
        } 
        if(ioctl(file, I2C_SLAVE, BMP280_ADD) < 0) { 
                perror("Failed to connect to the sensor\n"); 
                return 1; 
        }

        char dig_buff[6]; 
        char tmp_buff[3];

        if(writeReg(file, CTRL_MEAS, TEMP_ONLY_NORMAL_MODE)==1){ 
                return 1; 
        } 
        if(readReg(file, DIG_START, dig_buff, 6)==1){ 
                return 1; 
        } 
        if(readReg(file, TEMP_START, tmp_buff, 3)==1){ 
                return 1; 
        } 
        close(file);

        printf("Dump :\n"); 
        int i; 
        for (i=0; i<6; i++){ 
                printf("%02x ",dig_buff[i]); 
        } 
        printf("\n"); 
        for (i=0; i<3; i++){ 
                printf("%02x ",tmp_buff[i]); 
        } 
        printf("\n"); 
        int adc_T = ((tmp_buff[0]<<16)|(tmp_buff[1]<<8)|(tmp_buff[2]))>>4; 
        unsigned short dig_T1 = (dig_buff[1]<<8)|(dig_buff[0]); 
        short dig_T2 = (dig_buff[3]<<8)|(dig_buff[2]); 
        short dig_T3 = (dig_buff[5]<<8)|(dig_buff[4]); 
        printf("adc_T is : %d \n", adc_T); 
        printf("dig_T1 is : %d \n", dig_T1); 
        printf("dig_T2 is : %d \n", dig_T2); 
        printf("dig_T3 is : %d \n", dig_T3); 
        printf("Temperature is : %f \n", myFunc(adc_T, dig_T1, dig_T2, dig_T3)); 
        return 0; 
}












































