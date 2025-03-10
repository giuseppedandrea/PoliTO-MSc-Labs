/*

Database:

CELL(CellId, x0, y0, x1, y1, CurrentPhone#, MaxCalls)
TELEPHONE(PhoneNo, x, y, PhoneState)
STATE-CHANGE(ChangeId, TimeStamp, PhoneNo, x, y, ChangeType)
EXCEPTION-LOG(ExId, CellId, ExceptionType)


*/




drop table CELL;
drop table TELEPHONE;
drop table STATE_CHANGE;
drop table EXCEPTION_LOG;



CREATE TABLE CELL(
	CellId INTEGER NOT NULL, 
	x0 DECIMAL(7,2)   NOT NULL, 
	y0 DECIMAL(7,2)   NOT NULL, 
	x1 DECIMAL(7,2)   NOT NULL, 
	y1 DECIMAL(7,2)   NOT NULL, 
	CurrentPhone# SMALLINT NOT NULL, 
	MaxCalls SMALLINT NOT NULL,
	PRIMARY KEY(CellId)
);



CREATE TABLE TELEPHONE(
	PhoneNo CHAR(10),
	x DECIMAL(7,2)   NOT NULL, 
	y DECIMAL(7,2)   NOT NULL, 
	PhoneState CHAR(6) NOT NULL,
	PRIMARY KEY(PhoneNo)
);





CREATE TABLE STATE_CHANGE(
	ChangeId INTEGER NOT NULL, 
	TimeStamp DATE NOT NULL, 
	PhoneNo CHAR(10), 
	x DECIMAL(7,2)   NOT NULL, 
	y DECIMAL(7,2)   NOT NULL, 
	ChangeType CHAR(1),
	PRIMARY KEY(ChangeId)
	
);





CREATE TABLE EXCEPTION_LOG(
	ExId INTEGER NOT NULL, 
	CellId INTEGER NOT NULL, 
	ExceptionType CHAR(1),
	PRIMARY KEY(ExId,CellId)
); 





INSERT INTO CELL(CellId, x0, y0, x1, y1, CurrentPhone#, MaxCalls)
       VALUES (1,0,0,10,10,0,3);

INSERT INTO CELL(CellId, x0, y0, x1, y1, CurrentPhone#, MaxCalls)
       VALUES (2,10,0,20,10,0,20);

INSERT INTO CELL(CellId, x0, y0, x1, y1, CurrentPhone#, MaxCalls)
       VALUES (3,0,10,10,20,0,20);

INSERT INTO CELL(CellId, x0, y0, x1, y1, CurrentPhone#, MaxCalls)
       VALUES (4,10,10,20,20,0,20);



 

