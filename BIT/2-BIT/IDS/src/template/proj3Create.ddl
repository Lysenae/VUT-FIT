CREATE SEQUENCE seq_Prava;
CREATE TABLE Mistnost (
  roomID                varchar2(255) NOT NULL, 
  OddelenioddID         varchar2(255) NOT NULL, 
  Zamestnanecrodnecislo varchar2(255) NOT NULL, 
  lokace                varchar2(255), 
  sirka                 number(10), 
  delka                 number(10), 
  vyska                 number(10), 
  oddeleniID            varchar2(255) NOT NULL, 
  zamestnanecID         varchar2(255) NOT NULL, 
  PRIMARY KEY (roomID));
CREATE TABLE Ucebna (
  roomID          number(10) NOT NULL, 
  mist_k_sezeni   number(10) NOT NULL, 
  pocet_rad       number(10), 
  mist_na_radu    number(10), 
  bloky           number(10), 
  MistnostroomID  varchar2(255) NOT NULL, 
  MistnostroomID2 varchar2(255) NOT NULL, 
  PRIMARY KEY (roomID, 
  MistnostroomID, 
  MistnostroomID2));
CREATE TABLE Kancelar (
  roomID         number(10) NOT NULL, 
  el_zasuvky     number(10) NOT NULL, 
  eth_zasuvky    number(10), 
  MistnostRoomID varchar2(255) NOT NULL, 
  MistnostroomID varchar2(255) NOT NULL, 
  PRIMARY KEY (roomID, 
  MistnostRoomID, 
  MistnostroomID));
CREATE TABLE Porucha (
  poruchaID              varchar2(255) NOT NULL, 
  ZarizenizarID          varchar2(255) NOT NULL, 
  Zamestnanecrodnecislo2 varchar2(255) NOT NULL, 
  Zamestnanecrodnecislo  varchar2(255) NOT NULL, 
  popis                  varchar2(255), 
  cas_oznam              date, 
  zprava_opravy          varchar2(255), 
  PRIMARY KEY (poruchaID));
CREATE TABLE Presun (
  presunID              varchar2(255) NOT NULL, 
  ZarizenizarID         varchar2(255) NOT NULL, 
  Zamestnanecrodnecislo varchar2(255) NOT NULL, 
  duvod                 varchar2(255), 
  provedeno             number(10), 
  cas_zadosti           date, 
  MistnostroomID        varchar2(255) NOT NULL, 
  MistnostroomID2       varchar2(255) NOT NULL, 
  PRIMARY KEY (presunID));
CREATE TABLE Prava (
  pravaID number(10) NOT NULL, 
  popis   varchar2(255), 
  PRIMARY KEY (pravaID));
CREATE TABLE Zarizeni (
  zarID                 varchar2(255) NOT NULL, 
  MistnostroomID        varchar2(255) NOT NULL, 
  Zamestnanecrodnecislo varchar2(255) NOT NULL, 
  znacka                varchar2(255), 
  model                 varchar2(255), 
  cena                  number(10), 
  specifikace           varchar2(255), 
  stav                  varchar2(255), 
  PRIMARY KEY (zarID));
CREATE TABLE Oddeleni (
  oddID                 varchar2(255) NOT NULL, 
  Zamestnanecrodnecislo varchar2(255) NOT NULL, 
  nazev                 varchar2(255), 
  popis                 varchar2(255), 
  PRIMARY KEY (oddID));
CREATE TABLE Zamestnanec (
  rodnecislo       varchar2(255) NOT NULL, 
  MistnostroomID   varchar2(255) NOT NULL, 
  OddelenioddID    varchar2(255) NOT NULL, 
  PravapravaID     number(10) NOT NULL, 
  login            varchar2(255), 
  jmeno            varchar2(255), 
  prijmeni         varchar2(255), 
  heslo            varchar2(255), 
  datum_narozeni   date, 
  mobil            number(10), 
  telefon_kancelar number(10), 
  mail             varchar2(255), 
  PRIMARY KEY (rodnecislo));
ALTER TABLE Zamestnanec ADD CONSTRAINT patril FOREIGN KEY (OddelenioddID) REFERENCES Oddeleni (oddID);
ALTER TABLE Oddeleni ADD CONSTRAINT melo FOREIGN KEY (Zamestnanecrodnecislo) REFERENCES Zamestnanec (rodnecislo);
ALTER TABLE Presun ADD CONSTRAINT presunuto FOREIGN KEY (ZarizenizarID) REFERENCES Zarizeni (zarID);
ALTER TABLE Porucha ADD CONSTRAINT poskozeno FOREIGN KEY (ZarizenizarID) REFERENCES Zarizeni (zarID);
ALTER TABLE Porucha ADD CONSTRAINT opravil FOREIGN KEY (Zamestnanecrodnecislo2) REFERENCES Zamestnanec (rodnecislo);
ALTER TABLE Porucha ADD CONSTRAINT zadal2 FOREIGN KEY (Zamestnanecrodnecislo) REFERENCES Zamestnanec (rodnecislo);
ALTER TABLE Presun ADD CONSTRAINT zadal FOREIGN KEY (Zamestnanecrodnecislo) REFERENCES Zamestnanec (rodnecislo);
ALTER TABLE Zarizeni ADD CONSTRAINT vlastnil FOREIGN KEY (Zamestnanecrodnecislo) REFERENCES Zamestnanec (rodnecislo);
ALTER TABLE Zamestnanec ADD CONSTRAINT mel FOREIGN KEY (PravapravaID) REFERENCES Prava (pravaID);
ALTER TABLE Ucebna ADD CONSTRAINT FKUcebna278811 FOREIGN KEY (MistnostroomID2) REFERENCES Mistnost (roomID);
ALTER TABLE Kancelar ADD CONSTRAINT FKKancelar432784 FOREIGN KEY (MistnostroomID) REFERENCES Mistnost (roomID);
ALTER TABLE Zarizeni ADD CONSTRAINT obsahovala FOREIGN KEY (MistnostroomID) REFERENCES Mistnost (roomID);
ALTER TABLE Zamestnanec ADD CONSTRAINT sídlil FOREIGN KEY (MistnostroomID) REFERENCES Mistnost (roomID);
ALTER TABLE Mistnost ADD CONSTRAINT spravoval FOREIGN KEY (Zamestnanecrodnecislo) REFERENCES Zamestnanec (rodnecislo);
ALTER TABLE Mistnost ADD CONSTRAINT patrila FOREIGN KEY (OddelenioddID) REFERENCES Oddeleni (oddID);
ALTER TABLE Presun ADD CONSTRAINT bude FOREIGN KEY (MistnostroomID) REFERENCES Mistnost (roomID);
ALTER TABLE Presun ADD CONSTRAINT bylo FOREIGN KEY (MistnostroomID2) REFERENCES Mistnost (roomID);

