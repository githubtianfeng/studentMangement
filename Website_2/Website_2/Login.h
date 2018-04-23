#pragma once
#include <stdio.h>
#include "share.h"
#include <windows.h>

//实现登录系统
int cgiMain01()
{
	//判断用户是否点击登录按钮
	if (cgiFormSubmitClicked("btnLogin") == cgiFormSuccess)
	{
		char username[256] = { 0 };
		char password[256] = { 0 };
		//判断获取username参数是否成功
		if (cgiFormString("username", username, sizeof(username)) != cgiFormSuccess)
		{
			showError("用户名不能为空\n");
			return 0;
		}
		//判断获取password参数是否成功
		if (cgiFormString("password", password, sizeof(password)) != cgiFormSuccess)
		{
			showError("密码不能为空\n");
			return 0;
		}

		/*
		char sql[1024] = { 0 };
		//存在SQL注入漏洞  密码为：1'or'1'='1    对应到password='%s'为：password='1'or'1'='1'
		sprintf(sql, "select count(*) from T_users where username='%s' and password='%s'", username, password);
		MYSQL_RES* result = executeQuery(sql);
		MYSQL_ROW row = mysql_fetch_row(result);
		char* count = row[0];

		if (strcmp(count, "0") == 0)
		{
			showError("用户名或密码错误\n");
		}
		else
		{
			showError("登录成功\n");
		}  */

		char sql[1024] = { 0 };
		//采用16进制生成形式，避免SQL注入漏洞   在.net,java等中采用参数化查询避免产生注入漏洞
		char hexUsername[1024] = { 0 };
		char hexPassword[1024] = { 0 };
		//产生16进制的 存放到hexUsername和hexPassword中
		mysql_hex_string(hexUsername, username, strlen(username));
		mysql_hex_string(hexPassword, password, strlen(password));
		//查询mysql数据库中是否存在username=用户password=密码的
		sprintf(sql, "select count(*) from T_users where username=0x%s and password=0x%s", hexUsername, hexPassword);
		MYSQL_RES* result = executeQuery(sql);		//生产结果集
		MYSQL_ROW row = mysql_fetch_row(result);	//定义一行
		char* count = row[0];					//将获取的第一行 放到count中
		mysql_free_result(result);			//释放结果集
		if (strcmp(count, "0") == 0)		//判断是否有密码，
		{
			showError("用户名或密码错误\n");
		}
		else			//若用户名 密码正确
		{
			//那么判断用户进入学生模块还是老师模块
			char mangement[20] = { 0 };
			if (cgiFormString("mangement", mangement, sizeof(mangement)) == cgiFormSuccess)
			{
				char teachers[20] = { 0 };
				char students[20] = { 0 };
				TMPL_varlist* varlist = 0;
				char buf[20] = { 0 };
				//老师模块
				if (strcmp(mangement, "teachers") == 0)
				{
					//从数据库查询成功
					//	showError("登录成功\n");
					/*
					{
						sprintf(buf, "teacher");
						varlist = TMPL_add_var(varlist, "choose", "teacher", 0);
						cgiHeaderContentType("text/html;charset=gbk");
						TMPL_write("Mysql_cgi_Login_html.htm", 0, 0, varlist, cgiOut, cgiOut);
					} */
					cgiHeaderCookieSetString("mod", "teacher", 20, "", "");
					cgiHeaderCookieSetString("action", "list", 20, "", "");
					cgiHeaderLocation("Website_2.cgi?mod=teacher&action=list");
					return 0;
				}
				//学生模块
				if (strcmp(mangement,"students")==0)
				{
					//从数据库查询成功
				/*	{
						sprintf(buf, "student");
						varlist = TMPL_add_var(varlist, "choose", buf, 0);
						cgiHeaderContentType("text/html;charset=gbk");
						TMPL_write("Mysql_cgi_Login_html.htm", 0, 0, varlist, cgiOut, cgiOut);
					}*/
					cgiHeaderCookieSetString("mod", "student", 20, "", "");
					cgiHeaderCookieSetString("action", "list", 20, "", "");
					cgiHeaderLocation("Website_2.cgi?mod=student&action=list");
					return 0;
				}
				else
				{

				}
			}
		}
	}
	else		//若用户没有点击登录按钮，那么就一直显示Mysql_cgi_Login_html.htm页面，等待用户输入即可
	{
		cgiHeaderContentType("text/html;charset=gbk");
		TMPL_write("Mysql_cgi_Login_html.htm", 0, 0, 0, cgiOut, cgiOut);
		strcpy()
	}
	return 0;
}