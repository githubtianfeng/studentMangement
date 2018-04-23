#pragma once

#include "cgic.h"		//cgi头文件，用来调用cgi库函数
#include "ctemplate.h"	//ctemplate头文件,用来调用模板引擎
#include <winsock.h>	//这是调用<mysql.h>里面的函数的必须头文件
#include <mysql.h>		//<mysql.h> 用来调用mysql.h里的库函数，实现对mysql数据库的操作

#include <objbase.h>

//非查询语句的函数
void executeNonQuery(char* sql)
{
	MYSQL* pConn = mysql_init(0);
	//判断连接数据库是否成功
	if (!mysql_real_connect(pConn, "localhost", "root", "root", "mangement", 3306, 0, 0))
	{
		goto error;
	}
	//将数据库编码改为gbk编码
	if (mysql_query(pConn, "set names gbk;"))
	{
		goto error;
	}
	//执行sql语句
	if (mysql_query(pConn, sql))
	{
		goto error;
	}
	goto exit;
error:
	cgiHeaderContentType("text/html;charset=gbk");
	fprintf(cgiOut, "出错：%s\n", mysql_error(pConn)); 
	//	printf("出错：%s\n", mysql_error(pConn));
exit:
	mysql_close(pConn);  //关闭数据库
}

MYSQL_RES* executeQuery(char * sql)
{
	MYSQL* pConn = mysql_init(0);
	//连接数据库是否成功
	if (!mysql_real_connect(pConn, "localhost", "root", "root", "mangement", 3306, 0, 0))
	{
		goto error;
	}
	//将数据库编码改为gbk编码
	if (mysql_query(pConn, "set names gbk"))
	{
		goto error;
	}
	//执行sql查询语句
	if (mysql_query(pConn, sql))
	{
		goto error;
	}
	//获取离线结果，数据是放到程序内存中的
	MYSQL_RES* result = mysql_store_result(pConn);
	mysql_close(pConn);		//关闭数据库
	return result;		//返回结果集
error:
	cgiHeaderContentType("text/html;charset=gbk");
	fprintf(cgiOut, "出错：%s\n", mysql_error(pConn));
	//	printf("出错：%s\n", mysql_error(pConn));
exit:
	mysql_close(pConn);  //关闭数据库
}

//错误消息  统一由Error页面显示
void showError(char* msg)
{
	TMPL_varlist* varlist = 0;
	varlist = TMPL_add_var(varlist, "msg", msg, 0);
	cgiHeaderContentType("text/html;charset=gbk");
	TMPL_write("Error.html", 0, 0, varlist, cgiOut, cgiOut);
}

//下拉列表  
void printSelectItems(char* buffer, char* sql, char* selectedValue)
{
	//生产结果集
	MYSQL_RES* result = executeQuery(sql);
	MYSQL_ROW row;
	while (row = mysql_fetch_row(result))
	{
		//要求第一列为Id,第二列为Name
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
		buffer += len;	//移动指针
	}
	mysql_free_result(result);		
}

//GUID算法代码：
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

//设置当前登陆用户Id
void setSessionUserId(int userid)
{
	BOOL needCreate = FALSE;		//是需要创建一个session记录还是更新session记录
	//Cookie中的sessionid
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

//获取当前用户Id,返回值就是用户Id,如果没有用户登录则返回-1
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
		else    //如果数据库中没有sessionid对应的记录，则也认为没有登陆
		{
			return -1;
		}
	}
	else	//如果cookie中没有sessionid,则也认为没有登录
	{
		return -1;
	}
}