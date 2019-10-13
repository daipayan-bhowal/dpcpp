/* Written for Embedded C++ */
/* Follow TR.18015 Standard Specs */
/* Daipayan's C preprocessor(dpcpp) version 3.1 */
/* Designed by Daipayan Bhowal */

#include <iostream> 
#include "datastructure.h"
#include <fstream>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <cstdlib>
#include <bits/stdc++.h> 
#define UTX 0
#define FWK 0
#define DEBUG 0
#define PARSE 0
#define LEVEL_INC 1
#define NO_OF_MAX_FILES 5
using namespace std;
#undef stack
bool is_id(string token,string term,const int term_char_option);
/**************  Defination ****************/
enum MacroType {
    define_t,
    include_t,
    line_t,
    undef_t,
    error_tt,
    pragma_t,
    defined_t,
    if_t,
    ifdef_t,
    ifndef_t,
    elif_t,
    else_t,
    endif_t,
    MAX_TYPE
};
MacroType getIndexFromStr(string s)
{
    string tmp;
    string arr[13] = { "define","include","line","undef","error","pragma","defined","if","ifdef","ifndef","elif","else","endif" };
    for(int i=0; i< 13; i++)
    {
        tmp = arr[i];
        if((tmp.compare(s)) == 0)
            return (MacroType) i;
    }
    return MAX_TYPE;
}
/************** Lexer Storage Structure ************/
class InputData
{ 
    public:
        char* line_str;
        int line_no;
        InputData()
        {
#ifdef DEBUG        
            static int alloc = 0;
           cout << "memory allocated:"<<alloc++ <<endl;
#endif
        }
        ~InputData()
        {
#ifdef DEBUG        
            static int dealloc = 0;
           cout << "deallocater called"<<dealloc++  <<endl;
#endif
            //    free(line_str);

        }
        void setValue(string s,int n)
        {
            //        InputData(s,n);
            line_str = (char*)malloc(sizeof(char)*(s.length()+1));
            strcpy(line_str,s.c_str());
            line_str[s.length()] = '\0';
            line_no = n;
        }

};
void RemoveInput(InputData **inp,int cnt)
{
    for(int i=0; i< cnt; i++)
    {
#ifdef DEBUG        
        cout << inp[i]->line_str <<endl;
#endif
        delete inp[i];
    }
}
class Tokens
{
    public:
        string tokens;
        int line_no;
        bool is_macro_keyword;
        MacroType macro_type;
        Tokens(Tokens *t)
        {
            this->tokens = t->tokens;
            this->line_no = t->line_no;
            this->is_macro_keyword = t->is_macro_keyword;
            this->macro_type = t->macro_type;
        }
        Tokens()
        {

        }
        void InsertTokens(int line,string tok,bool is_macro)
        {
            tokens = tok;
            line_no =  line;
            is_macro_keyword = is_macro;
            macro_type = getIndexFromStr(tok);
        }
        void viewTokens()
        {
            cout << line_no << ":"<< tokens <<endl;
        }
};
void RemoveTokens(Tokens *t)
{
    t->tokens = "";
    t->line_no = -1;
    t->is_macro_keyword = -1;
}
class CompilerData
{
    public:
        int line_number;
        string compilation_line;
        CompilerData()
        {

        }
        CompilerData(CompilerData* c)
        {
            this->line_number = c->line_number;
            this->compilation_line = c->compilation_line;
        }
        CompilerData(string l,int num)
        {
            compilation_line = l;
            line_number = num;
        }
        void InsertData(string l,int no)
        {
            compilation_line = l;
            line_number = no;
        }
};
/****************  Operation Strucures *********/
class MacroFunction
{
    int line_no;
    string def_name;
    Stack m;
    string body; 
};
/************** File Handling ****************/
int file_read(string input_file,InputData **input)
{
    string line;
    unsigned int count_lines=0;
    std::ifstream file(input_file.c_str());
    string str_line[100000];
    if (file.is_open())
    {   
        while ( getline (file,line) )
        {
            //  str_line[count_lines] = new char[line.length()];
            //   memcpy(str_line[count_lines], line.c_str(), line.length());
            str_line[count_lines] = line;
#ifdef DEBUG      
            cout <<count_lines << str_line[count_lines] <<endl;
#endif            
            count_lines++;
        }
        file.close();
    }   
    else 
        cout << "Unable to open file";

    InputData *inp = new InputData[count_lines];  // Issue in this line --> should be new InputData[5]
    for(int i=0; i<count_lines; i++)
    {
        // string temp(str_line[i]);
        // cout << temp << endl;
        inp[i].setValue(str_line[i],i+1);
#ifdef DEBUG        
        cout << inp[i].line_str << endl;
#endif        
        //   delete str_line[i];
    }
#ifdef DEBUG     
    cout << "file_read done !" <<endl;
#endif 
    *input = inp;   
    return count_lines;
}
const char* change_file_name(string file_name)
{
    int len = file_name.length() - 1; 
    char *s =(char*) file_name.c_str();
    s[len] = 'i';
    return (const char *)s;
}
void dump_file(string file_name,CompilerData *cd,int no_of_compilerdata,string par_file)
{
    ofstream fout; 
    const char *fl=change_file_name(par_file);

    fout.open (fl,ios::app);
    for(int i=0; i<no_of_compilerdata; i++)
    {
        if(cd[i].compilation_line != "")
            fout << cd[i].compilation_line <<"\n"; 
    }
    fout.close();
}
/************** Parser *****************/

