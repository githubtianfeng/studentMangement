#pragma once

#include "cgic.h"		//cgiͷ�ļ�����������cgi�⺯��
#include "ctemplate.h"	//ctemplateͷ�ļ�,��������ģ������
#include <winsock.h>	//���ǵ���<mysql.h>����ĺ����ı���ͷ�ļ�
#include <mysql.h>		//<mysql.h> ��������mysql.h��Ŀ⺯����ʵ�ֶ�mysql���ݿ�Ĳ���

#include <objbase.h>

//�ǲ�ѯ���ĺ���
void executeNonQuery(char* sql)
{
	MYSQL* pConn = mysql_init(0);
	//�ж��������ݿ��Ƿ�ɹ�
	if (!mysql_real_connect(pConn, "localhost", "root", "root", "mangement", 3306, 0, 0))
	{
		goto error;
	}
	//�����ݿ�����Ϊgbk����
	if (mysql_query(pConn, "set names gbk;"))
	{
		goto error;
	}
	//ִ��sql���
	if (mysql_query(pConn, sql))
	{
		goto error;
	}
	goto exit;
error:
	cgiHeaderContentType("text/html;charset=gbk");
	fprintf(cgiOut, "����%s\n", mysql_error(pConn)); 
	//	printf("����%s\n", mysql_error(pConn));
exit:
	mysql_close(pConn);  //�ر����ݿ�
}

MYSQL_RES* executeQuery(char * sql)
{
	MYSQL* pConn = mysql_init(0);
	//�������ݿ��Ƿ�ɹ�
	if (!mysql_real_connect(pConn, "localhost", "root", "root", "mangement", 3306, 0, 0))
	{
		goto error;
	}
	//�����ݿ�����Ϊgbk����
	if (mysql_query(pConn, "set names gbk"))
	{
		goto error;
	}
	//ִ��sql��ѯ���
	if (mysql_query(pConn, sql))
	{
		goto error;
	}
	//��ȡ���߽���������Ƿŵ������ڴ��е�
	MYSQL_RES* result = mysql_store_result(pConn);
	mysql_close(pConn);		//�ر����ݿ�
	return result;		//���ؽ����
error:
	cgiHeaderContentType("text/html;charset=gbk");
	fprintf(cgiOut, "����%s\n", mysql_error(pConn));
	//	printf("����%s\n", mysql_error(pConn));
exit:
	mysql_close(pConn);  //�ر����ݿ�
}

//������Ϣ  ͳһ��Errorҳ����ʾ
void showError(char* msg)
{
	TMPL_varlist* varlist = 0;
	varlist = TMPL_add_var(varlist, "msg", msg, 0);
	cgiHeaderContentType("text/html;charset=gbk");
	TMPL_write("Error.html", 0, 0, varlist, cgiOut, cgiOut);
}

//�����б�  
void printSelectItems(char* buffer, char* sql, char* selectedValue)
{
	//���������
	MYSQL_RES* result = executeQuery(sql);
	MYSQL_ROW row;
	while (row = mysql_fetch_row(result))
	{
		//Ҫ���һ��ΪId,�ڶ���ΪName
		char* id = row[0];
		char* name = row[1];
		int len;
		if (strcmp(id, selectedValue) == 0)
		{
			len = sprintf(buffer, "<option value='%s' selected>%s</option>", id, name);
		}
		else
		{
			len = sprintf(buffer, "<option value='%s'>%s</option>", id, name);
		}
		buffer += len;	//�ƶ�ָ��
	}
	mysql_free_result(result);		
}

//GUID�㷨���룺
void createGuid(char* guidStr)
{
	GUID guid = { 0 };
	CoCreateGuid(&guid);
	sprintf(guidStr, "{%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X}",
		guid.Data1,
		guid.Data2,
		guid.Data3,
		guid.Data4[0],
		guid.Data4[1],
		guid.Data4[2],
		guid.Data4[3],
		guid.Data4[4],
		guid.Data4[5],
		guid.Data4[6],
		guid.Data4[7]);
}

//���õ�ǰ��½�û�Id
void setSessionUserId(int userid)
{
	BOOL needCreate = FALSE;		//����Ҫ����һ��session��¼���Ǹ���session��¼
	//Cookie�е�sessionid
	char sessionId[50] = { 0 };
	if (cgiCookieString("SessionId", sessionId, sizeof(sessionId)) == cgiFormSuccess)
	{
		char sqlSelect[256] = { 0 };
		MYSQL_RES* result;
		sprintf(sqlSelect, "select * from T_sessions where Id='%s'", sessionId);
		result = executeQuery(sqlSelect);
		if (mysql_fetch_row(result))
		{
			char sqlUpdate[256] = { 0 };
			sprintf(sqlUpdate, "update T_sessions set UserId=%d where Id='%s'", userid, sessionId);
			executeNonQuery(sqlUpdate);
		}
		else
		{
			needCreate = TRUE;
		}
		mysql_free_result(result);
	}
	else
	{
		needCreate = TRUE;
	}
	if (needCreate)
	{
		char sqlInsert[256] = { 0 };
		createGuid(sessionId);
		cgiHeaderCookieSetString("SessionId", sessionId, 60 * 60, "", "127.0.0.1");
		sprintf(sqlInsert, "insert into T_sessions(Id,UserId) values('%s',%d)", sessionId, userid);
		executeNonQuery(sqlInsert);
	}
}

//��ȡ��ǰ�û�Id,����ֵ�����û�Id,���û���û���¼�򷵻�-1
int getSessionUserId()
{
	char sessionId[50] = { 0 };
	if (cgiCookieString("SessionId", sessionId, sizeof(sessionId)) == cgiFormSuccess)
	{
		char sqlSelect[256] = { 0 };
		sprintf(sqlSelect, "select UserId from T_sessions where Id='%s'", sessionId);
		MYSQL_RES* result = executeQuery(sqlSelect);
		MYSQL_ROW row = mysql_fetch_row(result);
		if (row)
		{
			return atoi(row[0]);
		}
		else    //������ݿ���û��sessionid��Ӧ�ļ�¼����Ҳ��Ϊû�е�½
		{
			return -1;
		}
	}
	else	//���cookie��û��sessionid,��Ҳ��Ϊû�е�¼
	{
		return -1;
	}
}