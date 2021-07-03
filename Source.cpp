/*███████████  ██████████    █████   █████  ███
░░███░░░░░███░░███░░░░░█   ░░███   ░░███  ░░░
░███    ░███ ░███  █ ░     ░███    ░███  ████   ██████  █████ ███ █████  ██████  ████████
░██████████  ░██████       ░███    ░███ ░░███  ███░░███░░███ ░███░░███  ███░░███░░███░░███
░███░░░░░░   ░███░░█       ░░███   ███   ░███ ░███████  ░███ ░███ ░███ ░███████  ░███ ░░░
░███         ░███ ░   █     ░░░█████░    ░███ ░███░░░   ░░███████████  ░███░░░   ░███
█████        ██████████       ░░███      █████░░██████   ░░████░████   ░░██████  █████
░░░░░        ░░░░░░░░░░         ░░░      ░░░░░  ░░░░░░     ░░░░ ░░░░     ░░░░░░  ░░░░░
*/
#include<iostream>
#include<windows.h>
#include<winnt.h>
#include<string>

DWORD getoffset(DWORD rva, PIMAGE_SECTION_HEADER sh, PIMAGE_NT_HEADERS nth);
char HEX[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
typedef struct offset {
	unsigned char ofs[100];
}*POffset;
int main()

{
	LPCWSTR file_address = L"{ADD YOUR FILE LINK HERE }";
	HANDLE filehndl = CreateFile(file_address, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD bytes_read,file_size = GetFileSize(filehndl, NULL);
	PVOID fileptr = VirtualAlloc(NULL,file_size,MEM_COMMIT,PAGE_READWRITE) ;
    ReadFile(filehndl, fileptr, file_size,&bytes_read, NULL);

	PIMAGE_DOS_HEADER(fileptr)->e_magic;
	PIMAGE_NT_HEADERS nt_headers=(PIMAGE_NT_HEADERS)((PCHAR)fileptr+PIMAGE_DOS_HEADER(fileptr)->e_lfanew);
	

	PIMAGE_FILE_HEADER file_header = PIMAGE_FILE_HEADER (&(nt_headers)->FileHeader);

	if (file_header->Machine == IMAGE_FILE_MACHINE_I386)
		std::cout << "32 Bit" << std::endl;
	else if (file_header->Machine == IMAGE_FILE_MACHINE_IA64)
		std::cout << "64 Bit" << std::endl;
	else if (file_header->Machine == IMAGE_FILE_MACHINE_AMD64)
		std::cout << "AMD 64 Bit" << std::endl;

		
	std::cout << "Sections : " << file_header->NumberOfSections << std::endl;
	std::cout << "Time & Date : " << file_header->TimeDateStamp<< std::endl;
	/*
	 HEX DUMP
	POffset hex = POffset(fileptr);
	int hex_counter = file_size;
	while (hex != NULL) {
		for (int i = 0; i < 16; i++)
		{



			int phex = (unsigned)hex->ofs[i];
			char a = HEX[phex / 16];
			char b = HEX[phex % 16];


			std::cout << a << b << " ";




		}
		hex_counter -= 16;
		if (hex_counter != 0)
		{
			hex = (POffset)(((char*)hex) + 16);
			std::cout << std::endl;
		}
		else
			break;
	}
	 End of Hex Dump 
	*/

	// Section Details

	PIMAGE_SECTION_HEADER section_header = IMAGE_FIRST_SECTION(nt_headers);
	PIMAGE_SECTION_HEADER pish = section_header;
	for (int i = 0; i < file_header->NumberOfSections; i++)
	{
		
		std::cout << "~~~~~~~~~ SECTION " << i << " ~~~~~~~~~~~~~~~~~\n\n";
		std::cout <<"Name :  "<<pish->Name << std::endl;
		std::cout << "Virtual Size :  " << pish->Misc.VirtualSize << std::endl;
		std::cout << "Virtual Address :  " << pish->VirtualAddress << std::endl;
		std::cout << "Raw Size :  " << pish->Misc.PhysicalAddress<< std::endl;
		std::cout << "Raw Address :  " << pish->SizeOfRawData << std::endl;
		std::cout << "Pointer to Raw Data :  " << pish->PointerToRawData << std::endl;
		std::cout << "Pointer to Relocations :  " << pish->PointerToRelocations << std::endl;
		std::cout << "Pointer to Line Numbers :  " << pish->PointerToLinenumbers << std::endl;
		std::cout << "Number of Line Numbers :  " << pish->NumberOfLinenumbers << std::endl;
		std::cout << "Number of Relocations :  " << pish->NumberOfRelocations << std::endl;
		pish++;
		//section_header += 28;
	}
	

	//PIMAGE_DATA_DIRECTORY data_directory = (PIMAGE_DATA_DIRECTORY)(PIMAGE_OPTIONAL_HEADER)(IMAGE_NT_HEADERS);

	
	
	PIMAGE_IMPORT_DESCRIPTOR import_descriptor = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD_PTR)fileptr + getoffset(nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress, section_header, nt_headers));
	std::cout << "~~~~~~~~~~~~~~~~~~~~~ Import Table ~~~~~~~~~~~~~~~~~~~~" << std::endl;
	while (import_descriptor->Name != NULL)
	{
		
		PCHAR dll = (PCHAR)((DWORD_PTR)fileptr + getoffset(import_descriptor->Name, section_header, nt_headers));
		std::cout <<"\t"<< dll << std::endl;
		import_descriptor++;




	}
	int b0op = 55;
}

DWORD getoffset(DWORD rva, PIMAGE_SECTION_HEADER sh, PIMAGE_NT_HEADERS nth)
{

	PIMAGE_SECTION_HEADER pish = sh;
	PIMAGE_NT_HEADERS pinh = nth;
	for (int i = 0; i < pinh->FileHeader.NumberOfSections; i++)
	{
		if (rva >= pish->VirtualAddress && rva < (pish->VirtualAddress + pish->Misc.VirtualSize))
		{
			break;
		}
	
			pish++;




	}
	return (rva - pish->VirtualAddress + pish->PointerToRawData);
}


