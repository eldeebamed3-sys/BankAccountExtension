//Update On Bank Account
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
#include <fstream>
#include<cstdlib>
using namespace std;
const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";

enum enMainMenuePermission
{
	Alls = -1,
	pShowClients = 1,
	pAddNewClients = 2,
	pDeleteClients = 4,
	pUpdateClients = 8,
	pFindClient = 16,
	pTransactions = 32,
	pManageUsers = 64,
};

void ShowMainMenue();
void ShowTransactionMainMenue();
void ShowManageUsersManue();
bool ChackAccessPermission(enMainMenuePermission Permission);
void Login();

struct sClient
{
	string AccountNumber;
	string PinCode;
	string ClientName;
	string Phone;
	double AccountBalance = 0;
	bool MarkOfDelete = false;
};

struct sUsers
{
	string UserName;
	string Password;
	int Permissions = 0;
	bool MarkOfDelete = false;
};

sUsers CurrentUser;

vector<string>SplitString(string S1, string Delim)
{
	vector<string>vString;
	short Pos = 0;
	string sWord;

	while ((Pos = (short)S1.find(Delim)) != std::string::npos)
	{
		sWord = S1.substr(0, Pos);
		if (sWord != " ")
		{
			vString.push_back(sWord);
		}
		S1.erase(0, Pos + Delim.length());
	}
	if (S1 != " ")
	{
		vString.push_back(S1);
	}

	return vString;
}

sClient ConvertLineOfRecord(string Line, string Sperator = " - ")
{
	sClient Client;
	vector <string>vClientData;
	vClientData = SplitString(Line, Sperator);

	Client.AccountNumber = vClientData[0];
	Client.PinCode = vClientData[1];
	Client.ClientName = vClientData[2];
	Client.Phone = vClientData[3];
	Client.AccountBalance = stod(vClientData[4]);

	return Client;
}

sUsers ConvertStringToRecord(string Line, string Sperator = " - ")
{
	sUsers User;
	vector<string>vUser;
	vUser = SplitString(Line, Sperator);

	User.UserName = vUser[0];
	User.Password = vUser[1];
	User.Permissions = stoi(vUser[2]);

	return User;
}

string ConvertRecordOfLine(sClient Client, string Sperator = " - ")
{
	string Line;

	Line += Client.AccountNumber + Sperator;
	Line += Client.PinCode + Sperator;
	Line += Client.ClientName + Sperator;
	Line += Client.Phone + Sperator;
	Line += to_string(Client.AccountBalance);

	return Line;
}

string ConvertRecordToString(sUsers User, string Sperator = " - ")
{
	string Line;
	Line += (User.UserName) + Sperator;
	Line += (User.Password) + Sperator;
	Line += to_string(User.Permissions);

	return Line;
}

vector<sClient>LoadClientDataFromFile(string FileName)
{
	vector<sClient>vClient;
	fstream MyFile;

	MyFile.open(FileName, ios::in);
	if (MyFile.is_open())
	{
		string Line;
		sClient Client;

		while (getline(MyFile, Line))
		{
			Client = ConvertLineOfRecord(Line);
			vClient.push_back(Client);
		}
		MyFile.close();
	}
	return vClient;
}

vector<sUsers>LoadUserDateFromFile(string FileName)
{
	vector<sUsers>vUser;
	fstream MyFile;

	MyFile.open(FileName, ios::in);
	if (MyFile.is_open())
	{
		string Line;
		sUsers User;

		while (getline(MyFile, Line))
		{
			User = ConvertStringToRecord(Line);
			vUser.push_back(User);
		}
		MyFile.close();
	}
	return vUser;
}

bool ClientExistClientByAccountNumber(string AccountNumber, sClient& Client)
{
	vector<sClient>vClient = LoadClientDataFromFile(ClientsFileName);

	for (sClient& C : vClient)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}

bool UserExsitUserByUserName(string UserName, sUsers& User)
{
	vector<sUsers>vUser = LoadUserDateFromFile(UsersFileName);

	for (sUsers& C : vUser)
	{
		if (C.UserName == UserName)
		{
			User = C;
			return true;
		}
	}
	return false;
}

