#include "aispg.h"

char* aispg::pg_connect(char ipaddr[15])
{
    double		res_double;
    char*		result;
    result = new char[10];
    stringstream iostr;
    char        query_string[256];                      /* holds constructed SQL query */
    PGconn     *conn;                                   /* holds database connection */
    PGresult   *res;                                    /* holds query result */
    int         i;

    conn = PQconnectdb("host=172.16.0.2 port=5432 dbname=ais user=ais password='boctok'");

    if (PQstatus(conn) == CONNECTION_BAD)
    {
        fprintf(stderr, "Connection to database failed.\n");
        fprintf(stderr, "%s", PQerrorMessage(conn));
        exit(1);
    }

    sprintf(query_string, "SELECT cid FROM clife WHERE tid = 15 and value='%s/32'",ipaddr);
    res = PQexec(conn, query_string);

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "SELECT query failed.\n");
        PQclear(res);
        PQfinish(conn);
        exit(1);
    }

    result = PQgetvalue(res, PQntuples(res)-1, 0);

    sprintf(query_string, "SELECT acc FROM account WHERE cid = '%s'",result);

    res = PQexec(conn, query_string);

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "SELECT query failed.\n");
        PQclear(res);
        PQfinish(conn);
        exit(1);
    }

    result = PQgetvalue(res, 0, 0);

    res_double = atof(result);								//Переводим в число и округляем
    res_double = floor(res_double/0.01 + 0.5)*0.01;

    iostr<<res_double;										//Переводим обратно в строку
    iostr>>result;

    PQclear(res);
    PQfinish(conn);

    return result;
}
