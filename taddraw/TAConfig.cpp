//config class
#include "oddraw.h"
#include <vector>
using namespace std;

#include "TAConfig.h"
#include "tamem.h"
#include "tafunctions.h"
#include <tchar.h>
#include <atlchecked.h>
#include "hook\etc.h"

TADRConfig::TADRConfig ()
{
	char TAexePath[MAX_PATH];
	char TAexeName[MAX_PATH];
	GetModuleFileNameA ( NULL, TAexePath, MAX_PATH);

	_splitpath_s ( TAexePath, IniFilePath_cstr, 3, &IniFilePath_cstr[2], MAX_PATH, TAexeName, MAX_PATH, NULL, 0);
	strcat_s ( IniFilePath_cstr, 0x100, TAIniStr);

	BYTE * IniFileData;
	DWORD IniFileLen;

	IniFileData= NULL;
	IniFileLen= 0;

	HANDLE File;
	File= CreateFileA ( IniFilePath_cstr, FILE_READ_ACCESS, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE==File)
	{
		return ;
	}

	IniFileLen= GetFileSize ( File, NULL);

	if (0<IniFileLen)
	{
		IniFileData= new BYTE[IniFileLen];

		ReadFile ( File, IniFileData, IniFileLen, &IniFileLen, NULL);

		LoadIniRegSetting ( IniFileData);
	}
	if (NULL!=IniFileData)
	{
		delete IniFileData;
	}
	CloseHandle ( File);
}

TADRConfig::~TADRConfig ()
{
	while (! RegStrings_vec.empty ())
	{
		delete RegStrings_vec.back ( );
		RegStrings_vec.pop_back ( );
	}

	while (! RegDwords_vec.empty ())
	{
		delete RegDwords_vec.back ( );
		RegDwords_vec.pop_back ( );
	}
}

HKEY TADRConfig::TARegPath_HKEY (void)
{
	DWORD dwDisposition;
	HKEY Rtn;
	RegCreateKeyEx ( HKEY_CURRENT_USER, "Software\\Cavedog Entertainment", NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &Rtn, &dwDisposition);
	RegCreateKeyEx ( Rtn, "Total Annihilation\\", NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &Rtn, &dwDisposition);
	return Rtn;
}

LONG TADRConfig::WriteTAReg_Dword (LPTSTR lpValueName, DWORD Value)
{
	HKEY TAhkey= TARegPath_HKEY ( );
	LONG rtn= RegWriteDword ( TAhkey, NULL, lpValueName, Value);
	RegCloseKey (TAhkey) ;
	return rtn;
}

LONG TADRConfig::WriteTAReg_Str (LPTSTR lpValueName, LPCSTR Data, DWORD Strlen)
{
	HKEY TAhkey= TARegPath_HKEY ( );
	LONG rtn=RegWriteStr ( TAhkey, NULL, lpValueName, Data, Strlen);
	RegCloseKey (TAhkey) ;
	return rtn;
}

