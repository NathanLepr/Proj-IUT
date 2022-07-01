#include "mbed.h"
#include "stm32746g_discovery_audio.h"
#include "stm32746g_discovery_sdram.h"
#include "UnbufferedSerial.h"
#include "stm32746g_discovery_lcd.h"
#include "SampleFilter.h"

typedef enum {
    BUFFER_OFFSET_NONE = 0,
    BUFFER_OFFSET_HALF = 1,
    BUFFER_OFFSET_FULL = 2,
} BUFFER_StateTypeDef;


#define AUDIO_BLOCK_SIZE   ((uint32_t)512)
#define AUDIO_BUFFER_IN     SDRAM_DEVICE_ADDR
#define AUDIO_BUFFER_OUT   (AUDIO_BUFFER_IN + (AUDIO_BLOCK_SIZE * 2))



volatile uint32_t  audio_rec_buffer_state = BUFFER_OFFSET_NONE;

uint16_t * ptrMEM;
uint16_t * ptrDO;
uint16_t * ptrDOd;
uint16_t * ptrRE;
uint16_t * ptrREd;
uint16_t * ptrMI;
uint16_t * ptrFA;
uint16_t * ptrFAd;
uint16_t * ptrSOL;
uint16_t * ptrSOLd;
uint16_t * ptrLA;
uint16_t * ptrLAd;
uint16_t * ptrSI;
uint16_t * ptrDO8;

int amplitude = 1000;

static UnbufferedSerial serial_read(PC_6,PC_7);


