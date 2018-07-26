#include <stdio.h> 
#include <stdint.h> 
#include <fcntl.h> 
#include <unistd.h>
#include <sys/ioctl.h> 
#include "i2c.h" 
#include <linux/i2c-dev.h>


#define BMP280_ADD 0x77     //BMP280 address
#define DIG_START 0x88      //temperament compensate register address
#define TEMP_START 0xFA     //temperament measurement register address

#define DIGP_START 0X8E     //pressure compensate register address
#define PRESS_START 0XF7    //pressure measurement register address 

#define CTRL_MEAS 0xF4      //control mode register address



#define TEMP_ONLY_NORMAL_MODE 0xFF  // 111 111 11

int32_t t_fine=0;

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

//compensation for temperament
float myFunc(int32_t adc_T, unsigned short dig_T1, short dig_T2, short dig_T3){ 
        double var1, var2, T; 
        var1 = (((double)adc_T)/16384.0-((double)dig_T1)/1024.0)*((double)dig_T2); 
        var2 = ((((double)adc_T)/131072.0-((double)dig_T1)/8192.0)*(((double)adc_T)/131072.0-((double)dig_T1)/8192.0))*((double)dig_T3); 
        t_fine =(int32_t)(var1+var2);
	T = (var1+var2)/5120.0; 
        return T; 
}

//compensation for pressure
float myFunc_p(int32_t adc_P,unsigned short dig_P1,short dig_P2,short dig_P3,short dig_P4,short dig_P5,short dig_P6,short dig_P7,short dig_P8,short dig_P9)
{
	double var1,var2,p;
	if(t_fine==0)
		printf("t_fine is 0!The pressure measurement is not right!Please check temperament measurement\n");
	var1 = ((double)t_fine/2.0)-64000.0;
	var2 = var1 * var1 * ((double)dig_P6) / 32768.0;
	var2 = var2 + var1 * ((double)dig_P5) * 2.0;
	var2 = (var2/4.0)+(((double)dig_P4) * 65536.0);
	var1 = (((double)dig_P3) * var1 * var1 / 524288.0 + ((double)dig_P2) * var1) / 524288.0;
	var1 = (1.0 + var1 / 32768.0)*((double)dig_P1);
	if (var1 == 0.0)
		return 0; // avoid exception caused by division by zero
	p=1048576.0-(double)adc_P;
	p=(p-(var2/4096.0))*6250.0/var1;
	var1=((double)dig_P9)*p*p/2147483648.0;
	var2=p*((double)dig_P8)/32768.0;
	p = p+(var1+var2+((double)dig_P7))/16.0;
	return p;

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

        char dig_buff[6];    // temperament compensation data
        char tmp_buff[3];    // temperament data
	char digp_buff[18];  // pressure compensation data 
	char pre_buff[3];    //pressure data

        if(writeReg(file, CTRL_MEAS, TEMP_ONLY_NORMAL_MODE)==1){ 
                return 1; 
        } 
	//temperament
        if(readReg(file, DIG_START, dig_buff, 6)==1){ 
                return 1; 
        } 
        if(readReg(file, TEMP_START, tmp_buff, 3)==1){ 
                return 1; 
        } 
	//pressure
	
        if(readReg(file, DIGP_START, digp_buff, 18)==1){ 
                return 1; 
        } 
        if(readReg(file, PRESS_START, pre_buff, 3)==1){ 
                return 1; 
        } 
        
	close(file);

        printf("temperament:\n"); 
        int i; 
        for (i=0; i<6; i++){ 
                printf("%02x ",dig_buff[i]); 
        } 
        printf("\n"); 
        for (i=0; i<3; i++){ 
                printf("%02x ",tmp_buff[i]); 
        }
	printf("\n"); 
        printf("pressure:\n");
	for (i=0;i<18;i++){
		printf("%02x ",digp_buff[i]);
	}
	printf("\n");
	for(i=0;i<3;i++){
		printf("%02x ",pre_buff[i]);
	}
	printf("\n");
	//temperament
        int adc_T = ((tmp_buff[0]<<16)|(tmp_buff[1]<<8)|(tmp_buff[2]))>>4;
        //pressure
	int adc_P = ((pre_buff[0]<<16)|(pre_buff[1]<<8)|(pre_buff[2]))>>4;
 	
	// for temperament
        unsigned short dig_T1 = (dig_buff[1]<<8)|(dig_buff[0]); 
        short dig_T2 = (dig_buff[3]<<8)|(dig_buff[2]); 
        short dig_T3 = (dig_buff[5]<<8)|(dig_buff[4]); 
	
	// for pressure
        unsigned short dig_P1 = (digp_buff[1]<<8)|(digp_buff[0]); 
        short dig_P2 = (digp_buff[3]<<8)|(digp_buff[2]); 
        short dig_P3 = (digp_buff[5]<<8)|(digp_buff[4]); 
        short dig_P4 = (digp_buff[7]<<8)|(digp_buff[6]); 
        short dig_P5 = (digp_buff[9]<<8)|(digp_buff[8]); 
        short dig_P6 = (digp_buff[11]<<8)|(digp_buff[10]); 
        short dig_P7 = (digp_buff[13]<<8)|(digp_buff[12]); 
        short dig_P8 = (digp_buff[15]<<8)|(digp_buff[14]); 
        short dig_P9 = (digp_buff[17]<<8)|(digp_buff[16]); 
	
	printf("adc_T is : %d \n", adc_T); 
        printf("dig_T1 is : %d \n", dig_T1); 
        printf("dig_T2 is : %d \n", dig_T2); 
        printf("dig_T3 is : %d \n", dig_T3); 
        printf("Temperature is : %f DegC\n", myFunc(adc_T, dig_T1, dig_T2, dig_T3)); 
         
	printf("adc_P is : %d \n", adc_P); 
        printf("dig_P1 is : %d \n", dig_P1); 
        printf("dig_P2 is : %d \n", dig_P2); 
        printf("dig_P3 is : %d \n", dig_P3); 
        printf("dig_P4 is : %d \n", dig_P4); 
        printf("dig_P5 is : %d \n", dig_P5); 
        printf("dig_P6 is : %d \n", dig_P6); 
        printf("dig_P7 is : %d \n", dig_P7); 
        printf("dig_P8 is : %d \n", dig_P8); 
        printf("dig_P9 is : %d \n", dig_P9); 
	printf("pressure is :%f KPa\n",myFunc_p(adc_P,dig_P1,dig_P2,dig_P3,dig_P4,dig_P5,dig_P6,dig_P7,dig_P8,dig_P9)/1000.0);

	return 0;
}












































