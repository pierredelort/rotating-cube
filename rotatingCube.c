#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>


#define SCREEN_WIDTH        120
#define SCREEN_HEIGHT       60
#define BUFFER_SIZE         SCREEN_WIDTH*SCREEN_HEIGHT

#define HORIZONTAL_PADDING  60
#define VERTICAL_PADDING    30
#define ASPECT_RATIO        ((float)9/16)

#define CUBE_SIZE           57
#define SAMPLE_RATE         (.8)
#define SLEEP_TIME          50000 // us

#define ROTATION_X          (M_PI/90)
#define ROTATION_Y          (M_PI/180)
#define ROTATION_Z          (M_PI/120)

#define BACKGROUND_CHAR     '.'


typedef struct
{
    float fX;
    float fY;
    float fZ;
} Coordinate;


float fAngleX = 0, fAngleY = 0, fAngleZ = 0;
char acScreenBuffer[BUFFER_SIZE];
float afYBuffer[BUFFER_SIZE];


void computeRotation(Coordinate* psPoint, float fThetaX, float fThetaY, float fThetaZ)
{
    float fX = 0, fY = 0, fZ = 0;

    fX = psPoint->fX*cos(fThetaY)*cos(fThetaZ) - psPoint->fY*cos(fThetaY)*sin(fThetaZ) + psPoint->fZ*sin(fThetaY);

    fY = psPoint->fX*(sin(fThetaX)*sin(fThetaY)*cos(fThetaZ) + cos(fThetaX)*sin(fThetaZ)) + \
        psPoint->fY*(cos(fThetaX)*cos(fThetaZ) - sin(fThetaX)*sin(fThetaY)*sin(fThetaZ)) - \
        psPoint->fZ*(sin(fThetaX)*cos(fThetaY));

    fZ = psPoint->fX*(sin(fThetaX)*sin(fThetaZ) - cos(fThetaX)*sin(fThetaY)*cos(fThetaZ)) + \
        psPoint->fY*(cos(fThetaX)*sin(fThetaY)*sin(fThetaZ) + sin(fThetaX)*cos(fThetaZ)) + \
        psPoint->fZ*(cos(fThetaX)*cos(fThetaY));


    psPoint->fX = fX;
    psPoint->fY = fY;
    psPoint->fZ = fZ;
}


void rotateSurface(char cSurfaceCharacter,
        int iMinSurfaceX, int iMaxSurfaceX,
        int iMinSurfaceY, int iMaxSurfaceY,
        int iMinSurfaceZ, int iMaxSurfaceZ)
{
    Coordinate sPoint;
    float fIncrementX, fIncrementY, fIncrementZ;
    int iScreenBufferIdx = 0;

    // rotate every point in the surface
    for (fIncrementX = iMinSurfaceX*CUBE_SIZE/2; fIncrementX <= iMaxSurfaceX*CUBE_SIZE/2; fIncrementX += SAMPLE_RATE)
    {
        for (fIncrementY = iMinSurfaceY*CUBE_SIZE/2; fIncrementY <= iMaxSurfaceY*CUBE_SIZE/2; fIncrementY += SAMPLE_RATE)
        {
            for (fIncrementZ = iMinSurfaceZ*CUBE_SIZE/2; fIncrementZ <= iMaxSurfaceZ*CUBE_SIZE/2; fIncrementZ += SAMPLE_RATE)
            {
                sPoint.fX = fIncrementX;
                sPoint.fY = fIncrementY;
                sPoint.fZ = fIncrementZ;

                computeRotation(&sPoint, fAngleX, fAngleY, fAngleZ);

                iScreenBufferIdx = ((int)(sPoint.fZ*ASPECT_RATIO) + VERTICAL_PADDING)*SCREEN_WIDTH + (int)sPoint.fX + HORIZONTAL_PADDING;

                if (acScreenBuffer[iScreenBufferIdx] == BACKGROUND_CHAR || sPoint.fY > afYBuffer[iScreenBufferIdx])
                {
                    afYBuffer[iScreenBufferIdx] = sPoint.fY;
                    acScreenBuffer[iScreenBufferIdx] = cSurfaceCharacter;
                }
            }
        }
    }
}


int main(void)
{
    int iCharacterCpt;

    for (;;)
    {
        // clear console
        printf("\x1b[2J\x1b[H");

        // reset buffer
        memset(acScreenBuffer, BACKGROUND_CHAR, BUFFER_SIZE);

        // Rotate all 6 surfaces of the cube around (0, 0, 0)
        rotateSurface('#', -1,  1, -1, -1, -1,  1); // front
        rotateSurface('@', -1,  1,  1,  1, -1,  1); // rear
        rotateSurface('^', -1, -1, -1,  1, -1,  1); // left
        rotateSurface('*',  1,  1, -1,  1, -1,  1); // right
        rotateSurface('-', -1,  1, -1,  1,  1,  1); // top
        rotateSurface('|', -1,  1, -1,  1, -1, -1); // bottom

        // draw cube
        for (iCharacterCpt = 0; iCharacterCpt < BUFFER_SIZE; iCharacterCpt++)
        {
            putchar(!(iCharacterCpt%SCREEN_WIDTH) ? '\n' : acScreenBuffer[iCharacterCpt]);
        }

        // update rotation angles
        fAngleX += ROTATION_X;
        fAngleY += ROTATION_Y;
        fAngleZ += ROTATION_Z;

        if (fAngleX >= 2*M_PI)
            fAngleX = 0;

        if (fAngleY >= 2*M_PI)
            fAngleY = 0;

        if (fAngleZ >= 2*M_PI)
            fAngleZ = 0;

        usleep(SLEEP_TIME);
    }

    return 0;
}