void Parse_Argument(stringstream &strstream,string def_macro,int line_no)
{
    string tok_str;
    Stack st;
    while(getline(strstream, tok_str, ','))
    {
        //   if(is_id(tok_str," ",0))
        //   {
        if(tok_str.at(tok_str.length() - 1) != ')')
        {  
#ifdef DEBUG 
            cout <<tok_str << endl;
#endif
            st.push(tok_str);
        }
        //          Remove_Multiline();

        //  }
        //    else
        //       {    cout <<"error: Macro Parameter error !"<< endl;
        //       exit(0);
        //       }      
    }
#ifdef DEBUG    
    cout << "Parse Argument Token:"<<tok_str<< tok_str.length()<<endl;
#endif    
    stringstream strstream2(tok_str);
    if(getline(strstream2, tok_str, ')'))
    {

        st.push(tok_str);
#ifdef DEBUG        
        cout <<tok_str << endl;
#endif        
    }
    else if(tok_str.length() != 0)
    {
        cout <<"error: \' ) \' expected !"<< endl;
        exit(0);
    }
#ifdef DEBUG 
      cout << tok_str << endl;
#endif
}
bool is_at_end(string &str,string end="\\")
{
    bool flag= false;
    for(int k= str.length() - 1; k >= 0; k--)
    {
        if(str.at(k) == ' ')
        {

        } 
        else if(str.at(k) == '\\')
        {  
                flag = true; 
                str.replace(k-1,2,"");               // Hack for replacing '\' with "" str 
                return flag;

        }
        else 
        {
            flag = false;
            return flag;

        }

    }
    return flag;
}
void Parse_Body(stringstream &strstream,string single_line, int line_no,InputData *inp,int count_lines)
{
    string rep = " ",s;
    bool flag = false;
    // Multi-line macro function

    string orig_str = single_line;
    if(is_at_end(orig_str,"\\") == false)
        return;
    for(int j = line_no + 1 ; j < count_lines; j++)
    {
        string str = inp[j].line_str;
        if(is_at_end(str,"\\") == true)
        {
            orig_str += " " + str;
            str = "";
            free(inp[j].line_str);
            inp[j].line_str = (char*)malloc(sizeof(char)+1);
            strcpy(inp[j].line_str," ");
            inp[j].line_str[1] = '\0';
            // strcpy(inp[j].line_str," ");
        }

    }
    // inp[line_no].line_str = orig_str.c_str();
#if DEBUG
    cout << "Original STring"<< orig_str << endl;
#endif   
}
/********************** is identifier *********************/
/*   Verifies whether the token string is an identifier   */
/**********************************************************/
bool is_id(string token,string term,const int term_char_option)
{
    bool valid = false;
    bool term_by_char = false;
    int len;
    if(isalpha(token.at(0)) || token.at(0) == '_' )
    {  

        if(term_char_option == 0)
        { len = token.length();
            goto IDENTIFIER;
        }
        else
            len = token.length() - 1;  

        for(int i=0; i< token.length(); i++)
        {

            //                cout << term.at(0) << token.at(i) <<endl;
            if(term.at(0) == token.at(i)) 
            {

                term_by_char = true;
                break;
            }

        }
        if(term_by_char == true)
        {    
IDENTIFIER:    
            for(int i=1;  i < len ; i++)
            {
                if(isalpha(token.at(i)) || isdigit(token.at(i)) || token.at(i) == '_')
                {    valid = true;
                }
                else
                {    valid = false;
                    if(term_char_option == 0) break;
                }
            }
        }
    }   
    return valid;
}
bool is_enclosed_bracket_str(string token,char front,char back)
{
    if(token.at(0) == front && token.at(token.length() -1) == back)
    {
        if(token.length() == 2)
            return false;

        return true;
    }

    return false;
}
bool is_num(string token)
{
    string::iterator it = token.begin();
    for(;it != token.end(); it++)
    {
        if(isdigit(*it) == 0)
        {
            return false;
        }
    } 
    return true;
}
int is_space(int i,string expr)
{
    while(i < expr.length())
    {

        if(expr.at(i) == ' ')
            i++;
        else {
            break;
        }

    }
    return i;
}
bool is_macro_operator(string prev_macro,string macro)
{
    if(prev_macro == "")
        return false;

    if(prev_macro == "defined" && (macro == "||" || macro == "&&"))
        return true;
}
/******************* removeMultiline ********************/
/* This function will remove multilins from compilation lines */
/*******************************************************/
void removeMultiline(CompilerData *cd,int line,int line_limit,int &flag)
{
    int rd_line = line;
    int i=0;
    string changed_str ="";
    while(is_at_end(cd[rd_line].compilation_line) == true && rd_line < line_limit)
    {
        flag = line;
        rd_line++;
    }

    if(rd_line == line)
        return;

    for(int j=line; j<rd_line; j++)
    {
        changed_str += " ";
        changed_str += cd[j+1].compilation_line;
        cd[j+1].compilation_line = "";
    }
       cd[line].compilation_line += changed_str;
       cd[rd_line].compilation_line = "";

     //  free(inp_data[rd_line].line_str);

}
/******************* removeComments ********************/
/* This function will remove comments from line string */
/*******************************************************/
void removeComments(string &line_str)
{
   int len =0;
   string CommentStart = "/*";
   string CommentEnd = "*/";
   static bool flag = false;
   int foundCommentStart = line_str.find(CommentStart);
   if(foundCommentStart >= 0 && foundCommentStart < string::npos)
   {
       int foundCommentEnd = line_str.find(CommentEnd);
       if(foundCommentEnd > foundCommentStart && foundCommentEnd != string::npos)
       {
           len = foundCommentEnd - foundCommentStart;
           line_str.replace(foundCommentStart,len+2,"");
           flag = false;

       } else if(foundCommentEnd > foundCommentStart && foundCommentEnd == string::npos)
       {
           len = line_str.length() -  foundCommentStart;
           line_str.replace(foundCommentStart,len,"");
           flag = true;
       }
       else if(foundCommentStart > foundCommentEnd)
       {
           len = line_str.length() -  foundCommentStart;
           line_str.replace(foundCommentStart,len,"");

           len = foundCommentEnd;
           line_str.replace(0,len+2,"");
           flag = true;
       }
   }
   else
   {
       int foundCommentEnd = line_str.find(CommentEnd);
       if(foundCommentEnd >= 0 && foundCommentEnd != string::npos)
       {
           len = foundCommentEnd;
           line_str.replace(0,len+2,"");
           flag = false;
       }
       else if(flag == true)
       {
           len = line_str.length();
           line_str.replace(0,len,"");
       }
       else
       {
           return;
       }
   }

}
/******************************************** Lexer **********************************************/
/* Lexical analysis of macros in C */
/*************************************************************************************************/

