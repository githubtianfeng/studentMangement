#pragma once
#include <stdio.h>
#include "share.h"
#include <windows.h>

//ʵ�ֵ�¼ϵͳ
int cgiMain01()
{
	//�ж��û��Ƿ�����¼��ť
	if (cgiFormSubmitClicked("btnLogin") == cgiFormSuccess)
	{
		char username[256] = { 0 };
		char password[256] = { 0 };
		//�жϻ�ȡusername�����Ƿ�ɹ�
		if (cgiFormString("username", username, sizeof(username)) != cgiFormSuccess)
		{
			showError("�û�������Ϊ��\n");
			return 0;
		}
		//�жϻ�ȡpassword�����Ƿ�ɹ�
		if (cgiFormString("password", password, sizeof(password)) != cgiFormSuccess)
		{
			showError("���벻��Ϊ��\n");
			return 0;
		}

		/*
		char sql[1024] = { 0 };
		//����SQLע��©��  ����Ϊ��1'or'1'='1    ��Ӧ��password='%s'Ϊ��password='1'or'1'='1'
		sprintf(sql, "select count(*) from T_users where username='%s' and password='%s'", username, password);
		MYSQL_RES* result = executeQuery(sql);
		MYSQL_ROW row = mysql_fetch_row(result);
		char* count = row[0];

		if (strcmp(count, "0") == 0)
		{
			showError("�û������������\n");
		}
		else
		{
			showError("��¼�ɹ�\n");
		}  */

		char sql[1024] = { 0 };
		//����16����������ʽ������SQLע��©��   ��.net,java���в��ò�������ѯ�������ע��©��
		char hexUsername[1024] = { 0 };
		char hexPassword[1024] = { 0 };
		//����16���Ƶ� ��ŵ�hexUsername��hexPassword��
		mysql_hex_string(hexUsername, username, strlen(username));
		mysql_hex_string(hexPassword, password, strlen(password));
		//��ѯmysql���ݿ����Ƿ����username=�û�password=�����
		sprintf(sql, "select count(*) from T_users where username=0x%s and password=0x%s", hexUsername, hexPassword);
		MYSQL_RES* result = executeQuery(sql);		//���������
		MYSQL_ROW row = mysql_fetch_row(result);	//����һ��
		char* count = row[0];					//����ȡ�ĵ�һ�� �ŵ�count��
		mysql_free_result(result);			//�ͷŽ����
		if (strcmp(count, "0") == 0)		//�ж��Ƿ������룬
		{
			showError("�û������������\n");
		}
		else			//���û��� ������ȷ
		{
			//��ô�ж��û�����ѧ��ģ�黹����ʦģ��
			char mangement[20] = { 0 };
			if (cgiFormString("mangement", mangement, sizeof(mangement)) == cgiFormSuccess)
			{
				char teachers[20] = { 0 };
				char students[20] = { 0 };
				TMPL_varlist* varlist = 0;
				char buf[20] = { 0 };
				//��ʦģ��
				if (strcmp(mangement, "teachers") == 0)
				{
					//�����ݿ��ѯ�ɹ�
					//	showError("��¼�ɹ�\n");
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
				//ѧ��ģ��
				if (strcmp(mangement,"students")==0)
				{
					//�����ݿ��ѯ�ɹ�
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
	else		//���û�û�е����¼��ť����ô��һֱ��ʾMysql_cgi_Login_html.htmҳ�棬�ȴ��û����뼴��
	{
		cgiHeaderContentType("text/html;charset=gbk");
		TMPL_write("Mysql_cgi_Login_html.htm", 0, 0, 0, cgiOut, cgiOut);
		strcpy()
	}
	return 0;
}