icp-robots
  Autoři:
    Martin Slezák (xsleza26)
    Jakub Antonín Štigler (xstigl00)

  Obecné informace:
    Pro grafické rozhraní jsme využili knihovnu Qt. Aplikaci jsme vyvijeli ve
    verzi Qt6, ale je kompatibilbí i s verzí Qt5.

    Aktuální konfigurace cmake v `src/CMakeLists.txt` je nastavená pro
    kompilaci s knihovnou Qt5.

    Pro sestavování využíváme cmake. Všechny soubory v adresáři `src` jsou
    psané ručně, generované soubory se generují automaticky v adresáři `build`.

  Diagram tříd:
    Je v souboru `class-diagram.pdf`. V diagramu nejsou uvedené metody nebo
    vlastnosti tříd které nejsou důležité pro demonstraci struktury programu.
    Všechny uvedené vlastnosti tříd jsou zpřístupněné pomocí metod, a proto se
    i na některých místech oběvuje přepisování poděděných vlastností. Některé
    vlastnosti jsou zpřístupněny jen pro čtení.

  Instrukce ke kompilaci/spuštění:
    Ke kompilaci je potřeba mít nainstalovaný make, cmake a knihovnu Qt5.

    Příkazy make:
      `make` nebo `make build`
        Zkompiluje kód do adresáře `build`. Výsledná spustitelná aplikace je
        `build/icp-robots`.

      `make run`
        Zkompiluje kód stejně jako `make build` a spustí aplikaci.

      `make doxygen`
        Vygeneruje html dokumentaci do adresáře `doc/html`.

      `make pack`
        Zabalí celý projekt do souboru.

      `make clean`
        Smaže všechny soubory generované pomocí make příkazů.
