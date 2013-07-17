/*
 * points.c
 * ----------------------------
 * Defines functions for manipulating data
 * for individual points, such as adding and
 * deleting points.
 */

#include "points.h"

static int dx;
static int dy;

void createPoint(int xCoord, int yCoord, struct Element* element)
{
    //If there are points available
    if (loq > 0)
    {
        //Decrement number of points available
        loq--;
        //Get the pointer to the particle
        int i = avail[loq];
        //Indicate that the particle is in use
        a_set[i] = TRUE;
        
        //Set x and y
        a_x[i] = xCoord;
        a_y[i] = yCoord;

        //Put it in the allcoords array
        allCoords[getIndex(xCoord, yCoord)] = i;

        // Set the element of the point
        a_element[i] = element;

        //Set the velocities
        a_xVel[i] = 0;
        a_yVel[i] = 0;

        //Set the frozen state
        a_frozen[i] = FALSE;

        //Clear all particle special vals
        clearSpecialVals(i);

        //Set the initial heat
        if(element->startingTemp == 0)
        {
            //__android_log_write(ANDROID_LOG_INFO, "TheElements", "Atmosphere temp");
            a_heat[i] = cAtmosphere->heat; //To be a variable later on
        }
        else
        {
            //__android_log_write(ANDROID_LOG_ERROR, "TheElements", "Element temp");
            a_heat[i] = element->startingTemp;
        }

        //Set the point in the pixels array
        setBitmapColor(xCoord, yCoord, element);

        //Unfreeze particles around it
        //unFreezeParticles(xCoord, yCoord);
    }
}
void deletePoint(int particle)
{
    //Store x and y in temporary variables for faster use
    int tempX = a_x[particle];
    int tempY = a_y[particle];

    //Unfreeze the particles around it
    //unFreezeParticles(tempX, tempY);
    //Clear it in the pixels array
    clearBitmapColor(tempX, tempY);
    //Clear it in the points array
    allCoords[getIndex(tempX, tempY)] = NULL;

    //Unset the particle
    a_set[particle] = FALSE;
    //Add it to the avail array
    avail[loq] = particle;
    loq++;
}
void unSetPoint(int particle)
{
    //Unset the particle
    a_set[particle] = FALSE;
    //Add it to the avail array
    avail[loq] = particle;
    loq++;
}

void setElement(int particle, struct Element* newElement)
{
    int i;
    a_element[particle] = newElement;
    clearSpecialVals(particle);
    setBitmapColor(a_x[particle], a_y[particle], newElement);
}

void setBitmapColor(int xCoord, int yCoord, struct Element* element)
{
    unsigned char* pixel = colors + (getColorIndex(xCoord,yCoord) *3);
    pixel[0] = element->red;
    pixel[1] = element->green;
    pixel[2] = element->blue;
}
void clearBitmapColor(int xCoord, int yCoord)
{
    unsigned char* pixel = colors +  (getColorIndex(xCoord, yCoord) *3 ); //shift faster than *4
    pixel[0] = cAtmosphere->backgroundRed;
    pixel[1] = cAtmosphere->backgroundGreen;
    pixel[2] = cAtmosphere->backgroundBlue;
}
void unFreezeParticles(int xCoord, int yCoord)
{
    /*
      int tempX, tempY;
      for (dx = -1; dx <= 1; dx++)
      {
      for (dy = -1; dy <= 1; dy++)
      {
      tempX = xCoord + dx;
      tempY = yCoord + dy;
      if (tempX < workWidth && tempX >= 0 && tempY < workHeight && tempY >= 0)
      {
      struct Particle* tempParticle = allCoords[getIndex(tempX, tempY)];

      if (tempParticle != NULL)
      {
      //Unfreeze the particle
      tempParticle->frozen = FALSE;
      }
      }
      }
      }
    */
}

//Change a particle's heat and fix it
void changeHeat(char *heat, int heatChange)
{
    if (heatChange < 0)
    {
        if (*heat + heatChange < 0)
        {
            *heat = 0;
        }
        else
        {
            *heat += heatChange;
        }
    }
    else
    {
        if (*heat + heatChange > 255)
        {
            *heat = 255;
        }
        else
        {
            *heat += heatChange;
        }
    }
}

//Function to check if a particle has a given special
char hasSpecial(int tempParticle, int special)
{
    char* specials;
    long start = a_element[tempParticle]->specials;
    long end = start + MAX_SPECIALS;
    for(specials = start; specials < end; ++specials)
    {
        if (*specials == special) return TRUE;
    }
    return FALSE;
}

//Gets a particle's special value
//WARNING: Just gets the first special of that type it finds
char getParticleSpecialVal(int tempParticle, int special)
{
    char * specials;
    long start =  a_element[tempParticle]->specials;
    long end = start + MAX_SPECIALS;
    for (specials = start; specials < end; ++specials)
    {
        if (*specials == special)
        {
            return a_specialVals[tempParticle][(long)specials-start];
        }
    }

    return SPECIAL_VAL_UNSET;
}
//Sets a particle's special value
//WARNING: Just sets the first special of that type it finds
void setParticleSpecialVal(int tempParticle, int special, char val)
{
    char * specials;
    long start = a_element[tempParticle]->specials;
    long end = start + MAX_SPECIALS;
    for (specials = start; specials < end; ++specials)
    {
        if (*specials == special)
        {
            a_specialVals[tempParticle][(long)specials-start] = val;
        }
    }
}
//Gets an element's special value
//WARNING: Just gets the first special of that type it finds
char getElementSpecialVal(struct Element* tempElement, int special)
{
    char * specials;
    long start = tempElement->specials;
    long end = start + MAX_SPECIALS;
    for (specials = start; specials < end; ++specials)
    {
        if (*specials == special)
        {
            return tempElement->specialVals[(long)specials-start];
        }
    }
}

void clearSpecialVals(int tempParticle)
{
    char* specialVals = a_specialVals[tempParticle];
    long end = (long)specialVals  + MAX_SPECIALS;
    for ( ; specialVals < end; ++specialVals)
    {
        *specialVals = SPECIAL_VAL_UNSET;
    }
}
