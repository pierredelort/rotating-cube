#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>


#define SCREEN_WIDTH		70
#define SCREEN_HEIGHT		50

#define HORIZONTAL_PADDING	25
#define VERTICAL_PADDING	10

#define CUBE_WIDTH 		20
#define SAMPLE_RATE		(.6)

#define ROTATION_X		(M_PI/180)
#define ROTATION_Y		(M_PI/180)
#define ROTATION_Z		(M_PI/180)


typedef struct
{
	double dX;
	double dY;
	double dZ;
} Coordinate;


void computeRotation(Coordinate* psPoint, double dThetaX, double dThetaY, double dThetaZ)
{
	double dX = 0, dY = 0, dZ = 0;

	dX = psPoint->dX*cos(dThetaY)*cos(dThetaZ) - psPoint->dY*cos(dThetaY)*sin(dThetaZ) + psPoint->dZ*sin(dThetaY);

	dY = psPoint->dX*(sin(dThetaX)*sin(dThetaY)*cos(dThetaZ) + cos(dThetaX)*sin(dThetaZ)) + \
		psPoint->dY*(cos(dThetaX)*cos(dThetaZ) - sin(dThetaX)*sin(dThetaY)*sin(dThetaZ)) - \
		psPoint->dZ*(sin(dThetaX)*cos(dThetaY));

	dZ = psPoint->dX*(sin(dThetaX)*sin(dThetaZ) - cos(dThetaX)*sin(dThetaY)*cos(dThetaZ)) + \
		psPoint->dY*(cos(dThetaX)*sin(dThetaY)*sin(dThetaZ) + sin(dThetaX)*cos(dThetaZ)) + \
		psPoint->dZ*(cos(dThetaX)*cos(dThetaY));

	psPoint->dX = dX;
	psPoint->dY = dY;
	psPoint->dZ = dZ;
}


int main(void)
{
	Coordinate sPoint;
	float fIncrementX, fIncrementY;
	double dAngleX = 0, dAngleY = 0, dAngleZ = 0;
	char screenBuffer[SCREEN_WIDTH*SCREEN_HEIGHT];
	int iCharacterCpt, iCharIdx;

	for (;;)
	{
		// clear console
		printf("\x1b[2J\x1b[H");

		// reset buffer
		memset(screenBuffer, '.', SCREEN_WIDTH*SCREEN_HEIGHT);

		// rotate every point in the surface
		for (fIncrementX = 0; fIncrementX < CUBE_WIDTH; fIncrementX += SAMPLE_RATE)
		{
			for (fIncrementY = 0; fIncrementY < CUBE_WIDTH; fIncrementY += SAMPLE_RATE)
			{
				sPoint.dX = fIncrementX;
				sPoint.dY = 0;
				sPoint.dZ = fIncrementY;

				computeRotation(&sPoint, dAngleX, dAngleY, dAngleZ);

//				iCharIdx = ((int)sPoint.dZ + VERTICAL_PADDING)*SCREEN_WIDTH + (int)sPoint.dX + HORIZONTAL_PADDING;
//				printf("x = %f, z = %f, idx = %d\n", sPoint.dX, sPoint.dZ, iCharIdx);

				screenBuffer[((int)sPoint.dZ + VERTICAL_PADDING)*SCREEN_WIDTH + (int)sPoint.dX + HORIZONTAL_PADDING] = '#';
			}
		}

		// draw cube
		for (iCharacterCpt = 0; iCharacterCpt < SCREEN_WIDTH*SCREEN_HEIGHT; iCharacterCpt++)
		{
			putchar(!(iCharacterCpt%SCREEN_WIDTH) ? '\n' : screenBuffer[iCharacterCpt]);
		}

		// update rotation angles
		dAngleX += ROTATION_X;
		dAngleY += ROTATION_Y;
		dAngleZ += ROTATION_Z;

		if (dAngleX >= 2*M_PI)
			dAngleX = 0;

		if (dAngleY >= 2*M_PI)
			dAngleY = 0;

		if (dAngleZ >= 2*M_PI)
			dAngleZ = 0;

		usleep(10000);
}

	return 0;
}
