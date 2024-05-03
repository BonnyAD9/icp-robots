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

    V diagramu se vyskytují i třídy z Qt které jsou nadtřídami některých našich
    tříd. U nich nejsou vypsány jejich vlastnosti a metody, a jsou u nich
    zobrazené jen relevantní nadtřídy. Jsou to třídy: `QObject`, `QWidget`,
    `QGraphicsRectItem`, `QGraphicsEllipseItem` a `QGraphicsScene`.

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

  Implementované funkcionality:
    Roboti/překážky se dají přidat přetáhnutím ze menu které se dá otevřít
    pomocí tlačítka `menu` v levém horním rohu.

    Robot/překážka se dá přesouvat tažením myší pří zmáčknutém levém tlačítku.

    Robot/překážka se dá vybrat pomocí kliknutí leveho nebo pravého tlačítka u
    myši. Vybraný oběkt je zvýrazněn žlutým obrysem.

    Robot/překážka se dá smazat po vybráním pomocí tlačítka `delete` v pravém
    horním rohu nebo zmáčknutím klávesy `Delete`. Odvybrat se dá robot/překážka
    pomocí tlačítka `deselect` v pravém horním rohu.

    Parametry robota se dají měnit v po jeho vybrání v horním menu. Změnitelné
    parametry jsou (z leva doprava):
      `type`
        Typ robota, dá se vybrat mezi moýnostmi:
          `Auto`
            Autonomní robot, jede dopředu dokud před sebou nevidí překážku v
            dané vzdálenosti, pak se otočí o daný úhel a zase pokračuje
            dopředu. Má modrou barvu.

          `Control`
            Robot který se dá ovládat uživatelem pomocí klávesnice. Má zelenou
            barvu.

          `Dummy`
            Robot který jede pořád dopředu. Má fialovou barvu.

      `speed`
        Nastavuje rychlost pohybu robota. (v pixelech za sekundu)

      `angle`
        Nastavuje aktuální otočení robota (ve stupních).

      `r. speed`
        Nastavuje rychlost otáčení robota (ve stupních za sekundu). Nejde
        nastavit u robota typu `Dummy`.

      `d. dist.`
        Nastavuje vzdálenost ve které autonomní robot detekuje překážku (v
        pixelech).

      `r. dist.`
        Nastavuje jak moc se autonomní robot otočí když detekuje překážku (ve
        stupních). Směr otáčení se nastavuje pomocí znaménka.

    Robot typu `Control` se ovládá pomocí šipek na klávesnici (dopředu,
    doprava, doleva) a aby šel ovládat tak musí být vybrán.

    Simulace se dá pozastavit/spustit pomocí tlačítka `play`/`pause` v pravém
    dolním rohu.

    Konfigurace místnosti se dá ukládatat/načíst z souboru který se napíše do
    pole v dolní části. Uložit do souboru se dá pomocí tlačítka `save` a načíst
    se dá pomocí tlačítka `load`.

  Formát souboru pro konfiguraci místnosti:
    Nejjednodušší bude ukázat na příkladu:
      room: 900x520
      obstacle: 60x60 [100, 200]
      robot: [200, 201] { speed: 0, angle: 90 }
      auto_robot: [200, 100] {
          speed: 20,
          angle: -90,
          elide_distance: 20,
          rotation_speed: 45,
          elide_rotation: 30
      }
      auto_robot: [300, 202] { speed: 40, angle: 90 }
      control_robot: [300, 300] { speed: 20, angle: 0, rotation_speed: 45 }

    V příkladu se vytvoří místnost o velikosti 900x520 pixelů a do ní se vloží
    jedna překážka, jeden robot typu `Dummy`, dva roboti typu `Auto` a jeden
    robot typu `Control`.

    `room` se může v jednom souboru vyskytovat maximálně jednou.

    `obstacle` definuje překážku. Překážka musí mít specifikovanou velikost
    (v tomto případě 60x60 pixelů) a pozici levého horního rohu ([100, 200]).

    `robot` vytváří robota typu `Dummy`. Musí mít specifkovanou pozici levého
    horního rohu svého obrysového čtverce ([200, 201]) a dále můžou být
    specifikované parametry. Parametry které nejsou specifikované budou mít
    přiřazenou výchozí hodnotu. Povolené parametry jsou: `speed` a `angle`.

    `auto` vytváří robota typu `Auto`. Podobně jako `robot` má pozici a
    parametry. Povolené parametry jsou: `speed`, `angle`, `elide_distance`,
    `rotation_speed` a `elide_rotation`.

    `auto` vytváří robota typu `Control`. Podobně jako `robot` má pozici a
    parametry. Povolené parametry jsou: `speed`, `angle` a `rotation_speed`.