sClient ReadClientData()
{
	sClient Client;

	cout << "Enter Account Number? ";
	getline(cin >> ws, Client.AccountNumber);

	sClient TempClient;
	while (ClientExistClientByAccountNumber(Client.AccountNumber, TempClient))
	{
		cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
		getline(cin, Client.AccountNumber);
	}

	cout << "Enter Pin Code? ";
	getline(cin, Client.PinCode);

	cout << "Enter Client Name? ";
	getline(cin, Client.ClientName);

	cout << "Enter Phone? ";
	getline(cin, Client.Phone);

	cout << "Enter Account Balance? ";
	cin >> Client.AccountBalance;

	return Client;
}

int ReadPermissionsToSet()
{
	int Permission = 0;
	char Answer = 'n';

	cout << "\n Do you want to give access?y/n? ";
	cin >> Answer;
	if (Answer == 'Y' || Answer == 'y')
	{
		return -1;
	}

	cout << "\n Do you want to give access to: \n";
	cout << "\n Show Client List?y/n? ";
	cin >> Answer;
	if (Answer == 'Y' || Answer == 'y')
	{
		Permission += enMainMenuePermission::pShowClients;
	}

	cout << "\n Add New Client?y/n? ";
	cin >> Answer;
	if (Answer == 'Y' || Answer == 'y')
	{
		Permission += enMainMenuePermission::pAddNewClients;
	}

	cout << "\n Delete Client?y/n? ";
	cin >> Answer;
	if (Answer == 'Y' || Answer == 'y')
	{
		Permission += enMainMenuePermission::pDeleteClients;
	}

	cout << "\n UpDate Client?y/n? ";
	cin >> Answer;
	if (Answer == 'Y' || Answer == 'y')
	{
		Permission += enMainMenuePermission::pUpdateClients;
	}

	cout << "\n Find Client?y/n? ";
	cin >> Answer;
	if (Answer == 'Y' || Answer == 'y')
	{
		Permission += enMainMenuePermission::pFindClient;
	}

	cout << "\n Transactions?y/n? ";
	cin >> Answer;
	if (Answer == 'Y' || Answer == 'y')
	{
		Permission += enMainMenuePermission::pTransactions;
	}
	cout << "\n Manage Users?y/n? ";
	cin >> Answer;
	if (Answer == 'Y' || Answer == 'y')
	{
		Permission += enMainMenuePermission::pManageUsers;
	}

	return Permission;
}

sUsers ReadUserData()
{
	sUsers User;

	cout << "Enter Username? ";
	getline(cin >> ws, User.UserName);
	
	while (UserExsitUserByUserName(User.UserName, User))
	{
		cout << "\nUser with [" << User.UserName << "] already exist, Enter another UserName? ";
		getline(cin >> ws, User.UserName);
	}

	cout << "Enter Password? ";
	getline(cin, User.Password);

	User.Permissions = ReadPermissionsToSet();

	return User;
}
//_______________________________________________________________

void ShowAccessDeniedMassage()
{
	cout << "\n------------------------------------------\n";
	cout << "Access Denied, \nYou Don't Have Permission to do this, \nPlease Conect to Admin.";
	cout << "\n------------------------------------------\n";
}

void AddClientsDataOfFile(string FileName, string ClientsLine)
{
	fstream MyFile;

	MyFile.open(FileName, ios::out | ios::app);
	if (MyFile.is_open())
	{
		MyFile << ClientsLine << endl;
		MyFile.close();
	}
}
void AddNewClients()
{
	sClient Client;

	Client = ReadClientData();
	AddClientsDataOfFile(ClientsFileName, ConvertRecordOfLine(Client));
}
void AddClients()
{
	char AddMore = 'Y';
	do
	{
		system("cls");
		cout << "\n-------------------------------------------------" << endl;
		cout << "\t Add New Clients Screen " << endl;
		cout << "-------------------------------------------------" << endl;
		
		cout << "Adding New Client:\n\n";
		AddNewClients();

		cout << "\nClient Added Successfully, do you want to add Add more clients? Y/N?";
		cin >> AddMore;
	} while (toupper(AddMore) == 'Y');
}

