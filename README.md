# KARNEL

Ilmselt kõige kiirem ja lihtsam lahendus kõikidele pomodoro timeri nõuetele, kerneli näol.

Punane -> aeg keskendu asjadele (20 min) / näite jaoks 10 sec

Sinakas-lilla -> aeg puhata (5 min) / näite jaoks 2 sec

## Käivitamine

Vaadake käivitamise kohta lähemalt [siit](limine-cxx-templateREADME.md)


## Karneli moodulid
| Fail                                    | Eesmärk                                                                                           |
|-----------------------------------------|---------------------------------------------------------------------------------------------------|
| `src/main.cpp`                          | Karneli alguspunkt; Framebufferi ja katkestuste sätestamine                                       |
| `src/gdt.cpp` / `inc/gdt.hpp`           | Globaalse Deskriptori Tabeli häälestus                                                            |
| `src/idt.cpp` / `inc/idt.hpp`           | Katkestuse Desckiptori Tabeli + ISR makett                                                        |
| `src/pic.cpp` / `inc/pic.hpp`           | 8259 PIC [algväärtustus](https://akit.cyber.ee/term/4312-algvaartustus) ja APICi desaktiveerimine |
| `src/pit.cpp` / `inc/pit.hpp`           | PIT taimer, `pit_sleep_ms()`                                                                      |
| `src/memory.cpp` / `inc/memory.hpp`     | Mälu funktsioonid                                                                                 |
| `inc/io.hpp`                            | Port I/O abistajad (`inb`/`outb`)                                                                 |
| `inc/ssfn.hpp`                          | SSFN2 fondi [ilmestaja](https://akit.cyber.ee/term/16742-rendering-1)                             |
| `src/pomodoro.cpp` / `inc/pomodoro.hpp` | Pomodoro [talitlusloogikakiht](https://akit.cyber.ee/term/3733-business-logic-layer)              |
