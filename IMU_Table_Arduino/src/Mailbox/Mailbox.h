#ifndef MAILBOX_H
#define MAILBOX_H

#include "Message_Structure.h"

#define HI_16(x)    ((x) & 0xFF00)
#define LO_16(x)    ((x) & 0x00FF)
#define HI_32(x)    ((x) & 0xFFFF0000)
#define LO_32(x)    ((x) & 0x0000FFFF)

#define MERGE_16(hi, lo)    ((((hi) & 0x00FF) << 8) + ((lo) & 0x00FF))
#define MERGE_32(hi, lo)    ((((hi) & 0x0000FFFF) << 16) + ((lo) & 0x0000FFFF))

/* The timeout threshold for communications in milliseconds ///////////////////////////////////////

    If the amount of time specified above (in ms) is reached without a proper message being
    received, the mailbox state will divert into Loss of Communications.
*/
#define _LOC_TIMEOUT_MS 100

class MailBox
{
public:

    //Structs
    typedef enum class MailboxState_T
    {
        eNormal         =   0x10,

        //Loss of Com states
        eLOC_1          =   0x11,
        eLOC_2          =   0x12,
        eLOC_3          =   0x13,

        //Recovery states
        eRecovery       =   0x18,
        eRecovery_LOC   =   0x19,

        //Startup phase
        eStart          =   0xAA
    };    

    //Public Functions
    MailBox();
    ~MailBox();

    RX_Message & Get_RX()   { return mRX; }

    MailboxState_T MailboxState() const { return stTXState; }

    void Set_Recovery()             { stTXState = MailboxState_T::eRecovery; }

    void Set_TX(TX_Message & oTX)    
    {
        //Set TX_Ready to true to show that a new message has been set
        bStartup = false;
        mTX = oTX;
    }

protected:

    //Protected Structs
    typedef struct Letter_T
    {
        //Message header
        uint8_t cMessageType;
        uint8_t nMessageLength;

        //Data
        char * cData;
        uint16_t nCRC : 15;

        //Stop byte
        const uint8_t nStopByte = 0xE7;
    };
    
    //Protected Functions
    MailboxState_T updateStateMachine();

    void Induce_LOC()   { bLOC_Induced = true; }
    void Process_RX();
    void Process_TX();
    void RX();
    void TX();

    //Protected Members
    uint8_t nTX_Message_Length, nRX_Message_Length;   //Dynamic message size depending on the mailbox status
    uint8_t nBytesReceived, nBytesSent;
    uint32_t nRXSequenceNum;

private:

    //Private Functions
    bool is_LOC(MailboxState_T stState)
    {
        if(stState == MailboxState_T::eLOC_1 ||
           stState == MailboxState_T::eLOC_2 ||
           stState == MailboxState_T::eLOC_3 ||
           stState == MailboxState_T::eRecovery_LOC)
            return true;
        else
            return false;
    }

    bool checkCRC(Letter_T & lLetter);

    uint16_t computeCRC(Letter_T & lLetter);
    uint16_t computeCRC(char * cStr, int nLen);
    
    virtual void RX_Specific(Letter_T & lLetter) = 0;    //Pure virtual function to define the RX subroutine specific to the supporting platform
    virtual void TX_Specific(Letter_T & lLetter) = 0;    //Pure virtual function to define the TX subroutine specific to the supporting platform

    //Private Members                                          //Flags for message timing
    bool bLOC_Induced, bStartup;
    char cRX_Buf[_RX_MESSAGE_LENGTH_NORMAL], cTX_Buf[_TX_MESSAGE_LENGTH_NORMAL];    //Char buffers for serial messaging

    MailboxState_T stTXState, stRXState;
    RX_Message mRX;                                 //RX Message structure
    TX_Message mTX;                                 //TX Message structure
};

#endif