void AddNewUsers()
{
	sUsers User;
	User = ReadUserData();
	AddClientsDataOfFile(UsersFileName, ConvertRecordToString(User));
}
void AddUsers()
{
	char AddMore = 'Y';
	do
	{
		system("cls");
		cout << "\n-------------------------------------------------" << endl;
		cout << "\t Add New Users Screen " << endl;
		cout << "-------------------------------------------------" << endl;

		cout << "Add New User: \n\n";
		AddNewUsers();

		cout << "\nUser Added Successfully, do you want to add Add more Users? Y/N?";
		cin >> AddMore;
		
	} while (toupper(AddMore) == 'Y');
}

//-----------------Show Client Data----------------------------------------
void PrintClientData(sClient Client)
{
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(10) << left << Client.PinCode;
	cout << "| " << setw(40) << left << Client.ClientName;
	cout << "| " << setw(15) << left << Client.Phone;
	cout << "| " << setw(10) << left << Client.AccountBalance;
}

void ShowClientList(vector<sClient>vClient)
{
	cout << "\n\t\t\t\t\t Client List (" << vClient.size() << ") Client(s)." << endl;

	cout << "__________________________________________________________";
	cout << "__________________________________________________________\n" << endl;
	cout << "| " << setw(15) << right << "Account Number ";
	cout << "| " << setw(10) << left << "Pin Code ";
	cout << "| " << setw(40) << left << "Client Name ";
	cout << "| " << setw(15) << left << "Phone ";
	cout << "| " << setw(15) << left << "Balance ";
	cout << "\n__________________________________________________________";
	cout << "__________________________________________________________\n" << endl;
	
	if (vClient.size() == 0)
		cout << "\t\t\tNo Clients Aveilable In The System!";
		
	else
	
	for (sClient& Client : vClient)
	{
		PrintClientData(Client);
		cout << endl;
	}
	cout << "\n__________________________________________________________";
	cout << "__________________________________________________________\n" << endl;
}

void PrintUserDate(sUsers User)
{
	cout << "| " << setw(15) << left << User.UserName;
	cout << "| " << setw(10) << left << User.Password;
	cout << "| " << setw(10) << left << User.Permissions;
}
void ShowUserList(vector<sUsers>vUser)
{
	cout << "\n\t\t\t\t\t Users List (" << vUser.size() << ") User(s)." << endl;
	cout << "___________________________________________________________";
	cout << "__________________________________________________________\n" << endl;
	cout << "| " << setw(15) << left << "User Name ";
	cout << "| " << setw(10) << left << "Password ";
	cout << "| " << setw(10) << left << "Permission ";
	cout << "\n___________________________________________________________";
	cout << "_________________________________________________________\n" << endl;

	if (vUser.size() == 0)
	{
		cout << "\t\t\tNo Users Aveilable In The System!";
	}
	else
	{
		for (sUsers &User : vUser)
		{
			PrintUserDate(User);
			cout << "\n";
		}
		cout << "\n___________________________________________________________";
		cout << "__________________________________________________________\n" << endl;
	}
}
//---------------------------------------------------------

void PrintDataClientOfLine(sClient Client)
{
	cout << "\nThe following are the client details: " << endl;
	cout << "--------------------------------------\n";
	cout << "Account Number : " << Client.AccountNumber;
	cout << "\nPin Code       : " << Client.PinCode;
	cout << "\nClient Name    : " << Client.ClientName;
	cout << "\nPhone          : " << Client.Phone;
	cout << "\nAccount Balance: " << Client.AccountBalance;
	cout << "\n--------------------------------------\n";
}

void PrintDataUserOfString(sUsers User)
{
	cout << "\nthe following are the cient dateils:\n";
	cout << "-------------------------------------\n";
	cout << "Username   : " << User.UserName;
	cout << "\nPassword   : " << User.Password;
	cout << "\nPermission : " << User.Permissions;
	cout << "\n-------------------------------------\n";
}

bool FindClientByAccountNumber(string AccountNumber, vector<sClient>vClient, sClient& Client)
{
	for (sClient& C : vClient)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}

bool FindUserByUserName(string UserName,vector<sUsers>vUser , sUsers& User)
{
	for (sUsers& C : vUser)
	{
		if (C.UserName == UserName)
		{
			User = C;
			return true;
		}
	}
	return false;
}

bool FindUserByUserNameAndPassword(string UserName, string Password, sUsers& User)
{
	vector<sUsers>vUser = LoadUserDateFromFile(UsersFileName);
	for (sUsers& C : vUser)
	{
		if (C.UserName == UserName && C.Password == Password)
		{
			User = C;
			return true;
		}
	}
	return false;
}

