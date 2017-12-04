#include "TaskExcutor.h"



TaskExcutor::TaskExcutor()
{
	
}


TaskExcutor::~TaskExcutor()
{
}

vector<HomePageContentStr> TaskExcutor::GetDataFromDB(const Task& task)
{
	//临时模拟数据库操作
	//耗时操作
	//cout << "in the function: 'TaskExcutor::GetDataFromDB' " << endl;
	//select
	map<string, int> typeToSize;
	typeToSize["HomePageRequest"] = 3;
	typeToSize["HomePageRefresh"] = 2;
	typeToSize["HomePageLoadMore"] = 3;

	int userId = atoi(task.userID.c_str());
	int contentSize = typeToSize[task.taskType];

	HomePageContentStr homePageContentStr;
	vector<HomePageContentStr> homePageContentArr(contentSize, homePageContentStr);
	for (int i = 0; i < contentSize; ++i)
	{
		//cout << "Debug DB" << contentSize << endl;
		homePageContentArr[i].username = "上升的力量";
		homePageContentArr[i].userprofile = "电子与信息学院/王者荣耀爱好者";
		
		if (task.taskType == "HomePageRequest")
			homePageContentArr[i].title = "为什么安其拉打不了上单？";
		else if(task.taskType == "HomePageRefresh")
			homePageContentArr[i].title = "Refresh为什么安其拉打不了上单？";
		else
			homePageContentArr[i].title = "LoadMore为什么安其拉打不了上单？";
		
		homePageContentArr[i].profile = "作为一个强控法师，我认为安其拉完全有能力胜任上单的位置，为什么我一走上单队友就骂我坑。附带安其拉介绍：安琪拉的定位是法师，在改版加强后，安琪拉伤害主要来源于技能的释放以及被动的叠加灼烧效果。所以在出装上，追求的自然是高法术攻击的装备，特别是能够配合被动效果的装备绝对勘称性价比最高。";
		homePageContentArr[i].comments = "23";
		homePageContentArr[i].replys = "4";
		homePageContentArr[i].care = "26";
		homePageContentArr[i].resourceId = "12345";
	}
	return homePageContentArr;
}

string TaskExcutor::Run(Task task)
{
	//cout << "in the function: 'TaskExcutor::Run' " << endl;
	if (task.taskType == "HomePageRequest" || 
		task.taskType == "HomePageRefresh" ||
		task.taskType == "HomePageLoadMore")
	{
		//cout << "task.taskType" << task.taskType << endl;
		return CreateHomePage(task);
	}
	else if(task.taskType == "DynamicPageRequest")
	{
		return CreateDynamicPage(task);
	}
	else
	{
		string responseData("Type error");
		return responseData;
	}
}

/*构造HomePage*/
string TaskExcutor::CreateHomePage(const Task& task)
{
	vector<HomePageContentStr> homePageContentArr;
	homePageContentArr = GetDataFromDB(task);
	int contentSize = homePageContentArr.size();
	cJSON *homePageContent, *contentArray;
	homePageContent = cJSON_CreateObject();
	if (!homePageContent)
	{
		cout << "construct json homePageContent fail" << endl;
	}
	cJSON_AddItemToObject(homePageContent, "type", cJSON_CreateString("HomePageResponse"));
	contentArray = cJSON_CreateArray();
	if (!contentArray)
	{
		cout << "build json child fail" << endl;
	}
	cJSON_AddItemToObject(homePageContent, "Content", contentArray);

	//构造homePageContent
	cJSON* pContentArr[contentSize];
	for (int i = 0; i < contentSize; ++i)
	{
		//从数据库获取相关首页内容结构体
		//GetDataFromDB(homePageContentStr);
		//cout << "构造homePageContent" << endl;
		pContentArr[i] = cJSON_CreateObject();
		if (!pContentArr[i])
		{
			cout << "build json pContentArr[i] fail" << endl;
		}
		//向pContentArr[i]加入数据
		cJSON_AddItemToObject(pContentArr[i], "username", cJSON_CreateString(homePageContentArr[i].username.c_str()));
		cJSON_AddItemToObject(pContentArr[i], "userprofile", cJSON_CreateString(homePageContentArr[i].userprofile.c_str()));
		cJSON_AddItemToObject(pContentArr[i], "title", cJSON_CreateString(homePageContentArr[i].title.c_str()));
		cJSON_AddItemToObject(pContentArr[i], "profile", cJSON_CreateString(homePageContentArr[i].profile.c_str()));
		cJSON_AddItemToObject(pContentArr[i], "comments", cJSON_CreateString(homePageContentArr[i].comments.c_str()));
		cJSON_AddItemToObject(pContentArr[i], "replys", cJSON_CreateString(homePageContentArr[i].replys.c_str()));
		cJSON_AddItemToObject(pContentArr[i], "care", cJSON_CreateString(homePageContentArr[i].care.c_str()));
		cJSON_AddItemToObject(pContentArr[i], "resourceId", cJSON_CreateString(homePageContentArr[i].resourceId.c_str()));
		//将contnet加入contentArray数组
		cJSON_AddItemToArray(contentArray, pContentArr[i]);
	}
	//homePageContent转换为字符串，输出观察
	char *pts_HomePageContent;
	pts_HomePageContent = cJSON_PrintUnformatted(homePageContent);
	string HomePageContent = pts_HomePageContent;
	//cout << "return homepage data:" << endl;
	//cout << cJSON_Print(homePageContent) << endl;
	free(pts_HomePageContent);
	cJSON_Delete(homePageContent);
	return HomePageContent;
}

string TaskExcutor::CreateDynamicPage(const Task & task)
{
	return string();
}
