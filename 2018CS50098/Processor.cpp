#include<fstream>
#include<sstream>
#include<math.h>

using namespace std;

int main(){
    ifstream infile;
    infile.open("input.txt");
    ofstream outfile;
    outfile.open("output.txt");
    string str;
    while(getline(infile,str)){
        start://I will start reading the number from here
        istringstream operands;
        operands.str(str);
        string num1,num2;
        operands>>num1>>num2;
        int exp1=stoi(num1.substr(1,8),nullptr,2)-127;   //I got my exponent of the first number.
        int exp2=stoi(num2.substr(1,8),nullptr,2)-127;   //I got my exponent of the second number.
        double sig1,sig2;
        if(exp1==-127){
            sig1=pow(-1,stoi(num1.substr(0,1)))*stoi("0"+num1.substr(9,23),nullptr,2);
        }else{
            sig1=pow(-1,stoi(num1.substr(0,1)))*stoi("1"+num1.substr(9,23),nullptr,2);
        }   //significant for first number is calculated till here with sign.
        if(exp2==-127){
            sig2=pow(-1,stoi(num2.substr(0,1)))*stoi("0"+num2.substr(9,23),nullptr,2);
        }else{
            sig2=pow(-1,stoi(num2.substr(0,1)))*stoi("1"+num2.substr(9,23),nullptr,2);
        }   //significant for second number is calculated till here with sign.
        int cycle=0;
        int exp;
        double sig;
        int temp;
        string sign;
        step1:
        cycle++; //clock cycle count done
        if(exp1==128){
            outfile<<"INVALID INPUT ";
            goto stop;
        }   //Invalid Input detected and stoped by printing message to inform user.
        if(exp2==128){
            outfile<<"INVALID INPUT ";
            goto stop;
        }   //Invalid Input detected and stoped by printing message to inform user.
        if(exp1<exp2){
            sig1=sig1/pow(2,exp2-exp1);
            exp1=exp2;
        }   //shifting done to make exponents equal
        else{
            sig2=sig2/pow(2,exp1-exp2);
            exp2=exp1;
        }   //shifting done to make exponents equal
        step2:
        cycle++;  //clock cycle count done
        sig=sig1+sig2; //Addition of significands done and got sofar significand of result
        exp=exp1;   //sofar exponent of result
        step3:
        cycle++;
        while(abs(sig)>=pow(2,24)){
            sig=sig/2;
            exp++;
        }   //normalizing in the range [2^(23) to 2^(24))
        while((abs(sig)<pow(2,23))&&(sig!=0)){
            sig=sig*2;
            exp--;
        }   //normalizing in the range [2^(23) to 2^(24))
        if(sig==0){
            exp=-127;
        }   //zero is handled separately
        if(exp>=128){
            outfile<<"OVERFLOW ";
            goto stop;
        }   //checking result OVERFLOW
        if((exp<=-127)&&(sig!=0)){
            outfile<<"UNDERFLOW ";
            goto stop;
        }   //checking result UNDERFLOW, Zero is handled separately
        step4:
        cycle++;
        if(sig<0){
            sign="1";  //store sign for negetive
        }else{
            sign="0";  //store sign for non-negetive
        }
        temp=abs(sig)*10;
        if(temp%10>=5){
            temp=temp/10+1;  //significand result
        }else{
            temp=temp/10;    //significand result
        }
        if(((temp<pow(2,24))&&(temp>=pow(2,23)))||(temp==0)){
            outfile<<sign+bitset<8>(exp+127).to_string()+bitset<24>(temp).to_string().substr(1,23)<<" ";
            goto stop; //sign then exponent added with bais then lower 23 bits of significand
        }else{
            sig=pow(-1,stoi(sign))*temp;
            goto step3;
        }   //If not normalized follow again from step 3
        stop:
        outfile<<cycle<<endl;
    }
    infile.close();
    outfile.close();
    return 0;
}