bool MarkOfDeleteClientFromFile(string AccountNumber, vector<sClient>& vClient)
{
	for (sClient& C : vClient)
	{
		if (C.AccountNumber == AccountNumber)
		{
			C.MarkOfDelete = true;
			return true;
		}
	}
	return false;
}

bool MarkOfDeleteuserFromFile(string userName, vector<sUsers>& vUser)
{
	for (sUsers& C : vUser)
	{
		if (C.UserName == userName)
		{
			C.MarkOfDelete = true;
			return true;
		}
	}
	return false;
}

vector<sClient>SaveClientDataFromFile(string FileName, vector<sClient>& vClient)
{
	fstream MyFile;

	MyFile.open(FileName, ios::out);
	string DataLine;
	if (MyFile.is_open())
	{
		for (sClient C : vClient)
		{
			if (C.MarkOfDelete == false)
			{
				DataLine = ConvertRecordOfLine(C);
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
	return vClient;
}

vector<sUsers>SaveUserDateFromFile(string FileName, vector<sUsers>& vUser)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);
	string DataLine;
	if (MyFile.is_open())
	{
		for (sUsers C : vUser)
		{
			if (C.MarkOfDelete == false)
			{
				DataLine = ConvertRecordToString(C);
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
	return vUser;
}

bool DeleteClientByAccountNumber(string AccountNumber, vector<sClient>& vClient)
{
	sClient Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClient, Client))
	{
		cout << "\n-------------------------------------------------" << endl;
		cout << "\t Delete Client Screen " << endl;
		cout << "-------------------------------------------------" << endl;
		PrintDataClientOfLine(Client);

		cout << "\n\nAre you want to delete this client? Y/N? ";
		cin >> Answer;
		if (Answer == 'Y' || Answer == 'y')
		{
			MarkOfDeleteClientFromFile(AccountNumber, vClient);
			SaveClientDataFromFile(ClientsFileName, vClient);

			cout << "\n\nClient Deleted Successfully.";
			return true;
		}
	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!\n";
		return false;
	}

	return false;
}



bool DeleteUserByUserName(string UserName, vector<sUsers>& vUser)
{
	sUsers User;
	char Answer = 'n';

	if (FindUserByUserName(UserName, vUser, User))
	{
		cout << "\n-------------------------------------------------" << endl;
		cout << "\t Delete Client Screen " << endl;
		cout << "-------------------------------------------------" << endl;
		PrintDataUserOfString(User);

		cout << "\n\nAre you want to delete this User? Y/N? ";
		cin >> Answer;
		if (Answer == 'Y' || Answer == 'y')
		{
			MarkOfDeleteuserFromFile(UserName, vUser);
			SaveUserDateFromFile(UsersFileName, vUser);

			cout << "\n\nUser Dalete Successfully.\n";
			return true;
		}
	}
	else
	{
		cout << "\nUser with UserName (" << UserName << ") is Not Found!\n";
		return false;
	}
	return false;
}

//----------------Update-----------------------------------------
sClient ChanageClientRecord(string AccountNumber)
{
	sClient Client;
	Client.AccountNumber = AccountNumber;

	cout << "\nEnter PinCode? ";
	getline(cin >> ws, Client.PinCode);

	cout << "Enter Name? ";
	getline(cin, Client.ClientName);

	cout << "Enter Phone? ";
	getline(cin, Client.Phone);

	cout << "Enter AccountBalance? ";
	cin >> Client.AccountBalance;

	return Client;
}

sUsers ChanageUserRecord(string UserName)
{
	sUsers User;
	User.UserName = UserName;

	cout << "\nEnter PassWord? ";
	getline(cin >> ws, User.Password);

	return User;
}

bool UpdateClientByAccountNumber(string AccountNumber, vector<sClient>& vClient)
{
	sClient Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClient, Client))
	{
		PrintDataClientOfLine(Client);

		cout << "\n\nAre you want to Update this client? Y/N? ";
		cin >> Answer;
		if (Answer == 'Y' || Answer == 'y')
		{
			for (sClient& C : vClient)
			{
				if (C.AccountNumber == AccountNumber)
				{
					C = ChanageClientRecord(AccountNumber);
					break;
				}
			}

			SaveClientDataFromFile(ClientsFileName, vClient);
			cout << "\n\nClient Update Successfully.\n";
			return true;
		}
	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!\n";
		return false;
	}
}

