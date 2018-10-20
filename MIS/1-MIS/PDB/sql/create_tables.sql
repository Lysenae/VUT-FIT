DROP TABLE reservation;
DROP TABLE photo;
DROP TABLE customer;
DROP TABLE area;
DROP TABLE point_type;

-- ####################
CREATE TABLE point_type (
  id   NUMBER NOT null,
  name VARCHAR(20),
  CONSTRAINT pk_point_type PRIMARY KEY (id)
);

COMMIT;

-- ####################
CREATE TABLE area (
  id       NUMBER NOT null,
  name     VARCHAR(50),
  pt_type  NUMBER,
  geometry SDO_GEOMETRY,
  valid    NUMBER,
  CONSTRAINT pk_area PRIMARY KEY (id),
  CONSTRAINT fk_area_pt_type FOREIGN KEY (pt_type) REFERENCES point_type(id) ON DELETE CASCADE
);

DROP SEQUENCE area_seq;
CREATE SEQUENCE area_seq START WITH 1 INCREMENT BY 1;

DELETE FROM USER_SDO_GEOM_METADATA WHERE
  TABLE_NAME = 'AREA' AND COLUMN_NAME = 'GEOMETRY';

COMMIT;

-- ####################
CREATE TABLE customer (
  id      NUMBER      NOT null,
  name    VARCHAR(32) NOT null,
  surname VARCHAR(32) NOT null,
  address VARCHAR(32) NOT null,
  city    VARCHAR(32) NOT null,
  psc     VARCHAR(32) NOT null,
  phone   VARCHAR(32) NOT null,
  email   VARCHAR(32) NOT null,
  CONSTRAINT pk_customer PRIMARY KEY (id)
);

DROP SEQUENCE customer_seq;
CREATE SEQUENCE customer_seq  START WITH 1 INCREMENT BY 1;

COMMIT;

-- ####################
CREATE TABLE photo (
  id     NUMBER NOT NULL,
  area   NUMBER NOT NULL,
  img    ORDSYS.ORDImage,
  img_si ORDSYS.SI_StillImage,
  img_ac ORDSYS.SI_AverageColor,
  img_ch ORDSYS.SI_ColorHistogram,
  img_pc ORDSYS.SI_PositionalColor,
  img_tx ORDSYS.SI_Texture,
  CONSTRAINT pk_photos   PRIMARY KEY (id),
  CONSTRAINT fk_area_photo FOREIGN KEY (area) REFERENCES area(id) ON DELETE CASCADE
);

DROP SEQUENCE photo_seq;
CREATE SEQUENCE photo_seq START WITH 1 INCREMENT BY 1;

COMMIT;

-- ####################
CREATE TABLE reservation (
  id        NUMBER NOT NULL,
  customer  NUMBER NOT NULL,
  area      NUMBER NOT NULL,
  date_from DATE   NOT NULL,
  date_to   DATE   NOT NULL,
  CONSTRAINT pk_reservation           PRIMARY KEY (id),
  CONSTRAINT fk_reservation_customer  FOREIGN KEY (customer) REFERENCES customer(id) ON DELETE CASCADE,
  CONSTRAINT fk_reservation_area      FOREIGN KEY (area)     REFERENCES area(id)     ON DELETE CASCADE,
  CONSTRAINT uk_reservation_area_date UNIQUE (area, date_from, date_to)
);

DROP SEQUENCE reservation_seq;
CREATE SEQUENCE reservation_seq START WITH 1 INCREMENT BY 1;

COMMIT;
