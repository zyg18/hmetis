#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <getopt.h>
#define mp make_pair
using namespace std;

class Config {
public:
    int numNodeLeft = 200;   // number of node left in the last layer of coarsening
    int numInstances = 20;  // number of initial partition done in "initial partition" phase
    double dropRate = 0.3;    // drop partition this rate worse than the best partition in each uncoarsening layer
    int maxCoarsenIter = 30;   // maximum number of coarening layer (stop coarsening even if the number of node left is larger than "numNodeLeft")
    int maxRefineIter = 30;    // maximum number of coarening layer in refinement (stop coarsening even if the number of node left is larger than "numNodeLeft")
    string coarsenScheme = "MHEC";    // coarsening scheme when coarsening (HEC/MHEC)
    string coarsenScheme_r = "MHEC"; // coarsening scheme when doing refinement (HEC/MHEC)
    string initialParScheme = "FM";
    string uncoarsenScheme = "FM";
    string uncoarsenScheme_r = "FM";
};
class BOX{
    public:
        BOX(){}
        int numOfInst;
        int numOfNet;
        vector<int>layer;
        vector<vector<int> > net;
        int Maxpercent;
};

static bool isInt(char* c)
{
    for (int i=0; c[i]; i++)
    {
        if (!isdigit(c[i]))
            return false;
    }
    return true;
}