bool UpdateUserByUserName(string UserName, vector<sUsers>& vUser)
{
	sUsers User;
	char Answer = 'n';

	if (FindUserByUserName(UserName, vUser, User))
	{
		PrintDataUserOfString(User);

		cout << "\nDo you want to give full access?Y/N? ";
		cin >> Answer;
		if (Answer == 'Y' || Answer == 'y')
		{
			for (sUsers C : vUser)
			{
				if (C.UserName == UserName)
				{
					C = ChanageUserRecord(UserName);
					break;
				}
			}
			SaveUserDateFromFile(UsersFileName, vUser);
			cout << "\n\n User Update Successfully.\n";
		}
	}
	else
	{
		cout << "\nUser with UserName (" << UserName << ") is Not Found!\n";
		return false;
	}
	return false;
}

//-----------------------------------------------------------------

void displayTotalBalance(sClient Client)
{
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(40) << left << Client.ClientName;
	cout << "| " << setw(15) << left << Client.AccountBalance;
}

void ShowTotalBalanceOfClients(vector<sClient>vClient)
{
	sClient Client;
	
	cout << "\t\t\t\t\t Balance List (" << vClient.size() << ") Clients(s).\n";
	cout << "__________________________________________________________";
	cout << "__________________________________________________________\n" << endl;;
	cout << "| " << setw(15) << left << "Account Number ";
	cout << "| " << setw(40) << left << "Client Name ";
	cout << "| " << setw(15) << left << "Balance ";
	cout << "\n__________________________________________________________";
	cout << "__________________________________________________________\n" << endl;
	double TotalBalance = 0;

	if (vClient.size() == 0)
		cout << "\t\t\t No Clients Available In The System!";

	else

	for (sClient& Client : vClient)
	{
		displayTotalBalance(Client);
		TotalBalance += Client.AccountBalance;
		cout << endl;
	}
	cout << "\n__________________________________________________________";
	cout << "__________________________________________________________\n" << endl;

	cout << "\t\t\t\t\t Total Balances = " << TotalBalance << endl;
}

bool DepositClientByAccountNumber(string AccountNumber, vector<sClient>& vClient, double Amount)
{
	sClient Client;
	char Answer = 'n';

	cout << "\nAre you sure you want perfrom this transactions?Y/N? ";
	cin >> Answer;
	if (Answer == 'Y' || Answer == 'y')
	{
		for (sClient& C : vClient)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance += Amount;
				SaveClientDataFromFile(ClientsFileName, vClient);
				cout << "\n\nDone Successfully. New Balance is " << C.AccountBalance;

				return true;
			}
		}
		return false;
	}
}

string ReadAccountNumber()
{
	string AccountNumber = " ";

	cout << "\nPlease enter Account Number? ";
	cin >> AccountNumber;

	return AccountNumber;
}

string ReadUserName()
{
	string UserName;
	cout << "\nPlease enter Username? ";
	cin >> UserName;
	return UserName;
}

void ShowDepositScreen()
{
	cout << "\n------------------------------------------\n";
	cout << "\t\tDeposit Screen";
	cout << "\n------------------------------------------\n";

	sClient Client;
	vector<sClient>vClient = LoadClientDataFromFile(ClientsFileName);
	string AccountNumber = ReadAccountNumber();

	while (!FindClientByAccountNumber(AccountNumber, vClient, Client))
	{
		cout << "\nClient with [" << AccountNumber << "] dose not exist.\n";
		AccountNumber = ReadAccountNumber();	
	}

	PrintDataClientOfLine(Client);
	
	double Amount = 0;
	cout << "\nPlease enter deposit amount? ";
	cin >> Amount;

	DepositClientByAccountNumber(AccountNumber, vClient, Amount);
}

