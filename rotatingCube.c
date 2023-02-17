#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>


#define SCREEN_WIDTH		60
#define SCREEN_HEIGHT		40

#define HORIZONTAL_PADDING	25
#define VERTICAL_PADDING	20

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


double dAngleX = 0, dAngleY = 0, dAngleZ = 0;
char screenBuffer[SCREEN_WIDTH*SCREEN_HEIGHT];
double dYBuffer[SCREEN_WIDTH*SCREEN_HEIGHT];


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


void rotateSurface(int iMinSurfaceX, int iMaxSurfaceX,
		int iMinSurfaceY, int iMaxSurfaceY,
		int iMinSurfaceZ, int iMaxSurfaceZ,
		char surfaceCharacter)
{
	Coordinate sPoint;
	float fIncrementX, fIncrementY, fIncrementZ;
	int iScreenBufferIdx = 0;

	// rotate every point in the surface
	for (fIncrementX = iMinSurfaceX; fIncrementX <= iMaxSurfaceX; fIncrementX += SAMPLE_RATE)
	{
		for (fIncrementY = iMinSurfaceY; fIncrementY <= iMaxSurfaceY; fIncrementY += SAMPLE_RATE)
		{
			for (fIncrementZ = iMinSurfaceZ; fIncrementZ <= iMaxSurfaceZ; fIncrementZ += SAMPLE_RATE)
			{
				sPoint.dX = fIncrementX;
				sPoint.dY = fIncrementY;
				sPoint.dZ = fIncrementZ;

				computeRotation(&sPoint, dAngleX, dAngleY, dAngleZ);

				iScreenBufferIdx = ((int)sPoint.dZ + VERTICAL_PADDING)*SCREEN_WIDTH + (int)sPoint.dX + HORIZONTAL_PADDING;

				//if (sPoint.dY > dYBuffer[iScreenBufferIdx])
				{
					dYBuffer[iScreenBufferIdx] = sPoint.dY;
					screenBuffer[iScreenBufferIdx] = surfaceCharacter;
				}
			}
		}
	}
}


int main(void)
{
	int iCharacterCpt, iFaceCpt;

	for (;;)
	{
		// clear console
		printf("\x1b[2J\x1b[H");

		// reset buffer
		memset(screenBuffer, ' ', SCREEN_WIDTH*SCREEN_HEIGHT);
		memset(dYBuffer, 0, SCREEN_WIDTH*SCREEN_HEIGHT);

		// Rotate all 6 surfaces of the cube
		rotateSurface(-CUBE_WIDTH/2, CUBE_WIDTH/2, -CUBE_WIDTH/2, -CUBE_WIDTH/2, -CUBE_WIDTH/2, CUBE_WIDTH/2, '#');
		rotateSurface(-CUBE_WIDTH/2, -CUBE_WIDTH/2, -CUBE_WIDTH/2, CUBE_WIDTH/2, -CUBE_WIDTH/2, CUBE_WIDTH/2, '$');
		rotateSurface(-CUBE_WIDTH/2, CUBE_WIDTH/2, CUBE_WIDTH/2, CUBE_WIDTH/2, -CUBE_WIDTH/2, CUBE_WIDTH/2, '+');
		rotateSurface(CUBE_WIDTH/2, CUBE_WIDTH/2, -CUBE_WIDTH/2, CUBE_WIDTH/2, -CUBE_WIDTH/2, CUBE_WIDTH/2, '%');
		rotateSurface(-CUBE_WIDTH/2, CUBE_WIDTH/2, -CUBE_WIDTH/2, CUBE_WIDTH/2, CUBE_WIDTH/2, CUBE_WIDTH/2, '*');
		rotateSurface(-CUBE_WIDTH/2, CUBE_WIDTH/2, -CUBE_WIDTH/2, CUBE_WIDTH/2, -CUBE_WIDTH/2, -CUBE_WIDTH/2, '&');

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

		usleep(15000);
	}

	return 0;
}

