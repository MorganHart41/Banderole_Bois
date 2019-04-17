#ifndef SUBPOSITION_H
#define SUBPOSITION_H


class SubPosition
{
public:
    //Constructor
    SubPosition(){}
    SubPosition(float newStep, float newX, float newY, float newZ){step = newStep; positionX = newX; positionY = newY; positionZ = newZ;}

    //Getters
    float getStep(){ return step;}
    float getPositionX(){ return positionX;}
    float getPositionY(){ return positionY;}
    float getPositionZ(){ return positionZ;}

    //Setters
    void setStep(float newStep){ step = newStep;}
    void setPositionX(float newPositionX){ positionX = newPositionX;}
    void setPositionY(float newPositionY){ positionY = newPositionY;}
    void setPositionZ(float newPositionZ){ positionZ = newPositionZ;}

private:
    float step;
    float positionX;
    float positionY;
    float positionZ;
};

#endif // PROCEDURALSTEP_H