void ShowWithdrawScreen()
{
	cout << "\n------------------------------------------\n";
	cout << "\t\tWithdraw Screen";
	cout << "\n------------------------------------------\n";
	sClient Client;
	vector<sClient>vClient = LoadClientDataFromFile(ClientsFileName);
	string AccountNumber = ReadAccountNumber();

	while (!FindClientByAccountNumber(AccountNumber, vClient, Client))
	{
		cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
		AccountNumber = ReadAccountNumber();
	}

	PrintDataClientOfLine(Client);

	double Amount = 0;
	cout << "\nPlease enter withdraw amount? ";
	cin >> Amount;

	while (Amount > Client.AccountBalance)
	{
		cout << "\nAmount Exceeds the balance, you can withdraw up to : " << Client.AccountBalance << endl;
		cout << "\nPlease enter onther Amount? ";
		cin >> Amount;
	}

	DepositClientByAccountNumber(AccountNumber, vClient, Amount * -1);
}

//------------------------------------------------------
void ShowListScreen()
{
	if (!ChackAccessPermission(enMainMenuePermission::pShowClients))
	{
		ShowAccessDeniedMassage();
		return;
	}

	vector<sClient>vClient = LoadClientDataFromFile(ClientsFileName);
	ShowClientList(vClient);
}

void ShowAddScreen()
{
	if (!ChackAccessPermission(enMainMenuePermission::pAddNewClients))
	{
		ShowAccessDeniedMassage();
		return;
	}
	cout << "\n-------------------------------------------------" << endl;
	cout << "\t Add New Clients Screen " << endl;
	cout << "-------------------------------------------------" << endl;
	AddClients();
}

void ShowDeleteClientScreen()
{
	if (!ChackAccessPermission(enMainMenuePermission::pDeleteClients))
	{
		ShowAccessDeniedMassage();
		return;
	}
	cout << "\n-------------------------------------------------" << endl;
	cout << "\t Delete Client Screen " << endl;
	cout << "\n-------------------------------------------------" << endl;

	vector<sClient>vClient = LoadClientDataFromFile(ClientsFileName);
	string AccountNumber = ReadAccountNumber();
	DeleteClientByAccountNumber(AccountNumber, vClient);
}

void ShowUpdateScreen()
{
	if (!ChackAccessPermission(enMainMenuePermission::pUpdateClients))
	{
		ShowAccessDeniedMassage();
		return;
	}

	cout << "\n-------------------------------------------------" << endl;
	cout << "\t Update Client Screen " << endl;
	cout << "-------------------------------------------------" << endl;
	vector<sClient>vClient = LoadClientDataFromFile(ClientsFileName);
	string AccountNumber = ReadAccountNumber();
	UpdateClientByAccountNumber(AccountNumber, vClient);
}

void ShowFindScreen()
{
	if (!ChackAccessPermission(enMainMenuePermission::pFindClient))
	{
		ShowAccessDeniedMassage();
		return;
	}

	cout << "\n-------------------------------------------------" << endl;
	cout << "\t  Find Client Screen " << endl;
	cout << "-------------------------------------------------" << endl;
	vector<sClient>vClient = LoadClientDataFromFile(ClientsFileName);
	sClient Client;
	string AccountNumber = ReadAccountNumber();
	if (ClientExistClientByAccountNumber(AccountNumber, Client))
	{
		PrintDataClientOfLine(Client);
	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") Not Found!";
	}
}
//-----------------------------------------------------------------

bool ChackAccessPermission(enMainMenuePermission Permission)
{
	if (CurrentUser.Permissions == enMainMenuePermission::Alls)
	{
		return true;
	}
	if ((Permission & CurrentUser.Permissions) == Permission)
		return true;
	else
		return false;
}

void GoBackMainMenu()
{
	cout << "\n\nPress any to key to go back to Main Menue...";
	system("Pause>0");
	ShowMainMenue();
}

void GoBackTransactionMenu()
{
	cout << "\n\nPress any to key to go back to Main Menue...";
	system("Pause>0");
    ShowTransactionMainMenue();
}

void GoBackMansgeMenue()
{
	cout << "\n\nPress any to key to go back to Main Menue...";
	system("Pause>0");
	ShowManageUsersManue();
}

enum enTransactionsOption
{
	eDeposit = 1,
	eWithdraw = 2,
	eTotalBalance = 3,
	eMainMenu = 4
};

short ReadTransctionsMenueOption()
{
	short Option = 0;
	cout << "Choose what do you want to do? [1 to 4]? ";
	cin >> Option;

	while (cin.fail() || Option < 1 || Option > 4)
	{
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cout << "Invalid Number, Enter Number from [1 to 4]: ";
		cin >> Option;
	}
	return Option;
}

