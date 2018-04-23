#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "share.h"
#include <io.h>
#include <windows.h>


//����ʹ��cookie  ����ʹ�����Ļ������⣬���Ծ������û���ΪӢ��

//mod����ȷ�����ĸ�ģ�飬action����ȷ����ʲô������
//�б�List������addnew����������addnewSubmit��
//�޸�edit���޸ı���editSubmit

/*
StudentMgr.cgi?mod=teacher&action=list  �г�������ʦ������
StudentMgr.cgi?mod=teacher&action=addnew ����������ʦģ��
StudentMgr.cgi?mod=teacher&action=addnewSubmit ������ʦ���涯��
StudentMgr.cgi?mod=student&action=list		�г�����ѧ������
StudentMgr.cgi?mod=user&action=login		�û���½
*/


//ʵ��ѧ��ģ��ͽ�ʦģ��
int cgiMain()
{
	char mod[32] = { 0 };
	char action[32] = { 0 };
	//������cgi�������ٴ���������
	//StudentMgr.cgi?mod=teacher&action=list  �г�������ʦ������
	
	if (cgiFormString("mod", mod, sizeof(mod)) != cgiFormSuccess)
	{
		showError("û���ṩmod����");
		return 0;
	}
	if (cgiFormString("action", action, sizeof(action)) != cgiFormSuccess)
	{
		showError("û���ṩaction����");
		return 0;
	}
	/*
	  */
	//�����mainģ��
	if (strcmp(mod, "main") == 0)
	{
		//������home
		if (strcmp(action, "home") == 0)
		{
			int userid = getSessionUserId();
			char sqlSelect[256] = { 0 };
			MYSQL_RES* result;
			MYSQL_ROW row;
			if (userid == -1)
			{
				showError("�����¼���ܷ��ʣ�");
				return;
			}
			
			char buf1[12] = { 0 };
			cgiCookieString("mod", buf1, sizeof(buf1));		//����cookies  ȷ���û����
			TMPL_varlist* varlist = 0;
			
			if (strcmp(buf1, "student") == 0)
			{
				sprintf(sqlSelect, "select username from T_studentusers where Id=%d", userid);
			}
			else if (strcmp(buf1, "teacher") == 0)
			{
				sprintf(sqlSelect, "select username from T_teacherusers where Id=%d", userid);
			}
			else if (strcmp(buf1, "admin") == 0)
			{
				sprintf(sqlSelect, "select username from T_adminusers where Id=%d", userid);
			}
			result = executeQuery(sqlSelect);
			row = mysql_fetch_row(result);

			if (row)
			{
				char* username = row[0];
				if (strcmp(buf1, "teacher") == 0 || strcmp(buf1, "student") == 0)
				{
					varlist = TMPL_add_var(varlist, "Name", buf1, "UserName", username, 0);
					cgiHeaderContentType("text/html;charset=gbk");
					TMPL_write("Main.htm", 0, 0, varlist, cgiOut, cgiOut);
				}
				//����Ȳ��ǽ�ʦģ�飬Ҳ����ѧ��ģ��,�Ǿ��ǹ���Աģ��
				else
				{
					cgiHeaderContentType("text/html;charset=gbk");
					TMPL_write("Main_admin.htm", 0, 0, 0, cgiOut, cgiOut);
				}
			}
			else
			{
				showError("����UserIdû���ҵ��û���Ϣ��");
				return;
			}
			mysql_free_result(result);
		}
		else if (strcmp(action, "adminSubmit")==0)
		{
			char username[50] = { 0 };
			cgiCookieString("UserName", username, sizeof(username));

			//��ȡid�ж���student����teacher��������coookie

			TMPL_varlist* varlist = 0;
			char choose[20] = { 0 };
			cgiFormString("choose", choose, sizeof(choose));		//��ȡMain_admin�ĵ�ѡ��ť����
			varlist = TMPL_add_var(varlist, "Name", choose, "UserName", username, 0);		//����ȡ�Ĳ�������Main_admin��ģ����Ĳ���
			//���ﴫ��������ν
			cgiHeaderCookieSetString("mod", choose, 200, "", "127.0.0.1");	//��ѡ�еĲ�������mod����
			cgiHeaderContentType("text/html;charset=gbk");
			TMPL_write("Main.htm", 0, 0, varlist, cgiOut, cgiOut);
		}
		else if (strcmp(action, "login") == 0)
		{
			cgiHeaderCookieSetString("mod_admin", "", 5, "", "127.0.0.1");		//�Թ���Ա��ݲ�ѯ���һ����mod_admin�ÿգ���������ֵһֱ��admin�����鷳�ˣ�����
	//		MessageBoxA(0, "", "", 0);
			//ʹ��cookie�����û���
			char username[50] = { 0 };
			cgiCookieString("UserName", username, sizeof(username));
			TMPL_varlist* varlist = 0;
			varlist = TMPL_add_var(varlist, "UserName", username, 0);
			cgiHeaderContentType("text/html;charset=gbk");
			TMPL_write("Login.htm", 0, 0, varlist, cgiOut, cgiOut);
		}

		else if (strcmp(action, "loginSubmit") == 0)
		{
         /* cgiHeaderCookieSetString("mod_admin", "", 5, "", "127.0.0.1");		//�Թ���Ա��ݲ�ѯ���һ����mod_admin�ÿգ���������ֵһֱ��admin�����鷳�ˣ�����
			
			char username[256] = { 0 };
			char password[256] = { 0 };
			//�жϻ�ȡusername�����Ƿ�ɹ�
			if (cgiFormString("username", username, sizeof(username)) != cgiFormSuccess)
			{
				showError("�û�������Ϊ��\n");
			//	TMPL_varlist* varlist = 0;
			//	varlist = TMPL_add_var(varlist, "msg", "�û���������д", 0);
				return 0;
			}
			//�жϻ�ȡpassword�����Ƿ�ɹ�
			if (cgiFormString("password", password, sizeof(password)) != cgiFormSuccess)
			{
				showError("���벻��Ϊ��\n");
				return 0;
			} */
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

	       /* char sql[1024] = { 0 };
			//����16����������ʽ������SQLע��©��   ��.net,java���в��ò�������ѯ�������ע��©��
			char hexUsername[1024] = { 0 };
			char hexPassword[1024] = { 0 };
			//����16���Ƶ� ��ŵ�hexUsername��hexPassword��
			mysql_hex_string(hexUsername, username, strlen(username));
			mysql_hex_string(hexPassword, password, strlen(password));
			//��ѯmysql���ݿ����Ƿ����username=�û�password=�����
			sprintf(sql, "select Id,username,password from T_users where username=0x%s and password=0x%s", hexUsername, hexPassword);
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
				cgiHeaderCookieSetString("UserName", username, 60 * 60 * 24, "", "127.0.0.1");		//����kookie
				setSessionUserId(atoi(count));
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
						cgiHeaderCookieSetString("mod", "teacher", 200, "", "127.0.0.1");		//����cookie �������û����teacher
				//		cgiHeaderCookieSetString("Username", username, 3000, "", "127.0.0.1");	//����kookie ���ĸ��û�
						cgiHeaderLocation("Website_2.cgi?mod=main&action=home");
						return 0;
					}
					//ѧ��ģ��
					if (strcmp(mangement, "students") == 0)
					{
						//�����ݿ��ѯ�ɹ�
						cgiHeaderCookieSetString("mod", "student", 200, "", "127.0.0.1");			//����cookie �������û����student
					//	cgiHeaderCookieSetString("Username", hexUsername, 2000, "", "127.0.0.1");		//����kookie ���ĸ��û�
						cgiHeaderLocation("Website_2.cgi?mod=main&action=home");	
						return 0;
					}
					//����Աģ��
					else if (strcmp(mangement,"admin") ==0 )
					{
						cgiHeaderCookieSetString("mod", "admin", 200, "", "127.0.0.1");
						cgiHeaderCookieSetString("mod_admin", "admin", 200, "", "127.0.0.1");		//����ǹ���Ա������cookie
						cgiHeaderLocation("Website_2.cgi?mod=main&action=home");
						return 0;
					}
					else
					{

					}
				}
			} */
		//	MessageBoxA(0, "", "", 0);
			cgiHeaderCookieSetString("mod_admin", "", 5, "", "127.0.0.1");		//�Թ���Ա��ݲ�ѯ���һ����mod_admin�ÿգ���������ֵһֱ��admin�����鷳�ˣ�����
			char username[256] = { 0 };
			char password[256] = { 0 };
			//�жϻ�ȡusername�����Ƿ�ɹ�
			if (cgiFormString("username", username, sizeof(username)) != cgiFormSuccess)
			{
				showError("�û�������Ϊ��\n");
				//	TMPL_varlist* varlist = 0;
				//	varlist = TMPL_add_var(varlist, "msg", "�û���������д", 0);
				return 0;
			}
			//�жϻ�ȡpassword�����Ƿ�ɹ�
			if (cgiFormString("password", password, sizeof(password)) != cgiFormSuccess)
			{
				showError("���벻��Ϊ��\n");
				return 0;
			} 
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
					cgiHeaderCookieSetString("mod", "teacher", 200, "", "127.0.0.1");		//����cookie �������û����teacher
					//cgiHeaderCookieSetString("Username", username, 3000, "", "127.0.0.1");	//����kookie ���ĸ��û�
				//	cgiHeaderLocation("Website_2.cgi?mod=main&action=home");
				//	return 0;
				}
				//ѧ��ģ��
				if (strcmp(mangement, "students") == 0)
				{
					//�����ݿ��ѯ�ɹ�
					cgiHeaderCookieSetString("mod", "student", 200, "", "127.0.0.1");			//����cookie �������û����student
					//cgiHeaderCookieSetString("Username", hexUsername, 2000, "", "127.0.0.1");		//����kookie ���ĸ��û�
					//	cgiHeaderLocation("Website_2.cgi?mod=main&action=home");
					//	return 0;
				}
				//����Աģ��
				else if (strcmp(mangement, "admin") == 0)
				{
					cgiHeaderCookieSetString("mod", "admin", 200, "", "127.0.0.1");
					cgiHeaderCookieSetString("mod_admin", "admin", 200, "", "127.0.0.1");	//����ǹ���Ա������cookie
				//	cgiHeaderLocation("Website_2.cgi?mod=main&action=home");
				//	return 0;
				}
				else
				{

				}
			}
			else
			{

			}
			char sql[1024] = { 0 };
			//����16����������ʽ������SQLע��©��   ��.net,java���в��ò�������ѯ�������ע��©��
			char hexUsername[1024] = { 0 };
			char hexPassword[1024] = { 0 };
			//����16���Ƶ� ��ŵ�hexUsername��hexPassword��
			mysql_hex_string(hexUsername, username, strlen(username));
			mysql_hex_string(hexPassword, password, strlen(password));

			char mod[10] = { 0 };
			cgiCookieString("mod", mod, sizeof(mod));
			char user_table[30] = { 0 };
			if (strcmp(mod, "teacher") == 0 || strcmp(mangement,"teachers")==0)
			{
				sprintf(user_table, "T_teacherUsers");
				//��ѯmysql���ݿ����Ƿ����username=�û�password=�����
				sprintf(sql, "select count(*),id from %s where username=0x%s and password=0x%s",user_table, hexUsername, hexPassword);
			}
			else if (strcmp(mod, "student") == 0 || strcmp(mangement, "students") == 0)
			{
				sprintf(user_table, "T_studentUsers");
				//��ѯmysql���ݿ����Ƿ����username=�û�password=�����
				sprintf(sql, "select count(*),id from %s where username=0x%s and password=0x%s", user_table, hexUsername, hexPassword);
			}
			else if (strcmp(mod, "admin") == 0 || strcmp(mangement, "admin") == 0)
			{
				sprintf(user_table, "T_adminUsers");
				//��ѯmysql���ݿ����Ƿ����username=�û�password=�����
				sprintf(sql, "select count(*),id from %s where username=0x%s and password=0x%s", user_table, hexUsername, hexPassword);
			}
			else

			{

			}	
			MYSQL_RES* result = executeQuery(sql);		//���������
			MYSQL_ROW row = mysql_fetch_row(result);	//����һ��
			char* count = row[0];					//����ȡ�ĵ�һ�� �ŵ�count��
			char *id = row[1];
			mysql_free_result(result);			//�ͷŽ����

			if (strcmp(count, "0") == 0)		//���û������
			{
				showError("�û������������\n");
			}
			else	//˵�������룬��ֱ���ض���mod=main&action=home��Ӧ�Ľ���
			{
				//�����ϴγɹ���½����û���
				cgiHeaderCookieSetString("UserName", username, 60 * 60 * 24, "", "127.0.0.1");		//����kookie
				setSessionUserId(atoi(row[1]));
				cgiHeaderLocation("Website_2.cgi?mod=main&action=home");	//
			} 
		} 
	}
	//���mod������teacher��ô������ʦģ��
	else if (strcmp(mod, "teacher") == 0)
	{	
		//���action������list����ô�г�������ʦ������
		if (strcmp(action, "list") == 0)
		{
			char mod_admin[10] = { 0 };
			char sql[256] = { 0 };
			cgiCookieString("mod_admin", mod_admin, sizeof(mod_admin));	
			if (strcmp(mod_admin, "admin") != 0)	//�����½�߲��ǹ���Ա
			{
				char username[100] = { 0 };
				cgiCookieString("UserName", username, sizeof(username));
				//���������	��T_teachers���в�ѯ��������
				sprintf(sql, "select Id,Name,PhoneNum from T_teachers where Name='%s'", username);
			}
			else
			{
				sprintf(sql, "select Id,Name,PhoneNum from T_teachers");
		//		cgiHeaderCookieSetString("mod_admin", "", 5, "", "127.0.0.1");		//�Թ���Ա��ݲ�ѯ���һ����mod_admin�ÿգ���������ֵһֱ��admin�����鷳�ˣ�����
			}
			MYSQL_RES* result = executeQuery(sql);
			TMPL_varlist* varlist = 0;
			TMPL_loop* loopTeachers = 0;
			MYSQL_ROW row;		//����һ��
			while (row = mysql_fetch_row(result))		//��ȡ��������һ��һ�е�ȡ��
			{
				char* id = row[0];			//��һ�� id
				char* name = row[1];		//�ڶ��� Name
				char* phoneNum = row[2];	//������ PhoneNum
				loopTeachers = TMPL_add_varlist(loopTeachers, TMPL_add_var(0, "Id", id, "Name", name, "PhoneNum", phoneNum, 0));
			}
			varlist = TMPL_add_loop(varlist, "teachers", loopTeachers);
			cgiHeaderContentType("text/html;charset=gbk");
			TMPL_write("TeacherList.htm", 0, 0, varlist, cgiOut, cgiOut);
			mysql_free_result(result);		//�ͷŽ����
		}
		//�ж��Ƿ�Ϊ������ť
		else if (strcmp(action, "addnew") == 0)
		{
			char mod_admin[10] = { 0 };
			cgiCookieString("mod_admin", mod_admin, sizeof(mod_admin));
			if (strcmp(mod_admin, "admin") != 0)
			{
				showError("�Բ�����û��Ȩ������û�");
				return 0;
			}	
			//����ǣ���ô��ʾTeacherAddNew.htmҳ��
			cgiHeaderContentType("text/html;charset=gbk");
			TMPL_write("TeacherAddNew.htm", 0, 0, 0, cgiOut, cgiOut);
		}
		//�ж��Ƿ�ΪaddnewSubmit������ʦ���涯��
		else if (strcmp(action, "addnewSubmit") == 0)
		{
			char name[256] = { 0 };
			char phoneNum[256] = { 0 };
			//�ж�Name�Ƿ�Ϊ��
			if (cgiFormString("Name", name, sizeof(name)) != cgiFormSuccess)
			{
				showError("��������Ϊ��");
				return 0;
			}
			//�ж�PhoneNum�Ƿ�Ϊ��
			if (cgiFormString("PhoneNum", phoneNum, sizeof(phoneNum)) != cgiFormSuccess)
			{
				showError("�ֻ��Ų���Ϊ��");
				return 0;
			}
			char sql[1024] = { 0 };
			//ִ��sql������䣬����Ϣ�������ݿ��е�T_teachers��
			sprintf(sql, "insert into T_teachers(Name,PhoneNum) values('%s','%s')", name, phoneNum);
			executeNonQuery(sql);
			//�ض�λ��mod=teacher&action=list������ʦģ�鲢�г����Խ�ʦ��Ϣ
			cgiHeaderLocation("Website_2.cgi?mod=teacher&action=list");
		}
		//�ж��Ƿ�Ϊ�޸Ķ���
		else if (strcmp(action, "edit") == 0)
		{
			int id;
			//��ȡҪ�޸ĵ�id
			if (cgiFormInteger("id", &id, 0) != cgiFormSuccess)
			{
				showError("���ṩҪ�޸ĵ�id");
				return 0;
			}
			//���ؾɵ�����
			char sql[1024] = { 0 };
			//���������  �����ݿ��в�ѯId=Ҫ�޸ĵ�id��
			sprintf(sql, "select Id,Name,PhoneNum from T_teachers where Id=%d",id);
			MYSQL_RES* result = executeQuery(sql);
			MYSQL_ROW row;
			if (row = mysql_fetch_row(result))  //��һ��	�ж��Ƿ���ڸ�Id
			{
				char* name = row[1];
				char* phoneNum = row[2];
				TMPL_varlist* varlist = 0;
				varlist = TMPL_add_var(varlist, "Id", row[0], "Name", name, "PhoneNum", phoneNum, 0);
				cgiHeaderContentType("text/html;charset=gbk");
				//��ʾTeacherEdit.htmҳ�棬�ȴ��û��޸�
				TMPL_write("TeacherEdit.htm", 0, 0, varlist, cgiOut, cgiOut);
			}
			else
			{
				showError("�Ҳ������id��Ӧ�Ľ�ʦ��Ϣ");
				return 0;
			}
			mysql_free_result(result);
		}
		//�ж��Ƿ�ΪeditSubmit�޸ı��涯��
		else if (strcmp(action, "editSubmit") == 0)
		{
			int id;
			char name[256] = { 0 };
			char phoneNum[256] = { 0 };
			if (cgiFormInteger("Id", &id, 0) != cgiFormSuccess)
			{
				showError("Id����Ϊ��");
				return 0;
			}
			//�ж�Name�Ƿ�Ϊ��
			if (cgiFormString("Name", name, sizeof(name)) != cgiFormSuccess)
			{
				showError("Name����Ϊ��");
				return 0;
			}
			//�ж�PhoneNum�Ƿ��
			if (cgiFormString("PhoneNum", phoneNum, sizeof(phoneNum)) != cgiFormSuccess)
			{
				showError("PhoneNum����Ϊ��");
				return 0;
			}
			char sql[1024] = { 0 };
			//����T_teachers��
			sprintf(sql, "update T_teachers set Name='%s',PhoneNum='%s' where Id=%d", name, phoneNum,id);
			executeNonQuery(sql);
			//�ض�λ��mod=teacher&action=list������ʦģ�鲢�г����Խ�ʦ��Ϣ
			cgiHeaderLocation("Website_2.cgi?mod=teacher&action=list");
		}
		//�ж��Ƿ�Ϊɾ�����������û����ɾ����ť
		else if (strcmp(action, "delete") == 0)
		{
			char mod_admin[10] = { 0 };
			cgiCookieString("mod_admin", mod_admin, sizeof(mod_admin));
			if (strcmp(mod_admin, "admin") != 0)
			{
				showError("�Բ�����û��ɾ��Ȩ��");
				return 0;
			}
			int id;
			//��ȡҪɾ����id
			if (cgiFormInteger("id", &id, 0) != cgiFormSuccess)
			{	
				showError("Id����Ϊ��");
				return 0;
			}

			char sql1[128] = { 0 };
			char delSql[128] = { 0 };
			//���������  �����ݿ��в�ѯId=Ҫ�޸ĵ�id��
			
			sprintf(sql1, "select Name from T_teachers where Id=%d", id);
			MYSQL_RES* result = executeQuery(sql1);
			MYSQL_ROW row;
			row = mysql_fetch_row(result);  //��һ��	�ж��Ƿ���ڸ�Id
			char* name = row[0];		//��ȡҪɾ������ѧ�����ǽ�ʦ
			sprintf(delSql, "delete from T_teacherusers where username='%s'", name);
			executeNonQuery(delSql);
			mysql_free_result(result);

			char sql[128] = { 0 };
			//ִ��ɾ��Id=Ҫɾ����id sql���
			sprintf(sql, "delete from T_teachers where Id=%d", id);
			executeNonQuery(sql);
			//�ض�λ��mod=teacher&action=list������ʦģ�鲢�г����Խ�ʦ��Ϣ
			cgiHeaderLocation("Website_2.cgi?mod=teacher&action=list");
		}
		else
		{
			showError("δ֪��action����");
			return 0;
		}
	}
	else if (strcmp(mod, "student") == 0)
	{
		if (strcmp(action, "list") == 0)
		{
			char mod_admin[50] = { 0 };
			char sql[512] = { 0 };
			char username[50] = { 0 };
			cgiCookieString("mod_admin", mod_admin, sizeof(mod_admin));			//����cookie��ȷ���Ƿ�Ϊ����Ա������cookie
			if (strcmp(mod_admin, "admin") != 0)	//�����½�߲��ǹ���Ա
			{
				cgiCookieString("UserName", username, sizeof(username));		//����cookie  ȷ���û����
				//���������	��T_students���в�ѯ��������			//��ѯstu.Name='%s'��ѧ��
				sprintf(sql, "select stu.Id, stu.Name, stu.BirthDay, stu.Height, stu.Weight, gender.Name, stu.BirthPlace, race.Name from T_students stu left join T_genders gender on stu.GenderId = gender.Id left join T_races race on stu.RaceId = race.Id where stu.Name='%s'",username);
			}
			else
			{
				sprintf(sql, "select stu.Id,stu.Name,stu.BirthDay,stu.Height,stu.Weight,gender.Name,stu.BirthPlace,race.Name from T_students stu left join T_genders gender on stu.GenderId = gender.Id left join T_races race on stu.RaceId = race.Id");
			//	cgiHeaderCookieSetString("mod_admin", "", 5, "", "127.0.0.1");		//�Թ���Ա��ݲ�ѯ���һ����mod_admin�ÿգ���������ֵһֱ��admin�����鷳�ˣ�����			
			}
			//���������	��T_students���в�ѯ��������
	//		MYSQL_RES* result = executeQuery("select Id,Name,BirthDay,Height,Weight,GenderId,BirthPlace,RaceId from T_students");
			//�����Ӳ�ѯ
		//	MYSQL_RES* result = executeQuery("select stu.Id,stu.Name,stu.BirthDay,stu.Height,stu.Weight,gender.Name,stu.BirthPlace,race.Name from T_students stu left join T_genders gender on stu.GenderId = gender.Id left join T_races race on stu.RaceId = race.Id");
			MYSQL_RES* result = executeQuery(sql);
			TMPL_varlist* varlist = 0;
			TMPL_loop* loopStudents = 0;
			MYSQL_ROW row;
			while (row = mysql_fetch_row(result))
			{
				char* id = row[0];			//��һ�� id
				char* name = row[1];		//�ڶ��� Name
				char* birthday = row[2];	//������ Birthday
				char* height = row[3];
				char* weight = row[4];
				char* gendername = row[5];
				char* birthplace = row[6];
				char* racename = row[7];
				loopStudents = TMPL_add_varlist(loopStudents, TMPL_add_var(varlist, "Id", id, "Name", name, "BirthDay",
					birthday, "Height", height, "Weight", weight, "GenderName", gendername, "BirthPlace", birthplace, "RaceName", racename, 0));
			}
			varlist = TMPL_add_loop(varlist, "students", loopStudents);
		//	varlist = TMPL_add_var(varlist, "msg", username, 0);
			cgiHeaderContentType("text/html;charset=gbk");
			TMPL_write("StudentList.htm", 0, 0, varlist, cgiOut, cgiOut);
			mysql_free_result(result);		//�ͷŽ����
		}
		//�ж϶����Ƿ�Ϊ��������
		else if (strcmp(action, "addnew") == 0)
		{
			char mod_admin[10] = { 0 };
			cgiCookieString("mod_admin", mod_admin, sizeof(mod_admin));
			if (strcmp(mod_admin, "admin") != 0)
			{
				showError("�Բ�����û��Ȩ������û�");
				return 0;
			}
			char genderitems[1024] = { 0 };
			char raceitems[1024] = { 0 };
			TMPL_varlist* varlist = 0; 
			printSelectItems(genderitems, "select Id,Name from T_genders", "");
			printSelectItems(raceitems, "select Id,Name from T_races", "");
			varlist = TMPL_add_var(varlist, "GenderItems", genderitems, 0);
			varlist = TMPL_add_var(varlist, "RaceItems", raceitems, 0);
			cgiHeaderContentType("text/html;charset=gbk");
			TMPL_write("StudentsAddNew.htm", 0, 0, varlist, cgiOut, cgiOut); 
		}
		//�ж��Ƿ�ΪaddnewSubmit����ѧ�����涯��
		else if (strcmp(action, "addnewSubmit") == 0)
		{
			char name[256] = { 0 };
			char birthday[50] = { 0 };
			char height[10] = { 0 };
			char weight[10] = { 0 };
			char genderid[5] = { 0 };
			char birthplace[100] = { 0 };
			char raceid[20] = { 0 };
			//�ж�Name�Ƿ�Ϊ��
			if (cgiFormString("Name", name, sizeof(name)) != cgiFormSuccess)
			{
				showError("��������Ϊ��");
				return 0;
			}
			//�жϳ��������Ƿ�Ϊ��
			if (cgiFormString("BirthDay", birthday, sizeof(birthday)) != cgiFormSuccess)
			{
				showError("�������ڲ���Ϊ��");
				return 0;
			}
			//�����Ƿ�Ϊ��
			if (cgiFormString("BirthPlace", birthplace, sizeof(birthplace)) != cgiFormSuccess)
			{
				showError("���᲻��Ϊ��");
				return 0;
			}
			cgiFormString("Height", height, sizeof(height));
			cgiFormString("Weight", weight, sizeof(weight));
			cgiFormString("GenderId", genderid, sizeof(genderid));
			cgiFormString("RaceId", raceid, sizeof(raceid));
			char sql[1024] = { 0 };
			//ִ��sql������䣬����Ϣ�������ݿ��е�T_students��
			sprintf(sql, "insert into T_students(Name,BirthDay,Height,Weight,GenderId,BirthPlace,RaceId) values('%s','%s','%s','%s','%s','%s','%s')", name, birthday,height,weight,genderid,birthplace,raceid);
			executeNonQuery(sql);
			//�ض�λ��mod=student&action=list������ʦģ�鲢�г����Խ�ʦ��Ϣ
			cgiHeaderLocation("Website_2.cgi?mod=student&action=list");
		}
		else if (strcmp(action, "edit") == 0)
		{
			int id;
			//��ȡҪ�޸ĵ�id
			if (cgiFormInteger("id", &id, 0) != cgiFormSuccess)
			{
				showError("���ṩҪ�޸ĵ�id");
				return 0;
			}
			//���ؾɵ�����
			char sql[1024] = { 0 };
			//���������  �����ݿ��в�ѯId=Ҫ�޸ĵ�id��
			sprintf(sql, "select Id,Name,BirthDay,Height,Weight,GenderId,BirthPlace,RaceId from T_students where Id=%d", id);
			MYSQL_RES* result = executeQuery(sql);
			MYSQL_ROW row;
			if (row = mysql_fetch_row(result))  //��һ��	�ж��Ƿ���ڸ�Id
			{
				char* name = row[1];		//�ڶ��� Name
				char* birthday = row[2];	//������ birthday
				char* height = row[3];
				char* weight = row[4];
				char* genderid = row[5];
				char* birthplace = row[6];
				char* raceid = row[7];
				char genderitems[1024] = { 0 };
				char raceitems[1024] = { 0 };

				TMPL_varlist* varlist = 0;
				varlist = TMPL_add_var(varlist, "Id", row[0], "Name", name, "BirthDay", birthday, "Height", height, "Weight", weight, "GenderId", genderid, "BirthPlace", birthplace, "RaceId", raceid, 0);
				printSelectItems(genderitems, "select Id,Name from T_genders", genderid);
				printSelectItems(raceitems, "select Id,Name from T_races", raceid);
				varlist = TMPL_add_var(varlist, "GenderItems", genderitems, "RaceItems", raceitems, 0);
				cgiHeaderContentType("text/html;charset=gbk");
				//��ʾTeacherEdit.htmҳ�棬�ȴ��û��޸�
				TMPL_write("StudentEdit.htm", 0, 0, varlist, cgiOut, cgiOut);
			}
			else
			{
				showError("�Ҳ������id��Ӧ�Ľ�ʦ��Ϣ");
				return 0;
			}
			mysql_free_result(result);
		}
		//�ж��Ƿ�ΪeditSubmit�޸ı��涯��
		else if (strcmp(action, "editSubmit") == 0)
		{
			int id;
			char name[256] = { 0 };
			char birthday[50] = { 0 };
			char height[10] = { 0 };
			char weight[10] = { 0 };
			char genderid[5] = { 0 };
			char birthplace[100] = { 0 };
			char raceid[20] = { 0 };
			if (cgiFormInteger("Id", &id, 0) != cgiFormSuccess)
			{
				showError("Id����Ϊ��");
				return 0;
			}
			//�ж�Name�Ƿ�Ϊ��
			if (cgiFormString("Name", name, sizeof(name)) != cgiFormSuccess)
			{
				showError("��������Ϊ��");
				return 0;
			}
			//�ж�BirthDay�Ƿ��
			if (cgiFormString("BirthDay", birthday, sizeof(birthday)) != cgiFormSuccess)
			{
				showError("�������ڲ���Ϊ��");
				return 0;
			}
			//�����Ƿ�Ϊ��
			if (cgiFormString("BirthPlace", birthplace, sizeof(birthplace)) != cgiFormSuccess)
			{
				showError("���᲻��Ϊ��");
				return 0;
			}
			cgiFormString("Height", height, sizeof(height));
			cgiFormString("Weight", weight, sizeof(weight));
			cgiFormString("GenderId", genderid, sizeof(genderid));
			cgiFormString("RaceId", raceid, sizeof(raceid));
			char sql[1024] = { 0 };
			//����T_students��
			sprintf(sql, "update T_students set Name='%s',BirthDay='%s',Height='%s',Weight='%s',GenderId='%s',BirthPlace='%s',RaceId='%s' where Id=%d", name, birthday, height, weight, genderid, birthplace, raceid, id);
			executeNonQuery(sql);
			//�ض�λ��mod=student&action=list������ʦģ�鲢�г����Խ�ʦ��Ϣ
			cgiHeaderLocation("Website_2.cgi?mod=student&action=list");
		}
		//�ж��Ƿ�Ϊɾ�����������û����ɾ����ť
		else if (strcmp(action, "delete") == 0)
		{
			char mod_admin[10] = { 0 };
			cgiCookieString("mod_admin", mod_admin, sizeof(mod_admin));
			if (strcmp(mod_admin, "admin") != 0)	//�����½�߲��ǹ���Ա
			{
				showError("�Բ�����û��Ȩ��ɾ���û�"); 
				return 0;
			}
			int id;
			//��ȡҪɾ����id
			if (cgiFormInteger("id", &id, 0) != cgiFormSuccess)
			{
				showError("Id����Ϊ��");
				return 0;
			}
			char sql1[128] = { 0 };
			char delSql[128] = { 0 };
			//���������  �����ݿ��в�ѯId=Ҫ�޸ĵ�id��

			sprintf(sql1, "select Name from T_students where Id=%d", id);
			MYSQL_RES* result = executeQuery(sql1);
			MYSQL_ROW row;
			row = mysql_fetch_row(result);  //��һ��	�ж��Ƿ���ڸ�Id
			char* name = row[0];		//��ȡҪɾ������ѧ�����ǽ�ʦ
			sprintf(delSql, "delete from T_studentusers where username='%s'", name);
			executeNonQuery(delSql);
			mysql_free_result(result);

			char sql[128] = { 0 };
			//ִ��ɾ��Id=Ҫɾ����id sql���
			sprintf(sql, "delete from T_students where Id=%d", id);
			executeNonQuery(sql);
			//�ض�λ��mod=student&action=list������ʦģ�鲢�г����Խ�ʦ��Ϣ
			cgiHeaderLocation("Website_2.cgi?mod=student&action=list");
		}
		else
		{
			showError("δ֪��action����");
			return 0;
		}
	}
	//����ģ��
	else if (strcmp(mod, "netdisk") == 0)
	{
		if (strcmp(action, "list") == 0)
		{
			//�鵽�ļ���  _findfirst,_findnext  ��io.h��
			long findHandler;
			struct _finddata_t data;
			TMPL_varlist* varlist = 0;
			TMPL_loop* loopFiles = 0;
			//   /*.*  ��ʾ�������͵��ļ�
			findHandler = _findfirst("netdisk/*.*", &data);
			if (findHandler == -1)	//˵����Ŀ¼������
			{
				//
			}
			//���������ļ�
			while (!_findnext(findHandler, &data))
			{
				if ((data.attrib & _A_SUBDIR) == _A_SUBDIR)	//���ļ���
				{
					//
				//	printf("��һ���ļ���\n");
					continue;
				}
				/*
				else
				{
					printf("��һ���ļ�\n");
				}  */
				//	data.size   //��ȡ�ļ���С���ֽڣ�
				char fileSize[256] = { 0 };
				char fileCreateTime[256] = { 0 };
			//	itoa(data.size, fileSize, 10);
				//����ļ���СС��1024,����ʾΪ�ֽ�
				//���С��1024*1024������ʾKB
				//���С��1024*1024*1024  ��ʾMB
				if (data.size < 1024)
				{
					sprintf(fileSize, "%d �ֽ�", data.size);
				}
				else if (data.size < 1024 * 1024)
				{
					sprintf(fileSize, "%.1f KB", data.size / 1024.0);
				}
				else if (data.size < 1024 * 1024 * 1024 )
				{
					sprintf(fileSize, "%.1f MB", data.size / 1024.0 / 1024);
				}
				else 
				{
					sprintf(fileSize, "%.1f GB", data.size / 1024.0 / 1024 / 1024);
				}
		/*		printf("�ļ���С:%d\n", data.size);
				printf("�ļ�����%s", data.name); */

				time_t timeCreate = data.time_create;
				strftime(fileCreateTime, sizeof(fileCreateTime), "%Y-%m-%d %H:%M:%S", localtime(&timeCreate));
				loopFiles = TMPL_add_varlist(loopFiles, TMPL_add_var(varlist, "Name", data.name, "Size", fileSize, "CreateTime", fileCreateTime, 0));
				//printf("�ļ��������ڣ�%s\n", fileCreateTime);
			}
			varlist = TMPL_add_loop(varlist, "files", loopFiles);
			cgiHeaderContentType("text/html;charset=gbk");
			TMPL_write("NetDiskList.htm", 0, 0, varlist, cgiOut, cgiOut);
			_findclose(findHandler);
		}
		else if (strcmp(action, "delete") == 0)
		{
			char mod_admin[10] = { 0 };
			cgiCookieString("mod_admin", mod_admin, sizeof(mod_admin));
			if (strcmp(mod_admin, "admin") != 0)	//�����½�߲��ǹ���Ա
			{
				showError("�Բ�����û��Ȩ��ɾ��");
				return 0;
			}
			char filename[256] = { 0 };
			char fullFileName[1024] = { 0 };
			if (cgiFormString("filename", filename, sizeof(filename)) != cgiFormSuccess)
			{
				showError("δ����filename");
				return 0;
			}
			sprintf(fullFileName, "netdisk/%s", filename);
			remove(fullFileName);
			cgiHeaderLocation("Website_2.cgi?mod=netdisk&action=list");
		}
		else if (strcmp(action, "upload") == 0)
		{
			cgiHeaderContentType("text/html;charset=gbk");
			TMPL_write("NetDiskUpload.htm", 0, 0, 0, cgiOut, cgiOut);
		}
		//���ȷ�ϵ���ϴ�
		else if (strcmp(action, "uploadSubmit") == 0)
		{
			//���裺�������õ�Ҫ�ϴ����ļ���cgiFormFileName  ���õ��ļ���׺�ж��ļ���׺_splitpath
			//��ȡ�ļ���СcgiFormFileSize
			//�������˴��ļ�cgiFormFileOpen�����ش��ļ�fopen
			//���������ļ����ݵ�������cgiFormFileRead
			//�رձ��ش򿪵��ļ�  �رշ������˴򿪵��ļ�
			//��ȡ�ϴ����ļ���
			char uploadFile[256] = { 0 };
			if (cgiFormFileName("uploadFile", uploadFile, sizeof(uploadFile)) != cgiFormSuccess)
			{
				showError("���ϴ��ļ�");
				return 0;
			}
			//��ȡ�ļ���׺
			char filetxt[10] = { 0 };
			_splitpath(uploadFile, NULL, NULL, NULL, filetxt);  //�õ��ļ���չ��
			if (stricmp(filetxt, ".jpg") != 0 && stricmp(filetxt, ".gif") != 0 && stricmp(filetxt, ".png") != 0 &&
				stricmp(filetxt, ".zip") != 0 && stricmp(filetxt, ".txt") != 0 && stricmp(filetxt, ".rar") != 0 &&
				stricmp(filetxt, ".doc") != 0 && stricmp(filetxt, ".7z") !=  0 && stricmp(filetxt, ".cpp") != 0)
			{
				showError("ֻ�����ϴ�jpg��gif��png��zip��txt��rar��doc��7z7�ļ�!");
				return 0;
			}
			//��ȡ�ļ���С
			size_t filesize;
			cgiFormFileSize("uploadFile", &filesize);
			if (filesize > 1024 * 1024 * 20)
			{
				showError("�ϴ����ļ���С���ܳ���20MB��");
				return 0;
			}
			char savedFileName[256] = { 0 };		//�ļ�ȫ·��
			sprintf(savedFileName, "netdisk/%s", uploadFile);
			cgiFilePtr file1;  //����һ��cgiFilePtr���ļ�ָ��
			cgiFormFileOpen("uploadFile", &file1); // �������˴��ļ�
			FILE* filelocal = fopen(savedFileName, "wb");     //�򿪱����ļ�
		//	char buf[2048] = { 0 };
			char* buf = (char*)malloc(sizeof(char)* 1024 * 1024 * 21);		//���Ͻ�30M�Ŀռ�
			//	size_t lenRead;
			int lenRead;
			//��������ָ���������ζ��ļ����ݵ�������
			while (cgiFormFileRead(file1, buf, sizeof(buf), &lenRead) == cgiFormSuccess)
			{
				//ѭ��д�뵽�����ļ���
				fwrite(buf, lenRead, 1, filelocal);
			}
			fclose(filelocal);  //�رձ��ش򿪵��ļ�
			cgiFormFileClose(file1);  //�رշ������˴򿪵��ļ�
			free(buf);
			cgiHeaderLocation("Website_2.cgi?mod=netdisk&action=list");
		}
		else
		{
			showError("δ֪��action����");
			return 0;
		}
	}