static bool parsing(int argc, char* argv[], Config& cf, BOX &box){
    const struct option opts[] = {
        {"nleft", 1, NULL, 'n'},
        {"ileft", 1, NULL, 'i'},
        {"dr", 1, NULL, 'r'},
        {"max-itr-c", 1, NULL, 'I'},
        {"max-itr-r", 1, NULL, 'R'},
        {"coarsen-scheme", 1, NULL, 'A'},
        {"refine-scheme", 1, NULL, 'B'},
        {"IP-scheme", 1, NULL, 'C'},
        {"UC-scheme", 1, NULL, 'D'},
        {"UC-scheme-r", 1, NULL, 'E'}
    };
    const char *optstring = "n:i:r:I:R:A:B:C:D:E";
    int c;

    while ((c = getopt_long(argc, argv, optstring, opts, NULL)) != -1)
    {
        switch(c)
        {
            case 'n':
                if (!isInt(optarg))
                {
                    printf ("--nleft: Please enter a positive integer\n");
                    return false;
                }
                cf.numNodeLeft = atoi(optarg);
                break;
            case 'i':
                if (!isInt(optarg))
                {
                    printf ("--ileft: Please enter a positive integer\n");
                    return false;
                }
                cf.numInstances = atoi(optarg);
                break;
            case 'r':
            {
                double value = std::atof(optarg);
                if (value == 0.0 && optarg != "0")
                {
                    printf("Drop rate (--dr) invalid.\n");
                    return false;
                }
                else if (value > 1 || value < 0)
                {
                    printf("Drop rate (--dr) must be a value between [0,1].\n");
                    return false;
                }
                else
                    cf.dropRate = value;
                break;
            }
            case 'I':
                if (!isInt(optarg))
                {
                    printf ("--max-itr-c: Please enter a positive integer\n");
                    return false;
                }
                cf.maxCoarsenIter = atoi(optarg);
                break;
            case 'R':
                if (!isInt(optarg))
                {
                    printf ("--max-itr-r: Please enter a positive integer\n");
                    return false;
                }
                cf.maxRefineIter = atoi(optarg);
                break;
            case 'A':
                if (strncmp(optarg, "EC", 2) == 0 || strncmp(optarg, "HEC", 3) == 0 || strncmp(optarg, "MHEC", 3) == 0)
                    cf.coarsenScheme = (string)optarg;
                else
                {
                    printf("Coarsen scheme (--coarsen-scheme) invalid, please use EC/HEC/MHEC.\n");
                    return false;
                }
                break;
            case 'B':
                if (strncmp(optarg, "EC", 2) == 0 || strncmp(optarg, "HEC", 3) == 0 || strncmp(optarg, "MHEC", 3) == 0)
                    cf.coarsenScheme_r = (string)optarg;
                else
                {
                    printf("Coarsen scheme in refinement phase (--refine-scheme) invalid, please use EC/HEC/MHEC.\n");
                    return false;
                }
                break;
            case 'C':
                if (strncmp(optarg, "FM", 2) == 0 || strncmp(optarg, "HER", 3) == 0)
                    cf.initialParScheme = (string)optarg;
                else
                {
                    printf("Initial partition scheme (--IP-scheme) invalid, please use FM/HER.\n");
                    return false;
                }
                break;
            case 'D':
                if (strncmp(optarg, "FM", 2) == 0 || strncmp(optarg, "HER", 3) == 0)
                    cf.uncoarsenScheme = (string)optarg;
                else
                {
                    printf("Uncoarsen scheme (--UC-scheme) invalid, please use FM/HER.\n");
                    return false;
                }
                break;
            case 'E':
                if (strncmp(optarg, "FM", 2) == 0 || strncmp(optarg, "HER", 3) == 0)
                    cf.uncoarsenScheme_r = (string)optarg;
                else
                {
                    printf("Uncoarsen scheme in refinement phase (--UC-scheme-r) invalid, please use FM/HER.\n");
                    return false;
                }
                break;
            case '?':
                printf("Unknown option %s\n", optarg);
                break;
            default:
                printf("------------------------------------\n");
        }
    }

    ifstream inputf(argv[optind]);
    if (!inputf.is_open())
    {
        printf("Cannot open input file...\n");
        return false;
    }
    
    string k = argv[optind];
    stringstream ss;
    int numOfInst, numOfNet;
    inputf>>numOfNet>>numOfInst;
    box.numOfInst=numOfInst;
    box.numOfNet=numOfNet;
    box.net.resize(box.numOfNet+1);
    string line;
    getline(inputf, line);

    for(int i=1;i<=numOfNet;i++){
      getline(inputf, line);
      stringstream ss(line);
      int num;
      while (ss >> num) {
        box.net[i].push_back(num);
      }
    }

    box.Maxpercent = 52;
    
    return true;
}
static bool isBookshelfSymbol(unsigned char c) {
    static char symbols[256] = {0};
    static bool inited = false;
    if (!inited) {
        symbols[(int)'('] = 1;
        symbols[(int)')'] = 1;
        // symbols[(int)'['] = 1;
        // symbols[(int)']'] = 1;
        symbols[(int)','] = 1;
        // symbols[(int)'.'] = 1;
        symbols[(int)':'] = 1;
        symbols[(int)';'] = 1;
        // symbols[(int)'/'] = 1;
        symbols[(int)'#'] = 1;
        symbols[(int)'{'] = 1;
        symbols[(int)'}'] = 1;
        symbols[(int)'*'] = 1;
        symbols[(int)'\"'] = 1;
        symbols[(int)'\\'] = 1;

        symbols[(int)' '] = 2;
        symbols[(int)'\t'] = 2;
        symbols[(int)'\n'] = 2;
        symbols[(int)'\r'] = 2;
        inited = true;
    }
    return symbols[(int)c] != 0;
}
static bool readBSLine(istream& is, vector<string>& tokens) {
    tokens.clear();
    string line;
    while (is && tokens.empty()) {
        // read next line in
        getline(is, line);

        char token[1024] = {0};
        int lineLen = (int)line.size();
        int tokenLen = 0;
        for (int i = 0; i < lineLen; i++) {
            char c = line[i];
            if (c == '#') {
                break;
            }
            if (isBookshelfSymbol(c)) {
                if (tokenLen > 0) {
                    token[tokenLen] = (char)0;
                    tokens.push_back(string(token));
                    token[0] = (char)0;
                    tokenLen = 0;
                }
            } else {
                token[tokenLen++] = c;
                if (tokenLen > 1024) {
                    // TODO: unhandled error
                    tokens.clear();
                    return false;
                }
            }
        }
        // line finished, something else in token
        if (tokenLen > 0) {
            token[tokenLen] = (char)0;
            tokens.push_back(string(token));
            tokenLen = 0;
        }
    }
    return !tokens.empty();
}