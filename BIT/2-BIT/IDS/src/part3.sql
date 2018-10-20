-- Drop section

DROP TABLE Mistnost CASCADE CONSTRAINTS;
DROP TABLE Ucebna CASCADE CONSTRAINTS;
DROP TABLE Kancelar CASCADE CONSTRAINTS;
DROP TABLE Porucha CASCADE CONSTRAINTS;
DROP TABLE Presun CASCADE CONSTRAINTS;
DROP TABLE Prava CASCADE CONSTRAINTS;
DROP TABLE Zarizeni CASCADE CONSTRAINTS;
DROP TABLE Oddeleni CASCADE CONSTRAINTS;
DROP TABLE Zamestnanec CASCADE CONSTRAINTS;

-- Create section

CREATE TABLE Mistnost (
  roomID                varchar2(7) NOT NULL,
  oddeleni              varchar2(4) NOT NULL,
  lokace                varchar2(30),
  sirka                 number(2),
  delka                 number(2),
  vyska                 number(2),
  PRIMARY KEY (roomID)
);

CREATE TABLE Ucebna (
  roomID                varchar2(7) NOT NULL,
  mist_k_sezeni         number(3) NOT NULL,
  pocet_rad             number(2),
  mist_na_radu          number(3),
  bloky                 number(2),
  PRIMARY KEY (roomID)
);

CREATE TABLE Kancelar (
  roomID                varchar2(7) NOT NULL,
  el_zasuvky            number(2) NOT NULL,
  eth_zasuvky           number(2),
  PRIMARY KEY (roomID)
);

CREATE TABLE Porucha (
  poruchaID             varchar2(5) NOT NULL,
  k_oprave              varchar2(5) NOT NULL,
  technik               varchar2(5) NOT NULL,
  oznamujici            varchar2(5) NOT NULL,
  popis                 varchar2(255),
  cas_oznam             date,
  zprava_opravy         varchar2(255),
  PRIMARY KEY (poruchaID)
);

CREATE TABLE Presun (
  presunID              varchar2(5) NOT NULL,
  zarizeni              varchar2(5) NOT NULL,
  zadatel               varchar2(5) NOT NULL,
  duvod                 varchar2(255),
  provedeno             date,
  cas_zadosti           date,
  odkud                 varchar2(7) NOT NULL,
  kam                   varchar2(7) NOT NULL,
  PRIMARY KEY (presunID)
);

CREATE TABLE Prava (
  pravaID                number(2) NOT NULL,
  popis                  varchar2(255),
  PRIMARY KEY (pravaID)
);

CREATE TABLE Zarizeni (
  zarID                 varchar2(5) NOT NULL,
  umisteno              varchar2(7) NOT NULL,
  zodpovida             varchar2(5) NOT NULL,
  znacka                varchar2(30),
  model                 varchar2(30),
  cena                  number(10),
  specifikace           varchar2(255),
  stav                  varchar2(100),
  PRIMARY KEY (zarID)
);

CREATE TABLE Oddeleni (
  oddID                 varchar2(4) NOT NULL,
  nazev                 varchar2(50),
  popis                 varchar2(255),
  PRIMARY KEY (oddID)
);

CREATE TABLE Zamestnanec (
  zamID                 varchar2(5) NOT NULL,
  rodnecislo            number(10) NOT NULL,
  pracoviste            varchar2(7) NOT NULL,
  prava                 number(2) NOT NULL,
  login                 varchar2(10) UNIQUE,
  jmeno                 varchar2(20) NOT NULL,
  prijmeni              varchar2(20) NOT NULL,
  heslo                 varchar2(12),
  datum_narozeni        date,
  mobil                 number(9),
  telefon_kancelar      number(9),
  mail                  varchar2(255),
  PRIMARY KEY (zamID)
);

-- Pridani cizich klicu

ALTER TABLE Mistnost ADD CONSTRAINT patrila FOREIGN KEY (oddeleni) REFERENCES Oddeleni (oddID);