// 	else if (strcmp(mod, "test") == 0)
// 	{
// 		if (strcmp(action, "set") == 0)
// 		{
// 			cgiHeaderCookieSetString("testname", "test", 20, "", "");
// 			cgiHeaderContentType("text/html;charset=gbk");
// 			fprintf(cgiOut, "д��ɹ�");
// 		}
// 		else if (strcmp(action, "get") == 0)
// 		{
// 			char value[256] = { 0 };
// 			if (cgiCookieString("testname", value, sizeof(value)) == cgiFormSuccess)
// 			{
// 				cgiHeaderContentType("text/html;charset=gbk");
// 				fprintf(cgiOut, "ok,%s", value);
// 			}
// 			else
// 			{
// 				cgiHeaderContentType("text/html;charset=gbk");;
// 				showError("cookie������");
// 			}
// 		}
// 	}
	else if (strcmp(mod, "value") == 0)
	{
		if (strcmp(action, "list") == 0)
		{
			cgiHeaderContentType("text/html;charset=gbk");
			TMPL_write("Pingjia.htm", 0, 0, 0, cgiOut, cgiOut);
		}
		else if (strcmp(action, "Submit")==0)
		{
			//��ȡ�û���kookie��
			char username[50] = { 0 };
			cgiCookieString("UserName", username, sizeof(username));

			int len;
			char* txt1;
			cgiFormStringSpaceNeeded("txt1", &len);    //cgiFormStringSpaceNeeded  ���ã���ȡ���޳��Ĳ���,���粩��
			txt1 = (char*)malloc(len + 1);
			cgiFormString("txt1", txt1, len);
			
			char sql[1024] = { 0 };
			//ִ��sql������䣬����Ϣ�������ݿ��еı�
			sprintf(sql, "update T_students set Pingjia='%s' where Name='%s'", txt1, username);
			executeNonQuery(sql);
			free(txt1);
			//�ض�λ��mod=teacher&action=list������ʦģ�鲢�г����Խ�ʦ��Ϣ
			cgiHeaderLocation("Website_2.cgi?mod=value&action=list");
		}
		else
		{
			showError("δ֪��action����");
			return 0;
		}
	}
	else
	{
		showError("δ֪��mod����");
		return 0;
	}
	return 0;
}