void PerformTransactionMenueOption(enTransactionsOption TransactionsOption)
{
	double Amount = 0;
	string AccountNumber;
	sClient Client;
	vector<sClient>vClient = LoadClientDataFromFile(ClientsFileName);

	switch (TransactionsOption)
	{
	case enTransactionsOption::eDeposit:
	{
		system("cls");
		ShowDepositScreen();
		GoBackTransactionMenu();
		break;
	}
	case enTransactionsOption::eWithdraw:
	{
		system("cls");
		ShowWithdrawScreen();
		GoBackTransactionMenu();
		break;
	}
	case enTransactionsOption::eTotalBalance:
	{
		system("cls");
		ShowTotalBalanceOfClients(vClient);
		GoBackTransactionMenu();
		break;
	}
	case enTransactionsOption::eMainMenu:
	{
		ShowMainMenue();
		break;
	}
	default:
	{
		cout << "\nInvalid Option";
		break;
	}

	}
}

void ShowTransactionMainMenue()
{
	if (!ChackAccessPermission(enMainMenuePermission::pTransactions))
	{
		ShowAccessDeniedMassage();
		GoBackMainMenu();
		return;
	}

	system("cls");
	cout << "=================================================\n";
	cout << "\t\t Transactions Menue Screen           \n";
	cout << "=================================================\n";
	cout << "\t [1] Deposit.\n";
	cout << "\t [2] Withdraw.\n";
	cout << "\t [3] Total Balances.\n";
	cout << "\t [4] Main Menue.\n";
	cout << "=================================================\n";

	PerformTransactionMenueOption((enTransactionsOption)ReadTransctionsMenueOption());
}

short ReadManageUsersOption()
{
	short Option = 0;
	cout << "Choose what do you want to do? [1 to 6]? ";
	cin >> Option;

	while (cin.fail() || Option < 1 || Option > 6)
	{
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cout << "Invalid Number, Enter Number from [1 to 6]: ";
		cin >> Option;
	}
	return Option;
}

enum enMansgeUsers
{
	eListUsers = 1,
	eAddUsers = 2,
	eDeleteUsers = 3,
	eUpdateUsers = 4,
	eFindUsers = 5,
	eMainMenue = 6
};

void PerfromManageUsersMenueoption(enMansgeUsers ManageUsers)
{
	sUsers User;
	vector<sUsers>vUser;
	string UserName = " ";

	switch (ManageUsers)
	{
	case enMansgeUsers::eListUsers:
	{
		system("cls");
		vUser = LoadUserDateFromFile(UsersFileName);
		ShowUserList(vUser);
		GoBackMansgeMenue();
		break;
	}
	case enMansgeUsers::eAddUsers:
	{
		system("cls");
		cout << "\n-------------------------------------------------" << endl;
		cout << "\t Add User Screen " << endl;
		cout << "-------------------------------------------------" << endl;
		AddUsers();
		GoBackMansgeMenue();
		break;
	}
	case enMansgeUsers::eDeleteUsers:
	{
		system("cls");
		cout << "\n-------------------------------------------------" << endl;
		cout << "\t Delete User Screen " << endl;
		cout << "-------------------------------------------------" << endl;
		vUser = LoadUserDateFromFile(UsersFileName);
		UserName = ReadUserName();
		DeleteUserByUserName(UserName, vUser);
		GoBackMansgeMenue();
		break;
	}
	case enMansgeUsers::eUpdateUsers:
	{
		system("cls");
		cout << "\n-------------------------------------------------" << endl;
		cout << "\t Update User Screen " << endl;
		cout << "-------------------------------------------------" << endl;
		vUser = LoadUserDateFromFile(UsersFileName);
		UserName = ReadUserName();
		UpdateUserByUserName(UserName, vUser);
		GoBackMansgeMenue();
		break;
	}
	case enMansgeUsers::eFindUsers:
	{
		system("cls");
		cout << "\n-------------------------------------------------" << endl;
		cout << "\t Find User Screen " << endl;
		cout << "-------------------------------------------------" << endl;
		UserName = ReadUserName();
		if (UserExsitUserByUserName(UserName, User))
		{
			PrintDataUserOfString(User);
		}
		else
		{
			cout << "\nUser with Account Number (" << UserName << ") Not Found!";
		}
		
		GoBackMansgeMenue();
		break;
	}
	case enMansgeUsers::eMainMenue:
	{
		ShowMainMenue();
		break;
	}

	}
}

