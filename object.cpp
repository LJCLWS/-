#include "lexer.h"
#include <vector>
#include <iostream>
#include "object.h"


using namespace std;



void object::writeFile()
{
	objectFile.open("C:\\Users\\ryan\\OneDrive\\iot\\Compilers\\Curriculum Design\\Compiler\\objectFile\\Demo.asm");
	//objectFile << ".MODEL flat, stdcall"<<'\n'<< "ExitProcess PROTO, ; exit program" << '\n';
	//objectFile << "dwExitCode:DWORD ; return code"<<'\n';
	objectFile << ".data" << '\n';
	
	for (int i = 0; i < MainSymbolList.size(); i++)
		objectFile << MainSymbolList[i].name << '\t' << "DD" <<"\t" <<"?" <<'\n';

	objectFile << ".code" << '\n';
	objectFile << '\n' << "main PROC" << '\n'<<'\n';
}
  
void object::init_active_inf()
{

	vector<vector<SymbolListCommon_Type>> temp_FuctionList = SymbolTable.get_FunctionList();
	vector<SymbolListCommon_Type> temp_SymbolList = SymbolTable.get_SymbolList();
	for (int j = 0; j < temp_SymbolList.size(); j++)
	{
		if (temp_SymbolList[j].cat == var_name || temp_SymbolList[j].cat == temp_var_name)
		{
			if (temp_SymbolList[j].cat == var_name)temp_SymbolList[j].is_active = ACTIVE;
			else if (temp_SymbolList[j].cat == temp_var_name)temp_SymbolList[j].is_active = UNACTIVE;
			MainSymbolList.push_back(temp_SymbolList[j]);
			unordere_MainSymbolList[temp_SymbolList[j].name] = temp_SymbolList[j].is_active;
		}
	}
	for (int k = 0; k < temp_FuctionList.size(); k++)
	{
		vector<SymbolListCommon_Type> temp= temp_FuctionList[k];
		for (int m = 0; m < temp.size(); m++)
		{
			if(temp[m].cat== var_name|| temp[m].cat==temp_var_name)
			{
				if (temp[m].cat == var_name)temp[m].is_active = ACTIVE;
				else if (temp[m].cat == temp_var_name)temp[m].is_active = UNACTIVE;
				MainSymbolList.push_back(temp[m]);
				unordere_MainSymbolList[temp[m].name] = temp[m].is_active;
			}
		}
	}
	

	for (int i = TempQT.size()-1; i >=0; i--)
	{
		if (TempQT[i].ttt.name != "_" && TempQT[i].ttt.type != digit_constant)
		{
			TempQT[i].ttt.is_active = unordere_MainSymbolList[TempQT[i].ttt.name];
			unordere_MainSymbolList[TempQT[i].ttt.name]= UNACTIVE;
		}
		else TempQT[i].ttt.is_active = A_NULL;

		if (TempQT[i].bbb.name != "_"&& TempQT[i].bbb.type != digit_constant)
		{
			TempQT[i].bbb.is_active = unordere_MainSymbolList[TempQT[i].bbb.name];
			unordere_MainSymbolList[TempQT[i].bbb.name] = i;
		}
		else TempQT[i].bbb.is_active = A_NULL;
		if (TempQT[i].aaa.name != "_" && TempQT[i].aaa.type != digit_constant)
		{
			TempQT[i].aaa.is_active = unordere_MainSymbolList[TempQT[i].aaa.name];
			unordere_MainSymbolList[TempQT[i].aaa.name] = i;
		}
		else TempQT[i].aaa.is_active = A_NULL;
	}
	

}


string object::LOCR(string var)
{
	string Register;
	if (RDL["EAX"] == var)Register= "EAX";
	else if (RDL["EBX"] == var)Register= "EBX";
	else if (RDL["ECX"] == var) Register= "ECX";
	else if (RDL["EDX"] == var) Register ="EDX";

	else 
	{
		for (int i = 0; i < RDLMEM.size(); i++)
		{
			if (RDLMEM[i] == var)
			{
				Register = "MEM";
				MEM_num=i;
			}
		}
		if(Register!="MEM")Register = "NO_ANY";
	}

	

	return Register;
}

