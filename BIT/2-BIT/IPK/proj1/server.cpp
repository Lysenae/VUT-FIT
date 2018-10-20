 /***
 * Author:      Daniel Klimaj; xklima22@stud.fit.vutbr.cz
 * Description: IPK Project 1 - Program for searching information about users of
 *              UNIX OS
 * File:        server.cpp
 ***/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <string>

using namespace std;

static const int BUF_MAX = 256;
static const int MAX_CONNECTIONS = 10;

// Converts char to int ('1' to 1)
int ctoi(char c)
{
  return (int)c - '0';
}

string to_lower(string s)
{
  string y = "";

  for(int i=0; i<s.length(); i++)
  {
    y += tolower(s[i]);
  }

  return y;
}

// Splits string by delimiter
vector<string> split(string s, char delimiter, bool lower = false)
{
  vector<string> strings;
  string current = "";
  for(int i=0; i<s.length(); i++)
  {
    if(s[i] == delimiter || i == s.length()-1)
    {
      if(i == s.length()-1 && s[i] != delimiter)
      {
        current += s[i];
      }
      if(!lower)
      {
        strings.push_back(current);
      }
      else
      {
        strings.push_back(to_lower(current));
      }
      current.clear();
    }
    else
    {
      current += s[i];
    }
  }

  return strings;
}

// Collect dead processes
void reap_zombies(int i)
{
  while(waitpid(-1, NULL, WNOHANG) > 0);
}

// Main
int main(int argc, char **argv)
{
  if(argc != 3)
  {
    cerr << "Invalid number of parameters:\nUSAGE: " << argv[0] << " -p port\n";
    return -1;
  }

  if(string(argv[1]) != "-p")
  {
    cerr << "Invalid option " << argv[1] << endl;
    return -1;
  }

  if(atoi(argv[2]) < 1024)
  {
    cerr << "Invalid or reserved port number " << argv[2] << endl;
    return -1;
  }

  int sock, req, pid;
  socklen_t req_size;
  char msg[BUF_MAX];
  struct hostent * hp;
  struct sockaddr_in sin;
  struct sigaction sigact;

  if((sock = socket(PF_INET, SOCK_STREAM, 0 )) < 0)
  {
    cerr << "Failed to create socket\n";
    return -1;
  }

  sin.sin_family       = PF_INET;
  sin.sin_port         = htons(atoi(argv[2]));
  sin.sin_addr.s_addr  = INADDR_ANY;

  if(bind(sock, (struct sockaddr *)&sin, sizeof(sin)) < 0)
  {
    cerr << "Error binding socket\n";
    return -1;
  }

  if(listen(sock, MAX_CONNECTIONS) < 0)
  {
    cerr << "Failed to listen socket (" << sock << ")\n";
    return -1;
  }

  // Reap zombie processes
  sigact.sa_handler = reap_zombies;
  sigemptyset(&sigact.sa_mask);
  sigact.sa_flags = SA_RESTART;
  if(sigaction(SIGCHLD, &sigact, NULL) < 0)
  {
      cerr << "SIGCHLD error\n";
      return -1;
  }

  // Wait for requests
  req_size = sizeof(sin);
  while(1)
  {
    if((req = accept(sock, (struct sockaddr *) &sin, &req_size)) < 0)
    {
      cerr << "Error on accept()\n";
      return -1;
    }

    // Fork process to handle multiple requests at once
    pid = fork();
    if(pid == 0) // Child process creation successful
    {
      string request = "";

      // Get whole message even when is longer than BUF_MAX
      while(1)
      {
        memset(msg, 0, sizeof(msg));
        if(recv(req, msg, sizeof(msg)-1, 0) < 0)
        {
          cerr << "Error on recv\n";
          return -1;
        }
        else
        {
          //cout << "Received: " << msg << endl;
          request = request + string(msg);
          if(request[request.length()-1] == ':')
          {
            break;
          }
        }
      }

      // Remove ':' from the end of the request
      if(request.length() > 0)
      {
        request.erase(request.length() - 1);
      }

      cout << "Request: " << request << endl;

      // Process request's content
      int lf = ctoi(request[0]);      // -L param order
      int uf = ctoi(request[1]);      // -U param order
      int gf = ctoi(request[2]);      // -G param order
      int nf = ctoi(request[3]);      // -N param order
      int hf = ctoi(request[4]);      // -H param order
      int sf = ctoi(request[5]);      // -S param order
      int n  = ctoi(request[6]);      // Number of requested columns

      int method = ctoi(request[7]);  // 1 (login) or 2 (uid)
      string key = request.substr(8); // Rest of request are logins/UIDs
      vector<string> keys = split(key, ' ', true);
      vector<string> lines;
      vector<vector<string> > valid_lines;
      string reply = "";
      string reply_part = "";

      // Open /etc/passwd and store it's contents in string vector
      ifstream file("/etc/passwd");
      string line;
      while(getline(file, line))
      {
        lines.push_back(line);
      }

      // Find desired records by login/UID
      for(int i=0; i<keys.size(); i++)
      {
        for(int j=0; j<lines.size(); j++)
        {
          vector<string> line_parts = split(lines[j], ':');
          if(method == 1) // Search by login
          {

            if(keys[i].compare(to_lower(line_parts[0])) == 0)
            {
              valid_lines.push_back(line_parts);
            }
          }
          else            // Search by UID
          {
            if(keys[i].compare(line_parts[2]) == 0)
            {
              valid_lines.push_back(line_parts);
            }
          }
        }

          // Add reply parts according to param order sent by client
        if(valid_lines.size() > 0)
        {
          for(int v=0; v<valid_lines.size(); v++)
          {
            for(int x=1; x<=n; x++)
            {
              if(lf == x)
              {
                if(!reply_part.empty())
                {
                  reply_part += ' ';
                }
                reply_part.append(valid_lines[v][0]);
              }
              if(uf == x)
              {
                if(!reply_part.empty())
                {
                  reply_part += ' ';
                }
                reply_part.append(valid_lines[v][2]);
              }
              if(gf == x)
              {
                if(!reply_part.empty())
                {
                  reply_part += ' ';
                }
                reply_part.append(valid_lines[v][3]);
              }
              if(nf == x)
              {
                if(!reply_part.empty())
                {
                  reply_part += ' ';
                }
                reply_part.append(valid_lines[v][4]);
              }
              if(hf == x)
              {
                if(!reply_part.empty())
                {
                  reply_part += ' ';
                }
                reply_part.append(valid_lines[v][5]);
              }
              if(sf == x)
              {
                if(!reply_part.empty())
                {
                  reply_part += ' ';
                }
                reply_part.append(valid_lines[v][6]);
              }
            }
            reply_part += '\n';
            reply.append(reply_part);
            reply_part.clear();
          }
        }
        else // Record not found, append error message to reply
        {
          reply_part = "Error: Unknown " + string(method == 1 ? "login" : "uid") +
            " " + keys[i] + "\n";
          reply.append(reply_part);
          reply_part.clear();
        }

        reply.append(reply_part);
        reply_part.clear();
        valid_lines.clear();
      }

      // Send reply
      reply += ':';
      if(send(req, reply.c_str(), reply.length(), 0) < 0)
      {
        cerr << "Error on send\n";
        return -1;
      }

      if(close(req) < 0)
      {
        cerr << "Error on close\n";
        return -1;
      }
      exit(0); // Exit child process
    }
  }

  if(close(sock) < 0)
  {
    cerr << "Error while closing socket (" << sock << ")\n";
    return -1;
  }

  return 0;
}

// server.cpp
