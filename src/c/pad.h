#ifndef PAD_H
#define PAD_H

// taken from PSL1GHT sdk
typedef struct 
{
    int len;                                /*!< \brief structure length */
    union{
        unsigned short button[64];          /*!< \brief array of button values */
        struct {
            unsigned short zeroes;                     /*!< \brief zero bits */
            unsigned int : 8;               /*!< \brief reserved */
            unsigned int seven : 4;         /*!< \brief always 0x7 */
            unsigned int halflen : 4;       /*!< \brief len/2 */
            unsigned int : 8;               /*!< \brief reserved */
            /* Button information */
            /* 0: UP, 1: DOWN */
            unsigned int BTN_LEFT : 1;      /*!< \brief left button */
            unsigned int BTN_DOWN : 1;      /*!< \brief down button */
            unsigned int BTN_RIGHT : 1;     /*!< \brief right button */
            unsigned int BTN_UP : 1;        /*!< \brief up button */
            unsigned int BTN_START : 1;     /*!< \brief start button */
            unsigned int BTN_R3 : 1;        /*!< \brief R3 button */
            unsigned int BTN_L3 : 1;        /*!< \brief L3 button */
            unsigned int BTN_SELECT : 1;    /*!< \brief select button */

            unsigned int : 8;               /*!< \brief reserved */
            unsigned int BTN_SQUARE : 1;    /*!< \brief square button */
            unsigned int BTN_CROSS : 1;     /*!< \brief cross button */
            unsigned int BTN_CIRCLE : 1;    /*!< \brief circle button */
            unsigned int BTN_TRIANGLE : 1;  /*!< \brief triangle button */
            unsigned int BTN_R1 : 1;        /*!< \brief R1 button */
            unsigned int BTN_L1 : 1;        /*!< \brief L1 button */
            unsigned int BTN_R2 : 1;        /*!< \brief R2 button */
            unsigned int BTN_L2 : 1;        /*!< \brief L2 button */

            /* Analog nub information */
            /* 0x0000 - 0x00FF */
            /*! \brief right analog nub horizontal position (\c 0x0000 - \c 0x00FF) */
            unsigned int ANA_R_H : 16;
            /*! \brief right analog nub vertical position (\c 0x0000 - \c 0x00FF) */
            unsigned int ANA_R_V : 16;
            /*! \brief left analog nub horizontal position (\c 0x0000 - \c 0x00FF) */
            unsigned int ANA_L_H : 16;
            /*! \brief left analog nub vertical position (\c 0x0000 - \c 0x00FF) */
            unsigned int ANA_L_V : 16;

            /* Pressure-sensitive information */
            /* 0x0000 - 0x00FF */
            /*! \brief right button pressure information (\c 0x0000 - \c 0x00FF) */
            unsigned int PRE_RIGHT : 16;
            /*! \brief left button pressure information (\c 0x0000 - \c 0x00FF) */
            unsigned int PRE_LEFT : 16;
            /*! \brief up button pressure information (\c 0x0000 - \c 0x00FF) */
            unsigned int PRE_UP : 16;
            /*! \brief down button pressure information (\c 0x0000 - \c 0x00FF) */
            unsigned int PRE_DOWN : 16;
            /*! \brief triangle button pressure information (\c 0x0000 - \c 0x00FF) */
            unsigned int PRE_TRIANGLE : 16;
            /*! \brief circle button pressure information (\c 0x0000 - \c 0x00FF) */
            unsigned int PRE_CIRCLE : 16;
            /*! \brief cross button pressure information (\c 0x0000 - \c 0x00FF) */
            unsigned int PRE_CROSS : 16;
            /*! \brief square button pressure information (\c 0x0000 - \c 0x00FF) */
            unsigned int PRE_SQUARE : 16;
            /*! \brief L1 button pressure information (\c 0x0000 - \c 0x00FF) */
            unsigned int PRE_L1 : 16;
            /*! \brief R1 button pressure information (\c 0x0000 - \c 0x00FF) */
            unsigned int PRE_R1 : 16;
            /*! \brief L2 button pressure information (\c 0x0000 - \c 0x00FF) */
            unsigned int PRE_L2 : 16;
            /*! \brief R2 button pressure information (\c 0x0000 - \c 0x00FF) */
            unsigned int PRE_R2 : 16;

            /* Sensor information */
            /* 0x0000 - 0x03FF */
            /*! \brief X sensor information (\c 0x0000 - \c 0x03FF) */
            unsigned int SENSOR_X : 16;
            /*! \brief Y sensor information (\c 0x0000 - \c 0x03FF) */
            unsigned int SENSOR_Y : 16;
            /*! \brief Z sensor information (\c 0x0000 - \c 0x03FF) */
            unsigned int SENSOR_Z : 16;
            /*! \brief G sensor information (\c 0x0000 - \c 0x03FF) */
            unsigned int SENSOR_G : 16;

            /* BD remote information */
            unsigned int BTN_BDLEN : 16;    /*! \brief BD remote len information */
            /*! 0x0000 - 0xffff */
            unsigned int BTN_BDCODE : 16;   /*! \brief BD remote code information see ioPadBdCode for values */

            /*! \brief reserved */
            char reserved[76];
        };
    };
} cellPadData;



#endif