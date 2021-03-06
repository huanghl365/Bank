#include "Tests.h"
#include "Database.h"

int DeleteClientTest(){	
	int id = 9999;	
	while (ClientExists(id, 0) != 0) id++;
	sqlite3_stmt *stmt;
	sqlite3_prepare(db, "INSERT INTO Client (CLIENTID, FIRSTNAME, LASTNAME) VALUES ((?), 'James', 'Blunt')", -1, &stmt, 0);
	sqlite3_bind_int(stmt, 1, id);
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	DeleteClientFromDB(id);
	if (!ClientExists(id, 0)) return SUCCESS;
	sqlite3_prepare(db, "DELETE FROM Client WHERE CLIENTID = (?)", -1, &stmt, 0);
	sqlite3_bind_int(stmt, 1, id);
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	return FAILURE;
}

int AddCardTest(){
	AddCardToDB(2, "Mr. Jack Michaelson", "01.12.2020", 537);
	if (CardExists(2)!=0)
	{
		DeleteCardFromDB(2);
		return SUCCESS;
	}
	else
	{
		return FAILURE;
	}
}

int GetClientTest(){
	Client testClient = GetClient(0);
	Client client;
	char query[300];
	sqlite3_stmt *statement;
	sprintf(query, "SELECT FirstName, LastName FROM Client WHERE ClientId = %d", 0);
	sqlite3_prepare(db, query, -1, &statement, 0);
	if (sqlite3_step(statement) == SQLITE_ROW) {
		client.Id = 0;
		strcpy(client.FirstName, sqlite3_column_text(statement, 0));
		strcpy(client.LastName, sqlite3_column_text(statement, 1));
	}
	sqlite3_finalize(statement);
	if (strcmp(client.FirstName, testClient.FirstName) == 0 && strcmp(client.LastName, testClient.LastName) == 0)
		return SUCCESS;
	return FAILURE;
}

int CreditTest(){
	int cardId = 0;
	float oldBalance, sum = 500;
	Account acc = GetAccountInfoByCardId(cardId);
	oldBalance = acc.Balance;
	CreditMoney(cardId, sum);
	acc = GetAccountInfoByCardId(cardId);
	CreditMoney(cardId, -sum);
	return acc.Balance == oldBalance + sum ? SUCCESS : FAILURE;
}

