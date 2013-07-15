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
        struct Particle* i = avail[loq];
        //Indicate that the particle is in use
        i->set = TRUE;
        
        //Set x and y
        i->x = xCoord;
        i->y = yCoord;

        //Put it in the allcoords array
        allCoords[getIndex(xCoord, yCoord)] = i;

        // Set the element of the point
        i->element = element;

        //Set the velocities
        i->xVel = 0;
        i->yVel = 0;

        //Set the frozen state
        i->frozen = FALSE;

        //Clear all particle special vals
        clearSpecialVals(i);

        //Set the initial heat
        if(element->startingTemp == 0)
        {
            //__android_log_write(ANDROID_LOG_INFO, "TheElements", "Atmosphere temp");
            i->heat = cAtmosphere->heat; //To be a variable later on
        }
        else
        {
            //__android_log_write(ANDROID_LOG_ERROR, "TheElements", "Element temp");
            i->heat = element->startingTemp;
        }

        //Set the point in the pixels array
        setBitmapColor(xCoord, yCoord, element);

        //Unfreeze particles around it
        //unFreezeParticles(xCoord, yCoord);
    }
}
void deletePoint(struct Particle* particle)
{
    //Store x and y in temporary variables for faster use
    int tempX = particle->x;
    int tempY = particle->y;

    //Unfreeze the particles around it
    //unFreezeParticles(tempX, tempY);
    //Clear it in the pixels array
    clearBitmapColor(tempX, tempY);
    //Clear it in the points array
    allCoords[getIndex(tempX, tempY)] = NULL;

    //Unset the particle
    particle->set = FALSE;
    //Add it to the avail array
    avail[loq] = particle;
    loq++;
}
void unSetPoint(struct Particle* particle)
{
    //Unset the particle
    particle->set = FALSE;
    //Add it to the avail array
    avail[loq] = particle;
    loq++;
}

void setElement(struct Particle* particle, struct Element* newElement)
{
    int i;
    particle->element = newElement;
    clearSpecialVals(particle);
    setBitmapColor(particle->x, particle->y, newElement);
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
char hasSpecial(struct Particle* tempParticle, int special)
{
    char* specials;
    long start = tempParticle->element->specials;
    long end = start + MAX_SPECIALS;
    for(specials = start; specials < end; ++specials)
    {
        if (*specials == special) return TRUE;
    }
    return FALSE;
}

//Gets a particle's special value
//WARNING: Just gets the first special of that type it finds
char getParticleSpecialVal(struct Particle* tempParticle, int special)
{
    char * specials;
    long start =  tempParticle->element->specials;
    long end = start + MAX_SPECIALS;
    for (specials = start; specials < end; ++specials)
    {
        if (*specials == special)
        {
            return tempParticle->specialVals[(long)specials-start];
        }
    }

    return SPECIAL_VAL_UNSET;
}
//Sets a particle's special value
//WARNING: Just sets the first special of that type it finds
void setParticleSpecialVal(struct Particle* tempParticle, int special, char val)
{
    char * specials;
    long start = tempParticle->element->specials;
    long end = start + MAX_SPECIALS;
    for (specials = start; specials < end; ++specials)
    {
        if (*specials == special)
        {
            tempParticle->specialVals[(long)specials-start] = val;
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

void clearSpecialVals(struct Particle* tempParticle)
{
    char* specialVals = tempParticle->specialVals;;
    long end = (long)specialVals  + MAX_SPECIALS;
    for ( ; specialVals < end; ++specialVals)
    {
        *specialVals = SPECIAL_VAL_UNSET;
    }
}
