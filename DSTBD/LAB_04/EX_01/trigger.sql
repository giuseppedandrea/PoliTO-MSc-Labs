/*
Evento: UPDATE OF DNAME ON DIP
Semantica: AFTER / FOR EACH ROW
Condizione: NEW.DNAME = 'SALES' AND OLD.DNAME = 'ACCOUNTING'
Azione:
        1. Incrementare di 100 il salario di tutti gli impiegati del dipartimento che è passato da 'ACCOUNTING' a 'SALES'
*/
CREATE OR REPLACE TRIGGER UpdateDepartment
AFTER UPDATE OF DNAME ON DIP
FOR EACH ROW
WHEN (NEW.DNAME = 'SALES' AND OLD.DNAME = 'ACCOUNTING')
BEGIN
    UPDATE IMP
    SET SAL = SAL + 100
    WHERE DEPTNO = :NEW.DEPTNO;
END;