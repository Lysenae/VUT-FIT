INSERT INTO Mistnost(roomID, oddeleni, lokace, sirka, delka, vyska) VALUES (?, ?, ?, ?, ?, ?);
INSERT INTO Ucebna(roomID, mist_k_sezeni, pocet_rad, mist_na_radu, bloky) VALUES (?, ?, ?, ?, ?);
INSERT INTO Kancelar(roomID, el_zasuvky, eth_zasuvky) VALUES (?, ?, ?);
INSERT INTO Porucha(poruchaID, k_oprave, technik, oznamujici, popis, cas_oznam, zprava_opravy) VALUES (?, ?, ?, ?, ?, ?, ?);
INSERT INTO Presun(presunID, zarizeni, zadatel, duvod, provedeno, cas_zadosti, odkud, kam) VALUES (?, ?, ?, ?, ?, ?, ?, ?);
INSERT INTO Prava(pravaID, popis) VALUES (?, ?);
INSERT INTO Zarizeni(zarID, umisteno, zodpovida, znacka, model, cena, specifikace, stav) VALUES (?, ?, ?, ?, ?, ?, ?, ?);
INSERT INTO Oddeleni(oddID, nazev, popis) VALUES (?, ?, ?);
INSERT INTO Zamestnanec(zamID, rodnecislo, pracoviste, prava, login, jmeno,
    prijmeni, heslo, mobil, telefon_kancelar, mail) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);
INSERT INTO SpravceMistnosti(roomID, spravce) VALUES (?, ?);
INSERT INTO VedouciOddeleni(oddID, vedouci) VALUES (?, ?);
