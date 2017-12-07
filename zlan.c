/*
ZLAN, Zach's Lexical Analyzer for Netrek.
Copyright (C) 2010  Zachary Uram

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

  * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in
    the documentation and/or other materials provided with the
    distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

--

Parses a netrek game log, from client or INL server, for the beginning
of line note the timestamp and save all human generated messages to
TEAM, ALL and INDIVIDUAL to a file in HTML format.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>

#define MAXLINE 90
#define MAXLINEZ 30

#define TRUE 1
#define FALSE 0

unsigned int return_tokens(char *);
unsigned int process_tokens(char *);
void render(char *, char *);
void renderq(char *, char *);
void prephtml(char *);
void finishhtml(char *);
void prephtmlF(void);
void prephtmlR(void);
void prephtmlO(void);
void prephtmlK(void);
void prephtmlA(void);
void finishhtmlF(void);
void finishhtmlR(void);
void finishhtmlO(void);
void finishhtmlK(void);
void finishhtmlA(void);
unsigned int setContains(char x[], size_t currentSetSize, char y[]);

typedef struct
{
    unsigned int haveI;
    unsigned int haveA;
    unsigned int haveT;
    unsigned int haveF;
    unsigned int haveR;
    unsigned int haveO;
    unsigned int haveK;
} dohave;

dohave exists;
dohave check_exists(char *);

int main(int argc, char *argv[])
{
    char line[MAXLINE] = {0};
    char tokens[MAXLINE] = {0};
    char lineC[MAXLINE] = {0};
    char tokensC[MAXLINE] = {0};
    char lineA[MAXLINE] = {0};
    char tokensA[MAXLINE] = {0};
    char lineF[MAXLINE] = {0};
    char tokensF[MAXLINE] = {0};
    char lineR[MAXLINE] = {0};
    char tokensR[MAXLINE] = {0};
    char lineO[MAXLINE] = {0};
    char tokensO[MAXLINE] = {0};
    char lineK[MAXLINE] = {0};
    char tokensK[MAXLINE] = {0};
    char renderline[MAXLINE] = {0};
    char lineq[MAXLINE] = {0};
    char tokensq[MAXLINE] = {0};
    char renderlineq[MAXLINE] = {0};
    char *tbuf[MAXLINE];
    char *tbufC[MAXLINE];
    char *tbufA[MAXLINE];
    char *tbufF[MAXLINE];
    char *tbufR[MAXLINE];
    char *tbufO[MAXLINE];
    char *tbufK[MAXLINE];
    char *tbufq[MAXLINE];


    unsigned int do_finishhtml = 0;
    unsigned int do_finishhtmlF = 0;
    unsigned int do_finishhtmlR = 0;
    unsigned int do_finishhtmlO = 0;
    unsigned int do_finishhtmlK = 0;
    unsigned int do_finishhtmlA = 0;

    FILE *fpin, *fpinC, *fpinA, *fpinF, *fpinR, *fpinO, *fpinK, *fpinq;

    exists.haveI = 0;
    exists.haveA = 0;
    exists.haveT = 0;
    exists.haveF = 0;
    exists.haveR = 0;
    exists.haveO = 0;
    exists.haveK = 0;

    char template[] = "tmpfileXXXXXX";
#ifdef WIN32
    mktemp(template);
#else
    mkstemp(template);
#endif

    char templateq[] = "tmpfileXXXXXX";
#ifdef WIN32
    mktemp(templateq);
#else
    mkstemp(templateq);
#endif

    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s input_file ouput_file\n", argv[0]);
        return EXIT_FAILURE;
    }

    if ((fpin = fopen(argv[1], "r")) == NULL)
    {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    if ((fpinC = fopen(argv[1], "r")) == NULL)
    {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    if ((fpinA = fopen(argv[1], "r")) == NULL)
    {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    if ((fpinF = fopen(argv[1], "r")) == NULL)
    {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    if ((fpinR = fopen(argv[1], "r")) == NULL)
    {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    if ((fpinO = fopen(argv[1], "r")) == NULL)
    {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    if ((fpinK = fopen(argv[1], "r")) == NULL)
    {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    if ((fpinq = fopen(argv[1], "r")) == NULL)
    {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    const char *delims = {" "};

    while (fgets(lineq, MAXLINE, fpinq) != NULL)
    {

        unsigned int i = {0};
        unsigned int t = {0};
        unsigned int r = {0};

        lineq[strlen(lineq)-1] = '\0'; /* remove trailing newline */
        strcpy(tokensq,lineq);
        strcpy(renderlineq,lineq);

        tbufq[0] = strtok(tokensq,delims);

        t = return_tokens(lineq);

        for (i=0 ; i < t; i++)
        {

            tbufq[i] = strtok(NULL,delims);

            if (tbufq[i] == '\0')
            {
                printf("tbufq[i] is NULL\n");
                break;
            }
            else
            {
                r = process_tokens(tbufq[i]);
                if (r == 1)
                {
                    renderq(renderlineq,templateq);
                }
            }
        }
    }

    FILE *fpy;
    fpy = fopen(templateq,"r");

    long sizeq = 0;
    fseek(fpy, 0, SEEK_END);
    sizeq = ftell(fpy);
    fclose (fpy);

    char liney[MAXLINEZ] = {0};
    fpy = fopen(templateq,"r");
    char iSet[sizeq];
    unsigned int y;

    y = 0;
    size_t currentSetSizeI = 0;

    while (fgets(liney, MAXLINEZ, fpy) != NULL)
    {

        currentSetSizeI = strlen(iSet);

        if(setContains(iSet, currentSetSizeI, liney) == TRUE)
        {
        }
        else
        {
            iSet[y+y] = liney[9];
            iSet[y+y+1] = liney[10];
            y++;
        }
    }

    fclose(fpy);

    size_t SetSizeI = 0;
    SetSizeI = strlen(iSet);

    unsigned int h = 0;
    char q[8] = {0};
    for (h=0; h < (SetSizeI/2); h++)
    {
        q[0] = iSet[h+h];
        q[1] = iSet[h+h+1];
        q[2] = '.';
        q[3] = 'h';
        q[4] = 't';
        q[5] = 'm';
        q[6] = 'l';
        q[7] = '\0';

        FILE *fpu;

        fpu = fopen(q, "w+");
        fprintf(fpu,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
        fprintf(fpu,"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\"\n");
        fprintf(fpu,"                                    \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">\n");
        fprintf(fpu,"\n");
        fprintf(fpu,"<html\n");
        fprintf(fpu,"      xmlns=\"http://www.w3.org/1999/xhtml\"\n");
        fprintf(fpu,"      xml:lang=\"en-US\"\n");
        fprintf(fpu,"      lang=\"en-US\">\n");
        fprintf(fpu,"<head profile=\"http://www.w3.org/2005/10/profile\">\n");
        fprintf(fpu,"<link rel=\"icon\"\n");
        fprintf(fpu,"            type=\"image/ico\"\n");
        fprintf(fpu,"                  href=\"images/gytha-favicon.ico\" />\n");
        fprintf(fpu,"<meta http-equiv=\"Content-Type\" content=\"text/html;\" charset=\"utf-8\" />\n");
        fprintf(fpu,"<meta name=\"copyright\" content=\"Copyright 2010 oggthebase.org\" />\n");
        fprintf(fpu,"<meta name=\"robots\" content=\"index,follow\" />\n");
        fprintf(fpu,"<meta name=\"keywords\" content=\"gytha, netrek, client, documentation\" />\n");
        fprintf(fpu,"<meta name=\"description\" content=\"Settup up gytha dev environment\" />\n");
        fprintf(fpu,"<meta name=\"generator\" content=\"Coded in Emacs\" />\n");
        fprintf(fpu,"\n");
        fprintf(fpu,"<title>ZLAN</title>\n");
        fprintf(fpu,"<style type=\"text/css\">\n");
        fprintf(fpu,"\n");
        fprintf(fpu,"body {\n");
        fprintf(fpu,"    font: normal 11px auto \"Trebuchet MS\", Verdana, Arial, Helvetica, sans-serif;\n");
        fprintf(fpu,"    color: #4f6b72;\n");
        fprintf(fpu,"    background: #E6EAE9;\n");
        fprintf(fpu,"}\n");
        fprintf(fpu,"\n");
        fprintf(fpu,"a {\n");
        fprintf(fpu,"    color: #c75f3e;\n");
        fprintf(fpu,"  }\n");
        fprintf(fpu,"\n");
        fprintf(fpu,"#mytable {\n");
        fprintf(fpu,"    width: 700px;\n");
        fprintf(fpu,"    padding: 0;\n");
        fprintf(fpu,"    margin: 0;\n");
        fprintf(fpu,"    border-collapse: collapse;\n");
        fprintf(fpu,"}\n");
        fprintf(fpu,"\n");
        fprintf(fpu,"caption {\n");
        fprintf(fpu,"    padding: 0 0 5px 0;\n");
        fprintf(fpu,"    width: 700px;\n");
        fprintf(fpu,"    font: italic 11px \"Trebuchet MS\", Verdana, Arial, Helvetica, sans-serif;\n");
        fprintf(fpu,"    text-align: right;\n");
        fprintf(fpu,"    font-style: italic;\n");
        fprintf(fpu,"}\n");
        fprintf(fpu,"\n");
        fprintf(fpu,"th.nobg {\n");
        fprintf(fpu,"border-top: 0;\n");
        fprintf(fpu,"border-left: 0;\n");
        fprintf(fpu,"border-right: 1px solid #C1DAD7;\n");
        fprintf(fpu,"background: none;\n");
        fprintf(fpu,"}\n");
        fprintf(fpu,"\n");
        fprintf(fpu,"th.spec {\n");
        fprintf(fpu,"border-left: 1px solid #C1DAD7;\n");
        fprintf(fpu,"border-top: 0;\n");
        fprintf(fpu,"background: #fff no-repeat;\n");
        fprintf(fpu,"font: bold 10px \"Trebuchet MS\", Verdana, Arial, Helvetica,\n");
        fprintf(fpu,"sans-serif;\n");
        fprintf(fpu,"color: red;\n");
        fprintf(fpu,"}\n");
        fprintf(fpu,"\n");
        fprintf(fpu,"td.spec {\n");
        fprintf(fpu,"border-left: 1px solid #C1DAD7;\n");
        fprintf(fpu,"border-top: 0;\n");
        fprintf(fpu,"background: #fff no-repeat;\n");
        fprintf(fpu,"font: bold 10px \"Trebuchet MS\", Verdana, Arial, Helvetica,\n");
        fprintf(fpu,"sans-serif;\n");
        fprintf(fpu,"}\n");
        fprintf(fpu,"\n");
        fprintf(fpu,"</style>\n");
        fprintf(fpu,"\n");
        fprintf(fpu,"<body>\n");
        fprintf(fpu,"  \n");
        fprintf(fpu,"<table id=\"mytable\" cellspacing=\"0\" summary=\"Individual board messages\">\n");
        fprintf(fpu,"  <caption>Individual board messages </caption>\n");
        fprintf(fpu,"  <tr>\n");
        fprintf(fpu,"            <th scope=\"row\" class=\"spec\">%c%c</th>\n",q[0],q[1]);
        fprintf(fpu,"                                    <td class=\"spec\">");
        fclose(fpu);
    }

    while (fgets(lineC, MAXLINE, fpinC) != NULL)
    {
        unsigned int i = {0};
        unsigned int t = {0};

        lineC[strlen(lineC)-1] = '\0'; /* remove trailing newline */
        strcpy(tokensC,lineC);

        tbufC[0] = strtok(tokensC,delims);

        t = return_tokens(lineC);

        for (i=0 ; i < t; i++)
        {

            tbufC[i] = strtok(NULL,delims);


            if (tbufC[i] == '\0')
            {
                printf("tbufC[i] is NULL\n");
                break;
            }
            else
            {
                check_exists(tbufC[i]);
            }
        }

        if (exists.haveI == 1 || exists.haveA == 1 || exists.haveT == 1)
        {
            prephtml(argv[2]);
            do_finishhtml = 1;
            break;
        }
    }
    fclose(fpinC);

    while (fgets(lineA, MAXLINE, fpinA) != NULL)
    {
        unsigned int i = {0};
        unsigned int t = {0};

        lineA[strlen(lineA)-1] = '\0'; /* remove trailing newline */
        strcpy(tokensA,lineA);

        tbufA[0] = strtok(tokensA,delims);

        t = return_tokens(lineA);

        for (i=0 ; i < t; i++)
        {

            tbufA[i] = strtok(NULL,delims);

            if (tbufA[i] == '\0')
            {
                printf("tbufA[i] is NULL\n");
                break;
            }
            else
            {
                check_exists(tbufA[i]);
            }
        }

        if (exists.haveA == 1)
        {
            prephtmlA();
            do_finishhtmlA = 1;
            break;
        }

    }
    fclose(fpinA);


    while (fgets(lineF, MAXLINE, fpinF) != NULL)
    {
        unsigned int i = {0};
        unsigned int t = {0};

        lineF[strlen(lineF)-1] = '\0'; /* remove trailing newline */
        strcpy(tokensF,lineF);

        tbufF[0] = strtok(tokensF,delims);

        t = return_tokens(lineF);

        for (i=0 ; i < t; i++)
        {

            tbufF[i] = strtok(NULL,delims);

            if (tbufF[i] == '\0')
            {
                printf("tbufF[i] is NULL\n");
                break;
            }
            else
            {
                check_exists(tbufF[i]);
            }
        }

        if (exists.haveF == 1)
        {
            prephtmlF();
            do_finishhtmlF = 1;
            break;
        }

    }

    fclose(fpinF);


    while (fgets(lineR, MAXLINE, fpinR) != NULL)
    {
        unsigned int i = {0};
        unsigned int t = {0};

        lineR[strlen(lineR)-1] = '\0'; /* remove trailing newline */
        strcpy(tokensR,lineR);

        tbufR[0] = strtok(tokensR,delims);

        t = return_tokens(lineR);

        for (i=0 ; i < t; i++)
        {

            tbufR[i] = strtok(NULL,delims);

            if (tbufR[i] == '\0')
            {
                printf("tbufR[i] is NULL\n");
                break;
            }
            else
            {
                check_exists(tbufR[i]);
            }
        }

        if (exists.haveR == 1)
        {
            prephtmlR();
            do_finishhtmlR = 1;
            break;
        }

    }

    fclose(fpinR);

    while (fgets(lineO, MAXLINE, fpinO) != NULL)
    {
        unsigned int i = {0};
        unsigned int t = {0};

        lineO[strlen(lineO)-1] = '\0'; /* remove trailing newline */
        strcpy(tokensO,lineO);

        tbufO[0] = strtok(tokensO,delims);

        t = return_tokens(lineO);

        for (i=0 ; i < t; i++)
        {

            tbufO[i] = strtok(NULL,delims);

            if (tbufO[i] == '\0')
            {
                printf("tbufO[i] is NULL\n");
                break;
            }
            else
            {
                check_exists(tbufO[i]);
            }
        }

        if (exists.haveO == 1)
        {
            prephtmlO();
            do_finishhtmlO = 1;
            break;
        }

    }

    fclose(fpinO);

    while (fgets(lineK, MAXLINE, fpinK) != NULL)
    {
        unsigned int i = {0};
        unsigned int t = {0};

        lineK[strlen(lineK)-1] = '\0'; /* remove trailing newline */
        strcpy(tokensK,lineK);

        tbufK[0] = strtok(tokensK,delims);

        t = return_tokens(lineK);

        for (i=0 ; i < t; i++)
        {

            tbufK[i] = strtok(NULL,delims);

            if (tbufK[i] == '\0')
            {
                printf("tbufK[i] is NULL\n");
                break;
            }
            else
            {
                check_exists(tbufK[i]);
            }
        }

        if (exists.haveK == 1)
        {
            prephtmlK();
            do_finishhtmlK = 1;
            break;
        }
    }

    fclose(fpinK);

    while (fgets(line, MAXLINE, fpin) != NULL)
    {

        unsigned int i = {0};
        unsigned int t = {0};
        unsigned int r = {0};

        line[strlen(line)-1] = '\0'; /* remove trailing newline */
        strcpy(tokens,line);
        strcpy(renderline,line);

        tbuf[0] = strtok(tokens,delims);

        t = return_tokens(line);

        for (i=0 ; i < t; i++)
        {

            tbuf[i] = strtok(NULL,delims);

            if (tbuf[i] == '\0')
            {
                printf("tbuf[i] is NULL\n");
                break;
            }
            else
            {
                r = process_tokens(tbuf[i]);
                if (r == 1)
                {
                    render(renderline,template);
                }
            }
        }
    }

    fclose(fpin);

    char lineZ[MAXLINEZ] = {0};
    char linex[MAXLINEZ] = {0};

    FILE *fpz;
    FILE *fpx;
    FILE *fpw;

    fpz=fopen(template,"r");

    long size = 0;
    fseek (fpz, 0, SEEK_END);
    size=ftell (fpz);
    fclose (fpz);
    char checkZ[size];
    char zSet[size];

    fpx=fopen(template,"r");

    unsigned int i;

    i = 0;
    size_t currentSetSize = 0;
    unsigned int counter = 0;
    while (fgets(linex, MAXLINEZ, fpx) != NULL)
    {

        currentSetSize = strlen(zSet);

        if(setContains(zSet, currentSetSize, linex) == TRUE)
        {
        }
        else
        {
            zSet[i+i] = linex[9];
            zSet[i+i+1] = linex[10];

            char p[8] = {0};
            p[0] = zSet[i+i];
            p[1] = zSet[i+i+1];
            p[2] = '.';
            p[3] = 'h';
            p[4] = 't';
            p[5] = 'm';
            p[6] = 'l';
            p[7] = '\0';

            fpw = fopen(argv[2], "a+");
            fprintf(fpw,"<a href=\"%s\">%c%c</a>&nbsp;&nbsp;&nbsp;&nbsp;",p,p[0],p[1]);
            counter++;
            if (counter == 10 || counter == 20 || counter == 30 || counter == 40 || counter == 50 || counter == 60
                    || counter == 70 || counter == 80 || counter == 90 || counter == 100 || counter == 110 || counter == 120)
            {
                fprintf(fpw,"<br>");
            }
            fclose(fpw);

            i++;
        }
    }

    fclose(fpx);

    if (do_finishhtml == 1)
    {
        finishhtml(argv[2]);
    }

    if (do_finishhtmlF == 1)
    {
        finishhtmlF();
    }
    if (do_finishhtmlR == 1)
    {
        finishhtmlR();
    }
    if (do_finishhtmlO == 1)
    {
        finishhtmlO();
    }
    if (do_finishhtmlK == 1)
    {
        finishhtmlK();
    }
    if (do_finishhtmlA == 1)
    {
        finishhtmlA();
    }

    unsigned int b = 0;
    for (b=0; b < (SetSizeI/2); b++)
    {
        char p[8] = {0};
        p[0] = iSet[b+b];
        p[1] = iSet[b+b+1];
        p[2] = '.';
        p[3] = 'h';
        p[4] = 't';
        p[5] = 'm';
        p[6] = 'l';
        p[7] = '\0';

        FILE *fpd;
        fpd = fopen(p, "a+");
        fprintf(fpd,"</td>\n");
        fprintf(fpd," </tr>\n");
        fprintf(fpd,"</table>\n");
        fprintf(fpd,"&nbsp;\n");
        fprintf(fpd,"&nbsp;\n");
        fprintf(fpd,"&nbsp;\n");
        fprintf(fpd,"&nbsp;\n");
        fprintf(fpd,"\n");
        fprintf(fpd,"</body>\n");
        fprintf(fpd,"</html>\n");
        fprintf(fpd,"\n");

        fclose(fpin);
    }

    return 0;
}

unsigned int setContains(char x[], size_t currentSetSize, char y[])
{
    unsigned int status = 0;
    unsigned int i = 0;

    for (i=0; i < (currentSetSize/2); i++)
    {
        if((x[i+i] == y[9]) && (x[i+i+1] == y[10]))
        {
            status = 1;
        }
    }
    return status;
}

dohave check_exists(char c[])
{
    if ((((c[0] == 'F' || c[0] == 'R' || c[0] == 'O' || c[0] == 'K')
            && (isdigit(c[1]) || islower(c[1])))
            && (c[2] == '-')
            && (c[3] == '>'))
            && ((c[4] == 'F' || c[4] == 'R' || c[4] == 'O' || c[4] == 'K')
                && (isdigit(c[5]) || islower(c[5]))))
    {
        exists.haveI = 1;
    }
    else if ((((c[0] == 'F' || c[0] == 'R' || c[0] == 'O' || c[0] == 'K')
               && (isdigit(c[1]) || islower(c[1])))
              && (c[2] == '-')
              && (c[3] == '>'))
             && ((c[4] == 'A')
                 && (c[5] == 'L')
                 && (c[6] == 'L')))
    {
        exists.haveA = 1;
    }
    else if ((((c[0] == 'F' || c[0] == 'R' || c[0] == 'O' || c[0] == 'K')
               && (isdigit(c[1]) || islower(c[1])))
              && (c[2] == '-')
              && (c[3] == '>'))
             && ((c[4] == 'F')
                 && (c[5] == 'E')
                 && (c[6] == 'D')))
    {
        exists.haveF = 1;
    }
    else if ((((c[0] == 'F' || c[0] == 'R' || c[0] == 'O' || c[0] == 'K')
               && (isdigit(c[1]) || islower(c[1])))
              && (c[2] == '-')
              && (c[3] == '>'))
             && ((c[4] == 'R')
                 && (c[5] == 'O')
                 && (c[6] == 'M')))
    {
        exists.haveR = 1;
    }
    else if ((((c[0] == 'F' || c[0] == 'R' || c[0] == 'O' || c[0] == 'K')
               && (isdigit(c[1]) || islower(c[1])))
              && (c[2] == '-')
              && (c[3] == '>'))
             && ((c[4] == 'O')
                 && (c[5] == 'R')
                 && (c[6] == 'I')))
    {
        exists.haveO = 1;
    }
    else if ((((c[0] == 'F' || c[0] == 'R' || c[0] == 'O' || c[0] == 'K')
               && (isdigit(c[1]) || islower(c[1])))
              && (c[2] == '-')
              && (c[3] == '>'))
             && ((c[4] == 'K')
                 && (c[5] == 'L')
                 && (c[6] == 'I')))
    {
        exists.haveK = 1;
    }
    else if ((((c[0] == 'F' || c[0] == 'R' || c[0] == 'O' || c[0] == 'K')
               && (isdigit(c[1]) || islower(c[1])))
              && (c[2] == '-')
              && (c[3] == '>'))
             && ((c[4] == 'F') && (c[5] == 'E') && (c[6] == 'D'))
             || ((c[4] == 'R') && (c[5] == 'O') && (c[6] == 'M'))
             || ((c[4] == 'O') && (c[5] == 'R') && (c[6] == 'I'))
             || ((c[4] == 'K') && (c[5] == 'L') && (c[6] == 'I')))
    {
        exists.haveT = 1;
    }

    return exists;
}

unsigned int return_tokens(char *p)
{

    unsigned int numtokens = {0};
    size_t length = {0};
    size_t i = {0};
    length = strlen(p);

    if (!isdigit(p[0]))
    {
        numtokens++;
    }
    for (i=0; i < length; i++)
    {
        if ((!isblank(p[i]) && isblank(p[i+1])) || (!isblank(p[i]) && (p[i+1] = '\0')))
        {
            numtokens++;
        }
    }
    return numtokens;
}

unsigned int process_tokens(char c[])
{
    size_t numchars = {0};
    unsigned int linestatus = {0};

    numchars = strlen(c);

    if ((((c[0] == 'F' || c[0] == 'R' || c[0] == 'O' || c[0] == 'K')
            && (isdigit(c[1]) || islower(c[1])))
            && (c[2] == '-')
            && (c[3] == '>'))
            && ((c[4] == 'F' || c[4] == 'R' || c[4] == 'O' || c[4] == 'K')
                && (isdigit(c[5]) || islower(c[5]))
                || ((c[4] == 'A') && (c[5] == 'L') && (c[6] == 'L'))
                || ((c[4] == 'F') && (c[5] == 'E') && (c[6] == 'D'))
                || ((c[4] == 'R') && (c[5] == 'O') && (c[6] == 'M'))
                || ((c[4] == 'O') && (c[5] == 'R') && (c[6] == 'I'))
                || ((c[4] == 'K') && (c[5] == 'L') && (c[6] == 'I'))))
    {
        linestatus = 1;
    }

    return linestatus;
}

void render(char *c, char *t)
{
    size_t length = {0};
    length = strlen(c);

    char newarray[MAXLINE] = {0};
    char newarrayF[MAXLINE] = {0};
    char newarrayR[MAXLINE] = {0};
    char newarrayO[MAXLINE] = {0};
    char newarrayK[MAXLINE] = {0};
    char newarrayA[MAXLINE] = {0};

    size_t length2 = {0};
    size_t j = {0};
    size_t o = {0};

    length2 = strlen(c);

    unsigned int count = {0};
    unsigned int countA = {0};
    unsigned int countF = {0};
    unsigned int countR = {0};
    unsigned int countO = {0};
    unsigned int countK = {0};

    for (j=0; j < length2; j++)
    {
        if ((isblank(c[j]) && ((c[j+1] == 'F' || c[j+1] == 'R' || c[j+1] == 'O' || c[j+1] == 'K')
                               && (isdigit(c[j+2]) || islower(c[j+2]))
                               && (c[j+3] == '-')
                               && (c[j+4] == '>'))
                && ((c[j+5] == 'F' || c[j+5] == 'R' || c[j+5] == 'O' || c[j+5] == 'K')
                    && (isdigit(c[j+6]) || islower(c[j+6])))))
        {
            count++;
            newarray[0] = c[j+1];
            newarray[1] = c[j+2];
            newarray[2] = c[j+3];
            newarray[3] = c[j+4];
            newarray[4] = c[j+5];
            newarray[5] = c[j+6];

            for (o = 6; o < (length2 - j); o++)
            {
                newarray[o] = c[j+o+1];
            }

            if (((newarray[0] == 'F' || newarray[0] == 'R' || newarray[0] == 'O' || newarray[0] == 'K')
                    && (isdigit(newarray[1]) || islower(newarray[1]))
                    && (newarray[2] == '-')
                    && (newarray[3] == '>'))
                    && ((newarray[4] == 'F' || newarray[4] == 'R' || newarray[4] == 'O' || newarray[4] == 'K')
                        && (isdigit(newarray[5]) || islower(newarray[5]))))
            {

                char p[8] = {0};

                p[0] = newarray[0];
                p[1] = newarray[1];

                FILE *f;
                FILE *fpinz;

                p[2] = '.';
                p[3] = 'h';

                p[4] = 't';
                p[5] = 'm';
                p[6] = 'l';
                p[7] = '\0';

                size_t numchars = {0};
                numchars = strlen(p);

                fpinz = fopen(t, "a+");
                f = fopen(p, "a+");

                fprintf(f,"%s\n",newarray);
                fprintf(f,"<br>\n");
                fclose(f);

                fprintf(fpinz,"<a href=\"%s\">%s</a>",p,p);
                fclose(fpinz);
            }
        }
        else if ((isblank(c[j]) && ((c[j+1] == 'F' || c[j+1] == 'R' || c[j+1] == 'O' || c[j+1] == 'K')
                                    && (isdigit(c[j+2]) || islower(c[j+2]))
                                    && (c[j+3] == '-')
                                    && (c[j+4] == '>'))
                  && ((c[j+5] == 'A')
                      && (c[j+6] == 'L')
                      && (c[j+7] == 'L'))))
        {
            countA++;
            newarrayA[0] = c[j+1];
            newarrayA[1] = c[j+2];
            newarrayA[2] = c[j+3];
            newarrayA[3] = c[j+4];
            newarrayA[4] = c[j+5];
            newarrayA[5] = c[j+6];
            newarrayA[6] = c[j+7];

            for (o = 7; o < (length2 - j); o++)
            {
                newarrayA[o] = c[j+o+1];
            }

            if (((newarrayA[0] == 'F' || newarrayA[0] == 'R' || newarrayA[0] == 'O' || newarrayA[0] == 'K')
                    && (isdigit(newarrayA[1]) || islower(newarrayA[1]))
                    && (newarrayA[2] == '-')
                    && (newarrayA[3] == '>'))
                    && ((newarrayA[4] == 'A')
                        && (newarrayA[5] == 'L')
                        && (newarrayA[6] == 'L')))
            {
                FILE *f;
                f = fopen("ALL.html", "a+");
                fprintf(f,"%s\n",newarrayA);
                fprintf(f,"<br>\n");
                fclose(f);

            }
        }
        else if ((isblank(c[j]) && ((c[j+1] == 'F' || c[j+1] == 'R' || c[j+1] == 'O' || c[j+1] == 'K')
                                    && (isdigit(c[j+2]) || islower(c[j+2]))
                                    && (c[j+3] == '-')
                                    && (c[j+4] == '>'))
                  && ((c[j+5] == 'F')
                      && (c[j+6] == 'E')
                      && (c[j+7] == 'D'))))
        {
            countF++;
            newarrayF[0] = c[j+1];
            newarrayF[1] = c[j+2];
            newarrayF[2] = c[j+3];
            newarrayF[3] = c[j+4];
            newarrayF[4] = c[j+5];
            newarrayF[5] = c[j+6];
            newarrayF[6] = c[j+7];

            for (o = 7; o < (length2 - j); o++)
            {
                newarrayF[o] = c[j+o+1];
            }

            if (((newarrayF[0] == 'F' || newarrayF[0] == 'R' || newarrayF[0] == 'O' || newarrayF[0] == 'K')
                    && (isdigit(newarrayF[1]) || islower(newarrayF[1]))
                    && (newarrayF[2] == '-')
                    && (newarrayF[3] == '>'))
                    && ((newarrayF[4] == 'F')
                        && (newarrayF[5] == 'E')
                        && (newarrayF[6] == 'D')))
            {
                FILE *f;
                f = fopen("FED.html", "a+");
                fprintf(f,"%s\n",newarrayF);
                fprintf(f,"<br>\n");
                fclose(f);
            }
        }
        else if ((isblank(c[j]) && ((c[j+1] == 'F' || c[j+1] == 'R' || c[j+1] == 'O' || c[j+1] == 'K')
                                    && (isdigit(c[j+2]) || islower(c[j+2]))
                                    && (c[j+3] == '-')
                                    && (c[j+4] == '>'))
                  && ((c[j+5] == 'R')
                      && (c[j+6] == 'O')
                      && (c[j+7] == 'M'))))
        {
            countR++;
            newarrayR[0] = c[j+1];
            newarrayR[1] = c[j+2];
            newarrayR[2] = c[j+3];
            newarrayR[3] = c[j+4];
            newarrayR[4] = c[j+5];
            newarrayR[5] = c[j+6];
            newarrayR[6] = c[j+7];

            for (o = 7; o < (length2 - j); o++)
            {
                newarrayR[o] = c[j+o+1];
            }

            if (((newarrayR[0] == 'F' || newarrayR[0] == 'R' || newarrayR[0] == 'O' || newarrayR[0] == 'K')
                    && (isdigit(newarrayR[1]) || islower(newarrayR[1]))
                    && (newarrayR[2] == '-')
                    && (newarrayR[3] == '>'))
                    && ((newarrayR[4] == 'R')
                        && (newarrayR[5] == 'O')
                        && (newarrayR[6] == 'M')))
            {
                FILE *f;
                f = fopen("ROM.html", "a+");
                fprintf(f,"%s\n",newarrayR);
                fprintf(f,"<br>\n");
                fclose(f);
            }
        }
        else if ((isblank(c[j]) && ((c[j+1] == 'F' || c[j+1] == 'R' || c[j+1] == 'O' || c[j+1] == 'K')
                                    && (isdigit(c[j+2]) || islower(c[j+2]))
                                    && (c[j+3] == '-')
                                    && (c[j+4] == '>'))
                  && ((c[j+5] == 'O')
                      && (c[j+6] == 'R')
                      && (c[j+7] == 'I'))))
        {
            countO++;
            newarrayO[0] = c[j+1];
            newarrayO[1] = c[j+2];
            newarrayO[2] = c[j+3];
            newarrayO[3] = c[j+4];
            newarrayO[4] = c[j+5];
            newarrayO[5] = c[j+6];
            newarrayO[6] = c[j+7];

            for (o = 7; o < (length2 - j); o++)
            {
                newarrayO[o] = c[j+o+1];
            }

            if (((newarrayO[0] == 'F' || newarrayO[0] == 'R' || newarrayO[0] == 'O' || newarrayO[0] == 'K')
                    && (isdigit(newarrayO[1]) || islower(newarrayO[1]))
                    && (newarrayO[2] == '-')
                    && (newarrayO[3] == '>'))
                    && ((newarrayO[4] == 'O')
                        && (newarrayO[5] == 'R')
                        && (newarrayO[6] == 'I')))
            {
                FILE *f;
                f = fopen("ORI.html", "a+");
                fprintf(f,"%s\n",newarrayO);
                fprintf(f,"<br>\n");
                fclose(f);
            }
        }
        else if ((isblank(c[j]) && ((c[j+1] == 'F' || c[j+1] == 'R' || c[j+1] == 'O' || c[j+1] == 'K')
                                    && (isdigit(c[j+2]) || islower(c[j+2]))
                                    && (c[j+3] == '-')
                                    && (c[j+4] == '>'))
                  && ((c[j+5] == 'K')
                      && (c[j+6] == 'L')
                      && (c[j+7] == 'I'))))
        {
            countK++;
            newarrayK[0] = c[j+1];
            newarrayK[1] = c[j+2];
            newarrayK[2] = c[j+3];
            newarrayK[3] = c[j+4];
            newarrayK[4] = c[j+5];
            newarrayK[5] = c[j+6];
            newarrayK[6] = c[j+7];

            for (o = 7; o < (length2 - j); o++)
            {
                newarrayK[o] = c[j+o+1];
            }

            if (((newarrayK[0] == 'F' || newarrayK[0] == 'R' || newarrayK[0] == 'O' || newarrayK[0] == 'K')
                    && (isdigit(newarrayK[1]) || islower(newarrayK[1]))
                    && (newarrayK[2] == '-')
                    && (newarrayK[3] == '>'))
                    && ((newarrayK[4] == 'K')
                        && (newarrayK[5] == 'L')
                        && (newarrayK[6] == 'I')))
            {
                FILE *f;
                f = fopen("KLI.html", "a+");
                fprintf(f,"%s\n",newarrayK);
                fprintf(f,"<br>\n");
                fclose(f);
            }
        }
    }
}


void renderq(char *c, char *t)
{
    size_t length = {0};
    length = strlen(c);

    char newarray[MAXLINE] = {0};

    size_t length2 = {0};
    size_t j = {0};
    size_t o = {0};

    length2 = strlen(c);

    unsigned int count = {0};

    for (j=0; j < length2; j++)
    {
        if ((isblank(c[j]) && ((c[j+1] == 'F' || c[j+1] == 'R' || c[j+1] == 'O' || c[j+1] == 'K')
                               && (isdigit(c[j+2]) || islower(c[j+2]))
                               && (c[j+3] == '-')
                               && (c[j+4] == '>'))
                && ((c[j+5] == 'F' || c[j+5] == 'R' || c[j+5] == 'O' || c[j+5] == 'K')
                    && (isdigit(c[j+6]) || islower(c[j+6])))))
        {
            count++;
            newarray[0] = c[j+1];
            newarray[1] = c[j+2];
            newarray[2] = c[j+3];
            newarray[3] = c[j+4];
            newarray[4] = c[j+5];
            newarray[5] = c[j+6];

            for (o = 6; o < (length2 - j); o++)
            {
                newarray[o] = c[j+o+1];
            }

            if (((newarray[0] == 'F' || newarray[0] == 'R' || newarray[0] == 'O' || newarray[0] == 'K')
                    && (isdigit(newarray[1]) || islower(newarray[1]))
                    && (newarray[2] == '-')
                    && (newarray[3] == '>'))
                    && ((newarray[4] == 'F' || newarray[4] == 'R' || newarray[4] == 'O' || newarray[4] == 'K')
                        && (isdigit(newarray[5]) || islower(newarray[5]))))
            {

                char p[8] = {0};

                p[0] = newarray[0];
                p[1] = newarray[1];

                FILE *fpinz;

                p[2] = '.';
                p[3] = 'h';

                p[4] = 't';
                p[5] = 'm';
                p[6] = 'l';
                p[7] = '\0';

                size_t numchars = {0};
                numchars = strlen(p);

                fpinz = fopen(t, "a+");
                fprintf(fpinz,"<a href=\"%s\">%s</a>",p,p);
                fclose(fpinz);
            }
        }
    }
}


void prephtml(char *z)
{

    FILE *fpin;
    fpin = fopen(z, "w+");

    fprintf(fpin,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(fpin,"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\"\n");
    fprintf(fpin,"                                    \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"<html\n");
    fprintf(fpin,"      xmlns=\"http://www.w3.org/1999/xhtml\"\n");
    fprintf(fpin,"      xml:lang=\"en-US\"\n");
    fprintf(fpin,"      lang=\"en-US\">\n");
    fprintf(fpin,"<head profile=\"http://www.w3.org/2005/10/profile\">\n");
    fprintf(fpin,"<link rel=\"icon\"\n");
    fprintf(fpin,"            type=\"image/ico\"\n");
    fprintf(fpin,"                  href=\"images/gytha-favicon.ico\" />\n");
    fprintf(fpin,"<meta http-equiv=\"Content-Type\" content=\"text/html;\" charset=\"utf-8\" />\n");
    fprintf(fpin,"<meta name=\"copyright\" content=\"Copyright 2010 oggthebase.org\" />\n");
    fprintf(fpin,"<meta name=\"robots\" content=\"index,follow\" />\n");
    fprintf(fpin,"<meta name=\"keywords\" content=\"gytha, netrek, client, documentation\" />\n");
    fprintf(fpin,"<meta name=\"description\" content=\"Settup up gytha dev environment\" />\n");
    fprintf(fpin,"<meta name=\"generator\" content=\"Coded in Emacs\" />\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"<title>ZLAN</title>\n");
    fprintf(fpin,"<style type=\"text/css\">\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"body {\n");
    fprintf(fpin,"    font: normal 11px auto \"Trebuchet MS\", Verdana, Arial, Helvetica, sans-serif;\n");
    fprintf(fpin,"    color: #4f6b72;\n");
    fprintf(fpin,"    background: #E6EAE9;\n");
    fprintf(fpin,"}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"a {\n");
    fprintf(fpin,"    color: #c75f3e;\n");
    fprintf(fpin,"  }\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"#mytable {\n");
    fprintf(fpin,"    width: 700px;\n");
    fprintf(fpin,"    padding: 0;\n");
    fprintf(fpin,"    margin: 0;\n");
    fprintf(fpin,"    border-collapse: collapse;\n");
    fprintf(fpin,"}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"caption {\n");
    fprintf(fpin,"    padding: 0 0 5px 0;\n");
    fprintf(fpin,"    width: 700px;\n");
    fprintf(fpin,"    font: italic 11px \"Trebuchet MS\", Verdana, Arial, Helvetica, sans-serif;\n");
    fprintf(fpin,"    text-align: right;\n");
    fprintf(fpin,"    font-style: italic;\n");
    fprintf(fpin,"}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"th.nobg {\n");
    fprintf(fpin,"border-top: 0;\n");
    fprintf(fpin,"border-left: 0;\n");
    fprintf(fpin,"border-right: 1px solid #C1DAD7;\n");
    fprintf(fpin,"background: none;\n");
    fprintf(fpin,"}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"th.spec {\n");
    fprintf(fpin,"border-left: 1px solid #C1DAD7;\n");
    fprintf(fpin,"border-top: 0;\n");
    fprintf(fpin,"background: #fff no-repeat;\n");
    fprintf(fpin,"font: bold 10px \"Trebuchet MS\", Verdana, Arial, Helvetica,\n");
    fprintf(fpin,"sans-serif;\n");
    fprintf(fpin,"color: red;\n");
    fprintf(fpin,"}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"td.spec {\n");
    fprintf(fpin,"border-left: 1px solid #C1DAD7;\n");
    fprintf(fpin,"border-top: 0;\n");
    fprintf(fpin,"background: #fff no-repeat;\n");
    fprintf(fpin,"font: bold 10px \"Trebuchet MS\", Verdana, Arial, Helvetica,\n");
    fprintf(fpin,"sans-serif;\n");
    fprintf(fpin,"}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"</style>\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"<body>\n");
    fprintf(fpin,"  \n");
    fprintf(fpin,"<table id=\"mytable\" cellspacing=\"0\" summary=\"Indidivual board messages\">\n");
    fprintf(fpin,"  <caption>Individual board messages </caption>\n");
    fprintf(fpin,"  <tr>\n");
    fprintf(fpin,"            <th scope=\"row\" class=\"spec\">Players</th>\n");
    fprintf(fpin,"                                    <td class=\"spec\">");

    fclose(fpin);
}

void finishhtml(char *z)
{

    FILE *fpin;
    fpin = fopen(z, "a+");

    fprintf(fpin,"</td>\n");
    fprintf(fpin," </tr>\n");
    fprintf(fpin,"</table>\n");
    fprintf(fpin,"&nbsp;\n");
    fprintf(fpin,"&nbsp;\n");
    fprintf(fpin,"&nbsp;\n");
    fprintf(fpin,"&nbsp;\n");
    fprintf(fpin,"<table id=\"mytable\" cellspacing=\"0\" summary=\"Team board messages\">\n");
    fprintf(fpin,"  <caption>Team board messages </caption>\n");
    fprintf(fpin,"            &nbsp;<tr>\n");
    fprintf(fpin,"                                <th scope=\"row\" class=\"spec\">Team board messages</th>\n");
    fprintf(fpin,"                                                              <td class=\"spec\"><a href=\"FED.html\">FED</a>&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"ROM.html\">ROM</a>&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"ORI.html\">ORI</a>&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"KLI.html\">KLI  </a></td>\n");
    fprintf(fpin,"                                                                                                   </tr>\n");
    fprintf(fpin,"</table>\n");
    fprintf(fpin,"&nbsp;\n");
    fprintf(fpin,"&nbsp;\n");
    fprintf(fpin,"&nbsp;\n");
    fprintf(fpin,"&nbsp;\n");
    fprintf(fpin,"<table id=\"mytable\" cellspacing=\"0\" summary=\"ALL board messages\">\n");
    fprintf(fpin,"  <caption>ALL board messages </caption>\n");
    fprintf(fpin,"            <tr>\n");
    fprintf(fpin,"                                <th scope=\"row\" class=\"spec\">ALL board messages</th>\n");
    fprintf(fpin,"                                                              <td class=\"spec\"><a href=\"ALL.html\">ALL</a></td>\n");
    fprintf(fpin,"                                                                                                   </tr>\n");
    fprintf(fpin,"</table>\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"</body>\n");
    fprintf(fpin,"</html>\n");
    fprintf(fpin,"\n");

    fclose(fpin);
}

void prephtmlF(void)
{

    FILE *fpin;
    fpin = fopen("FED.html", "w+");

    fprintf(fpin,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(fpin,"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\"\n");
    fprintf(fpin,"                                    \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"<html\n");
    fprintf(fpin,"      xmlns=\"http://www.w3.org/1999/xhtml\"\n");
    fprintf(fpin,"      xml:lang=\"en-US\"\n");
    fprintf(fpin,"      lang=\"en-US\">\n");
    fprintf(fpin,"<head profile=\"http://www.w3.org/2005/10/profile\">\n");
    fprintf(fpin,"<link rel=\"icon\"\n");
    fprintf(fpin,"            type=\"image/ico\"\n");
    fprintf(fpin,"                  href=\"images/gytha-favicon.ico\" />\n");
    fprintf(fpin,"<meta http-equiv=\"Content-Type\" content=\"text/html;\" charset=\"utf-8\" />\n");
    fprintf(fpin,"<meta name=\"copyright\" content=\"Copyright 2010 oggthebase.org\" />\n");
    fprintf(fpin,"<meta name=\"robots\" content=\"index,follow\" />\n");
    fprintf(fpin,"<meta name=\"keywords\" content=\"gytha, netrek, client, documentation\" />\n");
    fprintf(fpin,"<meta name=\"description\" content=\"Settup up gytha dev environment\" />\n");
    fprintf(fpin,"<meta name=\"generator\" content=\"Coded in Emacs\" />\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"<title>ZLAN</title>\n");
    fprintf(fpin,"<style type=\"text/css\">\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"body {\n");
    fprintf(fpin,"    font: normal 11px auto \"Trebuchet MS\", Verdana, Arial, Helvetica, sans-serif;\n");
    fprintf(fpin,"    color: #4f6b72;\n");
    fprintf(fpin,"    background: #E6EAE9;\n");
    fprintf(fpin,"}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"a {\n");
    fprintf(fpin,"    color: #c75f3e;\n");
    fprintf(fpin,"  }\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"#mytable {\n");
    fprintf(fpin,"    width: 700px;\n");
    fprintf(fpin,"    padding: 0;\n");
    fprintf(fpin,"    margin: 0;\n");
    fprintf(fpin,"    border-collapse: collapse;\n");
    fprintf(fpin,"}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"caption {\n");
    fprintf(fpin,"    padding: 0 0 5px 0;\n");
    fprintf(fpin,"    width: 700px;\n");
    fprintf(fpin,"    font: italic 11px \"Trebuchet MS\", Verdana, Arial, Helvetica, sans-serif;\n");
    fprintf(fpin,"    text-align: right;\n");
    fprintf(fpin,"    font-style: italic;\n");
    fprintf(fpin,"}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"th.nobg {\n");
    fprintf(fpin,"border-top: 0;\n");
    fprintf(fpin,"border-left: 0;\n");
    fprintf(fpin,"border-right: 1px solid #C1DAD7;\n");
    fprintf(fpin,"background: none;\n");
    fprintf(fpin,"}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"th.spec {\n");
    fprintf(fpin,"border-left: 1px solid #C1DAD7;\n");
    fprintf(fpin,"border-top: 0;\n");
    fprintf(fpin,"background: #fff no-repeat;\n");
    fprintf(fpin,"font: bold 10px \"Trebuchet MS\", Verdana, Arial, Helvetica,\n");
    fprintf(fpin,"sans-serif;\n");
    fprintf(fpin,"color: red;\n");
    fprintf(fpin,"}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"td.spec {\n");
    fprintf(fpin,"border-left: 1px solid #C1DAD7;\n");
    fprintf(fpin,"border-top: 0;\n");
    fprintf(fpin,"background: #fff no-repeat;\n");
    fprintf(fpin,"font: bold 10px \"Trebuchet MS\", Verdana, Arial, Helvetica,\n");
    fprintf(fpin,"sans-serif;\n");
    fprintf(fpin,"}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,".linkgroup a {display: block}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"</style>\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"<body>\n");
    fprintf(fpin,"  \n");
    fprintf(fpin,"<table id=\"mytable\" cellspacing=\"0\" summary=\"Federation board messages\">\n");
    fprintf(fpin,"  <caption>Individual board messages </caption>\n");
    fprintf(fpin,"  <tr>\n");
    fprintf(fpin,"            <th scope=\"row\" class=\"spec\">FED</th>\n");
    fprintf(fpin,"                                    <td class=\"spec\">");
    fprintf(fpin,"<div class=\"linkgroup\">");

    fclose(fpin);
}

void prephtmlR(void)
{
    FILE *fpin;
    fpin = fopen("ROM.html", "w+");

    fprintf(fpin,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(fpin,"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\"\n");
    fprintf(fpin,"                                    \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"<html\n");
    fprintf(fpin,"      xmlns=\"http://www.w3.org/1999/xhtml\"\n");
    fprintf(fpin,"      xml:lang=\"en-US\"\n");
    fprintf(fpin,"      lang=\"en-US\">\n");
    fprintf(fpin,"<head profile=\"http://www.w3.org/2005/10/profile\">\n");
    fprintf(fpin,"<link rel=\"icon\"\n");
    fprintf(fpin,"            type=\"image/ico\"\n");
    fprintf(fpin,"                  href=\"images/gytha-favicon.ico\" />\n");
    fprintf(fpin,"<meta http-equiv=\"Content-Type\" content=\"text/html;\" charset=\"utf-8\" />\n");
    fprintf(fpin,"<meta name=\"copyright\" content=\"Copyright 2010 oggthebase.org\" />\n");
    fprintf(fpin,"<meta name=\"robots\" content=\"index,follow\" />\n");
    fprintf(fpin,"<meta name=\"keywords\" content=\"gytha, netrek, client, documentation\" />\n");
    fprintf(fpin,"<meta name=\"description\" content=\"Settup up gytha dev environment\" />\n");
    fprintf(fpin,"<meta name=\"generator\" content=\"Coded in Emacs\" />\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"<title>ZLAN</title>\n");
    fprintf(fpin,"<style type=\"text/css\">\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"body {\n");
    fprintf(fpin,"    font: normal 11px auto \"Trebuchet MS\", Verdana, Arial, Helvetica, sans-serif;\n");
    fprintf(fpin,"    color: #4f6b72;\n");
    fprintf(fpin,"    background: #E6EAE9;\n");
    fprintf(fpin,"}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"a {\n");
    fprintf(fpin,"    color: #c75f3e;\n");
    fprintf(fpin,"  }\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"#mytable {\n");
    fprintf(fpin,"    width: 700px;\n");
    fprintf(fpin,"    padding: 0;\n");
    fprintf(fpin,"    margin: 0;\n");
    fprintf(fpin,"    border-collapse: collapse;\n");
    fprintf(fpin,"}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"caption {\n");
    fprintf(fpin,"    padding: 0 0 5px 0;\n");
    fprintf(fpin,"    width: 700px;\n");
    fprintf(fpin,"    font: italic 11px \"Trebuchet MS\", Verdana, Arial, Helvetica, sans-serif;\n");
    fprintf(fpin,"    text-align: right;\n");
    fprintf(fpin,"    font-style: italic;\n");
    fprintf(fpin,"}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"th.nobg {\n");
    fprintf(fpin,"border-top: 0;\n");
    fprintf(fpin,"border-left: 0;\n");
    fprintf(fpin,"border-right: 1px solid #C1DAD7;\n");
    fprintf(fpin,"background: none;\n");
    fprintf(fpin,"}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"th.spec {\n");
    fprintf(fpin,"border-left: 1px solid #C1DAD7;\n");
    fprintf(fpin,"border-top: 0;\n");
    fprintf(fpin,"background: #fff no-repeat;\n");
    fprintf(fpin,"font: bold 10px \"Trebuchet MS\", Verdana, Arial, Helvetica,\n");
    fprintf(fpin,"sans-serif;\n");
    fprintf(fpin,"color: red;\n");
    fprintf(fpin,"}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"td.spec {\n");
    fprintf(fpin,"border-left: 1px solid #C1DAD7;\n");
    fprintf(fpin,"border-top: 0;\n");
    fprintf(fpin,"background: #fff no-repeat;\n");
    fprintf(fpin,"font: bold 10px \"Trebuchet MS\", Verdana, Arial, Helvetica,\n");
    fprintf(fpin,"sans-serif;\n");
    fprintf(fpin,"}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,".linkgroup a {display: block}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"</style>\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"<body>\n");
    fprintf(fpin,"  \n");
    fprintf(fpin,"<table id=\"mytable\" cellspacing=\"0\" summary=\"Romulan board messages\">\n");
    fprintf(fpin,"  <caption>Individual board messages </caption>\n");
    fprintf(fpin,"  <tr>\n");
    fprintf(fpin,"            <th scope=\"row\" class=\"spec\">ROM</th>\n");
    fprintf(fpin,"                                    <td class=\"spec\">");
    fprintf(fpin,"<div class=\"linkgroup\">");

    fclose(fpin);
}

void prephtmlO(void)
{
    FILE *fpin;
    fpin = fopen("ORI.html", "w+");

    fprintf(fpin,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(fpin,"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\"\n");
    fprintf(fpin,"                                    \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"<html\n");
    fprintf(fpin,"      xmlns=\"http://www.w3.org/1999/xhtml\"\n");
    fprintf(fpin,"      xml:lang=\"en-US\"\n");
    fprintf(fpin,"      lang=\"en-US\">\n");
    fprintf(fpin,"<head profile=\"http://www.w3.org/2005/10/profile\">\n");
    fprintf(fpin,"<link rel=\"icon\"\n");
    fprintf(fpin,"            type=\"image/ico\"\n");
    fprintf(fpin,"                  href=\"images/gytha-favicon.ico\" />\n");
    fprintf(fpin,"<meta http-equiv=\"Content-Type\" content=\"text/html;\" charset=\"utf-8\" />\n");
    fprintf(fpin,"<meta name=\"copyright\" content=\"Copyright 2010 oggthebase.org\" />\n");
    fprintf(fpin,"<meta name=\"robots\" content=\"index,follow\" />\n");
    fprintf(fpin,"<meta name=\"keywords\" content=\"gytha, netrek, client, documentation\" />\n");
    fprintf(fpin,"<meta name=\"description\" content=\"Settup up gytha dev environment\" />\n");
    fprintf(fpin,"<meta name=\"generator\" content=\"Coded in Emacs\" />\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"<title>ZLAN</title>\n");
    fprintf(fpin,"<style type=\"text/css\">\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"body {\n");
    fprintf(fpin,"    font: normal 11px auto \"Trebuchet MS\", Verdana, Arial, Helvetica, sans-serif;\n");
    fprintf(fpin,"    color: #4f6b72;\n");
    fprintf(fpin,"    background: #E6EAE9;\n");
    fprintf(fpin,"}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"a {\n");
    fprintf(fpin,"    color: #c75f3e;\n");
    fprintf(fpin,"  }\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"#mytable {\n");
    fprintf(fpin,"    width: 700px;\n");
    fprintf(fpin,"    padding: 0;\n");
    fprintf(fpin,"    margin: 0;\n");
    fprintf(fpin,"    border-collapse: collapse;\n");
    fprintf(fpin,"}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"caption {\n");
    fprintf(fpin,"    padding: 0 0 5px 0;\n");
    fprintf(fpin,"    width: 700px;\n");
    fprintf(fpin,"    font: italic 11px \"Trebuchet MS\", Verdana, Arial, Helvetica, sans-serif;\n");
    fprintf(fpin,"    text-align: right;\n");
    fprintf(fpin,"    font-style: italic;\n");
    fprintf(fpin,"}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"th.nobg {\n");
    fprintf(fpin,"border-top: 0;\n");
    fprintf(fpin,"border-left: 0;\n");
    fprintf(fpin,"border-right: 1px solid #C1DAD7;\n");
    fprintf(fpin,"background: none;\n");
    fprintf(fpin,"}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"th.spec {\n");
    fprintf(fpin,"border-left: 1px solid #C1DAD7;\n");
    fprintf(fpin,"border-top: 0;\n");
    fprintf(fpin,"background: #fff no-repeat;\n");
    fprintf(fpin,"font: bold 10px \"Trebuchet MS\", Verdana, Arial, Helvetica,\n");
    fprintf(fpin,"sans-serif;\n");
    fprintf(fpin,"color: red;\n");
    fprintf(fpin,"}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"td.spec {\n");
    fprintf(fpin,"border-left: 1px solid #C1DAD7;\n");
    fprintf(fpin,"border-top: 0;\n");
    fprintf(fpin,"background: #fff no-repeat;\n");
    fprintf(fpin,"font: bold 10px \"Trebuchet MS\", Verdana, Arial, Helvetica,\n");
    fprintf(fpin,"sans-serif;\n");
    fprintf(fpin,"}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,".linkgroup a {display: block}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"</style>\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"<body>\n");
    fprintf(fpin,"  \n");
    fprintf(fpin,"<table id=\"mytable\" cellspacing=\"0\" summary=\"Orion board messages\">\n");
    fprintf(fpin,"  <caption>Individual board messages </caption>\n");
    fprintf(fpin,"  <tr>\n");
    fprintf(fpin,"            <th scope=\"row\" class=\"spec\">ORI</th>\n");
    fprintf(fpin,"                                    <td class=\"spec\">");
    fprintf(fpin,"<div class=\"linkgroup\">");

    fclose(fpin);
}

void prephtmlK(void)
{
    FILE *fpin;
    fpin = fopen("KLI.html", "w+");

    fprintf(fpin,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(fpin,"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\"\n");
    fprintf(fpin,"                                    \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"<html\n");
    fprintf(fpin,"      xmlns=\"http://www.w3.org/1999/xhtml\"\n");
    fprintf(fpin,"      xml:lang=\"en-US\"\n");
    fprintf(fpin,"      lang=\"en-US\">\n");
    fprintf(fpin,"<head profile=\"http://www.w3.org/2005/10/profile\">\n");
    fprintf(fpin,"<link rel=\"icon\"\n");
    fprintf(fpin,"            type=\"image/ico\"\n");
    fprintf(fpin,"                  href=\"images/gytha-favicon.ico\" />\n");
    fprintf(fpin,"<meta http-equiv=\"Content-Type\" content=\"text/html;\" charset=\"utf-8\" />\n");
    fprintf(fpin,"<meta name=\"copyright\" content=\"Copyright 2010 oggthebase.org\" />\n");
    fprintf(fpin,"<meta name=\"robots\" content=\"index,follow\" />\n");
    fprintf(fpin,"<meta name=\"keywords\" content=\"gytha, netrek, client, documentation\" />\n");
    fprintf(fpin,"<meta name=\"description\" content=\"Settup up gytha dev environment\" />\n");
    fprintf(fpin,"<meta name=\"generator\" content=\"Coded in Emacs\" />\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"<title>ZLAN</title>\n");
    fprintf(fpin,"<style type=\"text/css\">\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"body {\n");
    fprintf(fpin,"    font: normal 11px auto \"Trebuchet MS\", Verdana, Arial, Helvetica, sans-serif;\n");
    fprintf(fpin,"    color: #4f6b72;\n");
    fprintf(fpin,"    background: #E6EAE9;\n");
    fprintf(fpin,"}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"a {\n");
    fprintf(fpin,"    color: #c75f3e;\n");
    fprintf(fpin,"  }\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"#mytable {\n");
    fprintf(fpin,"    width: 700px;\n");
    fprintf(fpin,"    padding: 0;\n");
    fprintf(fpin,"    margin: 0;\n");
    fprintf(fpin,"    border-collapse: collapse;\n");
    fprintf(fpin,"}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"caption {\n");
    fprintf(fpin,"    padding: 0 0 5px 0;\n");
    fprintf(fpin,"    width: 700px;\n");
    fprintf(fpin,"    font: italic 11px \"Trebuchet MS\", Verdana, Arial, Helvetica, sans-serif;\n");
    fprintf(fpin,"    text-align: right;\n");
    fprintf(fpin,"    font-style: italic;\n");
    fprintf(fpin,"}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"th.nobg {\n");
    fprintf(fpin,"border-top: 0;\n");
    fprintf(fpin,"border-left: 0;\n");
    fprintf(fpin,"border-right: 1px solid #C1DAD7;\n");
    fprintf(fpin,"background: none;\n");
    fprintf(fpin,"}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"th.spec {\n");
    fprintf(fpin,"border-left: 1px solid #C1DAD7;\n");
    fprintf(fpin,"border-top: 0;\n");
    fprintf(fpin,"background: #fff no-repeat;\n");
    fprintf(fpin,"font: bold 10px \"Trebuchet MS\", Verdana, Arial, Helvetica,\n");
    fprintf(fpin,"sans-serif;\n");
    fprintf(fpin,"color: red;\n");
    fprintf(fpin,"}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"td.spec {\n");
    fprintf(fpin,"border-left: 1px solid #C1DAD7;\n");
    fprintf(fpin,"border-top: 0;\n");
    fprintf(fpin,"background: #fff no-repeat;\n");
    fprintf(fpin,"font: bold 10px \"Trebuchet MS\", Verdana, Arial, Helvetica,\n");
    fprintf(fpin,"sans-serif;\n");
    fprintf(fpin,"}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,".linkgroup a {display: block}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"</style>\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"<body>\n");
    fprintf(fpin,"  \n");
    fprintf(fpin,"<table id=\"mytable\" cellspacing=\"0\" summary=\"Klingon board messages\">\n");
    fprintf(fpin,"  <caption>Individual board messages </caption>\n");
    fprintf(fpin,"  <tr>\n");
    fprintf(fpin,"            <th scope=\"row\" class=\"spec\">KLI</th>\n");
    fprintf(fpin,"                                    <td class=\"spec\">");
    fprintf(fpin,"<div class=\"linkgroup\">");
    fclose(fpin);
}

void prephtmlA(void)
{

    FILE *fpin;
    fpin = fopen("ALL.html", "w+");

    fprintf(fpin,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(fpin,"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\"\n");
    fprintf(fpin,"                                    \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"<html\n");
    fprintf(fpin,"      xmlns=\"http://www.w3.org/1999/xhtml\"\n");
    fprintf(fpin,"      xml:lang=\"en-US\"\n");
    fprintf(fpin,"      lang=\"en-US\">\n");
    fprintf(fpin,"<head profile=\"http://www.w3.org/2005/10/profile\">\n");
    fprintf(fpin,"<link rel=\"icon\"\n");
    fprintf(fpin,"            type=\"image/ico\"\n");
    fprintf(fpin,"                  href=\"images/gytha-favicon.ico\" />\n");
    fprintf(fpin,"<meta http-equiv=\"Content-Type\" content=\"text/html;\" charset=\"utf-8\" />\n");
    fprintf(fpin,"<meta name=\"copyright\" content=\"Copyright 2010 oggthebase.org\" />\n");
    fprintf(fpin,"<meta name=\"robots\" content=\"index,follow\" />\n");
    fprintf(fpin,"<meta name=\"keywords\" content=\"gytha, netrek, client, documentation\" />\n");
    fprintf(fpin,"<meta name=\"description\" content=\"Settup up gytha dev environment\" />\n");
    fprintf(fpin,"<meta name=\"generator\" content=\"Coded in Emacs\" />\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"<title>ZLAN</title>\n");
    fprintf(fpin,"<style type=\"text/css\">\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"body {\n");
    fprintf(fpin,"    font: normal 11px auto \"Trebuchet MS\", Verdana, Arial, Helvetica, sans-serif;\n");
    fprintf(fpin,"    color: #4f6b72;\n");
    fprintf(fpin,"    background: #E6EAE9;\n");
    fprintf(fpin,"}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"a {\n");
    fprintf(fpin,"    color: #c75f3e;\n");
    fprintf(fpin,"  }\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"#mytable {\n");
    fprintf(fpin,"    width: 700px;\n");
    fprintf(fpin,"    padding: 0;\n");
    fprintf(fpin,"    margin: 0;\n");
    fprintf(fpin,"    border-collapse: collapse;\n");
    fprintf(fpin,"}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"caption {\n");
    fprintf(fpin,"    padding: 0 0 5px 0;\n");
    fprintf(fpin,"    width: 700px;\n");
    fprintf(fpin,"    font: italic 11px \"Trebuchet MS\", Verdana, Arial, Helvetica, sans-serif;\n");
    fprintf(fpin,"    text-align: right;\n");
    fprintf(fpin,"    font-style: italic;\n");
    fprintf(fpin,"}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"th.nobg {\n");
    fprintf(fpin,"border-top: 0;\n");
    fprintf(fpin,"border-left: 0;\n");
    fprintf(fpin,"border-right: 1px solid #C1DAD7;\n");
    fprintf(fpin,"background: none;\n");
    fprintf(fpin,"}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"th.spec {\n");
    fprintf(fpin,"border-left: 1px solid #C1DAD7;\n");
    fprintf(fpin,"border-top: 0;\n");
    fprintf(fpin,"background: #fff no-repeat;\n");
    fprintf(fpin,"font: bold 10px \"Trebuchet MS\", Verdana, Arial, Helvetica,\n");
    fprintf(fpin,"sans-serif;\n");
    fprintf(fpin,"color: red;\n");
    fprintf(fpin,"}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"td.spec {\n");
    fprintf(fpin,"border-left: 1px solid #C1DAD7;\n");
    fprintf(fpin,"border-top: 0;\n");
    fprintf(fpin,"background: #fff no-repeat;\n");
    fprintf(fpin,"font: bold 10px \"Trebuchet MS\", Verdana, Arial, Helvetica,\n");
    fprintf(fpin,"sans-serif;\n");
    fprintf(fpin,"}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,".linkgroup a {display: block}\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"</style>\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"<body>\n");
    fprintf(fpin,"  \n");
    fprintf(fpin,"<table id=\"mytable\" cellspacing=\"0\" summary=\"ALL board messages\">\n");
    fprintf(fpin,"  <caption>Individual board messages </caption>\n");
    fprintf(fpin,"  <tr>\n");
    fprintf(fpin,"            <th scope=\"row\" class=\"spec\">ALL</th>\n");
    fprintf(fpin,"                                    <td class=\"spec\">");
    fprintf(fpin,"<div class=\"linkgroup\">");
    fclose(fpin);
}

void finishhtmlF(void)
{

    FILE *fpin;
    fpin = fopen("FED.html", "a+");

    fprintf(fpin,"</div>");
    fprintf(fpin,"</td>\n");
    fprintf(fpin," </tr>\n");
    fprintf(fpin,"</table>\n");
    fprintf(fpin,"&nbsp;\n");
    fprintf(fpin,"&nbsp;\n");
    fprintf(fpin,"&nbsp;\n");
    fprintf(fpin,"&nbsp;\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"</body>\n");
    fprintf(fpin,"</html>\n");
    fprintf(fpin,"\n");

    fclose(fpin);
}

void finishhtmlR(void)
{

    FILE *fpin;
    fpin = fopen("ROM.html", "a+");

    fprintf(fpin,"</div>");
    fprintf(fpin,"</td>\n");
    fprintf(fpin," </tr>\n");
    fprintf(fpin,"</table>\n");
    fprintf(fpin,"&nbsp;\n");
    fprintf(fpin,"&nbsp;\n");
    fprintf(fpin,"&nbsp;\n");
    fprintf(fpin,"&nbsp;\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"</body>\n");
    fprintf(fpin,"</html>\n");
    fprintf(fpin,"\n");

    fclose(fpin);
}

void finishhtmlO(void)
{

    FILE *fpin;
    fpin = fopen("ORI.html", "a+");

    fprintf(fpin,"</div>");
    fprintf(fpin,"</td>\n");
    fprintf(fpin," </tr>\n");
    fprintf(fpin,"</table>\n");
    fprintf(fpin,"&nbsp;\n");
    fprintf(fpin,"&nbsp;\n");
    fprintf(fpin,"&nbsp;\n");
    fprintf(fpin,"&nbsp;\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"</body>\n");
    fprintf(fpin,"</html>\n");
    fprintf(fpin,"\n");

    fclose(fpin);
}

void finishhtmlK(void)
{

    FILE *fpin;
    fpin = fopen("KLI.html", "a+");

    fprintf(fpin,"</div>");
    fprintf(fpin,"</td>\n");
    fprintf(fpin," </tr>\n");
    fprintf(fpin,"</table>\n");
    fprintf(fpin,"&nbsp;\n");
    fprintf(fpin,"&nbsp;\n");
    fprintf(fpin,"&nbsp;\n");
    fprintf(fpin,"&nbsp;\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"</body>\n");
    fprintf(fpin,"</html>\n");
    fprintf(fpin,"\n");

    fclose(fpin);
}

void finishhtmlA(void)
{

    FILE *fpin;
    fpin = fopen("ALL.html", "a+");

    fprintf(fpin,"</div>");
    fprintf(fpin,"</td>\n");
    fprintf(fpin," </tr>\n");
    fprintf(fpin,"</table>\n");
    fprintf(fpin,"&nbsp;\n");
    fprintf(fpin,"&nbsp;\n");
    fprintf(fpin,"&nbsp;\n");
    fprintf(fpin,"&nbsp;\n");
    fprintf(fpin,"\n");
    fprintf(fpin,"</body>\n");
    fprintf(fpin,"</html>\n");
    fprintf(fpin,"\n");

    fclose(fpin);
}
