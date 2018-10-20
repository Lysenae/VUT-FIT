 /***
 * Author:      Daniel Klimaj; xklima22@stud.fit.vutbr.cz
 * Description: IPK Project 1 - Program for searching information about users of
 *              UNIX OS
 * File:        client.cpp
 ***/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

static const int BUF_MAX = 256;

// Splits string by delimiter
vector<string> split(string s, char delimiter)
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

      strings.push_back(current);
      current.clear();
    }
    else
    {
      current += s[i];
    }
  }

  return strings;
}

// Converts int to char (1 to '1')
char itoc(int i)
{
  return (char)i + '0';
}

// Main
int main(int argc, char **argv)
{
  string key  = "";
  string port = "";
  string host = "";

  bool login = false;
  bool uid   = false;

  int lflag = 0;
  int uflag = 0;
  int gflag = 0;
  int nflag = 0;
  int hflag = 0;
  int sflag = 0;

  int ch, tmp;
  int search_idx  = 0;
  int current_idx = 0;
  int order       = 0;

  string synopsis = "USAGE: " + string(argv[0]) +
    " -h host -p port -l login -u uid [-L -U -G -N -H -S]";

  // Parse aprams and set particular variables
  opterr = 0;
  while((ch = getopt(argc, argv, "h:p:l:u:LUGNHS")) != -1)
  {
    switch(ch)
    {
      case 'h':
        host.assign(optarg);
        current_idx++;
        break;
      case 'p':
        port.assign(optarg);
        current_idx++;
        break;
      case 'l':
        uid = false;
        login = true;
        key.assign(optarg);
        current_idx++;
        search_idx = current_idx;
        tmp = optind;
        while(tmp < argc && argv[tmp][0] != '-')
        {
          key += ' ';
          key.append(argv[tmp]);
          tmp++;
        }
        optind = tmp - 1;
        break;
      case 'u':
        login = false;
        uid = true;
        key.assign(optarg);
        current_idx++;
        search_idx = current_idx;
        tmp = optind;
        while(tmp < argc && argv[tmp][0] != '-')
        {
          key += ' ';
          key.append(argv[tmp]);
          tmp++;
        }
        optind = tmp - 1;
        break;
      case 'L':
        order++;
        lflag = order;
        current_idx++;
        break;
      case 'U':
        order++;
        uflag = order;
        current_idx++;
        break;
      case 'G':
        order++;
        gflag = order;
        current_idx++;
        break;
      case 'N':
        order++;
        nflag = order;
        current_idx++;
        break;
      case 'H':
        order++;
        hflag = order;
        current_idx++;
        break;
      case 'S':
        order++;
        sflag = order;
        current_idx++;
        break;
      case '?':
        cerr << "Unrecognized option used\n" << synopsis << endl;
        return -1;
      default:
        break;
    }
  }

  if(host.empty())
  {
    cerr << "Unspecified host\n" << synopsis << endl;
    return -1;
  }

  if(port.empty())
  {
    cerr << "Unspecified port\n" << synopsis << endl;
    return -1;
  }

  if(atoi(port.c_str()) < 1024)
  {
    cerr << "Invalid or reserved port number " << port << endl;
    return -1;
  }

  // No search method specified
  if(!uid && !login)
  {
    cerr << "Undefined search method\n" << synopsis << endl;
    return -1;
  }

  // No data to return
  if(!lflag && !uflag && !gflag && !nflag && !hflag && !sflag)
  {
    cerr << "Unspecified data to search\n" << synopsis;
    return -1;
  }

  int    n, sock;
  struct sockaddr_in sin;
  struct hostent *hptr;

  if((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
  {
    cerr << "Failed to create socket\n";
    return -1;
  }

  sin.sin_family = PF_INET;
  sin.sin_port   = htons(atoi(port.c_str()));
  if((hptr = gethostbyname(host.c_str())) == NULL)
  {
    cerr << "Gethostname error: " << host << endl;
    return -1;
  }
  memcpy(&sin.sin_addr, hptr->h_addr, hptr->h_length);

  if(connect(sock, (struct sockaddr *)&sin, sizeof(sin)) < 0)
  {
    cerr << "Error connecting socket\n";
    return -1;
  }

  // Create request message
  // Index 0-5 - order of params (L, U, G, N, H, S)
  // Index 6   - number of specified params (should be > 0)
  // Index 7   - search method (1 - by login, 2 - by UID)
  // Index 8-n - keys (logins/uid) separated by space
  string request = "";
  request += itoc(lflag);
  request += itoc(uflag);
  request += itoc(gflag);
  request += itoc(nflag);
  request += itoc(hflag);
  request += itoc(sflag);
  request += itoc(order);
  request += (login ? '1' : '2');
  request.append(key);
  request += ':';

  if(send(sock, request.c_str(), request.length(), 0) < 0)
  {
    cerr << "Error on send\n";
    return -1;
  }

  char msg[BUF_MAX];
  string reply = "";

  // Get whole message even when is longer than BUF_MAX
  while(1)
  {
    memset(msg, 0, sizeof(msg));
    if(recv(sock, msg, sizeof(msg)-1, 0) < 0)
    {
      cerr << "Error on recv\n";
      return -1;
    }
    else
    {
      //cout << "Received: " << msg << endl;
      reply = reply + string(msg);
      if(reply[reply.length()-1] == ':')
      {
        break;
      }
    }
  }

  // Remove ':' from the end of the reply
  if(reply.length() > 0)
  {
    reply.erase(reply.length() - 1);
  }

  // Check for error messages and print replies
  int is_error;
  vector<string> replies = split(reply, '\n');
  for(int i=0; i<replies.size(); i++)
  {
    is_error = replies[i].find("Error: Unknown");
    if(is_error != std::string::npos)
    {
      cerr <<replies[i] << endl;
    }
    else
    {
      cout << replies[i] << endl;
    }
  }

  if(close(sock) < 0)
  { 
    cerr << "Error on close\n";
    return -1;
  }

  return 0;
}

// client.cpp