ALTER TABLE Ucebna ADD CONSTRAINT FKUcebna FOREIGN KEY (roomID) REFERENCES Mistnost (roomID);

ALTER TABLE Kancelar ADD CONSTRAINT FKKancelar FOREIGN KEY (roomID) REFERENCES Mistnost (roomID);

ALTER TABLE Porucha ADD CONSTRAINT poskozeno FOREIGN KEY (k_oprave) REFERENCES Zarizeni (zarID);
ALTER TABLE Porucha ADD CONSTRAINT opravil FOREIGN KEY (technik) REFERENCES Zamestnanec (zamID);
ALTER TABLE Porucha ADD CONSTRAINT oznamil FOREIGN KEY (oznamujici) REFERENCES Zamestnanec (zamID);

ALTER TABLE Presun ADD CONSTRAINT presunuto FOREIGN KEY (zarizeni) REFERENCES Zarizeni (zarID);
ALTER TABLE Presun ADD CONSTRAINT zadal FOREIGN KEY (zadatel) REFERENCES Zamestnanec (zamID);

ALTER TABLE Zarizeni ADD CONSTRAINT vlastnil FOREIGN KEY (zodpovida) REFERENCES Zamestnanec (zamID);
ALTER TABLE Zarizeni ADD CONSTRAINT obsahovala FOREIGN KEY (umisteno) REFERENCES Mistnost (roomID);

ALTER TABLE Zamestnanec ADD CONSTRAINT mel FOREIGN KEY (prava) REFERENCES Prava (pravaID);
ALTER TABLE Zamestnanec ADD CONSTRAINT sidlil FOREIGN KEY (pracoviste) REFERENCES Mistnost (roomID);

-- TRIGGER

CREATE OR REPLACE TRIGGER tr_rc
BEFORE INSERT OR UPDATE OF rodnecislo ON Zamestnanec
  FOR EACH ROW
  DECLARE
    rc Zamestnanec.rodnecislo%TYPE;
    d number(2);
    m number(2);
    r number(4);
    datum date;
  BEGIN
    rc := :NEW.rodnecislo;
    d  := MOD((rc / 10000),100);
    m  := MOD((rc / 1000000),100);
    r  := rc / 100000000;
    
    IF (MOD(rc, 11) <> 0) THEN
      Raise_application_Error (-20001, 'Neplatne rodne cislo: neni delitelne 11.');
    END IF;
    
    IF (MOD(rc,1000) = 0 AND LENGTH(rc) = 9) THEN
      Raise_application_Error (-20002, 'Neplatne rodne cislo: nesmi koncit 000.');
    END IF;
    
    IF (m > 50) THEN
      m := m - 50;
    END IF;
    
    IF (m > 20) THEN
      m := m - 20;
    END IF;
    
    IF (LENGTH(rc) = 9 AND r > 53)THEN
      r := 1800 + r;
    ELSE 
      r := 1900 + r;
    END IF;
    
    IF (LENGTH(rc) = 10 AND r > 53)THEN
      r := 1900 + r;
    ELSE 
      r := 2000 + r;
    END IF;
    
    BEGIN
      datum := TO_DATE(d||'-'||m||'-'||r, 'dd-mm-yyyy');
      EXCEPTION
      WHEN OTHERS THEN
                Raise_Application_Error(-20003, 'Neplatne rodno Cislo: neodpovida realnemu datumu.');
    END;
  END;
/  
--  / is ending symbol for trigger, you can use DML and DDL after that again

-- INSERT

INSERT INTO Prava(pravaID, popis) VALUES (1, 'Administrativni pracovnik');
INSERT INTO Prava(pravaID, popis) VALUES (2, 'Profesor');
INSERT INTO Prava(pravaID, popis) VALUES (3, 'Vedouci');
INSERT INTO Prava(pravaID, popis) VALUES (4, 'Technik');
INSERT INTO Prava(pravaID, popis) VALUES (5, 'Admin');

INSERT INTO Oddeleni(oddID, nazev, popis) 
  VALUES ('O-01', 'Informacni systemy', 'Formalni modely, databaze, management SW');
