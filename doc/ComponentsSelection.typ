#align(center)[
  #table(
    columns: 2,
    align: left,
    [*Imię i nazwisko:*], [Marcel Lis],
    [*Numer indeksu:*], [310296],
    [*Nazwa Projektu:*], [MedBox]
  )
]

= Dobór komponentów elektronicznych

#table(
  columns: (1fr, 1fr, 3fr),
  align: left,
  table.header(
    [*Nazwa komponentu*], [*Wybrany model*], [*Uzasadnienie wyboru*]
  ),
  [Mikrokontroler (MCU)], [ESP8266/ESP-12S], [Podstawowym argumentem wybrou tego mikrokontrolera było to, że mam go dostępnego od ręki. Po głębszej analizie i dostosownaniu dalszych elementów jak expander I2C wszystkie wyrpowadzanie GPIO są zajęte z potencjalnym zapasem. Dodaktkowym argumentem było zintegrowany w nim moduł Wi-Fi dzięki któremu możliwe jest wprowadzenie większej liczby funkcjonalności w projekcie.],
  [I2C GPIO Expander], [MCP23017-E/SO], [Wybór na ten układ padł ze względu na jego popularność, dostępność oraz łatwość integracji z mikrokontrolerem ESP8266. Układ ten oferuje 16 dodatkowych pinów GPIO, co znacząco redukuje zapotrzebowanie na układy scalone na płytce.],
  [Real-Time Clock (RTC)], [DS3231SN+TR], [Dodatkowym elementem, który został dodany do projektu jest zegar czasu rzeczywistego. Wybór padł na ten układ ze względu na kompromis między ceną a jego funkcjonalnościami. Przede wszystkim układ ten mimo że jest w obudowie SO16 to posiada on wbudowany rezonator kwarcowy znacząaco upraszczając romzmieszczenie elementów na płyce.],
  [Zasilanie], [USBC-C], [Trzymając się obecnie panujących standardów, zdecydowałem się na zasilanie projektu za pomocą portu USB-C. Ten wybór zapewnia szeroką kompatybilność z różnymi źródłami zasilania, a także umożliwia łatwe ładowanie i zasilanie urządzenia. Dodatkowo, USB-C oferuje możliwość przesyłania danych, co może być przydatne w przyszłości, jeśli zdecyduję się na dodanie funkcji komunikacji przewodowej.],
  [LDO], [TPS73733DCQ], [Wybierając ten stabilizator napięcia, kierowałem się jego wydajnością, niskim spadkiem napięcia (low dropout) oraz dostępnością. TPS73733DCQ jest w stanie dostarczyć stabilne napięcie 3.3V, co jest idealne dla mojego mikrokontrolera i innych komponentów. Dodatkowo, ten stabilizator charakteryzuje się niskim prądem quiescent, co przyczynia się do oszczędności energii w projekcie.],
)
