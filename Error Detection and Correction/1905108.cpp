#include<bits/stdc++.h>
#include<windows.h>

using namespace std;

#define GREEN 10
#define CYAN 11
#define RED 12
#define WHITE 7

string checksumGenerator(string msg, string gen){
    string cs="";
    for(int i=1; i<gen.length(); i++){
        msg+="0";
    }
    //cout<<msg<<endl;

    for(int i=0; i<=(msg.length()-gen.length()); i++){
        if(msg[i]=='0')
            continue;
        else{
            for(int j=0; j<gen.length(); j++){
                if(msg[i+j]==gen[j]){
                    msg[i+j]='0';
                }
                else{
                    msg[i+j]='1';
                }
            }
        }
        //cout<<msg<<endl;
    }

    for(int i=(msg.length()-gen.length()+1); i<msg.length(); i++){
        cs+=msg[i];
    }

    //cout<<"checksum: "<<cs<<endl;

    return cs;
}

int main(){
    int m=0;
    int row=0;
    double probability=0;
    bool errflag=false;
    mt19937 ran(time(NULL));
    vector<string> dblock; //data blocks
    vector<string> cblock; //checked bit + data block
    vector<string> eblock; //after err
    vector<string> fblock; //final
    string s,gen;
    string d;
    string checksum;


    //input
    cout<<"enter data string: ";
    getline(cin,s);
    cout<<"enter number of data bytes in a row (m): ";
    cin>>m;
    cout<<"enter probability (p): ";
    cin>>probability;
    cout<<"enter generator polynomial: ";
    cin>>gen;

    //padding
    int padding=s.length()%m;
    if(padding)
        padding=m-padding;
    //cout<<"padding "<<padding<<endl;

    for(int i=0; i<padding; i++){
        s+="~";
    }

    cout<<"\ndata string after padding: "<<s<<endl;

    //printing ascii(debug)
    //for(int i=0; i<s.length(); i++){
    //    cout<<bitset<8>((int)s[i])<<endl;
    //}

    //data block
    row=s.length()/m;
    int k=0;
    for(int i=0; i<row; i++){
        d="";
        for(int j=0; j<m; j++){
            d+=bitset<8>((int)s[k]).to_string();
            k++;
        }
        dblock.push_back(d);
    }

    cout<<"\ndata block (ascii code of m charecters per row):\n";
    for(int i=0; i<dblock.size(); i++){
        cout<<dblock[i]<<endl;
    }

    //check bits
    int msg=dblock[0].length();
    int r=0;

    while((msg+r+1)>(pow(2,r))){
        r++;
    }

    int n=1;
    for(int i=0; i<row; i++){
        d="";
        k=0;
        n=1;
        for(int j=0; j<(msg+r); j++){
            if(n == j+1){
                d+="0";
                n=n*2;
            }
            else{
                d+=dblock[i][k];
                k++;
            }
        }
        cblock.push_back(d);
    }

    //check bit calculation
    int np,flag;
    int exor=0;

    for(int ii=0; ii<cblock.size(); ii++){
        d=cblock[ii];
        //check bit in a row
        n=1;
        while(n<=msg+r){
            np=n;
            flag=1;
            exor=0;
            for(int i=n; i<=msg+r; i++){
                if(flag){
                    //cout<<i<<" ";
                    exor^=d[i-1]-'0';
                }
                np--;
                if(np==0){
                    np=n;
                    flag=1-flag;
                }
            }
            d[n-1]=exor+'0';
            n=n*2;
            //cout<<endl;
        }
        cblock[ii]=d;
    }

    cout<<"\ndata block after adding check bits:\n";
    for(int i=0; i<cblock.size(); i++){
        n=1;
        for(int j=0; j<cblock[i].length(); j++){
            if(n == j+1){
                n=n*2;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),GREEN);
                cout<<cblock[i][j];
            }
            else{
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),WHITE);
                cout<<cblock[i][j];
            }
        }
        cout<<endl;
    }

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),WHITE);

    //column wise
    string col="", ecol="";
    cout<<"\ndata bits after column-wise serialization:\n";
    for(int j=0; j<(msg+r); j++){
        for(int i=0; i<cblock.size(); i++){
            col+=cblock[i][j];
        }
    }
    cout<<col<<endl;

    ///err detecting part

    //checksum
    checksum=checksumGenerator(col,gen);
    cout<<"\ndata bits after appending CRC checksum (sent frame):\n";
    cout<<col;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),CYAN);
    cout<<checksum<<endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),WHITE);
    col+=checksum;

    //adding err in received frame
    //err in checksum, needed i guess...
    ecol="";
    for(int i=0; i<col.length(); i++){
        if(ran()%100 < probability*100){
            d=(1+'0'-col[i])+'0';
        }
        else{
            d=col[i];
        }
        ecol+=d;
    }

    ///print received frame
    cout<<"\nreceived frame:\n";
    for(int i=0; i<col.length(); i++){
        if(col[i]!=ecol[i]){
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),RED);
            cout<<ecol[i];
        }
        else{
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),WHITE);
            cout<<ecol[i];
        }
    }
    cout<<endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),WHITE);

    //detect err
    checksum=checksumGenerator(ecol,gen);
    for(int i=0; i<checksum.length(); i++){
        if(checksum[i]=='1')
            errflag=true;
    }

    cout<<"\nresult of CRC checksum matching: ";
    if(errflag){
        cout<<"error detected\n";
    }
    else{
        cout<<"no error detected\n";
    }

    //after removing checksum
    cout<<"\ndata block after removing CRC checksum bits:\n";
    for(int i=0; i<row; i++){
        eblock.push_back("");
    }

    int l=0;
    while(l<ecol.length()-gen.length()+1){
        for(int i=0; i<row; i++){
            eblock[i]+=ecol[l];
            l++;
        }
    }

    for(int i=0; i<cblock.size(); i++){
        for(int j=0; j<cblock[i].length(); j++){
            if(eblock[i][j] != cblock[i][j]){
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),RED);
                cout<<eblock[i][j];
            }
            else{
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),WHITE);
                cout<<eblock[i][j];
            }
        }
        cout<<endl;
    }

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),WHITE);


    ///check bit remove er time e err correct korte hobe...
    int crtCount=0;
    for(int ii=0; ii<eblock.size(); ii++){
        d=eblock[ii];
        //err correction in a row
        crtCount=0;
        n=1;
        while(n<=msg+r){
            np=n-1;
            if(np>0)
                flag=1;
            else
                flag=0;
            exor=0;
            //single check bit
            for(int i=n+1; i<=msg+r; i++){
                if(flag){
                    //cout<<i<<" ";
                    exor^=d[i-1]-'0';
                }
                np--;
                if(np<=0){
                    np=n;
                    flag=1-flag;
                }

            }
            if(d[n-1]!=(exor+'0')){
               // cout<<n<<"-"<<exor<<endl;
                crtCount+=n;
            }
            n=n*2;
            //cout<<endl;
        }
        //cout<<crtCount<<endl;
        if(crtCount<=eblock[ii].length() && crtCount>0){
            eblock[ii][crtCount-1]=(1+'0'-eblock[ii][crtCount-1])+'0';
        }
        //cout<<eblock[ii]<<endl;
    }

    //after removing check bits
    cout<<"\ndata block after removing check bits:\n";
    for(int i=0; i<eblock.size(); i++){
        n=1;
        d="";
        for(int j=0; j<eblock[i].length(); j++){
            if(n == j+1){
                n=n*2;
            }
            else{
                d+=eblock[i][j];
            }
        }
        eblock[i]=d;
        //data block
        for(int b=0; b<m; b++){
            fblock.push_back(d.substr(b*8,8));
        }
        cout<<eblock[i]<<endl;
    }

    //output
    d="";
    char ken;
    for(int i=0; i<fblock.size(); i++){
        //cout<<fblock[i]<<endl;
        ken=0;
        for(int j=0; j<fblock[i].length(); j++){
            ken=ken*2;
            ken+=fblock[i][j]-'0';
        }
        d+=ken;
    }

    cout<<"\noutput frame: "<<d<<endl<<endl;

}
