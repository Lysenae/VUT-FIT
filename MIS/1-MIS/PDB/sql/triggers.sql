create or replace trigger insert_area
before insert on area
for each row
begin
    select area_seq.nextval into :new.id from dual;
end;
/

create or replace trigger insert_customer
before insert on customer
for each row
begin
    select customer_seq.nextval into :new.id from dual;
end;
/

create or replace trigger insert_photo
before insert on photo
for each row
begin
    select photo_seq.nextval into :new.id from dual;
end;
/

create or replace trigger insert_reservation
before insert on reservation
for each row
begin
    select reservation_seq.nextval into :new.id from dual;
end;
/
