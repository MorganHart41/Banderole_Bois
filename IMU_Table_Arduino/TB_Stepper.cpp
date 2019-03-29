#include "TB_Stepper.h"

//  TB_Stepper -- Public Functions  ///////////////////////////////////////////////////////////////

/*-------------------------------------------------------------------------------------------------
  Constructor -- Default
*/
TB_Stepper::TB_Stepper() 
{
  nSteps = 0;
}

/*-------------------------------------------------------------------------------------------------
  Initialize

  Initializes the stepper instance, mapping the pins required for operation.

  INPUT (int) --  nPulsePin
                  The number corresponding to the digital pin that drives the pulse signal to the
                  stepper motor.
  
  INPUT (int) --  nDirectionPin
                  The number corresponding to the digital pin that drives the direction signal to the
                  stepper motor.
  
  INPUT (int) --  nEnablePin
                  The number corresponding to the digital pin that drives the enable signal to the
                  stepper motor.
  
  OUTPUT (void)
*/
void TB_Stepper::Initialize(int nPulsePin, int nDirectionPin, int nEnablePin) 
{
  this->nPulsePin = nPulsePin;
  this->nDirectionPin = nDirectionPin;
  this->nEnablePin = nEnablePin;

  pinMode(nPulsePin, OUTPUT);
  pinMode(nDirectionPin, OUTPUT);
  pinMode(nEnablePin, OUTPUT);
}

/*-------------------------------------------------------------------------------------------------
  Step

  Increments the number of steps for the stepper motor to complete.

  INPUT (int) --  nSteps
                  The number of steps for the motor to take. Adds on to what is currently left.
  
  OUTPUT (void)
*/
void TB_Stepper::Step(int nSteps) 
{
  this->nSteps += nSteps;
}

static void TB_Stepper::Set_Enable(TB_Stepper & stepperInstance, const bool bEnable)
{
  stepperInstance.Set_Enable(bEnable);
}

static void TB_Stepper::Set_StepsPerSecond(TB_Stepper & stepperInstance, const float fStepsPerSecond)
{
  stepperInstance.Set_StepsPerSecond(fStepsPerSecond);
}

static void TB_Stepper::Task_Step(void * vParameters)
{
  volatile bool bPulse;
  volatile float fToggleDelay;
  TickType_t tLastWakeTime = xTaskGetTickCount();

  //Initialize stepper instance from the task parameters
  TB_Stepper * stepperInstance = (TB_Stepper *)vParameters;

  //Infinite Loop
  while(1)
  {
    //Toggle the pulse pin
    bPulse = stepperInstance->Toggle();

    //  Calculate the delay between each pulse toggle:
    //
    //    1. Time-between-each-step = 1 second / fStepsPerSecond
    //    2. Toggle delay (in seconds) = Time-Between-Each-Step / 2
    //    3. Toggle delay (in milliseconds) = Toggle delay (in seconds) * 1000
    //
    //  Do this only on high pulses so that each step window is split evenly (the delay only updates before the next step)
    //
    if(bPulse)
      fToggleDelay = ((1 / stepperInstance->fStepsPerSecond) / 2) * 1000;
    
    //Delay the task until next toggle
    vTaskDelayUntil(&tLastWakeTime, pdMS_TO_TICKS((int)fToggleDelay));
  }
}

//  TB_STepper -- Private Functions ///////////////////////////////////////////////////////////////

bool TB_Stepper::Toggle()
{
  //  Boolean to keep track of which state the pulse pin is currently in
  //
  //  Each motor step has two phases:
  //    Low Pulse -- bToggle = false
  //    High Pulse -- bToggle = true
  //
  //  When disabled, bToggle will remain false in order to keep the pulse pin LOW
  //
  static bool bToggle = true;

  //Invert bToggle if the stepper is enabled, keep it the same if not
  bToggle = (bEnabled) ? ((bToggle) ? false : true) : bToggle;

  //  Determine the required state of the directional pin:
  //
  //    If going in the positive direction, the direction signal is LOW
  //    If going in the negative direction, the direction signal is HIGH
  //
  bool bDir = (nSteps >= 0) ? false : true;
  digitalWrite(nDirectionPin, bDir);

  if(bEnabled || !bToggle)             //If the stepper is enabled or if the pulse pin needs to return to LOW
  {
    digitalWrite(nPulsePin, bToggle); //Write the toggle state to the pulse pin

    //Update the steps left if on the high pulse
    nSteps = (bToggle) ? ((bDir) ? (nSteps+1) : (nSteps-1)) : nSteps;
  }

  return bToggle;
}