INSERT INTO Oddeleni(oddID, nazev, popis) 
  VALUES ('O-02', 'Pocitacove systemy', 'Diagnostika, architerktury, akcelerovani');
INSERT INTO Oddeleni(oddID, nazev, popis) 
  VALUES ('O-03', 'Inteligentni systemy', 'Biometricke systemy, robotika, Petriho site');
INSERT INTO Oddeleni(oddID, nazev, popis) 
  VALUES ('O-04', 'Grafika a multimedia', 'Grafika, zvuk, obraz, medicina');
INSERT INTO Oddeleni(oddID, nazev, popis) 
  VALUES ('O-05', 'Dekanat', 'Vedeni� fakulty');
INSERT INTO Oddeleni(oddID, nazev, popis) 
  VALUES ('O-06', 'CVT', 'Centrum vypocetni techniky');
INSERT INTO Oddeleni(oddID, nazev, popis) 
  VALUES ('O-07', 'Vyzkumni centrum IT', 'IT4Innovations');

INSERT INTO Mistnost(roomID, oddeleni, lokace, sirka, delka, vyska)
  VALUES ('M-21-01', 'O-05', 'Budova 2 p.1', 5, 4, 3);
INSERT INTO Mistnost(roomID, oddeleni, lokace, sirka, delka, vyska)
  VALUES ('M-22-01', 'O-01', 'Budova 2 p.2', 25, 25, 10);
INSERT INTO Mistnost(roomID, oddeleni, lokace, sirka, delka, vyska)
  VALUES ('M-23-01', 'O-03', 'Budova 2 p.3', 3, 4, 3);
INSERT INTO Mistnost(roomID, oddeleni, lokace, sirka, delka, vyska)
  VALUES ('M-11-01', 'O-04', 'Budova 1 p.1', 8, 5, 3);
INSERT INTO Mistnost(roomID, oddeleni, lokace, sirka, delka, vyska)
  VALUES ('M-12-01', 'O-03', 'Budova 1 p.2', 6, 5, 3);
INSERT INTO Mistnost(roomID, oddeleni, lokace, sirka, delka, vyska)
  VALUES ('M-13-01', 'O-02', 'Budova 1 p.3', 6, 5, 3);
INSERT INTO Mistnost(roomID, oddeleni, lokace, sirka, delka, vyska)
  VALUES ('M-14-01', 'O-07', 'Budova 1 p.4', 8, 5, 3);
  
INSERT INTO Ucebna(roomID, mist_k_sezeni, pocet_rad, mist_na_radu, bloky)
  VALUES ('M-22-01', 152, 8, 19, 3);
INSERT INTO Ucebna(roomID, mist_k_sezeni, pocet_rad, mist_na_radu, bloky)
  VALUES ('M-11-01', 20, 4, 5, 1);
INSERT INTO Ucebna(roomID, mist_k_sezeni, pocet_rad, mist_na_radu, bloky)
  VALUES ('M-12-01', 20, 4, 5, 1);
  
INSERT INTO Kancelar(roomID, el_zasuvky, eth_zasuvky)
  VALUES ('M-21-01', 10, 5);
INSERT INTO Kancelar(roomID, el_zasuvky, eth_zasuvky)
  VALUES ('M-23-01', 8, 4);
INSERT INTO Kancelar(roomID, el_zasuvky, eth_zasuvky)
  VALUES ('M-13-01', 12, 6);
INSERT INTO Kancelar(roomID, el_zasuvky, eth_zasuvky)
  VALUES ('M-14-01', 16, 8);
  
INSERT INTO Zamestnanec(zamID, rodnecislo, pracoviste, prava, login, jmeno,
    prijmeni, heslo, mobil, telefon_kancelar, mail)
    VALUES ('Z1', 9012286778, 'M-23-01', 2, 'xklima22', 'Daniel',
    'Klimaj', 'heslo', 012345678, 987654321, 'xklima22@stud.fit.vutbr.cz');
