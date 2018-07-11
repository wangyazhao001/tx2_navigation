/*************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
/***********************************************
char name[6][1024]={
"$GPGGA,060826.00,2236.91284,N,11403.24705,E,2,08,1.03,107.8,M,-2.4,M,,0000*4A",
"$GPGSA,A,3,24,22,14,12,15,25,18,42,15,16,22,28,2.20,1.03,1.95*01",
"$GPGSV,3,1,11,12,31,117,,14,30,290,46,15,19,060,42,18,70,010,45*78",
"$GPGLL,2236.91284,N,11403.24705,E,060826.00,A,D*66",
"$GPRMC,060827.00,A,2236.91267,N,11403.24701,E,0.001,,130214,,,D*79",
"$GPVTG,,T,,M,0.029,N,0.054,K,D*2C"
};
*********************************************/
void extract0(void);
void extract1(void);
void extract2(void);
void extract3(void);
void extract4(void);
void extract5(void);
void analyse(char *q);

/********************************************/
char *dive=",*";
char *p;

void analyse(char *q)
{
int flag;

//for(int i=0;i<6;i++){

p=strtok(q,dive);

if(!strcmp("$GPGGA",p)){
	flag=0;
	printf("deal GPGGA data---flag=%d  %s\n",flag,p);

}else if(!strcmp("$GPGSA",p)){
	flag=1;
	printf("deal GPGSA data---flag=%d\n",flag);

}else if(!strcmp("$GPGSV",p)){
	flag=2;
	printf("deal GPGSV data---flag=%d\n",flag);

}else if(!strcmp("$GPGLL",p)){
	flag=3;
	printf("deal GPGLL data---flag=%d\n",flag);

}else if(!strcmp("$GPRMC",p)){
	flag=4;
	printf("deal GPRMC data---flag=%d\n",flag);

}else if(!strcmp("$GPVTG",p)){
	flag=5;
	printf("deal GPVYG data---flag=%d\n",flag);

}else{
	flag=6;
	
}

switch(flag)
{

case 0:extract0();break;
case 1:extract1();break;
case 2:extract2();break;
case 3:extract3();break;
case 4:extract4();break;
case 5:extract5();break;
case 6:printf("can't deal with the datas with this style\n\n");break;
default:printf("the style of data is wrong!please try again\n");break;

}

//}


}

/*********************************************************************/
void extract0(void)
{

char *a[18]={0};
int i=0;

while(p!=NULL){

	p=strtok(NULL,dive);
	*(a+i)=p;
	i++;
}


printf("UTC_time:%s \nlatitude:%s \nN/S:%s \nlongitude:%s \nE/W:%s\n",
		a[0],a[1],a[2],a[3],a[4]);
printf("the mode of locatian(1-->SPS 2-->Differential 3-->PPS):%s\n",
		a[5]);

printf("the number of satellate:%s \nHDOP:%s \nMSL:%s %s\n",
		a[6],a[7],a[8],a[9]);

printf("sealevel:%s %s\nDifferential time:%s \n",
		a[10],a[11],a[12]);

printf("That's $GPGGA all datas\n");

printf("------------------------------------------\n\n");
}

//*********************************************
void extract1(void)
{


char *a[24]={0};
int i=0;

while(p!=NULL){

	p=strtok(NULL,dive);
	*(a+i)=p;
	i++;
}

printf("location mode1(M-->mannul A--->auto):%s\n",a[0]);
printf("location mode2:%sD mode\n",a[1]);

printf("access1(satellate):%s\naccess2:%s\naccess3:%s\naccess4:%s\n",
		a[2],a[3],a[4],a[5]);
printf("access5(satellate):%s\naccess6:%s\naccess7:%s\naccess8:%s\n",
		a[6],a[7],a[8],a[9]);

if(i>16){
printf("access9(satellate):%s\naccess10:%s\naccess11:%s\naccess12:%s\n",
		a[10],a[11],a[12],a[13]);
}

printf("PDOP:%s\nHDOP:%s\nVDOP:%s\n",a[i-5],a[i-4],a[i-3]);


printf("That's $GPGSA all datas\n");

printf("------------------------------------------\n\n");
}

/****************************************/

void extract2(void)
{

char *a[28]={0};
int i=0;

while(p!=NULL){

	p=strtok(NULL,dive);
	*(a+i)=p;
	i++;
}

printf("Number of messages:%s\nmessage serial number:%s\n",a[0],a[1]);
printf("the number of satellate:%s\nsatellate ID:%s\n",a[2],a[3]);
printf("Elevation angle:%s\nAzimuth:%s\n",a[4],a[5]);
if(i>=21)
	printf("Carrier to noise ratio:%s\n",a[6]);
	

printf("satellate ID:%s\nElevation angle:%s\nAzimuth:%s\n",
		a[6],a[7],a[8]);
printf("Carrier to noise ratio:%s\n",a[9]);

if(i>18){
printf("satellate ID:%s\nElevation angle:%s\nAzimuth:%s\n",
		a[10],a[11],a[12]);
printf("Carrier to noise ratio:%s\n",a[13]);
}
printf("satellate ID:%s\nElevation angle:%s\nAzimuth:%s\n",
		a[i-6],a[i-5],a[i-4]);
printf("Carrier to noise ratio:%s\n",a[i-3]);

printf("That's all GPGSV data\n");


printf("------------------------------------------\n\n");
}


void extract3(void)
{

char *a[10]={0};
int i=0;

while(p!=NULL){

	p=strtok(NULL,dive);
	*(a+i)=p;
	i++;
}

printf("latitude:%s\nN/S:%s\nlongitude:%s\nE/W:%s",
		a[0],a[1],a[2],a[3]);
printf("UTC:%s\nstatu(A->vaild V->invalid):%s\n",
		a[4],a[5]);
printf("locate model(A->auto D->differential E->estimate N->invaild):%s\n",
			a[6]);

printf("------------------------------------------\n\n");
}

void extract4(void)
{

char *a[14]={0};
int i=0;

while(p!=NULL){

	p=strtok(NULL,dive);
	*(a+i)=p;
	i++;
}

printf("UTC:%s\nstatu(A->valid V->invalid):%s\n",a[0],a[1]);
printf("latitude:%s\nN/S:%s\nlongitude:%s\nE/W:%s\n",
		a[2],a[3],a[4],a[5]);
printf("ground speed:%s\ndate:%s\n",a[6],a[7]);
printf("model(A->auto D->differential E->estimate N->invalid):%s\n",a[8]);

printf("------------------------------------------\n\n");
}

void extract5(void)
{

char *a[10]={0};
int i=0;

while(p!=NULL){

	p=strtok(NULL,dive);
	*(a+i)=p;
	i++;
}

printf("True north:%s\nmagnetic:%s\nground speed(knots):%s",
		a[0],a[1],a[2]);
printf("Fixed bytes(no meanings):%s\nground speed(kilometer/h):%s %s\n",
		a[3],a[4],a[5]);

printf("model(A->auto D->differential E->estimate N->invalid):%s\n",a[6]);


printf("------------------------------------------\n\n");
}

