SELECT roomID, oddeleni, spravce, lokace, sirka, delka, vyska FROM Mistnost;
SELECT roomID, mist_k_sezeni, pocet_rad, mist_na_radu, bloky FROM Ucebna;
SELECT roomID, el_zasuvky, eth_zasuvky FROM Kancelar;
SELECT poruchaID, zarizeni, k_oprave, technik, popis, cas_oznam, zprava_opravy FROM Porucha;
SELECT presunID, zarizeni, zadatel, duvod, provedeno, cas_zadosti, odkud, kam FROM Presun;
SELECT pravaID, popis FROM Prava;
SELECT zarID, umisteni, zodpovida, znacka, model, cena, specifikace, stav FROM Zarizeni;
SELECT oddID, vedouci, nazev, popis FROM Oddeleni;
SELECT rodnecislo, pracoviste, oddeleni, prava, login, jmeno, prijmeni, heslo, datum_narozeni, mobil, telefon_kancelar, mail FROM Zamestnanec;

