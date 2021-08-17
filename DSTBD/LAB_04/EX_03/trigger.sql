/*
Evento: INSERT OR UPDATE OF JOB ON IMP
Semantica: AFTER / FOR EACH ROW
Condizione: -
Azione:
        1. Verificare se :NEW.JOB è presente nella tabella SUMMARY
        2. Se :NEW.JOB non è presente nella tabella SUMMARY, inserire un nuovo record per il nuovo lavoro con NUM pari a 1\
        3. Se :NEW.JOB è presente nella tabella SUMMARY, incrementare NUM del nuovo lavoro
        4. Se :OLD.JOB non è NULL, decrementare NUM del vecchio lavoro
*/
CREATE OR REPLACE TRIGGER UpdateSummary
AFTER INSERT OR UPDATE OF JOB ON IMP
FOR EACH ROW
DECLARE
N NUMBER;
BEGIN
    -- Verificare se :NEW.JOB è presente nella tabella SUMMARY
    SELECT COUNT(*) INTO N
    FROM SUMMARY
    WHERE JOB = :NEW.JOB;
    -- Se :NEW.JOB non è presente nella tabella SUMMARY
    IF (N = 0) THEN
        -- Inserire un nuovo record per il nuovo lavoro con NUM pari a 1
        INSERT INTO SUMMARY(JOB, NUM)
        VALUES (:NEW.JOB, 1);
    -- Se :NEW.JOB è presente nella tabella SUMMARY
    ELSE
        -- Ottenere valore NUM del nuovo lavoro
        SELECT NUM INTO N
        FROM SUMMARY
        WHERE JOB = :NEW.JOB;
        -- Incrementare NUM del nuovo lavoro
        UPDATE SUMMARY
        SET NUM = N + 1
        WHERE JOB = :NEW.JOB;
    END IF;
    -- Se :OLD.JOB non è NULL
    IF (:OLD.JOB IS NOT NULL) THEN
        -- Ottenere valore NUM del vecchio lavoro
        SELECT NUM INTO N
        FROM SUMMARY
        WHERE JOB = :OLD.JOB;
        -- Decrementare NUM del vecchio lavoro
        UPDATE SUMMARY
        SET NUM = N - 1
        WHERE JOB = :OLD.JOB;
    END IF;
END;