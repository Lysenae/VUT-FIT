UPDATE Mistnost SET oddeleni = ?, spravce = ?, lokace = ?, sirka = ?, delka = ?, vyska = ?  WHERE roomID = ?;
UPDATE Ucebna SET mist_k_sezeni = ?, pocet_rad = ?, mist_na_radu = ?, bloky = ? WHERE roomID = ?;
UPDATE Kancelar SET el_zasuvky = ?, eth_zasuvky = ? WHERE roomID = ?;
UPDATE Porucha SET zarizeni = ?, k_oprave = ?, technik = ?, popis = ?, cas_oznam = ?, zprava_opravy = ? WHERE poruchaID = ?;
UPDATE Presun SET zarizenizar = ?, zadatel = ?, duvod = ?, provedeno = ?, cas_zadosti = ?, odkud = ?, kam = ? WHERE presunID = ?;
UPDATE Prava SET popis = ? WHERE pravaID = ?;
UPDATE Zarizeni SET umisteno = ?, zodpovida = ?, znacka = ?, model = ?, cena = ?, specifikace = ?, stav = ? WHERE zarID = ?;
UPDATE Oddeleni SET vedouci = ?, nazev = ?, popis = ? WHERE oddID = ?;
UPDATE Zamestnanec SET pracoviste = ?, oddeleni = ?, prava = ?, login = ?, jmeno = ?, prijmeni = ?, heslo = ?, datum_narozeni = ?, mobil = ?, telefon_kancelar = ?, mail = ? WHERE rodnecislo = ?;

