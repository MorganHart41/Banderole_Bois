#include "Message_Structure.h"

// RX_Message -- Public Functions /////////////////////////////////////////////////////////////////

/*-------------------------------------------------------------------------------------------------
    Constructor -- Default
*/
RX_Message::RX_Message()
{
    nSequenceNum = nHashCompare = 0;

    stStatus.bOperation_En = 0;
    stStatus.bMotor_x_En = 0;
    stStatus.bMotor_y_En = 0;
    stStatus.bMotor_z_En = 0;

    fPosition_x_cmd = fPosition_y_cmd = fPosition_z_cmd = 0;
    fVelocity_x_cmd = fVelocity_y_cmd = fVelocity_z_cmd = 0;
    fTorque_x_cmd = fTorque_y_cmd = fTorque_z_cmd = 0;
}

/*-------------------------------------------------------------------------------------------------
    Constructor -- Accepts RX_Message_Structure_Normal_T structure
*/
RX_Message::RX_Message(RX_Message_Structure_Normal_T & stMessage)
{
    nSequenceNum = stMessage.nSequenceNum;

    stStatus.bOperation_En = stMessage.stStatus.bOperation_En;
    stStatus.bMotor_x_En = stMessage.stStatus.bMotor_x_En;
    stStatus.bMotor_y_En = stMessage.stStatus.bMotor_y_En;
    stStatus.bMotor_z_En = stMessage.stStatus.bMotor_z_En;

    fPosition_x_cmd = stMessage.fPosition_x_cmd;
    fPosition_y_cmd = stMessage.fPosition_y_cmd;
    fPosition_z_cmd = stMessage.fPosition_z_cmd;
    fVelocity_x_cmd = stMessage.fVelocity_x_cmd;
    fVelocity_y_cmd = stMessage.fVelocity_y_cmd;
    fVelocity_z_cmd = stMessage.fVelocity_z_cmd;
    fTorque_x_cmd = stMessage.fTorque_x_cmd;
    fTorque_y_cmd = stMessage.fTorque_y_cmd;
    fTorque_z_cmd = stMessage.fTorque_z_cmd;
}

/*-------------------------------------------------------------------------------------------------
    Constructor -- Accepts RX_Message_Structure_T structure
*/
RX_Message::RX_Message(RX_Message_Structure_Recovery_T & stMessage)
{
    nSequenceNum = stMessage.nSequenceNum;

    stStatus.bOperation_En = 0;
    stStatus.bMotor_x_En = 0;
    stStatus.bMotor_y_En = 0;
    stStatus.bMotor_z_En = 0;

    nHashCompare = stMessage.nHashCompare;
}

/*-------------------------------------------------------------------------------------------------
    Destructor -- Default
*/
RX_Message::~RX_Message()
{

}

// TX_Message -- Public Functions  ////////////////////////////////////////////////////////////////

/*-------------------------------------------------------------------------------------------------
    Constructor -- Default
*/
TX_Message::TX_Message()
{
    nSequenceNum = nHashCompare = 0;

    stStatus.stFlags_x.bOverTemp = 0;
    stStatus.stFlags_x.bOverVoltage = 0;
    stStatus.stFlags_x.bOverCurrent = 0;
    stStatus.stFlags_x.bOverVelocity = 0;
    stStatus.stFlags_x.bOverTorque = 0;

    stStatus.stFlags_y.bOverTemp = 0;
    stStatus.stFlags_y.bOverVoltage = 0;
    stStatus.stFlags_y.bOverCurrent = 0;
    stStatus.stFlags_y.bOverVelocity = 0;
    stStatus.stFlags_y.bOverTorque = 0;

    stStatus.stFlags_z.bOverTemp = 0;
    stStatus.stFlags_z.bOverVoltage = 0;
    stStatus.stFlags_z.bOverCurrent = 0;
    stStatus.stFlags_z.bOverVelocity = 0;
    stStatus.stFlags_z.bOverTorque = 0;

    stStatus.bOperating = 0;
    stStatus.bCommandInProgress = 0;
    stStatus.bCommandComplete = 0;

    fPosition_x_cmd = fPosition_y_cmd = fPosition_z_cmd = 0;
    fVelocity_x_cmd = fVelocity_y_cmd = fVelocity_z_cmd = 0;
    fTorque_x_cmd = fTorque_y_cmd = fTorque_z_cmd = 0;
    fPosition_x = fPosition_y = fPosition_z = 0;
    fVelocity_x = fVelocity_y = fVelocity_z = 0;
    fTorque_x = fTorque_y = fTorque_z = 0;
    fTemp_x = fTemp_y = fTemp_z = 0;
    fVoltage_x = fVoltage_y = fVoltage_z = 0;
    fCurrent_x = fCurrent_y = fCurrent_z = 0;
    fDiffDegrees_x = fDiffDegrees_y = fDiffDegrees_z = 0;
    fDiffSteps_x = fDiffSteps_y = fDiffSteps_z = 0;
}

