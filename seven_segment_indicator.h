#pragma once

#include "pinlist.h"

/// управление семимегментными индикаторами
/// A_...H_ - классы выводов сегментов, чтобы зажечь подать ноль 
/// Ks - классы выводов, управляющие катодами. для выбора подать 1
template<
   class A_, class B_, class C_, class D_, class E_, class F_, class G_, class H_,
   class ... Ks_
>
class SSI // seven-segment indicator 
{
public:
   static constexpr uint32_t indicatorQty = sizeof...(Ks_);
   uint8_t buffer [indicatorQty];
   bool    point  [indicatorQty];


   SSI (Timer& timer, uint32_t refreshTime = 2_ms)
      : buffer      {0},
        index       (0),
        timer       (timer)
   {
      // CONFIGURE_PIN (A_, OutputHighSpeed);
      // CONFIGURE_PIN (B_, OutputHighSpeed);
      // CONFIGURE_PIN (C_, OutputHighSpeed);
      // CONFIGURE_PIN (D_, OutputHighSpeed);
      // CONFIGURE_PIN (E_, OutputHighSpeed);
      // CONFIGURE_PIN (F_, OutputHighSpeed);
      // CONFIGURE_PIN (G_, OutputHighSpeed);
      // CONFIGURE_PIN (H_, OutputHighSpeed);
      timer.setTimeAndStart (refreshTime);
   }


   /// переключает индикаторы
   void operator ()();



private:
   uint8_t index;
   Timer& timer;

   using indicators = PinList<Ks_...>;
   using segments = PinList<A_,B_,C_,D_,E_,F_,G_,H_>;
};





static constexpr uint8_t symbols[] = {
   //ABCDEFGH
   0b00000011, // 0   ==A==
   0b10011111, // 1  ||   ||
   0b00100101, // 2  F|   |B
   0b00001101, // 3  ||   ||
   0b10011001, // 4   ==G==
   0b01001001, // 5  ||   ||
   0b01000001, // 6  E|   |C
   0b00011111, // 7  ||   ||
   0b00000001, // 8   ==D==  (H)
   0b00001001, // 9
   0b10010001, // 10 H
   0b11100011, // 11 L
   0b11111111, // 12 Space
   0b11111101  // 13 -
};






template<class A_, class B_, class C_, class D_, class E_, class F_, class G_, class H_,class ... Ks_>
void SSI<A_,B_,C_,D_,E_,F_,G_,H_,Ks_...>::operator ()()
{
   if (timer.event()) {
      indicators::Write(0);
      ++index;
      if (index == indicatorQty)
         index = 0;
      uint8_t symbol = symbols[buffer[index]];
      if (point[index])
         symbol &= ~1u;
      segments::Write(symbol);
      indicators::Write(1u << index);
   }
}