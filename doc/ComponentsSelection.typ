#align(center)[
  #table(
    columns: 2,
    align: left,
    [*Imię i nazwisko:*], [Marcel Lis],
    [*Numer indeksu:*], [310296],
    [*Nazwa Projektu:*], [MedBox]
  )
]

= Electronic Components Selection

#table(
  columns: (1fr, 3fr),
  align: left,
  table.header(
    [*Component Name*], [*Selected Model*], [*Reason for Selection*]
  ),
  [Microcontroller (MCU)], [ESP-12S], [Describe why this specific MCU was chosen (e.g., performance, low power, features).],
  [I2C GPIO Expander], [PCA9555], [Explain the need for additional I/O pins and why I2C was suitable.],
  [Real-Time Clock (RTC)], [DS3231], [Explain why precise timekeeping is required for the MedBox project.],
  [Power Supply / Battery], [Li-Ion Battery 1000mAh], [Include details about voltage requirements, capacity, and form factor.],
  [USB Interface], [CH340G], [Describe communication needs or power delivery configuration.]
)
