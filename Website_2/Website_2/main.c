#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "share.h"
#include <io.h>
#include <windows.h>


//由于使用cookie  可能使用中文会有问题，所以尽可能用户名为英文

//mod参数确定是哪个模块，action参数确定是什么动作，
//列表List、新增addnew、新增保存addnewSubmit、
//修改edit、修改保存editSubmit

/*
StudentMgr.cgi?mod=teacher&action=list  列出所有老师的数据
StudentMgr.cgi?mod=teacher&action=addnew 进入新增老师模块
StudentMgr.cgi?mod=teacher&action=addnewSubmit 新增老师保存动作
StudentMgr.cgi?mod=student&action=list		列出所有学生数据
StudentMgr.cgi?mod=user&action=login		用户登陆
*/


//实现学生模块和教师模块
int cgiMain()
{
	char mod[32] = { 0 };
	char action[32] = { 0 };
	//对所有cgi请求至少带两个参数
	//StudentMgr.cgi?mod=teacher&action=list  列出所有老师的数据
	
	if (cgiFormString("mod", mod, sizeof(mod)) != cgiFormSuccess)
	{
		showError("没有提供mod参数");
		return 0;
	}
	if (cgiFormString("action", action, sizeof(action)) != cgiFormSuccess)
	{
		showError("没有提供action参数");
		return 0;
	}
	/*
	  */
	//如果是main模块
	if (strcmp(mod, "main") == 0)
	{
		//动作是home
		if (strcmp(action, "home") == 0)
		{
			int userid = getSessionUserId();
			char sqlSelect[256] = { 0 };
			MYSQL_RES* result;
			MYSQL_ROW row;
			if (userid == -1)
			{
				showError("必须登录才能访问！");
				return;
			}
			
			char buf1[12] = { 0 };
			cgiCookieString("mod", buf1, sizeof(buf1));		//接收cookies  确认用户身份
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
				//如果既不是教师模块，也不是学生模块,那就是管理员模块
				else
				{
					cgiHeaderContentType("text/html;charset=gbk");
					TMPL_write("Main_admin.htm", 0, 0, 0, cgiOut, cgiOut);
				}
			}
			else
			{
				showError("根据UserId没有找到用户信息！");
				return;
			}
			mysql_free_result(result);
		}
		else if (strcmp(action, "adminSubmit")==0)
		{
			char username[50] = { 0 };
			cgiCookieString("UserName", username, sizeof(username));

			//获取id判断是student还是teacher，并发出coookie

			TMPL_varlist* varlist = 0;
			char choose[20] = { 0 };
			cgiFormString("choose", choose, sizeof(choose));		//获取Main_admin的单选按钮参数
			varlist = TMPL_add_var(varlist, "Name", choose, "UserName", username, 0);		//将获取的参数给了Main_admin的模板里的参数
			//这里传不传无所谓
			cgiHeaderCookieSetString("mod", choose, 200, "", "127.0.0.1");	//将选中的参数传给mod参数
			cgiHeaderContentType("text/html;charset=gbk");
			TMPL_write("Main.htm", 0, 0, varlist, cgiOut, cgiOut);
		}
		else if (strcmp(action, "login") == 0)
		{
			cgiHeaderCookieSetString("mod_admin", "", 5, "", "127.0.0.1");		//以管理员身份查询完后一定将mod_admin置空，否则它的值一直是admin，就麻烦了！！！
	//		MessageBoxA(0, "", "", 0);
			//使用cookie保存用户名
			char username[50] = { 0 };
			cgiCookieString("UserName", username, sizeof(username));
			TMPL_varlist* varlist = 0;
			varlist = TMPL_add_var(varlist, "UserName", username, 0);
			cgiHeaderContentType("text/html;charset=gbk");
			TMPL_write("Login.htm", 0, 0, varlist, cgiOut, cgiOut);
		}

		else if (strcmp(action, "loginSubmit") == 0)
		{
         /* cgiHeaderCookieSetString("mod_admin", "", 5, "", "127.0.0.1");		//以管理员身份查询完后一定将mod_admin置空，否则它的值一直是admin，就麻烦了！！！
			
			char username[256] = { 0 };
			char password[256] = { 0 };
			//判断获取username参数是否成功
			if (cgiFormString("username", username, sizeof(username)) != cgiFormSuccess)
			{
				showError("用户名不能为空\n");
			//	TMPL_varlist* varlist = 0;
			//	varlist = TMPL_add_var(varlist, "msg", "用户名必须填写", 0);
				return 0;
			}
			//判断获取password参数是否成功
			if (cgiFormString("password", password, sizeof(password)) != cgiFormSuccess)
			{
				showError("密码不能为空\n");
				return 0;
			} */
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

	       /* char sql[1024] = { 0 };
			//采用16进制生成形式，避免SQL注入漏洞   在.net,java等中采用参数化查询避免产生注入漏洞
			char hexUsername[1024] = { 0 };
			char hexPassword[1024] = { 0 };
			//产生16进制的 存放到hexUsername和hexPassword中
			mysql_hex_string(hexUsername, username, strlen(username));
			mysql_hex_string(hexPassword, password, strlen(password));
			//查询mysql数据库中是否存在username=用户password=密码的
			sprintf(sql, "select Id,username,password from T_users where username=0x%s and password=0x%s", hexUsername, hexPassword);
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
				cgiHeaderCookieSetString("UserName", username, 60 * 60 * 24, "", "127.0.0.1");		//发出kookie
				setSessionUserId(atoi(count));
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
						cgiHeaderCookieSetString("mod", "teacher", 200, "", "127.0.0.1");		//发出cookie 即发出用户身份teacher
				//		cgiHeaderCookieSetString("Username", username, 3000, "", "127.0.0.1");	//发出kookie 即哪个用户
						cgiHeaderLocation("Website_2.cgi?mod=main&action=home");
						return 0;
					}
					//学生模块
					if (strcmp(mangement, "students") == 0)
					{
						//从数据库查询成功
						cgiHeaderCookieSetString("mod", "student", 200, "", "127.0.0.1");			//发出cookie 即发出用户身份student
					//	cgiHeaderCookieSetString("Username", hexUsername, 2000, "", "127.0.0.1");		//发出kookie 即哪个用户
						cgiHeaderLocation("Website_2.cgi?mod=main&action=home");	
						return 0;
					}
					//管理员模块
					else if (strcmp(mangement,"admin") ==0 )
					{
						cgiHeaderCookieSetString("mod", "admin", 200, "", "127.0.0.1");
						cgiHeaderCookieSetString("mod_admin", "admin", 200, "", "127.0.0.1");		//如果是管理员，发出cookie
						cgiHeaderLocation("Website_2.cgi?mod=main&action=home");
						return 0;
					}
					else
					{

					}
				}
			} */
		//	MessageBoxA(0, "", "", 0);
			cgiHeaderCookieSetString("mod_admin", "", 5, "", "127.0.0.1");		//以管理员身份查询完后一定将mod_admin置空，否则它的值一直是admin，就麻烦了！！！
			char username[256] = { 0 };
			char password[256] = { 0 };
			//判断获取username参数是否成功
			if (cgiFormString("username", username, sizeof(username)) != cgiFormSuccess)
			{
				showError("用户名不能为空\n");
				//	TMPL_varlist* varlist = 0;
				//	varlist = TMPL_add_var(varlist, "msg", "用户名必须填写", 0);
				return 0;
			}
			//判断获取password参数是否成功
			if (cgiFormString("password", password, sizeof(password)) != cgiFormSuccess)
			{
				showError("密码不能为空\n");
				return 0;
			} 
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
					cgiHeaderCookieSetString("mod", "teacher", 200, "", "127.0.0.1");		//发出cookie 即发出用户身份teacher
					//cgiHeaderCookieSetString("Username", username, 3000, "", "127.0.0.1");	//发出kookie 即哪个用户
				//	cgiHeaderLocation("Website_2.cgi?mod=main&action=home");
				//	return 0;
				}
				//学生模块
				if (strcmp(mangement, "students") == 0)
				{
					//从数据库查询成功
					cgiHeaderCookieSetString("mod", "student", 200, "", "127.0.0.1");			//发出cookie 即发出用户身份student
					//cgiHeaderCookieSetString("Username", hexUsername, 2000, "", "127.0.0.1");		//发出kookie 即哪个用户
					//	cgiHeaderLocation("Website_2.cgi?mod=main&action=home");
					//	return 0;
				}
				//管理员模块
				else if (strcmp(mangement, "admin") == 0)
				{
					cgiHeaderCookieSetString("mod", "admin", 200, "", "127.0.0.1");
					cgiHeaderCookieSetString("mod_admin", "admin", 200, "", "127.0.0.1");	//如果是管理员，发出cookie
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
			//采用16进制生成形式，避免SQL注入漏洞   在.net,java等中采用参数化查询避免产生注入漏洞
			char hexUsername[1024] = { 0 };
			char hexPassword[1024] = { 0 };
			//产生16进制的 存放到hexUsername和hexPassword中
			mysql_hex_string(hexUsername, username, strlen(username));
			mysql_hex_string(hexPassword, password, strlen(password));

			char mod[10] = { 0 };
			cgiCookieString("mod", mod, sizeof(mod));
			char user_table[30] = { 0 };
			if (strcmp(mod, "teacher") == 0 || strcmp(mangement,"teachers")==0)
			{
				sprintf(user_table, "T_teacherUsers");
				//查询mysql数据库中是否存在username=用户password=密码的
				sprintf(sql, "select count(*),id from %s where username=0x%s and password=0x%s",user_table, hexUsername, hexPassword);
			}
			else if (strcmp(mod, "student") == 0 || strcmp(mangement, "students") == 0)
			{
				sprintf(user_table, "T_studentUsers");
				//查询mysql数据库中是否存在username=用户password=密码的
				sprintf(sql, "select count(*),id from %s where username=0x%s and password=0x%s", user_table, hexUsername, hexPassword);
			}
			else if (strcmp(mod, "admin") == 0 || strcmp(mangement, "admin") == 0)
			{
				sprintf(user_table, "T_adminUsers");
				//查询mysql数据库中是否存在username=用户password=密码的
				sprintf(sql, "select count(*),id from %s where username=0x%s and password=0x%s", user_table, hexUsername, hexPassword);
			}
			else

			{

			}	
			MYSQL_RES* result = executeQuery(sql);		//生产结果集
			MYSQL_ROW row = mysql_fetch_row(result);	//定义一行
			char* count = row[0];					//将获取的第一行 放到count中
			char *id = row[1];
			mysql_free_result(result);			//释放结果集

			if (strcmp(count, "0") == 0)		//如果没有密码
			{
				showError("用户名或密码错误\n");
			}
			else	//说明有密码，就直接重定向到mod=main&action=home对应的界面
			{
				//保存上次成功登陆后的用户名
				cgiHeaderCookieSetString("UserName", username, 60 * 60 * 24, "", "127.0.0.1");		//发出kookie
				setSessionUserId(atoi(row[1]));
				cgiHeaderLocation("Website_2.cgi?mod=main&action=home");	//
			} 
		} 
	}
	//如果mod参数是teacher那么进入老师模块
	else if (strcmp(mod, "teacher") == 0)
	{	
		//如果action参数是list，那么列出所有老师的数据
		if (strcmp(action, "list") == 0)
		{
			char mod_admin[10] = { 0 };
			char sql[256] = { 0 };
			cgiCookieString("mod_admin", mod_admin, sizeof(mod_admin));	
			if (strcmp(mod_admin, "admin") != 0)	//如果登陆者不是管理员
			{
				char username[100] = { 0 };
				cgiCookieString("UserName", username, sizeof(username));
				//生产结果集	从T_teachers表中查询所有数据
				sprintf(sql, "select Id,Name,PhoneNum from T_teachers where Name='%s'", username);
			}
			else
			{
				sprintf(sql, "select Id,Name,PhoneNum from T_teachers");
		//		cgiHeaderCookieSetString("mod_admin", "", 5, "", "127.0.0.1");		//以管理员身份查询万后一定将mod_admin置空，否则它的值一直是admin，就麻烦了！！！
			}
			MYSQL_RES* result = executeQuery(sql);
			TMPL_varlist* varlist = 0;
			TMPL_loop* loopTeachers = 0;
			MYSQL_ROW row;		//定义一行
			while (row = mysql_fetch_row(result))		//将取出的数据一行一行的取出
			{
				char* id = row[0];			//第一列 id
				char* name = row[1];		//第二列 Name
				char* phoneNum = row[2];	//第三列 PhoneNum
				loopTeachers = TMPL_add_varlist(loopTeachers, TMPL_add_var(0, "Id", id, "Name", name, "PhoneNum", phoneNum, 0));
			}
			varlist = TMPL_add_loop(varlist, "teachers", loopTeachers);
			cgiHeaderContentType("text/html;charset=gbk");
			TMPL_write("TeacherList.htm", 0, 0, varlist, cgiOut, cgiOut);
			mysql_free_result(result);		//释放结果集
		}
		//判断是否为新增按钮
		else if (strcmp(action, "addnew") == 0)
		{
			char mod_admin[10] = { 0 };
			cgiCookieString("mod_admin", mod_admin, sizeof(mod_admin));
			if (strcmp(mod_admin, "admin") != 0)
			{
				showError("对不起，您没有权限添加用户");
				return 0;
			}	
			//如果是，那么显示TeacherAddNew.htm页面
			cgiHeaderContentType("text/html;charset=gbk");
			TMPL_write("TeacherAddNew.htm", 0, 0, 0, cgiOut, cgiOut);
		}
		//判断是否为addnewSubmit新增老师保存动作
		else if (strcmp(action, "addnewSubmit") == 0)
		{
			char name[256] = { 0 };
			char phoneNum[256] = { 0 };
			//判断Name是否为空
			if (cgiFormString("Name", name, sizeof(name)) != cgiFormSuccess)
			{
				showError("姓名不能为空");
				return 0;
			}
			//判断PhoneNum是否为空
			if (cgiFormString("PhoneNum", phoneNum, sizeof(phoneNum)) != cgiFormSuccess)
			{
				showError("手机号不能为空");
				return 0;
			}
			char sql[1024] = { 0 };
			//执行sql插入语句，将信息插入数据库中的T_teachers表
			sprintf(sql, "insert into T_teachers(Name,PhoneNum) values('%s','%s')", name, phoneNum);
			executeNonQuery(sql);
			//重定位到mod=teacher&action=list，即教师模块并列出所以教师信息
			cgiHeaderLocation("Website_2.cgi?mod=teacher&action=list");
		}
		//判断是否为修改动作
		else if (strcmp(action, "edit") == 0)
		{
			int id;
			//获取要修改的id
			if (cgiFormInteger("id", &id, 0) != cgiFormSuccess)
			{
				showError("请提供要修改的id");
				return 0;
			}
			//加载旧的数据
			char sql[1024] = { 0 };
			//生产结果集  从数据库中查询Id=要修改的id的
			sprintf(sql, "select Id,Name,PhoneNum from T_teachers where Id=%d",id);
			MYSQL_RES* result = executeQuery(sql);
			MYSQL_ROW row;
			if (row = mysql_fetch_row(result))  //有一条	判断是否存在该Id
			{
				char* name = row[1];
				char* phoneNum = row[2];
				TMPL_varlist* varlist = 0;
				varlist = TMPL_add_var(varlist, "Id", row[0], "Name", name, "PhoneNum", phoneNum, 0);
				cgiHeaderContentType("text/html;charset=gbk");
				//显示TeacherEdit.htm页面，等待用户修改
				TMPL_write("TeacherEdit.htm", 0, 0, varlist, cgiOut, cgiOut);
			}
			else
			{
				showError("找不到这个id对应的教师信息");
				return 0;
			}
			mysql_free_result(result);
		}
		//判断是否为editSubmit修改保存动作
		else if (strcmp(action, "editSubmit") == 0)
		{
			int id;
			char name[256] = { 0 };
			char phoneNum[256] = { 0 };
			if (cgiFormInteger("Id", &id, 0) != cgiFormSuccess)
			{
				showError("Id不能为空");
				return 0;
			}
			//判断Name是否为空
			if (cgiFormString("Name", name, sizeof(name)) != cgiFormSuccess)
			{
				showError("Name不能为空");
				return 0;
			}
			//判读PhoneNum是否空
			if (cgiFormString("PhoneNum", phoneNum, sizeof(phoneNum)) != cgiFormSuccess)
			{
				showError("PhoneNum不能为空");
				return 0;
			}
			char sql[1024] = { 0 };
			//更新T_teachers表
			sprintf(sql, "update T_teachers set Name='%s',PhoneNum='%s' where Id=%d", name, phoneNum,id);
			executeNonQuery(sql);
			//重定位到mod=teacher&action=list，即教师模块并列出所以教师信息
			cgiHeaderLocation("Website_2.cgi?mod=teacher&action=list");
		}
		//判断是否为删除动作，若用户点击删除按钮
		else if (strcmp(action, "delete") == 0)
		{
			char mod_admin[10] = { 0 };
			cgiCookieString("mod_admin", mod_admin, sizeof(mod_admin));
			if (strcmp(mod_admin, "admin") != 0)
			{
				showError("对不起，您没有删除权限");
				return 0;
			}
			int id;
			//获取要删除的id
			if (cgiFormInteger("id", &id, 0) != cgiFormSuccess)
			{	
				showError("Id不能为空");
				return 0;
			}

			char sql1[128] = { 0 };
			char delSql[128] = { 0 };
			//生产结果集  从数据库中查询Id=要修改的id的
			
			sprintf(sql1, "select Name from T_teachers where Id=%d", id);
			MYSQL_RES* result = executeQuery(sql1);
			MYSQL_ROW row;
			row = mysql_fetch_row(result);  //有一条	判断是否存在该Id
			char* name = row[0];		//获取要删除的是学生还是教师
			sprintf(delSql, "delete from T_teacherusers where username='%s'", name);
			executeNonQuery(delSql);
			mysql_free_result(result);

			char sql[128] = { 0 };
			//执行删除Id=要删除的id sql语句
			sprintf(sql, "delete from T_teachers where Id=%d", id);
			executeNonQuery(sql);
			//重定位到mod=teacher&action=list，即教师模块并列出所以教师信息
			cgiHeaderLocation("Website_2.cgi?mod=teacher&action=list");
		}
		else
		{
			showError("未知的action参数");
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
			cgiCookieString("mod_admin", mod_admin, sizeof(mod_admin));			//接收cookie，确认是否为管理员发出的cookie
			if (strcmp(mod_admin, "admin") != 0)	//如果登陆者不是管理员
			{
				cgiCookieString("UserName", username, sizeof(username));		//接收cookie  确定用户身份
				//生产结果集	从T_students表中查询所有数据			//查询stu.Name='%s'的学生
				sprintf(sql, "select stu.Id, stu.Name, stu.BirthDay, stu.Height, stu.Weight, gender.Name, stu.BirthPlace, race.Name from T_students stu left join T_genders gender on stu.GenderId = gender.Id left join T_races race on stu.RaceId = race.Id where stu.Name='%s'",username);
			}
			else
			{
				sprintf(sql, "select stu.Id,stu.Name,stu.BirthDay,stu.Height,stu.Weight,gender.Name,stu.BirthPlace,race.Name from T_students stu left join T_genders gender on stu.GenderId = gender.Id left join T_races race on stu.RaceId = race.Id");
			//	cgiHeaderCookieSetString("mod_admin", "", 5, "", "127.0.0.1");		//以管理员身份查询万后一定将mod_admin置空，否则它的值一直是admin，就麻烦了！！！			
			}
			//生产结果集	从T_students表中查询所有数据
	//		MYSQL_RES* result = executeQuery("select Id,Name,BirthDay,Height,Weight,GenderId,BirthPlace,RaceId from T_students");
			//左连接查询
		//	MYSQL_RES* result = executeQuery("select stu.Id,stu.Name,stu.BirthDay,stu.Height,stu.Weight,gender.Name,stu.BirthPlace,race.Name from T_students stu left join T_genders gender on stu.GenderId = gender.Id left join T_races race on stu.RaceId = race.Id");
			MYSQL_RES* result = executeQuery(sql);
			TMPL_varlist* varlist = 0;
			TMPL_loop* loopStudents = 0;
			MYSQL_ROW row;
			while (row = mysql_fetch_row(result))
			{
				char* id = row[0];			//第一列 id
				char* name = row[1];		//第二列 Name
				char* birthday = row[2];	//第三列 Birthday
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
			mysql_free_result(result);		//释放结果集
		}
		//判断动作是否为新增动作
		else if (strcmp(action, "addnew") == 0)
		{
			char mod_admin[10] = { 0 };
			cgiCookieString("mod_admin", mod_admin, sizeof(mod_admin));
			if (strcmp(mod_admin, "admin") != 0)
			{
				showError("对不起，您没有权限添加用户");
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
		//判断是否为addnewSubmit新增学生保存动作
		else if (strcmp(action, "addnewSubmit") == 0)
		{
			char name[256] = { 0 };
			char birthday[50] = { 0 };
			char height[10] = { 0 };
			char weight[10] = { 0 };
			char genderid[5] = { 0 };
			char birthplace[100] = { 0 };
			char raceid[20] = { 0 };
			//判断Name是否为空
			if (cgiFormString("Name", name, sizeof(name)) != cgiFormSuccess)
			{
				showError("姓名不能为空");
				return 0;
			}
			//判断出生日期是否为空
			if (cgiFormString("BirthDay", birthday, sizeof(birthday)) != cgiFormSuccess)
			{
				showError("出生日期不能为空");
				return 0;
			}
			//籍贯是否为空
			if (cgiFormString("BirthPlace", birthplace, sizeof(birthplace)) != cgiFormSuccess)
			{
				showError("籍贯不能为空");
				return 0;
			}
			cgiFormString("Height", height, sizeof(height));
			cgiFormString("Weight", weight, sizeof(weight));
			cgiFormString("GenderId", genderid, sizeof(genderid));
			cgiFormString("RaceId", raceid, sizeof(raceid));
			char sql[1024] = { 0 };
			//执行sql插入语句，将信息插入数据库中的T_students表
			sprintf(sql, "insert into T_students(Name,BirthDay,Height,Weight,GenderId,BirthPlace,RaceId) values('%s','%s','%s','%s','%s','%s','%s')", name, birthday,height,weight,genderid,birthplace,raceid);
			executeNonQuery(sql);
			//重定位到mod=student&action=list，即教师模块并列出所以教师信息
			cgiHeaderLocation("Website_2.cgi?mod=student&action=list");
		}
		else if (strcmp(action, "edit") == 0)
		{
			int id;
			//获取要修改的id
			if (cgiFormInteger("id", &id, 0) != cgiFormSuccess)
			{
				showError("请提供要修改的id");
				return 0;
			}
			//加载旧的数据
			char sql[1024] = { 0 };
			//生产结果集  从数据库中查询Id=要修改的id的
			sprintf(sql, "select Id,Name,BirthDay,Height,Weight,GenderId,BirthPlace,RaceId from T_students where Id=%d", id);
			MYSQL_RES* result = executeQuery(sql);
			MYSQL_ROW row;
			if (row = mysql_fetch_row(result))  //有一条	判断是否存在该Id
			{
				char* name = row[1];		//第二列 Name
				char* birthday = row[2];	//第三列 birthday
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
				//显示TeacherEdit.htm页面，等待用户修改
				TMPL_write("StudentEdit.htm", 0, 0, varlist, cgiOut, cgiOut);
			}
			else
			{
				showError("找不到这个id对应的教师信息");
				return 0;
			}
			mysql_free_result(result);
		}
		//判断是否为editSubmit修改保存动作
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
				showError("Id不能为空");
				return 0;
			}
			//判断Name是否为空
			if (cgiFormString("Name", name, sizeof(name)) != cgiFormSuccess)
			{
				showError("姓名不能为空");
				return 0;
			}
			//判读BirthDay是否空
			if (cgiFormString("BirthDay", birthday, sizeof(birthday)) != cgiFormSuccess)
			{
				showError("出生日期不能为空");
				return 0;
			}
			//籍贯是否为空
			if (cgiFormString("BirthPlace", birthplace, sizeof(birthplace)) != cgiFormSuccess)
			{
				showError("籍贯不能为空");
				return 0;
			}
			cgiFormString("Height", height, sizeof(height));
			cgiFormString("Weight", weight, sizeof(weight));
			cgiFormString("GenderId", genderid, sizeof(genderid));
			cgiFormString("RaceId", raceid, sizeof(raceid));
			char sql[1024] = { 0 };
			//更新T_students表
			sprintf(sql, "update T_students set Name='%s',BirthDay='%s',Height='%s',Weight='%s',GenderId='%s',BirthPlace='%s',RaceId='%s' where Id=%d", name, birthday, height, weight, genderid, birthplace, raceid, id);
			executeNonQuery(sql);
			//重定位到mod=student&action=list，即教师模块并列出所以教师信息
			cgiHeaderLocation("Website_2.cgi?mod=student&action=list");
		}
		//判断是否为删除动作，若用户点击删除按钮
		else if (strcmp(action, "delete") == 0)
		{
			char mod_admin[10] = { 0 };
			cgiCookieString("mod_admin", mod_admin, sizeof(mod_admin));
			if (strcmp(mod_admin, "admin") != 0)	//如果登陆者不是管理员
			{
				showError("对不起，您没有权限删除用户"); 
				return 0;
			}
			int id;
			//获取要删除的id
			if (cgiFormInteger("id", &id, 0) != cgiFormSuccess)
			{
				showError("Id不能为空");
				return 0;
			}
			char sql1[128] = { 0 };
			char delSql[128] = { 0 };
			//生产结果集  从数据库中查询Id=要修改的id的

			sprintf(sql1, "select Name from T_students where Id=%d", id);
			MYSQL_RES* result = executeQuery(sql1);
			MYSQL_ROW row;
			row = mysql_fetch_row(result);  //有一条	判断是否存在该Id
			char* name = row[0];		//获取要删除的是学生还是教师
			sprintf(delSql, "delete from T_studentusers where username='%s'", name);
			executeNonQuery(delSql);
			mysql_free_result(result);

			char sql[128] = { 0 };
			//执行删除Id=要删除的id sql语句
			sprintf(sql, "delete from T_students where Id=%d", id);
			executeNonQuery(sql);
			//重定位到mod=student&action=list，即教师模块并列出所以教师信息
			cgiHeaderLocation("Website_2.cgi?mod=student&action=list");
		}
		else
		{
			showError("未知的action参数");
			return 0;
		}
	}
	//网盘模块
	else if (strcmp(mod, "netdisk") == 0)
	{
		if (strcmp(action, "list") == 0)
		{
			//查到文件：  _findfirst,_findnext  在io.h中
			long findHandler;
			struct _finddata_t data;
			TMPL_varlist* varlist = 0;
			TMPL_loop* loopFiles = 0;
			//   /*.*  表示所有类型的文件
			findHandler = _findfirst("netdisk/*.*", &data);
			if (findHandler == -1)	//说明此目录不存在
			{
				//
			}
			//遍历所有文件
			while (!_findnext(findHandler, &data))
			{
				if ((data.attrib & _A_SUBDIR) == _A_SUBDIR)	//是文件夹
				{
					//
				//	printf("是一个文件夹\n");
					continue;
				}
				/*
				else
				{
					printf("是一个文件\n");
				}  */
				//	data.size   //获取文件大小（字节）
				char fileSize[256] = { 0 };
				char fileCreateTime[256] = { 0 };
			//	itoa(data.size, fileSize, 10);
				//如果文件大小小于1024,则显示为字节
				//如果小于1024*1024，则显示KB
				//如果小于1024*1024*1024  显示MB
				if (data.size < 1024)
				{
					sprintf(fileSize, "%d 字节", data.size);
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
		/*		printf("文件大小:%d\n", data.size);
				printf("文件名：%s", data.name); */

				time_t timeCreate = data.time_create;
				strftime(fileCreateTime, sizeof(fileCreateTime), "%Y-%m-%d %H:%M:%S", localtime(&timeCreate));
				loopFiles = TMPL_add_varlist(loopFiles, TMPL_add_var(varlist, "Name", data.name, "Size", fileSize, "CreateTime", fileCreateTime, 0));
				//printf("文件创建日期：%s\n", fileCreateTime);
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
			if (strcmp(mod_admin, "admin") != 0)	//如果登陆者不是管理员
			{
				showError("对不起，你没有权限删除");
				return 0;
			}
			char filename[256] = { 0 };
			char fullFileName[1024] = { 0 };
			if (cgiFormString("filename", filename, sizeof(filename)) != cgiFormSuccess)
			{
				showError("未设置filename");
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
		//如果确认点击上传
		else if (strcmp(action, "uploadSubmit") == 0)
		{
			//步骤：服务器拿到要上传的文件名cgiFormFileName  再拿到文件后缀判断文件后缀_splitpath
			//获取文件大小cgiFormFileSize
			//服务器端打开文件cgiFormFileOpen、本地打开文件fopen
			//服务器读文件内容到数组中cgiFormFileRead
			//关闭本地打开的文件  关闭服务器端打开的文件
			//获取上传的文件名
			char uploadFile[256] = { 0 };
			if (cgiFormFileName("uploadFile", uploadFile, sizeof(uploadFile)) != cgiFormSuccess)
			{
				showError("请上传文件");
				return 0;
			}
			//获取文件后缀
			char filetxt[10] = { 0 };
			_splitpath(uploadFile, NULL, NULL, NULL, filetxt);  //拿到文件扩展名
			if (stricmp(filetxt, ".jpg") != 0 && stricmp(filetxt, ".gif") != 0 && stricmp(filetxt, ".png") != 0 &&
				stricmp(filetxt, ".zip") != 0 && stricmp(filetxt, ".txt") != 0 && stricmp(filetxt, ".rar") != 0 &&
				stricmp(filetxt, ".doc") != 0 && stricmp(filetxt, ".7z") !=  0 && stricmp(filetxt, ".cpp") != 0)
			{
				showError("只允许上传jpg、gif、png、zip、txt、rar、doc、7z7文件!");
				return 0;
			}
			//获取文件大小
			size_t filesize;
			cgiFormFileSize("uploadFile", &filesize);
			if (filesize > 1024 * 1024 * 20)
			{
				showError("上传的文件大小不能超过20MB！");
				return 0;
			}
			char savedFileName[256] = { 0 };		//文件全路径
			sprintf(savedFileName, "netdisk/%s", uploadFile);
			cgiFilePtr file1;  //定义一个cgiFilePtr的文件指针
			cgiFormFileOpen("uploadFile", &file1); // 服务器端打开文件
			FILE* filelocal = fopen(savedFileName, "wb");     //打开本地文件
		//	char buf[2048] = { 0 };
			char* buf = (char*)malloc(sizeof(char)* 1024 * 1024 * 21);		//堆上建30M的空间
			//	size_t lenRead;
			int lenRead;
			//服务器按指定长度依次读文件内容到数组中
			while (cgiFormFileRead(file1, buf, sizeof(buf), &lenRead) == cgiFormSuccess)
			{
				//循环写入到本地文件中
				fwrite(buf, lenRead, 1, filelocal);
			}
			fclose(filelocal);  //关闭本地打开的文件
			cgiFormFileClose(file1);  //关闭服务器端打开的文件
			free(buf);
			cgiHeaderLocation("Website_2.cgi?mod=netdisk&action=list");
		}
		else
		{
			showError("未知的action参数");
			return 0;
		}
	}
// 	else if (strcmp(mod, "test") == 0)
// 	{
// 		if (strcmp(action, "set") == 0)
// 		{
// 			cgiHeaderCookieSetString("testname", "test", 20, "", "");
// 			cgiHeaderContentType("text/html;charset=gbk");
// 			fprintf(cgiOut, "写入成功");
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
// 				showError("cookie不存在");
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
			//获取用户名kookie后
			char username[50] = { 0 };
			cgiCookieString("UserName", username, sizeof(username));

			int len;
			char* txt1;
			cgiFormStringSpaceNeeded("txt1", &len);    //cgiFormStringSpaceNeeded  作用：获取无限长的参数,比如博客
			txt1 = (char*)malloc(len + 1);
			cgiFormString("txt1", txt1, len);
			
			char sql[1024] = { 0 };
			//执行sql插入语句，将信息插入数据库中的表
			sprintf(sql, "update T_students set Pingjia='%s' where Name='%s'", txt1, username);
			executeNonQuery(sql);
			free(txt1);
			//重定位到mod=teacher&action=list，即教师模块并列出所以教师信息
			cgiHeaderLocation("Website_2.cgi?mod=value&action=list");
		}
		else
		{
			showError("未知的action参数");
			return 0;
		}
	}
	else
	{
		showError("未知的mod参数");
		return 0;
	}
	return 0;
}