void lex(const char* file_dir ,Tokens *Tk, int *no_of_tokens,CompilerData *cd,int *no_of_compilerdata,InputData *inp_data)
{
    /* MAP_MACRO_TYPE */
    string fl = string(file_dir);
    int count_lines=0,comp_lines = 0,no_of_defines = 0;
    count_lines=file_read(fl,&inp_data);
    string TERM_LINE = "LINE_SEPERATOR",prev_macro_str = "";
    string line_str,tmpline,macro_str; 
    int first_char = 0,cnt_tok = 0;
#ifdef DEBUG    
    cout << "inp_data" << inp_data[0].line_str<<"count lines"<< count_lines<<endl;
#endif    
    for(int line=0;inp_data != NULL && line < count_lines; line++)
    {
        //   line = *it; /* input_data::line_str */
        line_str = inp_data[line].line_str;
        
        removeComments(line_str);

        
        first_char = is_space(0,line_str);
        if(first_char == line_str.length())
        {
            continue;
        }
        if(line_str.at(first_char) == '#')
        {
            tmpline = line_str;
            line_str = line_str.substr(first_char + 1);

#ifdef DEBUG
             cout  <<"line_str is:"<< line_str << endl;
#endif
            // stringstream class check1 
            stringstream strstream(line_str); 
            string tok_str; 

            if(getline(strstream, tok_str, ' '))
            {
                macro_str = tok_str;
                //  cout  <<"tok_str is:"<< tok_str << endl;
#ifdef DEBUG
             cout  << first_char + macro_str.length() - 1 << " line no."<<tmpline << endl;
#endif
                //  int tmp=is_space(first_char + macro_str.length() - 1,tmpline);
                if(macro_str == "define")   /* Multiline macro */
                {
                    no_of_defines++; 
                    Tk[cnt_tok++].InsertTokens(line,macro_str,true);
                    if(getline(strstream, tok_str, ' ')) // Need to add support for '\t' or convert all tabs to spaces
                    {

                        if(is_id(tok_str,"(",1)) 
                        {
                            macro_str = tok_str;
                            stringstream strstream2(tok_str);
                            if(getline(strstream2, tok_str, '('))
                            {
                                //  cout << tok_str << endl;
                                macro_str = macro_str.substr(tok_str.length()+1);
                                //   cout << macro_str << endl;
                                RemoveTokens(&Tk[cnt_tok]);
                                cnt_tok--; // to overwrite the existing token
                                Parse_Argument(strstream2,macro_str,line);
                                //    cout << tmpline <<endl;
                                Parse_Body(strstream,tmpline,line,inp_data,count_lines);
                                continue;
                            }
                        }
                        else if(is_id(tok_str," ",0))
                        {

                            Tk[cnt_tok++].InsertTokens(line,tok_str,false);
                        }
                        else
                        {
                            cout << "error: #define wrongly used:" << tok_str <<"line num:"<< line << endl;                           
                        }

                    }
                }

                else  if(     macro_str == "include" )
                {   
                    Tk[cnt_tok++].InsertTokens(line,macro_str,true);
#ifdef DEBUG                    
                    cout << "Inside include "<<endl;
#endif                    
                    if(getline(strstream, tok_str, ' '))
                    {
                        if(is_enclosed_bracket_str(tok_str,'<','>') == 0 || is_enclosed_bracket_str(tok_str,'\"','\"') == 0)
                        {
                            // tok_str = tok_str.substr(1,tok_str.length() - 1);
                            Tk[cnt_tok++].InsertTokens(line,tok_str,false);
                        }
                        else 
                        {
                            cout << "error: header file name is missing !" << endl;
                            exit(0);
                        }

                    }

                }
                else  if(     macro_str == "line")
                {

                    Tk[cnt_tok++].InsertTokens(line,macro_str,true);
                    if(getline(strstream, tok_str, ' '))
                    {
                        if(is_num(tok_str) == true)
                        {
                            Tk[cnt_tok++].InsertTokens(line,tok_str,false);
                            if(getline(strstream, tok_str, ' '))
                            {

                                if(is_enclosed_bracket_str(tok_str,'\"','\"') == 0)
                                {

                                    tok_str = tok_str.substr(1,tok_str.length() - 1);
                                    Tk[cnt_tok++].InsertTokens(line,tok_str,false);
                                }
                            }

                        } 

                    }
                }
                else if(  
                        macro_str == "error" ||
                        macro_str == "pragma")
                {

                    Tk[cnt_tok++].InsertTokens(line,macro_str,true);
#ifdef DEBUG                    
                    cout <<"Pragma leg:"<< macro_str <<endl;
#endif                    
                    if(getline(strstream, tok_str, ' '))
                    {

                        if(is_enclosed_bracket_str(tok_str,'\"','\"') == 0)
                        {

                            //tok_str = tok_str.substr(0,tok_str.length() - 1);
                            cout << tok_str<< endl; 
                            Tk[cnt_tok++].InsertTokens(line,tok_str,false);

                        }
                    }

                }
                else if(  
                        macro_str == "undef" ||
                        macro_str == "defined" ||
                        macro_str == "if"  ||
                        macro_str == "ifdef" ||
                        macro_str == "elif" ||
                        macro_str == "ifndef")
                {
                    prev_macro_str = macro_str;
                    Tk[cnt_tok++].InsertTokens(line,macro_str,true);
                    if(getline(strstream, tok_str, ' '))
                    {

                        Tk[cnt_tok++].InsertTokens(line,tok_str,false);

                    }
                }
                else if( 
                        macro_str == "else" ||
                        macro_str == "endif" ) 
                {

                    Tk[cnt_tok++].InsertTokens(line,macro_str,true);
                }
                /*           if( is_macro_operator(prev_macro_str,macro_str))
                             {

                             } */
                else if(macro_str.length() != 0)
                {
                    cout <<line<<" "<<count_lines<< " error: invalid preprocessing directive "<< macro_str<<" " <<file_dir << endl;
                    exit(0);   
                }      
            } 
            else
            {

                cout << "error: "<< macro_str << endl;
                exit(0);
            }     
            // Tokenizing w.r.t. space ' ' 
            while(getline(strstream, tok_str, ' '))
            {
                Tk[cnt_tok++].InsertTokens(line,tok_str,false);
            }
            Tk[cnt_tok++].InsertTokens(line,TERM_LINE,false);

        }
        else 
        {
            cout <<"CompilerData:"<<line_str<<endl;
            cd[comp_lines++].InsertData(line_str,line);
        }
    }
    *no_of_tokens = cnt_tok;
    *no_of_compilerdata = comp_lines;
#if DEBUG
    for(int j=0; j<cnt_tok; j++)
    {
        Tk[j].viewTokens();
    }
    cout << "Lexer complete execution !" <<endl;
#endif
}
/******************* Parser ***************************/
class Define_storage
{
    public:
        //   size_t sz;
        bool use_bit;
        string define_tok_name;
        string define_rep_str;
        int line_num;
        Define_storage(): use_bit(true)
        {}
        ~Define_storage() {}
        inline void save_define_op(string tok_name,string rep,int line)
        {
            this->define_tok_name = tok_name;
            this->define_rep_str = rep;
            this->line_num = line;
        }
};
#if 0
class Filedata
{
    string line;
};
class Include_storage
{
    string file_name;
    int line_no_insert;
    Filedata &fptr;
};
#endif
inline bool check_define_op(Define_storage *D, string tok_name)
{
    bool ret = false;
    for(;D != NULL; D++)
    {
        if(D->define_tok_name == tok_name)
            return true;
    }
    return ret;
}