INSERT INTO Zamestnanec(zamID, rodnecislo, pracoviste, prava, login, jmeno,
    prijmeni, heslo, mobil, telefon_kancelar, mail)
    VALUES ('Z2', 9402145225, 'M-13-01', 4, 'xvecer18', 'Vojtech',
    'Vecera', 'ahojaa', 678954321, 678912345, 'xvecer18@stud.fit.vutbr.cz');
INSERT INTO Zamestnanec(zamID, rodnecislo, pracoviste, prava, login, jmeno,
    prijmeni, heslo, mobil, telefon_kancelar, mail)
    VALUES ('Z3', 9256229389, 'M-14-01', 5, 'malina', 'Jan',
    'Malina', '123456', 879790123, 312354894, 'malina@fit.vutbr.cz');
INSERT INTO Zamestnanec(zamID, rodnecislo, pracoviste, prava, login, jmeno,
    prijmeni, heslo, mobil, telefon_kancelar, mail)
    VALUES ('Z4', 8954105501, 'M-13-01', 4, 'tesarc', 'Ctibor',
    'Tesar', 'password', 879465489, 312354910, 'tesar@fit.vutbr.cz');
INSERT INTO Zamestnanec(zamID, rodnecislo, pracoviste, prava, login, jmeno,
    prijmeni, heslo, mobil, telefon_kancelar, mail)
    VALUES ('Z5', 9151118977, 'M-21-01', 3, 'novakb', 'Blazej',
    'Novak', 'blazej', 698454684, 312354873, 'novak@fit.vutbr.cz');

INSERT INTO Zarizeni(zarID, umisteno, zodpovida, znacka, model, cena, specifikace, stav)
  VALUES ('D0001', 'M-14-01', 'Z3', 'Acer', 'abc-567', 5999, 'tablet', 'Vadný konektor');
INSERT INTO Zarizeni(zarID, umisteno, zodpovida, znacka, model, cena, specifikace, stav)
  VALUES ('D0002', 'M-13-01', 'Z4', 'HP', 'LaserJet1235', 3000, 'tiskarna', 'Ok');
INSERT INTO Zarizeni(zarID, umisteno, zodpovida, znacka, model, cena, specifikace, stav)
  VALUES ('D0003', 'M-21-01', 'Z5', 'Lenovo', 'Thinkpad T50', 20000, 'notebook', 'Špatná baterie');
INSERT INTO Zarizeni(zarID, umisteno, zodpovida, znacka, model, cena, specifikace, stav)
  VALUES ('D0004', 'M-21-01', 'Z3', 'LG', 'SuperModel-Mk2', 8000, 'mobil', 'Ok');
INSERT INTO Zarizeni(zarID, umisteno, zodpovida, znacka, model, cena, specifikace, stav)
  VALUES ('D0005', 'M-14-01', 'Z5', 'Samsung', 'SmartTV2456', 14000, 'monitor', 'Ok');
INSERT INTO Zarizeni(zarID, umisteno, zodpovida, znacka, model, cena, specifikace, stav)
  VALUES ('D0006', 'M-23-01', 'Z1', 'Genius', 'Silver-5894', 5999, 'reproduktory', 'Ok');
  
INSERT INTO Porucha(poruchaID, k_oprave, technik, oznamujici, popis, cas_oznam, zprava_opravy)
  VALUES ('P0005', 'D0001', 'Z4', 'Z4', 'Vadny konektor napajeni', TO_DATE('2015-08-09', 'yyyy-mm--dd'), '');
INSERT INTO Porucha(poruchaID, k_oprave, technik, oznamujici, popis, cas_oznam, zprava_opravy)
  VALUES ('P0004', 'D0003', 'Z2', 'Z2', 'Spatna baterie', TO_DATE('2015-03-20', 'yyyy-mm--dd'), '');
INSERT INTO Porucha(poruchaID, k_oprave, technik, oznamujici, popis, cas_oznam, zprava_opravy)
  VALUES ('P0003', 'D0003', 'Z2', 'Z5', 'Nereaguje touchpad', TO_DATE('2015-03-08', 'yyyy-mm--dd'), 'Vymenen touchpad');
