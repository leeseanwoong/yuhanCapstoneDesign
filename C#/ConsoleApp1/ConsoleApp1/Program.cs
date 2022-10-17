using System;
using TestWeb;

namespace ConsoleApp1
{
    class Program
    {
        static void Main(string[] args)
        {
            //압축
            ZipManager.ZipFiles(@"D:\ZipTest\unzipFolder\", @"C:\Users\YUHAN\Desktop\ZipFile\Video.zip", "1234", false);
            //압축해제
            ZipManager.UnZipFiles(@"C:\Users\YUHAN\Desktop\ZipFile\Video.zip", @"D:\ZipTest\unzipFolder\", "1234", false);
        }
    }
}