int main()
{
    
    //initialisation des codes LCD
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
    BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);
    
    char data[2]={0,0};
    char datapre[2]={0,0};
    int i=0;
    

        
    
    //initialisation des variables : 
    
    int y=0;
    int freqcarre= 900;
    
    int tempo = 500000;
    int datacase = 0;
    
    //On créer les différentes fréquences des notes
    
    double freqDO = 261.63;
    double freqDOd = 277.18;
    double freqRE = 293.66;
    double freqREd = 311.13;
    double freqMI = 329.63;
    double freqFA = 349.23;
    double freqFAd = 369.99;
    double freqSOL = 392.0;
    double freqSOLd = 415.30;
    double freqLA = 440.00;
    double freqLAd = 466.16;
    double freqSI = 493.88;
    double freqDO8 = 523.25;
    
    //On créer les taille mémoire des différentes notes
    
    //123*2
    uint32_t sizeDO = 674;
    //123*2
    uint32_t sizeDOd = 636;
    //109*2
    uint32_t sizeRE = 601;
    //123*2
    uint32_t sizeREd = 567;
    //123*2
    uint32_t sizeMI = 535;
    //109*2
    uint32_t sizeFA = 505;
    //123*2
    uint32_t sizeFAd = 477;
    //123*2
    uint32_t sizeSOL = 450;
    //123*2
    uint32_t sizeSOLd = 425;
    //109*2
    uint32_t sizeLA = 401;
    //123*2
    uint32_t sizeLAd = 378;
    //123*2
    uint32_t sizeSI = 357;
    //109*2
    uint32_t sizeDO8 = 337;
    
    
    

    //initialisation des périphériques
    BSP_AUDIO_IN_OUT_Init(INPUT_DEVICE_DIGITAL_MICROPHONE_2, OUTPUT_DEVICE_HEADPHONE, DEFAULT_AUDIO_IN_FREQ, DEFAULT_AUDIO_IN_BIT_RESOLUTION, DEFAULT_AUDIO_IN_CHANNEL_NBR);
    printf("AUDIO loop from digital micro (U20 & U21 components on board) to headphone (CN10 jack connector)\n");


    /* Initialize SDRAM buffers */
    BSP_SDRAM_Init();
    memset((uint16_t *)AUDIO_BUFFER_IN, 0, AUDIO_BLOCK_SIZE * 2);
    memset((uint16_t *)AUDIO_BUFFER_OUT, 0, AUDIO_BLOCK_SIZE * 2);
    printf("SDRAM init done\n");

    audio_rec_buffer_state = BUFFER_OFFSET_NONE;
    
    //on modifie la fréquence d'échantillonnage à 44100Hz

    BSP_AUDIO_OUT_SetFrequency(44100);


    //replissage de la mémoire pour les notes :
    
    // POUR UN DO
    ptrDO = (uint16_t *)AUDIO_BUFFER_OUT;

    for(i=0; i<sizeDO; i+=2) {
        *(ptrDO + i) = IIRFloat(amplitude*sin(3.14159*freqDO*i/44100.0)) ;
        *(ptrDO + i+1) = IIRFloat(amplitude*sin(3.14159*freqDO*i/44100.0)) ;
    }
    
    // POUR UN DO#
    ptrDOd = (uint16_t *)AUDIO_BUFFER_OUT + sizeDO;

    for(i=0; i<sizeDOd; i+=2) {
        *(ptrDOd + i) = IIRFloat(amplitude*sin(3.14159*freqDOd*i/44100.0)) ;
        *(ptrDOd + i+1) = IIRFloat(amplitude*sin(3.14159*freqDOd*i/44100.0)) ;
    }


    // POUR UN RE
    ptrRE = (uint16_t *)AUDIO_BUFFER_OUT + sizeDO + sizeDOd;

    for(i=0; i<sizeRE; i+=2) {
        *(ptrRE + i) = IIRFloat(amplitude*sin(3.14159*freqRE*i/44100.0)) ;
        *(ptrRE + i+1) = IIRFloat(amplitude*sin(3.14159*freqRE*i/44100.0)) ;
    }
    
    // POUR UN RE#
    ptrREd = (uint16_t *)AUDIO_BUFFER_OUT + sizeDO + sizeDOd + sizeRE;

    for(i=0; i<sizeREd; i+=2) {
        *(ptrREd + i) = IIRFloat(amplitude*sin(3.14159*freqREd*i/44100.0)) ;
        *(ptrREd + i+1) = IIRFloat(amplitude*sin(3.14159*freqREd*i/44100.0)) ;
    }
    
    // POUR UN MI
    ptrMI = (uint16_t *)AUDIO_BUFFER_OUT + sizeDO + sizeDOd + sizeRE + sizeREd;

    for(i=0; i<sizeMI; i+=2) {
        *(ptrMI + i) = IIRFloat(amplitude*sin(3.14159*freqMI*i/44100.0)) ;
        *(ptrMI + i+1) = IIRFloat(amplitude*sin(3.14159*freqMI*i/44100.0)) ;
    }
    
    // POUR UN FA
    ptrFA = (uint16_t *)AUDIO_BUFFER_OUT + sizeDO + sizeDOd + sizeRE + sizeREd + sizeMI;

    for(i=0; i<sizeFA; i+=2) {
        *(ptrFA + i) = IIRFloat(amplitude*sin(3.14159*freqFA*i/44100.0)) ;
        *(ptrFA + i+1) = IIRFloat(amplitude*sin(3.14159*freqFA*i/44100.0)) ;
    }
    
    // POUR UN FA#
    ptrFAd = (uint16_t *)AUDIO_BUFFER_OUT + sizeDO + sizeDOd + sizeRE + sizeREd + sizeMI + sizeFA;

    for(i=0; i<sizeFAd; i+=2) {
        *(ptrFAd + i) = IIRFloat(amplitude*sin(3.14159*freqFAd*i/44100.0)) ;
        *(ptrFAd + i+1) = IIRFloat(amplitude*sin(3.14159*freqFAd*i/44100.0)) ;
    }
    
    // POUR UN SOL
    ptrSOL = (uint16_t *)AUDIO_BUFFER_OUT + sizeDO + sizeDOd + sizeRE + sizeREd + sizeMI + sizeFA + sizeFAd;

    for(i=0; i<sizeSOL; i+=2) {
        *(ptrSOL + i) = IIRFloat(amplitude*sin(3.14159*freqSOL*i/44100.0)) ;
        *(ptrSOL + i+1) = IIRFloat(amplitude*sin(3.14159*freqSOL*i/44100.0)) ;
    }
    
    // POUR UN SOL#
    ptrSOLd = (uint16_t *)AUDIO_BUFFER_OUT + sizeDO + sizeDOd + sizeRE + sizeREd + sizeMI + sizeFA + sizeFAd + sizeSOL;

    for(i=0; i<sizeSOLd; i+=2) {
        *(ptrSOLd + i) = IIRFloat(amplitude*sin(3.14159*freqSOLd*i/44100.0)) ;
        *(ptrSOLd + i+1) = IIRFloat(amplitude*sin(3.14159*freqSOLd*i/44100.0)) ;
    }
    
    // POUR UN LA
    ptrLA = (uint16_t *)AUDIO_BUFFER_OUT + sizeDO + sizeDOd + sizeRE + sizeREd + sizeMI + sizeFA + sizeFAd + sizeSOL + sizeSOLd;

    for(i=0; i<sizeLA; i+=2) {
        *(ptrLA + i) = IIRFloat(amplitude*sin(3.14159*freqLA*i/44100.0)) ;
        *(ptrLA + i+1) = IIRFloat(amplitude*sin(3.14159*freqLA*i/44100.0)) ;
    }
    
    // POUR UN LA#
    ptrLAd = (uint16_t *)AUDIO_BUFFER_OUT + sizeDO + sizeDOd + sizeRE + sizeREd + sizeMI + sizeFA + sizeFAd + sizeSOL + sizeSOLd + sizeLA;

    for(i=0; i<sizeLAd; i+=2) {
        *(ptrLAd + i) = IIRFloat(amplitude*sin(3.14159*freqLAd*i/44100.0)) ;
        *(ptrLAd + i+1) = IIRFloat(amplitude*sin(3.14159*freqLAd*i/44100.0)) ;
    }
    
    // POUR UN SI
    ptrSI = (uint16_t *)AUDIO_BUFFER_OUT + sizeDO + sizeDOd + sizeRE + sizeREd + sizeMI + sizeFA + sizeFAd + sizeSOL + sizeSOLd + sizeLA + sizeLAd;

    for(i=0; i<sizeSI; i+=2) {
        *(ptrSI + i) = IIRFloat(amplitude*sin(3.14159*freqSI*i/44100.0)) ;
        *(ptrSI + i+1) = IIRFloat(amplitude*sin(3.14159*freqSI*i/44100.0)) ;
    }
    
    // POUR UN DO8
    ptrDO8 = (uint16_t *)AUDIO_BUFFER_OUT + sizeDO + sizeDOd + sizeRE + sizeREd + sizeMI + sizeFA + sizeFAd + sizeSOL + sizeSOLd + sizeLA + sizeLAd + sizeSI;

    for(i=0; i<sizeDO8; i+=2) {
        *(ptrDO8 + i) = IIRFloat(amplitude*sin(3.14159*freqDO8*i/44100.0)) ;
        *(ptrDO8 + i+1) = IIRFloat(amplitude*sin(3.14159*freqDO8*i/44100.0)) ;
    }
    
    //Envoie des trames dans le codec
    BSP_AUDIO_OUT_SetAudioFrameSlot(CODEC_AUDIOFRAME_SLOT_02);
    
    /*Init transmission UART*/
    serial_read.baud(115200);//les bauds de la carte qui a été mesuré
    serial_read.format(8, SerialBase::None,1);//(nb de bit par trame, pas de parité, un seul stop bit)

    while (1) {
        

        
        datapre[0] = data[0];
        datapre[1] = data[1];
        
        if(serial_read.readable()==true){
            serial_read.read(&data, 1);

            if((data[0]==datapre[0])||(data[1]==datapre[1])){
            }
            else{
                BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
            }       
        }
            
        
        if((data[0]==0x00)&&(data[1]==0x20)){
            BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
        }
        
       if((data[0]==0x00)&&(data[1]==0x00)){
            BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
        }
        
        //Jouer le do
        if((data[0]&0x01)==0x01){

            if (BSP_AUDIO_OUT_Play((uint16_t *)ptrDO, sizeDO ) != AUDIO_OK) {
                printf("BSP_AUDIO_OUT_Play error\n");
            }
        }
        
        //Jouer le re
        if((data[0]&0x02)==0x02){

            if (BSP_AUDIO_OUT_Play((uint16_t *)ptrRE, sizeRE ) != AUDIO_OK) {
                printf("BSP_AUDIO_OUT_Play error\n");
            }
        }
        
        //Jouer le mi
        if((data[0]&0x04)==0x04){

            if (BSP_AUDIO_OUT_Play((uint16_t *)ptrMI, sizeMI ) != AUDIO_OK) {
                printf("BSP_AUDIO_OUT_Play error\n");
            }
        }
        
        //Jouer le fa
        if((data[0]&0x08)==0x08){

            if (BSP_AUDIO_OUT_Play((uint16_t *)ptrFA, sizeFA ) != AUDIO_OK) {
                printf("BSP_AUDIO_OUT_Play error\n");
            }
        }
        
        //Jouer le sol
        if((data[0]&0x10)==0x10){

            if (BSP_AUDIO_OUT_Play((uint16_t *)ptrSOL, sizeSOL ) != AUDIO_OK) {
                printf("BSP_AUDIO_OUT_Play error\n");
            }
        }
        
        //Jouer le la
        if((data[0]&0x20)==0x20){
            
            if (BSP_AUDIO_OUT_Play((uint16_t *)ptrLA, sizeLA ) != AUDIO_OK) {
                printf("BSP_AUDIO_OUT_Play error\n");
            }
        }
        
        //Jouer le si
        if((data[0]&0x40)==0x40){
            //BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
            if (BSP_AUDIO_OUT_Play((uint16_t *)ptrSI, sizeSI) != AUDIO_OK) {
                printf("BSP_AUDIO_OUT_Play error\n");
            }
        }
        
        //Jouer le do2
        if((data[0]&0x80)==0x80){

            if (BSP_AUDIO_OUT_Play((uint16_t *)ptrDO8, sizeDO8) != AUDIO_OK) {
                printf("BSP_AUDIO_OUT_Play error\n");
            }
        }
        
        //Jouer le do#
        if((data[1]&0x01)==0x01){

            if (BSP_AUDIO_OUT_Play((uint16_t *)ptrDOd, sizeDOd ) != AUDIO_OK) {
                printf("BSP_AUDIO_OUT_Play error\n");
            }
        }
        
        //Jouer le re#
        if((data[1]&0x02)==0x02){

            if (BSP_AUDIO_OUT_Play((uint16_t *)ptrREd, sizeREd ) != AUDIO_OK) {
                printf("BSP_AUDIO_OUT_Play error\n");
            }       
        }
        
        //Jouer le fa#
        if((data[1]&0x04)==0x04){

            if (BSP_AUDIO_OUT_Play((uint16_t *)ptrFAd, sizeFAd) != AUDIO_OK) {
                printf("BSP_AUDIO_OUT_Play error\n");
            }
        }
        
        //Jouer le sol#
        if((data[1]&0x08)==0x08){
            
            if (BSP_AUDIO_OUT_Play((uint16_t *)ptrSOLd, sizeSOLd ) != AUDIO_OK) {
                printf("BSP_AUDIO_OUT_Play error\n");
            }
        }  
        
        //Jouer le la#
        if((data[1]&0x10)==0x10){

            if (BSP_AUDIO_OUT_Play((uint16_t *)ptrLAd, sizeLAd) != AUDIO_OK) {
                printf("BSP_AUDIO_OUT_Play error\n");
            }
        }
        
    }
}


/*-------------------------------------------------------------------------------------
       Callbacks implementation:
           the callbacks API are defined __weak in the stm32746g_discovery_audio.c file
           and their implementation should be done in the user code if they are needed.
           Below some examples of callback implementations.
  -------------------------------------------------------------------------------------*/
/**
  * @brief Manages the DMA Transfer complete interrupt.
  * @param None
  * @retval None
  */
void BSP_AUDIO_IN_TransferComplete_CallBack(void)
{
    audio_rec_buffer_state = BUFFER_OFFSET_FULL;
}

/**
  * @brief  Manages the DMA Half Transfer complete interrupt.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_IN_HalfTransfer_CallBack(void)
{
    audio_rec_buffer_state = BUFFER_OFFSET_HALF;
}

/**
  * @brief  Audio IN Error callback function.
  * @param  None
  * @retval None
  */
  
void BSP_AUDIO_IN_Error_CallBack(void)
{
    printf("BSP_AUDIO_IN_Error_CallBack\n");
}
