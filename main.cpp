#define FILE_EXTENSION ".txt"
#define ALPHABET_SIZE 26
#include<fstream>
#include<string>
#include<cstring>
#include<vector>
#include<iostream>
#include<stack>
#include<queue>
#include<ctime>
 
using namespace std;
 
 
#include<filesystem>
namespace fs = std::filesystem;
 
enum Mode{
   EXACT, PREFIX, SUFFIX, AND, OR
};
 
//Trie
struct TrieNode{
   struct TrieNode* children[ALPHABET_SIZE];
 
   //代表word被end
   bool isEndOfWord;
};
 
struct Save{
   TrieNode* node;
   string title;
};
 
TrieNode* getNode(){ //建立新的node
   struct TrieNode* newNode = new TrieNode;
   newNode->isEndOfWord = false;
 
   for(int i=0; i<ALPHABET_SIZE; i++){
       newNode->children[i] = NULL;
   }
 
   return newNode;
}
 
void insert(TrieNode* root, string key){
   struct TrieNode* pCrawl = root;
 
   for(int i=0; i<key.length(); i++){
       char lower_key = tolower(key[i]);
       int index = lower_key - 'a';
       //int index = 1;
       //如果key[i]字元沒有存在Trie中，就新增一個Node代表key[i]字元
       if(pCrawl->children[index] == NULL ) pCrawl->children[index] = getNode();
       //往下爬一層
       pCrawl = pCrawl->children[index];
   }
 
   //標記最後的node為有單字
   pCrawl->isEndOfWord = true;
}
 
void insert_inverse(TrieNode* root, string key){
   struct TrieNode* pCrawl = root;
 
   for(int i=key.length()-1; i>=0; i--){
       char lower_key = tolower(key[i]);
       int index = lower_key - 'a';
       //int index = 1;
       //如果key[i]字元沒有存在Trie中，就新增一個Node代表key[i]字元
       if(pCrawl->children[index] == NULL ) pCrawl->children[index] = getNode();
       //往下爬一層
       pCrawl = pCrawl->children[index];
   }
 
   //標記最後的node為有單字
   pCrawl->isEndOfWord = true;
}
 
bool search(TrieNode* root, string key, Mode mode){
   TrieNode* pCrawl = root;
 
   if(mode == EXACT){
       for(int i=0; i<key.length(); i++){
           int index = key[i] - 'a';
           //如果不存在key[i]這個字元，代表不可能存在key這個單字，所以直接return false
           if(!pCrawl->children[index]) return false;
           //往下爬一層
           pCrawl = pCrawl->children[index];
       }
       //跑到尾巴後判斷是否有單字
       return (pCrawl->isEndOfWord);
   }
   else if(mode == PREFIX){ //ex: graphic
        for(int i=0; i<key.length(); i++){
            int index = key[i] - 'a';
            //如果不存在key[i]這個字元，代表不可能存在key這個單字，所以直接return false
            if(!pCrawl->children[index]) return false;
            //往下爬一層
            pCrawl = pCrawl->children[index];
        }
        //跑到尾巴後判斷是否有單字
        return true;
   }
   else if(mode == SUFFIX){ //ex: photograph
        for(int i=key.length()-1; i>=0; i--){
            int index = key[i] - 'a';
            //如果不存在key[i]這個字元，代表不可能存在key這個單字，所以直接return false
            if(!pCrawl->children[index]) return false;
            //往下爬一層
            pCrawl = pCrawl->children[index];
        }
        //跑到尾巴後判斷是否有單字
        return true;
       //here
   }
   return false;
}
 
//-------------------------------------------------------------------------
 
// Utility Func
 
// string parser : output vector of strings (words) after parsing
vector<string> word_parse(vector<string> tmp_string){
   vector<string> parse_string;
   for(auto& word : tmp_string){
       string new_str;
       for(auto &ch : word){
           if(isalpha(ch)) //判斷是否為字母
               new_str.push_back(ch);
       }
       //NOTE:注意這裡新增了判段不要是空string的部分
        if(!new_str.empty()){
            parse_string.emplace_back(new_str); //類似push_back()
        }
   }
   return parse_string;
}
 
vector<string> split(const string& str, const string& delim) {
   vector<string> res;
   if("" == str) return res;
   //先將要切割的字串從string型別轉換為char*型別
   char * strs = new char[str.length() + 1] ; //不要忘了
   strcpy(strs, str.c_str());
 
   char * d = new char[delim.length() + 1];
   strcpy(d, delim.c_str());
 
   char *p = strtok(strs, d);
   while(p) {
       string s = p; //分割得到的字串轉換為string型別
       res.push_back(s); //存入結果陣列
       p = strtok(NULL, d);
   }
 
   return res;
}
 
