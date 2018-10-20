// File:        Client.cs
// Author:      Daniel Klimaj; xklima22@stud.fit.vutbr.cz
// Description: IpkClient, HTTP client for 2nd IPK project

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
        LogHead,
        LogPayload,
    }

    /// <summary>
    /// Ipk client.
    /// </summary>
    class IpkClient
    {
        private string       uri_str;
        private Uri          uri;
        private TcpClient    server;
        private string       execPath;

        /// <summary>
        /// Initializes a new instance of the <see cref="IPK.IpkClient"/> class.
        /// </summary>
        /// <param name="uri">URI.</param>
        public IpkClient(string uri)
        {
            this.uri_str = uri;
            this.execPath = Path.GetDirectoryName(System.Reflection.Assembly.
                GetExecutingAssembly().GetName().CodeBase);
            this.execPath = this.execPath.Replace("file:", "");
        }

        /// <summary>
        /// Sends the request to passed URI or throws error.
        /// </summary>
        public void SendRequest()
        {
            bool ok = Uri.IsWellFormedUriString(uri_str, UriKind.Absolute);

            if(!ok)
            {
                throw new Exception("Not well formed URI string");
            }

            uri = new Uri(uri_str, UriKind.Absolute);

            if(uri.Scheme != "http")
            {
                throw new Exception("Protocol must be http");
            }

            server = new TcpClient();
            
            IPAddress addr;
            int port;

            // Convert Host to IPAddress
            try
            {
                addr = Dns.GetHostEntry(uri.Host).AddressList[0];
            }
            catch
            {
                throw new Exception("Failed to convert Host string to IPAddress");
            }

            // Convert Port to integer
            try
            {
                port = Convert.ToInt32(uri.Port);
            }
            catch
            {
                throw new Exception("Failed to convert Port to Int32");
            }

            // Connect to server
            try
            {
                server.Connect(addr, port);
            }
            catch
            {
                throw new Exception("Failed to connect to server");
            }

            string reqStr = FormRequest(uri);
            byte[] reqBytes = Encoding.UTF8.GetBytes(reqStr);
            NetworkStream strm = server.GetStream();

            Log(LogType.LogOut, reqStr);

            // Send request
            try
            {
                strm.Write(reqBytes, 0, reqBytes.Length);
            }
            catch
            {
                throw new Exception("Could not write to server socket");
            }

            Thread.Sleep(10);

            byte[] respBytes = new byte[server.ReceiveBufferSize];
            strm.Read(respBytes, 0, respBytes.Length);
            string response = "";

            // At least it works ;)
            int j=0;
            while(true)
            {
                if(respBytes[j] > 0)
                {
                    response += Convert.ToChar(respBytes[j]);
                }
                else
                {
                    break;
                }
                j++;
            }

            // Wait for chunks if Transfer-Encoding: chunked is in header
            string chunkBody = "";
            bool error = false;
            bool chunked = false;
            string content = "";
            if(response.Contains("Transfer-Encoding: chunked"))
            {
                int c;
                int cnt = 0;
                bool bSize = true;
                bool bSizeR = false;
                bool bStrR = false;
                string xSize = "";
                int nSize = 0;

                chunked = true;

                while(true)
                {
                    c = strm.ReadByte();
                    if(c == -1)
                    {
                        break;
                    }
                    
                    chunkBody += Convert.ToChar(c);
                    
                    if(bSize)
                    {
                        xSize += Convert.ToChar(c);
                        if(c == '\r')
                        {
                            bSizeR = true;
                        }
                        
                        if(c == '\n')
                        {
                            if(!bSizeR)
                            {
                                error = true;
                                break;
                            }
                            else
                            {
                                bSize = false;
                                bSizeR = false;
                                nSize = Convert.ToInt32(xSize.Trim(), 16);
                                cnt = 0;
                                xSize = "";
                            }
                        }
                    }
                    else
                    {
                        if(cnt < nSize)
                        {
                            content += Convert.ToChar(c);
                            cnt++;
                        }
                        else
                        {
                            if(!bStrR)
                            {
                                if(c != '\r')
                                {
                                    error = true;
                                    break;
                                }
                                else
                                {
                                    bStrR = true;
                                }
                            }
                            else
                            {
                                if(c == '\n')
                                {
                                    bStrR = false;
                                    bSize = true;
                                }
                                else
                                {
                                    error = true;
                                    break;
                                }
                            }
                        }
                    }
                }
            }

            if(error)
            {
                Console.Error.WriteLine("CLIENT: Invalid Chunked Transfer Encoding");
            }

            // Create logs
            if(chunked)
            {
                Log(LogType.LogHead, response);
                Log(LogType.LogIn, response + chunkBody);
                
                Log(LogType.LogPayload, content);
            }
            else
            {
                string head;
                string cntnt;
                SplitByString(response, "\r\n\r\n", out head, out cntnt);
                Log(LogType.LogIn, response);
                Log(LogType.LogHead, head);
                Log(LogType.LogPayload, cntnt);
            }

            // Close socket and stream
            strm.Close();
            server.Close();
        }

        /// <summary>
        /// Forms the request.
        /// </summary>
        /// <returns>The request.</returns>
        /// <param name="uri">URI.</param>
        private string FormRequest(Uri uri)
        {
            string request =
                "GET " + uri.LocalPath + " HTTP/1.1\r\n" +
                "Host: " + uri.Host + ":" + uri.Port + "\r\n" +
                "Connection: keep-alive\r\n" +
                "User-Agent: IpkClient\r\n\r\n";
                
            return request;
        }

        /// <summary>
        /// Log the specified logType and data.
        /// </summary>
        /// <param name="logType">Log type.</param>
        /// <param name="data">Data to log.</param>
        private void Log(LogType logType, string data)
        {
            string fileName;

            fileName = CreateFile(logType);

            if(File.Exists(fileName))
            {
                File.Delete(fileName);
            }

            File.WriteAllText(fileName, data);
        }

        /// <summary>
        /// Creates the file name.
        /// </summary>
        /// <returns>The file name.</returns>
        /// <param name="logType">Log type.</param>
        private string CreateFile(LogType logType)
        {
            string timestamp = DateTime.Now.ToString("yyyy-MM-dd:HH:mm:ss");
            string suffix;
            if(logType == LogType.LogIn)
            {
                suffix = "in.log";
            }
            else if(logType == LogType.LogOut)
            {
                suffix = "out.log";
            }
            else if(logType == LogType.LogHead)
            {
                suffix = "header";
            }
            else
            {
                suffix = "payload";
            }
            
            string fn = "ipkHttpClient-" + timestamp + "." + suffix;
            fn = execPath + "/" + fn;
            Console.WriteLine("CLIENT: Creating Log file: {0}", fn);
            
            return fn;
        }

        /// <summary>
        /// Splits the string by other string to head and content strings.
        /// </summary>
        /// <param name="orig">Original string.</param>
        /// <param name="pattern">Pattern.</param>
        /// <param name="head">Head.</param>
        /// <param name="content">Content.</param>
        private void SplitByString(string orig, string pattern, out string head,
        out string content)
        {
            head    = "";
            content = "";

            int i = orig.IndexOf(pattern);
            if(i > -1)
            {
                head = orig.Substring(0, i+pattern.Length);
                content = orig.Substring(i+pattern.Length);
            }
        }
    }

    /// <summary>
    /// Client main.
    /// </summary>
    class ClientMain
    {
        /// <summary>
        /// The entry point of the program, where the program control starts and ends.
        /// </summary>
        /// <param name="args">The command-line arguments.</param>
        public static void Main(string[] args)
        {
            if(args.Length != 1)
            {
                Console.Error.WriteLine("CLIENT: Argument required");
                Environment.Exit(1);
            }

            if((args[0] == "-h") || (args[0] == "--help"))
            {
                PrintHelp();
                Environment.Exit(0);
            }

            IpkClient client = new IpkClient(args[0]);

            try
            {
                client.SendRequest();
            }
            catch(Exception ex)
            {
                Console.WriteLine("CLIENT: {0}", ex.Message);
                Environment.Exit(1);
            }

            Environment.Exit(0);
        }

        /// <summary>
        /// Prints the help.
        /// </summary>
        private static void PrintHelp()
        {
            string helpMsg =
                "Program: IPK #2 - Simple HTTP Client\n" +
                "Author:  Daniel Klimaj; xklima22@stud.fit.vutbr.cz\n" +
                "Usage:   ./ipkhttpclient [-h] URI\n\n" +
                "Parameters:\n" +
                "\t-h | --help \tShows this message";

            Console.WriteLine(helpMsg);
        }
    }
}