/************************** Do_undefine_op ****************************/
/*             Disables a single macro to be substituted              */
/**********************************************************************/

bool Do_undefine_op(Define_storage *D,int *storage_lines,int lineno,string tok_name)
{
    for(int i=0; i< *storage_lines; i++)
    {
        if(D[i].define_tok_name == tok_name)
        {
            D[i].use_bit = false;
        }

    }
}


#if 0
void inline fetch_str_fromfile_singleline(string &filename,string &content)
{
    ifstream fptr(filename.c_str());
    string fetch_str;
    while(fptr >> fetch_str)
    {
        content += fetch_str;
    }

    return;
}
#endif
inline string custom_hashinclude(string file)
{
    int len = file.length();
    string cwd ,full_path;
    if(file.at(len - 1) != 'h' || file.at(len - 2) != '.')
    {
        cout << "error: "<<file<<": No such file or directory"<<endl;
        exit(0);
    }
    if(getcwd((char*)cwd.c_str(), (size_t)cwd.length()) != NULL)
    {
        full_path = cwd;
        full_path += "\\";    
        full_path += file;
        //   fetch_str_fromfile_singleline(full_path,content);
        return full_path;
    }
    else 
    {
        cout << "getcwd() error" <<endl;
    }
    return "";
}
int search_file(const char *file_name)
{ 
    //  printf("Filename is %d\n", access(file_name,F_OK)); 
    if(access(file_name,F_OK) == 0)
    {
        return 1;
    }
    else
    {
        return -1;
    }

    return 0;
}
void system_hashinclude (string &file,string &path)
{
    int i,k=0,g,incl_dir_size=3;


#if defined(_WIN32) || defined(_WIN64)
    char slash[2] = "\\";
    char incl_dir[3][128]= { "C:\\MinGW\\include",
        "C:\\MinGW\\lib\\gcc\\mingw32\\4.8.1\\include",
        "C:\\MinGW\\mingw32\\lib\\gcc\\mingw32\\4.8.1\\include",
    };
#elif __linux 
    char slash[2] = "/";
    char incl_dir[3][128]= { "/usr/local/include",
        "/usr/target/include",
        "/usr/include" };
#endif
    char temp[128];
    for(i=0; i < incl_dir_size; i++) 
    {
        memset(temp,0,sizeof(temp));
        strcpy(temp,incl_dir[i]);
        k+=sizeof(incl_dir[i]);
        strcat(temp,slash);
        k+=sizeof(slash);
        strcat(temp,file.c_str());
        k+=sizeof(file.c_str());
        temp[k]='\0';
        if(search_file(temp) == 1 ) //search file at system level directory
        {
            cout<< "System directory found " << temp <<"!!" <<endl;
            path = string(temp);      
            // fetch_str_fromfile_singleline(file,content);
            // break;

        }
    }
}
/************************** Do_define_op ****************************/
/*             Replace all macros by macro substituion              */
/********************************************************************/
inline void Do_define_op(Define_storage *D,int storage_line,CompilerData *cd, int compiler_data)
{
    size_t found,len;
    string temp,temp2;
    for(int z=0; z<storage_line ; z++)
    {
        for(int k= z+1; k<storage_line; k++)
        {
            if(D[z].use_bit == false)
                continue;

            if(D[k].define_rep_str == D[z].define_tok_name)
            {
                D[k].define_rep_str = D[z].define_rep_str;
            }
            else 
            {
                int found = D[k].define_rep_str.find(D[z].define_tok_name);
                if(found != string::npos)
                {
                    D[k].define_rep_str.replace(found,D[z].define_tok_name.length(),D[z].define_tok_name);

                }

            }

        }

    }

    for(int j=0;j< storage_line ; j++) 
    {
        for(int i = 0; i < compiler_data;i++ ) 
        {
            if(D[j].use_bit == false)
                continue;

            found=cd[i].compilation_line.find(D[j].define_tok_name);
            if(found !=0 && found != string::npos)
            {
                len = D[j].define_tok_name.length();
#ifdef DEBUG                
                cout << "Do define op:" << found<<" "<<D[j].define_tok_name<< " "<< D[j].define_rep_str<<endl; 
#endif               
                temp = D[j].define_rep_str;
                temp2 = cd[i].compilation_line; 
                temp2.replace(found,len,"");
                temp2.insert(found,temp);
                cd[i].compilation_line = temp2;
                //  flag = true;

            }
            /*     if(flag == true && *it == D->define_rep_str)
                   {
             *it = temp;

             } */

        }
    }

}
/************************** define_op *****************************/
/*    Replace a single macro by macro subtitution mechanism       */
void define_op(Define_storage *D,int storage_line,CompilerData *cd, string def_str, int line)
{
    bool found = false;
    int i;
    for(i = 0; i< storage_line; i++)
    {
        if(D[i].define_tok_name.compare(def_str)== 0)
        {
            found = true; 
            break;
        }
    }
    if(found)
    {  
        int tmp_line;
        size_t len;
        int j;
        for(j=0; (tmp_line=cd[j].line_number) < line; j++)
            ;
        for(int k=0; k< storage_line; k++)
        {
            if(D[k].define_rep_str == def_str)
            {
                D[k].define_rep_str = D[i].define_rep_str;
            }
        }
        for(int k=0; k<j; k++)
        {
            int find=cd[k].compilation_line.find(def_str);
            if(find !=0 && find != string::npos)
            {

                len = def_str.length();
                string temp = D[i].define_rep_str;
                string temp2 = cd[k].compilation_line; 
                temp2.replace(find,len,"");
                temp2.insert(find,temp);
                cd[k].compilation_line = temp2;

            }
        }
    }
}
/********** Standard Predefined Macros **********/
class Func
{
    string function_name;
    int line_numb;
    bool function_parse(string line_str,int line)
    {
        bool flag = false;
        int i=0;
        string return_type,funct_name;
        while(i< line_str.length() && (line_str.at(i) != ' ' || line_str.at(i) != '\t' || line_str.at(i) != '\0'))
        {
            return_type += line_str.at(i);
            i++;
        }
        if(line_str.at(i) == '\0')
            return false;
        if(return_type == "int" || return_type == "void" || return_type == "float" || return_type == "unsigned" || return_type == "double" || return_type == "long")
            flag = true;

        if(flag == true)
        {

            while(i< line_str.length() && (line_str.at(i) != '('))
            {    
                funct_name += line_str.at(i);
                i++;
            }   
            if(line_str.at(i) = '\0')
                flag = false;

            if(line_str.at(i) = '(')
            { 
                function_name = funct_name;
                line_numb = line;
                flag = true;

            }

        }

    }
};
enum PredefinedType {
    t_file,
    t_line,
    t_date,
    t_time,
    t_stdc,
    t_cpp,
    t_func
};
/********************** Relace Predefined Macros *********************/
/*    This module is used to replace the standard predefined macros  */
/*********************************************************************/
void ReplacePredefinedMacros(CompilerData *comp,int no_of_compilerdata,string file)
{
    string tmp;
    bool replace = false;
    char buff[32];
    string arr[7] = { "__FILE__","__LINE__","__DATE__","__TIME__","__STDC__","__cplusplus","__func__"};

    time_t t=time(NULL); 
    struct tm *my_time = gmtime(&t);
    for(int j =0; j< no_of_compilerdata; j++)
    {
        string s = comp[j].compilation_line;
        int line = comp[j].line_number;
        for(int i=0; i< 7; i++)
        {
            tmp = arr[i];
            int found = s.find(tmp);
            //  cout <<tmp <<" "<< s << endl;
            if(found >= 0 && found < string::npos)
            {
                switch((PredefinedType) i)
                {
                    case t_file:
                        file.replace(file.length()-1,1,1,'c');
                        strcpy(buff,file.c_str());
                        break;

                    case t_line:
                        //   itoa(line,buff,10);
                        sprintf (buff, "%d", line+1);
                        break;

                    case t_date:

                        if(strftime(buff, sizeof buff, "%b %d %Y", my_time))
                        {
                            replace = true;
                        }

                        break;

                    case t_time:
                        if(strftime(buff, sizeof buff, "%H:%M:%S", my_time))
                        {
                            replace = true;
                        }
                        break;

                    case t_stdc:
                        buff[0] = '1';
                        buff[1] = '\0';
                        break;

                    case t_cpp:
                        buff[0] = '1';
                        buff[1] = '9';
                        buff[2] = '9';
                        buff[3] = '7';
                        buff[4] = '1';
                        buff[5] = '1';
                        buff[6] = 'L';
                        buff[7] = '\0';
                        break;

                    case t_func:
                        //strcpy(buff,function_name.c_str()); 
                        break;
                }

                string str = string(buff);   
                int len = tmp.length();
                s.replace(found,len,"");
                s.insert(found,str);
                comp[j].compilation_line = s;
            }
        }
    }

    return;
}
void Do_include_op(string file,string &path)
{
    string tok_str;
    if(is_enclosed_bracket_str(file,'<','>') == 0)
    {
        system_hashinclude(file,path);

    }
    else if(is_enclosed_bracket_str(file,'\"','\"') == 0)
    {
        custom_hashinclude(file);
    }

    //   Tokens *curr_t;
    //  lex(file,curr_t);   // should be parser
    //  file_lines=append_file(curr_t,prev_t);


}
class File
{
    string file_name;
    File( string fname)
    {
        file_name = fname;
    }   
};
class Line
{
    int orig_line;
    int line_to_disp;
    string file_name;
    void save_line_op(int line,Line &l,string file,File &f)
    {
        line_to_disp = line; 
        file_name = file;
    }
};
/************* MemManager ******************/
/*  Memory modules for memory management   */
/*******************************************/

