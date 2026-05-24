# KARNEL

Ilmselt kõige kiirem ja lihtsam lahendus kõikidele pomodoro timeri nõuetele, kerneli näol.

Punane -> aeg keskendu asjadele (25 min) / näite jaoks 10 sec

Roheline -> aeg puhata (5 min) / näite jaoks 2 sec

Oleku vahetamisel peab vajutama <kbd>u</kbd> tahte, et taimerit taas alustada. Mistahes hetkel võib ka panna taimeri 
pausile <kbd>u</kbd> tähega.

## Käivitamine

Vaadake käivitamise kohta lähemalt [siit](limine-cxx-templateREADME.md)


## Karneli moodulid
| Fail                                    | Eesmärk                                                                                           |
|-----------------------------------------|---------------------------------------------------------------------------------------------------|
| `src/gdt.cpp` / `inc/gdt.hpp`           | Globaalse Deskriptori Tabeli häälestus                                                            |
| `src/idt.cpp` / `inc/idt.hpp`           | Katkestuse Desckiptori Tabeli + ISR makett                                                        |
| `inc/io.hpp`                            | Port I/O abistajad (`inb`/`outb`)                                                                 |
| `src/main.cpp`                          | Karneli alguspunkt; Framebufferi ja katkestuste sätestamine                                       |
| `src/memory.cpp` / `inc/memory.hpp`     | Mälu funktsioonid                                                                                 |
| `src/pic.cpp` / `inc/pic.hpp`           | 8259 PIC [algväärtustus](https://akit.cyber.ee/term/4312-algvaartustus) ja APICi desaktiveerimine |
| `src/pit.cpp` / `inc/pit.hpp`           | PIT taimer, `pit_sleep_ms()`                                                                      |
| `src/pomodoro.cpp` / `inc/pomodoro.hpp` | Pomodoro [talitlusloogikakiht](https://akit.cyber.ee/term/3733-business-logic-layer)              |
| `src/renderer.cpp` / `inc/renderer.hpp` | Millegi kuvamiseks päriselt                                                                       |
| `src/ps2.cpp` / `inc/ps2.hpp`           | PS/2 klaviatuuri seadistus ja lugemine                                                            | 
| `inc/ssfn.hpp`                          | SSFN2 fondi [ilmestaja](https://akit.cyber.ee/term/16742-rendering-1)                             |
| `src/string.cpp` / `inc/string.hpp`     | Lihtne String klassi implementatsioon                                                             |