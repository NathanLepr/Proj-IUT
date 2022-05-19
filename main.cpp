#include "mbed.h"
#include "stm32746g_discovery_audio.h"
#include "stm32746g_discovery_sdram.h"


typedef enum {
    BUFFER_OFFSET_NONE = 0,
    BUFFER_OFFSET_HALF = 1,
    BUFFER_OFFSET_FULL = 2,
} BUFFER_StateTypeDef;

#define AUDIO_BLOCK_SIZE   ((uint32_t)512)
#define AUDIO_BUFFER_IN     SDRAM_DEVICE_ADDR
#define AUDIO_BUFFER_OUT   (AUDIO_BUFFER_IN + (AUDIO_BLOCK_SIZE * 2))

volatile uint32_t  audio_rec_buffer_state = BUFFER_OFFSET_NONE;
    int i, y;
    uint16_t * ptrMEM;

int main()
{
    printf("\n\nAUDIO LOOPBACK EXAMPLE START:\n");

    BSP_AUDIO_IN_OUT_Init(INPUT_DEVICE_DIGITAL_MICROPHONE_2, OUTPUT_DEVICE_HEADPHONE, DEFAULT_AUDIO_IN_FREQ, DEFAULT_AUDIO_IN_BIT_RESOLUTION, DEFAULT_AUDIO_IN_CHANNEL_NBR);
    printf("AUDIO loop from digital micro (U20 & U21 components on board) to headphone (CN10 jack connector)\n");


    /* Initialize SDRAM buffers */
    BSP_SDRAM_Init();
    memset((uint16_t *)AUDIO_BUFFER_IN, 0, AUDIO_BLOCK_SIZE * 2);
    memset((uint16_t *)AUDIO_BUFFER_OUT, 0, AUDIO_BLOCK_SIZE * 2);
    printf("SDRAM init done\n");

    //audio_rec_buffer_state = BUFFER_OFFSET_NONE;

    /* Start Recording 
    if (BSP_AUDIO_IN_Record((uint16_t *)AUDIO_BUFFER_IN, AUDIO_BLOCK_SIZE) != AUDIO_OK) {
        printf("BSP_AUDIO_IN_Record error\n");
    }*/

    /* Start Playback */
    BSP_AUDIO_OUT_SetAudioFrameSlot(CODEC_AUDIOFRAME_SLOT_02);
    if (BSP_AUDIO_OUT_Play((uint16_t *)AUDIO_BUFFER_OUT, AUDIO_BLOCK_SIZE * 2) != AUDIO_OK) {
        printf("BSP_AUDIO_OUT_Play error\n");
    }

    while (1) {
        
        ptrMEM = (uint16_t *)SDRAM_DEVICE_ADDR;
        
        //écriture première moitié de buffer IN
        
        for (i=0;i<AUDIO_BLOCK_SIZE;i++)
        {  
            if(y<50){
                *(ptrMEM + i)= 1000;
                y++;
            }
            else if(y>50&&y<100){
                *(ptrMEM + i)= 0;
                y++;
            }
            else if(y>=100){
                y=0;
            }
        }
        
        /* Wait end of half block recording 
        while (audio_rec_buffer_state == BUFFER_OFFSET_HALF) {
        }
        audio_rec_buffer_state = BUFFER_OFFSET_NONE;*/

        /* Copy de la première moitié du buffer IN dans seconde moitié Buffer OUT */
        memcpy((uint16_t *)(AUDIO_BUFFER_OUT), (uint16_t *)(AUDIO_BUFFER_IN), AUDIO_BLOCK_SIZE);

        /* Wait end of one block recording 
        while (audio_rec_buffer_state == BUFFER_OFFSET_FULL) {
        }
        audio_rec_buffer_state = BUFFER_OFFSET_NONE;*/
        
        ptrMEM = (uint16_t *)SDRAM_DEVICE_ADDR + AUDIO_BLOCK_SIZE;
        
        //écriture seconde moitié de buffer IN
        for (i=0;i<AUDIO_BLOCK_SIZE;i++)
        {  
            if(y<50){
                *(ptrMEM + (AUDIO_BLOCK_SIZE) + i)= 1000;
                y++;
            }
            else if(y>50&&y<100){
                *(ptrMEM +(AUDIO_BLOCK_SIZE) + i)= 0;
                y++;
            }
            else if(y>=100){
                y=0;
            }
        }

        /* Copy de la seconde moitié du buffer IN dans seconde moitié Buffer OUT */
        memcpy((uint16_t *)(AUDIO_BUFFER_OUT + (AUDIO_BLOCK_SIZE)), (uint16_t *)(AUDIO_BUFFER_IN + (AUDIO_BLOCK_SIZE)), AUDIO_BLOCK_SIZE);
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
