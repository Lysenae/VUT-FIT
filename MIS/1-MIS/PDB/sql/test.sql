SELECT * FROM area;

delete from area where id < 20;


commit;

DROP SEQUENCE area_seq;
CREATE SEQUENCE area_seq START WITH 1 INCREMENT BY 1;

SELECT DISTINCT a1.id, a2.id
FROM area a1, area a2
WHERE a1.id = 4 AND a1.id<>a2.id AND
a1.valid=1 AND a2.valid=1 AND
a1.pt_type > 2 AND a2.pt_type <= 2 AND
SDO_RELATE(a1.geometry, a2.geometry, 'MASK=OVERLAPBDYINTERSECT+CONTAINS+INSIDE') = 'TRUE'
ORDER BY a1.id;


SELECT id, SDO_NN_DISTANCE(1) AS dist FROM area
WHERE SDO_NN(geometry, (SELECT o.geometry FROM area o
WHERE o.id = 8), 'sdo_batch_size = 1', 1) = 'TRUE' AND
ROWNUM <= 1 AND id <> 8 ORDER BY dist;