Save build_essay(string path, int mode){
    if(mode == 0){
        Save record;
        string file, title_name, tmp;
        fstream fi;
        vector<string> tmp_string;
       
        TrieNode* root = getNode();
       
        fi.open(path, ios::in); //開啟檔案
       
        getline(fi, tmp);
        record.title = tmp;
        tmp_string = split(tmp, " ");
       
        vector<string> content = word_parse(tmp_string);
       
        for(auto &word : content){//輸出內容
            //cout << word << " ";
            insert(root, word);
        }
       
        while(getline(fi, tmp)){ //讀取剩下的內容，一行一行讀
            tmp_string = split(tmp, " ");
       
            vector<string> content = word_parse(tmp_string);
       
            for(auto &word : content){//輸出內容
                //cout << word << " ";
                insert(root, word);
            }
       
        }
       
        // CLOSE FILE
        fi.close();
        //cout << "build a Trie\n";
        record.node = root;
        return record;
    }
    else if(mode == 1){
        Save record;
        string file, title_name, tmp;
        fstream fi;
        vector<string> tmp_string;
       
        TrieNode* root = getNode();
       
        fi.open(path, ios::in); //開啟檔案
       
        getline(fi, tmp);
        record.title = tmp;
        tmp_string = split(tmp, " ");
       
        vector<string> content = word_parse(tmp_string);
       
        for(auto &word : content){//輸出內容
            //cout << word << " ";
            insert_inverse(root, word);
        }
       
        while(getline(fi, tmp)){ //讀取剩下的內容，一行一行讀
            tmp_string = split(tmp, " ");
       
            vector<string> content = word_parse(tmp_string);
       
            for(auto &word : content){//輸出內容
                //cout << word << " ";
                insert_inverse(root, word);
            }
       
        }
       
        // CLOSE FILE
        fi.close();
        //cout << "build a Trie\n";
        record.node = root;
        return record;
    }
   
}
 