void TADRConfig::LoadIniRegSetting (/*__in __out */LPBYTE IniFileData)
{
	//load the [reg] part 
	//RegStrings_vec
	//RegDwords_vec
	try
	{
		char LineBuf[0x200];
		char RegName[0x100];
		char RegType[0x20];
		char RegData[0x200];
		char * TagSign_cstrp;
		int LineLen;
		DWORD Value;
		RegDword * RegDword_tmp;
		RegString * RegString_tmp;

		char* tmp_cstrp= strchr ( reinterpret_cast<char *>(IniFileData), '[');
		char * tm1_cstrp= strchr ( reinterpret_cast<char *>(IniFileData), ']');

		while (true)
		{
			if ((NULL==tmp_cstrp)||
				(NULL==tm1_cstrp))
			{//no [REG] in 
				goto end;
			}
			memcpy ( LineBuf, tmp_cstrp+ 1, tm1_cstrp- tmp_cstrp- 1);
			//*tm1_cstrp= 0;
			ATL::Checked::strupr_s ( LineBuf, 0x200);
			if (0==strncmp ( LineBuf, "REG", 3))
			{
				break;
			}
			tmp_cstrp= strchr ( tm1_cstrp+ 1, '[');
			if (NULL==tmp_cstrp)
			{
				goto end;
			}
			tm1_cstrp= strchr ( tmp_cstrp, ']');
		}
		
		//try to found next "[]" after "REG"
		tmp_cstrp= strchr ( tm1_cstrp, '[');
		if (NULL!=tmp_cstrp)
		{// then make it be file end
			tmp_cstrp[0]= 0;
		}

		
		//load reg
		// "\n"
		do 
		{
			TagSign_cstrp= tm1_cstrp;
			if (NULL==TagSign_cstrp)
			{
				goto end;
			}

			tmp_cstrp= strchr ( TagSign_cstrp, '\n');   // next
			tm1_cstrp= NULL;
			if (NULL!=tmp_cstrp)
			{
				tm1_cstrp= strchr ( tmp_cstrp+ 1, '\n');// next next line
			}
			else
			{
				tmp_cstrp= TagSign_cstrp;// last line
			}

			if (NULL==tm1_cstrp)
			{// end of file
				LineLen= strlen ( tmp_cstrp);
			}
			else
			{
				LineLen= tm1_cstrp- tmp_cstrp;
			}

			if (LineLen>0x200)
			{// error!too long line, this is bad end
				//LineLen= MAX_PATH;
				goto end;
			}
			
			memcpy ( LineBuf, tmp_cstrp+ 1, LineLen);//tmp_cstrp==\n
			LineBuf [LineLen]= 0;
			tmp_cstrp= trim_crlf_ ( LineBuf);


			tmp_cstrp= strchr ( tmp_cstrp, '\"');
			if (NULL==tmp_cstrp)
			{
				//no quota in this line, goto next line
				continue;
			}
			TagSign_cstrp= strchr ( tmp_cstrp+ 1, '\"');
			if (NULL==TagSign_cstrp)
			{
				//no 2rd quota in this line
				continue;
			}
			TagSign_cstrp= strchr ( TagSign_cstrp+ 1, '=');
			if (NULL==TagSign_cstrp)
			{
				//no equal sign in this line
				continue;
			}
			*TagSign_cstrp= 0;
			strcpy_s ( RegName, 0x100, tmp_cstrp+ 1);
			memcpy ( RegData, TagSign_cstrp+ 1, LineBuf+ LineLen- TagSign_cstrp- 1- 1);

			//now check RegType

			memcpy ( RegType, RegData, 0x20);
			RegType[0x1f]= 0;// truncate the end of RegType str;

			ATL::Checked::strlwr_s ( RegType, 0x20); 

			tmp_cstrp= strstr ( RegType, "dword:");
			if (NULL!=tmp_cstrp)
			{//REG_DWORD
				RegData [tmp_cstrp- RegType+ 4]= '0';
				RegData [tmp_cstrp- RegType+ 5]= 'x';

				Value= _strtol_l ( &RegData[tmp_cstrp- RegType+ 4], NULL, 0x10, NULL);
				* strchr ( RegName , '\"')= 0;
				RegDword_tmp= new RegDword ( RegName , strlen ( RegName ), Value);
				RegDwords_vec.push_back ( RegDword_tmp);
			}
			else if (NULL!=strstr ( RegType, "\""))
			{//REG_SZ
				TagSign_cstrp= strstr ( RegData, "\"");
				TagSign_cstrp+= 1;
				tmp_cstrp= strstr ( TagSign_cstrp, "\"");
				if (NULL!=tmp_cstrp)
				{
					*tmp_cstrp= 0;
				}
				* strchr ( RegName, '\"')= 0;
				RegString_tmp= new RegString (  RegName , strlen ( RegName ), TagSign_cstrp, strlen ( TagSign_cstrp));
				RegStrings_vec.push_back ( RegString_tmp);
			}
		} while ( NULL!=tm1_cstrp);
	}
	catch (...)
	{
		;
	}
end:
	;
}

BOOL TADRConfig::GetIniBool (LPCSTR ConfigName, BOOL Default)
{
	//
	char StrBuf[0x100];
	char * DefaultStr= "false";
	if (Default)
	{
		DefaultStr= "true";
	}
	else
	{
		DefaultStr= "false";
	}

	GetPrivateProfileStringA ( "Preferences", ConfigName, DefaultStr, StrBuf, 0x100, IniFilePath_cstr);
	 
	_strlwr_s ( StrBuf, 0x100);
	if (NULL!=strstr ( StrBuf, "true"))
	{
		return TRUE;
	}
	return FALSE;
}

int TADRConfig::GetIniInt (LPCSTR ConfigName, int DefaultValue)
{
	return GetIniFileInt ( ConfigName, DefaultValue);
}

