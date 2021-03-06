#pragma once

#include "RCC.h"
#include "DebugVar.h"
#include "timers.h"
#include "flash.h"
#include "pin.h"
#include "spi.h"
#include "buzzer.h"
#include "literals.h"
#include "buttons.h"
#include "inputCounter.h"
#include "seven_segment_indicator.h"
#include "modbusSlave.h"
#include "adc.h"


Timer ledTimer;
Timer spiTimer;

using BlueLed = PC8;

// семисегментный индикатор
SSI<PD0,PD1,PD2,PD4,PD5,PD6,PD7,PC0,PC1,PC2> ssi;


// энергонезависимые данные
struct FlashData {
   uint16_t d1 {1};
   uint16_t d2 {3};
};
Flash<FlashData, FLASH::Sector::_10> flash {};


struct SPIdata {
   uint16_t time;
   uint16_t currentTemperature;
   uint16_t targetTemperature;
};
SPIoverDMA<SPI1, SPIdata> spi;

// шим зуммера
using PWMout = PA6;
using PWMtimer = TIM3;
using PWM_ = PWM<PWMtimer, PWMout>;
PWM_ pwm;

// зуммер
auto buzzer = Buzzer<PWM_> (pwm, 4000_Hz); 


using But1 = PA0;
using But2 = PA1;
using But3 = PA2;
Buttons<false,But1,But2,But3> buttons;


using GreenLed = PC9;

// частотометр
InputCounter<TIM1, PA9> counter;

// уарт модбаса
using RXpin = PA3;
using TXpin = PA2;
using RTSpin = PA1;
using LEDpin = BlueLed;
const uint8_t bufSize = 30;
using USART_ = USART<USART1, bufSize, RXpin, TXpin, RTSpin, LEDpin>;
USART_ uart;

// модбас
struct InRegs {
   uint16_t reg0;
   uint16_t reg1;
};
struct OutRegs {
   uint16_t reg0;
   uint16_t reg1;
};
auto modbus = MBslave<InRegs, OutRegs, USART_> (uart);
// действия на входные регистры модбаса
#define ADR(reg)    GET_ADR(InRegs, reg)
inline void reaction (uint16_t regAdr)
{
   switch ( regAdr ) {
      case ADR(reg0):
         ; // сделать чтото
         break;
      case ADR(reg1):
         ; // сделать чтото
         break;
      default: ;
   }
}


// 
constexpr uint16_t bufferSize = 256;
ADCaverageSized<bufferSize, ADC1, PA0> current;


// эта функция вызывается первой в startup файле
extern "C" void CLKinit (void)
{
   // FLASH->ACR |= FLASH_ACR_PRFTBE;
   FLASH::set (FLASH::Latency::_1);

   RCC::setAHBprescaler (RCC_ral::AHBprescaler::AHBnotdiv);
   RCC::setAPBprecsaler (RCC_ral::APBprescaler::APBnotdiv);
   RCC::systemClockSwitch (RCC_ral::SystemClockSwitch::CS_PLL);
   RCC::setPLLsource (RCC_ral::PLLsource::HSIdiv2);
   RCC::setPLLmultiplier (RCC_ral::PLLmultiplier::_12);
   RCC::PLLon();
   RCC::waitPLLready();
}
