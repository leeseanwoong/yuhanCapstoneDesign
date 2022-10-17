// ZipUnzip.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// SimpleZip.cpp : Defines the entry point for the console application.
// Usage: SimpleZip [flag] [source file]
//        flag: c - compress
//               d - decompress

#include <stdio.h>
#include <windows.h>
#include "zlib/zipper.h"
#include "zlib/unzipper.h"


int main(int argc, char* argv[])
{
	char *srcFileName;
	bool bSrcIsDirectory, br;
	DWORD fileAttr;

	CZipper zip;

	if (zip.OpenZip("D:\\123.zip",  false))
	{
		zip.AddFolderToZip("D:\\123", false);
		zip.CloseZip();
	}
		// �Ķ���� Ȯ��
	if(argc != 5) {
		printf("��");
		
	}

	srcFileName = argv[2];

	// �ҽ� ���� Ȯ��
	bSrcIsDirectory = ((fileAttr = GetFileAttributes(srcFileName)) & FILE_ATTRIBUTE_DIRECTORY) > 0;
	if(fileAttr == 0xFFFFFFFF) {
		printf("file not exist or has unknown problems");
		exit(0);
	}

	switch(argv[1][0]) {
			 case 'c':
			 case 'C':
				 // ���丮�� ���
				 if(bSrcIsDirectory) {
					 br = CZipper::ZipFolder(srcFileName, FALSE);
				 }
				 // ������ ���
				 else {
					 br = CZipper::ZipFile(srcFileName);
				 }

				 printf(br ? "compression completed." : "error occured.");
				 break;

			 case 'd':
			 case 'D':
				 // ���丮�� ���
				 if(bSrcIsDirectory) {
					 printf("%s isn't a file", srcFileName);
					 
				 }
				 // ������ ���
				 else {
					 CUnzipper uz;
					 br = uz.OpenZip(srcFileName);
					 if(br) br = uz.UnzipTo(".");
					 printf(br ? "decompression completed." : "error occured.");
				 }
				 break;

			 default:
				 printf("wrong flag.");
				 break;
	}
	return 0;
}