int main(int argc, char *argv[])
{
    int a = clock();
   vector<Save> Trie;
   vector<Save> InversedTrie;
   vector<string> key;
 
   string data_dir = argv[1] + string("/");
   string query = string(argv[2]);
   string output = string(argv[3]);
 
    //對每個essay建立trie
 
 
   string path = data_dir;
   int index = 0;
   for(auto &entry : fs::directory_iterator(path)){
        //單純計算有幾筆檔案要讀
        index++;
   }
   cout << "index = " << index << "\n";
 
    for(int i=0; i<index; i++){
        Trie.push_back(build_essay(path + to_string(i) + ".txt", 0));
        InversedTrie.push_back(build_essay(path + to_string(i) + ".txt", 1));
    }
   //先去開啟query.txt
   fstream query_handler;
   string tmp;
   //vector<string> tmp_string;
   vector<string> essay_output;
   //vector<string> content;
   query_handler.open(query, ios::in);
   // GET CONTENT LINE BY LINE
 
    while(getline(query_handler, tmp)){ //讀取剩下的內容，每次讀取一整行
       
        vector<string> tmp_string = split(tmp, " "); //切好的字串，包括特殊符號和AND OR
        vector<string> content = word_parse(tmp_string); //只剩關鍵字的部分

 
        vector<Mode> op;
       //先記錄要怎麼運算關鍵字
        for(auto &word: tmp_string){
            Mode mode;
            //cout << word << " " ;
            if(word[0] == '"') mode = EXACT;
            else if(word[0] == '*') mode = SUFFIX;
            else if(word[0] == '+') mode = AND;
            else if(word[0] == '/') mode = OR;
            else mode = PREFIX;
           
            op.push_back(mode);
        }

        //計算
        int hasTitle = 0;
        for(int i=0; i<index; i++){ //跑每個Trie
            stack<bool> Desicions;
            int id_count = 0;
            int needTo = 0;
            for(auto mode: op){
               
                if(mode == AND){
                    needTo = 1; //代表下次要AND
                }
                else if(mode == OR){
                    needTo = 2; //代表下次要OR
                }
                else if(mode == SUFFIX){
                   
                    if(needTo == 1){ //要做AND
                        bool last = Desicions.top();
                        Desicions.pop();
                        if(last == false) Desicions.push( false );
                        else{
                            bool ans = search(InversedTrie[i].node, content[id_count], mode); //SUFFIX是inverseTrie
                            Desicions.push( (ans && last) );
                           
                        }
                        needTo = 0;
                    }
                    else if(needTo == 2){ //要做OR
                        bool last = Desicions.top();
                        Desicions.pop();
 
                        if(last == true) Desicions.push( true );
                        else{
                            bool ans = search(InversedTrie[i].node, content[id_count], mode); //SUFFIX是inverseTrie
                            Desicions.push( (ans || last) );
                        }
                       
                        needTo = 0;
                    }
                    else{
                        bool ans = search(InversedTrie[i].node, content[id_count], mode); //SUFFIX是inverseTrie
                        Desicions.push(ans); //普通版本
                    }
                    id_count++;
                }
                else if(mode == PREFIX){
                    if(needTo == 1){ //要做AND
                        bool last = Desicions.top();
                        Desicions.pop();
                        if(last == false) Desicions.push( false );
                        else{
                            bool ans = search(Trie[i].node, content[id_count], mode);
                            Desicions.push( (ans && last) );
                           
                        }
                        needTo = 0;
                    }
                    else if(needTo == 2){ //要做OR
                        bool last = Desicions.top();
                        Desicions.pop();
 
                        if(last == true) Desicions.push( true );
                        else{
                            bool ans = search(Trie[i].node, content[id_count], mode);
                            Desicions.push( (ans || last) );
                        }
                       
                        needTo = 0;
                    }
                    else{
                        bool ans = search(Trie[i].node, content[id_count], mode);
                        Desicions.push(ans); //普通版本
                    }
                    id_count++;
                }
                else if(mode == EXACT){
                   if(needTo == 1){ //要做AND
                        bool last = Desicions.top();
                        Desicions.pop();
                        if(last == false) Desicions.push( false );
                        else{
                            bool ans = search(Trie[i].node, content[id_count], mode);
                            Desicions.push( (ans && last) );
                           
                        }
                        needTo = 0;
                    }
                    else if(needTo == 2){ //要做OR
                        bool last = Desicions.top();
                        Desicions.pop();
 
                        if(last == true) Desicions.push( true );
                        else{
                            bool ans = search(Trie[i].node, content[id_count], mode);
                            Desicions.push( (ans || last) );
                        }
                       
                        needTo = 0;
                    }
                    else{
                        bool ans = search(Trie[i].node, content[id_count], mode);
                        Desicions.push(ans); //普通版本
                    }
                    id_count++;
                }
            }
            //到這裡跑完所有關鍵字的判斷，Desicions中應該會剩下一個bool
            //判斷這個bool來決定這篇essay是否符合搜尋規則
            if(Desicions.top() == true){
                essay_output.push_back(Trie[i].title);
                hasTitle = 1;
            }
        }  
       
        if(hasTitle == 0) essay_output.push_back("Not Found!");
       
    }    
    //cout << "\n";
    query_handler.close();
 
    //做到這裡，做到會儲存到""空字串
    //["reflect", "", "graph"]
   
    fstream output_handler;
    output_handler.open(output, ios::out);
   // cout << "\n";
    for(auto title: essay_output){
        output_handler << title << "\n";
        //cout << title << "\n";
    }
    //cout << "\nEND\n";
    output_handler.close();
 
    int b = clock();
    cout << b-a << " ms\n";
    return 0;
}
 
 
// 1. UPPERCASE CHARACTER & LOWERCASE CHARACTER ARE SEEN AS SAME.
// 2. FOR SPECIAL CHARACTER OR DIGITS IN CONTENT OR TITLE -> PLEASE JUST IGNORE, YOU WONT NEED TO CONSIDER IT.
//    EG : "AB?AB" WILL BE SEEN AS "ABAB", "I AM SO SURPRISE!" WILL BE SEEN AS WORD ARRAY AS ["I", "AM", "SO", "SURPRISE"].
// 3. THE OPERATOR IN "QUERY.TXT" IS LEFT ASSOCIATIVE
//    EG : A + B / C == (A + B) / C
 
//
 
//////////////////////////////////////////////////////////
 
/*
流程：
一、先用split()和word_parse()來吃進整個essay data
二、把每篇essay data建立一個Trie
三、讀取關鍵字，並處理關鍵字
四、把關鍵字丟進Trie裡比對
五、輸出篇名
 
 
做到：
一、處理SUFFIX（在search()）
   要Trie兩下
二、處理AND OR 模式（在main）
 
*/
 