int TADRConfig::EnumIniRegInfo_Begin (PEnumRegInfo * iterator_arg, LPCSTR * Name_pp, LPCVOID * Data_p)
{//return all count
	PEnumRegInfo iterator_pvar;

	int Rtn= 0;

	iterator_pvar= new EnumRegInfo;
	iterator_pvar->Dword_iter= RegDwords_vec.begin ( );
	iterator_pvar->String_iter= RegStrings_vec.begin();
	iterator_pvar->Count= 0;

	*Name_pp= NULL;
	*Data_p=  NULL;
/*
	if (RegStrings_vec.empty ( ))
	{
		if (!RegStrings_vec.empty ( ))
		{
			*Name_pp= (iterator_pvar->String_iter)->Name();
			*Data_p=  reinterpret_cast<LPCVOID>((iterator_pvar->String_iter)->Str());	
			(iterator_pvar)->Count+= 1;
			Rtn= 1;
			++((iterator_pvar->String_iter));
		}

	}
	else
	{
		*Name_pp= (iterator_pvar->Dword_iter)->Name();
		*Data_p=  reinterpret_cast<LPVOID>((iterator_pvar->Dword_iter)->Value());
		iterator_pvar->Count+= 1;
		
		Rtn= 0;
		++((*iterator_pvar->Dword_iter));
	}*/

	 *(iterator_arg)= iterator_pvar;
	return EnumIniRegInfo_Next (  iterator_arg,  Name_pp, Data_p);
}

int TADRConfig::EnumIniRegInfo_Next (PEnumRegInfo * iterator_arg, LPCSTR * Name_pp, LPCVOID *  Data_p)
{//return 0 for dword, return 1 for str 
	
	PEnumRegInfo iterator_var;
	iterator_var= *iterator_arg;
	int Rtn= 0;

	*Name_pp= NULL;
	*Data_p=  NULL;

	if (iterator_var->Dword_iter==RegDwords_vec.end ( ))
	{
		if ((iterator_var->String_iter)!=RegStrings_vec.end ( ))
		{

			*Name_pp= (*iterator_var->String_iter)->Name();
			*Data_p=  reinterpret_cast<LPCVOID>((*iterator_var->String_iter)->Str());	
			iterator_var->Count+= 1;
			Rtn= 1;
			++((iterator_var->String_iter));
		}
	}
	else
	{
		
		*Name_pp= (*iterator_var->Dword_iter)->Name();
		*Data_p=  reinterpret_cast<LPVOID>((*iterator_var->Dword_iter)->Value());
		iterator_var->Count+= 1;
		Rtn= 0;
		++(iterator_var->Dword_iter);
	}

	return Rtn;//iterator_var->Count;;
}
int TADRConfig::EnumIniRegInfo_End (PEnumRegInfo * iterator_arg)
{//return un-enumed counter
	PEnumRegInfo iterator_var;
	iterator_var= *iterator_arg;

	int Rtn= RegStrings_vec.size ( )+ RegDwords_vec.size ( )- iterator_var->Count;
	delete iterator_arg;
	return Rtn;
}


RegDword::RegDword (LPSTR Name, int NameLen, DWORD Value)
{
	myName= NULL;
	myValue= 0;

	if ((NULL==Name)
		||(0>=NameLen))
	{
		return;
	}

	myValue= Value;
	myName= new char [NameLen+ 1];
	memcpy ( myName, Name, NameLen+ 1);
}
RegDword::~RegDword ()
{
	if (NULL!=myName)
	{
		delete myName;
	}

}
LPCSTR RegDword::Name(void)
{
	return myName;
}

DWORD RegDword::Value(void)
{
	return myValue;
}

RegString::RegString (LPSTR Name, int NameLen, LPSTR Str, int mystrLen)
{
	myName= NULL;
	myStr= NULL;

	if ((NULL==Name)
		||(0>=NameLen))
	{
		return;
	}

	myStr= new char [mystrLen+ 1];
	memcpy ( myStr, Str, mystrLen+ 1);

	myName= new char [NameLen+ 1];
	memcpy ( myName, Name, NameLen+ 1);
}

RegString::~RegString ()
{
	if (NULL!=myName)
	{
		delete myName;
	}
	if (NULL!=myStr)
	{
		delete myStr;
	}
}

LPCSTR RegString::Name (void)
{
	return myName;
}

LPCSTR RegString::Str (void)
{
	return myStr;
}