#define MEM_INIT()     \
CompilerData *cd = new CompilerData[10000];  \
InputData *inp_data;   \
Tokens *Tk = new Tokens[10000];    \
Stack include_files;      \

#define MEM_DEINIT()     \
    delete[] cd;         \
    delete[] Tk;         \
   

/***************** Parsing Module ************************/
/*This module is used to parse the preprocessor directives*/
/*Lex is called before parsing for getting the tokens    */
/*********************************************************/
void Parser(string file,CompilerData *comp,InputData *ip, Tokens *t,Stack incl,string par_file)
{
    static int define_counter = 0;         
    int no_of_tokens =0,no_of_compilerdata = 0,no_of_linelist=0; 
    //Define_storage *Def = new Define_storage[no_of_defines];
    bool prev_check_define[256] = { false };
    int line_list[1000];
    bool exe;
    static int if_counter = 0;

    Define_storage *D = new Define_storage[10000];

    lex(file.c_str(),t,&no_of_tokens,comp,&no_of_compilerdata,ip);

    for(int i=0;i < no_of_compilerdata; i++)  /* Hack for Removing multiline macro */
    {
        int flag = -1;
        removeMultiline(comp,i, no_of_compilerdata,flag);
        if(flag != -1)  /* Body Access check */
        {
           int line_no = comp[i].line_number;
           for(int j=0; j<no_of_tokens; j++)  /* Appending compilation line(Body) to token, now body of multiline macro will be pushed as token */
           {
               if(t[j].line_no == line_no - 1)
               {
                   t[j+1].tokens += " "+comp[i].compilation_line;
                   cout << "Insert Multiline  into Tokens" <<t[j+1].tokens;
                   comp[i].compilation_line = "";                
                   break;
               }
           }

        }
    }

    if(no_of_tokens == 0)
    {
        return;
    }   

    for(int i=0; i< no_of_tokens; i++)
    {
        if( t[i].is_macro_keyword == 1)
        {
            // cout << "Parser:"  <<endl;
            MacroType Idx=getIndexFromStr(t[i].tokens);
            int line_no = t[i].line_no;
            switch(Idx)
            { 
                case  define_t:
                    {
                     //   cout << "Save define op:" << t[i+1].tokens<< " " << t[i+2].tokens<< endl;
                        D[define_counter].use_bit = true; 
                        D[define_counter].save_define_op(t[i+1].tokens,t[i+2].tokens,line_no);
                        define_counter++;
                        continue; 
                    }
                case  include_t:
                    {  /*  I.file_name = file;
                           I.line_no_insert = line_no; */
                        //Do_include_op(); TO DO
                        string include = t[i+1].tokens,full_path;
                        include = include.substr(1,include.length() - 2);
                 //       cout << "Include file is: " << include <<endl;
                        incl.push(include);

                        MEM_INIT();
                        string path;
                        Do_include_op(include,path);
                        Parser(path,cd,inp_data, Tk,include_files,par_file);
                        //dump_file(file,path,line_no);
                        //  include_files_lines.push(ln);
                        /* Append filemechanism using compiler data and call parser again */  
                        MEM_DEINIT();
                        continue;
                    }
#if 0
                case  line_t:
                    {
                        if(Tk[i+1] == NULL || Tk[i+2] == NULL)
                        {
                            cout << "Furthur Tokens are NULL" << endl;
                            continue;
                        }
                        string line_parse = Tk[i+1].tokens;
                        string fname_parse = Tk[i+2].tokens;
                        Do_line_op(line_parse,fname_parse);       
                        continue; 
                    }
#endif                    
                case  undef_t:
                    {  
                        string macro_str = t[i+1].tokens;
                        //cout <<file<<" "<<line_no<<" "<< t[i].tokens << " " << t[i+1].tokens << " " <<D[0].define_tok_name << endl;
                        define_op(D,define_counter,comp,macro_str,line_no); 
                        Do_undefine_op(D,&define_counter,line_no,macro_str);
                        continue; 
                    }   
#if 0 
                case  error_tt:

                    continue;       
                case  pragma_t:

                    continue;       
                case  defined_t:

                    continue;       
                case  if_t:
                    {   if_counter++;
                        string flag=Tk[i].tokens;
                        prev_check_define[if_counter] = check_define_op(flag);

                        if(prev_check_define[if_counter] == true)
                            ;
                        else
                        { 
                            markBodyStart(start,line);
                            continue;
                        }
                        continue;       
                    }
                case  ifdef_t:
                    if_counter++;
                    prev_check_define[if_counter] = check_define_op();

                    if(prev_check_define[if_counter] == true)
                        ;
                    else
                    {
                        markBodyStart(start,line);
                        continue;
                    }
                    continue;       
                case  ifndef_t:
                    if_counter++;
                    prev_check_define[if_counter] = check_define_op();
                    continue;       
                case  elif_t:
                    {   string flag=Tk[i].tokens;
                        check_flag = check_define_op(flag);
                        if(if_counter > 0 && check_flag == true)
                        {
                            if(prev_check_define[if_counter] == false)
                            {
                                markBodyEnd();
                                removeBody();
                                exe = true; 
                            } else
                            {
                                continue;
                            }
                        }
                        else if(if_counter > 0 && check_flag == false)
                        {
                            markBodyStart();
                        }
                        else if(if_counter <= 0)
                        {
                            cout << "error: #if missing !!" << endl;
                            exit(0);
                        }
                        continue;  
                    }     
                case  else_t:
                    if(if_counter > 0 && (check_flag == false || prev_check_define[if_counter] == false))
                    {
                        markBodyEnd();
                        removeBody(); 
                        exe = true;       
                    }
                    else if(if_counter > 0 && (check_flag == true || prev_check_define[if_counter] == true))
                    {
                        markBodyStart();
                       
                    }
                    else
                    {
                        cout << "error: #if missing !!" << endl;
                        exit(0);
                    }

                    continue;       
                case  endif_t:
                    if_counter--;
                    if(if_counter != 0)
                    {
                        cout << "error: #if missing !!"<< endl;
                        exit(0);
                    }
                    if(exe == true)
                    {
                       markBodyEnd();
                       removeBody();
                    }
                    
                    continue;
#endif
                default:
                    if(exe == false)
                    {
                        //remove_lines(start_line+1, endline-1); // TO DO 
                    }
                    continue;
            }
        }
    }

    Do_define_op(D,define_counter,comp,no_of_compilerdata);
    ReplacePredefinedMacros(comp,no_of_compilerdata,par_file);
    dump_file(file,comp,no_of_compilerdata,par_file);

  /*  delete[] comp;
    delete[] t; */
}
#if UTX
int main()  /* g++ -g -fno-stack-protector prep3_1.cpp datastructure.cpp */
{
    system("rm -rf p4.i");
    string str = "p4.c";

    MEM_INIT();

    Parser(str,cd,inp_data, Tk,include_files,str);

    MEM_DEINIT();


    return 0; 
}
#elif FWK