INSERT INTO Porucha(poruchaID, k_oprave, technik, oznamujici, popis, cas_oznam, zprava_opravy)
  VALUES ('P0002', 'D0001', 'Z4', 'Z4', 'Nedekuje pam. kartu', TO_DATE('2015-02-27', 'yyyy-mm--dd'), 'Vyměněn slot pro kartu');
INSERT INTO Porucha(poruchaID, k_oprave, technik, oznamujici, popis, cas_oznam, zprava_opravy)
  VALUES ('P0001', 'D0005', 'Z4', 'Z2', 'Vadne pixely', TO_DATE('2015-03-28', 'yyyy-mm--dd'), 'Provedena reklamace');
  
INSERT INTO Presun(presunID, zarizeni, zadatel, duvod, provedeno, cas_zadosti, odkud, kam)
  VALUES ('T0001', 'D0001', 'Z3', 'Nepouzivam', TO_DATE('2015-04-03 9:20','yyyy-mm--dd hh24:mi'), TO_DATE('2015-03-28 18:10', 'yyyy-mm--dd hh24:mi'), 'M-14-01', 'M-13-01');
INSERT INTO Presun(presunID, zarizeni, zadatel, duvod, provedeno, cas_zadosti, odkud, kam)
  VALUES ('T0002', 'D0002', 'Z4', 'Zbytecne velka', TO_DATE('','yyyy-mm--dd hh24:mi'), TO_DATE('2015-03-28 18:10', 'yyyy-mm--ddhh24:mi'), 'M-13-01', 'M-21-01');

COMMIT;

-- 2 spojeni 2 tabulek
-- Dotaz na to kde ma ktere oddeleni sve ucebny a kancelare
SELECT o.nazev as Oddeleni, m.lokace
  FROM Mistnost m, Oddeleni o
  WHERE o.oddID = m.oddeleni
  ORDER BY o.oddID
;

-- Jak velkou ma kdo kancelar?
SELECT z.jmeno, z.prijmeni, m.sirka, m.delka, m.vyska
  FROM Mistnost m, Zamestnanec z
  WHERE z.pracoviste = m.roomID
  ORDER BY z.zamID
; 

-- 1 spojeni 3 tabulek
-- Do ktereho oddeleni ktery zamestnanec pracuje a kde ho najdeme?

SELECT z.jmeno, z.prijmeni, m.lokace as Pracoviste, o.nazev as Oddeleni
  FROM Mistnost m, Oddeleni o, Zamestnanec z
  WHERE o.oddID = m.oddeleni and z.pracoviste = m.roomID
  ORDER BY z.zamID
;

-- 2 dotazy s group by a agregacni funkci
-- Kolik zarizeni ktery technik opravil?

SELECT z.jmeno, z.prijmeni, COUNT(*) as Pocet
  FROM Zamestnanec z, Porucha p
  WHERE z.zamID = p.technik
  GROUP BY z.jmeno, z.prijmeni
;

-- Jakou ma ktere oddeleni nejvetsi mistnost a jakou ta ma plochu?

SELECT o.nazev as Nazev_mistnosti, MAX(m.vyska*m.sirka) as Plocha
  FROM Oddeleni o, Mistnost m
  WHERE m.oddeleni = o.oddID
  GROUP BY o.nazev
;

-- 1 dotaz s EXISTS
-- Ktera zarizeni jiz mela poruchu?

SELECT DISTINCT zarID as ID_zarizeni, znacka, model
  FROM Zarizeni zar
  WHERE EXISTS(
    SELECT *
    FROM Porucha p
    WHERE p.k_oprave = zar.zarID
  )
;

-- 1 dotaz s IN s vnorenym selectem
-- Ktera zarizeni jiz zmenila sveho opatrovnika?

SELECT zarID as ID_zarizeni, znacka, model
  FROM Zarizeni
  WHERE zarID IN(
    SELECT zarizeni
    FROM Presun
  )
;

COMMIT;