/*-------------------------------------------------------------------------------------------------
    Destructor -- Default
*/
TX_Message::~TX_Message()
{

}

// TX_Message -- Private Functions ////////////////////////////////////////////////////////////////

/*-------------------------------------------------------------------------------------------------
    encode_MessageStructure --  2 Overloads

    1.
        Encodes the provided TX_Message_Stucture_Normal_T with the information stored within the TX
        message.

        INPUT (TX_Message_Structure_Normal_T &) --  stTX
            Empty message structure which is populated with the TX message data.

        OUTPUT (void)

    2.
        Encodes the provided TX_Message_Stucture_Normal_T with the information stored within the TX
        message.

        INPUT (TX_Message_Structure_Normal_T &) --  stTX
            Empty message structure which is populated with the TX message data.

        OUTPUT (void)
*/

//Overload 1:
void TX_Message::encode_MessageStructure(TX_Message_Structure_Normal_T & stTX)
{
    stTX.stStatus.stFlags_x.bOverTemp = stStatus.stFlags_x.bOverTemp;
    stTX.stStatus.stFlags_x.bOverVoltage = stStatus.stFlags_x.bOverVoltage;
    stTX.stStatus.stFlags_x.bOverCurrent = stStatus.stFlags_x.bOverCurrent;
    stTX.stStatus.stFlags_x.bOverVelocity = stStatus.stFlags_x.bOverVelocity;
    stTX.stStatus.stFlags_x.bOverTorque = stStatus.stFlags_x.bOverTorque;

    stTX.stStatus.stFlags_y.bOverTemp = stStatus.stFlags_y.bOverTemp;
    stTX.stStatus.stFlags_y.bOverVoltage = stStatus.stFlags_y.bOverVoltage;
    stTX.stStatus.stFlags_y.bOverCurrent = stStatus.stFlags_y.bOverCurrent;
    stTX.stStatus.stFlags_y.bOverVelocity = stStatus.stFlags_y.bOverVelocity;
    stTX.stStatus.stFlags_y.bOverTorque = stStatus.stFlags_y.bOverTorque;

    stTX.stStatus.stFlags_z.bOverTemp = stStatus.stFlags_z.bOverTemp;
    stTX.stStatus.stFlags_z.bOverVoltage = stStatus.stFlags_z.bOverVoltage;
    stTX.stStatus.stFlags_z.bOverCurrent = stStatus.stFlags_z.bOverCurrent;
    stTX.stStatus.stFlags_z.bOverVelocity = stTX.stStatus.stFlags_z.bOverVelocity;
    stTX.stStatus.stFlags_z.bOverTorque = stTX.stStatus.stFlags_z.bOverTorque;

    stTX.stStatus.bOperating = stTX.stStatus.bOperating;
    stTX.stStatus.bCommandInProgress = stTX.stStatus.bCommandInProgress;
    stTX.stStatus.bCommandComplete = stTX.stStatus.bCommandComplete;

    stTX.fPosition_x_cmd = fPosition_x_cmd;
    stTX.fPosition_y_cmd = fPosition_y_cmd;
    stTX.fPosition_z_cmd = fPosition_z_cmd;
    stTX.fVelocity_x_cmd = fVelocity_x_cmd;
    stTX.fVelocity_y_cmd = fVelocity_y_cmd;
    stTX.fVelocity_z_cmd = fVelocity_z_cmd;
    stTX.fTorque_x_cmd = fTorque_x_cmd;
    stTX.fTorque_y_cmd = fTorque_y_cmd;
    stTX.fTorque_z_cmd = fTorque_z_cmd;
    stTX.fPosition_x = fPosition_x;
    stTX.fPosition_y = fPosition_y;
    stTX.fPosition_z = fPosition_z;
    stTX.fVelocity_x = fVelocity_x;
    stTX.fVelocity_y = fVelocity_y;
    stTX.fVelocity_z = fVelocity_z;
    stTX.fTorque_x = fTorque_x;
    stTX.fTorque_y = fTorque_y;
    stTX.fTorque_z = fTorque_z;
    stTX.fTemp_x = fTemp_x;
    stTX.fTemp_y = fTemp_y;
    stTX.fTemp_z = fTemp_z;
    stTX.fVoltage_x = fVoltage_x;
    stTX.fVoltage_y = fVoltage_y;
    stTX.fVoltage_z = fVoltage_z;
    stTX.fCurrent_x = fCurrent_x;
    stTX.fCurrent_y = fCurrent_y;
    stTX.fCurrent_z = fCurrent_z;
    stTX.fDiffDegrees_x = fDiffDegrees_x;
    stTX.fDiffDegrees_y = fDiffDegrees_y;
    stTX.fDiffDegrees_z = fDiffDegrees_z;
    stTX.fDiffSteps_x = fDiffSteps_x;
    stTX.fDiffSteps_y = fDiffSteps_y;
    stTX.fDiffSteps_z = fDiffSteps_z;
}

