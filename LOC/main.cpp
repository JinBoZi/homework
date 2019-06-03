/*
 * main.cpp
 *
 *  Created on: 2019年5月6日
 *      Author: LiuBinbin
 */

#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <direct.h>
#include <io.h>
#include <iomanip>

using namespace std;

int states[9][7]={
		//换行,斜杠,星号,双引号,单引号,反斜杠,其他
		{0,5,0,1,3,0,0},
		{1,1,1,0,1,2,1},
		{1,1,1,1,1,1,1},
		{3,3,3,3,0,4,3},
		{3,3,3,3,3,3,3},
		{0,6,7,1,3,0,0},
		{0,6,6,6,6,6,6},
		{7,7,8,7,7,7,7},
		{7,0,7,7,7,7,7}
};
//int lines[3]={0,0,0};

void getLine(string fileName,vector<int>& lines)
{
	int curState=0;
	int flag=0;
	fstream file;
	char cur;
	file.open(fileName,ios::in);
	while(!file.eof()){
		cur = file.get();
		//file >> cur;
		//cout << cur;
		switch(cur){
			case '\n' :
				curState = states[curState][0];
				if(curState==0)
					lines[flag]++;
				if(curState==7)
					lines[2]++;
				flag=0;
				break;
			case '/' :
				curState = states[curState][1];
				if(curState==6 && flag==0)
					flag=2;
				break;
			case '*' :
				if(flag==0)
					flag=2;
				curState = states[curState][2];
				break;
			case '\"' :
				if(flag==0)
					flag=1;
				curState = states[curState][3];
				break;
			case '\'' :
				if(flag==0)
					flag=1;
				curState = states[curState][4];
				break;
			case '\\' :
				if(flag==0)
					flag=1;
				curState = states[curState][5];
				break;
			case ' ' :
			case '\t' :
				curState = states[curState][6];
				break;
			default :
				if(flag==0)
					flag=1;
				curState = states[curState][6];
				break;
		}
	}
	cout<<setw(50)<<left << fileName;
	cout<<setw(20)<<left << lines[0];
	cout<<setw(20)<<left << lines[1];
	cout<<setw(20)<<left << lines[2];
	cout << endl;
	//cout << fileName << "\t" << lines[0] << "\t" << lines[1] << "\t" << lines[2] << endl;
}

void getLines(vector<string>& files,vector<vector<int>>& lines)
{
	int len=files.size();
	for(int index=0;index<len;index++){
		vector<int> curLines(3);
		getLine(files[index],curLines);
		lines.push_back(curLines);
	}
}

int getFiles(vector<string>& files, string dir)
{
    string dirNew = dir + "\\*.*";

    intptr_t handle;
    _finddata_t findData;

    handle = _findfirst(dir.c_str(), &findData);
	if (handle == -1){
		// 检查是否成功
		cout << "The path is invalid!" << endl;
		return -1;
	}
	else if(!(findData.attrib&_A_SUBDIR)){
		files.push_back(dir);
		return 0;
	}

    handle = _findfirst(dirNew.c_str(), &findData);
    if (handle == -1){
    	// 检查是否成功
    	cout << "The path is invalid!" << endl;
    	return -1;
    }

    do
    {
    	//获取文件完整路径名
    	dirNew = dir+"\\"+findData.name;
        if (findData.attrib & _A_SUBDIR)
        {
        	//如果是目录,递归搜索
            if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
                continue;
            getFiles(files,dirNew);
        }
        else
        {
        	//如果是文件,保存完整路径名
        	files.push_back(dirNew);
        }
    } while (_findnext(handle, &findData) == 0);

    _findclose(handle);    // 关闭搜索句柄

    return 0;
}

int main(int argc,char **argv)
{
	if(argc != 2){
		cout << "Please scanf LOC path(The path should be an absolute path of your source code.)" << endl;
		cout << "example: LOC H:\\osdc" << endl;
	}
	vector<vector<int>> lines;
	vector<string> files;
	//files.push_back(argv[1]);
	int ret = getFiles(files,argv[1]);
	if(ret == -1)
		return 0;
	cout<<setw(50)<<left << "FileName";
	cout<<setw(20)<<left << "Null lines";
	cout<<setw(20)<<left << "Lines of code";
	cout<<setw(20)<<left << "Annotation lines";
	cout << endl;
	getLines(files,lines);
	return 0;
}
