/*
Evento: INSERT ON TICKETS
Semantica: AFTER / FOR EACH ROW
Condizione: NEW.CARDNO IS NOT NULL
Azione:
        1. Ottenere il numero di miglia del volo
        2. Inserire le informazioni appropriate nella tabella CREDITS
        3. Ottenere il numero totale di miglia associate alla carta
        4. Ottenere lo status attuale della carta
        5. Verificare se il cliente ha cambiato STATUS
        6. Se il cliente ha cambiato STATUS, aggiornare la tabella CARDS ed inserire l'informazione necessaria nella tabella NOTIFY
*/
CREATE OR REPLACE TRIGGER NewTicket
AFTER INSERT ON TICKETS
FOR EACH ROW
WHEN (NEW.CARDNO IS NOT NULL)
DECLARE
FlightMiles NUMBER(5);
TotalMiles NUMBER(5);
CurrentStatus VARCHAR2(16);
NewStatus VARCHAR2(16);
ChangeStatus BOOLEAN;
ID NUMBER(1);
BEGIN
    -- Ottenere il numero di miglia del volo
    SELECT MILES INTO FlightMiles
    FROM FLIGHTS
    WHERE FLIGHTID = :NEW.FLIGHTID;
    -- Inserire le informazioni appropriate nella tabella CREDITS
    INSERT INTO CREDITS(TICKETID, CARDNO, MILES)
    VALUES (:NEW.TICKETID, :NEW.CARDNO, FlightMiles);
    -- Ottenere il numero totale di miglia associate alla carta
    SELECT SUM(MILES) INTO TotalMiles
    FROM CREDITS
    WHERE CARDNO = :NEW.CARDNO;
    -- Ottenere lo status attuale della carta
    SELECT STATUS INTO CurrentStatus
    FROM CARDS
    WHERE CARDNO = :NEW.CARDNO;
    -- Verificare se il cliente ha cambiato STATUS
    IF (CurrentStatus = 'SILVER' AND TotalMiles > 30000 AND TotalMiles <=50000) THEN
        NewStatus := 'GOLD';
        ChangeStatus := TRUE;
    ELSIF (CurrentStatus = 'SILVER' AND TotalMiles > 50000) THEN
        NewStatus := 'PREMIUM';
        ChangeStatus := TRUE;
    ELSIF (CurrentStatus = 'GOLD' AND TotalMiles > 50000) THEN
        NewStatus := 'PREMIUM';
        ChangeStatus := TRUE;
    END IF;
    -- Se il cliente ha cambiato STATUS
    IF (ChangeStatus = TRUE) THEN
        -- Aggiornare la tabella CARDS
        UPDATE CARDS
        SET STATUS = NewStatus
        WHERE CARDNO = :NEW.CARDNO;
        -- Ottenere il valore attuale di NOTIFYNO
        SELECT MAX(NOTIFYNO) INTO ID
        FROM NOTIFY
        WHERE CARDNO = :NEW.CARDNO;
        IF (ID IS NULL) THEN
            ID := 0;
        END IF;
        -- Inserire l'informazione necessaria nella tabella NOTIFY
        INSERT INTO NOTIFY(CARDNO, NOTIFYNO, NOTIFYDATE, OLDSTATUS, NEWSTATUS, TOTALMILES)
        VALUES (:NEW.CARDNO, ID + 1, SYSDATE, CurrentStatus, NewStatus, TotalMiles);
    END IF;
END;