void ShowManageUsersManue()
{
	if (!ChackAccessPermission(enMainMenuePermission::pManageUsers))
	{
		ShowAccessDeniedMassage();
		GoBackMainMenu();
		return;
	}

	system("cls");
	cout << "=================================================\n";
	cout << "\t Manage Users Manue Menue Screen           \n";
	cout << "=================================================\n";
	cout << "\t [1] List Users.\n";
	cout << "\t [2] Add New Users.\n";
	cout << "\t [3] Delete Users.\n";
	cout << "\t [4] Update Users.\n";
	cout << "\t [5] Find Users.\n";
	cout << "\t [6] Main Menue.\n";
	cout << "=================================================\n";

	PerfromManageUsersMenueoption((enMansgeUsers)ReadManageUsersOption());
}

short ReadMainMenueOption()
{
	short Choice = 0;
	cout << "Choose what do you want to do? [1 to 8]? ";
	cin >> Choice;

	while (cin.fail() || Choice < 1 || Choice > 8)
	{
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cout << "Invalid Number, Enter Number from [1 to 8]: ";
		cin >> Choice;
	}
	return Choice;
}

enum enMainMenueOption
{
	eListClients = 1,
	eAddClient = 2,
	eDeleteClient = 3,
	eUpdateClient = 4,
	eFindClient = 5,
	eShowTransactions = 6,
	eShowManageUser = 7,
	eExsit = 8
};

void PerformMainMenuOption(enMainMenueOption MainMenueOption)
{
	sClient Client;
	vector<sClient>vClient;
	vClient = LoadClientDataFromFile(ClientsFileName);
	string AccountNumber = " ";

	switch (MainMenueOption)
	{
	case enMainMenueOption::eListClients:
	{
		system("cls");
		ShowListScreen();
		GoBackMainMenu();
		break;
	}
	case enMainMenueOption::eAddClient:
	{
		system("cls");
		ShowAddScreen();
		GoBackMainMenu();
		break;
	}
	case enMainMenueOption::eDeleteClient:
	{
		system("cls");
		ShowDeleteClientScreen();
		GoBackMainMenu();
		break;
	}
	case enMainMenueOption::eUpdateClient:
	{
		system("cls");
		ShowUpdateScreen();
		GoBackMainMenu();
		break;
	}
	case enMainMenueOption::eFindClient:
	{
		system("cls");
		ShowFindScreen();
		GoBackMainMenu();
		break;
	}
	case enMainMenueOption::eShowTransactions:
	{
		system("cls");
		ShowTransactionMainMenue();
		break;
	}
	case enMainMenueOption::eShowManageUser:
	{
		system("cls");
		ShowManageUsersManue();
		break;
	}
	case enMainMenueOption::eExsit:
	{
		system("cls");
		Login();
		break;
	}
	default:
	
		cout << "Invalid Option!\n";
		break;
	}
}

void ShowMainMenue()
{
	system("cls");
	cout << "=================================================\n";
	cout << "\t\t Main Menue Screen           \n";
	cout << "=================================================\n";
	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client Info.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Transaction.\n";
	cout << "\t[7] Manage Users.\n";
	cout << "\t[8] Logout.\n";
	cout << "=================================================\n";
	PerformMainMenuOption((enMainMenueOption)ReadMainMenueOption());
}

bool LoadUserInfo(string UserName, string Password)
{
	if (FindUserByUserNameAndPassword(UserName, Password, CurrentUser))
		return true;
	else
		return false;
}

void Login()
{
	bool LoginFaild = false;
	string UserName, Password;
	do
	{
		system("cls");
		cout << "\n-------------------------------------------------" << endl;
		cout << "\t\t Login Screen " << endl;
		cout << "-------------------------------------------------" << endl;

		if (LoginFaild)
		{
			cout << "Invlaid Username/Password!\n";
		}

		cout << "Enter Usernamer? ";
		cin >> UserName;
		cout << "Enter Password? ";
		cin >> Password;

		LoginFaild = !LoadUserInfo(UserName, Password);
	
	} while (LoginFaild);

	ShowMainMenue();
}

int main()
{
	Login();

	system("Pause>0");
	return 0;
}