string object::is_EMPTY()
{
	string Register;
	if (!RDL["EAX"].size())Register = "EAX";
	else if (!RDL["EBX"].size())Register = "EBX";
	else if (!RDL["ECX"].size()) Register = "ECX";
	else if (!RDL["EDX"].size()) Register = "EDX";

	else  Register = "NO_ANY";
	return Register;
}

int object::object_CODE(string Reg,string Reg1)
{
	objectFileTemp_insert("MOV", Reg, Reg1);
	return 0;
}

void object::GETR(FourQtType qt_Ele, string &R1, string &B1, string &C1)
{
	string Register;
	string Register_empty;
	string QReg;
	Register=LOCR(qt_Ele.aaa.name);
	Register_empty = is_EMPTY();
	if ((Register != "NO_ANY")&& (Register != "MEM"))    
	{
		if (qt_Ele.aaa.is_active >= 0)
		{
			if (Register_empty != "NO_ANY")//有空寄存器
			{
				 //保存活跃变量
				object_CODE(Register_empty, Register);
				RDL[Register_empty] = qt_Ele.aaa.name;  //更新描述表
				RDL[Register] = qt_Ele.ttt.name;
			}
			else
			{
				object_CODE(qt_Ele.aaa.name, Register);//保存活跃变量
				RDLMEM.push_back(qt_Ele.aaa.name);// = MEM_num;

				RDL[Register] = qt_Ele.ttt.name;
			}

			if (LOCR(qt_Ele.ttt.name) == "NO_ANY" || LOCR(qt_Ele.ttt.name) == "MEM")R1 = qt_Ele.ttt.name;
			else R1 = LOCR(qt_Ele.ttt.name);
			//B1 = LOCR(qt_Ele.aaa.name);
			if (LOCR(qt_Ele.bbb.name) == "NO_ANY" || LOCR(qt_Ele.bbb.name) == "MEM")C1 = qt_Ele.bbb.name;
			else C1 = LOCR(qt_Ele.bbb.name);

			//RDL.erase(RDL[Register]);
		}
		else
		{
			if((qt_Ele.ttt.name!="_")&& qt_Ele.ttt.is_active>=0)RDL[Register] = qt_Ele.ttt.name;
			else
			{
				map<string, string>::iterator iter = RDL.begin();
				for (iter; iter != RDL.end();)
				{
					if (iter->first == Register)
						RDL.erase(iter++);   //清除不活沃点6
					else iter++;
				}
	        
			}
				

			if (LOCR(qt_Ele.ttt.name) == "NO_ANY" || LOCR(qt_Ele.ttt.name) == "MEM")R1 = qt_Ele.ttt.name;
			else R1 = LOCR(qt_Ele.ttt.name);
			//B1 = LOCR(qt_Ele.aaa.name);
			if (LOCR(qt_Ele.bbb.name) == "NO_ANY" || LOCR(qt_Ele.bbb.name) == "MEM")C1 = qt_Ele.bbb.name;
			else C1 = LOCR(qt_Ele.bbb.name);
		}
	}

	else if (Register_empty != "NO_ANY"&&qt_Ele.aaa.name != "_")
	{
		object_CODE(Register_empty, qt_Ele.aaa.name);
		//if (qt_Ele.operation == "=")
			RDL[Register_empty] = qt_Ele.ttt.name;
		//else RDL[Register_empty] = qt_Ele.aaa.name;

			if (LOCR(qt_Ele.ttt.name) == "NO_ANY" || LOCR(qt_Ele.ttt.name) == "MEM")R1 = qt_Ele.ttt.name;
			else R1 = LOCR(qt_Ele.ttt.name);
			//B1 = LOCR(qt_Ele.aaa.name);
			if (LOCR(qt_Ele.bbb.name) == "NO_ANY" || LOCR(qt_Ele.bbb.name) == "MEM")C1 = qt_Ele.bbb.name;
			else C1 = LOCR(qt_Ele.bbb.name);
	}
	
	else if (qt_Ele.operation == "+" || qt_Ele.operation == "*")//可交换运算
	{
		Register = LOCR(qt_Ele.bbb.name);
		if (qt_Ele.bbb.is_active >= 0)
		{
			object_CODE(qt_Ele.bbb.name, Register);//保存活跃变量
			RDLMEM.push_back(qt_Ele.bbb.name);// = MEM_num;

			RDL[Register] = qt_Ele.ttt.name;
		}

		else
		{
			RDL[LOCR(qt_Ele.ttt.name)].erase();
			RDL[Register] = qt_Ele.ttt.name;
		}

		if (LOCR(qt_Ele.ttt.name) == "NO_ANY"|| LOCR(qt_Ele.ttt.name) == "MEM")R1 = qt_Ele.ttt.name;
		else R1 = LOCR(qt_Ele.ttt.name);
		//B1 = LOCR(qt_Ele.aaa.name);
		if (LOCR(qt_Ele.aaa.name) == "NO_ANY"|| LOCR(qt_Ele.aaa.name) == "MEM")C1 = qt_Ele.aaa.name;
		else C1 = LOCR(qt_Ele.aaa.name);
	}
	else if(qt_Ele.aaa.name !="_")
	{

		QReg = "EDX";
		object_CODE(RDL[QReg],QReg);
		object_CODE(QReg, qt_Ele.aaa.name);
		RDLMEM.push_back(RDL[QReg]);
		
		RDL[QReg] = qt_Ele.ttt.name;

		if (LOCR(qt_Ele.ttt.name) == "NO_ANY" || LOCR(qt_Ele.ttt.name) == "MEM")R1 = qt_Ele.ttt.name;
		else R1 = LOCR(qt_Ele.ttt.name);
		//B1 = LOCR(qt_Ele.aaa.name);
		if (LOCR(qt_Ele.bbb.name) == "NO_ANY" || LOCR(qt_Ele.bbb.name) == "MEM")C1 = qt_Ele.bbb.name;
		else C1 = LOCR(qt_Ele.bbb.name);
	}
	else
	{
		;
	}

}


