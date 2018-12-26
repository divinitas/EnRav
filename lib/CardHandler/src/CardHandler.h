#ifndef _CARD_HANDLER_H
    #define _CARD_HANDLER_H

    #include "Arduino.h"

    #include "MFRC522.h"

    class CardHandler
    {

        #define     INFORMATION_BLOCK_MIFARE_1K         4
        #define     INFORMATION_BLOCK_MIFARE_ULTRA      8

        #define     INFORMATION_BLOCK_SIZE              16


        #define     TARGET_BLOCK_MIFARE_1K              8
        #define     TARGET_BLOCK_MIFARE_ULTRA           16
   
        public:

            enum CardStatus {
                NoCard,
                ValidCard,
                UnknownCard,
            };

            // Constructor.  Only sets pin values.  Doesn't touch the chip.  Be sure to call begin()!
            CardHandler(uint8_t cs, uint8_t rst);
            ~CardHandler();

            enum CardStatus getCardStatus();

            void begin( QueueHandle_t *pPlayerCommandQueue );

        private:
                    // this object stores nfc tag data
            typedef struct {
                union {
                    struct {
                        struct {
                            uint32_t    Cookie;             // 4 byte
                            uint32_t    Version;            // 4 byte
                        } Header;
                        struct {
                            int16_t     TargetLength;       // 2 byte
                            uint8_t     Configuration;      // 1 byte
                            uint8_t     LastListPostion;    // 1 Byte
                            uint32_t    LastFilePosition;   // 4 byte
                        } MetaData;
                    }Entry;                             // complete 16 byte
                    uint8_t             Raw[16];
                }Information;

                char                    *pTarget;
                bool                    TagValid;
            } nfcTag_s;


            TaskHandle_t            m_handle;

            MFRC522                 *m_pRfReader;
            MFRC522::MIFARE_Key     m_MFRC522Key;
            nfcTag_s                m_NfcTag;

            enum CardStatus         m_CardStatus;
            byte                    m_lastCardUid[10];
            uint8_t                 m_lastCardUidSize;


            static const    uint32_t    magicKey = 0x13374258;

            //
            void Run( void );
            void CleanUp( void );

            void DumpByteArray(const char *header, byte *buffer, byte bufferSize);
            
            static void TaskFunctionAdapter(void *pvParameters);

            bool ReadInformationFromTag( void );
            bool WriteInformationToTag( void );

    };

#endif