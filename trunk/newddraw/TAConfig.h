#pragma once

extern LPCSTR TAIniStr;

class RegDword
{
	LPSTR myName;
	DWORD myValue;
public:
	RegDword (LPSTR Name, int NameLen, DWORD Value);
	~RegDword ();

	LPCSTR Name (void);
	DWORD Value (void);
};

class RegString
{
	LPSTR myName;
	LPSTR myStr;
public:
	RegString (LPSTR Name, int NameLen, LPSTR Str, int mystrLen);
	~RegString ();

	LPCSTR Name (void);
	LPCSTR Str (void);
};

typedef struct _EnumRegInfo
{
	vector<RegDword *>::iterator Dword_iter;
	vector<RegString *>::iterator String_iter;
	int Count;
}EnumRegInfo, * PEnumRegInfo;

class TADRConfig 
{
private:
	char IniFilePath_cstr[MAX_PATH];

	vector<RegString *> RegStrings_vec;
	vector<RegDword *> RegDwords_vec;

public:
	TADRConfig ();
	~TADRConfig ();

	BOOL GetIniBool (LPCSTR ConfigName, BOOL Default);
	int GetIniInt (LPCSTR ConfigName, int DefaultValue);

	int EnumIniRegInfo_End (PEnumRegInfo * iterator_arg);
	int EnumIniRegInfo_Next (PEnumRegInfo * iterator_arg, LPCSTR * Name_pp, LPCVOID *  Data_p);
	int EnumIniRegInfo_Begin (PEnumRegInfo * iterator_arg, LPCSTR * Name_pp, LPCVOID *  Data_p);

	LONG WriteTAReg_Str (LPTSTR lpValueName, LPCSTR Data, DWORD Strlen);
	LONG WriteTAReg_Dword (LPTSTR lpValueName, DWORD Value);
	

private:
	void LoadIniRegSetting ( LPBYTE IniFileData);
	HKEY TARegPath_HKEY (void);
};

extern TADRConfig * MyConfig;;