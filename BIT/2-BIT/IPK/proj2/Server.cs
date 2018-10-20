// File:        Server.cs
// Author:      Daniel Klimaj; xklima22@stud.fit.vutbr.cz
// Description: IpkServer, HTTP server for 2nd IPK project

using System;
using System.Text;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace IPK
{
    /// <summary>
    /// Log Type.
    /// </summary>
    enum LogType
    {
        LogIn,
        LogOut,
    };

    /// <summary>
    /// Ipk server.
    /// </summary>
    public class IpkServer
    {
        private int port;
        private int chunkSize;
        private int chunkMinTime;

        private TcpListener listener;
        private TcpClient   client;
        
        private string execPath;
        private string preservedLog;

        private static bool active = true;

        /// <summary>
        /// Initializes a new instance of the <see cref="IPK.IpkServer"/> class.
        /// </summary>
        /// <param name="port">Port number on which server listens</param>
        /// <param name="chunkSize">Size of 1 chunk of data</param>
        /// <param name="chunkMinTime">Number of miliseconds between sending
        /// 2 chunks of data</param>
        public IpkServer(int port, int chunkSize = -1, int chunkMinTime = -1)
        {
            this.port = port;
            this.chunkSize = chunkSize;
            this.chunkMinTime = chunkMinTime;
            this.execPath = Path.GetDirectoryName(System.Reflection.Assembly.
                GetExecutingAssembly().GetName().CodeBase);
            this.execPath = this.execPath.Replace("file:", "");
            this.preservedLog = null;
        }
        
        /// <summary>
        /// Accessor for private static field active.
        /// </summary>
        /// <value><c>true</c> if active; otherwise, <c>false</c>.</value>
        public static bool Active
        {
            get
            {
                return active;
            }
            
            set
            {
                active = value;
            }
        }
        
        /// <summary>
        /// Run this instance.
        /// </summary>
        public void Run()
        {
            IPAddress ipAddress = Dns.GetHostEntry("localhost").AddressList[0];

            try
            {
                listener = new TcpListener(ipAddress, port);
            }
            catch(Exception)
            {
                Console.Error.WriteLine("SERVER: Failed to initialize TCP listener");
                Environment.Exit(1);
            }

            listener.Start();
            while(IpkServer.Active)
            {
                if(listener.Pending())
                {
                    Console.WriteLine("SERVER: Accepting pending connection...");
                    client = listener.AcceptTcpClient();
                    ParseRequest();
                    client.Close();
                    Console.WriteLine("SERVER: Closing connection...\n");
                }
            }
            listener.Stop();
        }

        /// <summary>
        /// Parses the request.
        /// </summary>
        public void ParseRequest()
        {
            NetworkStream inStream = client.GetStream();
            if(inStream.CanRead)
            {
                // Check if request is not just string of nulls
                int b = inStream.ReadByte();
                if(b > 0)
                {
                    byte[] bytes = new byte[2048];
                    inStream.Read(bytes, 0, 2048);
                    string inData = Convert.ToChar(b) +
                        Encoding.UTF8.GetString(bytes);
                    string request = GetRequestLine(inData);
                    string[] reqParts = request.Split(' ');
                    string httpMethod = reqParts[0].Trim();
                    string reqPath = reqParts[1].Trim();

                    Log(LogType.LogIn, inData);
                    
                    if(httpMethod == "GET")
                    {
                        HttpGet(reqPath);
                    }
                    else
                    {
                        Console.Error.WriteLine("SERVER: Can not handle method {0}",
                            httpMethod);
                    }
                    inData = "";
                    bytes = null;
                    inStream.Close();
                }
                else
                {
                    Console.Error.WriteLine("SERVER: Invalid request, skipping...");
                }
            }
        }

        /// <summary>
        /// Http GET handler.
        /// </summary>
        /// <param name="reqPath">Local path sent by request</param>
        public void HttpGet(string reqPath)
        {

            string path = execPath + "/www" + reqPath;
            
            if(!File.Exists(path))
            {
                ResponseError();
            }
            else
            {
                ResponseSuccess(path);
            }
        }

        /// <summary>
        /// Gets the first line of request.
        /// </summary>
        /// <returns>The first line of request.</returns>
        /// <param name="str">String.</param>
        private string GetRequestLine(string str)
        {
            string reqLine = "";
            if(str.Length > 0)
            {
                foreach(char c in str)
                {
                    if(c == '\r')
                        continue;
                    if(c == '\n')
                        break;
                    reqLine += c; 
                }
            }

            return reqLine;
        }

        /// <summary>
        /// Responses error.
        /// </summary>
        private void ResponseError()
        {
            NetworkStream response = client.GetStream();
            if(response.CanWrite)
            {
                string outData = "HTTP/1.1 404 File not found\r\n" +
                    "Content-Type: text/html\r\n" +
                    "Connection: close\r\n\r\n" +
                    "<html><body><h1>404 File Not Found</h1></body></html>\r\n";

                byte[] data = Encoding.UTF8.GetBytes(outData);
                try
                {
                    response.Write(data, 0, outData.Length);
                }
                catch
                {
                    Console.Error.WriteLine("SERVER: Error while writing " +
                        "to client socket");
                    return;
                }
                Log(LogType.LogOut, outData);
            }
            else
            {
                Console.Error.WriteLine("SERVER: Unable to write to client stream");
            }
            response.Close();
        }

        /// <summary>
        /// Responses the success.
        /// </summary>
        /// <param name="filePath">File path.</param>
        private void ResponseSuccess(string filePath)
        {
            string fileContents = "";
            try
            {
                fileContents = File.ReadAllText(filePath);
            }
            catch
            {
                fileContents = "";
            }

            if(chunkMinTime <= -1)
            {
                if((chunkSize > 0) && (fileContents.Length > chunkSize))
                {
                    fileContents = fileContents.Substring(0, chunkSize);
                }

                NetworkStream response = client.GetStream();
                if(response.CanWrite)
                {
                    string outData = "HTTP/1.1 200 OK\r\n" +
                        "Content-Type: text/plain\r\n" +
                        "Content-Length: " + fileContents.Length + "\r\n" +
                        "Connection: close\r\n\r\n" +
                        fileContents + "\r\n";
    
                    byte[] data = Encoding.UTF8.GetBytes(outData);
                    try
                    {
                        response.Write(data, 0, outData.Length);
                    }
                    catch
                    {
                        Console.Error.WriteLine("SERVER: Error while writing " +
                            "to client socket");
                        return;
                    }
                    Log(LogType.LogOut, outData);
                }
                else
                {
                    Console.Error.WriteLine("SERVER: Unable to write to client stream");
                }
                response.Close();
            }
            else
            {
                NetworkStream response = client.GetStream();
                if(response.CanWrite)
                {
                    string[] chunks = CreateChunks(fileContents);
                    for(int i=-1; i<=chunks.Length; i++)
                    {
                        string outData = "";
                        if(i==-1)
                        {
                            outData = "HTTP/1.1 200 OK\r\n" +
                                "Content-Type: text/plain\r\n" +
                                "Transfer-Encoding: chunked\r\n\r\n";
                        }
                        else if(i < chunks.Length)
                        {
                            outData = chunks[i].Length.ToString("X") + "\r\n" +
                                chunks[i] + "\r\n";
                        }
                        else
                        {
                            outData = "0\r\n\r\n";
                        }
        
                        byte[] data = Encoding.UTF8.GetBytes(outData);
                        try
                        {
                            response.Write(data, 0, data.Length);
                        }
                        catch
                        {
                            Console.Error.WriteLine("SERVER: Error while writing " +
                                "to client socket");
                            return;
                        }
                        Log(LogType.LogOut, outData, true);
                        data = null;
                        Thread.Sleep(chunkMinTime);
                    }
                }
                else
                {
                    Console.Error.WriteLine("SERVER: Unable to write to client stream");
                }
                preservedLog = null;
                response.Close();
            }
        }

        /// <summary>
        /// Creates the chunks.
        /// </summary>
        /// <returns>The chunks.</returns>
        /// <param name="str">Text to create chunks from.</param>
        private string[] CreateChunks(string str)
        {
            int size = str.Length;
            List<string> strings = new List<string>();
            string temp;
            
            if(size == 0)
            {
                strings.Add("");
            }
            else
            {
                int chunkCnt = size % chunkSize == 0 ? size / chunkSize :
                    size / chunkSize + 1;

                for(int i=0; i<chunkCnt; i++)
                {
                    temp = "";
                    if(i < chunkCnt - 1)
                    {
                        temp = str.Substring(i * chunkSize, chunkSize);
                    }
                    else
                    {
                        temp = str.Substring(i * chunkSize);
                    }
                    strings.Add(temp);
                }
            }

            return strings.ToArray();
        }

        /// <summary>
        /// Logger.
        /// </summary>
        /// <param name="logType">Log type.</param>
        /// <param name="data">Data.</param>
        /// <param name="preserve">If set to <c>true</c> append data.</param>
        private void Log(LogType logType, string data, bool preserve = false)
        {
            string fileName;
            FileStream fs;

            if(preserve)
            {
                if(preservedLog == null)
                {
                    fileName = CreateFile(logType);
                
                    if(File.Exists(fileName))
                    {
                        File.Delete(fileName);
                    }
                    
                    preservedLog = string.Copy(fileName);
                    fs = new FileStream(fileName, FileMode.CreateNew);
                }
                else
                {
                    fileName = string.Copy(preservedLog);
                    fs = new FileStream(fileName, FileMode.Append);
                }
            }
            else
            {
                fileName = CreateFile(logType);

                if(File.Exists(fileName))
                {
                    File.Delete(fileName);
                }

                fs = new FileStream(fileName, FileMode.CreateNew);
            }

            byte[] byteData = Encoding.UTF8.GetBytes(data);
            int i = 0;
            while(i < byteData.Length)
            {
                if(byteData[i] == 0)
                {
                    break;
                }
                else
                {
                    fs.WriteByte(byteData[i]);
                }
                i++;
            }
            fs.Close();
        }

        /// <summary>
        /// Creates the file for Log Type.
        /// </summary>
        /// <returns>File name.</returns>
        /// <param name="logType">Log type.</param>
        private string CreateFile(LogType logType)
        {
            string timestamp = DateTime.Now.ToString("yyyy-MM-dd:HH:mm:ss");
            string suffix;
            if(logType == LogType.LogIn)
            {
                suffix = "in";
            }
            else
            {
                suffix = "out";
            }

            string fn = "ipkHttpServer-" + timestamp + "." + suffix + ".log";
            fn = execPath + "/" + fn;
            Console.WriteLine("SERVER: Creating Log file: {0}", fn);
            
            return fn;
        }
    }

    /// <summary>
    /// Server main.
    /// </summary>
    public class ServerMain
    {
        private static bool bHelp      = false;
        private static bool bPort      = false;
        private static int  portNo     = -1;
        private static bool bChunkSize = false;
        private static int  chunkSize  = -1;
        private static bool bChunkTime = false;
        private static int  chunkTime  = -1;

        /// <summary>
        /// The entry point of the program, where the program control starts and ends.
        /// </summary>
        /// <param name="args">The command-line arguments.</param>
        public static void Main (string[] args)
        {
            string[] sopts =
                {"h", "c:", "p:", "t:"};
            string[] lopts =
                {"help", "chunk-max-size:", "port:", "min-chunk-time:"};

            Hashtable opts = new Hashtable();

            try
            {
                opts = ParseOpts(args, sopts, lopts);
            }
            catch(Exception ex)
            {
                Console.Error.WriteLine("SERVER: {0}", ex.Message);
                Environment.Exit(1);
            }

            bool ok = ProcessParams(opts);
            if(!ok)
            {
                Environment.Exit(1);
            }

            if(bHelp)
            {
                PrintHelp();
                Environment.Exit(0);
            }

            if(!bPort)
            {
                Console.Error.WriteLine("SERVER: Unspecified port number");
                Environment.Exit(1);
            }
            
            if(portNo < 1024)
            {
                Console.Error.WriteLine("SERVER: Invalid port number {0}", portNo);
                Environment.Exit(1);
            }
            
            IpkServer srv = new IpkServer(portNo, chunkSize, chunkTime);

            Console.CancelKeyPress += new ConsoleCancelEventHandler(OnSigInt);
            srv.Run();
            
            Environment.Exit(0);
        }

        /// <summary>
        /// Handles the SIGINT event.
        /// </summary>
        /// <param name="sender">Sender.</param>
        /// <param name="e">Event.</param>
        private static void OnSigInt(object sender, ConsoleCancelEventArgs e)
        {
            e.Cancel = true;
            IpkServer.Active = false;
        }

        /// <summary>
        /// Parses the command line arguments.
        /// </summary>
        /// <returns>Parsed opts in Hashtable</returns>
        /// <param name="opts">Command line arguments</param>
        /// <param name="shortOpts">Short opts.</param>
        /// <param name="longOpts">Long opts.</param>
        private static Hashtable ParseOpts(string[] opts, string[] shortOpts,
                                   string[] longOpts)
        {
            Hashtable ht_opts = new Hashtable();
            int optind = 0;

            while(optind < opts.Length)
            {
                if(opts[optind].StartsWith("--"))
                {
                    for(int i=0; i<longOpts.Length; i++)
                    {
                        if(longOpts[i][longOpts[i].Length-1] == ':')
                        {
                            string arg = longOpts[i].Remove(longOpts[i].Length-1);

                            if((opts[optind].Length > 3) &&
                            (opts[optind].Substring(2) == arg))
                            {
                                optind++;
                                if((optind < opts.Length) &&
                                (!opts[optind].StartsWith("-")))
                                {
                                    try
                                    {
                                        ht_opts.Add(arg, opts[optind]);
                                    }
                                    catch(ArgumentException)
                                    {
                                    }
                                    break;
                                }
                                else
                                {
                                    throw new Exception("Option \'" + arg +
                                        "\' requires an argument");
                                }
                            }
                        }
                        else
                        {
                            if((opts[optind].Length > 3) &&
                            (opts[optind].Substring(2) == longOpts[i]))
                            {
                                try
                                {
                                    ht_opts.Add(longOpts[i], null);
                                }
                                catch(ArgumentException)
                                {
                                }
                                break;
                            }
                        }
                    }
                }
                else if(opts[optind].StartsWith("-") && opts[optind].Length == 2)
                {
                    for(int i=0; i<shortOpts.Length; i++)
                    {
                        if(shortOpts[i].Length == 2)
                        {
                            if(shortOpts[i][1] == ':')
                            {
                                string arg = shortOpts[i][0].ToString();

                                if(opts[optind][1].ToString() == arg)
                                {
                                    optind++;
                                    if((optind < opts.Length) &&
                                    (!opts[optind].StartsWith("-")))
                                    {
                                        try
                                        {
                                            ht_opts.Add(arg, opts[optind]);
                                        }
                                        catch(ArgumentException)
                                        {
                                        }
                                        break;
                                    }
                                    else
                                    {
                                        throw new Exception("Option \'" + arg +
                                            "\' requires an argument");
                                    }
                                }
                            }
                            else
                            {
                                throw new Exception("Pattern \'" + shortOpts[i] +
                                    "\' contains invalid character(s)");
                            }
                        }
                        else if(shortOpts[i].Length == 1)
                        {
                            if(opts[optind][1].ToString() == shortOpts[i])
                            {
                                try
                                {
                                    ht_opts.Add(shortOpts[i], null);
                                }
                                catch(ArgumentException)
                                {
                                }
                                break;
                            }
                        }
                        else
                        {
                            throw new Exception("Pattern \'" + shortOpts[i] +
                                "\' is too long");
                        }
                    }
                }
                else
                {
                    throw new Exception("Unrecognized option \'" +
                        opts[optind] + "\'");
                }
                optind++;
            }

            return ht_opts;
        }

        /// <summary>
        /// Processes the command line arguments and sets fields accordingly.
        /// </summary>
        /// <returns><c>true</c>, if parameters was processed, <c>false</c> otherwise.</returns>
        /// <param name="opts">Command line arguments</param>
        private static bool ProcessParams(Hashtable opts)
        {
            foreach(string key in opts.Keys)
            {
                try
                {
                    if(((key == "h") || (key == "help")) && !bHelp)
                    {
                        bHelp = true;
                    }
                    else if(((key == "p") || (key == "port")) && !bPort)
                    {
                        bPort = true;
                        portNo = Convert.ToInt32(opts[key]);
                    }
                    else if(((key == "c") || (key == "chunk-max-size")) &&
                    !bChunkSize)
                    {
                        bChunkSize = true;
                        chunkSize = Convert.ToInt32(opts[key]);
                    }
                    else if(((key == "t") || (key == "min-chunk-time")) &&
                    !bChunkTime)
                    {
                        bChunkTime = true;
                        chunkTime = Convert.ToInt32(opts[key]);
                    }
                }
                catch(FormatException)
                {
                    Console.Error.WriteLine("SERVER: Invalid optarg format " + 
                        opts[key] + " for " + key);
                    return false;
                }
                catch(OverflowException)
                {
                    Console.Error.WriteLine("SERVER: Value " + opts[key] +
                        " does not fit in Int32");
                    return false;
                }
            }

            return true;
        }

        /// <summary>
        /// Prints the help.
        /// </summary>
        private static void PrintHelp()
        {
            string helpMsg =
                "Program: IPK #2 - Simple HTTP Server\n" +
                "Author:  Daniel Klimaj; xklima22@stud.fit.vutbr.cz\n" +
                "Usage:   ./ipkhttpserver [-h] [-c chunk-max-size] [-p port] " +
                "[-t min-chunk-time]\n\n" +
                "Parameters:\n" +
                "\t-h | --help \t\tShows this message\n" +
                "\t-p | --port \t\tPort number (> 1023)\n" +
                "\t-c | --chunk-max-size\tMaximum size for 1 chunk\n" +
                "\t-t | --min-chunk-time\tMinimal interval between sending " +
                "2 chunks of data";

            Console.WriteLine(helpMsg);
        }
    }
}

// Server.cs
