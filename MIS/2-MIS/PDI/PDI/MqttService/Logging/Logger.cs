using System;
using System.IO;

namespace MqttService.Logging
{
    public static class Logger
    {
        private static StreamWriter file;
        private static string path;
        private static bool initialized = false;

        public static void Init()
        {
            if (!initialized)
            {
                path        = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "log.log");
                initialized = true;
            }
        }
        
        private static void Log(LogLevel level, string message)
        {
            if (!initialized)
                Init();

            var now = DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss");
            file = File.AppendText(path);
            file.WriteLine("{0}: [{1}]: {2}", now, LogLevelToString(level), message);
            file.Close();
        }

        public static void Info(string message)
        {
            Log(LogLevel.INFO, message);
        }

        public static void Debug(string message)
        {
            Log(LogLevel.DEBUG, message);
        }

        public static void Warn(string message)
        {
            Log(LogLevel.WARN, message);
        }

        public static void Error(string message)
        {
            Log(LogLevel.ERROR, message);
        }

        public static void Info(string fmt, params object [] args)
        {
            Log(LogLevel.INFO, String.Format(fmt, args));
        }

        public static void Debug(string fmt, params object[] args)
        {
            Log(LogLevel.DEBUG, String.Format(fmt, args));
        }

        public static void Warn(string fmt, params object[] args)
        {
            Log(LogLevel.WARN, String.Format(fmt, args));
        }

        public static void Error(string fmt, params object[] args)
        {
            Log(LogLevel.ERROR, String.Format(fmt, args));
        }

        private static string LogLevelToString(LogLevel level, bool abbr = true)
        {
            string value = "";
            switch (level)
            {
                case (LogLevel.INFO):
                    value = "INFO";
                    break;
                case (LogLevel.DEBUG):
                    value = "DEBUG";
                    break;
                case (LogLevel.WARN):
                    value = "WARN";
                    break;
                case (LogLevel.ERROR):
                    value = "ERROR";
                    break;
                default:
                    return "";
            }
            return abbr ? value[0].ToString() : value;
        }
    }
}
