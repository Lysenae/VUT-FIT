-- xstane33, Jiří Staněk
-- xskota07, Jiří Skoták
-- xklima22, Daniel Klimaj

DROP TABLE reservation CASCADE CONSTRAINTS;
DROP TABLE photo CASCADE CONSTRAINTS;
DROP TABLE customer CASCADE CONSTRAINTS;
DROP TABLE area CASCADE CONSTRAINTS;
DROP TABLE point_type CASCADE CONSTRAINTS;

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

INSERT INTO USER_SDO_GEOM_METADATA VALUES (
    'area', 'geometry',
    SDO_DIM_ARRAY(SDO_DIM_ELEMENT('X', 0, 1000, 1), SDO_DIM_ELEMENT('Y', 0, 1000, 1)),
    NULL
);

CREATE INDEX areal_sidx ON area(geometry) INDEXTYPE is MDSYS.SPATIAL_INDEX;

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

-- ####################
CREATE TABLE photo (
  id     NUMBER NOT NULL,
  reservation   NUMBER NOT NULL,
  img    ORDSYS.ORDImage,
  img_si ORDSYS.SI_StillImage,
  img_ac ORDSYS.SI_AverageColor,
  img_ch ORDSYS.SI_ColorHistogram,
  img_pc ORDSYS.SI_PositionalColor,
  img_tx ORDSYS.SI_Texture,
  CONSTRAINT pk_photos   PRIMARY KEY (id),
  CONSTRAINT fk_reservation_photo FOREIGN KEY (reservation) REFERENCES reservation(id) ON DELETE CASCADE
);

DROP SEQUENCE photo_seq;
CREATE SEQUENCE photo_seq START WITH 1 INCREMENT BY 1;

COMMIT;


DROP SEQUENCE reservation_seq;
CREATE SEQUENCE reservation_seq START WITH 1 INCREMENT BY 1;

COMMIT;

-- point_type
INSERT INTO point_type(id, name) VALUES (1, 'Point');
INSERT INTO point_type(id, name) VALUES (2, 'Line');
INSERT INTO point_type(id, name) VALUES (3, 'Line Polygon');
INSERT INTO point_type(id, name) VALUES (4, 'Rectangle');
INSERT INTO point_type(id, name) VALUES (5, 'Circle');
INSERT INTO point_type(id, name) VALUES (6, 'Composite');
COMMIT;

create or replace trigger insert_area before insert on area for each row begin select area_seq.nextval into :new.id from dual;end;/

create or replace trigger insert_customer before insert on customer for each row begin select customer_seq.nextval into :new.id from dual;end;/

create or replace trigger insert_photo before insert on photo for each row begin select photo_seq.nextval into :new.id from dual;end;/

create or replace trigger insert_reservation before insert on reservation for each row begin select reservation_seq.nextval into :new.id from dual;end;/

COMMIT;


INSERT INTO area(name, pt_type, geometry, valid) VALUES (
    'r_rect',
    4,
    SDO_GEOMETRY(2003, NULL, NULL,
        SDO_ELEM_INFO_ARRAY(1, 1003, 3),
        SDO_ORDINATE_ARRAY(10, 10, 210, 410)
    ),
    1
);

INSERT INTO area(name, pt_type, geometry, valid) VALUES (
    'r_circle',
    5,
    SDO_GEOMETRY(2004, NULL, NULL,
        SDO_ELEM_INFO_ARRAY(1, 1003, 4),
        SDO_ORDINATE_ARRAY(400,10, 400,110, 450,60)
    ),
    1
);

COMMIT;

INSERT INTO customer(name,surname,address,city,psc,phone,email) VALUES (
    'Kolar','random1','random1','random1','random1','random1','random1'
);
INSERT INTO customer(name,surname,address,city,psc,phone,email) VALUES (
    'Rychly','random2','random2','random2','random2','random2','random2'
);

COMMIT;

INSERT INTO reservation(customer, area, date_from, date_to) VALUES (
    1, 1,
    TO_DATE('2016/12/06 21:00:30', 'yyyy/mm/dd hh24:mi:ss'),
    TO_DATE('2016/12/20 23:00:00', 'yyyy/mm/dd hh24:mi:ss')
);

INSERT INTO reservation(customer, area, date_from, date_to) VALUES (
    2, 2,
    TO_DATE('2016/12/10 15:20:00', 'yyyy/mm/dd hh24:mi:ss'),
    TO_DATE('2016/12/23 20:00:00', 'yyyy/mm/dd hh24:mi:ss')
);

COMMIT;
