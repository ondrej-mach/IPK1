# HTTP Server
Tento projekt je minimalistická implementace HTTP serveru v C++. 
Implementace je rozdělena na knihovnu server a zbytek programu. 

### Knihovna server
Knihovna poskytuje třídu Server a metody pro jeho použití. 
Tato třída má veřejné metody init, end a handleConnection, které využívají socket API pro komunikaci.
Server má vlastní implementaci protokolu HTTP. 
Tato implementace je velmi jednoduchá a omezená, podporuje pouze metodu GET.
Parsování URL je také zjednodušené, server se zajímá pouze o pathname.
Poté podle cesty vybere odpovídající záznam ze seznamu Entries a vyvolá jeho funkci.
Návrat této funkce se stane tělem HTTP odpovědi.

Díky seznamu Entries je knihovna snadná na použití, stačí stanovit cesty (např. /hostname) a přiřadit jim funkce (getHostname).
Pokud klient požaduje cestu, která nemá žádný záznam, server vrací 404 Not Found.
V případě použití jiné metody než GET server vrací 405 Method Not Allowed.

### Hlavní tělo programu
Ve funkci main jsou načteny argumenty programu.
Poté je inicializován server a je spuštěn v nekonečné smyčce.
Všechny funkce, které geneují odesílaný obsah jsou v modulu info.

Funkce getHostname využívá POSIX funkce gethostname.

Funkce getCpuInfo přečte soubor /proc/cpuinfo. 
V něm najde řádek nadepsaný 'model name' a z něj přečte modelová jméno procesoru.

Funkce getCpuUsage je po technické stránce nejzajímavější.
Pracuje se souborem /proc/stat, aby získala současné využití procesoru.
Soubor je přečten celkem dvakrát, v intervalu 0.2s.
Funkce odečte kolik času procesor odpracoval a vydělí ho celkovým uběhlým časem.
Tím je získáno využití procesoru v procentech, které je výstupem funkce.

## Sestavení a spuštění
Program je sestaven příkazem make v kořenové složce.
Po překladu a linkování vznikne spustitelný soubor hinfosvc.
Všechny objektové soubory a spustitelný prokra lze najednou smazat příkazem make clean.

Program hinfosvc bere při spuštění jeden poziční argument, který určuje číslo portu.
Příkaz `./hinfosvc 8080` spustí server na portu 8080.

## Příklady použití
Server jako tento může mít spoutu využití, proto uvedu pouze pár.

Program lze využít pro monitorování vzdáleného počítače.
Sledované metriky si můžeme do serveru snadno dopsat.

Server se hodí pro spouštění CGI skriptů.

Po drobném rozšíření by server mohl procházet souborový systém v počítači a odesílat html soubory.
Tak by šlo zprovoznit jednoduché statické webové stránky.
