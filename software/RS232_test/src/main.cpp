#include "serial.hpp"
#include "incubator.hpp"
#include "filedummy.hpp"

#include <fmt/format.h>


//TODO: 1. Thread A -> empfängt mit getPackage -> ruft CO2Calc oder TempCalc auf
//      2. Thread B schreibt regelmäßig SendRequest (CO2 und Temp) -> eventuell in einem?
//      3. Unterscheidung zwischen CO2 und Temp Paketen anhand von Command

int main()
{
  Incubator<FileDummy<true>> inc{"trace_out"};
  while (true)
  {
    std::this_thread::sleep_for(std::chrono::seconds{1});
  }
}
