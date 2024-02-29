/*=================================================================================*/
/*  Program     : Smart Home AVR Project                                           */
/*  Description : This Code For Slave MCU to Control the Home                      */
/*  Author      : Osama Shalan & Ebraam                                            */
/*  Date        : 30/11/2023                                                       */
                                           
/*=================================================================================*/
/* Include Header Files From LIB */
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"
/* Include Header Files From MCAL Layer */
#include "../MCAL/DIO/DIO_Interface.h"
#include "../MCAL/ADC/ADC_Interface.h"
#include"../MCAL/TIMERS/TIMER1/TIMER1_Interface.h"
#include"../MCAL/GIE/GIE_Interface.h"
#include"../MCAL/USART/USART_Interface.h"
/* Include Header Files From HAL Layer */
#include "../HAL/LED/LED_Interface.h"
#include "../HAL/LCD/LCD_Interface.h"
#include"../HAL/SERVO_MOTOR/SERVO_Interface.h"
#define F_CPU 8000000UL
#include <util/delay.h>
/****** Functions Prototypes ******/
void StartProgram_SMART_HOME (void);
void OpenLight1 			 (void);
void OpenLight2 			 (void);
void OpenLight3 			 (void);
void OpenLight4 			 (void);
void CloseLight1             (void);
void CloseLight2             (void);
void CloseLight3             (void);
void CloseLight4             (void);
void OpenFan 				 (void);
void CloseFan 				 (void);
void OpenWindow 			 (void);
void CloseWindow 			 (void);
void OpenTV 				 (void);
void CloseTV 				 (void);
//void OpenPump 			 (void);
//void ClosePump 			 (void);
void UART_CallBack 		  (u8 Data);
/******** Global Variables ********/
u8 ExitAutoData = 0 ;
int main()
{
	/*===========================================*/
	/*=========   APP Initializations   =========*/
	TIMER1_voidInit ();
	USART_voidInit ();
	LCD_voidInit ();
	ADC_voidInit ();
	DIO_voidSetPinDirection (PORTA,PIN0,OUTPUT);
	DIO_voidSetPinDirection (PORTB,PIN0,OUTPUT);
	DIO_voidSetPinDirection (PORTB,PIN3,OUTPUT);
	DIO_voidSetPinDirection (PORTB,PIN4,OUTPUT);
	DIO_voidSetPinDirection (PORTB,PIN5,OUTPUT);
	DIO_voidSetPinDirection (PORTB,PIN6,OUTPUT);
	DIO_voidSetPinDirection (PORTB,PIN7,OUTPUT);
	DIO_voidSetPinDirection(PORTD,PIN6,OUTPUT);   // Buzzer OUTPUT
	DIO_voidSetPinValue(PORTD,PIN6,LOW);		  // Buzzer OFF
	/*===========================================*/
	/*=========   APP Local Variables   =========*/
	u8 Access = 0;
	u8 ActuatorData = 0 ;
	u8 TVData = 0 ;
	u8 ModeData = 0 ;
	u8 LightData = 0 ;
	u8 FanData = 0 ;
	//u8 PumpData = 0 ;
	u16 Digital_Signal_Temp=0;
	u16 Digital_Signal_LDR=0;
	u16 Digital_Signal_Smoke=0;
	u16 Analog_Signal_Temp=0;
	u16 Analog_Signal_LDR=0;
	u16 Analog_Signal_Smoke=0;
	u16 Temp_Value=0;
	u16 Smoke_Value= 50 ;
	/*===========================================*/
	/*=========        APP Start        =========*/
	StartProgram_SMART_HOME();					  // MY LCD Start Program
	USART_voidSendData('x');
	USART_voidRecieveData (&Access);			  // Recive Data From UART Master AVR To Open Access to My Home
	if( Access == 'Z' )
	{
		LCD_voidClearLine(0);
		LCD_voidClearLine(1);
		LCD_voidGoTo_XY ( 0 , 0 );
		LCD_voidWriteString ("   Welome To ");
		LCD_voidGoTo_XY (1,0);
		LCD_voidWriteString ("  Your home :)");
		while(1)
		{
			USART_voidRecieveData (&ModeData);
			if( ModeData == 'A' )
			{
				LCD_voidClearLine(0);
				LCD_voidClearLine(1);
				LCD_voidGoTo_XY ( 0 , 0 );
				LCD_voidWriteString (" Automatic Mode ");
				_delay_ms(800);
				LCD_voidClearLine(0);
				LCD_voidClearLine(1);
				LCD_voidGoTo_XY ( 0 , 8 );
				LCD_voidWriteChar ('/');
				LCD_voidGoTo_XY ( 1 , 8 );
				LCD_voidWriteChar ('/');
				GIE_void_GI_Enable(ON);
				while(1)
				{
					//USART_voidAsyncRecieveDataCallBack (&UART_CallBack);	// Async UART function for Automatic Mode Only To avoid CPU Stuck
					if( ExitAutoData == '#')
					{
						ModeData = 0 ;
						ExitAutoData = 0 ;
						CloseLight1 ();
						CloseLight2 ();
						CloseLight3 ();
						CloseLight4 ();
						//ClosePump();
						CloseTV();
						CloseFan();
						LCD_voidClearLine(0);
						LCD_voidClearLine(1);
						GIE_void_GI_Enable(OFF);
						break ;
					}
						/*---------------------------------------------------------------------------------------------------------------------*/
						/* Air Condition System  */
						Digital_Signal_Temp =  ADC_u16ReadSynchronus ( ADC0 ) ;
						Analog_Signal_Temp = (Digital_Signal_Temp * 5000UL ) /1024 ;   // 5000 mv   to read Analog_Signal by mv.
						Temp_Value = Analog_Signal_Temp / 10 ;
						if (Temp_Value > 25)
						{	OpenFan();
							CloseWindow();	}
						else
						{	CloseFan();
							OpenWindow();	}
						/*---------------------------------------------------------------------------------------------------------------------*/
						/* Light System */
						/*Digital_Signal_LDR =  ADC_u16ReadSynchronus ( ADC1 ) ;
						Analog_Signal_LDR = (Digital_Signal_LDR * 5000UL ) /1024 ;   // 5000 mv   to read Analog_Signal by mv.
						if (Analog_Signal_LDR > 2400)
						{	OpenLight();	}
						else
						{	CloseLight();	}
						/*---------------------------------------------------------------------------------------------------------------------*/
						/* Fire Alarm and Fire Fitting System */
						/*Digital_Signal_Smoke =  ADC_u16ReadSynchronus ( ADC2 ) ;
						Analog_Signal_Smoke =  ( (Digital_Signal_Smoke * 114UL) / 1023 ) + 9 ;
						if ( Analog_Signal_Smoke > Smoke_Value )
						{
							while(Analog_Signal_Smoke > Smoke_Value)
							{
								Digital_Signal_Smoke =  ADC_u16ReadSynchronus ( ADC2 ) ;
								Analog_Signal_Smoke =  ( (Digital_Signal_Smoke * 114UL) / 1023 ) + 9 ;
								LCD_voidGoTo_XY ( 0 , 0 );
								LCD_voidWriteString (" Fire!!  Fire!! ");
								LCD_voidGoTo_XY ( 1 , 0 );
								LCD_voidWriteString (" Fire!!  Fire!! ");
								OpenPump();
							}
							LCD_voidClearLine(0);
							LCD_voidClearLine(1);
							LCD_voidGoTo_XY ( 0 , 8 );
							LCD_voidWriteChar ('/');
							LCD_voidGoTo_XY ( 1 , 8 );
							LCD_voidWriteChar ('/');
						}
						else
						{	ClosePump();	}
						/*---------------------------------------------------------------------------------------------------------------------*/
						/* TV System */
						/*OpenTV();
						if ( TVData == '1' )
						{	OpenTV();	}
						else if ( TVData == '0' )
						{	CloseTV();	}
						/*---------------------------------------------------------------------------------------------------------------------*/
				}
			}
			else if ( ModeData == 'M')
			{
				LCD_voidClearLine(0);
				LCD_voidClearLine(1);
				LCD_voidGoTo_XY ( 0 , 0 );
				LCD_voidWriteString (" Manual Mode.. ");
				_delay_ms(800);
				LCD_voidClearLine(0);
				LCD_voidClearLine(1);
				LCD_voidGoTo_XY ( 0 , 8 );
				LCD_voidWriteChar ('/');
				LCD_voidGoTo_XY ( 1 , 8 );
				LCD_voidWriteChar ('/');
				while(1)
				{
					USART_voidRecieveData (&ActuatorData);
				/*---------------------------------------------------------------------------------------------------------------------*/
					/* Control TV */
					if ( ActuatorData == 'T')
					{
						USART_voidRecieveData (&TVData);
						if ( TVData == '1' )
						{	OpenTV();	}
						else if ( TVData == '0' )
						{	CloseTV();	}
					}
				/*---------------------------------------------------------------------------------------------------------------------*/
					/* Control Light Bedroom */
					else if ( ActuatorData == 'L')
					{
						USART_voidRecieveData (&LightData);
						if ( LightData == '1' )
						{	OpenLight1();	}
						else if ( LightData == '0' )
						{	CloseLight1();	}
					}
				/*---------------------------------------------------------------------------------------------------------------------*/
				/* Control Light Kitchen */
					else if ( ActuatorData == 'C')
					{
						USART_voidRecieveData (&LightData);
						if ( LightData == '1' )
						{	OpenLight2();	}
						else if ( LightData == '0' )
						{	CloseLight2();	}
					}
				/*---------------------------------------------------------------------------------------------------------------------*/
				/* Control Light Bathroom */
					else if ( ActuatorData == 'B')
					{
						USART_voidRecieveData (&LightData);
						if ( LightData == '1' )
						{	OpenLight3();	}
						else if ( LightData == '0' )
						{	CloseLight3();	}
					}
				/*---------------------------------------------------------------------------------------------------------------------*/
				/* Control Light Living room */
					else if ( ActuatorData == 'K')
					{
						USART_voidRecieveData (&LightData);
						if ( LightData == '1' )
						{	OpenLight4();	}
						else if ( LightData == '0' )
						{	CloseLight4();	}
					}
				/*---------------------------------------------------------------------------------------------------------------------*/
					/* Control Fan */
					else if ( ActuatorData == 'F')
					{
						USART_voidRecieveData (&FanData);
						if ( FanData == '1' )
						{	OpenFan();
							CloseWindow();	}
						else if ( FanData == '0' )
						{	CloseFan();
							OpenWindow();	}
					}
				/*---------------------------------------------------------------------------------------------------------------------*/
					/* Control Water Pump */
					/*else if ( ActuatorData == 'P')
					{
						USART_voidRecieveData (&PumpData);
						if ( PumpData == '1' )
						{
							OpenPump();
						}
						else if ( PumpData == '0' )
						{	ClosePump();		}
					}
				/*---------------------------------------------------------------------------------------------------------------------*/
					/* Exit from Manual Mode */
					else if ( ActuatorData == '#')
					{
						ModeData = 0 ;
						ActuatorData = 0 ;
						CloseLight1 ();
						CloseLight2 ();
						CloseLight3 ();
						CloseLight4 ();
						//ClosePump();
						CloseTV();
						CloseFan();
						break;
					}
				}
			}
		}
	}
	else if ( Access == 'N')
	{
		LCD_voidClearLine(0);
		LCD_voidClearLine(1);
		LCD_voidGoTo_XY (0,0);
		LCD_voidWriteString ("You are thief!!");
		LCD_voidGoTo_XY (1,0);
		LCD_voidWriteString ("Restart System..");
	}
}
/*=============================================================================*/
void StartProgram_SMART_HOME (void)
{
	LCD_voidGoTo_XY (0,0);
	LCD_voidWriteString ("Osama / Ebraam");
	LCD_voidGoTo_XY (1,3);
	LCD_voidWriteString ("Our Project");
	_delay_ms(700);
	LCD_voidClearLine(0);
	LCD_voidClearLine(1);
	LCD_voidGoTo_XY (0,1);
	LCD_voidWriteString (" Welcome To MY");
	LCD_voidGoTo_XY (1,0);
	LCD_voidWriteString ("  Smart Home :) ");
}
/*=============================================================================*/
void UART_CallBack (u8 Data)
{
	ExitAutoData = Data ;
}
/*=============================================================================*/
void OpenLight1 (void)
{
	LCD_voidGoTo_XY ( 0 , 0 );
	LED_voidStatus_Led_Pin (PORTB,PIN4,ON);
	LCD_voidWriteString ("1-ON");
}
/*=============================================================================*/
void CloseLight1 (void)
{
	LCD_voidGoTo_XY ( 0 , 0 );
	LED_voidStatus_Led_Pin (PORTB,PIN4,OFF);
	LCD_voidWriteString ("1-OFF");
}
/*=============================================================================*/
void OpenLight2 (void)
{
	LCD_voidGoTo_XY ( 0 , 4 );
	LED_voidStatus_Led_Pin (PORTB,PIN5,ON);
	LCD_voidWriteString ("2-ON");
}
/*=============================================================================*/
void CloseLight2 (void)
{
	LCD_voidGoTo_XY ( 0 , 4 );
	LED_voidStatus_Led_Pin (PORTB,PIN5,OFF);
	LCD_voidWriteString ("2-OFF");
}
/*=============================================================================*/
void OpenLight3 (void)
{
	LCD_voidGoTo_XY ( 1 , 0 );
	LED_voidStatus_Led_Pin (PORTB,PIN6,ON);
	LCD_voidWriteString ("3-ON");
}
/*=============================================================================*/
void CloseLight3 (void)
{
	LCD_voidGoTo_XY ( 1 , 0 );
	LED_voidStatus_Led_Pin (PORTB,PIN6,OFF);
	LCD_voidWriteString ("3-OFF");
}
/*=============================================================================*/
void OpenLight4 (void)
{
	LCD_voidGoTo_XY ( 1 , 4 );
	LED_voidStatus_Led_Pin (PORTB,PIN7,ON);
	LCD_voidWriteString ("4-ON");
}
/*=============================================================================*/
void CloseLight4 (void)
{
	LCD_voidGoTo_XY ( 1 , 4 );
	LED_voidStatus_Led_Pin (PORTB,PIN7,OFF);
	LCD_voidWriteString ("4-OFF");
}
/*=============================================================================*/
void OpenFan (void)
{
	LCD_voidGoTo_XY ( 0 , 10 );
	DIO_voidSetPinValue (PORTB,PIN0,HIGH);
	LCD_voidWriteString ("Fan-ON");
}
/*=============================================================================*/
void CloseFan (void)
{
	LCD_voidGoTo_XY ( 0 , 10 );
	DIO_voidSetPinValue (PORTB,PIN0,LOW);
	LCD_voidWriteString ("FanOFF");
}
/*=============================================================================*/
void OpenWindow (void)
{
		SERVO_voidRotateSpecificAngle_OC1A ( 180 );
}
/*=============================================================================*/
void CloseWindow (void)
{
		SERVO_voidRotateSpecificAngle_OC1A ( 0 );
}
/*=============================================================================*/
void OpenTV (void)
{
	DIO_voidSetPinValue (PORTB,PIN3,HIGH);
	LCD_voidGoTo_XY ( 1 , 10 );
	LCD_voidWriteString ("TV-ON");
	LCD_voidClearGrid(1,15);
}
/*=============================================================================*/
void CloseTV (void)
{
	DIO_voidSetPinValue (PORTB,PIN3,LOW);
	LCD_voidGoTo_XY ( 1 , 10 );
	LCD_voidWriteString ("TV-OFF");

}
/*=============================================================================*/
/*void OpenPump (void)
{

	DIO_voidSetPinValue (PORTB,PIN2,HIGH);
	DIO_voidSetPinValue(PORTD,PIN6,HIGH);		// Buzzer ON
}
/*void ClosePump (void)
{
	LCD_voidGoTo_XY ( 0 , 0 );
	DIO_voidSetPinValue (PORTB,PIN2,LOW);
	LCD_voidWriteString ("PumpOFF");
	DIO_voidSetPinValue(PORTD,PIN6,LOW);		// Buzzer OFF
}
/*=============================================================================*/