void object::Result_Code()
{
	string R1, B1, C1;
	FourQtType qt_Ele;
	string temp_guanxi;
	static int jump_addr=0;
	for (int i = 0; i < TempQT.size(); i++)
	{
		qt_Ele = TempQT[i];
		GETR(qt_Ele, R1, B1, C1);
		if (qt_Ele.operation == "+")
		{
			objectFileTemp_insert("ADD", R1, C1);
		}
		else if (qt_Ele.operation == "-")
		{
			objectFileTemp_insert("SUB", R1, C1);
		}
		else if (qt_Ele.operation == "*")
		{
			objectFileTemp_insert("IMUL", R1, C1);
		}
		else if (qt_Ele.operation == "/")
		{

		}
		else if (qt_Ele.operation == "=")
		{
			
		}
		else if (qt_Ele.operation == ">"|| qt_Ele.operation == "<"|| qt_Ele.operation == ">="|| qt_Ele.operation == "<="
			|| qt_Ele.operation == "=="|| qt_Ele.operation == "!=")
		{
			temp_guanxi = qt_Ele.operation;
			objectFileTemp_insert("CMP", R1, C1);			
		}
		else if (qt_Ele.operation == "if")
		{
			if (RDL["EAX"].size())objectFileTemp_insert("MOV", RDL["EAX"], "EAX");
			if (RDL["EBX"].size())objectFileTemp_insert("MOV", RDL["EBX"], "EBX");
			if (RDL["ECX"].size())objectFileTemp_insert("MOV", RDL["ECX"], "ECX");
			if (RDL["EDX"].size())objectFileTemp_insert("MOV", RDL["EDX"], "EDX");

			map<string, string>::iterator iter = RDL.begin();
			for (iter; iter != RDL.end();)
			{
					RDL.erase(iter++);   //清除不活沃点6
			}

			if(temp_guanxi==">")objectFileTemp_insert("JNG", "?", "_");
			else if (temp_guanxi == "<")objectFileTemp_insert("JNL", "?", "_");
			else if(temp_guanxi == ">=")objectFileTemp_insert("JL", "?", "_");
			else if (temp_guanxi == "<=")objectFileTemp_insert("JG", "?", "_");
			else if (temp_guanxi == "==")objectFileTemp_insert("JNE", "?", "_");
			else if (temp_guanxi == "!=")objectFileTemp_insert("JE", "?", "_");
			jump_addr = objectFileTemp.size() - 1;
			
		}
		else if (qt_Ele.operation == "else")
		{
			if (RDL["EAX"].size())objectFileTemp_insert("MOV", RDL["EAX"], "EAX");
			if (RDL["EBX"].size())objectFileTemp_insert("MOV", RDL["EBX"], "EBX");
			if (RDL["ECX"].size())objectFileTemp_insert("MOV", RDL["ECX"], "ECX");
			if (RDL["EDX"].size())objectFileTemp_insert("MOV", RDL["EDX"], "EDX");

			map<string, string>::iterator iter = RDL.begin();
			for (iter; iter != RDL.end();)
			{
				RDL.erase(iter++);   //清除不活沃点6
			}


			objectFileTemp_insert("JMP", "?", "_");
			objectFileTemp[jump_addr].r1 = "EL";
			jump_addr = objectFileTemp.size() - 1;
			objectFileTemp_insert("EL:", "", "_");
			
		}
		else if (qt_Ele.operation == "if_e")
		{
			objectFileTemp_insert("IFEND:", "", "_");
			objectFileTemp[jump_addr].r1 = "IFEND";

		}
		else if (qt_Ele.operation == "while")
		{
			if (temp_guanxi == ">")objectFileTemp_insert("JNG", "?", "_");
			else if (temp_guanxi == "<")objectFileTemp_insert("JNL", "?", "_");
			else if (temp_guanxi == ">=")objectFileTemp_insert("JL", "?", "_");
			else if (temp_guanxi == "<=")objectFileTemp_insert("JG", "?", "_");
			else if (temp_guanxi == "==")objectFileTemp_insert("JNE", "?", "_");
			else if (temp_guanxi == "!=")objectFileTemp_insert("JE", "?", "_");
		}



	}
	
	for (int i = 0; i < objectFileTemp.size(); i++)
	{
		objectFile << objectFileTemp[i].op << '\t' << objectFileTemp[i].r1;
			if(objectFileTemp[i].r2!="_")objectFile<< ',' << objectFileTemp[i].r2;
			else  ;
			objectFile << '\n';
	}

	objectFile << '\n' << "ret" << '\n' ;
	objectFile << "main ENDP" << '\n';
	objectFile << "END" << '\n';
	objectFile.close();
}

void object::objectFileTemp_insert(string op, string r1, string r2)
{
	objectFile_Type temp;
	temp.op = op;
	temp.r1 = r1;
	temp.r2 = r2;
	objectFileTemp.push_back(temp);
}

void object::MainSymbolList_print(void)
{
	//输出所有变量符号表

		cout << endl << "总符号表如下：" << endl;
		for (int i = 0; i < MainSymbolList.size(); i++)
			cout << MainSymbolList[i].name << "," << MainSymbolList[i].type << "," << MainSymbolList[i].cat << "," << MainSymbolList[i].is_active << endl;
}
