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
  roomID                varchar2(255) NOT NULL,
  oddeleni              varchar2(255) NOT NULL,
  lokace                varchar2(255),
  sirka                 number(10),
  delka                 number(10),
  vyska                 number(10),
  PRIMARY KEY (roomID)
);

CREATE TABLE Ucebna (
  roomID                varchar2(255) NOT NULL,
  mist_k_sezeni         number(10) NOT NULL,
  pocet_rad             number(10),
  mist_na_radu          number(10),
  bloky                 number(10),
  PRIMARY KEY (roomID)
);

CREATE TABLE Kancelar (
  roomID                varchar2(255) NOT NULL,
  el_zasuvky            number(10) NOT NULL,
  eth_zasuvky           number(10),
  PRIMARY KEY (roomID)
);

CREATE TABLE Porucha (
  poruchaID             varchar2(255) NOT NULL,
  k_oprave              varchar2(255) NOT NULL,
  technik               varchar2(15) NOT NULL,
  oznamujici            varchar2(15) NOT NULL,
  popis                 varchar2(255),
  cas_oznam             date,
  zprava_opravy         varchar2(255),
  PRIMARY KEY (poruchaID)
);

CREATE TABLE Presun (
  presunID              varchar2(255) NOT NULL,
  Zarizeni              varchar2(255) NOT NULL,
  zadatel               varchar2(15) NOT NULL,
  duvod                 varchar2(255),
  provedeno             number(10),
  cas_zadosti           date,
  odkud                 varchar2(255) NOT NULL,
  kam                   varchar2(255) NOT NULL,
  PRIMARY KEY (presunID)
);

CREATE TABLE Prava (
  pravaID                number(10) NOT NULL,
  popis                  varchar2(255),
  PRIMARY KEY (pravaID)
);

CREATE TABLE Zarizeni (
  zarID                 varchar2(255) NOT NULL,
  umisteno              varchar2(255) NOT NULL,
  zodpovida             varchar2(15) NOT NULL,
  znacka                varchar2(255),
  model                 varchar2(255),
  cena                  number(10),
  specifikace           varchar2(255),
  stav                  varchar2(255),
  PRIMARY KEY (zarID)
);

CREATE TABLE Oddeleni (
  oddID                 varchar2(255) NOT NULL,
  nazev                 varchar2(255),
  popis                 varchar2(255),
  PRIMARY KEY (oddID)
);

CREATE TABLE Zamestnanec (
  zamID                 varchar2(15) NOT NULL,
  rodnecislo            number(10) NOT NULL,
  pracoviste            varchar2(255) NOT NULL,
  prava                 number(10) NOT NULL,
  login                 varchar2(255),
  jmeno                 varchar2(255) NOT NULL,
  prijmeni              varchar2(255) NOT NULL,
  heslo                 varchar2(255),
  datum_narozeni        date,
  mobil                 number(10),
  telefon_kancelar      number(10),
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
                Raise_Application_Error(-20003, 'Neplatne rodne cislo: neodpovida realnemu datumu.');
    END;
  END;
/  
--  / is ending symbol for trigger, you can use DML and DDL after that again

-- INSERT

INSERT INTO Prava(pravaID, popis) VALUES (1, 'Host');
INSERT INTO Prava(pravaID, popis) VALUES (2, 'Uzivatel');
INSERT INTO Prava(pravaID, popis) VALUES (3, 'Admin');

INSERT INTO Oddeleni(oddID, nazev, popis) VALUES ('O-01', 'Blok A', 'Ucetnictvi');
INSERT INTO Oddeleni(oddID, nazev, popis) VALUES ('O-02', 'Blok B', 'Ucebny');
INSERT INTO Oddeleni(oddID, nazev, popis) VALUES ('O-03', 'Blok C', 'Kancelare');

INSERT INTO Mistnost(roomID, oddeleni, lokace, sirka, delka, vyska)
  VALUES ('M-01-01', 'O-01', 'tam nekde', 25, 25, 25);
INSERT INTO Mistnost(roomID, oddeleni, lokace, sirka, delka, vyska)
  VALUES ('M-02-01', 'O-02', 'tam nekde', 25, 25, 25);
INSERT INTO Mistnost(roomID, oddeleni, lokace, sirka, delka, vyska)
  VALUES ('M-03-01', 'O-03', 'tam nekde', 25, 25, 25);
  
INSERT INTO Ucebna(roomID, mist_k_sezeni, pocet_rad, mist_na_radu, bloky)
  VALUES ('M-02-01', 250, 25, 10, 5);
  
INSERT INTO Kancelar(roomID, el_zasuvky, eth_zasuvky)
  VALUES ('M-01-01', 10, 5);
INSERT INTO Kancelar(roomID, el_zasuvky, eth_zasuvky)
  VALUES ('M-03-01', 20, 12);
  
INSERT INTO Zamestnanec(zamID, rodnecislo, pracoviste, prava, login, jmeno,
    prijmeni, heslo, mobil, telefon_kancelar, mail)
    VALUES ('Z1', 9012286778, 'M-03-01', 3, 'xklima22', 'Daniel',
    'Klimaj', 'heslo', 0123456789, 9876543210, 'xklima22@stud.fit.vutbr.cz');
INSERT INTO Zamestnanec(zamID, rodnecislo, pracoviste, prava, login, jmeno,
    prijmeni, heslo, mobil, telefon_kancelar, mail)
    VALUES ('Z2', 9402145225, 'M-02-01', 2, 'xvecer18', 'Vojtěch',
    'Večeřa', 'ahojaa', 6789543210 , 6789012345, 'xvecer18@stud.fit.vutbr.cz');

INSERT INTO Zarizeni(zarID, umisteno, zodpovida, znacka, model, cena, specifikace, stav)
  VALUES ('D0001', 'M-03-01', 'Z1', 'Brand', 'SuperModel', 5999, 'Spec', 'Poskozeno');
INSERT INTO Zarizeni(zarID, umisteno, zodpovida, znacka, model, cena, specifikace, stav)
  VALUES ('D0002', 'M-01-01', 'Z1', 'Abcd', 'SuperModel-Mk2', 5999, 'Spec', 'Ok');
  
INSERT INTO Porucha(poruchaID, k_oprave, technik, oznamujici, popis, cas_oznam, zprava_opravy)
  VALUES ('P0001', 'D0001', 'Z2', 'Z1', 'Neco se pokazilo', TO_DATE('2015-03-28', 'yyyy-mm--dd'), 'Opravit');
INSERT INTO Porucha(poruchaID, k_oprave, technik, oznamujici, popis, cas_oznam, zprava_opravy)
  VALUES ('P0002', 'D0002', 'Z1', 'Z2', 'Neco se pokazilo', TO_DATE('2015-03-29', 'yyyy-mm--dd'), 'Opravit');
  
INSERT INTO Presun(presunID, zarizeni, zadatel, duvod, provedeno, cas_zadosti, odkud, kam)
  VALUES ('T0001', 'D0001', 'Z1', 'Duvod', 0, TO_DATE('2015-03-28 18:10', 'yyyy-mm--dd hh24:mi'), 'M-01-01', 'M-02-01');
INSERT INTO Presun(presunID, zarizeni, zadatel, duvod, provedeno, cas_zadosti, odkud, kam)
  VALUES ('T0002', 'D0002', 'Z2', 'Nelibi se mi', 0, TO_DATE('2015-03-28 18:10', 'yyyy-mm--ddhh24:mi'), 'M-02-01', 'M-03-01');

COMMIT;