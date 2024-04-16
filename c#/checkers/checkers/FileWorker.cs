using System.IO;

namespace checkers;

public static class FileWorker
{
    private const string DirName = "history"; 
    public static bool LoadData(string? fileName, ref string loadedData)
    {
        try
        {
            var currentDir = Directory.GetCurrentDirectory();
            var platform = Environment.OSVersion.Platform;
            string separator;

            switch (platform)
            {
                case PlatformID.Unix:
                    separator = "/";
                    break;
                default:
                    separator = "\\";
                    break;
            }
            var filePath = currentDir + separator + DirName + separator + fileName;
            
            using var reader = new StreamReader(filePath);
            loadedData = reader.ReadToEnd();
            return true;
        }
        catch (Exception e)
        {
            Console.WriteLine("The file could not be read:");
            Console.WriteLine(e.Message);
            return false;
        }
    }

    public static void SaveData(string fileName, string data)
    {
        var currentDir = Directory.GetCurrentDirectory();
        var platform = Environment.OSVersion.Platform;
        string separator;

        switch (platform)
        {
            case PlatformID.Unix:
                separator = "/";
                break;
            default:
                separator = "\\";
                break;
        }

        var dirPath = currentDir + separator + DirName;
        if (!Directory.Exists(dirPath))
            Directory.CreateDirectory(dirPath);

        var filePath = dirPath + separator + fileName + ".txt";

        using StreamWriter writer = new StreamWriter(filePath, false);
        writer.Write(data);
    }
}