#else 
int main(int argc,char *argv[])
{   
    int i;
    char file_dir[64];
    char file_list[NO_OF_MAX_FILES][64];
    int file_len;

    if(argc < 2)
    {
        printf("%s: error: no input files\n", argv[0]);
        exit(0);
    }  
    else if(argc== 2)
    {
        memset(file_dir,0,sizeof(file_dir));
        strcpy(file_dir,argv[1]);
        file_len=strlen(argv[1])-1;
        if(argv[1][file_len-1]== '.' && (argv[1][file_len]=='c' || argv[1][file_len]=='h'))
        { 
         //   lex(file_dir);
             MEM_INIT();

             string file_name = string(file_dir);
             Parser(file_name,cd,inp_data, Tk,include_files,file_name);

             MEM_DEINIT();
        }
        else
        {
            printf("%s:%d:error : %s is not a C file format\n", __FILE__, 1 , argv[1]);
            exit(0);
        } 

    }
    else if(argc <= NO_OF_MAX_FILES)
    {
        file_len=strlen(argv[1])-1;
        if(argv[1][file_len-1]== '.' && (argv[1][file_len]=='c' || argv[1][file_len]=='h'))
        { 

            for(i=0;i<argc;i++)
            {
                memset(file_list[i],0,sizeof(file_list));
                strcpy(file_list[i],argv[i]);

                MEM_INIT();

                string file_name = string(file_list[i]);
                Parser(file_name,cd,inp_data, Tk,include_files,file_name);

                MEM_DEINIT();
            }
        }
        else
        {
            printf("%s:%d:error : %s is not a C file format\n", __FILE__, 1 , argv[1]);
            exit(0);
        } 


    }
    else
    {
        printf("%s:%d: %s: number of input files limit is %d\n", __FILE__, 1 ,argv[0], NO_OF_MAX_FILES);
        exit(0);
    }


    return 0;
}
#endif