//Overload 2:
void TX_Message::encode_MessageStructure(TX_Message_Structure_Recovery_T & stTX)
{
    stTX.stStatus.stFlags_x.bOverTemp = stStatus.stFlags_x.bOverTemp;
    stTX.stStatus.stFlags_x.bOverVoltage = stStatus.stFlags_x.bOverVoltage;
    stTX.stStatus.stFlags_x.bOverCurrent = stStatus.stFlags_x.bOverCurrent;
    stTX.stStatus.stFlags_x.bOverVelocity = stStatus.stFlags_x.bOverVelocity;
    stTX.stStatus.stFlags_x.bOverTorque = stStatus.stFlags_x.bOverTorque;

    stTX.stStatus.stFlags_y.bOverTemp = stStatus.stFlags_y.bOverTemp;
    stTX.stStatus.stFlags_y.bOverVoltage = stStatus.stFlags_y.bOverVoltage;
    stTX.stStatus.stFlags_y.bOverCurrent = stStatus.stFlags_y.bOverCurrent;
    stTX.stStatus.stFlags_y.bOverVelocity = stStatus.stFlags_y.bOverVelocity;
    stTX.stStatus.stFlags_y.bOverTorque = stStatus.stFlags_y.bOverTorque;

    stTX.stStatus.stFlags_z.bOverTemp = stStatus.stFlags_z.bOverTemp;
    stTX.stStatus.stFlags_z.bOverVoltage = stStatus.stFlags_z.bOverVoltage;
    stTX.stStatus.stFlags_z.bOverCurrent = stStatus.stFlags_z.bOverCurrent;
    stTX.stStatus.stFlags_z.bOverVelocity = stTX.stStatus.stFlags_z.bOverVelocity;
    stTX.stStatus.stFlags_z.bOverTorque = stTX.stStatus.stFlags_z.bOverTorque;

    stTX.stStatus.bOperating = stTX.stStatus.bOperating;
    stTX.stStatus.bCommandInProgress = stTX.stStatus.bCommandInProgress;
    stTX.stStatus.bCommandComplete = stTX.stStatus.bCommandComplete;

    stTX.fPosition_x = fPosition_x;
    stTX.fPosition_y = fPosition_y;
    stTX.fPosition_z = fPosition_z;
    stTX.fVelocity_x = fVelocity_x;
    stTX.fVelocity_y = fVelocity_y;
    stTX.fVelocity_z = fVelocity_z;
    stTX.fTorque_x = fTorque_x;
    stTX.fTorque_y = fTorque_y;
    stTX.fTorque_z = fTorque_z;
    stTX.fTemp_x = fTemp_x;
    stTX.fTemp_y = fTemp_y;
    stTX.fTemp_z = fTemp_z;
    stTX.fVoltage_x = fVoltage_x;
    stTX.fVoltage_y = fVoltage_y;
    stTX.fVoltage_z = fVoltage_z;
    stTX.fCurrent_x = fCurrent_x;
    stTX.fCurrent_y = fCurrent_y;
    stTX.fCurrent_z = fCurrent_z;
    stTX.fDiffDegrees_x = fDiffDegrees_x;
    stTX.fDiffDegrees_y = fDiffDegrees_y;
    stTX.fDiffDegrees_z = fDiffDegrees_z;
    stTX.fDiffSteps_x = fDiffSteps_x;
    stTX.fDiffSteps_y = fDiffSteps_y;
    stTX.fDiffSteps_z = fDiffSteps_z;

    stTX.nHashCompare = nHashCompare;
}